
CFLAGS=-g -Wall -I acutest/include/
CXXFLAGS=$(CFLAGS)
TARGET=test
OCTALTEST=octaltest

all: $(TARGET)
	./$^

$(TARGET): test.c base64.c base64.h
	$(CC) -o $(TARGET) $(CFLAGS) $<

base64.o: base64.c base64.h

octal_method_test.o: octal_method_test.cpp base64.h

$(OCTALTEST): base64.o octal_method_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

file.ctv: generate_data.py
	./generate_data.py > file.ctv

precompute_test: read_precompute.c base64.o file.ctv
	$(CC) -o $@ $(CFLAGS) $< base64.o

clean:
	$(RM) $(TARGET) $(OCTALTEST)
