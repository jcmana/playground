#include <Windows.h>

#include "../framework/module_intf.h"
#include "../framework/module_store.h"
#include "../framework/module_library_create.h"

#include "../framework/service_store.h"

int main(int argc, const char ** argv)
{
    module_store modules;
    service_store services;

    for (int argi = 1; argi != argc; ++argi)
    {
        const auto hModule = LoadLibrary(argv[argi]);

        typedef module_intf * (__cdecl * create_fn)();
        const auto pCreate = (create_module_fn)(GetProcAddress(hModule, "create"));

        const auto module_ptr = pCreate();
        module_ptr->load(services);
    }
}