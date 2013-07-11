
#ifndef OPWIG_GEN_SCRIPTCONVERTER_H_
#define OPWIG_GEN_SCRIPTCONVERTER_H_

#include <opwig/md/ptr.h>

namespace opwig {
namespace gen {

/// Base class that handles conversion of object between C++ and a scripting language.
class ScriptConverter {

  public:
    /// constructor
    ScriptConverter() {}
    
    /// Virtual destructor.
    virtual ~ScriptConverter() {}


};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_SCRIPTCONVERTER_H_


