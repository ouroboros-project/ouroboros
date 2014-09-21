
#ifndef OPWIG_MD_SEMANTICERROR_H_
#define OPWIG_MD_SEMANTICERROR_H_

#include <string>
#include <sstream>
#include <exception>

namespace opwig {
namespace md {

/// Represents a semantic error exception.
/** This class represents a semantic error exception.
    Used by some metadata classes when a invalid operation is tried, and more tipically
    by the parser. */
class SemanticError : public std::exception {
  public:
    /// Builds a new Semantic error with given values.
    /** Builds a new Semantic error with given values.
    * @param message The error message.
    * @param file Name of the (code) file which raised the error (use the macro __FILE__).
    * @param line_num Number of the line of code in the file which raised the error (use the macro __LINE__).
    */
    SemanticError(const std::string& message, const char* file, int line_num) : message_(message) { buildMessage(file, line_num); }
    /// Builds a new Semantic error with given values.
    /** Builds a new Semantic error with given values.
    * @param message The error message.
    * @param file Name of the (code) file which raised the error (use the macro __FILE__).
    * @param line_num Number of the line of code in the file which raised the error (use the macro __LINE__).
    */
    SemanticError(const char* message, const char* file, int line_num) : message_(message) { buildMessage(file, line_num); }

    /// Builds a new Semantic error with given values.
    /** Builds a new Semantic error with given values.
    * @param message The error message.
    */
    SemanticError(const std::string& message) : message_(message) {}
    /// Builds a new Semantic error with given values.
    /** Builds a new Semantic error with given values.
    * @param message The error message.
    */
    SemanticError(const char* message) : message_(message) {}

    /// Gets the exception  message/details.
    virtual const char* what() const throw() override;
  
  protected:
    std::string message_;
    
    void buildMessage(const char* file, int line);
};

inline const char* SemanticError::what() const throw() {
  return message_.c_str();
}

inline void SemanticError::buildMessage(const char* file, int line) {
    std::string fileStr (file);
    std::ostringstream msg (fileStr);
    msg << fileStr << ":" << line << ": " << message_;
    message_ = msg.str();
}

} // namespace opwig
} // namespace md

#endif // OPWIG_MD_SEMANTICERROR_H_

