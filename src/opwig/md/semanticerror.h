
#ifndef OPWIG_MD_SEMANTICERROR_H_
#define OPWIG_MD_SEMANTICERROR_H_

#include <string>
#include <sstream>
#include <exception>

namespace opwig {
namespace md {

class SemanticError : public std::exception {
  public:
    SemanticError(const std::string& message, const char* file, int line_num) : message_(message) { buildMessage(file, line_num); }
    SemanticError(const char* message, const char* file, int line_num) : message_(message) { buildMessage(file, line_num); }
    
    SemanticError(const std::string& message) : message_(message) {}
    SemanticError(const char* message) : message_(message) {}
    
    virtual const char* what() const throw() override;
  
  protected:
    std::string message_;
    
    void buildMessage(const char* file, int line);
};

inline const char* SemanticError::what() const throw() {
  return message_.c_str();
}

inline void SemanticError::buildMessage(const char* file, int line) {
    std::ostringstream msg (file);
    msg << ":" << line << ": " << message_;
    message_ = msg.str();
}

} // namespace opwig
} // namespace md

#endif // OPWIG_MD_SEMANTICERROR_H_

