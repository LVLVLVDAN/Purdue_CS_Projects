CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: ex2

lib/csim.a:
	(cd lib && $(MAKE))
	
ex2: ex2.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o ex2 ex2.c lib/csim.a -lm

clean:
	rm -f ex2
