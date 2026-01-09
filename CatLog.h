#pragma once
#include <iostream>
#include <string>
#include <vector>

enum EventType
{
  TYPE_NONE = 0,
  TYPE_FEED = 1 << 0, // 1
  TYPE_WEIGHT = 1 << 1, // 2
  TYPE_MEDS = 1 << 2  // 4
};

struct Event
{
  std::string date;
  std::string time;
  unsigned int type;
  double value;
  std::string notes;
};

class Cat
{
private:
  void displayStatistics() const;
  void displayRecentEvents() const;

public:
  std::string name;
  std::vector<Event> history;

  Cat(const std::string& n) : name(n) {}

  void addEvent(const std::string& date, const std::string& time, unsigned int type, double value, const std::string& notes = "");
  void generateReport() const;
};