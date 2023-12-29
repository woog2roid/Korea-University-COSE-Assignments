package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Cont.*

  def reduce(k: Cont, s: Stack): (Cont, Stack) = (k, s) match
    case (EmptyK, List(v)) => (EmptyK, List(v))
    case (EvalK(env, expr, k), s) => expr match
      case Num(n) => (k, NumV(n) :: s)
      case Add(l, r) => (EvalK(env, l, EvalK(env, r, AddK(k))), s)
      case Mul(l, r) => (EvalK(env, l, EvalK(env, r, MulK(k))), s)
      case Id(x) => (k, env.getOrElse(x, error("free identifier")) :: s)
      case Fun(p, b) => (k, CloV(p, b, env) :: s)
      case App(f, e) => (EvalK(env, f, EvalK(env, e, AppK(k))), s)
      case Vcc(x, b) => (EvalK(env + (x -> ContV(k, s)), b, k), s)
    case (AddK(k), r :: l :: s) => (k, numAdd(l, r) :: s)
    case (MulK(k), r :: l :: s) => (k, numMul(l, r) :: s)
    // application
    case (AppK(k), v :: v1 :: s) => v1 match
      case CloV(p, b, fenv) => (EvalK(fenv + (p -> v), b, k), s)
      case ContV(k1, s1) => (k1, v :: s1)
      case _ => error("not a function")

  /* Arithmetic Operation Helper functions */
  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(l: Value, r: Value): Value = (l, r) match
    case (NumV(l), NumV(r)) => {
      if (x == "/" && r == 0) error(s"invalid operation")
      else if (x == "%" && r == 0) error(s"invalid operation")
      else NumV(op(l, r))
    }
    case (l, r) => error(s"invalid operation")

  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
}
