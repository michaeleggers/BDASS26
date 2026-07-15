# Lead tasks

`leads.csv` contains data from a CRM system.
Each row represents a potential customer (= lead).  
"Source" describes how the lead came to place (via chatbot, tradeshow, ...)  
"Deal Stage" describes how far the deal has progressed so far ("Closed Won", "Contacted", "Closed Lost", ...)

NOTE: There are duplicate account IDs on purpose to have something to work with.

## Helpers:

### Import
```scala
import org.apache.spark.sql.functions._
import org.apache.spark.mllib.rdd.RDDFunctions._
```

### Load CSV
```scala
val df = spark.read
  .option("header", "true")
  .option("inferSchema", "true")
  .csv("leads.csv")

df.show(5)
```

### Spark context
```scala
val sc = spark.sparkContext
```


## 1. Transform loaded CSF dataframe (`df`) into spark RDD.

## 2. Find all duplicate "Account Id"s. Print the ID and how many duplicates for that ID exist.

## 3. Sort all captured duplicated IDs by their key in ascending and then descending order.

## 4. Sort all captured duplicated IDs by value in ascending/descending order, (least to most duplicates).

## 4. List all unique "Deal Stage"s. Also count how many unique Deal Stages exist in total.

## 4. Count the number of rows in the table.

## 5. In the "Notes" column, find how many unique words appear.

## 6. Count number of words in "Notes" column combined.

## 7. List how often a word appears in the "Notes" column and list them in ascending order.

## 8. Same as task 6 or 7 but give the ratio for each word-appearance in percent. (Average appearance).

## 9. Group all words with same occurence-count.

## 10. Get names of unique companies.

## 11. Group "Company" by its "Lead Owner".

## 12. Get **all** emails as String RDD and print them.

## 13. Get **all** email domains and print them.





