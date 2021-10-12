
#include "CBase64Stream.h"

#define BASE64_DIGITS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/"
#define PADDING "="

static int size4_encode(const unsigned char* input, Base64Quard* dest);

int
base64_encode(const unsigned char* input, size_t len, Base64Quard* dest) {
	int res;
	switch (len) {
		case 4:
			res = size4_encode(input, dest);
			break;
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
