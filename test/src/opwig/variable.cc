
class MDVariableTest : public MDBaseTest {
protected:
    void CheckVariable(const string& name, const string& type) {
        Ptr<Variable> var = Variable::Create(name, Type::Create(type,false));
        ASSERT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(type, var->type()->full_type());
    }
};

TEST_F (MDVariableTest, Create) {
    CheckVariable("varname", "vartype");
}

TEST_F (MDVariableTest, SingleVariable) {
    auto json = R"({
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "int"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("int var = 0;"), 0);

    TestVariable("var", "int", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, SinglePointerVariable) {
    auto json = R"({
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "int *"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("int *var = 0;"), 0);

    TestVariable("var", "int *", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, MultiVariable) {
    auto json = R"({
    "variables": [
        {
            "access": "public",
            "name": "var1",
            "qualified_name": "var1",
            "static": false,
            "type": "char *"
        },
        {
            "access": "public",
            "name": "var2",
            "qualified_name": "var2",
            "static": false,
            "type": "int"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("char* var1 = \"test\"; int var2 = 2;"), 0);
    TestScopeChildNums(global_, 2u, 0u, 0u, 0u);
    
    TestVariable("var1", "char *", AccessSpecifier::PUBLIC);
    TestVariable("var2", "int", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableConflict) {
    auto json = R"(
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "int"
        },
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "double"
        }
    ]
)";
    RunParseThrow(json);
    //RunParseThrow("int var = 0; double var;");
}

/* Pointless tests for json input.
TEST_F (MDVariableTest, VariableSequence) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("type var1, var2, var3;"), 0);
    TestScopeChildNums(global_, 3u, 0u, 0u, 0u);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableSequenceWithInitializer) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("type var1 = 1, var2 = 2, var3 = 3;"), 0);
    TestScopeChildNums(global_, 3u, 0u, 0u, 0u);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
} */

TEST_F (MDVariableTest, VariableInNamespace) {
    auto json = R"({
    "namespaces": [
        "abc"
    ],
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "abc::var",
            "static": false,
            "type": "int"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("namespace abc { int var; }"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 1u, 0u, 0u, 0u);
    TestVariable(abc, "var", "int", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableWithPtrConst) {
    auto json = R"({
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "int *const"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("int *const var = 0;"), 0);
    TestScopeChildNums(global_, 1u, 0u, 0u, 0u);

    TestVariable("var", "int *const", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, ConstVariableWithPtrConst) {
    auto json = R"({
    "variables": [
        {
            "access": "public",
            "name": "var",
            "qualified_name": "var",
            "static": false,
            "type": "const int *const"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("const int *const var = 0;"), 0);
    TestScopeChildNums(global_, 1u, 0u, 0u, 0u);

    TestVariable("var", "const int *const", AccessSpecifier::PUBLIC);
}
