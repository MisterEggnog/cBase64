#include <acutest.h>
#include <stdio.h>

struct base64_precompute {
	unsigned char raw[3];
	char three_length[4];
	char two_length[4];
	char one_length[4];
};

void read_base64_data(FILE* source, struct base64_precompute* dest);

TEST_LIST = {
	{ NULL, NULL },
};
