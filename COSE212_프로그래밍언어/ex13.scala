package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Type.*

  def typeCheck(expr: Expr, tenv: TypeEnv): Type = expr match
    case Num(_) => NumT
    case Add(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error(s"type error1")
    case Mul(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error(s"type error2")
    case Val(x, i, b) =>
      val t = typeCheck(i, tenv)
      typeCheck(b, tenv.addVar((x, t)))
    case Id(x) =>
      tenv.vars.getOrElse(x, error(s"type error3"))   
    case Fun(p, t, b) => 
      mustValid(t, tenv)
      val rty = typeCheck(b, tenv.addVar((p, t)))
      ArrowT(t, rty)
    case App(f, e) =>
      typeCheck(f, tenv) match
        case ArrowT(ptys, rty) =>
          val arguTy = typeCheck(e, tenv)
          mustSame(arguTy, ptys)
          rty
        case _ => error(s"type error4")
    // ??????????
    case TypeAbs(p, b) =>
      if (tenv.tys.contains(p)) error(s"type error5")
      val tenv1 = tenv.addType(p)
      PolyT(p, typeCheck(b, tenv1))
    case TypeApp(e, t) =>
      mustValid(t, tenv)
      typeCheck(e, tenv) match
        case PolyT(name, ty) => 
          subst(name, t, ty)
        case _ => error(s"type error6")
        
    // Helper Functions
  def subst(name: String, ty: Type, target: Type): Type = target match
    case NumT => NumT
    case ArrowT(pty, rty) =>
      ArrowT(subst(name, ty, pty), subst(name, ty, rty))
    case VarT(n) => if (n == name) ty else VarT(n)
    case PolyT(n, t) => if (n == name) PolyT(n, t) else PolyT(n, subst(name, ty, t))

  

  def mustSame(t1: Type, t2: Type): Unit = t1 match
    case NumT => if (t1 != t2) error(s"Type Mismatch: $t1 != $t2")
    case ArrowT(pty, rty) =>
      t2 match
        case ArrowT(pty2, rty2) =>
          mustSame(pty, pty2)
          mustSame(rty, rty2)
        case _ => error(s"Type Mismatch: $t1 != $t2")
    case VarT(name) =>
      t2 match
        case VarT(name2) =>
          if (name != name2) error(s"Type Mismatch: $t1 != $t2")
        case _ => error(s"Type Mismatch: $t1 != $t2")
    case PolyT(name, ty) =>
      t2 match
        case PolyT(name2, ty2) =>
          mustSame(ty, subst(name, VarT(name2), ty2))
        case _ => error(s"Type Mismatch: $t1 != $t2")
     

  def mustValid(ty: Type, tenv: TypeEnv): Type = ty match
    case NumT => NumT
    case ArrowT(pty, rty) =>
      ArrowT(mustValid(pty, tenv), mustValid(rty, tenv))
    case VarT(name) => 
      if(!tenv.tys.contains(name)) error(s"Type Error: $name is not a valid type")
      VarT(name)
    case PolyT(name, ty) => PolyT(name, mustValid(ty, tenv.addType(name)))

  def interp(expr: Expr, env: Env): Value = expr match
    case Num(n) => NumV(n)
    case Add(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(lv), NumV(rv)) => NumV(lv + rv)
      case _ => error(s"interp error")
    case Mul(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(lv), NumV(rv)) => NumV(lv * rv)
      case _ => error(s"interp error")
    case Val(x, i, b) =>
      val v = interp(i, env)
      interp(b, env + (x -> v))
    case Fun(p, t, b) => CloV(p, b, env)
    case Id(x) => env.getOrElse(x, error(s"interp error"))
    case App(f, e) => interp(f, env) match
      case CloV(p, b, fenv) =>
        interp(b, fenv + (p -> interp(e, env)))
      case _ => error(s"interp error")
    case TypeAbs(p, b) => TypeAbsV(p, b, env)
    case TypeApp(e, t) => interp(e, env) match
      case TypeAbsV(p, b, tenv) => 
        interp(b, tenv)
      case _ => error(s"interp error")
      
}
