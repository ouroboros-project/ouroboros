
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
  MDNamespace::Ptr mdnamespace = MDNamespace::Create();
  EXPECT_TRUE(static_cast<bool>(mdnamespace));
  EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 0u);
}

TEST (MDNamespaceTest, NestSingle) {
  MDNamespace::Ptr  mdnamespace = MDNamespace::Create(),
                    nested = MDNamespace::Create();
  EXPECT_TRUE(static_cast<bool>(mdnamespace));
  EXPECT_TRUE(mdnamespace->AddNestedNamespace("nested", nested));
  EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 1u);
  EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", nested));
  EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", MDNamespace::Create()));
  EXPECT_EQ(mdnamespace->NestedNamespace("nested"), nested);
  EXPECT_NE(mdnamespace->NestedNamespace("macaco"), nested);
}

TEST (MDParserTest, EmptyFile) {
  istringstream input("");
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  MDNamespace::ConstPtr global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 0u);
}

namespace {
  string test00 =
    "namespace abc {}";
  string test01 =
    "namespace abc {}"
    "namespace def {}";
  string test02 =
    "namespace abc {"
    "  namespace def {}"
    "}";
}

TEST (MDParserTest, SingleEmptyNamespace) {
  istringstream input(test00);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  MDNamespace::ConstPtr global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  MDNamespace::ConstPtr nested = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested));
  EXPECT_EQ(nested->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, TwoEmptyNamespaces) {
  istringstream input(test01);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  MDNamespace::ConstPtr global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 2u);
  MDNamespace::ConstPtr nested1 = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested1));
  EXPECT_EQ(nested1->NestedNamespacesNum(), 0u);
  MDNamespace::ConstPtr nested2 = global->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(nested2));
  EXPECT_EQ(nested2->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, SingleNestedNamespace) {
  istringstream input(test02);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  MDNamespace::ConstPtr global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  MDNamespace::ConstPtr outer = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(outer));
  EXPECT_EQ(outer->NestedNamespacesNum(), 1u);
  MDNamespace::ConstPtr inner = outer->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(inner));
  EXPECT_EQ(inner->NestedNamespacesNum(), 0u);
}

