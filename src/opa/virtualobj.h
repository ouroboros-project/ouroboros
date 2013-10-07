
#ifndef OUROBOROS_SCRIPT_VIRTUALOBJ_H_
#define OUROBOROS_SCRIPT_VIRTUALOBJ_H_

#include <opa/virtualdata.h>
#include <opa/type.h>
#include <opa/virtualmachine.h>
#include <opa/virtualprimitive.h>
#include <opa/utils/uncopyable.h>

#include <list>
#include <vector>
#include <map>
#include <string>

#include <cstdio>

namespace opa {

class VirtualObj;
class Bind;
class TempList;

/// A proxy class wich represents virtual objects from scripting languages.
/** This class represents a virtual object from a scripting language, and
    is basically a proxy to a VirtualData instance that holds the actual object
    data from the VM.
*/
class VirtualObj {

  public:
    /// Pair of VirtualObjs
    typedef std::pair<VirtualObj,VirtualObj>    VirtualEntry;
    /// List of VirtualObjs
    typedef std::list<VirtualObj>               List;
    /// Vector of VirtualObjs
    typedef std::vector<VirtualObj>             Vector;
    /// Map of VirtualObjs (VObj->VObj)
    typedef std::map<VirtualObj,VirtualObj>     Map;

    /// Builds an <i>empty</i> virtual object.
    /** Attempting to use any method in a virtual object created this way will
     ** result in a segmentation fault.
     */
    explicit VirtualObj() :
        data_() {}

    /// Builds an virtual object with the given data.
    /** Creates a virtual object with the given data.
    * @param data is the VirtualData instance.
    */
    explicit VirtualObj(VirtualData::Ptr data) :
        data_(data) {}

    /// Builds an empty virtual object associated with the given Virtual Machine.
    /** Creates a empty virtual object associated with the given virtual machine.
    * @param vm is the virtual machine instance to be associated with.
    */
    explicit VirtualObj(VirtualMachine* vm) :
        data_(vm->NewData()) {}

    ~VirtualObj() {}

    /// Gets the contained script value as a C++ value.
    /** Tries to convert the contained script value to a C++ type value.
        Template parameter is the target C++ type.
    * @param disown is a boolean telling if we are stealing the ownership of the
            object from the VM.
    * @tparam T is the target C++ type.
    * @return the converted C++ type value.
    */
    template <class T>
    T value(bool disown = false) const {
        return VirtualPrimitive<T>::value(data_, disown);
    }

    /// Sets the contained script value to a C++ value.
    /** Tries to convert a given C++ value to the script, and set it as
        the contained data of this virtual object.
        Template parameter is the C++ type.
    * @param val is the C++ value to be converted.
    * @tparam T is the C++ type being converted.
    */
    template <class T>
    void set_value(T val) {
        VirtualPrimitive<T>::set_value(data_, val);
    }

    /// operator to set the contained data to the given C++ value.
    /** Operator= overload - sets the contained data to the (pointer 
        of the) given C++ value .

        Template paramater is the C++ type received.
    * @param obj is the pointer to the C++ type.
    * @tparam T is the C++ type being converted.
    * @return this instance.
    */
    template <class T>
    VirtualObj& operator=(T* obj) {
        data_->Wrap(
            static_cast<void*>(obj),
            TypeRegistry<T>::type()
        );
        return *this;
    }

    /// Gets the virtual machine this object is associated with.
    /** Returns the virtual machine this object is associated with.
    * @return a virtual machine pointer.
    * @see opa::VirtualMachine
    */
    VirtualMachine* machine() const { return data_->machine(); }

    /// Checks if this object is valid.
    /** Checks if this object is valid by checking the contained
        VirtualData.
    * @return boolean indicating if the object is valid (true) or not.
    */
    bool valid() const { return static_cast<bool>(data_); }

    /// Operator bool - returns if this object is valid.
    /** Returns if this object is valid.
    * @return boolean indicating if the object is valid (true) or not.
    * @see opa::VirtualObj::valid
    */
    operator bool() const { return valid(); }

