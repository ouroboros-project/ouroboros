
class DeclSpecifierTest : public ::testing::Test {

  protected:

    DeclSpecifier empty_, typed_, virtual_, const_;

    DeclSpecifierTest () :
        empty_(DeclSpecifier::EMPTY()),
        typed_(DeclSpecifier::TYPE(NestedNameSpecifier("type"))),
        virtual_(DeclSpecifier::VIRTUAL()),
        const_(DeclSpecifier::CONST()) {}

    void Check (const DeclSpecifier& which, const std::string& type, bool is_virtual,
                bool is_const) {
      EXPECT_EQ(type, which.type_specifier().ToString());
      EXPECT_EQ(is_virtual, which.is_virtual());
      EXPECT_EQ(is_const, which.is_const());
    }

};

TEST_F (DeclSpecifierTest, Constructor) {
    Check(empty_, "", false, false);
    Check(typed_, "type", false, false);
    Check(virtual_, "", true, false);
    Check(const_, "", false, true);
}

TEST_F (DeclSpecifierTest, FullType) {
    EXPECT_EQ("type", typed_.type()->full_type());
    EXPECT_EQ("const type", (DeclSpecifier::Join(typed_, const_).type()->full_type()));
    EXPECT_FALSE(const_.type());
}

TEST_F (DeclSpecifierTest, JoinTypeAndVirtualSpecs) {
    DeclSpecifier joined = DeclSpecifier::Join(typed_, virtual_);
    Check(joined, "type", true, false);
}

TEST_F (DeclSpecifierTest, JoinTypeAndConstSpecs) {
    DeclSpecifier joined = DeclSpecifier::Join(typed_, const_);
    Check(joined, "type", false, true);
}

TEST_F (DeclSpecifierTest, JoinVirtualAndConstSpecs) {
    DeclSpecifier joined = DeclSpecifier::Join(virtual_, const_);
    Check(joined, "", true, true);
}

TEST_F (DeclSpecifierTest, JoinTypeAndVirtualAndConstSpecs) {
    DeclSpecifier joined = DeclSpecifier::Join(DeclSpecifier::Join(typed_, virtual_), const_);
    Check(joined, "type", true, true);
}

TEST_F (DeclSpecifierTest, FailJoinDoubleVirtual) {
    EXPECT_THROW(DeclSpecifier::Join(virtual_, virtual_), std::exception);
}

TEST_F (DeclSpecifierTest, FailJoinDoubleConst) {
    EXPECT_THROW(DeclSpecifier::Join(const_, const_), std::exception);
}

TEST_F (DeclSpecifierTest, FailJoinDoubleType) {
    EXPECT_THROW(DeclSpecifier::Join(typed_, typed_), std::exception);
}

