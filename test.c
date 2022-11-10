
#include "base64.c"
#include <string.h>
#include <acutest.h>

void
trivial_encode_4c(void) {
	unsigned char input[3] = "";
	char output[5] = "";


	base64_encode(input, 3, output);

	TEST_CHECK(strcmp(output, "0000") == 0);
	TEST_MSG("Trivial encoding should be `0000`, result was `%s`\n", output);
}

void
smaller_input_sizes(void) {
	unsigned char input[5] = "";
	char output[5] = "";
	char results[][5] = { "00==", "000=" };

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

TEST_LIST = {
	{ "trivial_encode_4c", trivial_encode_4c },
	{ "smaller_input_sizes", smaller_input_sizes },
	{ "string to octet conversion functions", string_to_octet_conversion_functions },
	{ "Last sextet is properly masked", last_sextet_masks_correctly },
	{ NULL, NULL },
};
