#include "LogManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void LogManager::registerCat(const std::string& name)
{
  if (cats.find(name) == cats.end())
  {
    cats.emplace(name, Cat(name));
  }
}

Cat* LogManager::getCat(const std::string& name)
{
  if (cats.count(name))
  {
    return &cats.at(name);
  }
  return nullptr;
}

void LogManager::listCats() const
{
  if (cats.empty())
  {
    std::cout << "No registered cats." << std::endl;
    return;
  }
  std::cout << "\n--- Registered Cats ---" << std::endl;
  for (const auto& pair : cats)
  {
    std::cout << "- " << pair.first << std::endl;
  }
}

void LogManager::saveData() const
{
  std::ofstream file(dataFile);
  if (!file.is_open()) 
  {
    std::cerr << "Error: Cannot open file " << dataFile << " for writing!" << std::endl;
    return;
  }

  file << "CatName;Date;Time;TypeMask;Value;Notes\n";

  for (const auto& catPair : cats) 
  {
    const std::string& catName = catPair.first;
    for (const auto& event : catPair.second.history)
    {
      file << catName << ";"
        << event.date << ";"
        << event.time << ";"
        << event.type << ";"
        << event.value << ";"
        << event.notes << "\n";
    }
  }
  std::cout << "Saved " << cats.size() << " cats to file " << dataFile << std::endl;
  file.close();
}

void LogManager::loadData()
{
  std::ifstream file(dataFile);
  if (!file.is_open()) 
  {
    std::cout << "Created new data file: " << dataFile << std::endl;
    return;
  }

  std::string line;
  std::getline(file, line); // Skip header

  while (std::getline(file, line))
  {
    processLine(line);
  }

  std::cout << "Loaded data from " << dataFile << " for " << cats.size() << " cats." << std::endl;
  file.close();
}

void LogManager::processLine(const std::string& line)
{
  std::vector<std::string> segments = splitCSV(line);

  if (segments.size() != 6) return;

  try
  {
    const std::string& catName = segments[0];

    registerCat(catName);
    Cat* currentCat = getCat(catName);

    Event event;
    event.date = segments[1];
    event.time = segments[2];
    event.type = std::stoi(segments[3]);
    event.value = std::stod(segments[4]);
    event.notes = segments[5];

    currentCat->history.push_back(event);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error reading line: " << e.what() << " - Line skipped." << std::endl;
  }
}

std::vector<std::string> LogManager::splitCSV(const std::string& line) const
{
  std::stringstream ss(line);
  std::string segment;
  std::vector<std::string> segments;

  while (std::getline(ss, segment, ';'))
  {
    segments.push_back(segment);
  }
  return segments;
}