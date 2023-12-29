package kuplrg

/*
  COBALT:
  Comprehension-supported
  Boolean and Arithmetic Expression
  with Lists and Tuples
*/
object Implementation extends Template {

  import Expr.*
  import Value.*

  def interp(expr: Expr, env: Env): Value = expr match {
    /* Unit */
    case EUnit => UnitV

    /* Identifier */
    case EId(name) => env.getOrElse(name, error(s"free identifier"))

    /* Variable */
    case EVal(name, value, scope) => interp(scope, env + (name -> interp(value, env)))

    /* AE */
    case ENum(number) => NumV(number)
    case EAdd(left, rigiht) => numAdd(interp(left, env), interp(rigiht, env))
    case EMul(left, rigiht) => numMul(interp(left, env), interp(rigiht, env))
    case EDiv(left, rigiht) => numDiv(interp(left, env), interp(rigiht, env))
    case EMod(left, rigiht) => numMod(interp(left, env), interp(rigiht, env))

    /* Conditionals */
    case EBool(bool) => BoolV(bool)
    case EEq(left, rigiht) => BoolV(eq(interp(left, env), interp(rigiht, env)))
    case ELt(left, rigiht) => numLt(interp(left, env), interp(rigiht, env))
    case EIf(cond, thenExpr, elseExpr) => interp(cond, env) match
      case BoolV(true) => interp(thenExpr, env)
      case BoolV(false) => interp(elseExpr, env)
      case _ => error(s"not a boolean")

    /* List */
    case ENil => NilV
    case ECons(head, tail) => ConsV(interp(head, env), interp(tail, env))
    case EHead(left) => interp(left, env) match
      case NilV => error(s"empty list")
      case ConsV(head, tail) => head
      case _ => error(s"not a list")
    case ETail(left) => interp(left, env) match
      case NilV => error(s"empty list")
      case ConsV(head, tail) => tail
      case _ => error(s"not a list")
    case ELength(left) => length(interp(left, env))
    case EMap(left, fun) => map(interp(left, env), interp(fun, env))
    case EFlatMap(left, fun) => join(map(interp(left,env), interp(fun,env)))
    case EFilter(left, fun) => filter(interp(left, env), interp(fun, env))

    /* Tuple */
    case ETuple(exprs) => TupleV(exprs.map(interp(_, env)))
    case EProj(tuple, index) => interp(tuple, env) match
      case TupleV(list) => 
        if (index < list.length) list(index - 1) else error(s"out of bounds")
      case _ => error(s"not a tuple")

    
    /* Functions */
    case EFun(params, body) => CloV(params, body, () => env)
    case ERec(defs, scope) => 
      lazy val newEnv: Env = env ++ defs.map((x) => (x.name -> CloV(x.params, x.body, () => newEnv)))
      interp(scope, newEnv)
    case EApp(fun, args) => app(interp(fun, env), args.map(interp(_, env)))
  }

  /* !! BELOW FROM HERE: HELPER FUNCTIONS !! */

  /* 1. Arithmetic Operation Helper Funtions */
  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(left: Value, rigiht: Value): Value = (left, rigiht) match
    case (NumV(left), NumV(rigiht)) => {
      if (x == "/" && rigiht == 0) error(s"invalid operation")
      else if (x == "%" && rigiht == 0) error(s"invalid operation")
      else NumV(op(left, rigiht))
    }
    case (left, rigiht) => error(s"invalid operation")
  
  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
  val numDiv: (Value, Value) => Value = numBOp("/")(_ / _)
  val numMod: (Value, Value) => Value = numBOp("%")(_ % _)

  /* 2. Comparison Helper Funtions */
  /* Arithmetic Comparison Helper Funtions */
  type NCOp = (BigInt, BigInt) => Boolean
  def numCOp(op: NCOp)(left: Value, rigiht: Value): Value = (left, rigiht) match
    case (NumV(left), NumV(rigiht)) => BoolV(op(left, rigiht))
    case (left, rigiht) => error(s"invalid operation")
  val numLt: (Value, Value) => Value = numCOp(_ < _)
  
  /* Equivalent Comparison Helper Funtions */
  def eq(left: Value, right: Value): Boolean = (left, right) match
    case (UnitV, UnitV) => true
    case (NumV(left), NumV(rigiht)) => left == rigiht
    case (BoolV(left), BoolV(rigiht)) => left == rigiht
    case (NilV, NilV) => true
    case (ConsV(lh, lt), ConsV(rh, rt)) => eq(lh, rh) && eq(lt, rt)
    case (NilV, ConsV(_, _)) => false
    case (ConsV(_, _), NilV) => false
    case (TupleV(left), TupleV(rigiht)) =>
      if (left.length != rigiht.length) false
      else left.zip(rigiht).forall((x) => eq(x._1, x._2))
    case _ => error(s"invalid operation")

  /* 3. List Helper Funtions */
  def length(list: Value): Value = list match
    case NilV => NumV(0)
    case ConsV(head, tail) => numAdd(NumV(1), length(tail))
    case _ => error(s"not a list")

  def map(list: Value, fun: Value): Value = list match
    case NilV => NilV
    case ConsV(head, tail) => ConsV(app(fun, List(head)), map(tail, fun))
    case _ => error(s"not a list")

  def join(list: Value): Value = list match
    case NilV => NilV
    case ConsV(head, tail) => head match
      case NilV => join(tail)
      case ConsV(hh, ht) => ConsV(hh, join(ConsV(ht, tail)))
      case _ => error(s"not a list")
    case _ => error(s"not a list")

  def filter(list: Value, fun: Value): Value = list match
    case NilV => NilV
    case ConsV(head, tail) => app(fun, List(head)) match
      case BoolV(true) => ConsV(head, filter(tail, fun))
      case BoolV(false) => filter(tail, fun)
      case _ => error(s"not a boolean")
    case _ => error(s"not a list")

  /* 4. Function Application Helper Functions */
  def app(fun: Value, args: List[Value]): Value = fun match
    case CloV(params, body, fenv) => {
      if (params.length > args.length) {
        val newArgs = args ++ List.fill(params.length - args.length)(UnitV)
        interp(body, fenv() ++ (params.zip(newArgs).toMap))
      }
      else {
        interp(body, fenv() ++ (params.zip(args).toMap))
      }
    }
    case _ => error(s"not a function")
}
