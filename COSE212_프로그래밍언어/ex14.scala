package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Type.*

  def typeCheck(expr: Expr, tenv: TypeEnv): Type = expr match
    case Num(_) => NumT
    case Add(l, r) => 
      isSubtype(typeCheck(l, tenv), NumT)
      isSubtype(typeCheck(r, tenv), NumT)
      NumT
    case Mul(l, r) => 
      isSubtype(typeCheck(l, tenv), NumT)
      isSubtype(typeCheck(r, tenv), NumT)
      NumT
    case Id(x) => tenv.getOrElse(x, error("type error(3)"))
    case Val(x, None, i, b) =>
      val ity = typeCheck(i, tenv)
      typeCheck(b, tenv + (x -> ity))
    case Val(x, Some(ity), i, b) => 
      val ity2 = typeCheck(i, tenv)
      isSubtype(ity2, ity)
      typeCheck(b, tenv + (x -> ity))
    case Fun(p, ty, body) => 
      mustValid(ty, tenv)
      ArrowT(ty, typeCheck(body, tenv + (p -> ty)))
    case App(f, a) => (typeCheck(f, tenv), typeCheck(a, tenv)) match
      case (ArrowT(paramTy, retTy), argTy) =>
        isSubtype(argTy, paramTy)
        retTy
      case _ => error("type error(4)")
    case Record(fs) =>
      RecordT(
        fs.map((x, e) => (x, typeCheck(e, tenv))).toMap
      )
    case Access(record, field) =>
      typeCheck(record, tenv) match
        case RecordT(fields) => fields.getOrElse(field, error("type error(5)"))
        case _ => error("type error(6)")
    case Exit => BotT

  // aty <: bty 
  def isSubtype(aty: Type, bty: Type): Boolean = (aty, bty) match
    case (BotT, _) => true
    case (_, TopT) => true
    case (NumT, NumT) => true
    case (ArrowT(ap, ar), ArrowT(bp, br)) =>
      isSubtype(bp, ap) && isSubtype(ar, br)
    case (RecordT(afields), RecordT(bfields)) =>
      bfields.map((x, bty) => afields.get(x) match
        case None => error("type error(7)")
        case Some(aty) => isSubtype(aty, bty)
      ).foldLeft(true)(_ && _)
    case _ => error("type error(8)")

    
  def mustValid(ty: Type, tenv: TypeEnv): Type = ty match
    case NumT => NumT
    case ArrowT(paramTy, retTy) => ArrowT(mustValid(paramTy, tenv), mustValid(retTy, tenv))
    case RecordT(fields) => RecordT(fields.keys.zip(fields.values.map(mustValid(_, tenv))).toMap)
    case BotT => BotT
    case TopT => TopT
    case _ => error("type error(9)")

  def interp(expr: Expr, env: Env): Value = expr match
    case Num(n) => NumV(n)
    case Add(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(lv), NumV(rv)) => NumV(lv + rv)
      case _ => error()
    case Mul(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(lv), NumV(rv)) => NumV(lv * rv)
      case _ => error()
    case Fun(p, ty, body) => CloV(p, body, env)
    case Val(x, _, i, b) => interp(b, env + (x -> interp(i, env)))
    case Id(x) => env.getOrElse(x, error())
    case App(f, a) => interp(f, env) match
      case CloV(p, b, fenv) => 
        val v = interp(a, env)
        interp(b, fenv + (p -> v))
      case _ => error()
    case Record(f) =>
      RecordV(f.map((x, e) => (x, interp(e, env))).toMap)
    case Access(r, f) =>
      interp(r, env) match
        case RecordV(fields) => fields.getOrElse(f, error())
    case _ => error()
}
