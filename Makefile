INCDIR := $(shell pg_config --includedir-server) 
LIBDIR := $(shell pg_config --libdir)

pg_rest:
	gcc -shared -fPIC -o pg_rest.so pg_rest.c -I$(INCDIR) -L$(LIBDIR) -lpq -lcurl