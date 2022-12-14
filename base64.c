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
	unsigned char local_input[3] = { input[0] };
	dest[3] = dest[2] = PADDING;

	switch (len) {
		case 3:
			local_input[2] = input[2];
			dest[3] = BASE64_DIGITS[fourth_sextet(local_input)];
		case 2:
			local_input[1] = input[1];
			dest[2] = BASE64_DIGITS[third_sextet(local_input)];
		case 1:
			dest[1] = BASE64_DIGITS[second_sextet(local_input)];
			dest[0] = BASE64_DIGITS[first_sextet(local_input)];
			break;
		default:
			break;
	}
}
