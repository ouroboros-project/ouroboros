#include <opwig/md/type.h>
#include <opwig/md/variable.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>
#include <opwig/md/enum.h>
#include <opwig/md/accessspecifier.h>
#include <opwig/md/semanticerror.h>
#include <opwig/md/nestednamespecifier.h>
#include <opwig/json/reader.h>

#include "config.h"

#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>

#include <gtest/gtest.h>

#include <vector>
#include <list>

#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <functional>
#include <exception>
#include <stdexcept>

using std::vector;
using std::string;
using std::istringstream;
using std::list;
using std::function;

using opwig::md::Ptr;
using opwig::md::Type;
using opwig::md::Variable;
using opwig::md::Parameter;
using opwig::md::ParameterList;
using opwig::md::Function;
using opwig::md::Enum;
using opwig::md::Scope;
using opwig::md::Namespace;
using opwig::md::Class;
using opwig::md::AccessSpecifier;
using opwig::md::NestedNameSpecifier;
using opwig::md::BaseSpecifier;
using opwig::json::Reader;

class MDBaseTest : public ::testing::Test {
  protected:
    Ptr<const Namespace> global_;
    
    int RunParse(const string& str) {
        return RunParse(str, false);
    }
    int RunParse(const string& str, bool debug) {
        EXPECT_TRUE(false); // NYI

        Ptr<Namespace> global = Namespace::Create("");
        Reader reader(str, global);
        bool ret = reader.parse();
        global_ = global;

        return ret ? 0 : 1;
    }
    void RunParseThrow(const string& str) {
        EXPECT_TRUE(false); // NYI
        /*
        istringstream input (str);
        MDParser parser(input);
        EXPECT_THROW(parser.parse(), std::exception);
        */
    }
    
    //////////// variable
    void TestVariable(const string& name, const string& type, AccessSpecifier access) {
        TestVariable(global_, name, type, access);
    }
    void TestVariable(Ptr<const Scope> scope, const string& name, const string& type, AccessSpecifier access) {
        Ptr<const Variable> var = scope->GlobalVariable(name);
        ASSERT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(type, var->type()->full_type());
        EXPECT_EQ(access, var->access());
    }
    
    /////////// functions
    Ptr<const Function> TestFunction (const string& id, const string& name,
                                      const string& return_type, AccessSpecifier access,
                                      bool is_pure, bool is_virtual = false) {
        return TestFunction(global_, id, name, return_type, access, is_pure, is_virtual);
    }
    Ptr<const Function> TestFunction (Ptr<const Scope> scope, const string& id, const string& name,
                                      const string& return_type, AccessSpecifier access,
                                      bool is_pure, bool is_virtual = false) {
        Ptr<const Function> func = scope->NestedFunction(id);
        internalCheckFunction(func, name, return_type, access, is_pure, is_virtual);
        return func;
    }
    void internalCheckFunction (const Ptr<const Function>& func, const string& name,
                                const string& return_type, AccessSpecifier access, bool is_pure,
                                bool is_virtual = false) {
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(name, func->name());
        if (return_type.empty())
            EXPECT_FALSE(func->return_type());
        else
            EXPECT_EQ(return_type, func->return_type()->full_type());
        EXPECT_EQ(access, func->access());
        EXPECT_EQ(is_pure, func->is_pure());
        EXPECT_EQ(is_virtual, func->is_virtual());
    }
    
    void TestFunctionNoParameters(Ptr<const Function> func) {
        EXPECT_THROW(func->parameter_type(0), std::out_of_range);
        EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    }
    void TestFunctionParameter(Ptr<const Function> func, int paramIndex, const string& param_name, const string& param_type) {
        EXPECT_EQ(param_name, func->parameter_name(paramIndex));
        EXPECT_EQ(param_type, func->parameter_type(paramIndex)->full_type());
    }
    
