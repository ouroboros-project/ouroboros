class MDClassMembersTest : public MDBaseTest {
protected:
};

TEST_F (MDClassMembersTest, SingleVariable) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "base_class": [],
            "methods": []
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "private",
            "type": "type"
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { type var; };"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, SingleVariableDerivedClass) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "base_class": [{ "access": "public", "name": "base" }],
            "methods": []
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "private",
            "type": "type"
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name : public base { type var; };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 0u, 0u);
    TestClassAttributes(c, 1u, 0u, false);
    TestClassBaseByIndex(c, 0, ("base"), false, AccessSpecifier::PUBLIC);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, MultiVariable) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "base_class": [],
            "methods": []
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "private",
            "type": "type"
        },
        {
            "name": "var2",
            "qualified_name": "name::var2",
            "access": "private",
            "type": "type"
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { type var, var2; };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 2u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
    TestVariable(c, "var2", "type", AccessSpecifier::PRIVATE);
}

TEST_F (MDClassMembersTest, SingleFunction) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": [
                {
                    "name": "func",
                    "qualified_name": "name::func",
                    "access": "private",
                    "params": [ "type" ],
                    "return": "rtype",
                    "virtual": false,
                    "const": false,
                    "pure": false,
                    "deleted": false
                }
            ]
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { rtype func(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func(type)", "func", "rtype", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, SinglePureFunction) {
    auto json = R"({
    "classes": [
        {
            "access": "public",
            "base_class": [],
            "methods": [
                {
                    "access": "private",
                    "const": false,
                    "deleted": false,
                    "name": "func",
                    "params": [ "type" ],
                    "pure": true,
                    "qualified_name": "name::func",
                    "return": "rtype",
                    "const": false,
                    "virtual": true
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { virtual rtype func(type) = 0; };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func(type)", "func", "rtype", AccessSpecifier::PRIVATE, true, true);
    TestFunctionParameter(f, 0, "", "type");
}

/* This is testing an implementation detail...
TEST_F (MDClassMembersTest, ClassWithAccessSpecifier) {
    ASSERT_EQ(RunParse("class name { protected: };"), 0);
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    EXPECT_EQ(c->GetAccessSpecifier(), AccessSpecifier::PROTECTED);
}
*/

TEST_F (MDClassMembersTest, SingleVarAndFunction) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": [
                {
                    "name": "func",
                    "qualified_name": "name::func",
                    "access": "private",
                    "params": [ "type" ],
                    "return": "rtype",
                    "virtual": false,
                    "const": false,
                    "pure": false,
                    "deleted": false
                }
            ]
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "private",
            "type": "type"
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { type var; rtype func(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    TestVariable(c, "var", "type", AccessSpecifier::PRIVATE);
    auto f = TestFunction(c, "func(type)", "func", "rtype", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, SingleFunctionWithAccessSpecifier) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": [
                {
                    "name": "func",
                    "qualified_name": "name::func",
                    "access": "public",
                    "params": [ "type" ],
                    "return": "rtype",
                    "virtual": false,
                    "const": false,
                    "pure": false,
                    "deleted": false
                }
            ]
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: rtype func(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    auto f = TestFunction(c, "func(type)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, VarAndFunctionWithMultiAccessSpecifiers) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": [
                {
                    "name": "func",
                    "qualified_name": "name::func",
                    "access": "public",
                    "params": [ "type" ],
                    "return": "rtype",
                    "virtual": false,
                    "pure": false,
                    "const": false,
                    "deleted": false
                }
            ]
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "protected",
            "type": "type"
        }
    ]
    })";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: rtype func(type); protected: type var; };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 1u, 1u, 0u);
    TestClassAttributes(c, 0u, 0u, false);

    TestVariable(c, "var", "type", AccessSpecifier::PROTECTED);
    auto f = TestFunction(c, "func(type)", "func", "rtype", AccessSpecifier::PUBLIC, false);
    TestFunctionParameter(f, 0, "", "type");
}

TEST_F (MDClassMembersTest, NestedNamespaceError) {
    auto json = R"({
    "namespaces": [ "name::erro" ],
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": []
        }
    ]
    })";
    RunParseThrow(json);
    //EXPECT_EQ(RunParse("class name { namespace erro {} };"), 1);
}

TEST_F (MDClassMembersTest, NamesConflict) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "access": "public",
            "methods": [
                {
                    "name": "var",
                    "qualified_name": "name::var",
                    "access": "private",
                    "params": [ "type" ],
                    "return": "rtype",
                    "virtual": false,
                    "const": false,
                    "pure": false,
                    "deleted": false
                }
            ]
        }
    ],
    "variables": [
        {
            "name": "var",
            "qualified_name": "name::var",
            "access": "private",
            "type": "type"
        }
    ]
    })";
    RunParseThrow(json);
    //RunParseThrow("class name { type var; rtype var(type); };");
}


