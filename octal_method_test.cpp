#include "base64.h"
#include <acutest.h>
#include <array>
#include <cstdio>
#include <sstream>
#include <random>
#include <iomanip>
#include <functional>

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
	std::sscanf(octal.c_str(), "%2o%2o%2o%2o", &(output[0]), &(output[1]),
		&(output[2]), &(output[3]));
}

std::string
encode_form(const unsigned* indices, int length) {
	auto encoded_str = std::string(4, '=');
	encoded_str.at(0) = BASE64_DIGITS[indices[0]];
	encoded_str.at(1) = BASE64_DIGITS[indices[1]];
	if (length > 2)
		encoded_str.at(2) = BASE64_DIGITS[indices[2]];
	if (length == 4)
		encoded_str.at(3) = BASE64_DIGITS[indices[3]];
	return encoded_str;
}

inline bool
indices_same(char result, unsigned indice) {
	return result == BASE64_DIGITS[indice];
}

// size 4
inline bool
encoding_same(char result[], unsigned expected[]) {
	return indices_same(result[0], expected[0]) && indices_same(result[1], expected[1])
		&& indices_same(result[2], expected[2]) && indices_same(result[3], expected[3]);
}

using char_array = std::array<unsigned char, 3>;
using ArrayFiller = std::function<char_array(std::minstd_rand&)>;

void
encoding_same_as_octal_method(ArrayFiller fn, int length) {
	auto rand = std::minstd_rand(7);
	bool exit_early = false;

	for (int i = 0; i < 50; i++) {
		auto input = fn(rand);
		unsigned encoded_indices[4];
		auto joined_int = get_octal_int(input.data());
		auto octal_form = get_octal_form(joined_int);
		octal_encode(octal_form, encoded_indices);
		auto octal_str = encode_form(encoded_indices, length + 1);

		char result[5] = "";
		base64_encode(input.data(), length, result);

		exit_early = !TEST_CHECK_(octal_str == result,
			"Octal indices give different result than lib");
		TEST_MSG("octal_result is [%c(%d), %c(%d), %c(%d), %c(%d)]",
			octal_str[0], encoded_indices[0],
			octal_str[1], encoded_indices[1],
			octal_str[2], encoded_indices[2],
			octal_str[3], encoded_indices[3]);
		TEST_MSG("Library result \"%s\"", result);

		if (exit_early)
			break;
	}
}

void
three_byte_encoding() {
	encoding_same_as_octal_method([](auto& rng) {
		auto dist = std::uniform_int_distribution<unsigned char>(0);
		char_array arr { dist(rng), dist(rng), dist(rng) };
		return arr;
	}, 3);
}

void
two_byte_encoding() {
	encoding_same_as_octal_method([](auto& rng) {
		auto dist = std::uniform_int_distribution<unsigned char>(0);
		char_array arr { dist(rng), dist(rng), 0 };
		return arr;
	}, 2);
}

void
one_byte_encoding() {
	encoding_same_as_octal_method([](auto& rng) {
		auto dist = std::uniform_int_distribution<unsigned char>(0);
		char_array arr { dist(rng), 0, 0 };
		return arr;
	}, 1);
}

void
get_octal_form_test(unsigned input, const char* octal_form_output) {
	auto result = get_octal_form(input);
	TEST_CHECK(result == octal_form_output);
	TEST_MSG("get_octal returned %s", result.c_str());
}

void
get_octal_form_3_bytes_test() {
	unsigned int input = 0x4d616e; // "Man"
	auto octal_form_output = "23260556";
	get_octal_form_test(input, octal_form_output);
}

void
get_octal_int_test() {
	unsigned char input[] = { 0x12, 0x34, 0x56 };
	auto result = get_octal_int(input);
	TEST_CHECK(result == 0x123456);
	TEST_MSG("get_octal_int returned %X", result);
}

void
octal_encode_test(const std::string& input, unsigned expected[]) {
	unsigned result[4];
	octal_encode(input, result);

	for (int i = 0; i < 4; i++)
		TEST_CHECK(expected[i] == result[i]);
	TEST_MSG("Result is { %o, %o, %o, %o }", result[0], result[1], result[2], result[3]);
}

void
encodes_string_in_base64() {
	unsigned input[] = { 0, 0, 0, 0 };
	char results[][5] = { "0000", "000=", "00==" };
	std::string result;
	for (int i = 0; i < 3; i++) {
		TEST_CASE_("length %d", i);
		result = encode_form(input, 4 - i);
		TEST_CHECK(result == results[i]);
		TEST_MSG("Result was %s, should have been %s", result.c_str(), results[i]);
		TEST_MSG("Indices is %d", 4 - i);
	}
}

void
octal_encode_3_bytes_test() {
	auto input = std::string("11111111");
	unsigned expected[] = { 011, 011, 011, 011, };
	octal_encode_test(input, expected);
}

void
octal_fns_work_for_2_bytes() {
	TEST_CASE("24bit to octal string");
	{
		unsigned input = 0x4d6100; // "Ma"
		auto octal_form_output = "23260400";
		get_octal_form_test(input, octal_form_output);
	}

	TEST_CASE("octal string to indices");
	{
		auto input = std::string("23260400");
		unsigned expected[] = { 023, 026, 004, 000};
		octal_encode_test(input, expected);
	}
}

TEST_LIST = {
	{ "get octal int test", get_octal_int_test },
	{ "get octal form test", get_octal_form_3_bytes_test },
	{ "octal encode test", octal_encode_3_bytes_test },
	{ "encode_string_in_base64", encodes_string_in_base64 },
	{ "octal encode fns work for 2 bytes", octal_fns_work_for_2_bytes },
	{ "one_byte_encoding", one_byte_encoding },
	{ "two_byte_encoding", two_byte_encoding },
	{ "three_byte_encoding", three_byte_encoding },
	{ NULL, NULL },
};
