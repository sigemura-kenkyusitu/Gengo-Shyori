#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexical.h"

struct RsvWord{
  char *name;
  int  tok;
};

struct RsvWord rsvWord[] = {
  { "while" , lxWHL  } , { "do" , lxDO},
  { "done"  , lxDONE } , { "if" , lxIF},
  { "then"  , lxTHEN } , { "fi" , lxFI},
  { "print" , lxPRI  }};

FILE *fp;
char ch = 0;
char nextch = 0;
int val = 0;
char str[10];

/* 一文字読み込む */
void getCh(){
  if(nextch == 0)         // まだ一文字も読み込んで
    ch = '\n';            //  いないときは改行を格納
  else                    // それ以外では
    ch = nextch;          //  次の文字を現在の文字に
  nextch = fgetc(fp);     // 次の文字を読み込む
}

/* 空白を読み飛ばす */
void skipSpace(){
  while(isspace(ch)){     // 空白記号の間
    getCh();              //  文字を読み飛ばす
  }
}

/* 記号を読み込む */
int getSign(){
  int tok = lxNONTOK;                        // トークンの初期値はNONTOK
  if (ch == '>' && nextch != '=') {          // >なら
    tok = lxGT;                              //  tok=GT
  } else if (ch == '>' && nextch == '=') {   // >=なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxGE;                              //  tok=GE
  } else if (ch == '=' && nextch == '=') {   // ==なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxEQU;                             //  tok=EQU
  } else if (ch == '!' && nextch == '=') {   // !=なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxNEQU;                            //  tok=NEQU
  } else if (ch == '<' && nextch == '=') {   // <=なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxLE;                              //  tok=LE
  } else if (ch == '<' && nextch != '=') {   // <なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxLT;                              //  tok=LT
  } else if (ch == ':' && nextch == '=') {   // :=なら
    getCh();                                 //  1文字余計に読み飛ばす
    tok = lxASS;                             //  tok=ASS
  } else if (ch == '+') {                    // +なら
    tok = lxADD;                             //  tok=ADD
  } else if (ch == '-') {                    // -なら
     tok = lxSUB;                            //  tok=SUB
  } else if (ch == '*') {                    // *なら
    tok = lxMUL;                             //  tok=MUL
  } else if (ch == '/') {                    // /なら
    tok = lxDIV;                             //  tok=DIV
  } else if (ch == ';') {                    // ;なら
    tok = lxPI;                              //  tok=PI
  } else {                                   // それ以外なら
    printf("そんな記号ありませーん\n");      //  エラー表示して
    exit(1);                                 //  終了
  }
  getCh();                                   // 最低1文字は読み飛ばす
  return tok;                                // tok を返す
}

/* 数字を読み込む */
int getNum() {
  val = 0;                       // 初期値は 0
  while (isdigit(ch)) {          // 10進数字の間
    val = val*10 + ch - '0';     //  値を計算
    getCh();                     //  次の文字を読む
  }
  return lxNUM;
}

/* 予約語を読み込む */
int getWord(){
  int i;
  for (i=0; isalpha(ch); i=i+1) {        // 区切り文字以外なら
    str[i] = ch;                         //  strに読み込む
    getCh();                             //  1文字読み飛ばす
  }
  str[i] = '\0';                         // 文字列を完成させる
  for (i=0; i<lxRsvNum; i++) {           // 予約語の一覧表を捜す
    if (!strcmp(str, rsvWord[i].name))   //  同じつづりが見付かれば
      return rsvWord[i].tok;             //   それのトークン値を返す
  }                                      // 予約語以外なら
  printf("そんな予約後ねーよ!!\n");      // エラー表示して
  exit(1);                               // 終了
}

/* トークン値を取得する */
int getTok(){
  int tok = lxNONTOK;                          // tokの初期値はNONTOK
  if(ch==0)                                    // 一番最初なら
    getCh();                                   //  一文字取得
  skipSpace();                                 // 空白を読み飛ばす
  if(ch==EOF){                                 // EOFなら
    tok = lxEOF;                               //  tok=EOFに
  } else if (isdigit(ch)){                     // 数字だったら
    tok = getNum();                            //  数値を読み込む
  } else if (isalpha(ch) && isalpha(nextch)){  // 文字が続くなら
    tok = getWord();                           //  予約語を読み込む
  } else if (isalpha(ch) && !isalpha(nextch)){ // 一文字なら
    tok = lxNAME;                              //  変数
    getCh();                                   //  変数を読み飛ばす
  } else {                                     // それ以外なら
    tok = getSign();                           //  記号を読み込む
  }
  return tok;                                  // トークン値を返す
}

int main(int argc, char *argv[]){
  fp = fopen(argv[1],"r");         // ファイルオープン
  int tok = getTok();              // 最初のトークン取得
  while(tok != lxEOF){             // トークンが有る限り
    printf("%d\n",tok);                //  トークン出力
    tok = getTok();                //  次のトークン取得
  }
  return 0;                        // 終わり
}
