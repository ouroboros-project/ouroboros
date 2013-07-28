
class DeclSpecifierTest : public ::testing::Test {

  protected:

    DeclSpecifier nonvirtualnontyped_, nonvirtualtyped_, virtualnontyped_;

    DeclSpecifierTest () :
        nonvirtualnontyped_(DeclSpecifier::EMPTY()),
        nonvirtualtyped_(DeclSpecifier::TYPE(NestedNameSpecifier("type"))),
        virtualnontyped_(DeclSpecifier::VIRTUAL()) {}

};

TEST_F (DeclSpecifierTest, Constructor) {
    EXPECT_EQ("", nonvirtualnontyped_.type().ToString());
    EXPECT_FALSE(nonvirtualnontyped_.is_virtual());
    EXPECT_EQ("type", nonvirtualtyped_.type().ToString());
    EXPECT_FALSE(nonvirtualtyped_.is_virtual());
    EXPECT_EQ("", virtualnontyped_.type().ToString());
    EXPECT_TRUE(virtualnontyped_.is_virtual());
}

TEST_F (DeclSpecifierTest, JoinTypeAndVirtualSpecs) {
    DeclSpecifier joined = DeclSpecifier::Join(nonvirtualtyped_, virtualnontyped_);
    EXPECT_EQ("type", joined.type().ToString());
    EXPECT_TRUE(joined.is_virtual());
}

TEST_F (DeclSpecifierTest, FailJoinDoubleVirtual) {
    EXPECT_THROW(DeclSpecifier::Join(virtualnontyped_, virtualnontyped_), std::exception);
}

TEST_F (DeclSpecifierTest, FailJoinDoubleType) {
    EXPECT_THROW(DeclSpecifier::Join(nonvirtualtyped_, nonvirtualtyped_), std::exception);
}

