CC = gcc
BIN = ctf_traced

CFLAGS += -O2 

LIBS = -ldl

.PHONY: all
all: libchook_start.so 

libchook_start.so: libchook_start.c
	$(CC) $(CFLAGS) -shared -g -fPIC -ldl -o libchook_start.so $^ $(LIBS)

.PHONY: clean
clean:  
	rm -rf *.so
