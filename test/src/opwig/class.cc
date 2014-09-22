class MDClassTest : public MDBaseTest {
protected:
};

TEST_F (MDClassTest, CreateBaseSpecifier) {
    BaseSpecifier bspec ("name", true, AccessSpecifier::PUBLIC);
    EXPECT_EQ(bspec.base(), "name");
    EXPECT_TRUE(bspec.is_virtual());
    EXPECT_EQ(bspec.access_specifier(), AccessSpecifier::PUBLIC);
}

TEST_F (MDClassTest, CreateClass) {
    list<BaseSpecifier> bspecs;
    bspecs.push_back(BaseSpecifier("bname", true, AccessSpecifier::PUBLIC));
    Ptr<Class> c = Class::Create("cname", bspecs);
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "cname");
    EXPECT_EQ(c->base_specifiers().front().base(), "bname");
    EXPECT_TRUE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PUBLIC);   
}

TEST_F (MDClassTest, NamedClass) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "base_class": [],
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
}

TEST_F (MDClassTest, DerivedNamedClass) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "base_class": [{ "access": "protected", "name": "base" }],
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 1u, 0u, false);
    TestClassBaseByIndex(c, 0, ("base"), false, AccessSpecifier::PROTECTED);
}

TEST_F (MDClassTest, MultipleDerivedNamedClass) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "base_class": [
                { "access": "private", "name": "base1" },
                { "access": "public", "name": "base2" },
                { "access": "protected", "name": "base3" },
                { "access": "public", "name": "base4" }
            ],
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 4u, 0u, false);

    TestClassBaseByIndex(c, 0, ("base1"), false, AccessSpecifier::PRIVATE);
    TestClassBaseByIndex(c, 1, ("base2"), false, AccessSpecifier::PUBLIC);
    TestClassBaseByIndex(c, 2, ("base3"), false, AccessSpecifier::PROTECTED);
    TestClassBaseByIndex(c, 3, ("base4"), false, AccessSpecifier::PUBLIC);
}
 
TEST_F (MDClassTest, ClassInNamespace) {
    auto json = R"({
    "namespaces": [ "abc" ],
    "classes": [
        {
            "name": "name",
            "qualified_name": "abc::name",
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    
    auto c = TestClass(abc, "name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
}
 
TEST_F (MDClassTest, ClassInAndOutOfNamespace) {
    auto json = R"({
    "namespaces": [ "abc" ],
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "methods": []
        },
        {
            "name": "name",
            "qualified_name": "abc::name",
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);

    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    auto c2 = TestClass(abc, "name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassAttributes(c2, 0u, 0u, false);
}


TEST_F (MDClassTest, ClassInClass) {
    auto json = R"({
    "classes": [
        {
            "name": "abc",
            "qualified_name": "abc",
            "methods": []
        },
        {
            "name": "name",
            "qualified_name": "abc::name",
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto abc = TestClass("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u);
    TestClassAttributes(abc, 0u, 0u, false);
    
    auto c = TestClass(abc, "name", AccessSpecifier::PRIVATE, 0u, 0u, 0u);
    TestClassAttributes(c, 0u, 0u, false);
}

TEST_F (MDClassTest, ChildClassWithSameName) {
    ASSERT_EQ(RunParse("class name { class name {}; };"), 1);
}

TEST_F (MDClassTest, ClassInClassInClass) {
    auto json = R"({
    "classes": [
        {
            "name": "name",
            "qualified_name": "name",
            "methods": []
        },
        {
            "name": "middle",
            "qualified_name": "name::middle",
            "methods": []
        },
        {
            "name": "name",
            "qualified_name": "name::middle::name",
            "methods": []
        }
    ]})";
    ASSERT_EQ(RunParse(json), 0);
    
    auto c1 = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 1u);
    TestClassAttributes(c1, 0u, 0u, false);

    auto c2 = TestClass(c1, "middle", AccessSpecifier::PRIVATE, 0u, 0u, 1u);
    TestClassAttributes(c2, 0u, 0u, false);

    auto c3 = TestClass(c2, "name", AccessSpecifier::PRIVATE, 0u, 0u, 0u);
    TestClassAttributes(c3, 0u, 0u, false);
}

TEST_F (MDClassTest, ComplexClassInClass) {
    ASSERT_EQ(RunParse("class name { public: name* func() {} private: class name2 { name* func(type); }; };"), 0);
    
    auto c1 = TestClass("name", AccessSpecifier::PUBLIC, 0u, 1u, 1u);
    TestClassAttributes(c1, 0u, 0u, false);

    auto f = TestFunction(c1, "func()", "func", "name *", AccessSpecifier::PUBLIC, false);
    TestFunctionNoParameters(f);
    
    auto c2 = TestClass(c1, "name2", AccessSpecifier::PRIVATE, 0u, 1u, 0u);
    TestClassAttributes(c2, 0u, 0u, false);

    auto f2 = TestFunction(c2, "func(type)", "func", "name *", AccessSpecifier::PRIVATE, false);
    TestFunctionParameter(f2, 0, "", "type");
}

TEST_F (MDClassTest, ClassInClassWithConstructors) {
    ASSERT_EQ(RunParse("class name { public: name() {} private: class name2 { name2(type); name* name(); }; };"), 0);
    
    auto c1 = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 1u);
    TestClassAttributes(c1, 0u, 1u, false);
    auto ctor = TestClassConstructorByIndex(c1, 0, AccessSpecifier::PUBLIC);
    TestFunctionNoParameters(ctor);
    //TestFunctionParameter(ctor, 0, "", "type");

    auto c2 = TestClass(c1, "name2", AccessSpecifier::PRIVATE, 0u, 1u, 0u);
    TestClassAttributes(c2, 0u, 1u, false);

    auto ctor2 =  TestClassConstructorByIndex(c2, 0, AccessSpecifier::PRIVATE);
    TestFunctionParameter(ctor2, 0, "", "type");

    auto f2 = TestFunction(c2, "name()", "name", "name *", AccessSpecifier::PRIVATE, false);
    TestFunctionNoParameters(f2);
}

/*
parseando:
-class { }
-class : Base {}
-class : public Base {}
-class : virtual base1, public base2 {}
-class {} class {}
-namespace { class {} }
*/
