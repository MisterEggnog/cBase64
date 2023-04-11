
CFLAGS=-g -Wall -I acutest/include/
CXXFLAGS=$(CFLAGS)
OCTALTEST=octaltest
PRECOMPUTE=precomputed_tests

all: test

run_tests: test $(PRECOMPUTE) $(OCTALTEST)
	./test
	./$(PRECOMPUTE)
	./$(OCTALTEST)

test: test.c base64.c base64.h
	$(CC) -o $@ $(CFLAGS) $<

base64.o: base64.c base64.h

octal_method_test.o: octal_method_test.cpp base64.h

$(OCTALTEST): base64.o octal_method_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

file.ctv: generate_data.py
	./generate_data.py > file.ctv

$(PRECOMPUTE).o: $(PRECOMPUTE).c base64.h

$(PRECOMPUTE): $(PRECOMPUTE).o base64.o file.ctv
	$(CC) -o $@ $(CFLAGS) $< base64.o

clean:
	$(RM) test $(OCTALTEST) $(PRECOMPUTE) *.o
