#pragma once
#include <string>
#include <vector>
#include <map>
#include "CatLog.h"

struct CatAgeInfo {
  std::string name;
  int realYears;
  int humanYears;
};

class AgeCalculator {
private:
  static std::vector<CatAgeInfo> GenerateReportData(const std::map<std::string, Cat>& cats);

  static void PrintReportTable(const std::vector<CatAgeInfo>& reportData);

public:
  static int ConvertToHumanYears(int realYears);
  static int CalculateRealAge(const std::string& birthDate);

  static void PrintAgeReport(const std::map<std::string, Cat>& cats);
};