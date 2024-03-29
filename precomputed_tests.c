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
	int amount_read = fscanf(source, "%2hhx%2hhx%2hhx|%4s|%4s|%4s", &dest->raw[0],
		&dest->raw[1], &dest->raw[2], dest->three_str, dest->two_str, dest->one_str);

	assert(amount_read == 6 || amount_read == EOF);
	return amount_read;
}

#define DATA_FILE ("file.ctv")

typedef char*(*prep)(struct base64_precompute*);

void
test_decode(int expected_code, prep fn) {
	char buffer[BUFSIZ];
	struct base64_precompute data;
	FILE* input_data = fopen(DATA_FILE, "r");
	setbuf(input_data, buffer);

	while (read_base64_data(input_data, &data) != EOF) {
		bool stop = false;
		unsigned char raw[3] = "";
		char* encoded_str = fn(&data);
		int return_code = base64_decode(encoded_str, raw);

		TEST_CHECK(return_code == expected_code);
		TEST_MSG("returned %d instead of %d", return_code, expected_code);
		stop = !TEST_CHECK(memcmp(raw, data.raw, 3) == 0);
		TEST_MSG("Expected raw to be [%x, %x, %x], was [%x, %x, %x].",
			data.raw[0], data.raw[1], data.raw[2], raw[0], raw[1], raw[2]);
		TEST_MSG("given source string \"%s\"", encoded_str);

		if (stop)
			break;
	}

	fclose(input_data);
}

void
test_encode(int length, prep fn) {
	char buffer[BUFSIZ];
	struct base64_precompute data;
	FILE* input_data = fopen(DATA_FILE, "r");
	setbuf(input_data, buffer);

	while (read_base64_data(input_data, &data) != EOF) {
		bool exit;
		char encoded[5] = "";
		char* expected = fn(&data);
		base64_encode(data.raw, length, encoded);

		exit = !TEST_CHECK(strcmp(encoded, expected) == 0);
		TEST_MSG("Expected %s, received %s", expected, encoded);
		TEST_MSG("Given [%x, %x, %x]", data.raw[0], data.raw[1], data.raw[2]);

		if (exit)
			break;
	}

	fclose(input_data);
}

static char*
decode_3_prep(struct base64_precompute* data) {
	return data->three_str;
}

void
test_3_byte_decode(void) {
	test_decode(3, decode_3_prep);
}

static char*
decode_2_prep(struct base64_precompute* data) {
	data->raw[2] = 0;
	return data->two_str;
}

void
test_2_byte_decode(void) {
	test_decode(2, decode_2_prep);
}

static char*
decode_1_prep(struct base64_precompute* data) {
	data->raw[2] = 0;
	data->raw[1] = 0;
	return data->one_str;
}

void
test_1_byte_decode(void) {
	test_decode(1, decode_1_prep);
}

char*
encode_3_prep(struct base64_precompute* data) {
	return data->three_str;
}

void
test_3_byte_encode(void) {
	test_encode(3, encode_3_prep);
}

char*
encode_2_prep(struct base64_precompute* data) {
	return data->two_str;
}

void
test_2_byte_encode(void) {
	test_encode(2, encode_2_prep);
}

char*
encode_1_prep(struct base64_precompute* data) {
	return data->one_str;
}

void
test_1_byte_encode(void) {
	test_encode(1, encode_1_prep);
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
	TEST_MSG("Result was supposed to be 0xABCDEF, not 0x%02hhX%02hhX%02hhX",
		result.raw[0], result.raw[1], result.raw[2]);

	TEST_STR_IS_RIGHT(three_str, "three", "abcd");
	TEST_STR_IS_RIGHT(two_str, "two", "abc=");
	TEST_STR_IS_RIGHT(one_str, "one", "ab==");

	fclose(temp_file);
}

TEST_LIST = {
	{ "test_3_byte_decode", test_3_byte_decode },
	{ "test_2_byte_decode", test_2_byte_decode },
	{ "test_1_byte_decode", test_1_byte_decode },
	{ "test_3_byte_encode", test_3_byte_encode },
	{ "test_2_byte_encode", test_2_byte_encode },
	{ "test_1_byte_encode", test_1_byte_encode },
	{ "read_into_table_test", read_into_table_test },
	{ NULL, NULL },
};
