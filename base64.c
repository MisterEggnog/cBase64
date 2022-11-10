
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
