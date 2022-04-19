import os
import sys
import time
import hashlib

target_hash = input("Input a target hash: ")

numbers = '1234567890'
uptext = 'qwertyuiopasdfghjklzxcvbnm'
lotext = 'QWERTYUIOPASDFGHJKLZXCVBNM'

steach = uptext + lotext

lash_ash = open('log', 'w')
found_hash = open('saved', 'w')

def animate():
	print(f"[{gen_text_hash}] is [{gen_raw_text}]", end="\r")


counter = 0
while True:
	for i in range(len(steach)):
	gen_raw_text = steach[counter]
	gen_text_hash = hashlib.md5(str(gen_raw_text).encode()).hexdigest()
	animate()
	if gen_text_hash == target_hash:
		print(f"[{gen_raw_text}] is [{gen_text_hash}]")
		found_hash.write(f"[{gen_text_hash}] is [{gen_raw_text}]")
		break
	
	counter += 1
