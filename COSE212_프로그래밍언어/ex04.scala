package kuplrg

/* FVAE(VAE with First-Class Function) */
/* Concepts "Value" */
object Implementation extends Template {

  import Expr.*
  import Value.*

  /* static scoping */
  /* without helper function*/
  def interp(expr: Expr, env: Env): Value = expr match {
    // Operations by NumV
    case Num(n) => NumV(n)
    case Add(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => NumV(l + r)
      case _ => error(s"invalid operation")
    case Mul(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => NumV(l * r)
      case _ => error(s"invalid operation")

    case Val(x, i, b) => interp(b, env + (x -> interp(i, env)))
    case Id(x) => env.getOrElse(x, error(s"free identifier"))

    // Function
    case Fun(p, b) => CloV(p, b, env)
    case App(f, e) => interp(f, env) match {
      case CloV(p, b, fenv) => interp(b, fenv + (p -> interp(e, env)))
      case _ => error(s"not a function")
    }
  }

  /* dynamic scoping */
  /* with helper function*/  
  def interpDS(expr: Expr, env: Env): Value = expr match {

    // Operations by NumV
    case Num(n) => NumV(n)
    case Add(l, r) => numAdd(interpDS(l, env), interpDS(r, env))
    case Mul(l, r) => numMul(interpDS(l, env), interpDS(r, env)) 

    case Val(x, i, b) => interpDS(b, env + (x -> interpDS(i, env)))
    case Id(x) => env.getOrElse(x, error(s"free identifier"))

    // Function
    case Fun(p, b) => CloV(p, b, env)
    case App(f, e) => interpDS(f, env) match {
      case CloV(p, b, fenv) => interpDS(b, env + (p -> interpDS(e, env)))
      case _ => error(s"not a function")
    }
  }

  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(l: Value, r: Value): Value = (l, r) match
    case (NumV(l), NumV(r)) => NumV(op(l, r))
    case (l, r) => error(s"invalid operation: ${l.str} $x ${r.str}")

  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
}
