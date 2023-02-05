
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
#define THREE_OCTED_OUTPUT_INDICES { 19, 22, 5, 46 }

typedef unsigned char(*sextet_func)(const unsigned char*);

void
string_to_octet_conversion_functions(void) {
	unsigned char input[] = THREE_OCTET_INPUT;
	unsigned output[] = THREE_OCTED_OUTPUT_INDICES;
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
in_alphabet_return_false_4_garbage(void) {
	TEST_CHECK(!in_alphabet(';'));
	TEST_MSG("in_alphabet should fail for \';\'");
}

void
in_alphabet_accepts_padding(void) {
	TEST_CHECK(in_alphabet(PADDING));
}

void
get_b64_index_returns_neg1_for_garbage(void) {
	TEST_CHECK(get_b64_index('$') == BAD_CHAR);
}

void
in_alphabet_works_when_b64(void) {
	char b64_chars[] = BASE64_DIGITS;
	for (int i = 0; i < strlen(b64_chars); i++) {
		TEST_CASE_("%c", b64_chars[i]);
		if (!TEST_CHECK_(in_alphabet(b64_chars[i]), "%c", b64_chars[i]))
			break;
	}
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
	TEST_CHECK(get_b64_indices("=2ii", indices) == PADDING_ERR);
	TEST_CHECK(get_b64_indices("A=ii", indices) == PADDING_ERR);
	TEST_CHECK(get_b64_indices("A2=i", indices) == PADDING_ERR);
}

void
fail_decode_if_given_garbage(void) {
	char encoded[] = "[[[[";
	unsigned char raw[3];

	TEST_CHECK(base64_decode(encoded, raw) < 0);
	TEST_MSG("Garbage should return with a negative value");
}

void
halt_decode_if_given_whitespace(void) {
	char encoded[] = "    ";
	unsigned char raw[3];

	TEST_CHECK(base64_decode(encoded, raw) == 0);
	TEST_MSG("whitespace should return 0");
}

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ "smaller_input_sizes", smaller_input_sizes },
	{ "string to octet conversion functions", string_to_octet_conversion_functions },
	{ "Last sextet is properly masked", last_sextet_masks_correctly },
	{ "get_b64_index_returns_neg1_for_garbage", get_b64_index_returns_neg1_for_garbage },
	{ "in_alphabet_return_false_4_garbage", in_alphabet_return_false_4_garbage },
	{ "in_alphabet_accepts_padding", in_alphabet_accepts_padding },
	{ "in_alphabet_works_when_b64", in_alphabet_works_when_b64 },
	{ "ascii_digit_to_int_works", ascii_digit_to_int_works },
	{ "ascii_lowercase_to_int_works", ascii_lowercase_to_int_works },
	{ "ascii_uppercase_to_int_works", ascii_uppercase_to_int_works },
	{ "get_b64_index_works_with_b64", get_b64_index_works_with_b64 },
	{ "get_b64_indices_fills_indices", get_b64_indices_fills_indices },
	{ "get_b64_indices_fails_with_garbage", get_b64_indices_fails_with_garbage },
	{ "get_b64_indices_handles_bad_padding", get_b64_indices_handles_bad_padding },
	{ "fail_decode_if_given_garbage", fail_decode_if_given_garbage },
	{ "halt_decode_if_given_whitespace", halt_decode_if_given_whitespace },
	{ NULL, NULL },
};
