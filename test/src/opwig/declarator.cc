

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
        ParameterList parameters = {{"type1", "name1"}};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
    /* Multiple parameter list */ {
        ParameterList parameters = {{"type1", "name1"}, {"type2", "name2"}, {"type3", "name3"}};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
}
