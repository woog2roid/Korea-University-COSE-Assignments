package kuplrg

/* RFAE(FAE with Recursion and Conditionals) */
/* Concepts: "Desugaring", "Recursion" */
object Implementation extends Template {

  import Expr.*
  import Value.*

  /* Note: No Val anymore. */
  def interp(expr: Expr, env: Env): Value = expr match {
    /* Basic */
    case Num(n) => NumV(n)
    case Bool(b) => BoolV(b)
    case Id(x) => env.getOrElse(x, error(s"free identifier"))
    /* AE */
    case Add(l, r) => numAdd(interp(l, env), interp(r, env))
    case Mul(l, r) => numMul(interp(l, env), interp(r, env))
    case Div(l, r) => numDiv(interp(l, env), interp(r, env))
    case Mod(l, r) => numMod(interp(l, env), interp(r, env))
    /* Conditionals */
    case Eq(l, r) => numEq(interp(l, env), interp(r, env))
    case Lt(l, r) => numLt(interp(l, env), interp(r, env))
    case If(c, t, e) => interp(c, env) match
      case BoolV(true) => interp(t, env)
      case BoolV(false) => interp(e, env)
      case _ => error(s"not a boolean")
    /* Functions */
    /* CloV's env type: () => Env*/
    case Fun(p, b) => CloV(p, b, () => env)
    case Rec(n, p, b, s) => 
      lazy val newEnv: Env = env + (n -> CloV(p, b, () => newEnv))
      interp(s, newEnv)
    case App(f, e) => interp(f, env) match
      case CloV(p, b, fenv) => interp(b, fenv() + (p -> interp(e, env)))
      case _ => error(s"not a function")
  }

  /* Arithmetic Operation Helper functions */
  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(l: Value, r: Value): Value = (l, r) match
    case (NumV(l), NumV(r)) => {
      if (x == "/" && r == 0) error(s"invalid operation ${l} $x ${r}")
      else if (x == "%" && r == 0) error(s"invalid operation ${l} $x ${r}")
      else NumV(op(l, r))
    }
    case (l, r) => error(s"invalid operation: ${l.str} $x ${r.str}")

  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
  val numDiv: (Value, Value) => Value = numBOp("/")(_ / _)
  val numMod: (Value, Value) => Value = numBOp("%")(_ % _)

  /* Arithmetic Comparison Helper functions */
  type NCOp = (BigInt, BigInt) => Boolean
  def numCOp(x: String)(op: NCOp)(l: Value, r: Value): Value = (l, r) match
    case (NumV(l), NumV(r)) => BoolV(op(l, r))
    case (l, r) => error(s"invalid operation: ${l.str} $x ${r.str}")
  
  val numEq: (Value, Value) => Value = numCOp("==")(_ == _)
  val numLt: (Value, Value) => Value = numCOp("<")(_ < _)

}
