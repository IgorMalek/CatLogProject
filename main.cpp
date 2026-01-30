#define _CRT_SECURE_NO_WARNINGS 
#define NOMINMAX

#include <Windows.h>
#include <iostream>
#include <string>
#include <limits>
#include <ctime>   
#include <sstream> 
#include <iomanip>

#include "LogManager.h"
#include "ApiHandler.h"

using namespace std;

typedef enum {
  FEED_EVENT = 1,
  WEIGHT_EVENT,
  MEDICINE_EVENT,
  REPORT_EVENT,  
  LIST_EVENT,  
  ADD_CAT_EVENT,
  VIEW_PHOTO_EVENT,
  EXIT_EVENT
};

void SetColor(int colorCode)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorCode);
}

string getTodayDate()
{
  time_t t = time(nullptr);
  tm* now = localtime(&t);

  stringstream ss;
  ss << (now->tm_year + 1900) << "-"
    << setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
    << setfill('0') << setw(2) << now->tm_mday;
  return ss.str();
}

string getCurrentTime()
{
  time_t t = time(nullptr);
  tm* now = localtime(&t);

  stringstream ss;
  ss << setfill('0') << setw(2) << now->tm_hour << ":"
    << setfill('0') << setw(2) << now->tm_min << ":"
    << setfill('0') << setw(2) << now->tm_sec;
  return ss.str();
}

void displayMenu()
{
  SetColor(11);
  cout << "\n=== Cat Log ===" << endl;
  cout << "1. Log Feeding (FEED)" << endl;
  cout << "2. Log Weight (WEIGHT)" << endl;
  cout << "3. Log Medicine (MEDS)" << endl;
  cout << "4. Generate Report for Cat" << endl;
  cout << "5. List Registered Cats" << endl;
  cout << "6. Register New Cat" << endl;
  cout << "7. View Cat Photo (Online)" << endl;
  cout << "8. Exit and Save" << endl;
  cout << "---------------------------------" << endl;
  cout << "Select option: ";
  SetColor(7);
}

int getValidIntInput()
{
  int choice;
  cin >> choice;
  if (cin.fail())
  {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return -1;
  }
  return choice;
}

void handleAddCat(LogManager& manager)
{
  string newName, breed, dob, color;

  cout << "Enter name for the new cat: ";
  cin >> newName;

  if (newName.length() > 0)
  {
    cout << "Enter Breed (use_underscores): ";
    cin >> breed;

    cout << "Enter Date of Birth (YYYY-MM-DD): ";
    cin >> dob;

    cout << "Enter Fur Color (use_underscores): ";
    cin >> color;

    // Register with full details
    manager.registerCat(newName, breed, dob, color);
    cout << "Cat '" << newName << "' registered successfully with details." << endl;
  }
  else
  {
    cout << "Invalid name." << endl;
  }
}

Cat* selectCat(LogManager& manager)
{
  SetColor(14);
  manager.listCats();
  string catName;
  cout << "Enter cat name: ";
  SetColor(7);
  cin >> catName;
  Cat* cat = manager.getCat(catName);
  if (!cat) {
    cout << "Error: Cat with this name does not exist." << endl;
    return nullptr;
  }
  return cat;
}

void promptForEventData(string& date, string& time, double& value, string& notes)
{
  cout << "Enter date (YYYY-MM-DD) or 'today': ";
  cin >> date;

  if (date == "today" || date == "Today" || date == "t") 
  {
    date = getTodayDate();
    cout << "-> Using date: " << date << endl;
  }

  cout << "Enter time (HH:MM:SS) or 'now': ";
  cin >> time;

  if (time == "now" || time == "Now" || time == "n") 
  {
    time = getCurrentTime();
    cout << "-> Using time: " << time << endl;
  }

  cout << "Enter value (amount/weight/dose): ";
  cin >> value;

  cout << "Enter notes (one word or use_underscores): ";
  cin >> notes;
}

void handleLogEvent(LogManager& manager, int choice)
{
  Cat* cat = selectCat(manager);
  if (!cat) return;

  unsigned int eventType = TYPE_NONE;
  if (choice == FEED_EVENT) eventType = TYPE_FEED;
  else if (choice == WEIGHT_EVENT) eventType = TYPE_WEIGHT;
  else if (choice == MEDICINE_EVENT) eventType = TYPE_MEDS;

  string inputDate, inputTime, notes;
  double value;

  promptForEventData(inputDate, inputTime, value, notes);

  cat->addEvent(inputDate, inputTime, eventType, value, notes);
  cout << "Event added for " << cat->name << "!" << endl;
}

void handleReport(LogManager& manager)
{
  if (Cat* cat = selectCat(manager))
  {
    cat->generateReport(std::cout);
  }
}

int main()
{
  LogManager manager;

  manager.registerCat("Satoru", "British_Shorthair", "2022-05-20", "Black");
  manager.registerCat("MeiMei", "Persian_Longhair", "2023-08-15", "Grey");

  manager.loadData();

  bool running = true;
  while (running)
  {
    displayMenu();
    int choice = getValidIntInput();

    switch (choice)
    {
    case FEED_EVENT:
    case WEIGHT_EVENT:
    case MEDICINE_EVENT:
      handleLogEvent(manager, choice);
      break;

    case REPORT_EVENT:
      handleReport(manager);
      break;

    case LIST_EVENT:
      manager.listCats();
      break;

    case ADD_CAT_EVENT:
      handleAddCat(manager);
      break;

    case VIEW_PHOTO_EVENT:
      if (Cat* cat = selectCat(manager))
      {
        string myApiKey = ApiHandler::getApiKey();
        cat->viewPhoto(myApiKey);
      }
      break;

    case EXIT_EVENT:
      manager.saveData();
      cout << "Data saved. Goodbye!" << endl;
      running = false;
      break;

    default:
      cout << "Invalid option. Please try again." << endl;
      break;
    }
  }

  return 0;
}