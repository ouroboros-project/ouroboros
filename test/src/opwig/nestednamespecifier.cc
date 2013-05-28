class MDNestedNamesTest : public MDBaseTest {
protected:
};

TEST_F (MDNestedNamesTest, NameAndGlobalEmptyPath) {
    NestedNameSpecifier nestedName("name", true);
    
    ASSERT_EQ("name", nestedName.name());
    ASSERT_EQ("::name", nestedName.ToString());
    nestedName.set_name("another_name");
    ASSERT_EQ("another_name", nestedName.name());
    ASSERT_EQ("::another_name", nestedName.ToString());
    
    ASSERT_TRUE(nestedName.is_global());
    nestedName.set_global(false);
    ASSERT_FALSE(nestedName.is_global());
    ASSERT_EQ("another_name", nestedName.ToString());
}

TEST_F (MDNestedNamesTest, NonEmptyPath) {
    NestedNameSpecifier nestedName("name");
    nestedName.AddPath("foo");
    nestedName.AddPath("bar");
    
    ASSERT_EQ("foo::bar::name", nestedName.ToString());
    
    nestedName.set_global(true);
    ASSERT_EQ("::foo::bar::name", nestedName.ToString());
    
    nestedName.AddPath("wat");
    ASSERT_EQ("::foo::bar::wat::name", nestedName.ToString());
}

TEST_F (MDNestedNamesTest, VariableWithSimplePath) {
    ASSERT_EQ(RunParse("foo::type var = 0;"), 0);
    TestScopeChildNums(global_, 1u, 0u, 0u, 0u);

    TestVariable("var", "foo::type", AccessSpecifier::PUBLIC);
}

TEST_F (MDNestedNamesTest, FunctionWithSimpleAndGlobalPath) {
    ASSERT_EQ(RunParse("foo::rtype name(::type0 wat, bar::type1);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "foo::rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "wat", "::type0");
    TestFunctionParameter(f, 1, "", "bar::type1");
}

TEST_F (MDNestedNamesTest, NonExistentPath) {
    string test13 = "type thisdoesnotexist::func();";
    RunParseThrow(test13);
}

TEST_F (MDNestedNamesTest, ClassWithNestedBases) {
    ASSERT_EQ(RunParse("class name : virtual foo::base1, public ::bar::base2, protected virtual ::base3 {};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 1u, 0u);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 3u);

    TestClassBaseByIndex(c, 0, "foo::base1", true, AccessSpecifier::PRIVATE);
    TestClassBaseByIndex(c, 1, "::bar::base2", false, AccessSpecifier::PUBLIC);
    TestClassBaseByIndex(c, 2, "::base3", true, AccessSpecifier::PROTECTED);
}
