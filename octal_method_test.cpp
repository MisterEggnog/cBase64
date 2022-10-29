#include "base64.h"
#include <acutest.h>
#include <sstream>
#include <random>
#include <iomanip>

unsigned int
get_octal_int(unsigned char input[]) {
	 return (unsigned int)input[0] << 16 | (unsigned int)input[1] << 8 | (unsigned int)input[2];
}

std::string
get_octal_form(unsigned int input) {
	auto string_builder = std::ostringstream();
	string_builder << std::oct << std::setfill('0') << std::setw(8)
		<< input;
	return string_builder.str();
}

// output must be 4 bytes
// Will be written to
void
octal_encode(const std::string& octal, unsigned* output) {
}

void
encoding_same_as_octal_method() {
	auto rand = std::minstd_rand(7);
	auto dist = std::uniform_int_distribution<unsigned char>(0);

	for (int i = 0; i < 2; i++) {
		unsigned char input[] = { dist(rand), dist(rand), dist(rand) };
		unsigned encoded_indices[4];
		auto joined_int = get_octal_int(input);
		auto octal_form = get_octal_form(joined_int);

	}
}

void
get_octal_form_test() {
	unsigned int input = 0x4d616e; // "Man"
	auto octal_form_output = "23260556";
	auto result = get_octal_form(input);
	TEST_CHECK(result == octal_form_output);
	TEST_MSG("get_octal returned %s", result.c_str());
}

void
get_octal_int_test() {
	unsigned char input[] = { 0x12, 0x34, 0x56 };
	auto result = get_octal_int(input);
	TEST_CHECK(result == 0x123456);
	TEST_MSG("get_octal_int returned %X", result);
}

TEST_LIST = {
	{ "get octal int test", get_octal_int_test },
	{ "get octal form test", get_octal_form_test },
	{ "Library encoding provides same result as octal method", encoding_same_as_octal_method },
	{ NULL, NULL },
};
