#include "../framework/module_library.h"

#include "module_logger.h"

module_intf * create()
{
    return new module_logger();
}