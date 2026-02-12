#pragma once
#include <string>
#include <iostream>

class ApiHandler
{
public:
  static std::string ExtractKey(std::istream& stream);

  static std::string GetApiKey(const std::string& filename = "secrets.json");
};