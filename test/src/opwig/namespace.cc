
TEST (MDParserTest, EmptyFile) {
    istringstream input("");
    MDParser parser(input);
    EXPECT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    EXPECT_EQ(global->NestedNamespacesNum(), 0u);
}

TEST (NamespaceTest, Create) {
    Ptr<Namespace> mdnamespace = Namespace::Create("abc");
    ASSERT_TRUE(static_cast<bool>(mdnamespace));
    EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 0u);
    EXPECT_EQ("abc", mdnamespace->name());
}  

TEST (NamespaceTest, NestSingle) {
    Ptr<Namespace>  mdnamespace = Namespace::Create("abc"),
                    nested = Namespace::Create("nested");
    ASSERT_TRUE(static_cast<bool>(mdnamespace));
    EXPECT_TRUE(mdnamespace->AddNestedNamespace("nested", nested));
    EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 1u);
    EXPECT_EQ("abc", mdnamespace->name());
    EXPECT_THROW(mdnamespace->AddNestedNamespace("nested", nested), std::exception);
    EXPECT_THROW(mdnamespace->AddNestedNamespace("nested", Namespace::Create("nested")), std::exception);
    EXPECT_EQ(mdnamespace->NestedNamespace("nested"), nested);
    EXPECT_NE(mdnamespace->NestedNamespace("macaco"), nested);
    EXPECT_EQ("nested", nested->name());
}

TEST (NamespaceTest, AddSingleFunction) {
    Ptr<Namespace>  space = Namespace::Create("abc");
    Ptr<Function>   foo   = Function::Create("foo", "void", {}, false);
    EXPECT_TRUE(space->AddNestedFunction(foo));
    EXPECT_EQ(space->NestedFunctionsNum(), 1u);
    EXPECT_EQ(space->NestedFunction("foo"), foo);
    EXPECT_FALSE(static_cast<bool>(space->NestedFunction("bar")));
}

TEST (NamespaceTest, AddManyFunctions) {
    Ptr<Namespace>  space = Namespace::Create("abc");
    Ptr<Function>   foo1  = Function::Create("foo1", "void", {}, false);
    Ptr<Function>   foo2  = Function::Create("foo1", "void", {}, false);
    Ptr<Function>   foo3  = Function::Create("foo2", "void", {}, false);
    EXPECT_TRUE(space->AddNestedFunction(foo1));
    EXPECT_THROW(space->AddNestedFunction(foo2), std::exception);
    EXPECT_TRUE(space->AddNestedFunction(foo3));
    EXPECT_EQ(space->NestedFunctionsNum(), 2u);
    EXPECT_EQ(space->NestedFunction("foo1"), foo1);
    EXPECT_EQ(space->NestedFunction("foo2"), foo3);
    EXPECT_FALSE(static_cast<bool>(space->NestedFunction("foo3")));
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
    EXPECT_EQ("abc", nested->name());
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
    EXPECT_EQ("abc", nested1->name());
    Ptr<const Namespace> nested2 = global->NestedNamespace("def");
    EXPECT_TRUE(static_cast<bool>(nested2));
    EXPECT_EQ(nested2->NestedNamespacesNum(), 0u);
    EXPECT_EQ("def", nested2->name());
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
    EXPECT_EQ("abc", outer->name());
    Ptr<const Namespace> inner = outer->NestedNamespace("def");
    EXPECT_TRUE(static_cast<bool>(inner));
    EXPECT_EQ(inner->NestedNamespacesNum(), 0u);
    EXPECT_EQ("def", inner->name());
}

