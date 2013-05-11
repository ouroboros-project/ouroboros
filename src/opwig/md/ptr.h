
#ifndef OPWIG_MD_PTR_H_
#define OPWIG_MD_PTR_H_

#include <memory>

namespace opwig {
namespace md {

template <typename T>
using Ptr = std::shared_ptr<T>;

} // namespace opwig
} // namespace md

#endif // OPWIG_MD_PTR_H_