TEST_F (MDClassMembersTest, SingleConstructor) {
    auto json = R"({
    "classes": [
        {
            "access": "public",
            "base_class": [],
            "methods": [
                {
                    "access": "private",
                    "const": false,
                    "deleted": false,
                    "name": "name",
                    "params": [ "type" ],
                    "pure": false,
                    "qualified_name": "name::name",
                    "virtual": false
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { name(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}


TEST_F (MDClassMembersTest, MultiConstructor) {
    auto json = R"({
    "classes": [
    {
        "access": "public",
            "base_class" : [],
            "methods" : [
        {
            "access": "private",
                "const" : false,
                "deleted" : false,
                "name" : "name",
                "params" : [],
                "pure" : false,
                "qualified_name" : "name::name",
                "virtual" : false
        },
        {
            "access": "private",
            "const" : false,
            "deleted" : false,
            "name" : "name",
            "params" : [ "type" ],
            "pure" : false,
            "qualified_name" : "name::name",
            "virtual" : false
        }
            ],
            "name": "name",
            "qualified_name" : "name"
    }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { name(); name(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionNoParameters(ctor);
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor2, 0, "", "type");
}

TEST_F (MDClassMembersTest, MultiConstructorWithAccess) {
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
                    "params": [ "type" ],
                    "pure": false,
                    "qualified_name": "name::name",
                    "virtual": false
                },
                {
                    "access": "protected",
                    "const": false,
                    "deleted": false,
                    "name": "name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::name",
                    "virtual": false
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: name(type); protected: name(); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "type");
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionNoParameters(ctor2);
}


TEST_F (MDClassMembersTest, SingleDestructor) {
    auto json = R"({
    "classes": [
        {
            "access": "public",
            "base_class": [],
            "methods": [
                {
                    "access": "private",
                    "const": false,
                    "deleted": false,
                    "name": "~name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::~name",
                    "virtual": false
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { ~name(); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, false, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}


TEST_F (MDClassMembersTest, VirtualDestructor) {
    auto json = R"({
    "classes": [
        {
            "access": "public",
            "base_class": [],
            "methods": [
                {
                    "access": "private",
                    "const": false,
                    "deleted": false,
                    "name": "~name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::~name",
                    "virtual": true
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { virtual ~name(); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, true, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}

TEST_F (MDClassMembersTest, ConstructorAndDestructorWithAccess) {
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
                    "name": "~name",
                    "params": [],
                    "pure": false,
                    "qualified_name": "name::~name",
                    "virtual": false
                },
                {
                    "access": "protected",
                    "const": false,
                    "deleted": false,
                    "name": "name",
                    "params": [ "type" ],
                    "pure": false,
                    "qualified_name": "name::name",
                    "virtual": false
                }
            ],
            "name": "name",
            "qualified_name": "name"
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: ~name(); protected: name(type); };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, true);
    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
}

/* Definition tests...
TEST_F (MDClassMembersTest, SingleConstructorDefinition) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { name(type) {} };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}

TEST_F (MDClassMembersTest, SingleConstructorDefinitionWithInitializer) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { name(type) : foo(bar), dafuq(0 = 1) {} };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor, 0, "", "type");
}

TEST_F (MDClassMembersTest, MultiConstructorDefinition) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: name(type) {} protected: name() {} };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, false);
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PUBLIC);
    TestFunctionParameter(ctor, 0, "", "type");
    auto ctor2 = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionNoParameters(ctor2);
}

TEST_F (MDClassMembersTest, SingleDestructorDefinition) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { ~name() {} };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, true);
    TestClassDestructor(c, false, AccessSpecifier::PRIVATE);
    
    ASSERT_EQ(0u, c->constructors().size());
}

TEST_F (MDClassMembersTest, ConstructorAndDestructorDefinition) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("class name { public: ~name(){} protected: name(type){} name(type,type2){} };"), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 2u, true);
    TestClassDestructor(c, false, AccessSpecifier::PUBLIC);
    
    auto ctor = TestClassConstructorByIndex(c, 0, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
    ctor = TestClassConstructorByIndex(c, 1, AccessSpecifier::PROTECTED);
    TestFunctionParameter(ctor, 0, "", "type");
    TestFunctionParameter(ctor, 1, "", "type2");
}
*/
