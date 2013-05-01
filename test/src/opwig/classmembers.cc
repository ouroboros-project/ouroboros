
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
    EXPECT_EQ(var->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(var->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(var->access(), AccessSpecifier::PRIVATE);
    Ptr<const Variable> var2 = c->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type");
    EXPECT_EQ(var2->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(var->access(), AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
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
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
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
    EXPECT_EQ(var->access(), AccessSpecifier::PROTECTED);
    
    ASSERT_EQ(1u, c->NestedFunctionsNum());
    Ptr<const Function> func = c->NestedFunction("func");
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "func");
    EXPECT_EQ(func->return_type(), "rtype");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
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


TEST (MDParserClassMembersTest, SingleConstructor) {
    istringstream input("class name { name(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    ASSERT_FALSE(static_cast<bool>(c->destructor()));
    
    ASSERT_EQ(1u, c->constructors().size());
    Ptr<const Function> func = c->constructors().front();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
}


TEST (MDParserClassMembersTest, MultiConstructor) {
    istringstream input("class name { name(); name(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    ASSERT_FALSE(static_cast<bool>(c->destructor()));
    
    ASSERT_EQ(2u, c->constructors().size());
    Ptr<const Function> func = c->constructors().front();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
    
    func = c->constructors().back();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
}

TEST (MDParserClassMembersTest, MultiConstructorWithAccess) {
    istringstream input("class name { public: name(type); protected: name(); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    ASSERT_FALSE(static_cast<bool>(c->destructor()));
    
    ASSERT_EQ(2u, c->constructors().size());
    Ptr<const Function> func = c->constructors().front();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
    
    func = c->constructors().back();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PROTECTED);
}


TEST (MDParserClassMembersTest, SingleDestructor) {
    istringstream input("class name { ~name(); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    Ptr<const Function> func = c->destructor();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "~name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}


TEST (MDParserClassMembersTest, VirtualDestructor) {
    istringstream input("class name { virtual ~name(); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    Ptr<const Function> func = c->destructor();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "~name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    // TODO check if it is virtual?
    EXPECT_EQ(func->access(), AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}

TEST (MDParserClassMembersTest, ConstructorAndDestructorWithAccess) {
    istringstream input("class name { public: ~name(); protected: name(type); };");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    ASSERT_EQ(0u, c->NestedFunctionsNum());
    ASSERT_EQ(0u, c->NestedClassesNum());
    ASSERT_EQ(0u, c->GlobalVariablesNum());
    
    Ptr<const Function> func = c->destructor();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "~name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
    
    ASSERT_EQ(1u, c->constructors().size());
    func = c->constructors().front();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_EQ("type", func->parameter_type(0));
    EXPECT_EQ("", func->parameter_name(0));
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PROTECTED);
}

TEST (MDParserClassMembersTest, SimpleClass) {
    istringstream input("class name { public: name(); ~name(); rtype func(type); protected: rtype var; };");
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
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
    
    ASSERT_EQ(0u, c->NestedClassesNum());
    
    ASSERT_EQ(1u, c->GlobalVariablesNum());
    Ptr<const Variable> var = c->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "rtype");
    EXPECT_EQ(var->access(), AccessSpecifier::PROTECTED);
    
    func = c->destructor();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "~name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
    
    ASSERT_EQ(1u, c->constructors().size());
    func = c->constructors().front();
    ASSERT_TRUE(static_cast<bool>(func));
    EXPECT_EQ(func->name(), "name");
    EXPECT_EQ(func->return_type(), "");
    EXPECT_THROW(func->parameter_type(0), std::out_of_range);
    EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    EXPECT_FALSE(func->is_pure());
    EXPECT_EQ(func->access(), AccessSpecifier::PUBLIC);
}
