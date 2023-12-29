package kuplrg

object Implementation extends Template {

  import Expr.*
  import Value.*
  import Inst.*
  import Control.*

  // ---------------------------------------------------------------------------
  // Problem #1
  // ---------------------------------------------------------------------------
  def reduce(st: State): State =
    val State(k, s, h, mem) = st
    (k, s, h, mem) match
      case (Nil, _, _, _) => st
      case (IEval(env, e) :: k, s, h, mem) => e match
        case EUndef => State(k, UndefV :: s, h, mem)
        case ENum(n) => State(k, NumV(n) :: s, h, mem)
        case EBool(b) => State(k, BoolV(b) :: s, h, mem)

        case EAdd(l, r) => State(IEval(env, l) :: IEval(env, r) :: IAdd :: k, s, h, mem)
        case EMul(l, r) => State(IEval(env, l) :: IEval(env, r) :: IMul :: k, s, h, mem)
        case EDiv(l, r) => State(IEval(env, l) :: IEval(env, r) :: IDiv :: k, s, h, mem)
        case EMod(l, r) => State(IEval(env, l) :: IEval(env, r) :: IMod :: k, s, h, mem)

        case EEq(l, r) => State(IEval(env, l) :: IEval(env, r) :: IEq :: k, s, h, mem)
        case ELt(l, r) => State(IEval(env, l) :: IEval(env, r) :: ILt :: k, s, h, mem)

        case EVar(x, i, b) => State(IEval(env, i) :: IDef(List(x), env, b) :: k, s, h, mem) 
        case EId(x) => State(k, mem(lookup(env, x)) :: s, h, mem)
        case EAssign(x, e) => State(IEval(env, e) :: IWrite(lookup(env, x)) :: k, s, h, mem)
        case ESeq(l, r) => State(IEval(env, l) :: IPop :: IEval(env, r) :: k, s, h, mem)
        
        case EIf(c, t, e) => State(IEval(env, c) :: IJmpIf(KValue(IEval(env, t) :: k, s, h)) :: IEval(env, e) :: k, s, h, mem)
        
        case EWhile(c, b) =>
          val break = KValue(k, s, h)
          val continue = KValue(IPop :: IEval(env, EWhile(c, b)) :: k, s, h)
          val bodyHandler = h + (Break -> break) + (Continue -> continue)
          val bodyCont = KValue(IEval(env, b) :: IJmp(Continue) :: Nil, s, bodyHandler)
          State(IEval(env, c) :: IJmpIf(bodyCont) :: k, UndefV :: s, h, mem)
        case EBreak => State(IJmp(Break) :: Nil, UndefV :: s, h, mem)
        case EContinue => State(IJmp(Continue) :: Nil, UndefV :: s, h, mem)
        case EFun(params, body) => State(k, CloV(params, body, env) :: s, h, mem)
        case EApp(fun, args) => State(IEval(env, fun) :: args.map(arg => IEval(env, arg)).toList ::: ICall(args.length) :: k, s, h, mem)
        case EReturn(expr) => State(IEval(env, expr) :: IReturn :: k, s, h, mem)
        //case ETry(body, catchParam, catchExpr)
        case ETry(e1, x, e2) => 
          val finallyCont = KValue(k, s, h)
          val throwCont = KValue(IDef(List(x), env, e2):: k, s, h)
          val bodyHandler = h + (Throw -> throwCont) + (Finally -> finallyCont)
          State(IEval(env, e1) :: IJmp(Finally) :: Nil, s, bodyHandler, mem)
        case EThrow(e) => State(IEval(env, e) :: IJmp(Throw) :: Nil, s, h, mem)
        case EGen(params, body) => State(k, GenV(params, body, env) :: s, h, mem)
        case EIterNext(e1, None) => State(IEval(env, e1) :: IEval(env, EUndef) :: INext :: k, s, h, mem)
        case EIterNext(e1, Some(e2)) => State(IEval(env, e1) :: IEval(env, e2) :: INext :: k, s, h, mem)
        case EYield(e) => State(IEval(env, e) :: IYield :: Nil, BoolV(false) :: ContV(KValue(k, s, h)) :: s, h, mem)
        case EValueField(e) => State(IEval(env, e) :: IValueField :: k, s, h, mem)
        case EDoneField(e) => State(IEval(env, e) :: IDoneField :: k, s, h, mem) 
        case _ => error(s"invalid expression: $e")
      
