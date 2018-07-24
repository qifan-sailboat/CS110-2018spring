import numpy as np
import time

def radix_sort(array, base=10): 
	def list_to_buckets(array, base, iteration): 
		buckets = [[] for x in range(base)]
		for number in array:
			# Isolate the base-digit from the number
			digit = (number // (base ** iteration)) % base
			# Drop the number into the correct bucket
			buckets[digit].append(number)
		return buckets

	def buckets_to_list(buckets):
		numbers = []
		for bucket in buckets:
			# append the numbers in a bucket
			# sequentially to the returned array
			for number in bucket:
				numbers.append(number)
		return numbers

	maxval = max(array)

	it = 0
	# Iterate, sorting the array by each base-digit
	while base ** it <= maxval:
		array = buckets_to_list(list_to_buckets(array, base, it))
		it += 1

	return array

def mapreduce_radix_sort(array, base = 10):
	#TODO
	def list_to_buckets(array, base, iteration): 
		buckets = [[] for x in range(base)]
		for number in array:
			# Isolate the base-digit from the number
			digit = (number // (base ** iteration)) % base
			# Drop the number into the correct bucket
			buckets[digit].append(number)
		return buckets

	def buckets_to_list(buckets):
		numbers = []
		for bucket in buckets:
			# append the numbers in a bucket
			# sequentially to the returned array
			for number in bucket:
				numbers.append(number)
		return numbers

	maxval = max(array)

	it = 0
	# Iterate, sorting the array by each base-digit
	while base ** it <= maxval:
		array = buckets_to_list(list_to_buckets(array, base, it))
		it += 1

	return array


if __name__ == "__main__":
	arr_to_sort = np.random.randint(0,10000000,2000000, dtype=int)

	st = time.time()
	result1 = radix_sort(arr_to_sort.copy())
	ed = time.time() - st
	print("Wiki_radix takes %f"%ed)

	st = time.time()
	result2 = mapreduce_radix_sort(arr_to_sort.copy())
	ed = time.time() - st
	print("mapreduce_radix takes %f"%ed)

	print("Correct result?",np.array_equal(result1, result2))

