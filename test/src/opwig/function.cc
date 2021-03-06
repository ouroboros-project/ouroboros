class MDFunctionTest : public MDBaseTest {
protected:
};

TEST_F (MDFunctionTest, Create) {
    Parameter p1 = { Type::Create("type0", false), "name0" },
        p2 = { Type::Create("type1", false), "name1" };
    Ptr<Function> var = Function::Create("funcname", 
                                        Type::Create("returntype", false), 
                                        { p1, p2 },
                                        false, false);
    ASSERT_TRUE(static_cast<bool>(var));
    EXPECT_EQ("funcname", var->name());
    EXPECT_EQ("returntype", var->return_type()->full_type());
    ASSERT_EQ(2u, var->num_parameters());
    EXPECT_EQ("type0", var->parameter_type(0)->full_type());
    EXPECT_EQ("name0", var->parameter_name(0));
    EXPECT_EQ("type1", var->parameter_type(1)->full_type());
    EXPECT_EQ("name1", var->parameter_name(1));
    EXPECT_EQ("funcname(type0,type1)", var->id());
    EXPECT_FALSE(var->is_pure());
    EXPECT_FALSE(var->is_virtual());
    EXPECT_FALSE(var->is_static());
    EXPECT_THROW(var->parameter_type(2), std::out_of_range);
}

TEST_F (MDFunctionTest, GlobalNoArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name()", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
}

TEST_F (MDFunctionTest, GlobalSingleArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [ "type" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDFunctionTest, GlobalSinglePointerArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [ "type *" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type *)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg", "type *");
}

TEST_F (MDFunctionTest, GlobalSingleConstRefArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [ "const type &" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(const type &)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg", "const type &");
}

TEST_F (MDFunctionTest, GlobalDoubleArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [ "type0", "type1" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type0,type1)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
}

TEST_F (MDFunctionTest, GlobalMultipleArgFunction) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "params": [ "type0", "type1", "type2" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
    
    auto f = TestFunction("name(type0,type1,type2)", "name", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    TestFunctionParameter(f, 2, "", "type2");
}

//TEST_F (MDFunctionTest, GlobalSingleArgVirtualFunction) {
//    ASSERT_EQ(RunParse("virtual rtype name(type);"), 0);
//    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);
//    
//    auto f = TestFunction("name(type)", "name", "rtype", AccessSpecifier::PUBLIC, false, true);
//    TestFunctionParameter(f, 0, "", "type");
//}

//TEST_F (MDFunctionTest, GlobalBadDoubleVirtualFunction) {
//    RunParseThrow("virtual virtual rtype func();");
//}

//TEST_F (MDFunctionTest, GlobalBadDoubleReturnTypeFunction) {
//    ASSERT_EQ(1, RunParse("rtype virtual rtype func();"));
//}

TEST_F (MDFunctionTest, ManyDifferentFunctions) {
    auto x = RunParse(R"({
    "namespaces": [ "abc" ],
    "functions": [
        {
            "name": "name0",
            "qualified_name": "name0",
            "access": "public",
            "params": [ "ptype0", "ptype1" ],
            "return": "rtype0",
            "deleted": false
        },
        {
            "name": "name1",
            "qualified_name": "name1",
            "access": "public",
            "params": [ "ptype0", "ptype1" ],
            "return": "rtype1",
            "deleted": false
        },
        {
            "name": "name2",
            "qualified_name": "name2",
            "access": "public",
            "params": [ "ptype0", "ptype1" ],
            "return": "rtype2",
            "deleted": false
        },
        {
            "name": "name3",
            "qualified_name": "name3",
            "access": "public",
            "params": [ "ptype0 **" ],
            "return": "rtype3",
            "deleted": false
        },
        {
            "name": "name0",
            "qualified_name": "abc::name0",
            "access": "public",
            "params": [ "ptype0", "ptype1" ],
            "return": "rtype2",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
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
        auto f = TestFunction("name3(ptype0 **)", "name3", "rtype3", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "pname0", "ptype0 **");
    }
    /* fifth */ {
        auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 1u, 0u, 0u);
        auto f = TestFunction(abc, "name0(ptype0,ptype1)", "name0", "rtype2", AccessSpecifier::PUBLIC, false);
        TestFunctionParameter(f, 0, "", "ptype0");
        TestFunctionParameter(f, 1, "pnameX", "ptype1");
    }
}

/* No sense testing definitions
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
}*/

TEST_F (MDFunctionTest, DuplicateDeclaration) {
    RunParseThrow(R"({
    "functions": [
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [],
            "return": "rtype",
            "deleted": false
        },
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [],
            "return": "rtype",
            "deleted": false
        }
    ]})");
}

/* Definition test...
TEST_F (MDFunctionTest, DuplicateDefinition) {
    RunParseThrow("rtype func() {} rtype func() {}");
}*/

TEST_F (MDFunctionTest, ReturnTypeMismatch) {
    RunParseThrow(R"({
    "functions": [
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [],
            "return": "rtype",
            "deleted": false
        },
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [],
            "return": "crash",
            "deleted": false
        }
    ]})");
}

/* Definition test...
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
}*/

TEST_F (MDFunctionTest, FunctionOverload) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [ "type0", "type1" ],
            "return": "rtype",
            "deleted": false
        },
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [ "type0" ],
            "return": "rtype",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 2u, 0u, 0u);
    
    auto f = TestFunction("func(type0,type1)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    
    f = TestFunction("func(type0)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
}

TEST_F (MDFunctionTest, FunctionOverload2) {
    auto x = RunParse(R"({
    "functions": [
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [ "type0", "type1" ],
            "return": "rtype",
            "deleted": false
        },
        {
            "name": "func",
            "qualified_name": "func",
            "access": "public",
            "params": [],
            "return": "void",
            "deleted": false
        }
    ]})");
    ASSERT_EQ(x, 0);
    TestScopeChildNums(global_, 0u, 2u, 0u, 0u);
    
    auto f = TestFunction("func(type0,type1)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type0");
    TestFunctionParameter(f, 1, "", "type1");
    
    f = TestFunction("func()", "func", "void", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
}

TEST_F (MDFunctionTest, FunctionWithParamInitializer) {
    // TODO: add the default value to the meta data?
    auto json = R"({
    "functions": [
        {
            "access": "public",
            "deleted": false,
            "name": "func",
            "params": [
                "type0"
            ],
            "qualified_name": "func",
            "return": "rtype *"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("rtype* func (type0 arg1 = blabla);"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);

    auto f = TestFunction("func(type0)", "func", "rtype *", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
}

TEST_F (MDFunctionTest, FunctionWithMultiParamInitializer) {
    // TODO: add the default value to the meta data?
    auto json = R"({
    "functions": [
        {
            "access": "public",
            "deleted": false,
            "name": "func",
            "params": [
                "type0",
                "const char *const"
            ],
            "qualified_name": "func",
            "return": "rtype *"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("rtype* func (type0 arg1 = blabla, const char *const arg2 = \"tralala\");"), 0);
    TestScopeChildNums(global_, 0u, 1u, 0u, 0u);

    auto f = TestFunction("func(type0,const char *const)", "func", "rtype *", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg1", "type0");
    TestFunctionParameter(f, 1, "arg2", "const char *const");
}
