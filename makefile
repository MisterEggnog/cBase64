
CFLAGS=-g -Wall -I acutest/include/
PRECOMPUTE=precomputed_tests

all: test

run_tests: test $(PRECOMPUTE)
	./test
	./$(PRECOMPUTE)

test: test.c base64.c base64.h
	$(CC) -o $@ $(CFLAGS) $<

base64.o: base64.c base64.h

file.ctv: generate_data.py
	./generate_data.py > file.ctv

$(PRECOMPUTE).o: $(PRECOMPUTE).c base64.h

$(PRECOMPUTE): $(PRECOMPUTE).o base64.o file.ctv
	$(CC) -o $@ $(CFLAGS) $< base64.o

clean:
	$(RM) test $(PRECOMPUTE) *.o
