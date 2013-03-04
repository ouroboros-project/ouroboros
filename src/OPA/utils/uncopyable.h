
#ifndef OUROBOROS_UTILS_UNCOPIABLE_H_
#define OUROBOROS_UTILS_UNCOPIABLE_H_

namespace ouroboros {
namespace utils {

class Uncopyable {
  protected:
    Uncopyable() {}
    ~Uncopyable() {}
  private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

} /* namespace utils */
} /* namespace ouroboros */

#endif /* OUROBOROS_UTILS_UNCOPIABLE_H_ */
