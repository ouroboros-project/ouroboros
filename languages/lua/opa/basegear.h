
#ifndef OUROBOROS_SCRIPT_LUA_BASEGEAR_H_
#define OUROBOROS_SCRIPT_LUA_BASEGEAR_H_

#include <lua/opa/header.h>
#include <lua/opa/state.h>

#include <opa/utils/uncopyable.h>
#include <opa/exceptions.h>

#include <vector>

namespace opa {
namespace lua {

class BaseGear {

  public:

    class InternalSafeCall {

      public:

        ~InternalSafeCall() { gear_->settop(old_top_); }

        template <class A1>
        InternalSafeCall& Arg(A1 a1) {
            gear_->pushprimitive<A1>(a1);
            ++arg_num_;
            return *this;
        }

        template <class R>
        R GetResult(const R default_value) {
            R result = default_value;
            const Constant check = gear_.TracedCall(arg_num_,1);
            if (check == Constant::OK())
                result = gear_->toprimitive<R>(-1);
            else throw InternalError(check);
            arg_num_ = 0;
            return result;
        }

        void NoResult() {
            const Constant check = gear_.TracedCall(arg_num_,0);
            if (check != Constant::OK())
              throw InternalError(check);
        }

      private:

        friend class BaseGear;

        BaseGear&    gear_;
        int          old_top_;
        unsigned int arg_num_;

        InternalSafeCall(BaseGear& gear, lua_CFunction func) :
            gear_(gear),
            old_top_(gear->gettop()),
            arg_num_(1) {
            gear_->pushcfunction(func);
            gear_->pushudata(&gear_);
        }

        InternalSafeCall& operator=(const InternalSafeCall& rhs);

        static InternalVMError InternalError (const Constant& check) {
            return InternalVMError("Lua", "details: "+check.info());
        }

    };

    InternalSafeCall SafeCall(lua_CFunction func) {
        return InternalSafeCall(*this, func);
    }

    State* operator->() { return &L_; }

    /// Makes a traced call to a function.
    /** Behaves exactly like lua_pcall, with the usual traceback function from
     ** Lua.
     **
     ** [-(nargs+1),+(nres|0),e]
     */
    const Constant TracedCall (int nargs, int nres);

    const Constant Report (const Constant& c);

  protected:

    friend class InternalSafeCall;

    State L_;

    BaseGear(lua_State* L) :
        L_(L) {}

    ~BaseGear() {}

    State& L() { return L_; }
};

#define GETARGPTR(L,i,T,name) \
    T* name = static_cast<T*>((L).touserdata(i))

#define GETARG(L,i,T,name) \
    T& name = *(static_cast<T*>((L).touserdata(i)))

} /* namespace lua */
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_LUA_BASEGEAR_H_ */
