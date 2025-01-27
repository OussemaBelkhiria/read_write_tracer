#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
// return value is of type ssize_t because read returns -1 in case of an error
ssize_t read (int fd , void* buf , size_t nbytes) {
    if (nbytes != 0) {
      long rv = syscall(SYS_read, fd , buf , nbytes);
      if (rv < 0) return -1;
      return rv;
    }
    return 0;
}
ssize_t write (int fd,const void* buf, size_t nbytes) {
    if (nbytes != 0) {
        long rv = syscall(SYS_write, fd , buf , nbytes);
        if (rv < 0) return -1;
        return rv;
    }
    return 0;
}


