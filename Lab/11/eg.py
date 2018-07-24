import findspark
findspark.init()
from pyspark import SparkContext
'''
sc = SparkContext("local","wordCount")
text_RDD = sc.textFile("Computer Architecture - ShanghaiTech University.html")
counts_RDD = text_RDD.flatMap(lambda x: x.split(" ")).map(lambda x: (x, 1)).reduceByKey(lambda x,y: x+y)
print(counts_RDD.take(3))
'''
sc = SparkContext()
rdd = sc.parallelize([1,2,3,4])
temp = rdd.flatMap(lambda x: x)
print(temp.collect())