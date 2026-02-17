#include "AgeCalculator.h"
#include <iostream>
#include <ranges>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <array>

int AgeCalculator::ConvertToHumanYears(int realYears) 
{
  constexpr std::array<int, 3> baseAges{ 0, 15, 24 };

  constexpr int kMaxBaseIndex{ 2 };
  constexpr int kYearlyIncrement{ 4 };

  if (realYears > kMaxBaseIndex) [[likely]] 
  {
    const int extraYears{ realYears - kMaxBaseIndex };
    return baseAges.at(kMaxBaseIndex) + (extraYears * kYearlyIncrement);
  }

  if (realYears >= 0) 
  {
    return baseAges.at(static_cast<size_t>(realYears));
  }

  return baseAges.at(0);
}

int AgeCalculator::CalculateRealAge(const std::string& birthDate) 
{
  try 
  {
    if (birthDate == "Unknown" || birthDate.length() < 4) [[unlikely]] 
    {
      throw std::invalid_argument{ "Invalid birth date format provided." };
    }

    const int birthYear{ std::stoi(birthDate.substr(0, 4)) };

    const auto now{ std::chrono::system_clock::now() };
    const auto days{ std::chrono::floor<std::chrono::days>(now) };
    const std::chrono::year_month_day current_date{ std::chrono::year_month_day{days} };

    const int currentYear{ static_cast<int>(current_date.year()) };

    if (birthYear > currentYear) [[unlikely]] 
    {
      throw std::out_of_range{ "Birth year is in the future." };
    }

    return currentYear - birthYear;
  }
  catch (const std::invalid_argument& e) 
  {
    std::cerr << "[Validation Error] " << e.what() << std::endl;
    return -1;
  }
  catch (const std::out_of_range& e) 
  {
    std::cerr << "[Range Error] " << e.what() << std::endl;
    return -1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "[Unexpected Error] " << e.what() << std::endl;
    return -1;
  }
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

  if (reportData.empty()) 
  {
    std::cout << "No cats with valid birth dates found.\n";
    return;
  }

  std::cout << "\n=== CAT AGE REPORT ===\n";
  std::cout << std::left << std::setw(15) << "Name"
    << std::setw(10) << "Real Age"
    << std::setw(15) << "Human Years" << "\n";
  std::cout << "---------------------------------------------\n";

  ranges::for_each(reportData, [](const CatAgeInfo& info) 
    {
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