package kuplrg

object Implementation extends Template {

  // ---------------------------------------------------------------------------
  // Built-in Data Types => CLEAR
  // ---------------------------------------------------------------------------
  def sqsum(x: Int, y: Int): Int = x * x +  y * y

  def concat(left: String, right: String): String = left + right

  // ---------------------------------------------------------------------------
  // Funtions => CLEAR
  // function retruns "a function with Int => Int type"
  // ---------------------------------------------------------------------------  

  def subN(n: Int): Int => Int = x => x - n

  def twice(f: Int => Int): Int => Int = x => f(f(x))

  def compose(f: Int => Int, g: Int => Int): Int => Int = x => f(g(x))

  // ---------------------------------------------------------------------------
  // Lists
  // ---------------------------------------------------------------------------
  def sumOnlyOdd(l: List[Int]): Int = l match
    case Nil => 0
    case x :: xs => if x % 2 == 0 then sumOnlyOdd(xs)else x + sumOnlyOdd(xs)

  def foldWith(f: (Int, Int) => Int): List[Int] => Int = l => l.foldLeft(0)(f)

  def toSet(l: List[Int], from: Int): Set[Int] = l.drop(from).toSet

  // ---------------------------------------------------------------------------
  // Maps and Sets => CLEAR
  // ---------------------------------------------------------------------------
  def getOrZero(map: Map[String, Int], key: String): Int = map.getOrElse(key, 0)

  def setMinus(s1: Set[Int], s2: Set[Int]): Set[Int] = s1.diff(s2)

  // ---------------------------------------------------------------------------
  // Trees => CLEAR
  // ---------------------------------------------------------------------------
  import Tree.*

  def has(value: Int): Tree => Boolean = t => t match
    case Leaf(v) => v == value
    case Branch(l, v, r) => v == value || has(value)(l) || has(value)(r)

  def maxDepthOf(value: Int): Tree => Option[Int] = t => t match
    case Leaf(v) => if v == value then Some(0) else None
    case Branch(l, v, r) => 
      val lDepth = maxDepthOf(value)(l)
      val rDepth = maxDepthOf(value)(r)
      if lDepth == None && rDepth == None && v != value then None
      else if lDepth == None && rDepth == None && v == value then Some(0)
      else if lDepth == None then rDepth.map(_ + 1)
      else if rDepth == None then lDepth.map(_ + 1)
      else if lDepth.get > rDepth.get then lDepth.map(_ + 1) else rDepth.map(_ + 1)

  def mul(t: Tree): Int = t match
    case Leaf(v) => v
    case Branch(l, v, r) => mul(l) * v * mul(r)

  def countLeaves(t: Tree): Int = t match
    case Leaf(v) => 1
    case Branch(l, v, r) => countLeaves(l) + countLeaves(r)

  // postOrder needs stack => solve with recursion
  def postOrder(t: Tree): List[Int] = t match
    case Leaf(v) => List(v)
    case Branch(l, v, r) => postOrder(l) ++ postOrder(r) ++ List(v)

  // ---------------------------------------------------------------------------
  // Boolean Expressions => CLEAR
  // * expressed with tree architecture
  // ---------------------------------------------------------------------------
  import BE.*

  def countLiterals(expr: BE): Int = expr match
    case True => 1
    case False => 1
    case And(l, r) => countLiterals(l) + countLiterals(r)
    case Or(l, r) => countLiterals(l) + countLiterals(r)
    case Not(e) => countLiterals(e)

  def countNots(expr: BE): Int = expr match
    case True => 0
    case False => 0
    case And(l, r) => countNots(l) + countNots(r)
    case Or(l, r) => countNots(l) + countNots(r)
    case Not(e) => 1 + countNots(e)

  def depth(expr: BE): Int = expr match
    case True => 0
    case False => 0
    case And(l, r) => if depth(l) > depth(r) then depth(l) + 1 else depth(r) + 1
    case Or(l, r) => if depth(l) > depth(r) then depth(l) + 1 else depth(r) + 1
    case Not(e) => depth(e) + 1

  def getString(expr: BE): String = expr match
    case True => "true"
    case False => "false"
    case And(l, r) => "(" + getString(l) + " & " + getString(r) + ")"
    case Or(l, r) => "(" + getString(l) + " | " + getString(r) + ")"
    case Not(e) => "!" + getString(e)

  def eval(expr: BE): Boolean = expr match
    case True => true
    case False => false
    case And(l, r) => eval(l) && eval(r)
    case Or(l, r) => eval(l) || eval(r)
    case Not(e) => !eval(e)
}
