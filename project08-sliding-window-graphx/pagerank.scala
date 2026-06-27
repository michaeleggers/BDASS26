import org.apache.spark.mllib.rdd.RDDFunctions._

val sc = spark.sparkContext

// Create vertices
val nodes = Seq((1L, 1.0),(2L, 0.0),(3L, 0.0))
val vertRDD = sc.parallelize(nodes)

// Create edges
val edges = Seq(Edge(1L, 2L, 0.5), Edge(1L,3L, 0.5), Edge(2L,1L, 0.6), Edge(2L,1L,0.4), Edge(3L,1L,1.0))
val edgeRDD = sc.parallelize(edges)

val defaultVert = (0.0)

// Create graph using GraphX
val graph = Graph(vertRDD, edgeRDD, defaultVert)

// Use GraphX' 'aggregateMessages' to compute pagerank.
//
// TODO: The following code works but does not compute the pagerank.
// It is just there to experiment.
val distRDD = graph.aggregateMessages[Double](
     triplet => { // Map
        triplet.sendToDst(triplet.srcAttr)
     },
     (a, b) => (a + b) // Reduce
)
distRDD.collect().foreach(println(_))
