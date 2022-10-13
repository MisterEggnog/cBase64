
#include <string.h>
#include <acutest.h>
#include "CBase64Stream.h"

void
trivial_encode_4c(void) {
#if 0
	unsigned char data[] = {0, 0, 0};
	Base64Quard resl;
	Base64Quard exp;
	exp.quad = "0000";

	TEST_CHECK(base64_encode(data, 3, &resl) != 0);

	TEST_CHECK(strcmp(exp.str, resl.str) == 0);
	TEST_MSG("Trivial encoding should be `0000`, result was %s\n", resl.str);
#endif
}

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ NULL, NULL },
};