    /// Operator less - based of pointer value of contained data.
    //TODO: explain this bettter.
    bool operator<(const VirtualObj& rhs) const {
        return data_.get() < rhs.data_.get();
    }

    /// Executes this object as a callable.
    /** Executes this object as a callable, passing the given arguments
        to the call.
    * @param args is a std::list of VirtualObjs, containing the arguments in the same order to
            be passed to the function call.
    * @return A new VirtualObj instance containing the return value of the call.
    * @see opa::VirtualData::Execute
    */
    VirtualObj operator() (const List& args = List()) const;

    /// Gets a attribute with given key from this object.
    /** Gets a attribute with given key from this object.
    * @param key is the VObj containing the key identifying the attribute.
    * @return New VirtualObj instance containing the attribute value.
    */
    VirtualObj attribute(const VirtualObj& key) const {
        return VirtualObj(data_->GetAttribute(key.data_));
    }

    /// Gets a attribute with given key from this object.
    /** Operator[] overload to ease attribute retrival - calls attribute(...),
        getting a attribute with given key from this object.
    * @param key is the VObj containing the key identifying the attribute.
    * @return New VirtualObj instance containing the attribute value.
    * @see VirtualObj::attribute
    */
    VirtualObj operator[] (const VirtualObj& key) const {
        return attribute(key);
    }
    /// Gets a attribute with given key from this object.
    /** Operator[] overload to ease attribute retrival - calls attribute(...),
        getting a attribute with given key from this object.
    * @param key is the C-string containing the key (most likely the name) identifying the attribute.
    * @return New VirtualObj instance containing the attribute value.
    * @see VirtualObj::attribute
    */
    VirtualObj operator[] (const char* key) const {
        return attribute(Create(key, machine()));
    }
    /// Gets a attribute with given key from this object.
    /** Operator[] overload to ease attribute retrival - calls attribute(...),
        getting a attribute with given key from this object.
    * @param key is the string containing the key (most likely the name) identifying the attribute.
    * @return New VirtualObj instance containing the attribute value.
    * @see VirtualObj::attribute
    */
    VirtualObj operator[] (const std::string& key) const {
        return (*this)[key.c_str()];
    }
    
    /// Sets a attribute with given key to the given value.
    /** Sets a attribute with given key in this object to the given value.
    * @param key is the key identifying the attribute (most likely a string containing the name).
    * @param value is a VirtualObj containing the value to be set.
    * @return Usually the value received itself.
    */
    VirtualObj set_attribute (const VirtualObj& key, const VirtualObj& value) {
        return VirtualObj(
            data_->SetAttribute(key.data_, value.data_)
        );
    }

    /// Auxiliary operator-comma overload to build VirtualObj lists.
    /** Auxiliary operator-comma overload to build VirtualObj lists.
        This operator overload starts the list building procedure, 
        creating a temporary list from a single VirtualObj.

        The operator-comma are used so that the user can write code calling
        the operator() (execute) or operator| (bind) passing VirtualObj by
        VirtualObj, instead of creating a VirtualObj::List manually and passing
        it as the argument (as the methods actually receive). Thanks to 
        this operator, the system will automatically build the list for the user.
    * @param rhs is a VirtualObj to start the list.
    * @return A new TempList instance. 
    */
    TempList operator,(const VirtualObj& rhs) const;

    /// Auxiliary operator-comma overload to build VirtualObj lists.
    /** Auxiliary operator-comma overload to build VirtualObj lists.
        This operator overload continues the list building procedure, 
        adding this VObj instance to the received list, and returning the list.

        The operator-comma are used so that the user can write code calling
        the operator() (execute) or operator-pipe (bind) passing VirtualObj by
        VirtualObj, instead of creating a VirtualObj::List manually and passing
        it as the argument (as the methods actually receive). Thanks to 
        this operator, the system will automatically build the list for the user.
    * @param rhs is a VirtualObj::List.
    * @return The same received list, but with this object in the front. 
    */
    List& operator,(List& rhs) const {
        rhs.push_front(*this);
        return rhs;
    }

