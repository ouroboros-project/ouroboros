
class MDVariableTest : public ::testing::Test {
protected:
    Ptr<const Namespace> global_;
    
    int RunParse(const string& str) {
        istringstream input (str);
        MDParser parser(input);
        int ret = parser.parse();
        global_ = parser.global_namespace();
        EXPECT_TRUE(static_cast<bool>(global_));
        return ret;
    }
    void RunParseThrow(const string& str) {
        istringstream input (str);
        MDParser parser(input);
        EXPECT_THROW(parser.parse(), std::exception);
    }
    
    //////////// variable
    Ptr<const Variable> TestVariable(const string& name, const string& type) {
        return TestVariable(global_, name, type);
    }
    Ptr<const Variable> TestVariable(Ptr<const Scope> scope, const string& name, const string& type) {
        Ptr<const Variable> var = scope->GlobalVariable(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(type, var->type());
        return var;
    }
    
    /////////// namespace
    Ptr<const Namespace> TestNamespace(const string& name) {
        return TestNamespace(global_, name);
    }
    Ptr<const Namespace> TestNamespace(Ptr<const Scope> scope, const string& name) {
        Ptr<const Namespace> var = scope->NestedNamespace(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        return var;
    }
};

TEST_F (MDVariableTest, Create) {
    Ptr<Variable> var = Variable::Create("varname", "vartype");
    ASSERT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "varname");
    EXPECT_EQ(var->type(), "vartype");
}

TEST_F (MDVariableTest, SingleVariable) {
    string test11 = "type var = 0;";
    ASSERT_EQ(RunParse(test11), 0);

    TestVariable("var", "type");
}

TEST_F (MDVariableTest, MultiVariable) {
    string test12 = "type1 var1 = \"test\"; type2 var2 = 2;";
    ASSERT_EQ(RunParse(test12), 0);
    
    TestVariable("var1", "type1");
    TestVariable("var2", "type2");
}

TEST_F (MDVariableTest, VariableConflict) {
    string test13 = "type var = 0; type2 var;";
    RunParseThrow(test13);
}

TEST_F (MDVariableTest, VariableSequence) {
    string test14 = "type var1 = 1, var2 = 2, var3 = 3;";
    ASSERT_EQ(RunParse(test14), 0);
    
    TestVariable("var1", "type");
    TestVariable("var2", "type");
    TestVariable("var3", "type");
}

TEST_F (MDVariableTest, VariableInNamespace) {
    string test15 = "namespace abc { type var; }";
    ASSERT_EQ(RunParse(test15), 0);
    
    auto abc = TestNamespace("abc");
    TestVariable(abc, "var", "type");
}
