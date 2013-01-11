
#ifndef OUROBOROS_SCRIPT_VIRTUALDATA_H_
#define OUROBOROS_SCRIPT_VIRTUALDATA_H_

#include <memory>

#include <vector>
#include <list>
#include <map>
#include <functional>

#include <type.h>

/*namespace ouroboros {
namespace script {
class VirtualData;
}
}

namespace std {

template <>
struct less<std::shared_ptr<ouroboros::VirtualData> > :
binary_function <std::shared_ptr<ouroboros::VirtualData>,
                 std::shared_ptr<ouroboros::VirtualData>,
                 bool> {
    bool operator() (const std::shared_ptr<ouroboros::VirtualData>& x,
                     const std::shared_ptr<ouroboros::VirtualData>& y)
        const
        {return x.get()<y.get();}
};

}*/

namespace ouroboros {

class VirtualObj;
class LangWrapper;

/// Abstract class representing virtual script data.
class VirtualData : public std::enable_shared_from_this<VirtualData> {

  public:

    typedef std::shared_ptr<VirtualData>       Ptr;
    typedef std::shared_ptr<const VirtualData> ConstPtr;
    typedef std::weak_ptr<VirtualData>         WeakPtr;
    typedef std::vector<Ptr>                        Vector;
    typedef std::list<Ptr>                          List;
    typedef std::map<Ptr, Ptr>                      Map;

    virtual ~VirtualData() {}

    Ptr Copy() {
        return shared_from_this();
    }

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type, bool disown) const = 0;
    virtual const char* UnwrapString() const = 0;
    virtual bool UnwrapBoolean() const = 0;
    virtual int UnwrapInteger() const = 0;
    virtual double UnwrapNumber() const = 0;
    virtual Vector UnwrapVector() const = 0;
    virtual List UnwrapList() const = 0;
    virtual Map UnwrapMap() const = 0;

    /// Tries to wrap the given data with the given type.
    /** Returns a new VirtualData::Ptr with the data wrapped upon success.
     **
     ** The VirtualData object returned by this method is NOT NECESSARILY the
     ** same as the one that called it. Thus a safe way to use this is like
     ** this:
     **
     ** <code>
     ** VirtualData::Ptr vdata;<br>
     ** // ...<br>
     ** vdata = vdata->Wrap(...);
     ** </code>
     **
     ** @param data - a void pointer to the data being wrapped.
     ** @param type - a virtual type got from TypeRegistry<T>::type(), where
     **               T is the original declarated type of the data.
     ** @return A shared pointer to the wrapped data.
     ** @see ouroboros::TypeRegistry
     ** @depracated
     */
    virtual void Wrap(void* data, const VirtualType& type) = 0;
    virtual void WrapString(const char* str) = 0;
    virtual void WrapBoolean(bool boolean) = 0;
    virtual void WrapInteger(int number) = 0;
    virtual void WrapNumber(double number) = 0;

    virtual LangWrapper* wrapper () const = 0;

    /// Tries to execute ourselves as a function in a script language,
    /// passing the given arguments and returning the result.
    virtual Ptr Execute(const Vector& args) = 0;

    /// Tries to get a attribute with the given name from this object.
    /**
     */
    virtual Ptr GetAttribute(Ptr key) = 0;

    virtual Ptr SetAttribute(Ptr key, Ptr value) = 0;

    virtual void* unsafe_data() const = 0;

    virtual void AddToBuffer() {}
    virtual void RemoveFromBuffer() {}

  protected:

    VirtualData() {}

  private:



};

} /* namespace ouroboros */

#endif /* OUROBOROS_SCRIPT_VIRTUALDATA_H_ */
