
#include <opwig/mdnamespace.h>
#include <opwig/mdparser/mdparser.h>

#include <gtest/gtest.h>

#include <string>
#include <istream>
#include <sstream>

using std::string;
using std::istringstream;

using opwig::MDNamespace;
using opwig::MDParser;

TEST (MDNamespaceTest, Create) {
  EXPECT_TRUE(static_cast<bool>(MDNamespace::Create()));
}

TEST (MDParserTest, EmptyFile) {
  istringstream input("");
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  EXPECT_EQ(parser.global_namespace()->NestedNamespacesNum(), 0u);
}

