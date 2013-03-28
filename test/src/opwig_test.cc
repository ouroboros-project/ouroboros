
#include <opwig/md/namespace.h>
#include <opwig/parser/mdparser.h>

#include <gtest/gtest.h>

#include <string>
#include <istream>
#include <sstream>

using std::string;
using std::istringstream;

using opwig::md::Ptr;
using opwig::md::Namespace;
using opwig::MDParser;

TEST (NamespaceTest, Create) {
  Ptr<Namespace> mdnamespace = Namespace::Create();
  EXPECT_TRUE(static_cast<bool>(mdnamespace));
  EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 0u);
}

TEST (NamespaceTest, NestSingle) {
  Ptr<Namespace>  mdnamespace = Namespace::Create(),
                    nested = Namespace::Create();
  EXPECT_TRUE(static_cast<bool>(mdnamespace));
  EXPECT_TRUE(mdnamespace->AddNestedNamespace("nested", nested));
  EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 1u);
  EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", nested));
  EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", Namespace::Create()));
  EXPECT_EQ(mdnamespace->NestedNamespace("nested"), nested);
  EXPECT_NE(mdnamespace->NestedNamespace("macaco"), nested);
}

TEST (MDParserTest, EmptyFile) {
  istringstream input("");
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
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
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> nested = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested));
  EXPECT_EQ(nested->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, TwoEmptyNamespaces) {
  istringstream input(test01);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 2u);
  Ptr<const Namespace> nested1 = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested1));
  EXPECT_EQ(nested1->NestedNamespacesNum(), 0u);
  Ptr<const Namespace> nested2 = global->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(nested2));
  EXPECT_EQ(nested2->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, SingleNestedNamespace) {
  istringstream input(test02);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> outer = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(outer));
  EXPECT_EQ(outer->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> inner = outer->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(inner));
  EXPECT_EQ(inner->NestedNamespacesNum(), 0u);
}

