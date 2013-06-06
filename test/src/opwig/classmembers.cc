class MDClassMembersTest : public MDBaseTest {
protected:
};

TEST_F (MDClassMembersTest, SingleVariable) {
    ASSERT_EQ(RunParse("class name { type var; };"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, SingleVariableDerivedClass) {
    ASSERT_EQ(RunParse("class name : public base { type var; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 0u, 0u);
    TestClassAttributes(c, 1u, 0u, false);
    TestClassBaseByIndex(c, 0, "base", false, AccessSpecifier::PUBLIC);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, MultiVariable) {
    ASSERT_EQ(RunParse("class name { type var, var2; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 2u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
    TestVariable(c, "var2", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, SingleFunction) {
    ASSERT_EQ(RunParse("class name { rtype func(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, SinglePureFunction) {
    ASSERT_EQ(RunParse("class name { rtype func(type) = 0; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PRIVATE, true);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, ClassWithAccessSpecifier) {
    ASSERT_EQ(RunParse("class name { protected: };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    EXPECT_EQ(c->GetAccessSpecifier(), AccessSpecifier::PROTECTED);
}

TEST_F (MDClassMembersTest, SingleVarAndFunction) {
    ASSERT_EQ(RunParse("class name { type var; rtype func(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, SingleFunctionWithAccessSpecifier) {
    ASSERT_EQ(RunParse("class name { public: rtype func(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, VarAndFunctionWithMultiAccessSpecifiers) {
    ASSERT_EQ(RunParse("class name { public: rtype func(type); protected: type var; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    TestVariable(c, "var", "type", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, NestedNamespaceError) {
    EXPECT_EQ(RunParse("class name { namespace erro {} };"), 1);
}

TEST_F (MDClassMembersTest, NamesConflict) {
    RunParseThrow("class name { type var; rtype var(type); };");
}


TEST_F (MDClassMembersTest, SingleConstructor) {
    ASSERT_EQ(RunParse("class name { name(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}


TEST_F (MDClassMembersTest, MultiConstructor) {
    ASSERT_EQ(RunParse("class name { name(); name(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionNoParameters(ctor);
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor2, 0, "", "type");
}

TEST_F (MDClassMembersTest, MultiConstructorWithAccess) {
    ASSERT_EQ(RunParse("class name { public: name(type); protected: name(); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "type");
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionNoParameters(ctor2);
}


TEST_F (MDClassMembersTest, SingleDestructor) {
    ASSERT_EQ(RunParse("class name { ~name(); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, false, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}


TEST_F (MDClassMembersTest, VirtualDestructor) {
    ASSERT_EQ(RunParse("class name { virtual ~name(); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, true, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}

TEST_F (MDClassMembersTest, ConstructorAndDestructorWithAccess) {
    ASSERT_EQ(RunParse("class name { public: ~name(); protected: name(type); };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, true);
    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
}

TEST_F (MDClassMembersTest, SingleConstructorDefinition) {
    ASSERT_EQ(RunParse("class name { name(type) {} };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}

TEST_F (MDClassMembersTest, SingleConstructorDefinitionWithInitializer) {
    ASSERT_EQ(RunParse("class name { name(type) : foo(bar), dafuq(0 = 1) {} };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}

TEST_F (MDClassMembersTest, MultiConstructorDefinition) {
    ASSERT_EQ(RunParse("class name { public: name(type) {} protected: name() {} };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "type");
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionNoParameters(ctor2);
}

TEST_F (MDClassMembersTest, SingleDestructorDefinition) {
    ASSERT_EQ(RunParse("class name { ~name() {} };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, false, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}

TEST_F (MDClassMembersTest, ConstructorAndDestructorDefinition) {
    ASSERT_EQ(RunParse("class name { public: ~name(){} protected: name(type){} name(type,type2){} };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, true);
    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
    ctor = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
    TestFunctionParameter(ctor, 1, "", "type2");
}
