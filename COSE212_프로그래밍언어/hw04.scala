package kuplrg

import scala.collection.IndexedSeqView.Id

object Implementation extends Template {

  import Expr.*
  import RecDef.*
  import Value.*
  import Type.*
  import TypeInfo.*

  def typeCheck(expr: Expr, tenv: TypeEnv): Type = expr match
    case EUnit => UnitT
    case ENum(_) => NumT
    case EBool(_) => BoolT
    case EStr(_) => StrT
    case EId(x) => 
      if (tenv.vars.contains(x)) tenv.vars(x)
      else error(s"[Type Error] EId: Tenv not conatins id: $x .")
    case EAdd(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("[Type Error] EAdd")
    case EMul(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("[Type Error] EMul")
    case EDiv(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("[Type Error] EDiv")
    case EMod(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => NumT
      case _ => error("[Type Error] EMod")
    case EConcat(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (StrT, StrT) => StrT
      case _ => error("[Type Error] EConcat")
    case EEq(l, r) => 
      val (lty, rty) = (typeCheck(l, tenv), typeCheck(r, tenv))
      mustSame(lty, rty)
      BoolT
    case ELt(l, r) => (typeCheck(l, tenv), typeCheck(r, tenv)) match
      case (NumT, NumT) => BoolT
      case _ => error("[Type Error] ELt")
    case ESeq(l, r) => 
      typeCheck(l, tenv)
      typeCheck(r, tenv)
    case EIf(cond, thenExpr, elseExpr) =>
      typeCheck(cond, tenv) match
        case BoolT => 
          val (lty, rty) = (typeCheck(thenExpr, tenv), typeCheck(elseExpr, tenv))
          mustSame(lty, rty)
          lty
        case _ => error("[Type Error] EIf")
    case EVal(x, t, i, b) => t match
      case None =>
        typeCheck(b, tenv.addVar(x, typeCheck(i, tenv)))
      case Some(ty) =>
        mustSame(typeCheck(i, tenv), ty)
        typeCheck(b, tenv.addVar(x, ty))

    case EFun(ps, b) =>
      ps.foreach(p => mustValidType(p.ty, tenv))
      val penv = ps.map(p => p.name -> p.ty).toMap
      ArrowT(Nil, ps.map(_.ty), typeCheck(b, tenv.addVars(penv)))

    case EApp(f:Expr, tys:List[Type], args:List[Expr]) =>
      tys.foreach(ty => mustValidType(ty, tenv))
      typeCheck(f, tenv) match
        case ArrowT(tvars, ptys, rty) =>
          if (tvars.length == tys.length && ptys.length == args.length) 
            for (i <- 0 until args.length) 
              mustSame(typeCheck(args(i), tenv), substTys(ptys(i), tvars, tys))
            substTys(rty, tvars, tys)
          else error("[Type Error] EApp: Length Not Match")
        case _ => error(s"[Type Error] EApp: Not Arrow Type, ${typeCheck(f, tenv)}, $f")

    // 재귀 정의가 문제네 지금
    case ERecDefs(defs: List[RecDef], body: Expr) =>
      val newTenv = defs.foldLeft(tenv)((accTenv, d) => updateTenvForRecDef(accTenv, d))
      defs.foreach(d => rulesForRecDef(newTenv, d))
      mustValidType(typeCheck(body, newTenv), tenv)

    case EMatch(expr: Expr, mcases: List[MatchCase]) => typeCheck(expr, tenv) match
      //MatchCase(name: String, params: List[String], body: Expr):
      case IdT(name: String, tys: List[Type]) => tenv.tys(name) match
        case TIAdt(tvars: List[String], varts: Map[String, List[Param]]) =>
          if (mcases.length != varts.size) error("[Type Error] EMatch: Length Not Match")
          if (mcases.map(_.name).toSet != varts.keySet) error("[Type Error] EMatch: Name Not Match")
          
          val ttys = mcases.map(mc => {
            val types = varts(mc.name).map(p => substTys(p.ty, tvars, tys))
            val penv = mc.params.zip(types)
            val newTenv = tenv.addVars(penv)
            typeCheck(mc.body, newTenv)
          })

          ttys.foreach(mustSame(ttys(0), _))
          ttys(0)
        case _ => error("[Type Error] EMatch: Not an ADT")
      case _ => error("[Type Error] EMatch: Not a valid match expression")
      
    case EExit(ty: Type, expr: Expr) => 
      mustValidType(ty, tenv)
      mustSame(StrT, typeCheck(expr, tenv))
      ty
      
  def updateTenvForRecDef(tenv: TypeEnv, d: RecDef): TypeEnv = d match
    case LazyVal(name: String, ty: Type, init:Expr) =>
      tenv.addVar(name, ty)
    case RecFun(name: String, tvars: List[String], params:List[Param], rty:Type, body: Expr) =>
      tenv.addVar(name, ArrowT(tvars, params.map(_.ty), rty))
    
    // 마지막 테케가 IdT(Expr, List()) 형태임.
    // IdT를 tenv에 넣는 곳이 여기 밖에 없으니, 이곳에서 expr이 그대로 들어가는 로직이 있는지 확인
    case TypeDef(name: String, tvars: List[String], varts: List[Variant]) =>
      if (tenv.tys.contains(name)) error("[Type Error] updateTenvForRecDef: Duplicate(1)")
      val penv = varts.map {
        case v: Variant => // (name: String, params: List[Param])
        (v.name, ArrowT(tvars, v.params.map(_.ty), IdT(name, tvars.map(t => IdT(t, Nil)))))
      }.toMap
      tenv.addTypeName(name, tvars, varts).addVars(penv)
      
  def rulesForRecDef(tenv: TypeEnv, d: RecDef): Unit = d match
    case LazyVal(name, ty, init) =>
      mustSame(mustValidType(ty, tenv), typeCheck(init, tenv))
    case RecFun(name, tvars, params, rty, body) => 
      tvars.foreach(tvar => if (tenv.tys.contains(tvar)) error("[Type Error] rulesForRecDef"))
      val tenv2 = tenv.addTypeVars(tvars)
      params.foreach(p => mustValidType(p.ty, tenv2))
      mustSame(rty, typeCheck(body, tenv2.addVars(params.map(p => (p.name, p.ty)))))
    case TypeDef(name, tvars, varts) => 
      tvars.foreach(tvar => if (tenv.tys.contains(tvar)) error("[Type Error] rulesForRecDef"))
      val tenv2 = tenv.addTypeVars(tvars)
      varts.foreach(v => v.params.foreach(p => mustValidType(p.ty, tenv2)))

  def mustValidType(ty: Type, tenv: TypeEnv): Type = ty match
    case UnitT => UnitT
    case NumT => NumT
    case BoolT => BoolT
    case StrT => StrT
    case IdT(x: String, Nil) => 
      if (tenv.tys.contains(x)) IdT(x, Nil)
      else error("[Type Error] mustValidType: Nil")
    case IdT(x: String, tys: List[Type]) =>
      if (tenv.tys.contains(x))
        tenv.tys(x) match
          case TIAdt(tvars: List[String], variants: Map[String, List[Param]]) =>
            tys.foreach(ty => mustValidType(ty, tenv))
            IdT(x, tys)
          case _ => error("[Type Error] mustValidType: Idt, Type(1)")
      else error("[Type Error] mustValidType: Idt, Type(2)")
    case ArrowT(pvars: List[String], ptys: List[Type], rty: Type) => 
      val tenv2 = tenv.addTypeVars(pvars)
      ptys.foreach(ty => mustValidType(ty, tenv2))
      mustValidType(rty, tenv2)
      ArrowT(pvars, ptys, rty)

  def mustSame(lty: Type, rty: Type): Unit =
    if (!isSame(lty, rty))
      error(s"[Type Error] mustSame: $lty != $rty")
  
  def isSame(lty: Type, rty: Type): Boolean = (lty, rty) match
    case (UnitT, UnitT) => true
    case (NumT, NumT) => true
    case (BoolT, BoolT) => true
    case (StrT, StrT) => true
    case (IdT(x: String, tys:List[Type]), IdT(y, tys2)) => (tys, tys2) match
      case (Nil, Nil) => x == y
      case _ => 
        if (x == y) 
          if (tys.length == tys2.length)
            tys.zip(tys2).forall { case (item1, item2) => isSame(item1, item2) }
          else false
        else false
    case (ArrowT(tvars, ptys, rty :Type), ArrowT(tvars2, ptys2, rty2)) =>
      for (i <- 0 until ptys.length) 
        mustSame(ptys(i), substTys(ptys2(i), tvars2, tvars.map(t => IdT(t, Nil))))
      mustSame(rty, substTys(rty2, tvars2, tvars.map(t => IdT(t, Nil))))
      true
    case _ => false

  def substTy(ty: Type, name: String, subTy: Type): Type = ty match
    case UnitT => UnitT
    case NumT => NumT
    case BoolT => BoolT
    case StrT => StrT
    case IdT(x, Nil) => 
      if (name == x) subTy
      else IdT(x, Nil)
    case IdT(x, tys: List[Type]) =>
      IdT(x, tys.map(substTy(_, name, subTy)))
    case ArrowT(tvars, ptys, rty) => ArrowT(tvars, ptys.map(substTy(_, name, subTy)), substTy(rty, name, subTy))

  def substTys(bodyTy: Type, substNames: List[String], subTys: List[Type]): Type = 
    substNames
    .zip(subTys)
    .foldLeft(bodyTy)((bodyTy, x) => substTy(bodyTy, x._1, x._2))

  def interp(expr: Expr, env: Env): Value = expr match
    case EUnit => UnitV
    case ENum(n) => NumV(n)
    case EBool(b) => BoolV(b)
    case EId(x) => 
      val v = env.getOrElse(x, error(s"[Runtime Error] EId: $x"))
      v match
        case ExprV(e, env2) => interp(e, env2())
        case _ => v
    case EStr(s) => StrV(s)
    case EAdd(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => NumV(l + r)
      case _ => error("[Runtime Error] EAdd")
    case EMul(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => NumV(l * r)
      case _ => error("[Runtime Error] EMul")
    case EDiv(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => 
        if (r == 0) error("[Runtime Error] EDiv")
        else NumV(l / r)
      case _ => error("[Runtime Error] EDiv")
    case EMod(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => 
        if (r == 0) error("[Runtime Error] EMod")
        else NumV(l % r)
      case _ => error("[Runtime Error] EMod")
    case EEq(l, r) => eq(interp(l, env), interp(r, env))
    case ELt(l, r) => (interp(l, env), interp(r, env)) match
      case (NumV(l), NumV(r)) => BoolV(l < r)
      case _ => error("[Runtime Error] ELt")
    case EConcat(l, r) => (interp(l, env), interp(r, env)) match
      case (StrV(l), StrV(r)) => StrV(l + r)
      case _ => error("[Runtime Error] EConcat")
    case ESeq(left, right) => 
      interp(left, env)
      interp(right, env)
    case EIf(cond, thenExpr, elseExpr) => interp(cond, env) match
      case BoolV(true) => interp(thenExpr, env)
      case BoolV(false) => interp(elseExpr, env)
      case _ => error("[Runtime Error] EIf")
    case EVal(x, t, e, b) => t match
      case None =>
        val v = interp(e, env)
        interp(b, env + (x -> v))
      case Some(ty) =>
        val v = interp(e, env)
        interp(b, env + (x -> v))
    case EFun(ps: List[Param], b) =>
      CloV(ps.map(_.name), b, () => env)

    case EApp(f, tys, args) => interp(f, env) match
      case CloV(ps, b, fenv) =>
        val argvs = args.map(interp(_, env))
        val env2 = fenv() ++ ps.zip(argvs).map((x, v) => x -> v)
        interp(b, env2)
      case ConstrV(name) => 
        val argvs = args.map(interp(_, env))
        VariantV(name, argvs)
      case _ => error("[Runtime Error] EApp")    
    
    case ERecDefs(defs, body) =>
      lazy val newEnv: Env = defs.foldLeft(env)((accEnv, d) => {updateEnvForRecDef(accEnv, () => newEnv, d)})
      interp(body, newEnv)

    case EMatch(expr, mcases) => interp(expr, env) match
      case VariantV(name, argvs) => mcases.find(_.name == name) match
        case Some(MatchCase(name, params, body)) =>
          val env2 = env ++ params.zip(argvs).map((x, v) => x -> v)
          interp(body, env2)
        case None => error("[Runtime Error] EMatch")
      case _ => error("[Runtime Error] EMatch")
    case _ => error("[Runtime Error] interp")

  def updateEnvForRecDef(env: Env, lazyEnv: () => Env, d: RecDef): Env = d match
    case LazyVal(name, ty, init) => 
      lazy val newEnv: Env = env + (name -> ExprV(init, lazyEnv))
      newEnv
    case RecFun(name, tvars:List[String], params:List[Param], rty, body) =>
      lazy val newEnv: Env = env + (name -> CloV(params.map(p => p.name), body, lazyEnv))
      newEnv
    case TypeDef(name, tvars, varts:List[Variant]) =>
      lazy val newEnv: Env = env ++ varts.map(v => v.name -> ConstrV(v.name))
      newEnv

  def eq(l: Value, r: Value): Value = (l, r) match
    case (UnitV, UnitV) => BoolV(true)
    case (NumV(l), NumV(r)) => BoolV(l == r)
    case (BoolV(l), BoolV(r)) => BoolV(l == r)
    case (StrV(l), StrV(r)) => BoolV(l == r)
    case (VariantV(l:String, v1:List[Value]), VariantV(r:String, v2)) => 
      if (l == r && v1.length == v2.length)
        BoolV(v1.zip(v2).forall { case (item1, item2) => eq(item1, item2) == BoolV(true) })
      else BoolV(false)
    case _ => BoolV(false)
}