      case (IAdd :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, numAdd(NumV(l), NumV(r)) :: s, h, mem)
      case (IMul :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, numMul(NumV(l), NumV(r)) :: s, h, mem)
      case (IDiv :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, numDiv(NumV(l), NumV(r)) :: s, h, mem)
      case (IMod :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, numMod(NumV(l), NumV(r)) :: s, h, mem)
      // 예외처리: case (IAdd :: k, _ :: _ :: s, h, mem) => error(s"invalid operation")
      // numAdd에 구현되어있음.

      case (IEq :: k, UndefV :: UndefV :: s, h, mem) => State(k, BoolV(true) :: s, h, mem)
      case (IEq :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, BoolV(eq(NumV(l), NumV(r))) :: s, h, mem)
      case (IEq :: k, BoolV(r) :: BoolV(l) :: s, h, mem) => State(k, BoolV(eq(BoolV(l), BoolV(r))) :: s, h, mem)
      case (IEq :: k, IterV(r) :: IterV(l) :: s, h, mem) => State(k, BoolV(eq(IterV(l), IterV(r))) :: s, h, mem)
      case (IEq :: k, ResultV(rv, rd) :: ResultV(lv, ld) :: s, h, mem) => State(k, BoolV(eq(ResultV(lv, ld), ResultV(rv, rd))) :: s, h, mem)
      case (IEq :: k, _ :: _ :: s, h, mem) => State(k, BoolV(false) :: s, h, mem)
      
      case (ILt :: k, NumV(r) :: NumV(l) :: s, h, mem) => State(k, BoolV(l < r) :: s, h, mem)
      case (ILt :: k, _ :: _ :: s, h, mem) => error(s"invalid operation")
      
      case (IDef(xs, env, b) :: k, vs, h, mem) =>
        val paramLen = xs.length
        val allocMem = malloc(mem, paramLen)
        val newEnv = env ++ xs.zip(allocMem).toMap
        val newMem = mem ++ allocMem.zip(vs.take(paramLen).reverse).toMap
        State(IEval(newEnv, b) :: k, vs.drop(paramLen), h, newMem)
      case (IWrite(a) :: k,  v :: s, h, mem) => State(k, v :: s, h, mem + (a -> v))
      case (IPop :: k, v :: s, h, mem) => State(k, s, h, mem)

      case (IJmpIf(KValue(k, s, h)) :: _, BoolV(true) :: _, _, mem) => State(k, s, h, mem)
      case (IJmpIf(_) :: k, BoolV(false) :: s, h, mem) => State(k, s, h, mem)
      case (IJmpIf(_) :: _, _ :: _, _, _) => error(s"invalid operation")
      // 예외처리: BoolV이 아닌 경우
      
      case (IJmp(c) :: k, v :: s, h, mem) => 
        val KValue(jmpC, jmpS, jmpH) = lookup(h, c)
        h.get(Yield) match
          case Some(KValue(k, s, h)) => State(jmpC, v :: jmpS, jmpH + (Yield -> KValue(k, s, h)), mem)
          case _ => State(jmpC, v :: jmpS, jmpH, mem)
      case (ICall(n) :: k, vs, h, m) => vs(n) match 
          case CloV(xs, b, env) =>
            val returnKValule = KValue(k, vs.drop(n + 1), h)
            val bodyHandler = h + (Return -> returnKValule) - Break - Continue - Yield 
            if (n >= xs.length)
              State(IDef(xs, env, EReturn(b)) :: Nil, vs.take(n).drop(n - xs.length), bodyHandler, m)
            else
              State(IDef(xs, env, EReturn(b)) :: Nil, List.fill(xs.length - n)(UndefV) ++ vs.take(n), bodyHandler, m)            
          case GenV(xs, b, env) =>
            val KBody = List(IPop, IDef(xs, env, EReturn(ETry(b, "err", EId("err")))))
            val a = malloc(m)
            if (n >= xs.length)
              val SBody = vs.take(n).drop(n - xs.length)
              val CBody = ContV(KValue(KBody, SBody, Map.empty))
              State(k, IterV(a) :: s.drop(n + 1), h, m + (a -> CBody))
            else
              val SBody = List.fill(xs.length - n)(UndefV) ++ vs.take(n)
              val CBody = ContV(KValue(KBody, SBody, Map.empty))
              State(k, IterV(a) :: s.drop(n + 1), h, m + (a -> CBody))
          case x => error(s"invalid call: $x")
      
