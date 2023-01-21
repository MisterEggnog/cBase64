#!/usr/bin/env python
from random import seed, randint
from base64 import standard_b64encode
from typing import Tuple

def generate_b64() -> Tuple[int, str, str, str]:
	print(2**24 - 1)
	data = randint(0, 2**24 - 1)
	data_bytes = bytearray(data.to_bytes(3, byteorder='big'))
	
	three_bytes = standard_b64encode(data_bytes)
	data_bytes.pop()
	two_bytes = standard_b64encode(data_bytes)
	data_bytes.pop()
	one_bytes = standard_b64encode(data_bytes)
	
	return (data, str(three_bytes, 'utf-8'), str(two_bytes, 'utf-8'), str(one_bytes, 'utf-8'))

def b64_print(tup):
	print("{:03X}|{}|{}|{}".format(tup[0], tup[1], tup[2], tup[3]))

seed(6)
b64 = generate_b64()
print(b64)
b64_print(b64)
