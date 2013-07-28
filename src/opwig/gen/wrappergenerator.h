
#ifndef OPWIG_GEN_WRAPPERGENERATOR_H_
#define OPWIG_GEN_WRAPPERGENERATOR_H_

#include <opwig/md/ptr.h>
#include <fstream>
#include <string>

namespace opwig {

namespace md {
class Scope;
}

namespace gen {

class WrapperSpecification;

/** Class to generate wrapper code between an C++ API and 1 or more
    scripting languages. The API is known by their metadata, 
    which should be previously generated by our parser.
    @see opwig::MDParser */
class WrapperGenerator {

  public:
    /// constructor
    WrapperGenerator (const std::string& output_dir, const std::string& file_extension="cxx")
        : output_dir_(output_dir), wrap_file_extension_(file_extension) {}
    
    /// Virtual destructor.
    virtual ~WrapperGenerator() {}

    /** Traverses the metadata tree of the given root scope, generating wrapper code
        for the selected scripting languages and writing them to the corresponding
        source files. */
    void Generate (const std::string& module_name, const md::Ptr<const md::Scope>& root,
                   const md::Ptr<WrapperSpecification>& spec);
    
  protected:
    std::string output_dir_;
    std::string wrap_file_extension_;
    
    std::string generateBootstrap(const md::Ptr<WrapperSpecification>& spec) const;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERGENERATOR_H_


