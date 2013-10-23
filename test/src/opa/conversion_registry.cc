
#include <opa/type.h>
#include <gtest/gtest.h>

namespace {

class A {};
class B : public A {};

} // unnamed namespace

TEST (ConversionRegistry, DefineAndCheck) {
    opa::DefineConversion(typeid(B), typeid(A), opa::Convertibility::INHERITS);
    EXPECT_EQ(opa::Convertibility::INHERITS, opa::CheckConversion(typeid(B), typeid(A)));
}

TEST (ConversionRegistry, CheckNonExistant) {
    EXPECT_EQ(opa::Convertibility::NONE, opa::CheckConversion(typeid(B), typeid(int)));
}

