package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*

  def interp(expr: Expr, env: Env, mem: Mem): (Value, Mem) = expr match
    // number
    case Num(number: BigInt) =>
      (NumV(number), mem)
    // addition
    case Add(left: Expr, right: Expr) =>
      val (l, m1) = interp(left, env, mem)
      val (r, m2) = interp(right, env, m1)
      (numAdd(l, r), m2)
    // multiplication
    case Mul(left: Expr, right: Expr) =>
      val (l, m1) = interp(left, env, mem)
      val (r, m2) = interp(right, env, m1)
      (numMul(l, r), m2)
    // mutable variable definition
    case Var(name: String, init: Expr, body: Expr) =>
      val (v, m) = interp(init, env, mem)
      interp(body, env + (name -> m.size), m + (m.size -> v))
    // identifier lookup
    case Id(name: String) =>
        (mem(lookupId(env, name)), mem)
    // anonymous (lambda) function
    case Fun(param: String, body: Expr) =>
      (CloV(param, body, env), mem)
    // function application
    case App(fun: Expr, arg: Expr) => 
      val (fv, fmem) = interp(fun, env, mem) 
      fv match
        case CloV(param, body, fenv) =>
          val (av, amem) = interp(arg, env, fmem)
          val addr = malloc(amem)
          interp(body, fenv + (param -> addr), amem + (addr -> av))
        case _ => error(s"not a function")
    // variable assignment
    // memory changed!!
    case Assign(name: String, expr: Expr) =>
      val (v, m) = interp(expr, env, mem)
      (v, m + (env(name) -> v))
    // sequence
    case Seq(left: Expr, right: Expr) =>
      val (newVal, newMem) = interp(left, env, mem) 
      interp(right, env, newMem)

  def interpCBR(expr: Expr, env: Env, mem: Mem): (Value, Mem) = expr match
    // number
    case Num(number: BigInt) =>
      (NumV(number), mem)
    // addition
    case Add(left: Expr, right: Expr) =>
      val (l, m1) = interpCBR(left, env, mem)
      val (r, m2) = interpCBR(right, env, m1)
      (numAdd(l, r), m2)
    // multiplication
    case Mul(left: Expr, right: Expr) =>
      val (l, m1) = interpCBR(left, env, mem)
      val (r, m2) = interpCBR(right, env, m1)
      (numMul(l, r), m2)
    // mutable variable definition
    case Var(name: String, init: Expr, body: Expr) =>
      val (v, m) = interpCBR(init, env, mem)
      interpCBR(body, env + (name -> m.size), m + (m.size -> v))
    // identifier lookup
    case Id(name: String) =>
      (mem(lookupId(env, name)), mem)
    // anonymous (lambda) function
    case Fun(param: String, body: Expr) =>
      (CloV(param, body, env), mem)    
    // function application
    case App(fun: Expr, arg: Expr) => 
      val (fv, fmem) = interpCBR(fun, env, mem) 
      fv match
        case CloV(param, body, fenv) => arg match
          case Id(name) =>
            val addr = lookupId(env, name)
            interpCBR(body, fenv + (param -> addr), fmem)
          case _ =>
            val (av, amem) = interpCBR(arg, env, fmem)
            val addr = malloc(amem)
            interpCBR(body, fenv + (param -> addr), amem + (addr -> av))
        case _ => error(s"not a function")
    // variable assignment
    // memory changed!!
    case Assign(name: String, expr: Expr) =>
      val (v, m) = interpCBR(expr, env, mem)
      (v, m + (env(name) -> v))
    // sequence
    case Seq(left: Expr, right: Expr) =>
      val (newVal, newMem) = interpCBR(left, env, mem) 
      interpCBR(right, env, newMem)

  def lookupId(env: Env, name: String): Addr =
    env.getOrElse(name, error(s"free identifier"))

  def malloc(mem: Mem): Addr = mem.keySet.maxOption.fold(0)(_ + 1)

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
}
