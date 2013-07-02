
class DeclSpecifierTest : public ::testing::Test {
  protected:
    DeclSpecifier CreateNonVirtual () {
        return DeclSpecifier(NestedNameSpecifier("name"), false);
    }
};

TEST_F (DeclSpecifierTest, Constructor) {
    DeclSpecifier declspecifier = CreateNonVirtual();
}

