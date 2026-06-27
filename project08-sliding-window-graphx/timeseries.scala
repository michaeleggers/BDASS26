import org.apache.spark.mllib.rdd.RDDFunctions._ 

val data = List( (0.0,3.0), (1.0,3.0), (2.0,3.0), (3.0,2.0), (4.0,1.0), (5.0,2.0), (6.0,2.0), (7.0,2.0), (8.0,3.0), (9.0,4.0) )
scala.util.Random.shuffle(data)

val dataSortedByTime = data.sortBy(_._1) // first _ is element, second _1 is element of tuple (first one)
val dataSortedByTime = data.sortBy( case (tv, v) => t)

// Get all tuples that have the same value three times in a row.
val plateauTriplets = dataSortedByTime.sliding(3).collect {
    case window if window.forall(_._2 == window(0)._2) => window // case: is pattern matching.
 }.flatten.toList

// Make it to RDD:
val plateauTripletsRDD = sc.parallelize(plateauTriplets)

// Problem now: plateauTripletsRDD is not sorted anymore: Tuples are not combined
// as triplets anymore.

