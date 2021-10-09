#ifndef CBASE64STREAM_H_INCLUDED
#define CBASE64STREAM_H_INCLUDED

typedef struct {
  char quad[5];
} Base64Quard;

// Convert 3 digits to base64
// If len > 3 function will return error.
// len < 3, there will be padding
// len == 0 is ubi
Base64Quard
base64_encode(unsigned char input[], size_t len);

// Convert 4 base64 numbers into data, return pointer to array of data.
// ret_len contains the number of bytes.
// DO NOT DEALLOCATE THE RETURNED DATA.
// DATA WILL CHANGE UPON CALL.
unsigned char*
base64_decode(const Base64Data* data, size_t* ret_len);

#endif // CBASE64STREAM_H_INCLUDED
