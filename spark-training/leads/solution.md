# Lead tasks solution


## 1. Transform loaded CSF dataframe (`df`) into spark RDD.
```scala
val leadsRDD = df.rdd
```

## 2. Find all duplicate "Account Id"s. Print the ID and how many duplicates for that ID exist.

```scala
val account_ids = leadsRDD.map(row => row.getAs[String]("Account Id"))
val duplicate_ids = account_ids.map(id => (id, 1)).reduceByKey( (a,b) => a+b ).filter{
     | case (id, count) => count > 1 }
duplicate_ids.collect().foreach(println)

// Number of duplicates
duplicated_ids.count()
```

## 3. Sort all captured duplicated IDs by their key in ascending and then descending order.

See: https://spark.apache.org/docs/latest/api/python/reference/api/pyspark.RDD.sortByKey.html

```scala
// Ascending (default)
duplicate_ids.soryByKey().collect().foreach(println)

// Descending
duplicate_ids.sortByKey(false).collect().foreach(println)
```

## 4. Sort all captured duplicated IDs by value in ascending/descending order, (least to most duplicates).
```scala
// Ascending
duplicate_ids.sortBy( tuple => tuple._2 ).collect().foreach(println)

// Descending
duplicate_ids.sortBy( tuple => tuple._2, false ).collect().foreach(println)
```

## 4. List all unique "Deal Stage"s. Also count how many unique Deal Stages exist in total.
Via `distinct` tranform (expensive because of implicit shuffle operation):
```scala
val distinct_deals = deals.distinct()
```

Via `groupByKey`:
```scala
val deals_with_number = deals.map( deal => (deal, 1) )
deals_with_number.groupByKey().count()
```

## 4. Count the number of rows in the table.
```scala
leads.count()
```

## 5. In the "Notes" column, find how many unique words appear.
```scala
val notes = leadsRDD.map( row => row.getAs[String]("Notes") )

```

## 6. Count number of words in "Notes" column combined.
```scala
notes.flatMap(note => note.split(" ")).count()
```

## 7. List how often a word appears in the "Notes" column and list them in ascending order.
```scala
notes.flatMap(note => note.split(" ")).map(word => (word, 1)).reduceByKey( (a, b) => a + b).sortBy(tuple => tuple._2).collect().foreach(println)
```

## 8. Same as task 6 or 7 but give the ratio for each word-appearance in percent. (Average appearance).
```scala
val word_to_ratio = notes.flatMap(note => note.split(" ")).map(word => (word, 1)).reduceByKey(
 (a, b) => a + b).sortBy(tuple => tuple._2).flatMap{
     | case (word, count) => List((word, count/1000.0*100.0))}
```

## 9. Group all words with same occurence-count.
```scala
val word_to_count = notes.flatMap(note => note.split(" ")).map(word => (word, 1)).reduceByKey( (a, b) => a + b).sortBy(tuple => tuple._2)
 word_to_count.map( tuple => (tuple._2, tuple._1)).groupByKey().sortByKey().collect().foreach(println)
```

## 9. Count how many leads per "Source" exist. So check how many leads came via "Chatbot", "Trade Show", etc.

## 10. Get names of unique companies.

## 11. Group "Company" by its "Lead Owner".

## 12. Get **all** emails as String RDD and print them.

## 13. Get **all** email domains and print them.





