package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Type.*

  def typeCheck(expr: Expr, tenv: TypeEnv): Type = 
    expr match
      case Num(_) => NumT
      case Add(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
        case (NumT, NumT) => NumT
        case _ => error("type error")
      case Mul(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
        case (NumT, NumT) => NumT
        case _ => error("type error")
      case Val(x, i, b) => typeCheck(b, tenv + (x -> typeCheck(i, tenv)))
      case Id(x) => tenv.getOrElse(x, error("type error"))
      case Fun(p, t, b) => ArrowT(t, typeCheck(b, tenv + (p -> t)))
      case App(f, e) => typeCheck(f, tenv) match
        case ArrowT(p, r) =>
           if (p == typeCheck(e, tenv)) r else error("type error")
        case _ => error("type error")

  def interp(expr: Expr, env: Env): Value = expr match
    case Num(n) => NumV(n)
    case Add(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(n1), NumV(n2)) => NumV(n1 + n2)
      case _ => error("type error")
    case Mul(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(n1), NumV(n2)) => NumV(n1 * n2)
      case _ => error("type error")
    case Val(x, i, b) => interp(b, env + (x -> interp(i, env)))
    case Id(x) => env.getOrElse(x, error("error: free identifier"))
    case Fun(p, t, b) => CloV(p, b, env)
    case App(f, e) => interp(f, env) match
      case CloV(p, b, fenv) => interp(b, fenv + (p -> interp(e, env)))
      case _ => error("type error")
}
