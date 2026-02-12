#include <gtest/gtest.h>
#include <sstream>
#include "ApiHandler.h"

TEST(ApiHandlerTests, ExtractsKeyFromValidJson) 
{
  std::stringstream mockData;
  mockData << R"({ "apiKey": "MOCK_KEY_12345" })";

  std::string result = ApiHandler::ExtractKey(mockData);

  EXPECT_EQ(result, "MOCK_KEY_12345");
}

TEST(ApiHandlerTests, ReturnsEmptyIfKeyMissing) 
{
  std::stringstream mockData;
  mockData << R"({ "wrongField": "some_value" })";

  std::string result = ApiHandler::ExtractKey(mockData);

  EXPECT_EQ(result, "");
}

TEST(ApiHandlerTests, ReturnsEmptyOnBadJson) 
{
  std::stringstream mockData;
  mockData << "{ this is not json }";

  std::string result = ApiHandler::ExtractKey(mockData);

  EXPECT_EQ(result, "");
}