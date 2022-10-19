
#include "base64.h"

static void size4_encode(const unsigned char* input, char* dest);

static inline unsigned char first_sextet(const unsigned char* input) {
	return input[0] >> 2;
}

void
base64_encode(const unsigned char input[], size_t len, char dest[]) {
	switch (len) {
		case 4:
		case 3:
		case 2:
		case 1:
		default:
			break;
	}
}

static void
size4_encode(const unsigned char* input, char* dest) {
}
