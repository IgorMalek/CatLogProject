#include "ApiHandler.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string ApiHandler::getApiKey() {
  std::ifstream configFile("secrets.json");

  if (!configFile.is_open()) {
    std::cerr << "ERROR: File secrets.json has not been found.\n"
      << "Please create 'secrets.json' with the following format:\n"
      << "{\n  \"apiKey\": \"YOUR_KEY_HERE\"\n}\n";
    return "";
  }

  json configJson;
  try {
    configFile >> configJson;
    if (configJson.contains("apiKey")) {
      return configJson["apiKey"];
    }
    else {
      std::cerr << "ERROR: 'apiKey' field missing in secrets.json.\n";
      return "";
    }
  }
  catch (json::parse_error& e) {
    std::cerr << "ERROR: Failed to parse secrets.json: " << e.what() << "\n";
    return "";
  }
}