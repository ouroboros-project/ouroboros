

TEST (DeclaratorTest, ConstructorGetSetName) {
    Declarator declarator(NestedNameSpecifier("name"));
    ASSERT_EQ("name", declarator.name());
    declarator.set_nested_name(NestedNameSpecifier("another_name"));
    ASSERT_EQ("another_name", declarator.name());
}

TEST (DeclaratorTest, ParametersInformation) {
    using std::equal;
    Declarator declarator(NestedNameSpecifier("name"));
    EXPECT_FALSE(declarator.has_parameters());
    /* Empty parameter list */ {
        ParameterList parameters = {};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
    /* Single parameter list */ {
        ParameterList parameters = {{Type::Create("type1",false), "name1"}};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
    /* Multiple parameter list */ {
        auto p1 = Parameter( Type::Create("type1", false), "name1" ),
            p2 = Parameter( Type::Create("type2", false), "name2" ),
            p3 = Parameter( Type::Create("type3", false), "name3" );
        ParameterList parameters = { p1, p2, p3 };
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
}
