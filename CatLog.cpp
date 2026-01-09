#include "CatLog.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

std::string getEventTypeString(unsigned int type) {
  if (type & TYPE_FEED) return "FEED";
  if (type & TYPE_WEIGHT) return "WEIGHT";
  if (type & TYPE_MEDS) return "MEDS";
  return "UNKNOWN";
}

void Cat::addEvent(const std::string& date, const std::string& time, unsigned int type, double value, const std::string& notes) {
  Event newEvent;
  newEvent.date = date;
  newEvent.time = time;
  newEvent.type = type;
  newEvent.value = value;
  newEvent.notes = notes;
  history.push_back(newEvent);
}

void Cat::displayStatistics() const
{
  double totalFood = 0.0;
  int weightEntries = 0;
  double latestWeight = 0.0;
  int medsCount = 0;

  for (const auto& event : history)
  {
    if (event.type & TYPE_FEED)
    {
      totalFood += event.value;
    }
    else if (event.type & TYPE_WEIGHT) 
    {
      weightEntries++;
      latestWeight = event.value;
    }
    else if (event.type & TYPE_MEDS) medsCount++;
  }

  std::cout << "Total food amount (all time): " << totalFood << " g." << std::endl;
  std::cout << "Latest recorded weight: " << (latestWeight > 0.0 ? std::to_string(latestWeight) + " kg" : "No data") << std::endl;
  std::cout << "Total medicine administrations: " << medsCount << std::endl;
}

void Cat::displayRecentEvents() const
{
  std::cout << "\n--- Last 5 events ---" << std::endl;
  int start = std::max(0, (int)history.size() - 5);

  for (int i = start; i < history.size(); ++i)
  {
    const auto& event = history[i];
    std::string unit = " units";
    std::string typeStr = getEventTypeString(event.type);

    if (event.type & TYPE_WEIGHT) unit = " kg";
    if (event.type & TYPE_FEED) unit = " g";
    if (event.type & TYPE_MEDS) unit = " dose";

    std::cout << event.date << " " << event.time << " | "
      << std::left << std::setw(8) << typeStr << " | "
      << event.value << unit
      << " | Notes: " << event.notes << std::endl;
  }
}

void Cat::generateReport() const
{
  if (history.empty())
  {
    std::cout << "No historical data for cat " << name << "." << std::endl;
    return;
  }

  std::cout << "\n--- REPORT FOR CAT: " << name << " ---" << std::endl;
  std::cout << "Number of recorded events: " << history.size() << std::endl;

  displayStatistics();
  displayRecentEvents();

  std::cout << "--------------------------------------" << std::endl;
}