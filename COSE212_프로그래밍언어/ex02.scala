package kuplrg

/* VAE(AE with Variable) */
object Implementation extends Template {

  import Expr.*

  def interp(expr: Expr, env: Env): Value = expr match {
    // from ex01(AE)
    // only add env.
    case Num(n)    => n
    case Add(l, r) => interp(l, env) + interp(r, env)
    case Mul(l, r) => interp(l, env) * interp(r, env)
    
    // new part of ex02(VAE)
    // val x = i; b;
    // there is no gurantee that i is Num.
    case Val(x, i, b) => interp(b, env + (x -> interp(i, env)))
    // getOrElse
    case Id(x) => env.getOrElse(x, error(s"free identifier"))
  }

  /* The freeIds function returns the set of free identifiers in the given expression expr: */
  def freeIds(expr: Expr): Set[String] = expr match
    case Num(n) => Set.empty
    case Add(l, r) => freeIds(l) ++ freeIds(r)
    case Mul(l, r) => freeIds(l) ++ freeIds(r)
    case Val(x, i, b) => freeIds(i) ++ (freeIds(b) - x)
    case Id(x) => Set(x)

  /* The bindingIds function returns the set of binding identifiers in the given expression expr: */
  def bindingIds(expr: Expr): Set[String] = expr match {
    case Num(n) => Set.empty
    case Add(l, r) => bindingIds(l) ++ bindingIds(r)
    case Mul(l, r) => bindingIds(l) ++ bindingIds(r)
    case Val(x, i, b) => bindingIds(i) ++ bindingIds(b) + x
    case Id(x) => Set.empty
  }

  /* The boundIds function returns the set of bound identifiers in the given expression expr: */
  def boundIds(expr: Expr): Set[String] = expr match {
    case Num(n) => Set.empty
    case Add(l, r) => boundIds(l) ++ boundIds(r)
    case Mul(l, r) => boundIds(l) ++ boundIds(r)
    // curr scope안에 x가 없어서 free로 떴으면,
    // outer scope에 val x = i;가 있으므로 boundIds가 되는 것이 맞음.
    case Val(x, i, b) => boundIds(i) ++ (if (freeIds(b).contains(x)) boundIds(b) + x else boundIds(b))
    case Id(x) => Set.empty
  }


  /* The shadowedIds function returns the set of shadowed identifiers in the given expression expr: */
  def shadowedIds(expr: Expr): Set[String] = expr match {
    case Num(n) => Set.empty
    case Add(l, r) => shadowedIds(l) ++ shadowedIds(r)
    case Mul(l, r) => shadowedIds(l) ++ shadowedIds(r)
    case Val(x, i, b) => shadowedIds(i) ++ (if (bindingIds(b).contains(x)) shadowedIds(b) + x else shadowedIds(b))
    case Id(x) => Set.empty
  }
}
