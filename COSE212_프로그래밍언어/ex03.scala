package kuplrg

/* F1VAE(VAE with first-order function) */
object Implementation extends Template {

  import Expr.*

  def interp(expr: Expr, env: Env, fenv: FEnv): Value = expr match {
    // from ex02(VAE)
    // onlly add fenv.
    case Num(n) => n
    case Add(l, r) => interp(l, env, fenv) + interp(r, env, fenv)
    case Mul(l, r) => interp(l, env, fenv) * interp(r, env, fenv)
    case Val(x, i, b) => interp(b, env + (x -> interp(i, env, fenv)), fenv)
    case Id(x) => env.getOrElse(x, error(s"free identifier"))
    
    // Static Scoping
    case App(f, e) => //case App(fname: String, arg: Expr) =>
      val fdef = fenv.getOrElse(f, error(s"unknown function"))
      interp(fdef.body, Map(fdef.param -> interp(e, env, fenv)), fenv)
  }

  def interpDS(expr: Expr, env: Env, fenv: FEnv): Value = expr match {
    case Num(n) => n
    case Add(l, r) => interpDS(l, env, fenv) + interpDS(r, env, fenv)
    case Mul(l, r) => interpDS(l, env, fenv) * interpDS(r, env, fenv)
    case Val(x, i, b) => interpDS(b, env + (x -> interpDS(i, env, fenv)), fenv)
    case Id(x) => env.getOrElse(x, error(s"free identifier")) 

    // Dynamic Scoping
    case App(f, e) => 
      val fdef = fenv.getOrElse(f, error(s"unknown function"))
      interpDS(fdef.body, env + (fdef.param -> interpDS(e, env, fenv)), fenv)
  }
}
