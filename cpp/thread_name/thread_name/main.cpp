#if _MSC_VER >= 1600
#define DEFINE_SET_THREAD_NAME
#endif

#ifdef DEFINE_SET_THREAD_NAME
#include <windows.h>

#include <thread>
#include <chrono>

static constexpr DWORD MSVC_EXCEPTION = 0x406D1388;

struct alignas(8) THREADNAME_INFO
{
    DWORD dwType;       // Must be 0x1000.
    LPCSTR szName;      // Pointer to name (in user addr space).
    DWORD dwThreadID;   // Thread ID (-1=caller thread).
    DWORD dwFlags;      // Reserved for future use, must be zero.
};

void SetThreadName(DWORD id, const char * name) 
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = id;
    info.dwFlags = 0;

#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try 
    {
        RaiseException(MSVC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR *)(&info));
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
#pragma warning(pop)
}

void set_thread_name(const char * name)
{
#ifndef _NDEBUG
    SetThreadName(-1, name);
#endif
}

void set_thread_name(std::thread & thread, const char * name)
{
#ifndef _NDEBUG
    SetThreadName(GetThreadId(thread.native_handle()), name);
#endif
}
#endif

void thread_procedure()
{
    set_thread_name("worker");
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void main()
{
    set_thread_name("main");

    auto ta = std::thread(thread_procedure);
    set_thread_name(ta, "worker a");

    auto tb = std::thread(thread_procedure);
    set_thread_name(tb, "worker b");

    auto tc = std::thread(thread_procedure);
    set_thread_name(tc, "worker c");

    ta.join();
    tb.join();
    tc.join();
}
