CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: ex3

lib/csim.a:
	(cd lib && $(MAKE))
	
ex3: ex3.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o ex3 ex3.c lib/csim.a -lm

clean:
	rm -f ex3
