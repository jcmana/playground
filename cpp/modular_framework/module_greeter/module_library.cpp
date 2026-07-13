#include "../framework/module_library.h"

#include "module_greeter.h"

module_intf * create()
{
    return new module_greeter();
}
