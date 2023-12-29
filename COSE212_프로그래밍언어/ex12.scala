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
        typeCheck(b, tenv.addVar(x, t1)) 
      case Id(x) => tenv.vars.getOrElse(x, error(s"unbound variable: $x"))
      case Fun(ps, b) => 
        val ptys = ps.map(_.ty)
        for (pty <- ptys) mustValid(pty, tenv)
        val rty = typeCheck(b, tenv.addVars(ps.map(p => (p.name -> p.ty))))
        ArrowT(ptys, rty)
      case Rec(n, ps, rt, b, s) => 
        val ptys = ps.map(_.ty)
        for (pty <- ptys) mustValid(pty, tenv)
        mustValid(rt, tenv)
        val rty = typeCheck(b, 
          tenv
            .addVars(ps.map(p => (p.name -> p.ty)))
            .addVar(n, ArrowT(ptys, rt))
          )
        mustSame(rt, rty)
        typeCheck(s, tenv.addVar(n, ArrowT(ptys, rt)))
      case App(fun, args) =>
        typeCheck(fun, tenv) match
          case ArrowT(ptys, rty) =>
            if (ptys.length != args.length) error("type error")
            // for (arg <- args) mustValid(typeCheck(arg, tenv), tenv)
            for ((pty, arg) <- ptys.zip(args)) mustSame(typeCheck(arg, tenv), pty)
            rty
          case _ => error("type error")
      case If(cond, thenExpr, elseExpr) =>
        typeCheck(cond, tenv) match
          case BoolT => 
            mustSame(typeCheck(thenExpr, tenv), typeCheck(elseExpr, tenv))
            typeCheck(thenExpr, tenv)
          case _ => error("type error")
      case TypeDef(tn, ws, body) =>
        if (tenv.tys.contains(tn)) error(s"already defined type: $tn")
        val newTEnv = tenv.addType(tn, ws.map(w => w.name -> w.ptys).toMap)
        for (w <- ws; pty <- w.ptys) mustValid(pty, newTEnv)
          mustValid(typeCheck(body,newTEnv.addVars(ws.map(w => w.name -> ArrowT(w.ptys, NameT(tn))))), tenv)
      case Match(expr, cs) => typeCheck(expr, tenv) match
        case NameT(tn) =>
          val ts = tenv.tys.getOrElse(tn, error(s"unknown type: $tn"))
          val xs = cs.map(_.name).toSet
          if (ts.keySet != xs || xs.size != cs.length) error("invalid case")
            cs.map { case MatchCase(x, ps, b) =>
              typeCheck(b, tenv.addVars(ps zip ts(x)))
            }.reduce((lty, rty) => { mustSame(lty, rty); lty })
        case _ => error("not a variant")      

  

    // Helper Functions
    def mustSame(t1: Type, t2: Type): Unit = 
      if (t1 != t2) error(s"Type Mismatch: $t1 != $t2")
    def mustValid(ty: Type, tenv: TypeEnv): Type = ty match
      case NumT => NumT
      case BoolT => BoolT
      case ArrowT(ptys, rty) =>
        ArrowT(ptys.map(mustValid(_, tenv)), mustValid(rty, tenv))
      case NameT(tn) =>
        if (!tenv.tys.contains(tn)) error(s"invalid type name: $tn")
        else NameT(tn)

    

  
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
        case (NumV(l), NumV(r)) => 
          if (r == 0) error("division by zero")
          else NumV(l / r)
        case _ => error("type error")
      case Mod(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => 
          if (r == 0) error("division by zero")
          else NumV(l % r)
        case _ => error("type error")
      case Eq(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => BoolV(l == r)
        case _ => error("type error")
      case Lt(l, r) => (interp(l, env), interp(r, env)) match
        case (NumV(l), NumV(r)) => BoolV(l < r)
        case _ => error("type error")
      case Val(x, i, b) => 
        val v1 = interp(i, env)
        interp(b, env + (x -> v1))
      case Id(x) => env.getOrElse(x, error("type error"))
      case Fun(ps, b) => CloV(ps.map(_.name), b, () => env)
      case Rec(x, params, rty, body, scope) =>
        lazy val newEnv: Env = env + (x -> CloV(params.map(_.name), body, () => newEnv))
        interp(scope, newEnv)
      case App(fun, args) => interp(fun, env) match
        case CloV(ps, b, fenv) =>
          if (ps.length != args.length) error("type error")
          val vs = args.map(interp(_, env))
          interp(b, fenv() ++ ps.zip(vs))
        case ConstrV(n) => VariantV(n, args.map(interp(_, env)))
        case _ => error("type error")
      case If(cond, thenExpr, elseExpr) => interp(cond, env) match
        case BoolV(true) => interp(thenExpr, env)
        case BoolV(false) => interp(elseExpr, env)
        case _ => error("type error")
      case TypeDef(n, ws, body) => 
        interp(body, env ++ ws.map(w => w.name -> ConstrV(w.name)))
      case Match(expr, cases) => interp(expr, env) match
        case VariantV(wname, vs) => cases.find(_.name == wname) match
          case Some(MatchCase(_, ps, b)) =>
            if (ps.length != vs.length) error("arity mismatch")
              interp(b, env ++ (ps zip vs))
          case None => error(s"no such case: $wname")
        case v => error(s"not a variant: ${v.str}")    
}
