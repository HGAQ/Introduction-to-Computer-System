/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * bitXnor - ~(x^y) using only ~ and | 
 *   Example: bitXnor(6, -5) = 2
 *   Legal ops: ~ |
 *   Max ops: 7
 *   Rating: 1
 */
int bitXnor(int x, int y) {
  return (~(x|y)|(~(~x|~y)));
}
/* 
 * bitConditional - x ? y : z for each bit respectively
 *   Example: bitConditional(0b00110011, 0b01010101, 0b00001111) = 0b00011101
 *   Legal ops: & | ^ ~
 *   Max ops: 4
 *   Rating: 1
 */
int bitConditional(int x, int y, int z) {
  return (x & y) | (~x & z) ;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 16
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    int mask;
    m = m << 3;
    n = n << 3;
    mask = ((x >> m) ^ (x >> n)) & 0xFF;
    mask = (mask << m) | (mask << n);
    return (x ^ mask);
}
  /* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int logicalShift(int x, int n) {
   int u,v;
   u = 0x1 << 31;
   u = ~((u >> n) << 1);
   v = (x >> n);
   return  (u & v);
}
/* 
 * cleanConsecutive1 - change any consecutive 1 to zeros in the binary form of x.
 *   Consecutive 1 means a set of 1 that contains more than one 1.
 *   Examples cleanConsecutive1(0x10) = 0x10
 *            cleanConsecutive1(0xF0) = 0x0
 *            cleanConsecutive1(0xFFFF0001) = 0x1
 *            cleanConsecutive1(0x4F4F4F4F) = 0x40404040
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 4
 */
