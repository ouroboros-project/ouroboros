
namespace opwig {
namespace md {
namespace testing {

class MDMock {
  public:
    MDMock (int x, const string& the_name) :
        x_(x), name_(the_name) {}
    void set_access (AccessSpecifier the_access) {}
    bool operator == (const MDMock& rhs) const { return x_ == rhs.x_ && name_ == rhs.name_; }
    string id () const { return name_; }
    string name () const { return name_; }
  private:
    int     x_;
    string  name_;
};

class ContainerTest : public ::testing::Test {

  protected:

    Container<MDMock> container0_, container1_, container2_;

    ContainerTest () {}

    void SetUp () {
        container1_.Add(Ptr<MDMock>(new MDMock(1, "a")));
        container2_.Add(Ptr<MDMock>(new MDMock(2, "b")));
        container2_.Add(Ptr<MDMock>(new MDMock(3, "c")));
    }

};

TEST_F (ContainerTest, IterateEmpty) {
    for (auto entry : container0_.Iterate())
        EXPECT_TRUE(false);
}

TEST_F (ContainerTest, IterateOne) {
    for (auto entry : container1_.Iterate()) {
        EXPECT_EQ("a", entry.first);
        EXPECT_EQ(MDMock(1, "a"), *entry.second);
    }
}

TEST_F (ContainerTest, IterateTwo) {
    int i = 0;
    for (auto entry : container2_.Iterate()) {
        if (i==0) {
            EXPECT_EQ("b", entry.first);
            EXPECT_EQ(MDMock(2, "b"), *entry.second);
        } else {
            EXPECT_EQ("c", entry.first);
            EXPECT_EQ(MDMock(3, "c"), *entry.second);
        }
        ++i;
    }
}

} // namespace testing
} // namespace md
} // namespace opwig

