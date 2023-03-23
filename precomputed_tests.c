#include <acutest.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "base64.h"

struct base64_precompute {
	unsigned char raw[3];
	char three_str[5];
	char two_str[5];
	char one_str[5];
};

int read_base64_data(FILE* source, struct base64_precompute* dest) {
	int amount_read = fscanf(source, "%2hhx%2hhx%2hhx|%4s|%4s|%4s", &dest->raw[0], &dest->raw[1], &dest->raw[2], dest->three_str, dest->two_str, dest->one_str);

	assert(amount_read == 6 || amount_read == EOF);
	return amount_read;
}

#define DATA_FILE ("file.ctv")

bool
test_decode_return(int return_code, unsigned char raw[], struct base64_precompute data) {
		bool stop;

		stop = TEST_CHECK(return_code == 3);
		TEST_MSG("returned %d instead of 3", return_code);

		for (int i = 0; i < 3; i++)
			stop = !TEST_CHECK(raw[i] == data.raw[i]) || stop;
		TEST_MSG("Expected raw to be [%x, %x, %x], was [%x, %x, %x].", data.raw[0], data.raw[1], data.raw[2], raw[0], raw[1], raw[2]);

		TEST_MSG("given source string \"%s\"", data.three_str);

		return stop;
}

void
test_3_byte_decode(void) {
	char buffer[BUFSIZ];
	struct base64_precompute data;
	FILE* input_data = fopen(DATA_FILE, "r");
	setbuf(input_data, buffer);

	while (read_base64_data(input_data, &data) != EOF) {
		unsigned char raw[3] = "";
		int return_code = base64_decode(data.three_str, raw);


		if (test_decode_return(return_code, raw, data))
			break;
	}

	fclose(input_data);
}

#define RESULT_ERR_STR "result %s str was not \"%s\""
#define TEST_STR_IS_RIGHT(field_name, num, expstr) { \
	TEST_CHECK(strcmp(result.field_name, expstr) == 0); \
	TEST_MSG(RESULT_ERR_STR, num, expstr); \
}

void
read_into_table_test(void) {
	FILE* temp_file = tmpfile();
	fprintf(temp_file, "ABCDEF|abcd|abc=|ab==\n");
	rewind(temp_file);

	struct base64_precompute result;
	TEST_CHECK(read_base64_data(temp_file, &result) == 6);
	TEST_MSG("Successful read should return 6.");

	unsigned char expected_bytes[] = { 0xAB, 0xCD, 0xEF };
	for (int i = 0; i < 3; i++)
		TEST_CHECK(result.raw[i] == expected_bytes[i]);
	TEST_MSG("Result was supposed to be 0xABCDEF, not 0x%02hhX%02hhX%02hhX", result.raw[0], result.raw[1], result.raw[2]);

	TEST_STR_IS_RIGHT(three_str, "three", "abcd");
	TEST_STR_IS_RIGHT(two_str, "two", "abc=");
	TEST_STR_IS_RIGHT(one_str, "one", "ab==");

	fclose(temp_file);
}

TEST_LIST = {
	{ "test_3_byte_decode", test_3_byte_decode },
	{ "read_into_table_test", read_into_table_test },
	{ NULL, NULL },
};
