
#ifndef OPWIG_MDNAMESPACE_H_
#define OPWIG_MDNAMESPACE_H_

#include <string>
#include <map>
#include <memory>

namespace opwig {

/// Metadata class for C++ namespaces.
class MDNamespace {

  public:

    /// Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<MDNamespace> Ptr;

    /// Creates a new MDNamespace object. Must be used in place of the
    /// constructor.
    static Ptr Create ();

  private:

    std::map<std::string, Ptr> nested_namespaces_;

    MDNamespace () {}

};

inline MDNamespace::Ptr MDNamespace::Create () {
  return Ptr(new MDNamespace);
}

} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_

