package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Type.*

  def typeCheck(expr: Expr, tenv: TypeEnv): Type = expr match
    case Num(_) => NumT
    case Bool(_) => BoolT
    case Add(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("type error")
    case Mul(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("type error")
    case Div(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("type error")
    case Mod(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("type error")
    case Eq(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => BoolT
      case _ => error("type error")
    case Lt(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => BoolT
      case _ => error("type error")
    case Val(x, i, b) =>
      val t1 = typeCheck(i, tenv)
      val t2 = typeCheck(b, tenv + (x -> t1)) 
      t2
    case Id(x) => tenv.getOrElse(x, error("type error"))
    case Fun(p, pty, body) => ArrowT(pty, typeCheck(body, tenv + (p -> pty)))
    case App(f, e) => (typeCheck(f, tenv), typeCheck(e, tenv)) match
      case (ArrowT(p, r), a) if p == a => r
      case _ => error("type error")
    // case Rec
    case Rec(x, p, pty, rty, b, s) => 
      if (typeCheck(b, tenv + (x -> ArrowT(pty, rty)) + (p -> pty)) == rty)
        typeCheck(s, tenv + (x -> ArrowT(pty, rty)) + (p -> pty))
      else error("type error")

    case If(c, t, e) => typeCheck(c, tenv) match
      case BoolT => (typeCheck(t, tenv), typeCheck(e, tenv)) match
          case (t1, t2) if t1 == t2 => t1
          case _ => error("type error")
      case _ => error("type error")

    def interp(expr: Expr, env: Env): Value = expr match
      case Num(n) => NumV(n)
      case Bool(b) => BoolV(b)
      case Add(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => NumV(l + r)
        case _ => error("type error")
      case Mul(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => NumV(l * r)
        case _ => error("type error")
      case Div(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => if (r != 0) NumV(l / r) else error("invalid operation")
        case _ => error("type error")
      case Mod(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => if (r != 0) NumV(l % r) else error("invalid operation")
        case _ => error("type error")
      case Eq(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => BoolV(l == r)
        case _ => error("type error")
      case Lt(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => BoolV(l < r)
        case _ => error("type error")

      case Val(x, i, b) => interp(b, env + (x -> interp(i, env)))
      case Id(x) => env.getOrElse(x, error("free identifier"))

      case Fun(p, t, b) => CloV(p, b, () => env)

      case Rec(x, p, pty, rty, b, s) => 
        lazy val newEnv: Env = env + (x -> CloV(p, b, () => newEnv))
        interp(s, newEnv)
        
      case App(f, e) => interp(f, env) match
        case CloV(p, b, fenv) => interp(b, fenv() + (p -> interp(e, env)))
        case _ => error("error")
      case If(c, t, e) => interp(c, env) match
        case BoolV(true) => interp(t, env)
        case BoolV(false) => interp(e, env)
        case _ => error("error")
}
