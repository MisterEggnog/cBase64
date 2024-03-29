/*
 * Simple base64 encoding/decoding library.
 * <https://github.com/MisterEggnog/cBase64>
 *
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
base64_encode(const unsigned char raw[], size_t len, char encoded[]) {
	unsigned char local[3] = "";
	encoded[3] = encoded[2] = PADDING;
	local[0] = raw[0];

	switch (len) {
		case 3: local[2] = raw[2];
		case 2: local[1] = raw[1];
		default: break;
	}

	switch (len) {
		case 3:
			encoded[3] = BASE64_DIGITS[fourth_sextet(local)];
		case 2:
			encoded[2] = BASE64_DIGITS[third_sextet(local)];
		case 1:
			encoded[1] = BASE64_DIGITS[second_sextet(local)];
			encoded[0] = BASE64_DIGITS[first_sextet(local)];
			break;
		default:
			break;
	}
}

static inline int
ascii_digit_to_int(char n) {
	return n - '0';
}

static inline int
ascii_lowercase_to_int(char n) {
	return n - 'a';
}

static inline int
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
	} else if (c == PADDING) {
		// padding is treated as 0 for conversion purposes
		return 0;
	} else {
		return BAD_CHAR;
	}
}

static int
get_b64_indices(const char encoded[static 4], char indices[static 4]) {
	int first_padding = 0;
	int byte_count = 4;
	for (int i = 0; i < 4; i++) {
		indices[i] = get_b64_index(encoded[i]);

		if (indices[i] < 0)
			return BAD_CHAR;

		if (encoded[i] == PADDING) {
			indices[i] = 0;
			byte_count--;
			if (first_padding == 0)
				first_padding = 4 - i;
		}
	}

	if (first_padding + byte_count != 4)
		return PADDING_ERR;

	return byte_count;
}

static inline unsigned char
decode_first_byte(char indices[static 4]) {
	return indices[0] << 2 | indices[1] >> 4;
}

static inline unsigned char
decode_second_byte(char indices[static 4]) {
	return (indices[1] & 0xF) << 4 | indices[2] >> 2;
}

static inline unsigned char
decode_third_byte(char indices[static 4]) {
	return (indices[2] & 3) << 6 | indices[3];
}

size_t
base64_decode(const char encoded[], unsigned char raw[]) {
	char indices[4];
	int byte_count = get_b64_indices(encoded, indices);

	if (byte_count < 0)
		return byte_count;

	raw[0] = decode_first_byte(indices);
	raw[1] = decode_second_byte(indices);
	raw[2] = decode_third_byte(indices);

	return byte_count - 1;
}
