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

    TestVariable("var", "foo::type", AccessSpecifier::PUBLIC);
}

TEST_F (MDNestedNamesTest, FunctionWithSimpleAndGlobalPath) {
    ASSERT_EQ(RunParse("foo::rtype name(::type0 wat, bar::type1);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "foo::rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "wat", "::type0");
    TestFunctionParameter(f, 1, "", "bar::type1");
}
