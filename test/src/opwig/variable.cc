
class MDVariableTest : public MDBaseTest {
protected:
    void CheckVariable(const string& name, const string& type) {
        Ptr<Variable> var = Variable::Create(name, type);
        ASSERT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(type, var->type());
    }
};

TEST_F (MDVariableTest, Create) {
    CheckVariable("varname", "vartype");
}

TEST_F (MDVariableTest, SingleVariable) {
    string test11 = "type var = 0;";
    ASSERT_EQ(RunParse(test11), 0);

    TestVariable("var", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, MultiVariable) {
    string test12 = "type1 var1 = \"test\"; type2 var2 = 2;";
    ASSERT_EQ(RunParse(test12), 0);
    
    TestVariable("var1", "type1", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type2", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableConflict) {
    string test13 = "type var = 0; type2 var;";
    RunParseThrow(test13);
}

TEST_F (MDVariableTest, VariableSequence) {
    ASSERT_EQ(RunParse("type var1, var2, var3;"), 0);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableSequenceWithInitializer) {
    ASSERT_EQ(RunParse("type var1 = 1, var2 = 2, var3 = 3;"), 0);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableInNamespace) {
    string test15 = "namespace abc { type var; }";
    ASSERT_EQ(RunParse(test15), 0);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 1u, 0u, 0u, 0u);
    TestVariable(abc, "var", "type", AccessSpecifier::PUBLIC);
}
