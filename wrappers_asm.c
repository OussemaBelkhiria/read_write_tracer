/* because we have operands we use extended asm : assembler instructions with C Expression Operands*/
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>
ssize_t read (int fd , void* buf, size_t nbytes) {
    ssize_t rv = 0;
    if (nbytes != 0) {
    /* remarks :
    we are using x86 so we pass the args to rdi , rsi , rdx 
    clobbers : A comma-separated list of registers or other values changed by the AssemblerTemplate : we list the modified registers
    use of volatile : extended asm statments may produce side effects use volatile to turn off certain optimizations   */

    asm volatile ("xor %%rax, %%rax;"
          "mov %1 , %%edi;"
          "mov %2 , %%rsi ;"
          "mov %3 , %%rdx ;"
          "syscall;"
          "mov %%rax , %0 ;"
         :"=r" (rv)
         :"r" (fd), "r" (buf), "r" (nbytes) 
         :"%rax","%rdi","%rsi","%rdx"
         );
    }
    if (rv < 0) {
        errno = -rv;
        return -1;}
    return rv;
}
ssize_t write (int fd , const void* buf, size_t nbytes) {
    ssize_t rv = 0;
    if (nbytes != 0) {
    asm volatile ("mov $1, %%eax;"
          "mov %1 , %%edi;"
          "mov %2 , %%rsi ;"
          "mov %3 , %%rdx ;"
          "syscall;"
          "mov %%rax , %0 ;"
         :"=r" (rv)
         :"r" (fd), "r" (buf), "r" (nbytes) 
         :"%rax","%rdi","%rsi","%rdx"
         );
    }
    // set errno 
    if (rv < 0) {
        // negate rv before writing it to errno
        errno = -rv;
        return -1;}
    return rv;
}