    /// Operator-pipe overload, to create VirtualObj binds.
    /** Operator-pipe overload that creates a Bind between this
        VirtualObj instance and a given string.

        This operator overload and the returned Bind (a callable class) 
        is a auxiliary utility to call <b>methods</b> of script instance 
        contained in this VirtualObj. The difference between using the regular
        VirtualObj operator() and this operator to execute the VObj as callable 
        is that in scripting languages it is customary that the first argument to a
        class method is the class instance itself. The purpose of the Bind and this
        operator is exactly that - passing the VirtualObj instance along as the first
        argument to call a script method. 
    * @param method_name is the name of the method to be called. 
    * @return A new Bind instance, containing this VirtualObj and the method_name.
    */
    Bind operator|(const std::string& method_name);

    /// Operator<< overload to set attributes.
    /** Operator<< overload used to set a attribute. 
        Receives a VirtualObj::List, with the first element being
        the key and the second element the value to set.
        Calls VirtualObj::set_attribute internally.
    * @param entry is the VirtualObj::List with the key and value.
    * @return the VirtualObj value set in the attribute.
    */
    VirtualObj operator<<(const List& entry) {
        List::const_iterator it = entry.begin();
        return set_attribute(*(it), *(++it));
    }

    /// Static templatized method to create a VirtualObj from a C++ object.
    /** Static templatized method to create a VirtualObj containing a 
        given C++ object converted to script.
    * @param obj is a pointer to the C++ object.
    * @param vm is a pointer to the VirtualMachine of the target language.
    * @tparam T is the type of the received C++ object pointer.
    * @return a new VirtualObj instance, containing the given C++ object converted to script.
    */
    template <class T>
    static VirtualObj Create (T* obj, VirtualMachine* vm) {
        if (!vm) return VirtualObj();
        VirtualData::Ptr new_data = vm->NewData();
        new_data->Wrap(
            static_cast<void*>(obj),
            TypeRegistry<T>::type()
        );
        return VirtualObj(new_data);
    }

    /// Static method to create a VirtualObj from a C-string.
    /** Static method to create a VirtualObj containing a given
        C-string, converted to the given scripting language.
    * @param obj is a C-string.
    * @param vm is a pointer to the VirtualMachine of the target language.
    * @return a new VirtualObj instance, containing the given C-string converted to script.
    */
    static VirtualObj Create (const char* obj, VirtualMachine* vm);

    /// Static method to create a VirtualObj from a string.
    /** Static method to create a VirtualObj containing a given
        string, converted to the given scripting language.
    * @param str is a string instance.
    * @param vm is a pointer to the VirtualMachine of the target language.
    * @return a new VirtualObj instance, containing the given string converted to script.
    */
    static VirtualObj Create (const std::string& str, VirtualMachine* vm) {
        return Create(str.c_str(), vm);
    }
    
    /// Gets a raw, unsafe pointer to the contained data.
    /** This method returns a raw and unsafe pointer to the contained data.
        This was used internally, necessary for SWIG proxies to work... 
        DO NOT use this if you do not know what you're doing.
    * @return a raw unsafe pointer to the contained data.
    * @deprecated
    */
    void* unsafe_data() const {
        return data_->unsafe_data();
    }

  private:

