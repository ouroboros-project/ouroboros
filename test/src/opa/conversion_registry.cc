
#include <opa/type/conversionregistry.h>
#include <gtest/gtest.h>

namespace opa {
namespace type {
namespace testing {

namespace {

class A {};
class B : public A {};
class C : public A {};
class D : public B {};
class E {};
class F : public C, public E {};

} // unnamed namespace

class ConversionRegistryTest : public ::testing::Test {

  protected:

    ConversionRegistry  registry_;
    HeritageTable       table_;

    virtual void SetUp () {}

    void BuildSmallTable () {
        table_.clear();
        table_.insert({ typeid(B), { typeid(A) } });
    }

    void BuildBigTable () {
        table_.clear();
        table_.insert({ typeid(B), { typeid(A) } });
        table_.insert({ typeid(C), { typeid(A) } });
        table_.insert({ typeid(D), { typeid(B), typeid(A) } });
        table_.insert({ typeid(F), { typeid(C), typeid(E), typeid(A) } });
    }
    
    template <typename T, typename S>
    void CheckTwoWayConversion () {
        EXPECT_EQ(Convertibility::INHERITS, registry_.CheckConversion(typeid(S), typeid(T)));
        EXPECT_EQ(Convertibility::IS_BASE, registry_.CheckConversion(typeid(T), typeid(S)));
    }
    
    template <typename T, typename S>
    void CheckTwoWayNonConversion () {
        EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(S), typeid(T)));
        EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(T), typeid(S)));
    }

};

TEST_F (ConversionRegistryTest, DefineAndCheck) {
    registry_.DefineConversion(typeid(B), typeid(A), Convertibility::INHERITS);
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::INHERITS, registry_.CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckNonExistant) {
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckEmptyTable) {
    registry_.DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckSmallTable) {
    BuildSmallTable();
    registry_.DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(int)));
    CheckTwoWayConversion<A,B>();
    CheckTwoWayNonConversion<A,E>();
    CheckTwoWayNonConversion<F,B>();
    CheckTwoWayNonConversion<C,D>();
}

TEST_F (ConversionRegistryTest, CheckBigTable) {
    BuildBigTable();
    registry_.DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, registry_.CheckConversion(typeid(B), typeid(int)));
    CheckTwoWayConversion<A,B>();
    CheckTwoWayConversion<A,C>();
    CheckTwoWayConversion<A,D>();
    CheckTwoWayConversion<A,F>();
    CheckTwoWayConversion<B,D>();
    CheckTwoWayConversion<C,F>();
    CheckTwoWayConversion<E,F>();
    CheckTwoWayNonConversion<A,E>();
    CheckTwoWayNonConversion<F,B>();
    CheckTwoWayNonConversion<C,D>();
}

} // namespace testing
} // namespace type
} // namespace opa

