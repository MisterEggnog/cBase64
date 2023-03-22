
CFLAGS=-g -Wall -I acutest/include/
CXXFLAGS=$(CFLAGS)
UNITTEST=test
OCTALTEST=octaltest

all: $(UNITTEST)

$(UNITTEST): test.c base64.c base64.h
	$(CC) -o $(UNITTEST) $(CFLAGS) $<

base64.o: base64.c base64.h

octal_method_test.o: octal_method_test.cpp base64.h

$(OCTALTEST): base64.o octal_method_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

file.ctv: generate_data.py
	./generate_data.py > file.ctv

precomputed_tests.o: precomputed_tests.c base64.h

precomputed_tests: precomputed_tests.o base64.o file.ctv
	$(CC) -o $@ $(CFLAGS) $< base64.o

clean:
	$(RM) $(UNITTEST) $(OCTALTEST) precomputed_tests *.o
