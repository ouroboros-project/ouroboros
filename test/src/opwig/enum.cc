class MDEnumTest : public MDBaseTest {
protected:
};

TEST_F (MDEnumTest, SimpleEnum) {
    auto json = R"({
    "enums": [
        {
            "access": "public",
            "name": "name",
            "qualified_name": "name",
            "scoped": false,
            "values": [
                "Sup"
            ]
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("enum name {Sup};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 0u, 1u);

    vector<string> values = {"Sup"};
    TestEnum("name", AccessSpecifier::PUBLIC, "", values);
}

/* TODO: Anonymous enum are OK
TEST_F (MDEnumTest, AnonymousEnum) {
    auto json = R"({
})";
    ASSERT_EQ(RunParse(json), 0);
    //RunParseThrow("enum {Sup, Dude};");
}
*/

TEST_F (MDEnumTest, SimpleEnumWithExpressions) {
    auto json = R"({
    "enums": [
        {
            "access": "public",
            "name": "name",
            "qualified_name": "name",
            "scoped": false,
            "values": [
                "Sup",
                "Dude"
            ]
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("enum name {Sup = 0x123, Dude};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 0u, 1u);

    vector<string> values = {"Sup", "Dude"};
    TestEnum("name", AccessSpecifier::PUBLIC, "", values);
}

TEST_F (MDEnumTest, EnumWithBases) {
    auto json = R"({
    "enums": [
        {
            "access": "public",
            "name": "name",
            "qualified_name": "name",
            "scoped": false,
            "values": [
                "Sup",
                "Dude"
            ]
        }
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("enum name : char {Sup, Dude};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 0u, 1u);

    vector<string> values = {"Sup", "Dude"};
    TestEnum("name", AccessSpecifier::PUBLIC, "char", values);
}

/* Dunno.
TEST_F (MDEnumTest, EnumWithErroneousBase) {
    EXPECT_EQ(RunParse("enum name : wat {Sup, Dude};"), 1);
    EXPECT_EQ(RunParse("enum name : void {Sup, Dude};"), 1);
    EXPECT_EQ(RunParse("enum name : foo::bar {Sup, Dude};"), 1);
    EXPECT_EQ(RunParse("enum name : ::foo::bar {Sup, Dude};"), 1);
}
*/

TEST_F (MDEnumTest, EnumInNamespace) {
    auto json = R"({
    "enums": [
        {
            "access": "public",
            "name": "name",
            "qualified_name": "abc::name",
            "scoped": false,
            "values": [
                "Sup",
                "Dude"
            ]
        }
    ],
    "namespaces": [
        "abc"
    ]
})";
    ASSERT_EQ(RunParse(json), 0);
    //ASSERT_EQ(RunParse("namespace abc { enum name {Sup, Dude}; }"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u, 0u);

    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u, 1u);
    vector<string> values = {"Sup", "Dude"};
    TestEnum(abc, "name", AccessSpecifier::PUBLIC, "", values);
}

/* Redundant
TEST_F (MDEnumTest, EnumOutsideNamespace) {
    ASSERT_EQ(RunParse("namespace abc { } enum abc::name {Sup, Dude};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u, 0u);

    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u, 1u);
    vector<string> values = {"Sup", "Dude"};
    TestEnum(abc, "name", AccessSpecifier::PUBLIC, "", values);
}

TEST_F (MDEnumTest, EnumInAndOutsideNamespace) {
    ASSERT_EQ(RunParse("namespace abc { enum class name; } enum abc::name {Sup, Dude};"), 0);
    TestScopeChildNums(global_, 0u, 0u, 0u, 1u, 0u);

    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 0u, 0u, 1u);
    vector<string> values = {"Sup", "Dude"};
    TestEnum(abc, "name", AccessSpecifier::PUBLIC, "", values);
}
*/