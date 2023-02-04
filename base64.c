/*
 * Copyright 2022 Josiah Baldwin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "base64.h"
#include <ctype.h>

static inline unsigned char first_sextet(const unsigned char* input) {
	return input[0] >> 2;
}

static inline unsigned char second_sextet(const unsigned char* input) {
	return (0x03 & input[0]) << 4 | input[1] >> 4;
}

static inline unsigned char third_sextet(const unsigned char* input) {
	return (0x0F & input[1]) << 2 | input[2] >> 6;
}

static inline unsigned char fourth_sextet(const unsigned char* input) {
	return 63 & input[2];
}

void
base64_encode(const unsigned char input[], size_t len, char dest[]) {
	dest[3] = dest[2] = PADDING;

	switch (len) {
		case 3:
			dest[3] = BASE64_DIGITS[fourth_sextet(input)];
		case 2:
			dest[2] = BASE64_DIGITS[third_sextet(input)];
		case 1:
			dest[1] = BASE64_DIGITS[second_sextet(input)];
			dest[0] = BASE64_DIGITS[first_sextet(input)];
			break;
		default:
			break;
	}
}

static inline int
in_alphabet(char c) {
	return isalnum(c) || c == '+' || c == '/' || c == PADDING;
}

static int
ascii_digit_to_int(char n) {
	return n - '0';
}

static int
ascii_lowercase_to_int(char n) {
	return n - 'a';
}

static int
ascii_uppercase_to_int(char n) {
	return n - 'A';
}

#define BAD_CHAR (-1)
#define PADDING_ERR (BAD_CHAR - 1)

static int
get_b64_index(char c) {
	if (isupper(c)) {
		return ascii_uppercase_to_int(c);
	} else if (islower(c)) {
		return ascii_lowercase_to_int(c) + 26;
	} else if (isdigit(c)) {
		return ascii_digit_to_int(c) + 52;
	} else if (c == '+') {
		return 62;
	} else if (c == '/') {
		return 63;
	} else {
		return BAD_CHAR;
	}
}

static int
get_b64_indices(const char encoded[static 4], char indices[static 4]) {
	for (int i = 0; i < 4; i++) {
		indices[i] = get_b64_index(encoded[i]);
		if (indices[i] < 0)
			return BAD_CHAR;
	}
}

size_t
base64_decode(const char encoded[], unsigned char raw[]) {
	// TODO
}
