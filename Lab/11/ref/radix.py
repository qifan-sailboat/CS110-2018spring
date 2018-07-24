import findspark
findspark.init()
import numpy as np
import time
from pyspark import SparkContext

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
    sc = SparkContext("local","wordcount")
    buck_RDD = sc.parallelize(array,base)

    maxval = max(array)

    # print("round ",-1)

    # print("len: ",len(array))

    # print(buck_RDD.take(20))

#     print(buck_RDD.collect())

    it = 0

    while base ** it <= maxval:

        # print("round ",it)

        buck_RDD=buck_RDD.map(lambda num: (((num // (base ** it)) % base),[num])).reduceByKey(lambda x,y: x+y).sortByKey().flatMap(lambda a:a[1])

#         print(buck_RDD.collect())

        it += 1

    return buck_RDD.collect()


if __name__ == "__main__":
    arr_to_sort = np.random.randint(0,100000,200, dtype=int)

    st = time.time()
    result1 = radix_sort(arr_to_sort.copy())
    ed = time.time() - st
    print("Wiki_radix takes %f"%ed)

    st = time.time()
    result2 = mapreduce_radix_sort(arr_to_sort.copy())
    ed = time.time() - st
    print("mapreduce_radix takes %f"%ed)

    print("Correct result?",np.array_equal(result1, result2))
