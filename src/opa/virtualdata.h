
#ifndef OUROBOROS_SCRIPT_VIRTUALDATA_H_
#define OUROBOROS_SCRIPT_VIRTUALDATA_H_

#include <memory>

#include <vector>
#include <list>
#include <map>
#include <functional>

#include <opa/type.h>

namespace opa {

class VirtualObj;
class VirtualMachine;

/// Abstract class representing virtual script data.
/** Abstract class that represents and encapsulates an script object (from a particular VM).
    This class should be implemented for each supported scripting language in OPA. */
class VirtualData : public std::enable_shared_from_this<VirtualData> {

  public:
    /// Smart pointer for this class.
    typedef std::shared_ptr<VirtualData>       Ptr;
    /// Smart const pointer for this class.
    typedef std::shared_ptr<const VirtualData> ConstPtr;
    /// Smart weak pointer for this class.
    typedef std::weak_ptr<VirtualData>         WeakPtr;
    /// Vector of smart VirtualData pointers.
    typedef std::vector<Ptr>                        Vector;
    /// List of smart VirtualData pointers.
    typedef std::list<Ptr>                          List;
    /// Map of smart VirtualData pointers ( VData -> VData )
    typedef std::map<Ptr, Ptr>                      Map;

    virtual ~VirtualData() {}

    /// Gets a smart pointer to this instance.
    /** Gets a smart pointer to this VirtualData instance.
    * @return a smart pointer to this intance.
    */
    Ptr Copy() {
        return shared_from_this();
    }

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type, bool disown) const = 0;
    /// Unwraps to data contained in this object to a C string.
    virtual const char* UnwrapString() const = 0;
    /// Unwraps to data contained in this object to a boolean.
    virtual bool UnwrapBoolean() const = 0;
    /// Unwraps to data contained in this object to a integer.
    virtual int UnwrapInteger() const = 0;
    /// Unwraps to data contained in this object to a double.
    virtual double UnwrapNumber() const = 0;
    /// Unwraps to data contained in this object to a std::vector of VirtualDatas.
    virtual Vector UnwrapVector() const = 0;
    /// Unwraps to data contained in this object to a std::list of VirtualDatas.
    virtual List UnwrapList() const = 0;
    /// Unwraps to data contained in this object to a std::Map of VirtualData to VirtualData.
    virtual Map UnwrapMap() const = 0;

    /// Tries to wrap the given data with the given type.
    /** Returns a new VirtualData::Ptr with the data wrapped upon success.
     *
     * The VirtualData object returned by this method is NOT NECESSARILY the
     * same as the one that called it. Thus a safe way to use this is like
     * this:
     *
     * <code>
     * VirtualData::Ptr vdata;<br>
     * // ...<br>
     * vdata = vdata->Wrap(...);
     * </code>
     *
     * @param data - a void pointer to the data being wrapped.
     * @param type - a virtual type got from TypeRegistry<T>::type(), where
     *               T is the original declarated type of the data.
     * @return A shared pointer to the wrapped data.
     * @see opa::TypeRegistry
     * @depracated
     */
    virtual void Wrap(void* data, const VirtualType& type) = 0;
    /// Tries to set the contained data to the given C string.
    virtual void WrapString(const char* str) = 0;
    /// Tries to set the contained data to the given boolean.
    virtual void WrapBoolean(bool boolean) = 0;
    /// Tries to set the contained data to the given integer.
    virtual void WrapInteger(int number) = 0;
    /// Tries to set the contained data to the given double.
    virtual void WrapNumber(double number) = 0;

    /// Gets the VM to which this data is associated.
    /** Gets the VirtualMachine to which this data is associated.
    * @return VM instance that owns the data contained in this object.
    * @see opa::VirtualMachine
    */
    virtual VirtualMachine* machine () const = 0;

    /// Executes the contained data as a callable object.
    /** Tries to execute ourselves as a callable object (like a function),
        passing the given arguments and returning the result. 
    * @param args is a vector of VirtualData::Ptr, containing the arguments to
            the call.
    * @return A new VirtualData instance containing the result of the call (the return value).
    */
    virtual Ptr Execute(const Vector& args) = 0;

    /// Tries to get a attribute from this object.
    /** Tries to get a attribute with the given name from this object.
    * @param key is a VirtualData::Ptr containing the key to use to search the attribute
            (tipically a string value with the name)
    * @return A new VirtualData instance containing the attribute object.  
    */
    virtual Ptr GetAttribute(Ptr key) = 0;

    /// Tries to set a attribute in this object.
    /** Tries to set a attribute with the given key in this object.
    * @param key is a VirtualData::Ptr containing the key to set the attribute in (tipically
            a string the the attribute name).
    * @param value is a VirtualData::Ptr containing the value of the attribute.
    * @return A VirtualData instance containing the attribute. Tipically the same as the value parameter.
    */
    virtual Ptr SetAttribute(Ptr key, Ptr value) = 0;

    /// Gets a raw pointer to the contained data.
    /** Gets a raw void* to the contained data, or to something that represents it. 
        This depends on the implementation and should not be used without knowing about the implementation.
    * @return a unsafe void* to the contained data.
    * @deprecated
    */
    virtual void* unsafe_data() const = 0;

    /// Adds this data to a VM buffer.
    virtual void AddToBuffer() {}
    /// Removes this data from a VM buffer.
    virtual void RemoveFromBuffer() {}

  protected:

    VirtualData() {}

  private:



};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_VIRTUALDATA_H_ */
