#ifndef OUROBOROS_SCRIPT_EXCEPTIONS_H_
#define OUROBOROS_SCRIPT_EXCEPTIONS_H_

#include <string>
#include <exception>
#include <stdexcept>
    
namespace opa {

/*
-InvalidVMError
-IOError
-InternalVMError
*/

/** Class representing a invalid VM error. Examples: trying to use unregistered VMs,
    trying to use something from a VM in another, etc.*/
class InvalidVMError : public std::runtime_error {
  public:
    InvalidVMError (const std::string& vm_name, const std::string& message)
        : vm_name_(vm_name), std::runtime_error(message) {}
  
  protected:
    std::string vm_name_;
    std::string message_;
};

/** Class representing a IO error. Tipically raised when a file could not be
    opened, read or written to.  */
class IOError : public std::runtime_error {
  public:
    IOError (const std::string& msg) : std::runtime_error(msg) {}
};

/** Class representing a internal error on a VM. Raised when a VM API call
    gives an error or some exception occured inside the VM. */
class InternalVMError : public runtime_error {
  public:
    InternalVMError(const std::string& msg) : std::runtime_error(msg) {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_EXCEPTIONS_H_ */
