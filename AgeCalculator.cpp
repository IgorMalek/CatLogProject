#include "AgeCalculator.h"
#include <iostream>
#include <ranges>
#include <algorithm>
#include <chrono>
#include <iomanip>

int AgeCalculator::ConvertToHumanYears(int realYears) {
  if (realYears <= 0) return 0;
  if (realYears == 1) return 15;
  if (realYears == 2) return 24;
  return 24 + (realYears - 2) * 4;
}

int AgeCalculator::CalculateRealAge(const std::string& birthDate)
{
  if (birthDate == "Unknown" || birthDate.length() < 4) return -1;

  try
  {
    int birthYear = std::stoi(birthDate.substr(0, 4));

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    tm nowTm;
    localtime_s(&nowTm, &t);
    int currentYear = nowTm.tm_year + 1900;

    return currentYear - birthYear;
  }
  catch (...) { return -1; }
}

std::vector<CatAgeInfo> AgeCalculator::GenerateReportData(const std::map<std::string, Cat>& cats)
{
  namespace ranges = std::ranges;
  namespace views = std::views;

  auto pipeline = cats
    | views::values
    | views::filter([](const Cat& c)
      {
        return CalculateRealAge(c.birthDate) >= 0;
      })
    | views::transform([](const Cat& c) -> CatAgeInfo
      {
        int real = CalculateRealAge(c.birthDate);
        return { c.name, real, ConvertToHumanYears(real) };
      });

  std::vector<CatAgeInfo> results(pipeline.begin(), pipeline.end());

  if (!results.empty())
  {
    ranges::sort(results, ranges::greater{}, &CatAgeInfo::humanYears);
  }

  return results;
}

void AgeCalculator::PrintReportTable(const std::vector<CatAgeInfo>& reportData)
{
  namespace ranges = std::ranges;

  if (reportData.empty()) {
    std::cout << "No cats with valid birth dates found.\n";
    return;
  }

  std::cout << "\n=== CAT AGE REPORT ===\n";
  std::cout << std::left << std::setw(15) << "Name"
    << std::setw(10) << "Real Age"
    << std::setw(15) << "Human Years" << "\n";
  std::cout << "---------------------------------------------\n";

  ranges::for_each(reportData, [](const CatAgeInfo& info) {
    std::cout << std::left << std::setw(15) << info.name
      << info.realYears << " years  "
      << "->  " << info.humanYears << " years old\n";
    });

  std::cout << "---------------------------------------------\n";
}


void AgeCalculator::PrintAgeReport(const std::map<std::string, Cat>& cats)
{
  std::vector<CatAgeInfo> data = GenerateReportData(cats);

  PrintReportTable(data);
}