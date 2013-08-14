
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
    ASSERT_EQ(RunParse("type var = 0;"), 0);

    TestVariable("var", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, SinglePointerVariable) {
    ASSERT_EQ(RunParse("type *var = 0;"), 0);

    TestVariable("var", "type *", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, MultiVariable) {
    ASSERT_EQ(RunParse("type1 var1 = \"test\"; type2 var2 = 2;"), 0);
    TestScopeChildNums(global_, 2u, 0u, 0u, 0u);
    
    TestVariable("var1", "type1", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type2", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableConflict) {
    RunParseThrow("type var = 0; type2 var;");
}

TEST_F (MDVariableTest, VariableSequence) {
    ASSERT_EQ(RunParse("type var1, var2, var3;"), 0);
    TestScopeChildNums(global_, 3u, 0u, 0u, 0u);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableSequenceWithInitializer) {
    ASSERT_EQ(RunParse("type var1 = 1, var2 = 2, var3 = 3;"), 0);
    TestScopeChildNums(global_, 3u, 0u, 0u, 0u);
    
    TestVariable("var1", "type", AccessSpecifier::PUBLIC);
    TestVariable("var2", "type", AccessSpecifier::PUBLIC);
    TestVariable("var3", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableInNamespace) {
    ASSERT_EQ(RunParse("namespace abc { type var; }"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 1u, 0u, 0u, 0u);
    TestVariable(abc, "var", "type", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, VariableWithPtrConst) {
    ASSERT_EQ(RunParse("type *const var = 0;"), 0);
    TestScopeChildNums(global_, 1u, 0u, 0u, 0u);

    TestVariable("var", "type * const", AccessSpecifier::PUBLIC);
}

TEST_F (MDVariableTest, ConstVariableWithPtrConst) {
    ASSERT_EQ(RunParse("const type *const var = 0;"), 0);
    TestScopeChildNums(global_, 1u, 0u, 0u, 0u);

    TestVariable("var", "const type * const", AccessSpecifier::PUBLIC);
}
