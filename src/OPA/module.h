#ifndef OUROBOROS_SCRIPT_MODULE_H_
#define OUROBOROS_SCRIPT_MODULE_H_

#include <string>

namespace ouroboros {

template <class loader_t>
class Module {
  public:
    Module(const std::string& name, loader_t init_func) :
        name_(name),
        init_func_(init_func) {}
    const std::string& name() const { return name_; }
    loader_t init_func() const { return init_func_; }
  private:
    std::string     name_;
    loader_t        init_func_;
};

} /* namespace ouroboros */

#endif /* OUROBOROS_SCRIPT_MODULE_H_ */
