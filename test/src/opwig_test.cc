
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

namespace {
  string test00 =
    "namespace abc {}";
  string test01 =
    "namespace abc {}"
    "namespace def {}";
}

TEST (MDParserTest, SingleEmptyNamespace) {
  istringstream input(test00);
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  EXPECT_EQ(parser.global_namespace()->NestedNamespacesNum(), 1u);
  // TODO check namespace by name
}

TEST (MDParserTest, TwoEmptyNamespaces) {
  istringstream input(test01);
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  EXPECT_EQ(parser.global_namespace()->NestedNamespacesNum(), 2u);
  // TODO check namespace by name
}