    VirtualData::Ptr data_;

};

/// Auxiliary templatized function to convert sequences of VirtualDatas to VirtualObjs
/** Auxiliary templatized function to convert sequences (such as std::list or std::vector)
    of VirtualData (or derivates) to a sequence of VirtualObj, creating new instances of 
    VirtualObj with each data in the received sequence.
* @param data_seq is the sequence of VirtualDatas
* @tparam T is the type of the return sequence.
* @tparam U is the type of the sequence received in the parameters.
* @return a instance of sequence T containing VirtualObjs with the received datas.
*/
template <class T, class U>
T ConvertSequence (const U& data_seq) {
    T obj_seq;
    typename U::const_iterator it;
    for (it = data_seq.begin(); it != data_seq.end(); ++it)
        obj_seq.push_back(VirtualObj(*it));
    return obj_seq;
}

/// Auxiliary templatized function to convert a table of VirtualDatas to VirtualObjs
/** Auxiliary templatized function to convert tables (such as std::map)
    of VirtualData to VirtualData (or derivates) to a table of VirtualObj to VirtualObj, creating new 
    instances of VirtualObj with each data in the received table.
* @param data_map is the table of VirtualDatas
* @tparam T is the type of the return table.
* @tparam U is the type of the table received in the parameters.
* @return a instance of table T containing VirtualObjs with the received datas.
*/
template <class T, class U>
T ConvertTable (const U& data_map) {
    T obj_map;
    typename U::const_iterator it;
    for (it = data_map.begin(); it != data_map.end(); ++it) {
        obj_map.insert(std::pair<VirtualObj, VirtualObj>(
            VirtualObj(it->first),
            VirtualObj(it->second)));
    }
    return obj_map;
}

/// Specialization of VirtualObj::value() for template type VirtualObj::List.
template <>
inline VirtualObj::List VirtualObj::value<VirtualObj::List>(bool disown) const {
    return ConvertSequence<List>(data_->UnwrapList());
}

/// Specialization of VirtualObj::value() for template type VirtualObj::Vector.
template <>
inline VirtualObj::Vector VirtualObj::value<VirtualObj::Vector>(bool disown) const {
    return ConvertSequence<Vector>(data_->UnwrapVector());
}

/// Specialization of VirtualObj::value() for template type VirtualObj::Map.
template <>
inline VirtualObj::Map VirtualObj::value<VirtualObj::Map>(bool disown) const {
    return ConvertTable<Map>(data_->UnwrapMap());
}

/// Auxiliary VirtualObj/string Bind class.
/** This class is used to bind a VirtualObj (which should contain a script value
    of a instance of some class), to a string which should be the name of a method
    of the object's class.
    This class implements a operator() overload much like VirtualObj::operator(),
    but passing the VirtualObj instance contained as the first argument, along with
    the rest of the received arguments, and instead of executing the contained VirtualObj,
    it executes a attribute of the VObj, with the contained method_name.

    This is used to execute methods of a class instance, which usually require that the
    first argument received is a instance of the class. Normally scripting languages
    do this automatically in their interpreters, but since we're calling the methods
    manually using the language's API, we should pass the instance ourselves.
    This class and the VirtualObj::operator| helps the user do this.
*/
class Bind {
  public:
    /// Constructs a new Bind object, with given VirtualObj and method_name.
    Bind(VirtualObj& obj, const std::string& method_name) :
        obj_(obj),
        method_name_(obj.machine()) {
        method_name_.set_value(method_name.c_str());
    }
    /// Operator() overload with no arguments.
    VirtualObj operator() () const {
        std::list<VirtualObj> args;
        return obj_[method_name_]((obj_, args)); 
    }
    /// Operator() overload with arguments list.
    VirtualObj operator() (std::list<VirtualObj>& args) const {
        return obj_[method_name_]((obj_, args));
    }
  private:
    Bind& operator=(Bind&); // Bind cannot be copied.

    VirtualObj&   obj_;
    VirtualObj    method_name_;
};

/// Auxiliary class to build a VirtualObj::List with operator-comma.
/** Auxiliary class that contains the temporary VirtualObj::List being
    built by the operator-comma operations, started by VirtualObj.
    A instance of this class can be naturally converted (cast) to
    a VirtualObj::List.
    The user will most likely never use this class directly.
* @see VirtualObj::operator,
*/
class TempList {
  public:
    operator VirtualObj::List&() { return l_; }
    TempList& operator,(const VirtualObj& rhs) {
        l_.push_back(rhs);
        return *this;
    }
  private:
    friend class VirtualObj;
    TempList(const VirtualObj& first, const VirtualObj& second) :
        l_() {
        l_.push_back(first);
        l_.push_back(second);
    }
    VirtualObj::List l_;
};

inline TempList VirtualObj::operator,(const VirtualObj& rhs) const {
    return TempList(*this, rhs);
}

inline Bind VirtualObj::operator|(const std::string& method_name) {
    Bind result(*this, method_name);
    return result;
}

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_VIRTUALOBJ_H_ */
