
CFLAGS=-g -Wall -I acutest/include/

all: target

target: base64.o test.o
	$(CC) -o target $^

base64.o: base64.h

test.o: base64.h

clean:
	$(RM) *.o target
