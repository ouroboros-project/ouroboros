class MDComplexClassesTest : public MDBaseTest {
protected:
};


TEST_F (MDComplexClassesTest, SimpleClass) {
    ASSERT_EQ(RunParse("class name { public: name(); ~name(); rtype func(type); protected: rtype var; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassBaseNum(c, 0u);
    
    TestVariable(c, "var", "rtype", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");

    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    TestClassConstructorNum(c, 1u);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionNoParameters(ctor);
}

TEST_F (MDComplexClassesTest, SimpleClassWithFuncDefinitions) {
    ASSERT_EQ(RunParse("class name { public: name(){} ~name(){} rtype func(type) { } protected: rtype var; };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassBaseNum(c, 0u);
    
    TestVariable(c, "var", "rtype", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");

    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    TestClassConstructorNum(c, 1u);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionNoParameters(ctor);
}

TEST_F (MDComplexClassesTest, StandardClassDefinition) {
    string ctorDtor = "virtual ~name(){} name(int,double arg2) : stuff(arg0), wat(arg2) {} ";
    string methods = "char func(); void func2(type arg); ";
    string classCode = "class name : public base { public: "+ctorDtor+methods+" protected: type var; };";
    string classDefs = "char name::func() {} void name::func2(type arg) {} ";
    string code = "namespace abc { "+classCode+classDefs+" } ";
    ASSERT_EQ(RunParse(code), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    auto c = TestClass(abc, "name", AccessSpecifier::PUBLIC, 1u, 2u, 0u);
    TestClassBaseNum(c, 1u);
    TestClassBaseByIndex(c, 0, "base", false, AccessSpecifier::PUBLIC);
    TestClassDestructor(c, true, AccessSpecifier::PUBLIC);
    TestClassConstructorNum(c, 1u);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "int");
    TestFunctionParameter(ctor, 1, "arg2", "double");
    
    auto f = TestFunction(c, "func", "char", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
    f = TestFunction(c, "func2", "void", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg", "type");
    
    TestVariable(c, "var", "type", AccessSpecifier::PROTECTED);
}

