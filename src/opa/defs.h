/// Here are the common definitions of the script system.

#ifndef OUROBOROS_SCRIPT_DEFS_H_
#define OUROBOROS_SCRIPT_DEFS_H_

/// The ID, if defined, of the script language called 'name'.
#define LANG(name) name##Lang

/// Defines an ID for the script language called 'name'.
#define DEFINE_LANG_ID(name,id) \
    const LangID LANG(name) = id;

#ifndef SWIG_GENERATED
extern "C" {
/// Used with opaque pointers.
struct swig_type_info;

}
#endif

namespace opa {

/// Type used for script languages' IDs.
typedef size_t LangID;

/// Script languages ID's definition list.
DEFINE_LANG_ID(Lua,     0);
DEFINE_LANG_ID(Python,  1);


} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_DEFS_H_ */
