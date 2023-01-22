#include <acutest.h>
#include <string.h>
#include <stdio.h>

struct base64_precompute {
	unsigned char raw[3];
	char three_str[4];
	char two_str[4];
	char one_str[4];
};

void read_base64_data(FILE* source, struct base64_precompute* dest) {
}

#define RESULT_ERR_STR "result %s str was not \"%s\""

void
read_into_table_test(void) {
	FILE* temp_file = tmpfile();
	fprintf(temp_file, "ABCDEF|abcd|abc=|ab==\n");
	rewind(temp_file);

	struct base64_precompute result;
	read_base64_data(temp_file, &result);

	unsigned char expected_bytes[] = { 0xAB, 0xCD, 0xEF };
	for (int i = 0; i < 3; i++)
		TEST_CHECK(result.raw[i] == expected_bytes[i]);
	TEST_MSG("Result was supposed to be 0xABCDEF, not 0x%02hhX%02hhX%02hhX", result.raw[0], result.raw[1], result.raw[2]);

	TEST_CHECK(strcmp(result.three_str, "abcd") == 0);
	TEST_MSG(RESULT_ERR_STR, "three", "abcd");

	fclose(temp_file);
}

TEST_LIST = {
	{ "read_into_table_test", read_into_table_test },
	{ NULL, NULL },
};
