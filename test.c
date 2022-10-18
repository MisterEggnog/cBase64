
#include "base64.c"
#include <string.h>
#include <acutest.h>

void
trivial_encode_4c(void) {
	unsigned char input[3] = "";
	char output[5] = "";


	base64_encode(input, 3, output);

	TEST_CHECK(strcmp(output, "0000") == 0);
	TEST_MSG("Trivial encoding should be `0000`, result was `%s`\n", output);
}

// From wikipedia page for base64
#define THREE_OCTET_INPUT "Man"
#define THREE_OCTET_OUTPUT "TWFu"
#define THREE_OCTED_OUTPUT_INDICES { 19, 22, 5, 46 }

typedef unsigned(*sextet_func)(const unsigned char*);

void
first_sextet_encoding_correctly(void) {
	unsigned char input[] = THREE_OCTET_INPUT;
	unsigned output[] = THREE_OCTED_OUTPUT_INDICES;
	unsigned result = first_sextet(input);

	TEST_CHECK(result == output[0]);
	TEST_MSG("The first 6 bits of %d should be %d, not %d\n", input[0], output[0], result);
}

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ "Calculate first base64 number", first_sextet_encoding_correctly },
	{ NULL, NULL },
};
