
#ifndef OPWIG_MD_NESTEDNAMESPECIFIER_H_
#define OPWIG_MD_NESTEDNAMESPECIFIER_H_

#include <string>
#include <vector>

namespace opwig {
namespace md {

/// Base class for metadata objects.
class NestedNameSpecifier {
  public:
    NestedNameSpecifier(const std::string& obj_name) : name_(obj_name) {};
  
    /// Virtual destructor.
    virtual ~NestedNameSpecifier () {}
    
    
    

  protected:
    bool global_;
    std::vector<std::string> paths_;
    std::string name_;
};




} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NESTEDNAMESPECIFIER_H_


