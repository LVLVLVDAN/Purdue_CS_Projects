CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: ex0

lib/csim.a:
	(cd lib && $(MAKE))

ex0: ex0.c lib/csim.a  uniform.c expon.c
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o ex0 ex0.c lib/csim.a uniform.c expon.c -lm



clean:
	rm -f ex0
