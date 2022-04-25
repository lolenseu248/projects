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

def hash_found(gen_raw_text, gen_text_hash):
		print(f"[{gen_raw_text}] is [{gen_text_hash}]")
		found_hash.write(f"[{gen_text_hash}] is [{gen_raw_text}]")

counter = 0
found = False
while not found:
	for i in range(len(steach)):
		gen_raw_text = counter #steach[counter]
		gen_text_hash = hashlib.md5(str(gen_raw_text).encode()).hexdigest()
		animate()
			
		if gen_text_hash == target_hash:
			hash_found(gen_raw_text, gen_text_hash)
			found = True
			break

		counter += 1

	if found == True:
		break


