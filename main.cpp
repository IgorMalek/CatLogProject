#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <string>
#include <limits>
#include <ctime>   
#include <sstream> 
#include <iomanip> 
#include "LogManager.h"

using namespace std;

#define FEED_EVENT 1
#define WEIGHT_EVENT 2
#define MEDICINE_EVENT 3
#define REPORT_EVENT 4
#define LIST_EVENT 5
#define ADD_CAT_EVENT 6 
#define EXIT_EVENT 7    

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
  cout << "\n=== Cat Log ===" << endl;
  cout << "1. Log Feeding (FEED)" << endl;
  cout << "2. Log Weight (WEIGHT)" << endl;
  cout << "3. Log Medicine (MEDS)" << endl;
  cout << "4. Generate Report for Cat" << endl;
  cout << "5. List Registered Cats" << endl;
  cout << "6. Register New Cat" << endl;
  cout << "7. Exit and Save" << endl;
  cout << "---------------------------------" << endl;
  cout << "Select option: ";
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
  string newName;
  cout << "Enter name for the new cat: ";
  cin >> newName;

  if (newName.length() > 0) {
    manager.registerCat(newName);
    cout << "Cat '" << newName << "' registered successfully." << endl;
  }
  else {
    cout << "Invalid name." << endl;
  }
}

void handleLogEvent(LogManager& manager, int choice)
{
  manager.listCats();

  string catName;
  cout << "Enter cat name: ";
  cin >> catName;

  Cat* cat = manager.getCat(catName);
  if (!cat)
  {
    cout << "Error: Cat with this name does not exist." << endl;
    return;
  }

  unsigned int eventType = TYPE_NONE;
  if (choice == FEED_EVENT) eventType = TYPE_FEED;
  else if (choice == WEIGHT_EVENT) eventType = TYPE_WEIGHT;
  else if (choice == MEDICINE_EVENT) eventType = TYPE_MEDS;

  string inputDate, inputTime, notes;
  double value;

  cout << "Enter date (YYYY-MM-DD) or 'today': ";
  cin >> inputDate;

  if (inputDate == "today" || inputDate == "Today" || inputDate == "t") {
    inputDate = getTodayDate();
    cout << "-> Using date: " << inputDate << endl;
  }

  cout << "Enter time (HH:MM:SS) or 'now': ";
  cin >> inputTime;

  if (inputTime == "now" || inputTime == "Now" || inputTime == "n") {
    inputTime = getCurrentTime();
    cout << "-> Using time: " << inputTime << endl;
  }

  cout << "Enter value (amount/weight/dose): ";
  cin >> value;

  cout << "Enter notes (one word or use_underscores): ";
  cin >> notes;

  cat->addEvent(inputDate, inputTime, eventType, value, notes);
  cout << "Event added for " << catName << "!" << endl;
}

void handleReport(LogManager& manager)
{
  manager.listCats();
  string catName;
  cout << "Enter cat name for report: ";
  cin >> catName;

  if (Cat* cat = manager.getCat(catName))
    cat->generateReport();
  else
    cout << "Error: Cat with this name does not exist." << endl;
}

int main()
{
  LogManager manager;

  manager.registerCat("Tygrysek");
  manager.registerCat("Puszek");
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