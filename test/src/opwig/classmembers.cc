
TEST (MDParserClassMembersTest, SingleVariable) {
    istringstream input("class name { type var; };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, SingleVariableDerivedClass) {
    istringstream input("class name : public base { type var; };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 1);
    
    EXPECT_EQ(c->base_specifiers().front().name(), "base");
    EXPECT_FALSE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PUBLIC);   
    
    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, MultiVariable) {
    istringstream input("class name { type var, var2; };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);

    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var->name()), AccessSpecifier::PRIVATE);
    Ptr<const Variable> var2 = c->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var2->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, SingleFunction) {
    istringstream input("class name { rtype func(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);

    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(c->GetAccessSpecifierForName(func->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, SinglePureFunction) {
    istringstream input("class name { rtype func(type) = 0; };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);

    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_TRUE(func->is_pure());
    EXPECT_EQ(c->GetAccessSpecifierForName(func->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, ClassWithAccessSpecifier) {
    istringstream input("class name { protected: };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);

    EXPECT_EQ(c->GetAccessSpecifier(), AccessSpecifier::PROTECTED);
}

TEST (MDParserClassMembersTest, SingleVarAndFunction) {
    istringstream input("class name { type var; rtype func(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);

    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var->name()), AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(c->GetAccessSpecifierForName(func->name()), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, SingleFunctionWithAccessSpecifier) {
    istringstream input("class name { public: rtype func(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(c->GetAccessSpecifierForName(func->name()), AccessSpecifier::PUBLIC);
}

TEST (MDParserClassMembersTest, VarAndFunctionWithMultiAccessSpecifiers) {
    istringstream input("class name { public: rtype func(type); protected: type var; };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
    EXPECT_EQ(c->GetAccessSpecifierForName(var->name()), AccessSpecifier::PROTECTED);
    
    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(c->GetAccessSpecifierForName(func->name()), AccessSpecifier::PUBLIC); ///WRONG
}

TEST (MDParserClassMembersTest, NestedNamespaceError) {
    istringstream input("class name { namespace erro {} };");
    MDParser parser(input);
    EXPECT_EQ(parser.parse(), 1);
}

TEST (MDParserClassMembersTest, NamesConflict) {
    istringstream input("class name { type var; rtype var(type); };");
    MDParser parser(input);
    EXPECT_THROW(parser.parse(), std::exception);
}
