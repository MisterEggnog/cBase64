
CFLAGS=-g -Wall -I acutest/include/
TARGET=test

all: $(TARGET)
	./$^

$(TARGET): test.c base64.c base64.h
	$(CC) -o $(TARGET) $(CFLAGS) $<

clean:
	$(RM) $(TARGET)
