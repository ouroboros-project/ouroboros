
class MDNamespaceTest : public MDBaseTest {
protected:
    Ptr<Namespace> CheckNamespace(const string& name, unsigned int numNamespaces) {
        Ptr<Namespace> mdnamespace = Namespace::Create(name);
        EXPECT_TRUE(static_cast<bool>(mdnamespace));
        TestScopeChildNums(mdnamespace, 0u, 0u, 0u, numNamespaces);
        EXPECT_EQ(name, mdnamespace->name());
        return mdnamespace;
    }
};


TEST_F (MDNamespaceTest, EmptyFile) {
    auto x = RunParse(R"({})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 0u);
}

TEST_F (MDNamespaceTest, Create) {
    CheckNamespace("abc", 0u);
}  

TEST_F (MDNamespaceTest, NestSingle) {
    Ptr<Namespace>  mdnamespace = Namespace::Create("abc"),
                    nested = Namespace::Create("nested");
    ASSERT_TRUE(static_cast<bool>(mdnamespace));
    EXPECT_TRUE(mdnamespace->AddNestedNamespace(nested));
    EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 1u);
    EXPECT_EQ("abc", mdnamespace->name());
    EXPECT_THROW(mdnamespace->AddNestedNamespace(nested), std::exception);
    EXPECT_THROW(mdnamespace->AddNestedNamespace(Namespace::Create("nested")), std::exception);
    EXPECT_EQ(mdnamespace->NestedNamespace("nested"), nested);
    EXPECT_NE(mdnamespace->NestedNamespace("macaco"), nested);
    EXPECT_EQ("nested", nested->name());
}

TEST_F (MDNamespaceTest, AddSingleFunction) {
    Ptr<Namespace>  space = Namespace::Create("abc");
    Ptr<Function>   foo   = Function::Create("foo", Type::Create("void",false), ParameterList());
    EXPECT_TRUE(space->AddNestedFunction(foo));
    EXPECT_EQ(space->NestedFunctionsNum(), 1u);
    EXPECT_EQ(space->NestedFunction("foo()"), foo);
    EXPECT_FALSE(static_cast<bool>(space->NestedFunction("bar")));
}

TEST_F (MDNamespaceTest, AddManyFunctions) {
    Ptr<Namespace>  space = Namespace::Create("abc");
    Ptr<Function>   foo1  = Function::Create("foo1", Type::Create("void",false), ParameterList());
    Ptr<Function>   foo2  = Function::Create("foo1", Type::Create("void",false), ParameterList());
    Ptr<Function>   foo3  = Function::Create("foo2", Type::Create("void",false), ParameterList());
    EXPECT_TRUE(space->AddNestedFunction(foo1));
    EXPECT_THROW(space->AddNestedFunction(foo2), std::exception);
    EXPECT_TRUE(space->AddNestedFunction(foo3));
    EXPECT_EQ(space->NestedFunctionsNum(), 2u);
    EXPECT_EQ(space->NestedFunction("foo1()"), foo1);
    EXPECT_EQ(space->NestedFunction("foo2()"), foo3);
    EXPECT_FALSE(static_cast<bool>(space->NestedFunction("foo3()")));
}


TEST_F (MDNamespaceTest, SingleEmptyNamespace) {
    auto x = RunParse(R"({
    "namespaces": [ "abc" ]
    })");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u);
}

TEST_F (MDNamespaceTest, TwoEmptyNamespaces) {
    auto x = RunParse(R"({
    "namespaces": [ "abc", "def" ]
    })");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 2u);
    
    TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u);
    TestNamespace("def", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u);
}

TEST_F (MDNamespaceTest, SingleNestedNamespace) {
    auto x = RunParse(R"({
    "namespaces": [ "abc", "abc::def" ]
    })");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 1u);
    TestNamespace(abc, "def", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u);
}

