#include "ApiHandler.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string ApiHandler::ExtractKey(std::istream& stream) 
{
  json configJson;
  try 
  {
    stream >> configJson;
    if (configJson.contains("apiKey")) 
    {
      return configJson["apiKey"];
    }
  }
  catch (const json::parse_error&) 
  {
    return "";
  }
  return "";
}

std::string ApiHandler::GetApiKey(const std::string& filename) 
{
  std::ifstream configFile(filename);
  if (!configFile.is_open()) 
  {
    return "";
  }

  return ExtractKey(configFile);
}