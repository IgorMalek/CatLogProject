#include <iostream>
#include <string>
#include <limits>
#include "LogManager.h"

using namespace std;

#define FEED_EVENT 1
#define WEIGHT_EVENT 2
#define MEDICINE_EVENT 3
#define REPORT_EVENT 4
#define LIST_EVENT 5
#define EXIT_EVENT 6

void displayMenu()
{
  cout << "\n=== Cat Log ===" << endl;
  cout << "1. Log Feeding (FEED)" << endl;
  cout << "2. Log Weight (WEIGHT)" << endl;
  cout << "3. Log Medicine (MEDS)" << endl;
  cout << "4. Generate Report for Cat" << endl;
  cout << "5. List Registered Cats" << endl;
  cout << "6. Exit and Save" << endl;
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

  cout << "Enter date (YYYY-MM-DD): ";
  cin >> inputDate;
  cout << "Enter time (HH:MM:SS): ";
  cin >> inputTime;

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

  if (Cat* cat = manager.getCat(catName)) {
    cat->generateReport();
  }
  else {
    cout << "Error: Cat with this name does not exist." << endl;
  }
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