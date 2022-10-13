
#include "CBase64Stream.h"

static int size4_encode(const unsigned char* input, Base64Quard* dest);

void
base64_encode(const unsigned char input[], size_t len, char dest[]) {
	int res;
	switch (len) {
		case 4:
		case 3:
		case 2:
		case 1:
		default:
			res = 0;
	}
	return res;
}

static int
size4_encode(const unsigned char* input, Base64Quard* dest) {
	return 0;
}
