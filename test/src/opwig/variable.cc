
TEST (VariableTest, Create) {
  Ptr<Variable> var = Variable::Create("varname", "vartype");
  ASSERT_TRUE(static_cast<bool>(var));
  EXPECT_EQ(var->name(), "varname");
  EXPECT_EQ(var->type(), "vartype");
}

namespace {
    string test11 = 
        "type var = 0;";
    string test12 = 
        "type1 var1 = \"test\";"
        "type2 var2 = 2;";
    string test13 =
        "type var = 0;"
        "type2 var;";
    string test14 =
        "type var1 = 1, var2 = 2, var3 = 3;";
    string test15 =
        "namespace abc {"
        "type var;"
        "}";
}

TEST (MDParserTest, SingleVariable) {
    istringstream input(test11);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));

    Ptr<const Variable> var = global->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
}

TEST (MDParserTest, MultiVariable) {
    istringstream input(test12);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Variable> var1 = global->GlobalVariable("var1");
    EXPECT_TRUE(static_cast<bool>(var1));
    EXPECT_EQ(var1->name(), "var1");
    EXPECT_EQ(var1->type(), "type1");
    Ptr<const Variable> var2 = global->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type2");
}

TEST (MDParserTest, VariableConflict) {
    istringstream input(test13);
    MDParser parser(input);
    EXPECT_THROW(parser.parse(), std::exception);
}

TEST (MDParserTest, VariableSequence) {
    istringstream input(test14);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Variable> var1 = global->GlobalVariable("var1");
    EXPECT_TRUE(static_cast<bool>(var1));
    EXPECT_EQ(var1->name(), "var1");
    EXPECT_EQ(var1->type(), "type");
    Ptr<const Variable> var2 = global->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type");
    Ptr<const Variable> var3 = global->GlobalVariable("var3");
    EXPECT_TRUE(static_cast<bool>(var3));
    EXPECT_EQ(var3->name(), "var3");
    EXPECT_EQ(var3->type(), "type");
}

TEST (MDParserTest, VariableInNamespace) {
    istringstream input(test15);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Namespace> abc = global->NestedNamespace("abc");
    EXPECT_TRUE(static_cast<bool>(abc));
    Ptr<const Variable> var = abc->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
}
