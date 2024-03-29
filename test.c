
#include "base64.c"
#include <stdbool.h>
#include <string.h>
#include <acutest.h>

void
trivial_encode_4c(void) {
	unsigned char input[3] = "";
	char output[5] = "";


	base64_encode(input, 3, output);

	TEST_CHECK(strcmp(output, "AAAA") == 0);
	TEST_MSG("Trivial encoding should be `AAAA`, result was `%s`\n", output);
}

void
smaller_input_sizes(void) {
	unsigned char input[5] = "";
	char output[5] = "";
	char results[][5] = { "AA==", "AAA=" };

	for (int i = 0; i < 2; i++) {
		TEST_CASE_("%d bytes", i + 1);
		base64_encode(input, i + 1, output);
		TEST_CHECK(strcmp(output, results[i]) == 0);
		TEST_MSG("Result was %s, expected %s", output, results[i]);
	}
}

// From wikipedia page for base64
#define THREE_OCTET_INPUT "Man"
#define THREE_OCTET_OUTPUT "TWFu"
#define THREE_OCTET_OUTPUT_INDICES { 19, 22, 5, 46 }

typedef unsigned char(*sextet_func)(const unsigned char*);

void
string_to_octet_conversion_functions(void) {
	unsigned char input[] = THREE_OCTET_INPUT;
	unsigned output[] = THREE_OCTET_OUTPUT_INDICES;
	unsigned result;

	sextet_func fns[] = { first_sextet, second_sextet, third_sextet, fourth_sextet};
	char word_numbers[][10] = { "first", "second", "third", "fourth", };

	for (int i = 0; i < 4; i++) {
		TEST_CASE_("Read the %s 6 bits of 3 bytes", word_numbers[i]);

		result = fns[i](input);
		TEST_CHECK(result == output[i]);
		TEST_MSG("The %s 6 bits of `%s` should be %d, not %d\n", word_numbers[i], input, output[i], result);
	}
}

void
last_sextet_masks_correctly(void) {
	unsigned char input[] = { 255, 255, 255 };
	unsigned result = second_sextet(input);

	TEST_CHECK(result == 63);
}

// decode tests

void
get_b64_index_returns_neg1_for_garbage(void) {
	TEST_CHECK(get_b64_index('$') == BAD_CHAR);
}

#define IN_RANGE(z, h) (z >= 0 && z < h)
#define N_TO_INT_WORKS(fn, test_fn, upper) \
void \
fn ## _works(void) { \
	for (short i = 0; i < 127; i++) { \
		int result = fn(i); \
		TEST_CHECK((bool)(IN_RANGE(result, upper)) == (bool)test_fn(i)); \
		TEST_MSG("fn %d = %d", i, result); \
	} \
}

N_TO_INT_WORKS(ascii_digit_to_int, isdigit, 10);
N_TO_INT_WORKS(ascii_lowercase_to_int, islower, 26);
N_TO_INT_WORKS(ascii_uppercase_to_int, isupper, 26);

void
get_b64_index_works_with_b64(void) {
	for (int i = 0; i < 64; i++) {
		if (!TEST_CHECK_(get_b64_index(BASE64_DIGITS[i]) == i, "%d", i))
			return;
	}
}

void
get_b64_index_accepts_padding(void) {
	TEST_CHECK(get_b64_index(PADDING) == 0);
}

void
get_b64_indices_fills_indices(void) {
	char encoded[] = "////";
	char indices[5] = "";
	char expected[5] = { 63, 63, 63, 63 };

	get_b64_indices(encoded, indices);

	TEST_CHECK(strcmp(indices, expected) == 0);
	TEST_MSG("indices should be [63, 63, 63, 63] was [%d, %d, %d, %d]", indices[0], indices[1], indices[2], indices[3]);
}

void
get_b64_indices_fails_with_garbage(void) {
	char encoded[] = "[[[[";
	char indices[4];

	TEST_CHECK(get_b64_indices(encoded, indices) == BAD_CHAR);
}

void
get_b64_indices_handles_bad_padding(void) {
	char indices[4];
	TEST_CHECK(get_b64_indices("A=ii", indices) == PADDING_ERR);
	TEST_CHECK(get_b64_indices("A2=i", indices) == PADDING_ERR);
}

