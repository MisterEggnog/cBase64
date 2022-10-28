#include "base64.h"
#include <acutest.h>
#include <random>

void
encoding_same_as_octal_method() {
	auto rand = std::minstd_rand(7);
	auto dist = std::uniform_int_distribution<char>(-128);

	char input[4] = "";
	unsigned char output[4];

	for (int i = 0; i < 50; i++) {
		input[0] = dist(rand);
		input[1] = dist(rand);
		input[2] = dist(rand);
	}
}

TEST_LIST = {
	{ "Library encoding provides same result as octal method", encoding_same_as_octal_method },
	{ NULL, NULL },
};
