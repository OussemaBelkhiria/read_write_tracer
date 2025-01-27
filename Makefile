CC = cc
CFLAGS = -g -Wall -O2
all: librw_1.so librw_2.so tracer
librw_1.so: wrappers.c
	$(CC) $(CFLAGS) -shared -fPIC -ldl -o $@ $<
librw_2.so: wrappers_asm.c
	$(CC) $(CFLAGS) -shared -fPIC -ldl -o $@ $<
tracer: tracer.c
	$(CC) $(CFLAGS) tracer.c -o $@ -ldl