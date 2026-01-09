#pragma once
#include <string>
#include <vector>
#include <map>
#include "CatLog.h" 

class LogManager
{
private:
  std::map<std::string, Cat> cats;
  const std::string dataFile = "cat_data.csv";

  void processLine(const std::string& line);
  std::vector<std::string> splitCSV(const std::string& line) const;

public:
  void registerCat(const std::string& name);
  void loadData();
  void saveData() const;
  Cat* getCat(const std::string& name);
  void listCats() const;
};