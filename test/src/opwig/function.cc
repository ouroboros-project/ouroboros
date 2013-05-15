class MDFunctionTest : public MDBaseTest {
protected:
};

TEST_F (MDFunctionTest, Create) {
  Ptr<Function> var = Function::Create("funcname", "returntype", {{"type0", "name0"}, {"type1", "name1"}}, false);
  ASSERT_TRUE(static_cast<bool>(var));
  EXPECT_EQ(var->name(), "funcname");
  EXPECT_EQ(var->return_type(), "returntype");
  EXPECT_EQ(var->parameter_type(0), "type0");
  EXPECT_EQ(var->parameter_name(0), "name0");
  EXPECT_EQ(var->parameter_type(1), "type1");
  EXPECT_EQ(var->parameter_name(1), "name1");
  EXPECT_THROW(var->parameter_type(2), std::out_of_range);
}


TEST_F (MDFunctionTest, GlobalNoArgFunction) {
    ASSERT_EQ(RunParse("rtype name();"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
}

TEST_F (MDFunctionTest, GlobalSingleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDFunctionTest, GlobalDoubleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type0, type1);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, GlobalMultipleArgFunction) {
    ASSERT_EQ(RunParse("rtype name(type0, type1, type2);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name", "rtype", AccessSpecifier::PUBLIC, false);
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
        auto f = TestFunction("name0", "rtype0", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "", "ptype0");
        TestFunctionParameter(f, 1, "", "ptype1");
    }
    /* second */ {
        auto f = TestFunction("name1", "rtype1", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
        TestFunctionParameter(f, 1, "", "ptype1");
    }
    /* third */ {
        auto f = TestFunction("name2", "rtype2", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
        TestFunctionParameter(f, 1, "pname1", "ptype1");
    }
    /* forth */ {
        auto f = TestFunction("name3", "rtype3", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0");
    }
    /* fifth */ {
        auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 1u, 0u, 0u);
        auto f = TestFunction(abc, "name0", "rtype2", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "", "ptype0");
        TestFunctionParameter(f, 1, "pnameX", "ptype1");
    }
}
