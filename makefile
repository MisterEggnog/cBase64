
CFLAGS=-g -Wall

all: target

target: CBase64Stream.o test.o
	$(CC) -o target $^

CBase64Stream.o: CBase64Stream.h

test.o: CBase64Stream.h

clean:
	$(RM) *.o target
