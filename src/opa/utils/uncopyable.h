
#ifndef OUROBOROS_UTILS_UNCOPIABLE_H_
#define OUROBOROS_UTILS_UNCOPIABLE_H_

namespace opa {
namespace utils {

/// Base class for defining uncopyable classes.
class Uncopyable {
  protected:
    Uncopyable() {}
    ~Uncopyable() {}
  private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

} /* namespace utils */
} /* namespace opa */

#endif /* OUROBOROS_UTILS_UNCOPIABLE_H_ */
