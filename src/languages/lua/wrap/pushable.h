
#ifndef OPA_LUA_WRAP_PUSHABLE_H_
#define OPA_LUA_WRAP_PUSHABLE_H_

namespace opa {
namespace lua {
namespace wrap {

class Pushable {
  public:
    virtual ~Pushable() {}
    std::string name() const { return name_; }
    virtual void PushOnto(State &L) const = 0;
  protected:
    Pushable(const std::string& the_name) : name_(the_name) {}
  private:
    std::string name_;
};

} // namespace wrap
} // namespace lua
} // namespace opa

#endif // OPA_LUA_WRAP_PUSHABLE_H_

