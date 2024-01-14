#include <stdio.h>
int counter1To5(int x) {
  int con = !!(x^5);
  int cond = con<<31>>31;
  return (cond & (x+1)) | ( ~cond & 1);
}
int sameSign(int x, int y) {
  return !((0x80000000&x)^(0x80000000&y));
}
int leftBitCount(int x) {
  int is_fuyi = !(~x); 
  int leftshift = 16;
  int num4 = (!(0xffff0000 & x ^ 0xffff0000))<<4;
  int x4 = x<<num4;
  int num3 = (!(0xff000000 & x4 ^ 0xff000000))<<3;
  int x3 = x4<<num3;
  int num2 = (!(0xf0000000 & x3 ^ 0xf0000000))<<2;
  int x2 = x3<<num2;
  int num1 = (!(0xc0000000 & x2 ^ 0xc0000000))<<1;
  int x1 = x2<<num1;
  int num0 = (!(0x80000000 & x1 ^ 0x80000000));
  return num4+ num3 + num2 + num1  + num0 + is_fuyi;

}
int isGreater(int x, int y) {
  //sign
  int x_sign= 0x80000000 & x;
  int y_sign= 0x80000000 & y;
  int same_sign = !((x_sign) ^ (y_sign));
  int same_sign_great = !(0x80000000 & (x + ~y ));
  int diff_sign_great = !x_sign;
  return ((same_sign & same_sign_great)|(!same_sign & diff_sign_great));
}

int satMul3(int x) {
    //max 0x7fffffff/0x3=0x2aaaaaaa....0x1
    //                    not of
    //min 0x80000000/0x3=0xd5555556....
    //                    not of
    int x_sign = (0x80000000 & x);
    x_sign = x_sign>>31;
    int x_nomal = (x<<1)+x;
    int Tmax = 0x7FFFFFFF;
    int Tmin = 0x80000000;
    int neg_x_of = (0x80000000 & (x+0x2aaaaaaa));
    neg_x_of=neg_x_of>>31;
     //0 is ok,-1 is of
    
    int neg_ans = (neg_x_of & Tmin) | (~neg_x_of & x_nomal);
    int pos_x_of = (0x80000000 & (x+0xd5555555));
    pos_x_of=pos_x_of>>31;
    int pos_ans = (~pos_x_of & Tmax) | (pos_x_of & x_nomal);
    return (x_sign & neg_ans)|(~x_sign & pos_ans);
}

int subOK(int x, int y) {
  int a=x>>31;
  int b=y>>31;
  int c=(x+~y+1)>>31;
  return (a ^ b) && (a ^ c);
}
int trueFiveEighths(int x)
{  
  int sig = (x & 0x80000000);
  int sign = sig>>31;
  int orig= x & 7;
  int orgi5 = ((orig << 2) + orig + (sign & 7));
  int orgi58 = orgi5 >> 3;
  int over = x;
  int over8 = over >> 3;
  int over58 = (over8 << 2) + over8;
  int ans = over58 + orgi58 ;
  return ans;
}
int byteSwap(int x, int n, int m) {
  int p=m<<3;
  int q=n<<3;
  int fp=0xff<<p;
  int fq=0xff<<q;
  int a=(((x>>p)&0xff)<<q);
  int b=(((x>>q)&0xff)<<p);
  int c=x&((0xffffffff^fp)^fq);
  return a+b+c;
 }

 unsigned float_half(unsigned uf) {
  //0|000 0000 0|000 0000 0000 0000 0000 0000
  //0x7f800000 0x007fffff
  int sign = uf & 0x80000000;
  int exp = uf & 0x7f000000;
  int expp = uf & 0x7f800000;
  int frac = uf & 0x00ffffff;
  int fracc = uf & 0x007fffff;
  if((!(exp ^ 0x7f800000)) && frac ){
    return uf;
  }
  if(!(exp)){
    frac = frac>>1;
    return sign|exp|frac;
  }
  else {
    expp = expp - (1<<23);
    return sign|expp|fracc; 
  }
}

unsigned float_i2f(int x) {
  if(!x){
    return 0;
  }
  int sign = x & 0x80000000;
  if(sign){
    x = -x;
  }
  int a = 0x80000000;
  int num = 0;
  int first_pos=31;
  while (!(x >> first_pos))
    first_pos--;
  int exp = 127 + first_pos;
  int get_all_pos = (1 << first_pos) - 1;
  int all_pos = x & get_all_pos;
  int move_to_frac = 23 - first_pos;
  int right_to_frac = first_pos - 23;
  int frac_moved = 0;
  if(move_to_frac & 0x80000000u){
    //fucking rounding
    int temp_right = (0x1 << right_to_frac) - 1;
    int to_be_rounded = x & (temp_right);
    frac_moved = all_pos >>  right_to_frac;
    //return frac_moved;
    int normalize_round = to_be_rounded << (31 - (first_pos));
    int round_result = normalize_round > 128 || (normalize_round == 128 && (frac_moved & 1));
    frac_moved += round_result;
    //return round_result;
    if (frac_moved >> 23){
      frac_moved = frac_moved & 0x7fffff; exp++;
    }
    //return frac_moved;
  }
  else{
    //no need to round
    frac_moved = all_pos << move_to_frac;
  }
  return sign|frac_moved|exp<<23;
}

int float64_f2i(unsigned uf1, unsigned uf2) {
    int err = 0x80000000u;
    int S = (uf2>>31) & 0x1u;
    int exp = (uf2 >> 20) & 0x7ffu;
    int frac = uf2 & 0xfffffu + 0x100000u;
    int bias = 1023;
    int E = exp - bias;
    
    if(exp==0x7ff00000u){
      return err;
    }
    if(E < 0){
      return 0;
    }
    if(E >= 31){
      return err;
    }
    int ans;
    if(E <= 20){
      ans = frac >> (20 - E); 
    }
    if(E > 20){
      ans = frac << (E - 20) + uf1 >> (52 - E);
    }
    if(S){
      return -ans;
    }
    else{
      return ans;
    }
}
int main(){
    printf("Hello world!\n");
    printf("%d\n",counter1To5(-1));
    printf("0x%08x\n",float64_f2i(0, 1072693248));
    return 0;
}
/*
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)*/