
#include <opwig/mdnamespace.h>
#include <gtest/gtest.h>

using opwig::MDNamespace;

TEST (MDNamespaceTest, Create) {
  EXPECT_TRUE(static_cast<bool>(MDNamespace::Create()));
}