void
get_b64_indices_returns_num(void) {
	char indices[4];
	TEST_CHECK(get_b64_indices("AAAA", indices) == 4);
	TEST_CHECK(get_b64_indices("AAA=", indices) == 3);
	TEST_CHECK(get_b64_indices("AA==", indices) == 2);
}

// THREE_OCTET_INPUT "Man"
// THREE_OCTET_OUTPUT "TWFu"
// THREE_OCTET_OUTPUT_INDICES { 19, 22, 5, 46 }

void
decode_byte_test(unsigned char(*decode_fn)(char*), char exp) {
	char indices[] = THREE_OCTET_OUTPUT_INDICES;
	unsigned char res = decode_fn(indices);
	TEST_CHECK(res == exp);
	TEST_MSG("{ 0x%2X, 0x%2X, 0x%2X, 0x%2X } gives %d, not %d", indices[0], indices[1], indices[2], indices[3], res, exp);
}

void
decode_first_byte_test(void) {
	decode_byte_test(decode_first_byte, 'M');
}

void
decode_second_byte_test(void) {
	decode_byte_test(decode_second_byte, 'a');
}

void
decode_third_byte_test(void) {
	decode_byte_test(decode_third_byte, 'n');
}

void
fail_decode_if_given_garbage(void) {
	char encoded[] = "[[[[";
	unsigned char raw[3];
	int decode_result = base64_decode(encoded, raw);

	TEST_CHECK_(decode_result < 0, "%d", decode_result);
	TEST_MSG("Garbage should return with a negative value");
}

void
fail_decode_if_given_whitespace(void) {
	char encoded[] = "    ";
	unsigned char raw[3];
	int decode_result = base64_decode(encoded, raw);

	TEST_CHECK_(decode_result < 0, "%d", decode_result);
	TEST_MSG("whitespace should return a negative value");
}

void
decode_n_bytes(char* encoded, int bytes_exp, const char* exp_str) {
	unsigned char raw[5] = "";
	int bytes_read = base64_decode(encoded, raw);
	TEST_CHECK(bytes_read == bytes_exp);
	TEST_MSG("Should return %d bytes read, returned %d", bytes_exp, bytes_read);
	TEST_CHECK(strcmp((char*)raw, exp_str) == 0);
	TEST_MSG("unencoded should return `%s`, not `%s`", exp_str, raw);
}

void
decode_3_bytes(void) {
	char encoded[] = THREE_OCTET_OUTPUT;
	decode_n_bytes(encoded, 3, "Man");
}

void
decode_2_bytes(void) {
	char encoded[] = "TWE=";
	decode_n_bytes(encoded, 2, "Ma");
}

void
decode_1_byte(void) {
	char encoded[] = "TQ==";
	decode_n_bytes(encoded, 1, "M");
}

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ "smaller_input_sizes", smaller_input_sizes },
	{ "string to octet conversion functions", string_to_octet_conversion_functions },
	{ "Last sextet is properly masked", last_sextet_masks_correctly },
	{ "get_b64_index_returns_neg1_for_garbage", get_b64_index_returns_neg1_for_garbage },
	{ "ascii_digit_to_int_works", ascii_digit_to_int_works },
	{ "ascii_lowercase_to_int_works", ascii_lowercase_to_int_works },
	{ "ascii_uppercase_to_int_works", ascii_uppercase_to_int_works },
	{ "get_b64_index_works_with_b64", get_b64_index_works_with_b64 },
	{ "get_b64_index_accepts_padding", get_b64_index_accepts_padding },
	{ "get_b64_indices_fills_indices", get_b64_indices_fills_indices },
	{ "get_b64_indices_fails_with_garbage", get_b64_indices_fails_with_garbage },
	{ "get_b64_indices_handles_bad_padding", get_b64_indices_handles_bad_padding },
	{ "get_b64_indices_returns_num", get_b64_indices_returns_num },
	{ "decode_first_byte_test", decode_first_byte_test },
	{ "decode_second_byte_test", decode_second_byte_test },
	{ "decode_third_byte_test", decode_third_byte_test },
	{ "fail_decode_if_given_garbage", fail_decode_if_given_garbage },
	{ "fail_decode_if_given_whitespace", fail_decode_if_given_whitespace },
	{ "decode_3_bytes", decode_3_bytes },
	{ "decode_2_bytes", decode_2_bytes },
	{ "decode_1_byte", decode_1_byte },
	{ NULL, NULL },
};
