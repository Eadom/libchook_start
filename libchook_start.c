#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static void *libc_handle;
int (*real_libc_start_main)(int (*main)(int, char **, char **), int argc,
                            char **ubp_av, void (*init)(void),
                            void (*fini)(void), void (*rtld_fini)(void),
                            void(*stack_end));

void __my_start_main_hook(int (*main)(int, char **, char **),
                          int argc, char **ubp_av, void (*init)(void),
                          void (*fini)(void), void (*rtld_fini)(void),
                          void(*stack_end))
{
    puts("hooked!");
}

int __libc_start_main(int (*main)(int, char **, char **),
                      int argc, char **ubp_av, void (*init)(void),
                      void (*fini)(void), void (*rtld_fini)(void),
                      void(*stack_end))
{
    void *sym;

    libc_handle = dlopen("libc.so.6", RTLD_NOW);
    if (!libc_handle)
    {
        fprintf(stderr, "can't dlopen() libc\n");
        exit(1);
    }
    sym = dlsym(libc_handle, "__libc_start_main");
    if (!sym)
    {
        fprintf(stderr, "can't find the real __libc_start_main()\n");
        exit(1);
    }
    real_libc_start_main = sym;

    __my_start_main_hook(main, argc, ubp_av, init, fini,
                         rtld_fini, stack_end);

    return real_libc_start_main(main, argc, ubp_av, init, fini,
                                rtld_fini, stack_end);
}