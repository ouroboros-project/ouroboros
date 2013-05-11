
TEST (FunctionTest, Create) {
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

namespace {
    string test21 = 
        "rtype name();";
    string test22 = 
        "rtype name(type);";
    string test23 =
        "rtype name(type0, type1);";
    string test24 =
        "rtype name(type0, type1, type2);";
    string test25 =
        "rtype0 name0(ptype0, ptype1);"
        "rtype1 name1(ptype0 pname0, ptype1);"
        "rtype2 name2(ptype0 pname0, ptype1 pname1);"
        "rtype3 name3(ptype0 **pname0[]);"
        "namespace abc {"
        "  rtype name0(ptype0, ptype1 pnameX);"
        "}";
}

TEST (MDParserTest, GlobalNoArgFunction) {
    istringstream input("rtype name();");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    ASSERT_TRUE(static_cast<bool>(global));
    ASSERT_EQ(1u, global->NestedFunctionsNum());
    Ptr<const Function> func = global->NestedFunction("name");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
}

TEST (MDParserTest, GlobalSingleArgFunction) {
    istringstream input("rtype name(type);");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    ASSERT_TRUE(static_cast<bool>(global));
    ASSERT_EQ(1u, global->NestedFunctionsNum());
    Ptr<const Function> func = global->NestedFunction("name");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
}

TEST (MDParserTest, GlobalDoubleArgFunction) {
    istringstream input("rtype name(type0, type1);");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    ASSERT_TRUE(static_cast<bool>(global));
    ASSERT_EQ(1u, global->NestedFunctionsNum());
    Ptr<const Function> func = global->NestedFunction("name");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type0", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_EQ("type1", func->parameter_type(1));
    EXPECT_EQ("", func->parameter_name(1));
}

TEST (MDParserTest, GlobalMultipleArgFunction) {
    istringstream input("rtype name(type0, type1, type2);");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    ASSERT_TRUE(static_cast<bool>(global));
    ASSERT_EQ(1u, global->NestedFunctionsNum());
    Ptr<const Function> func = global->NestedFunction("name");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type0", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_EQ("type1", func->parameter_type(1));
    EXPECT_EQ("", func->parameter_name(1));
    EXPECT_EQ("type2", func->parameter_type(2));
    EXPECT_EQ("", func->parameter_name(2));
}

TEST (MDParserTest, ManyDifferentFunctions) {
    istringstream input(
        "rtype0 name0(ptype0, ptype1);"
        "rtype1 name1(ptype0 pname0, ptype1);"
        "rtype2 name2(ptype0 pname0, ptype1 pname1);"
        "rtype3 name3(ptype0 **pname0[]);"
        "namespace abc {"
        "  rtype2 name0(ptype0, ptype1 pnameX);"
        "}"
    );
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    ASSERT_TRUE(static_cast<bool>(global));
    ASSERT_EQ(4u, global->NestedFunctionsNum());
    /* first */ {
        Ptr<const Function> func = global->NestedFunction("name0");
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(func->name(), "name0");
        EXPECT_EQ(func->return_type(), "rtype0");
        EXPECT_EQ("ptype0", func->parameter_type(0));
        EXPECT_EQ("", func->parameter_name(0));
        EXPECT_EQ("ptype1", func->parameter_type(1));
        EXPECT_EQ("", func->parameter_name(1));
    }
    /* second */ {
        Ptr<const Function> func = global->NestedFunction("name1");
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(func->name(), "name1");
        EXPECT_EQ(func->return_type(), "rtype1");
        EXPECT_EQ("ptype0", func->parameter_type(0));
        EXPECT_EQ("pname0", func->parameter_name(0));
        EXPECT_EQ("ptype1", func->parameter_type(1));
        EXPECT_EQ("", func->parameter_name(1));
    }
    /* third */ {
        Ptr<const Function> func = global->NestedFunction("name2");
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(func->name(), "name2");
        EXPECT_EQ(func->return_type(), "rtype2");
        EXPECT_EQ("ptype0", func->parameter_type(0));
        EXPECT_EQ("pname0", func->parameter_name(0));
        EXPECT_EQ("ptype1", func->parameter_type(1));
        EXPECT_EQ("pname1", func->parameter_name(1));
    }
    /* forth */ {
        Ptr<const Function> func = global->NestedFunction("name3");
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(func->name(), "name3");
        EXPECT_EQ(func->return_type(), "rtype3");
        EXPECT_EQ("ptype0", func->parameter_type(0));
        EXPECT_EQ("pname0", func->parameter_name(0));
    }
    /* fifth */ {
        Ptr<const Namespace> abc = global->NestedNamespace("abc");
        ASSERT_TRUE(static_cast<bool>(abc));
        Ptr<const Function> func = abc->NestedFunction("name0");
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(func->name(), "name0");
        EXPECT_EQ(func->return_type(), "rtype2");
        EXPECT_EQ("ptype0", func->parameter_type(0));
        EXPECT_EQ("ptype1", func->parameter_type(1));
        EXPECT_EQ("pnameX", func->parameter_name(1));
    }
}
