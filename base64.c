
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
	unsigned indices[] = { first_sextet(input), second_sextet(input), third_sextet(input), fourth_sextet(input) };

	for (int i = 0; i < 4; i++)
		dest[i] = BASE64_DIGITS[indices[i]];
}
