
CFLAGS=-g -Wall -I acutest/include/
TARGET=test

all: $(TARGET)
	./$^

$(TARGET): base64.o test.o
	$(CC) -o $(TARGET) $^

base64.o: base64.h

test.o: base64.h

clean:
	$(RM) *.o $(TARGET)
