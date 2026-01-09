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
  const std::string detailsFile = "cat_details.csv";

  void processLine(const std::string& line);
  std::vector<std::string> splitCSV(const std::string& line) const;

  void loadRegistry();

public:
  void registerCat(const std::string& name, const std::string& breed = "Unknown", const std::string& dob = "Unknown", const std::string& color = "Unknown");
  void loadData();
  void saveData() const;
  Cat* getCat(const std::string& name);
  void listCats() const;
};