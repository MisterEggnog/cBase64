
CFLAGS=-g -Wall -I acutest/include/

all: target

target: CBase64Stream.o test.o
	$(CC) -o target $^

CBase64Stream.o: base64.h

test.o: base64.h

clean:
	$(RM) *.o target