    /////////// namespace
    Ptr<const Namespace> TestNamespace(const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses, size_t numNamespaces, size_t numEnums=0u) {
        return TestNamespace(global_, name, access, numVariables, numFunctions, numClasses, numNamespaces, numEnums);
    }
    Ptr<const Namespace> TestNamespace(Ptr<const Scope> scope, const string& name, AccessSpecifier access, 
                                       size_t numVariables, size_t numFunctions,
                                       size_t numClasses, size_t numNamespaces, size_t numEnums=0u) {
        Ptr<const Namespace> var = scope->NestedNamespace(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(access, var->access());
        TestScopeChildNums(var, numVariables, numFunctions, numClasses, numNamespaces, numEnums);
        return var;
    }
    
    void TestScopeChildNums(Ptr<const Scope> scope, size_t numVariables, size_t numFunctions,
                            size_t numClasses, size_t numNamespaces, size_t numEnums=0u) {
        EXPECT_EQ(numVariables, scope->GlobalVariablesNum());
        EXPECT_EQ(numFunctions, scope->NestedFunctionsNum());
        EXPECT_EQ(numClasses, scope->NestedClassesNum());
        EXPECT_EQ(numNamespaces, scope->NestedNamespacesNum());
        EXPECT_EQ(numEnums, scope->NestedEnumsNum());
    }
    
    ////////////// class
    Ptr<const Class> TestClass(const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses, size_t numEnums=0u) {
        return TestClass(global_, name, access, numVariables, numFunctions, numClasses, numEnums);
    }
    Ptr<const Class> TestClass(Ptr<const Scope> scope, const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses, size_t numEnums=0u) {
        Ptr<const Class> var = scope->NestedClass(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(access, var->access());
        TestScopeChildNums(var, numVariables, numFunctions, numClasses, 0u, numEnums);
        return var;
    }
    
    void TestClassAttributes(Ptr<const Class> c, size_t numBases, size_t numConstructors, bool destructorDefined) {
        ASSERT_EQ(numBases, c->base_specifiers().size());
        ASSERT_EQ(numConstructors, c->constructors().size());
        ASSERT_EQ(destructorDefined, static_cast<bool>(c->destructor()));
    }

    void TestClassBaseByIndex(Ptr<const Class> c, int baseIndex, const string& name, bool is_virtual, AccessSpecifier access) {
        auto bspec = c->base_specifiers().begin();
        for (int i = 0; i < baseIndex; i++, bspec++);

        EXPECT_EQ(c, bspec->base());
        EXPECT_EQ(is_virtual, bspec->is_virtual());
        EXPECT_EQ(access, bspec->access_specifier());   
    }
    void TestClassDestructor(Ptr<const Class> c, bool is_virtual, AccessSpecifier access) {
        Ptr<const Function> func = c->destructor();
        internalCheckFunction(func, "~"+c->name(), "", access, false, is_virtual);
        //TODO: CHECK IF IS VIRTUAL
        TestFunctionNoParameters(func);
    }
    Ptr<const Function> TestClassConstructorByIndex(Ptr<const Class> c, int ctorIndex, AccessSpecifier access) {
        Ptr<const Function> func = c->constructors()[ctorIndex];
        internalCheckFunction(func, c->name(), "", access, false);
        return func;
    }
    
    //////////// enum
    void TestEnum(const string& name, AccessSpecifier access, const string& base, const vector<string>& values) {
        TestEnum(global_, name, access, base, values);
    }
    void TestEnum(Ptr<const Scope> scope, const string& name, AccessSpecifier access, const string& base, const vector<string>& values) {
        Ptr<const Enum> var = scope->NestedEnum(name);
        ASSERT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(access, var->access());
        ASSERT_EQ(base, var->base());
        ASSERT_EQ(values.size(), var->values().size());
        EXPECT_TRUE(equal(values.begin(), values.end(), var->values().begin()));
    }
};

// Units
//#include <opwig/declspecifier.cc>
//#include <opwig/declarator.cc>

// Features - Parsing
#include <opwig/nestednamespecifier.cc>
#include <opwig/container.cc>
#include <opwig/class.cc>
#include <opwig/classmembers.cc>
#include <opwig/function.cc>
#include <opwig/namespace.cc>
#include <opwig/variable.cc>
#include <opwig/enum.cc>
#include <opwig/complexclasses.cc>

// Features - Generating
#include <opwig/proxygenerator.cc>
#include <opwig/wrappergenerator.cc>

