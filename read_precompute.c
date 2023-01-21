#include <acutest.h>
#include <stdio.h>

struct base64_precompute {
	unsigned char raw[3];
	char three_length[4];
	char two_length[4];
	char one_length[4];
};

void read_base64_data(FILE* source, struct base64_precompute* dest) {
}


void
read_into_table_test(void) {
	FILE* temp_file = tmpfile();
	fprintf(temp_file, "ABCDEF|abcd|abc=|ab==\n");
	rewind(temp_file);

	struct base64_precompute result;
	read_base64_data(temp_file, &result);

	fclose(temp_file);
}

TEST_LIST = {
	{ "read_into_table_test", read_into_table_test },
	{ NULL, NULL },
};
