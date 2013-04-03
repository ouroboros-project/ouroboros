
#ifndef OPWIG_MD_SEMANTICERROR_H_
#define OPWIG_MD_SEMANTICERROR_H_

#include <string>
#include <exception>

namespace opwig {
namespace md {

class SemanticError : public std::exception {
  public:
    SemanticError(const std::string& message) : message_(message) {}
    SemanticError(const char* message) : message_(message) {}
    
    virtual const char* what() const throw() override;
  
  protected:
    std::string message_;
};

inline const char* SemanticError::what() const throw() {
  return message_.c_str();
}


} // namespace opwig
} // namespace md

#endif // OPWIG_MD_SEMANTICERROR_H_

