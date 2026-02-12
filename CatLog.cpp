#define NOMINMAX
#include "CatLog.h"
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

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
  double totalFood = std::accumulate(history.begin(), history.end(), 0.0,
    [](double sum, const Event& e) 
    {
      return (e.type & TYPE_FEED) ? sum + e.value : sum;
    });

  long long feedCount = std::count_if(history.begin(), history.end(),
    [](const Event& e) 
    {
      return (e.type & TYPE_FEED);
    });

  long long medsCount = std::count_if(history.begin(), history.end(),
    [](const Event& e) 
    {
      return (e.type & TYPE_MEDS);
    });

  auto isWeightEvent = [](const Event& e) { return (e.type & TYPE_WEIGHT); };
  auto firstWeightIt = std::find_if(history.begin(), history.end(), isWeightEvent);
  auto latestWeightIt = std::find_if(history.rbegin(), history.rend(), isWeightEvent);

  std::cout << "Total food amount (all time): " << totalFood << " g";
  if (feedCount > 0) 
  {
    std::cout << " (Avg " << (totalFood / feedCount) << " g/meal)";
  }
  std::cout << std::endl;

  std::cout << "Weight: ";
  if (latestWeightIt != history.rend())
  {
    double latestVal = latestWeightIt->value;
    double firstVal = firstWeightIt->value;

    std::cout << latestVal << " kg";

    if (firstWeightIt != history.end() && firstVal != latestVal) 
    {
      double change = latestVal - firstVal;
      std::cout << " (Change: " << (change > 0 ? "+" : "") << change << " kg)";
    }
  }
  else 
  {
    std::cout << "No data";
  }
  std::cout << std::endl;

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

void Cat::generateReport(std::ostream& os) const
{
  if (history.empty() && breed == "Unknown")
  {
    os << "No historical data for cat " << name << "." << std::endl;
    return;
  }

  os << "\n--- REPORT FOR CAT: " << name << " ---" << "\n"
     << "Breed: " << breed << " | Born: " << birthDate << " | Color: " << furColor << "\n"
     << "Number of recorded events: " << history.size() << "\n";

  displayStatistics();
  displayRecentEvents();

  std::cout << "--------------------------------------" << std::endl;
}

void Cat::viewPhoto(const std::string& apiKey) const
{
  std::string url = "https://cataas.com/cat";

  if (furColor != "Unknown" && !furColor.empty())
    url += "/" + furColor;
  else
    url += "?";

  if (!apiKey.empty())
  {
    if (url.find('?') != std::string::npos) 
    {
      if (url.back() == '?')
        url += "api_key=" + apiKey;
      else
        url += "&api_key=" + apiKey;
    }
    else 
    {
      url += "?api_key=" + apiKey;
    }
  }

  std::cout << "Opening browser: " << url << " ..." << std::endl;
  ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}