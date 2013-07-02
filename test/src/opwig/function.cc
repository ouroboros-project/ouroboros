class MDFunctionTest : public MDBaseTest {
protected:
};

TEST_F (MDFunctionTest, Create) {
    Ptr<Function> var = Function::Create("funcname", "returntype", {{"type0", "name0"}, {"type1", "name1"}}, false, false);
    ASSERT_TRUE(static_cast<bool>(var));
    EXPECT_EQ("funcname", var->name());
    EXPECT_EQ("returntype", var->return_type());
    EXPECT_EQ("type0", var->parameter_type(0));
    EXPECT_EQ("name0", var->parameter_name(0));
    EXPECT_EQ("type1", var->parameter_type(1));
    EXPECT_EQ("name1", var->parameter_name(1));
    EXPECT_EQ("funcname(type0,type1)", var->id());
    EXPECT_FALSE(var->is_pure());
    EXPECT_FALSE(var->is_virtual());
    EXPECT_THROW(var->parameter_type(2), std::out_of_range);
}

TEST_F (MDFunctionTest, GlobalNoArgFunction) {
    ASSERT_EQ(RunParse("rtype name();"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name()", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
}

TEST_F (MDFunctionTest, GlobalSingleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDFunctionTest, GlobalDoubleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type0, type1);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type0,type1)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, GlobalMultipleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type0, type1, type2);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type0,type1,type2)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    TestFunctionParameter(f, 2, "", "type2");
}

TEST_F (MDFunctionTest, ManyDifferentFunctions) {
    string input(
        "rtype0 name0(ptype0, ptype1);"
        "rtype1 name1(ptype0 pname0, ptype1);"
        "rtype2 name2(ptype0 pname0, ptype1 pname1);"
        "rtype3 name3(ptype0 **pname0[]);"
        "namespace abc {"
        "  rtype2 name0(ptype0, ptype1 pnameX);"
        "}"
    );
    ASSERT_EQ(RunParse(input), 0);
    TestScopeChildNums(global_, 0u, 4u, 0u, 1u);
    
    /* first */ {
        auto f = TestFunction("name0(ptype0,ptype1)", "name0", "rtype0", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "", "ptype0");
        TestFunctionParameter(f, 1, "", "ptype1");
    }
    /* second */ {
        auto f = TestFunction("name1(ptype0,ptype1)", "name1", "rtype1", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
        TestFunctionParameter(f, 1, "", "ptype1");
    }
    /* third */ {
        auto f = TestFunction("name2(ptype0,ptype1)", "name2", "rtype2", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
        TestFunctionParameter(f, 1, "pname1", "ptype1");
    }
    /* forth */ {
        auto f = TestFunction("name3(ptype0)", "name3", "rtype3", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
    }
    /* fifth */ {
        auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 1u, 0u, 0u);
        auto f = TestFunction(abc, "name0(ptype0,ptype1)", "name0", "rtype2", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "", "ptype0");
        TestFunctionParameter(f, 1, "pnameX", "ptype1");
    }
}

TEST_F (MDFunctionTest, SimpleFunctionDefinition) {
    ASSERT_EQ(RunParse("rtype name(type0 arg1, type1) { return rtype; }"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type0,type1)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, SimpleFunctionDefinitionInNamespace) {
    ASSERT_EQ(RunParse("namespace abc { rtype name(type0 arg1, type1) { return rtype; } }"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 1u, 0u, 0u);
    auto f = TestFunction(abc, "name(type0,type1)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, SimpleFunctionDefinitionOutsideNamespace) {
    ASSERT_EQ(RunParse("namespace abc { rtype name(type0 arg1, type1); } rtype abc::name(type0 arg1, type1) { return rtype; }"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 1u, 0u, 0u);
    auto f = TestFunction(abc, "name(type0,type1)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, DuplicateDeclaration) {
    RunParseThrow("rtype func(); rtype func();");
}

TEST_F (MDFunctionTest, DuplicateDefinition) {
    RunParseThrow("rtype func() {} rtype func() {}");
}

TEST_F (MDFunctionTest, ReturnTypeMismatch) {
    RunParseThrow("rtype func(); crash func() {}");
}

TEST_F (MDFunctionTest, InvalidDefinition) {
    RunParseThrow("rtype func {}");
}

TEST_F (MDFunctionTest, MultiFunctionDefinition) {
    ASSERT_EQ(RunParse("rtype func(type0 arg1, type1) { } rtype func2(type0) {}"), 0);
    TestScopeChildNums(global_, 0u, 2u, 0u, 0u);
    
    auto f = TestFunction("func(type0,type1)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    
    f = TestFunction("func2(type0)", "func2", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
}

TEST_F (MDFunctionTest, FunctionOverload) {
    ASSERT_EQ(RunParse("rtype func(type0, type1); rtype func(type0) {}"), 0);
    TestScopeChildNums(global_, 0u, 2u, 0u, 0u);
    
    auto f = TestFunction("func(type0,type1)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    
    f = TestFunction("func(type0)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
}

TEST_F (MDFunctionTest, FunctionOverload2) {
    ASSERT_EQ(RunParse("rtype func(type0, type1); void func() {}"), 0);
    TestScopeChildNums(global_, 0u, 2u, 0u, 0u);
    
    auto f = TestFunction("func(type0,type1)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    
    f = TestFunction("func()", "func", "void", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
}
