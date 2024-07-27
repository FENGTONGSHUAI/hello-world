/*
file name: ctors.cpp
*/
typedef void (*init_func)(void);
#ifdef WIN32
// win版本暂不实现
#else
void run_hooks();
extern "C" void do_global_ctors()
{
    run_hooks;
}
#endif