int cleanConsecutive1(int x){
  int a,xl,xr;
  a = ~ (1 << 31);
  xl = x&(x<<1);
  xr = x&(x>>1 & a);
  return x ^ (xl | xr);
}
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
  int is_fuyi,n0 ,n1 ,n2 ,n3 ,n4 ,num4 ,x4 ,num3 ,x3 ,num2 ,x2 ,num1 ,x1 ,num0;
  is_fuyi = !(~x); 

  n0 = 0x1 << 31;
  n1 = n0>>1;
  n2 = n1>>2;
  n3 = n2>>4;
  n4 = n3>>8;

  num4 = (!((n4 & x )^ n4))<<4;
  x4 = x<<num4;
  num3 = (!((n3 & x4 )^ n3))<<3;
  x3 = x4<<num3;
  num2 = (!((n2 & x3 )^ n2))<<2;
  x2 = x3<<num2;
  num1 = (!((n1 & x2 )^ n1))<<1;
  x1 = x2<<num1;
  num0 = (!((n0 & x1 )^ n0));
  return num4+ num3 + num2 + num1 + num0 + is_fuyi;
}
/* 
 * counter1To5 - return 1 + x if x < 5, return 1 otherwise, we ensure that 1<=x<=5
 *   Examples counter1To5(2) = 3,  counter1To5(5) = 1
 *   Legal ops: ~ & ! | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int counter1To5(int x) {
  int con,cond;
  con = !!(((~x) & 5)|(x & (~5)));
  cond = con<<31>>31;
  return (cond & (x+1)) | ( ~cond & 1);
}
/* 
 * sameSign - return 1 if x and y have same sign, and 0 otherwise
 *   Examples sameSign(0x12345678, 0) = 1, sameSign(0xFFFFFFFF,0x1) = 0
 *   Legal ops: ! ~ & ! ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int sameSign(int x, int y) {
  int u;
  u = 0x1 << 31;
  return !(u&(x^y));
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    //max 0x7fffffff/0x3=0x2aaaaaaa....0x1
    //                    not of
    //min 0x80000000/0x3=0xd5555556....
    //                    not of
    int mm, sign, xx, x2_of, xxx, x3_of, of, sx;
    mm = 1<<31;
    sign = (mm & x)>>31;
    xx = x + x;
    x2_of = (xx ^ x) >> 31;
    xxx = xx + x;
    x3_of = (xxx ^ x) >> 31;
    of = x2_of | x3_of;
    sx = (~sign & ((1<<31) + (~0x0))) | (sign & ((1<<31)));
    return (~of & xxx) | (of & (sx));
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  //sign
  int mm,x_sign,y_sign,same_sign,same_sign_great,diff_sign_great;
  mm = 1<<31;
  x_sign= mm & x;
  y_sign= mm & y;
  same_sign = !((x_sign) ^ (y_sign));
  same_sign_great = !(mm & (x + ~y));
  diff_sign_great = !x_sign;
  return ((same_sign & same_sign_great)|((!same_sign) & diff_sign_great));
}
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  int a,b,c;
  a=x>>31;
  b=y>>31;
  c=(x+~y+1)>>31;
  return !((a ^ b) & (a ^ c));
}

/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x)
{  
  int mm,sig,sign,orig,orgi5,orgi58,over,over8,over58,ans;
  mm = 1<<31;
  sig = (x & mm);
  sign = sig>>31;
  orig= x & 7;
  orgi5 = ((orig << 2) + orig + (sign & 7));
  orgi58 = orgi5 >> 3;
  over = x;
  over8 = over >> 3;
  over58 = (over8 << 2) + over8;
  ans = over58 + orgi58 ;
  return ans;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  //0|000 0000 0|000 0000 0000 0000 0000 0000
  //0x7f800000 0x007fffff
  int sign,exp,expp,frac,fracc,end,endd;
  sign = uf & 0x80000000;
  exp = uf & 0x7f000000;
  expp = uf & 0x7f800000;
  frac = uf & 0x00ffffff;
  fracc = uf & 0x007fffff;
  end = uf & 0x1;
  if((!(expp ^ 0x7f800000))){
    return uf;
  }
  if(!(exp)){
    frac = frac>>1;
    endd = frac & 0x1;
    if(end && endd){
      frac = frac+1;
    }
    return sign|exp|frac;
  }
  else {
    expp = expp - (1<<23);
    /*if(end){
      fracc = fracc+1;
      return sign|(expp + fracc);
    }*/
    return sign|expp|fracc; 
  }
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  int sign,first_pos,exp,get_all_pos,all_pos,move_to_frac,right_to_frac,frac_moved,temp_right,to_be_rounded,normalize_round,round_result;
  if(x){
    sign = x & 0x80000000;
    if(sign){
      x = -x;
    }
    first_pos = 31;
    while (!(x >> first_pos))first_pos--;
    exp = 127 + first_pos;
    get_all_pos = (1 << first_pos) - 1;
    all_pos = x & get_all_pos;
    move_to_frac = 23 - first_pos;
    right_to_frac = first_pos - 23;
    frac_moved = all_pos << move_to_frac;
    if(move_to_frac & 0x80000000u){
      //fucking rounding
      frac_moved = all_pos >>  right_to_frac;
      //return frac_moved;
      normalize_round = (x << (31 - (first_pos))) & 0xff;
      round_result = normalize_round > 128 || (normalize_round == 128 && (frac_moved & 1));
      frac_moved += round_result;
      //return round_result;
      if (frac_moved >> 23){
        frac_moved = frac_moved & 0x7fffff; 
        exp++;
      }
      //return frac_moved;
    }
    return sign|frac_moved|exp<<23;
  }
  return 0;
}
/* 
 * float64_f2i - Return bit-level equivalent of expression (int) f
 *   for 64 bit floating point argument f.
 *   Argument is passed as two unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   double-precision floating point value.
 *   Notice: uf1 contains the lower part of the f64 f
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 20
 *   Rating: 4
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    int err, S, exp, frac, bias, E, ans;
    err = 0x80000000;
    S = (uf2>>31) & 0x1;
    exp = (uf2 >> 20) & 0x7ff;
    frac = (uf2 & 0xfffff) + 0x100000;
    bias = 1023;
    E = exp - bias;
    
    if(exp==0x7ff00000u){
      return err;
    }
    if(E < 0){
      return 0;
    }
    if(E >= 31){
      return err;
    }
    if(E <= 20){
      ans = frac >> (20 - E); 
    }
    if(E > 20){
      ans = (frac << (E - 20)) + (uf1 >> (52 - E));
    }
    if(S){
      return -ans;
    }
    else{
      return ans;
    }
}
/* 
 * float_negpwr2 - Return bit-level equivalent of the expression 2.0^-x
 *   (2.0 raised to the power -x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^-x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 20 
 *   Rating: 4
 */
unsigned float_negpwr2(int x) {
    //inf
    if( x <= -128){
      return 0x7f800000;
    }
    //0 2^-126*2*-23
    if(x > 149){
      return 0;
    }
    //008 1*2^-126
    if(x > 126){
      return 0x1<<(149-x);
    }
    //exp-127=-x
    else{
      return (-x+127)<<23;
    }
}
