# Rezepte

## Wordcount 
Classic MapReduce pattern.
- `map`
- `reduceByKey`
```scala
rdd.map(x => (x, 1))
    .reduceByKey(_ + _)
//  .reduceByKey( (a, b) => a+b )
```

## Durchschnitt pro Key
Bsp: Abteilungen mit Gehaltslisten fuer Mitarbeiter. Fuer jede Abteilung
Durchschnittliches Gehalt ermitteln.

- `map` -> Tupel bauen mit Abteilung als key das auf Tupel mit Gehalt und 1 enthaelt zeigt.
Die 1 ist zum Mitzaehlen, wie viele Gehaelter es fuer eine Abteilung gibt.
- `reduceByKey` -> Fuer alle keys, die uebereinstimmen, die Gehaelter und die '1'
aufaddieren. -> RDD mit (Abteilung, (Gesamtgehalt, Num_Gehaelter)) als Resultat.
- `mapValues` -> Alle values fuer jeden key durch gehen und Gesamtgehalt/Num_Gehaelter.

```scala
val sc = spark.sparkContext

val mitarbeiter = sc.parallelize(List(
       ("Vertrieb", "Anna", 3200), ("IT", "Ben", 4100),
       ("Vertrieb", "Clara", 2900), ("IT", "David", 4500),
       ("HR", "Emma", 3600)
    ))

mitarbeiter.map( mitarb => (mitarb._1, (mitarb._3, 1) ) ).
    reduceByKey( (a,b) => (a._1 + b._1, a._2 + b._2) ).
    mapValues( valueTuple => valueTuple._1 / valueTuple._2 ).collect()
```

## Min/Max mit reduce
```scala
rdd.reduce((a, b) => if (a._2 > b._2) a else b)
```

## Tokenizing text
It is like `map` and then `flatten` -> One input, many outputs.

```scala
val lines = sc.parallelize(Seq(
  "hello world",
  "spark scala"
))

val words =
  lines.flatMap(_.split("\\s+"))

words.collect()
// hello world spark scala
```

## Only transform values
```scala
val rdd =
  sc.parallelize(
    Seq(
      ("A",1),
      ("B",2)
    )
  )

rdd
  .mapValues(_ * 10)
  .collect()
```

## Distinct via map
```scala
val words = sc.parallelize(Seq(
  "a","a","b","c","b"
))

words
  .map( (_,()) )
  .reduceByKey((_,_) => ())
  .keys
  .collect()
```
This is the same as using `distinct`:
```scala
val words = sc.parallelize(Seq(
  "a","a","b","c","b"
))

words.distinct().collect()
```

