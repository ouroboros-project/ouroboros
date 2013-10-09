
#ifndef OPWIG_MD_PTR_H_
#define OPWIG_MD_PTR_H_

#include <memory>

namespace opwig {
namespace md {

/// Templatized shared smart pointer.
/** Templatized smart pointer with garbage collection.
* @tparam T C++ type of contained pointer.
*/
template <typename T>
using Ptr = std::shared_ptr<T>;

/// Templatized weak smart pointer.
/** Templatized smart pointer with a weak reference to
    avoid cyclic references.
* @tparam T C++ type of contained pointer.
*/
template <typename T>
using WeakPtr = std::weak_ptr<T>;

/// Templatized unique smart pointer.
/** Templatized smart pointer with a unique reference to
    avoid multiple references to the same instance.
* @tparam T C++ type of contained pointer.
*/
template <typename T>
using UniquePtr = std::unique_ptr<T>;

} // namespace opwig
} // namespace md

#endif // OPWIG_MD_PTR_H_

