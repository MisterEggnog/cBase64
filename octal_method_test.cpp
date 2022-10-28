#include "base64.h"
#include <acutest.h>
#include <sstream>
#include <random>
#include <iomanip>

// input must be size 4
std::string
get_octal_form(unsigned char input[]) {
	auto twenty_fourbit = (unsigned int)input[0] << 16
		| (unsigned int)input[1] << 8
		| (unsigned int)input[0];
	auto string_builder = std::ostringstream();
	string_builder << std::oct << std::setfill('0') << std::setw(8) << twenty_fourbit;
	return string_builder.str();
}

void
encoding_same_as_octal_method() {
	auto rand = std::minstd_rand(7);
	auto dist = std::uniform_int_distribution<char>(-128);

	unsigned char input[4] = "";
	unsigned char output[4];
	unsigned int twenty_fourbit;

	for (int i = 0; i < 50; i++) {
		input[0] = dist(rand);
		input[1] = dist(rand);
		input[2] = dist(rand);
	}
}

void
get_octal_form_test() {
	unsigned char input[] = "Man";
	auto result = get_octal_form(input);
	TEST_CHECK(result == "19220546");
	TEST_MSG("get_octal returned %s", result.c_str());
}

TEST_LIST = {
	{ "get octal form test", get_octal_form_test },
	{ "Library encoding provides same result as octal method", encoding_same_as_octal_method },
	{ NULL, NULL },
};
