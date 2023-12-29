package kuplrg

/* EX07: LFAE(FAE with Lazy Evaluation) */
/* Concepts: "Lazy Evaluation" */
object Implementation extends Template {

  import Expr.*
  import Value.*

  def interp(expr: Expr, env: Env): Value = expr match
    // numbers
    case Num(number: BigInt) => NumV(number)
    // additions
    case Add(left: Expr, right: Expr) => numAdd(strict(interp(left, env)), strict(interp(right, env)))
    // multiplications
    case Mul(left: Expr, right: Expr) => numMul(strict(interp(left, env)), strict(interp(right, env)))
    // identifier lookups
    case Id(name: String) => env.getOrElse(name, error(s"free identifier"))
    // anonymous (lambda) functions
    case Fun(param: String, body: Expr) => CloV(param, body, env)
    // function applications
    case App(fun: Expr, arg: Expr) => strict(interp(fun, env)) match
      case CloV(param, body, fenv) => interp(body, fenv + (param -> ExprV(arg, env)))
      case v => error(s"not a function")

  // Strict Evaluation
  def strict(v: Value): Value =  v match 
    case ExprV(e, env) => interp(e, env)
    case _ => v

  // Helper Function
  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(l: Value, r: Value): Value = (strict(l), strict(r)) match
    case (NumV(l), NumV(r)) => NumV(op(l, r))
    case (l, r) => error(s"invalid operation")
  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
}
