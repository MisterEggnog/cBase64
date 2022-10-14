
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

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ NULL, NULL },
};
