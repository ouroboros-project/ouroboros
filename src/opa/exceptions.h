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
    InvalidVMError (const std::string& vm_name, const std::string& msg)
        : std::runtime_error("["+vm_name+"] Invalid error:\n"+msg) {}
};

/** Class representing a IO error. Tipically raised when a file could not be
    opened, read or written to.  */
class IOError : public std::runtime_error {
  public:
    IOError (const std::string& filename, const std::string& op) 
        : std::runtime_error("[I/O] Error: could not perform "+op+" on file '"+filename+"'") {}
};

/** Class representing a internal error on a VM. Raised when a VM API call
    gives an error or some exception occured inside the VM. */
class InternalVMError : public runtime_error {
  public:
    InternalVMError(const std::string& vm_name, const std::string& msg) 
        : std::runtime_error("["+vm_name+"] Internal error:\n"+msg) {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_EXCEPTIONS_H_ */
