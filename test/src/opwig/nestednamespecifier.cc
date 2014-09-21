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
    
    auto f = TestFunction("name(::type0,bar::type1)", "name", "foo::rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "wat", "::type0");
    TestFunctionParameter(f, 1, "", "bar::type1");
}

TEST_F (MDNestedNamesTest, NonExistentPath) {
    string test13 = "type thisdoesnotexist::func();";
    RunParseThrow(test13);
}

TEST_F (MDNestedNamesTest, ClassWithNestedBases) {
    auto x = RunParse(R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "base_class": [
                { "access": "private", "name": "foo::base1" },
                { "access": "public", "name": "bar::base2" },
                { "access": "protected", "name": "base3" }
            ],
            "methods": []
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 0u, 1u, 0u);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 3u, 0u, false);

    TestClassBaseByIndex(c, 0, "foo::base1", false, AccessSpecifier::PRIVATE);
    TestClassBaseByIndex(c, 1, "bar::base2", false, AccessSpecifier::PUBLIC);
    TestClassBaseByIndex(c, 2, "base3", false, AccessSpecifier::PROTECTED);
}

TEST_F (MDNestedNamesTest, AddingFunctionToAnotherScope) {
    ASSERT_EQ(RunParse("class name {}; type name::func(name arg1, name arg2);"), 0);
    TestScopeChildNums(global_, 0u, 0u, 1u, 0u);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func(name,name)", "func", "type", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "arg1", "name");
    TestFunctionParameter(f, 1, "arg2", "name");
}

//TODO: The following test fails with exception bad_cast... While this test should fail
//(gcc also fails with it), it should not fail with bad_cast...
//However to fix this we would most likely need to refactor the entire grammar to recognize
//SCOPE_NAMEs...
/*TEST_F (MDNestedNamesTest, AddingFunctionToAnotherScopeFromGlobal) {
    ASSERT_EQ(RunParse("namespace wat { type ::name::func(type); } class name {};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 1u, 1u);

    auto wat = TestNamespace("wat", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false;

    auto f = TestFunction(c, "func(type)", "func", "type", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "", "type");
}*/

TEST_F (MDNestedNamesTest, AddingFunctionToAnotherScopeFromGlobalError) {
    RunParseThrow("namespace wat { type name::func(type); } class name {};");
}
