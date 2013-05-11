#ifndef OUROBOROS_CONFIG_CONFIG_H_
#define OUROBOROS_CONFIG_CONFIG_H_

// Version
#define OUROBOROS_VERSION_MAJOR 0
#define OUROBOROS_VERSION_MINOR 1
#define OUROBOROS_VERSION_PATCH 0

#define OUROBOROS_BIGVERSION "0.1"
#define OUROBOROS_VERSION "0.1.0"

///


///

#define OUROBOROS_CREATE_BINDINGS
#define OUROBOROS_LUA_BINDINGS
#define OUROBOROS_PYTHON_BINDINGS
#define OUROBOROS_ENABLED(X) OUROBOROS_##X##_ENABLED

#endif // OUROBOROS_CONFIG_CONFIG_H_