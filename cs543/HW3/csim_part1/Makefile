CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: ex1 

lib/csim.a:
	(cd lib && $(MAKE))

ex1: ex1.c lib/csim.a  uniform.c expon.c
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o ex1 ex1.c lib/csim.a uniform.c expon.c -lm



clean:
	rm -f ex1 
