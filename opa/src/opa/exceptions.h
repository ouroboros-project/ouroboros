#ifndef OUROBOROS_SCRIPT_EXCEPTIONS_H_
#define OUROBOROS_SCRIPT_EXCEPTIONS_H_

#include <string>
#include <exception>
#include <stdexcept>
    
namespace opa {

/// Class representing a invalid VM error.
/** Class representing a invalid VM error. Usually means the user tried to do
    something invalid, or mixed up VMs. Examples: trying to use unregistered VMs,
    trying to use something from a VM in another, etc.*/
class InvalidVMError : public std::runtime_error {
  public:
    /// Initializes the exception with given VM name and message.
    /** Initializes the exception with given VM name and message.
    * @param vm_name is the name of the VM that raised the exception.
    * @param msg is the message depicting/explaining the error.
    */
    InvalidVMError (const std::string& vm_name, const std::string& msg)
        : std::runtime_error("["+vm_name+"] Invalid error:\n"+msg) {}
};

/// Class representing a IO error.
/** Class representing a IO error. Tipically raised when a file could not be
    opened, read or written to.  */
class IOError : public std::runtime_error {
  public:
    /// Initializes the exception with given file name and operation.
    /** Initializes the exception with given file name and operation.
    * @param filename is the name of the file involved in the operation that raised the exception.
    * @param op is a string naming the operation that errored.
    */
    IOError (const std::string& filename, const std::string& op) 
        : std::runtime_error("[I/O] Error: could not perform "+op+" on file '"+filename+"'") {}
};

/// Class representing a internal error on a VM.
/** Class representing a internal error on a VM. Raised when a VM API call
    gives an error or some exception occured inside the VM. */
class InternalVMError : public std::runtime_error {
  public:
    /// Initializes the exception with given VM name and message.
    /** Initializes the exception with given VM name and message.
    * @param vm_name is the name of the VM that raised the exception.
    * @param msg is the message depicting/explaining the error.
    */
    InternalVMError(const std::string& vm_name, const std::string& msg) 
        : std::runtime_error("["+vm_name+"] Internal error:\n"+msg) {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_EXCEPTIONS_H_ */
