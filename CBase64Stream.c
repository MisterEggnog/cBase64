
#include "CBase64Stream.h"

static void size4_encode(const unsigned char* input, char* dest);

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
