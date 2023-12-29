package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Cont.*

  def interpCPS(expr: Expr, env: Env, k: Value => Value): Value = expr match
    case Num(n) => k(NumV(n))
    case Add(l, r) =>
      interpCPS(l, env, {
        lv => interpCPS(r, env, {
          rv => k(numAdd(lv, rv))
        })
      })
    case Mul(l, r) =>
      interpCPS(l, env, {
        lv => interpCPS(r, env, {
          rv => k(numMul(lv, rv))
        })
      })
    case Id(x) => k(lookupId(x, env))
    case Fun(p, b) => k(CloV(p, b, env))
    case App(f, e) => interpCPS(f, env, fv => fv match
      case CloV(p, b, fenv) => interpCPS(e, env, ev => 
        interpCPS(b, fenv + (p -> ev), k))
      case _ => error(s"not a function")
    )
  
  def lookupId(x: String, env: Env): Value = env.getOrElse(x, error(s"free identifier: $x"))

  def reduce(k: Cont, s: Stack): (Cont, Stack) = (k, s) match
    case (EmptyK, s) => (EmptyK, s)
    case (EvalK(env, expr, k), s) => expr match
      case Num(n) => (k, NumV(n) :: s)
      case Add(l, r) => (EvalK(env, l, EvalK(env, r, AddK(k))), s)
      case Mul(l, r) => (EvalK(env, l, EvalK(env, r, MulK(k))), s)
      case Id(x) => (k, lookupId(x, env) :: s)
      case Fun(p, b) => (k, CloV(p, b, env) :: s)
      case App(f, e) => (EvalK(env, f, EvalK(env, e, AppK(k))), s)
    case (AddK(k), r :: l :: s) => (k, numAdd(l, r) :: s)
    case (MulK(k), r :: l :: s) => (k, numMul(l, r) :: s)
    case (AppK(k), v :: CloV(p, b, fenv) :: s) => (EvalK(fenv + (p -> v), b, k), s)
    case (AppK(k), v :: v1 :: s) => error(s"not a function")

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
