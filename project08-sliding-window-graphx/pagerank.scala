import org.apache.spark.mllib.rdd.RDDFunctions._
import org.apache.spark.graphx._

val sc = spark.sparkContext

// Create vertices
val nodes = Seq((1L, 1.0),(2L, 1.0),(3L,1.0))
val vertRDD = sc.parallelize(nodes)

// Create edges
val edges = Seq(Edge(1L, 2L, 0.5),Edge(1L, 3L, 0.5),Edge(2L,3L,1.0),Edge(3L,1L,1.0))
val edgeRDD = sc.parallelize(edges)

val defaultVert = (0.0)

// Create graph using GraphX
// NOTE: Must be var to reassign later in iterative pagerank alg.
// val makes stuff immutable and is like a const.
var graph = Graph(vertRDD, edgeRDD, defaultVert)

// Use GraphX' 'aggregateMessages' to compute pagerank.
//
// TODO: The following code works but does not compute the pagerank.
// It is just there to experiment.
// prRDD = 'page rank RDD'
val prRDD = graph.aggregateMessages[Double](
   triplet => { // Map
      triplet.sendToDst(triplet.srcAttr)
   },
   (a, b) => (a + b) // Reduce
)

// This kinda works(?)
val prRDD = graph.aggregateMessages[Double](
   triplet => {
      triplet.sendToDst(triplet.srcAttr * triplet.attr)
   },
   (a, b) => (a)
)

// This actually does the right thing (I think)
// but without the chance of going to other page
// without direct link.
val prRDD = graph.aggregateMessages[Double](
   triplet => {
    triplet.sendToDst(triplet.srcAttr * triplet.attr)
   },
   (a,b) => (a + b)
)

// Print it:
prRDD.collect().foreach(println(_))

// 0.85 Dampening factor that gives the possibility to
// go to another page with prob = (1.0-0.85)/3.0 even
// when there is no link to that page from current page.
val prRDD = graph.aggregateMessages[Double](
      triplet => {
       triplet.sendToDst(triplet.srcAttr * triplet.attr * 0.85)
      },
      (a,b) => (a + b)
)

// Update the graph by using the messages above.
val updatedGraph = graph.outerJoinVertices(prRDD) {
     case (vertID: Long, rank: Double, Some(attrList)) => rank + (1.0-0.85)/3.0
     case (vertID: Long, rank: Double, None) => rank + (1.0-0.85)/3.0
}

// Here the second case pattern math is not needed because
// already handled by getOrElse. It unpacks the optional sumOpt
// (which is the sum of all received messages, that is the reduce
// part in aggregateMessages function) and is 0.0 if vertex
// hasn't received any messages at all.
val updatedGraph = graph.outerJoinVertices(prRDD) {
   case (vertID: Long, rank: Double, sumOpt) => sumOpt.getOrElse(0.0) + (1.0-0.85)/3.0
   }

// Print updated graph vertices:
updatedGraph.vertices.top(3).mkString("\n")

// Actual (working???) implementation.
// 0.85 is dampening factor.
// (1-d)/N is the chance of getting some contribution
// even if it never receives any messages (sumOpt is always 0.0).
for (i <- 1 to 100) {
  val contrib = graph.aggregateMessages[Double](
   triplet => {
       triplet.sendToDst(triplet.srcAttr * triplet.attr)
   },
   (a, b) => a + b
  )
  
  graph = graph.outerJoinVertices(contrib){
      case (vertID, currentRank, sumOpt) => {
         val sum = sumOpt.getOrElse(0.0)
         (1.0 - 0.85)/3.0 + 0.85 * sum
     }
  }
}
// Print the final page-rank result.
// Total probabilities per vertex should sum up
// to 1.0 approx.
graph.vertices.top(3).foreach(node => println(node))




