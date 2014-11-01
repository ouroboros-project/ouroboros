class MDComplexClassesTest : public MDBaseTest {
protected:
};


TEST_F (MDComplexClassesTest, SimpleClass) {
    auto json = R"({
    "classes": [
        {
            "access": "public",
            "base_class": [],
            "methods": [
                {
                    "access": "public",
                    "const": false,
                    "deleted": false,
                    "name": "name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::name",
                    "static": false,
                    "virtual": false
                },
                {
                    "access": "public",
                    "const": false,
                    "deleted": false,
                    "name": "~name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::~name",
                    "static": false,
                    "virtual": false
                },
                {
                    "access": "public",
                    "const": false,
                    "deleted": false,
                    "name": "func",
                    "params": [
                        "double"
                    ],
                    "pure": false,
                    "qualified_name": "name::func",
                    "return": "int",
                    "static": false,
                    "virtual": false
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ],
    "variables": [
        {
            "access": "protected",
            "name": "var",
            "qualified_name": "name::var",
            "static": true,
            "type": "int"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: name(); ~name(); int func(double); protected: int var; };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 1u, true);
    
    TestVariable(c, "var", "int", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func(double)", "func", "int", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "double");

    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionNoParameters(ctor);
}

/* Definition test...
TEST_F (MDComplexClassesTest, SimpleClassWithFuncDefinitions) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: name(){} ~name(){} int func(double) { } protected: int var; };"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 1u, true);
    
    TestVariable(c, "var", "int", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func(double)", "func", "int", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "double");

    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);

    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionNoParameters(ctor);
}

TEST_F (MDComplexClassesTest, StandardClassDefinition) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    
    string ctorDtor = "virtual ~name(){} name(int,double arg2) : stuff(arg0), wat(arg2) {} ";
    string methods = "char func(); void func2(bool arg); ";
    string classCode = "class name : public base { public: "+ctorDtor+methods+" protected: bool var; };";
    string classDefs = "char name::func() {} void name::func2(bool arg) {} ";
    string code = "namespace abc { "+classCode+classDefs+" } ";
    ASSERT_EQ(RunParse(code), 0);

    TestScopeChildNums(global_, 0u, 0u, 0u, 1u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    auto c = TestClass(abc, "name", AccessSpecifier::PUBLIC, 1u, 2u, 0u);
    TestClassAttributes(c, 1u, 1u, true);
    TestClassBaseByIndex(c, 0, ("base"), false, AccessSpecifier::PUBLIC);
    TestClassDestructor(c, true, AccessSpecifier::PUBLIC);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "int");
    TestFunctionParameter(ctor, 1, "arg2", "double");
    
    auto f = TestFunction(c, "func()", "func", "char", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
    f = TestFunction(c, "func2(bool)", "func2", "void", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "arg", "bool");
    
    TestVariable(c, "var", "bool", AccessSpecifier::PROTECTED);
}
*/
