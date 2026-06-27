import org.apache.spark.mllib.rdd.RDDFunctions._ 

val sc = spark.sparkContext

val data = List( (0.0,3.0), (1.0,3.0), (2.0,3.0), (3.0,2.0), (4.0,1.0), (5.0,2.0), (6.0,2.0), (7.0,2.0), (8.0,3.0), (9.0,4.0) )
val dataRand = scala.util.Random.shuffle(data)

val rdd = sc.parallelize(dataRand)

val rddSorted = rdd.sortBy(_._1) // first _ is element, second _1 is element of tuple (first one)
// Can also use this to sort by time. More readable imho:
val rddSorted = rdd.sortBy( { case (t, v) => t } )

// NOTE: RDD is lazily evaluated. So when printing rddSorted via:
// eg. sortedRDD.foreach(elem => print(elem)) will print the list
// in unsorted state. 'sortBy' is a TRANSFORMATION, this will not
// evaluate the RDD. It just creates a new one with the 'sortBy' transformation
// "attached".
// One must evaluate the RDD to actually apply the sortBy function:
// eg. via an ACTION such as 'collect': sortedRDD.collect().foreach(elem => print(elem))
// @see: https://rajanand.org/spark/spark-transformations-vs-actions

// Get all tuples that have the same value three times in a row.
// This is pretty fancy, I guess?
val plateauRDD = rddSorted.sliding(3).collect {
    case window if window.forall(_._2 == window(0)._2) => window // case: is pattern matching.
 }.flatten.toList 

// Maybe more readable:
// WARNING: Works but does *NOT* the right thing:
// It evaluates to bool inside collect predicate and we get a list of bools!
val plateauRDD = rddSorted.sliding(3).collect(
     | {case window => window(0)._2 == window(1)._2 && window(1)._2 == window(2)._2 })

// This actually works. It generates an Array where each entry is a tuple
// of (time, value) pairs.
val plateauRDD = rddSorted.sliding(3).filter({
     | case Array((t1,v1),(t2,v2),(t3,v3)) => v1 == v2 && v2 == v3})

// This also works:
val plateauRDD = rddSorted.sliding(3).filter({
     | case window => window(0)._2 == window(1)._2 && window(1)._2 == window(2)._2})

plateauRDD.collect().foreach(elem => println(elem.mkString(" -> ")))
// Prints:
// (0.0,3.0) -> (1.0,3.0) -> (2.0,3.0)
// (5.0,2.0) -> (6.0,2.0) -> (7.0,2.0)
