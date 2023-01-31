
#include "base64.c"
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
	TEST_CHECK(get_b64_index('$') == OUT_OF_ALPHABET);
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

void
char_to_int_works(void) {
	for (char i = 0; i < 127; i++) {
		TEST_CHECK_((get_ascii_digit(i) >= 0) == isdigit(i), "ascii char %d", i);
	}
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
	{ "char_to_int_works", char_to_int_works },
	{ "fail_decode_if_given_garbage", fail_decode_if_given_garbage },
	{ "halt_decode_if_given_whitespace", halt_decode_if_given_whitespace },
	{ NULL, NULL },
};