      case (IReturn :: k, v :: s, h, mem) =>
        h.get(Yield) match
          case Some(KValue(_, _, _)) => 
            val done = ContV(KValue(IReturn :: Nil, Nil, Map.empty))
            State(IYield :: Nil, v :: BoolV(true) :: done :: s, h, mem)
          case _ => State(IJmp(Return) :: Nil, v :: Nil, h, mem)
      case (INext :: k, v :: IterV(a):: s, h, mem) =>
        mem.get(a) match
          case Some(ContV(KValue(k2, s2, h2))) =>
            val cont = KValue(k, IterV(a) :: s, h)
            val bodyHandler = h2 + (Yield -> cont) + (Return -> cont)
            State(k2, v :: s2, bodyHandler, mem)
          case _ => error(s"invalid iterator")
      case (IYield :: _, v :: BoolV(b) :: v2 :: _, h, mem) =>
        h.get(Yield) match
          case Some(KValue(k2, IterV(a) :: s2, h2)) => 
            State(k2, ResultV(v, b) :: s2, h2, mem + (a -> v2))
          case _ => error(s"invalid yield")
      case (IValueField :: k, ResultV(v, _) :: s, h, mem) => State(k, v :: s, h, mem)
      case (IDoneField :: k, ResultV(_, b) :: s, h, mem) => State(k, BoolV(b) :: s, h, mem)
      case (_, _, _, _) => error(s"invalid operation")

  // ---------------------------------------------------------------------------
  // Problem #2
  // ---------------------------------------------------------------------------
  def bodyOfSquares: String = """
    var i = from;
    while (i <= to) {
      yield i * i;
      i++;
    }
  """

  // ---------------------------------------------------------------------------
  // Helper functions
  // ---------------------------------------------------------------------------
  // 1. Arithmetic Expressions
  type NBOp = (BigInt, BigInt) => BigInt
  def numBOp(x: String)(op: NBOp)(left: Value, rigiht: Value): Value = (left, rigiht) match
    case (NumV(left), NumV(rigiht)) => {
      // Error case Div By 0
      if (x == "/" && rigiht == 0) error(s"invalid operation")
      else if (x == "%" && rigiht == 0) error(s"invalid operation")
      else NumV(op(left, rigiht))
    }
    case (left, rigiht) => error(s"invalid operation")
  val numAdd: (Value, Value) => Value = numBOp("+")(_ + _)
  val numMul: (Value, Value) => Value = numBOp("*")(_ * _)
  val numDiv: (Value, Value) => Value = numBOp("/")(_ / _)
  val numMod: (Value, Value) => Value = numBOp("%")(_ % _)

  // 2. Boolean Expressions
  def eq(l: Value, r: Value): Boolean = (l, r) match
    case (UndefV, UndefV) => true
    case (NumV(l), NumV(r)) => l == r
    case (BoolV(l), BoolV(r)) => l == r
    case (IterV(l), IterV(r)) => l == r
    case (ResultV(lv, ld), ResultV(rv, rd)) => eq(lv, rv) && ld == rd
    case _ => false

  def malloc(mem: Mem, n: Int): List[Addr] =
    val a = malloc(mem)
    (0 until n).toList.map(a + _)

  def malloc(mem: Mem): Addr = mem.keySet.maxOption.fold(0)(_ + 1)

  // 3. Look Up
  def lookup(env: Env, x: String): Addr = // variable
    env.getOrElse(x, error(s"free identifier: $x"))
  def lookup(handler: Handler, x: Control): KValue = // control operation
    handler.getOrElse(x, error(s"invalid control operation: $x"))
}
