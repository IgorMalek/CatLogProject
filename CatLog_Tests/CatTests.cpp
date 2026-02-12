#include <gtest/gtest.h>
#include "CatLog.h" 
#include "LogManager.h"

TEST(CatTests, ConstructorSetsFieldsCorrectly) {
  Cat myCat("Satoru", "British_Shorthair", "2022-05-20", "Black");

  EXPECT_EQ(myCat.name, "Satoru");
  EXPECT_EQ(myCat.breed, "British_Shorthair");
  EXPECT_EQ(myCat.furColor, "Black");
  EXPECT_TRUE(myCat.history.empty());
}

TEST(CatTests, AddEventIncreasesHistory) {
  Cat myCat("MeiMei");

  myCat.addEvent("2023-01-01", "12:00", TYPE_FEED, 85.5, "Tuna");

  ASSERT_EQ(myCat.history.size(), 1);

  EXPECT_EQ(myCat.history[0].value, 85.5);
  EXPECT_EQ(myCat.history[0].notes, "Tuna");
}

TEST(ManagerTests, RegisterAndGetCat) {
  LogManager manager;
  manager.registerCat("TestCat", "Unknown", "2020-01-01", "White");

  Cat* retrievedCat = manager.getCat("TestCat");

  ASSERT_NE(retrievedCat, nullptr);
  EXPECT_EQ(retrievedCat->name, "TestCat");
}


TEST(ManagerTests, GetInvalidCatReturnsNull) {
  LogManager manager;
  Cat* ghostCat = manager.getCat("Ghost");

  EXPECT_EQ(ghostCat, nullptr);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}