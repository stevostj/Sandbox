#include "pch.h"
#include "GameEnginePlugin.h"

TEST(TestInitializeOK_StaticLoad, TestInitialize) {
  int initialize_rv = Initialize();
  EXPECT_EQ(initialize_rv, 0);
}