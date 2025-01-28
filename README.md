# wrappers.c , wrappers_asm.c
creating wrappers for the syscalls read and write using C and inline assembly. the files are compiled into shared libraries. the libraries are usable by any application to replace the original glibc implementations of the two wrappers

# tracer.c :
trace the syscalls read and write performed by another program using ptrace() syscall.
