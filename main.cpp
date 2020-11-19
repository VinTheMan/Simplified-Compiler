/* 
 * File:   main.cpp
 * Author: Vincent
 *
 * Created on 2016年4月7日, 上午 5:40
 */

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

/*
 * 
 */

enum TokenType {
  IDENTIFIER = 34512, CONSTANT = 87232, SPECIAL = 29742
};


typedef char * CharPtr;

struct Column {
  int column; // 此token(的字首)在本line有出現於此column
  Column * next; // 下一個此token(在本line)有出現的column
}; // Column

typedef Column * ColumnPtr;

struct Line { // struct Line 記錄了4種資訊
  int line; // 此token放在哪一 line
  ColumnPtr firstAppearAt; // 指到此token第一次出現在此line的哪一column
  ColumnPtr lastAppearAt; // 指到此token最後出現在此line的哪一column
  Line * next; // 同一 token 下一次出現在哪一 line
};

typedef Line * LinePtr;

struct Token1 { // Struct token 紀錄了4種資訊
  CharPtr tokenStr; // tokenStr 放你切下來的token
  TokenType type; // type 紀錄此token是屬於哪一個case
  LinePtr firstAppearOn; // 此 token 第一次出現在哪一 line
  LinePtr lastAppearOn; // 此 token 最後出現在哪一 line
  Token1 * next; // 指向下一個token
}; // struct Token1

typedef Token1 * TokenPtr;

TokenPtr gFront = NULL, gRear = NULL; // 分別指向Token串列的頭跟尾
TokenPtr gHead = NULL, gTemp = NULL ; // 用來存頭，讓gFront一直指回來這裡。用來製造新節點

typedef char Str100[ 100 ]; // 此型別是打算用來宣告一個陣列變數、以供讀字串之用

int gLine = 1; // 我剛讀進來的char所在的line number(一讀就加)

int gColumn = 0; // 我剛讀進來的char所在的column number(一讀就加)


void Position(char & first) ;  // keep track of
// 所get到的char的line number and column number

void Token(char & first) ;  // get the next non-white-space char」
// 此function要負責「跳過comment」

void Ident( char & first ) ;  // Case1底線 或 英文字母 開頭，之後跟著英文字母或底線或數字
void Num( char tokky[200], char & first ) ;  // 數字 與 字串 與 字元
void Special( bool is2char, char & first); // 特殊符號


void Insert( TokenPtr & gTemp, TokenPtr & gFront, TokenPtr & gRear) ; // 把gTemp插入list
// void Find () ;      //使用者互動要搜尋某token時
void React(); // 互動介面
void Out() ; // 印出整個linked list


bool uHas = false ; // 給insert用的

// ///////////////////////////

int main(int argc, char** argv) {
  char first = '\0' ; // 每個token的字首，之後也用它來讀完整個token
  
                                     // 先把Token都get進去，包含EOF，但一發現是EOF字串，則迴圈停下
 
  bool dontstop = true ; 
  Token(first) ;
  
  
  
 if ( gTemp != NULL && strcmp(gTemp->tokenStr, "END_OF_FILE" ) == 0 ) {
   cout << '\0' << endl ; 
    // React();
  } // if E
  else {
    while ( dontstop == true ) {
      first = '\0' ;
      if ( gTemp != NULL && strcmp( gTemp->tokenStr, "END_OF_FILE" ) == 0 ) {
        dontstop = false ; 
         
      } // if 

      
      else if ( gTemp != NULL && strcmp(gTemp->tokenStr, "END_OF_FILE" ) != 0) {
        Insert( gTemp, gFront, gRear ) ; 
        gFront = gHead ; 
        // cout << gFront->tokenStr << endl ; // TEST
         
        Token(first) ;
      } // else if 
      
      // cout << 87 << endl ; // TEST
      
      
    } // while 
    
    
  } // else 
  
 
  Out() ; 
  // React();
  
  return 0;
} // main()

////////////////////////////////////////////////////////////////////////

void Position(char & first) { // keep track of
  // 所get到的char的line number and column number
  // 這個function會一直讀到非空或非換行為止
  if ( first == '\0' ) {
    scanf( "%c", &first ) ;
    gColumn++ ; 
  
  } // if 
  else {
    while ( first == ' '  || first == '\n' || first == '\t' ) {
      if ( first == '\n' ) {
        gLine++ ; 
        gColumn = 1 ; 
      } // if
      else {
        gColumn++ ;  
      } // else 
    
      scanf( "%c", &first ) ; 
    
    
    } // while 
    
  } // else 
    
  
} // Position()

/////////////////////////////////////////////////////////////////////////////

void Token(char & first) { // get the next non-white-space char」
  // 此function要負責「跳過comment」

  char tokky[200] = {0} ; //用來存token候選人 
  int i = 0 ; 
  char look = '\0' ; 
  bool is2char = false ; // 用來判斷是否是2個的特殊符號
  

  Position(first);
 
  // cout << first << gLine << gColumn << endl ; 
  
  if ( first == '+' || first == '-' || first == '*' || first == '/' || first == '<' ||
          first == '>' || first == '=' || first == '!' || first == '?' || first == '^' || 
          first == '"' || first == 39 || first == '.' || first == ',' || first == '(' ||
          first == ')' || first == '{' || first == '}' || first == '[' || first == ']' ||
          first == ';' || first == ':' || first == '%' || first == '|' || first == '#' || 
          first == '&' ) {          // Case3 
    
    look = cin.peek() ; 
     
    if ( first == '/' && look == '/' ) { // 準備讀掉註解(單行)
      while ( first != '\n' ) {
        scanf( "%c", &first ) ; 
      } // while 
      
      gLine++ ; // 因為把enter讀掉了
    } // if 
    else if ( first == '/' && look == '*' ) { // 準備讀掉註解(段落)
      while ( first != '*' && look != '/' ) {
        if ( first == '\n' ) {
          gLine++ ;
        } // if 
        
        scanf( "%c", &first ) ; 
        look = cin.peek() ; 
      } // while
    } // else if 
    else if ( look == ' ' || look == '\n' || look == '\t' ) {
      is2char = false ; 
      Special( is2char, first );
    } // else if 
    else if ( first ==  '<' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '>' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '=' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '!' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '<' && look == '<' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '>' && look == '>' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '&' && look == '&' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '/' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '*' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '-' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '+' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '-' && look == '>' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '+' && look == '+' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '%' && look == '=' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    else if ( first == '|' && look == '|' ) {
      is2char = true ; 
      Special( is2char, first );
    } // else if 
    //...........................................// Case 2
    else if ( first == '.' && look == '0') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '1') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '2') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '3') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '4') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '5') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '6') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for 
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '7') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '8') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for 
      Num( tokky, first );
    } // else if 
    else if ( first == '.' && look == '9') {
      for ( i = 0 ; i <= 200 ; i++ ) {
        tokky[i] = '\0' ;
      } // for 
      Num( tokky, first );
    } // else if 
    //.............................................// Case2
    else if ( first == '"' ) {       //可能為字串 
      for ( i = 0 ; first != '"' && first != '\n' ; i++ ) {
        tokky[i] = first ; 
        scanf( "%c", &first ) ; 
      } // for 
      
      if ( first == '"' ) {                // 是字串
        tokky[i] = first ; 
        Num( tokky, first );
      } // if 
      else if ( first == '\n' ){       // 不是字串
        cin.putback( first ) ;
        first = '"' ; 
        for ( i = i - 1 ; i >= 1 ; i = i - 1 ) {
          cin.putback( tokky[i] ) ; 
        } // for 
        
        is2char = false ; 
        
        for ( i = 0 ; i <= 200 ; i++ ) {
          tokky[i] = '\0' ;
        } // for
        Special( is2char, first );
      } // else if 
      
    } // else if 
    else if ( first == 39 ) {        //可能為字元
      tokky[0] = first ; 
      for ( i = 1 ; i <= 2 ; i++ ) {
        scanf( "%c", &first ) ; 
        tokky[i] = first ; 
      } // for
      
      if ( first == 39 || tokky[2] == 39 ) { // 是字元
        Num( tokky, first );
      } // if 
      else {                                                 // 不是字元
        cin.putback(tokky[2]) ;
        cin.putback(tokky[1]) ;
        first = tokky[0] ;
        is2char = false ; 
        for ( i = 0 ; i <= 200 ; i++ ) {
          tokky[i] = '\0' ;
        } // for
        Special( is2char, first );
      } // else 
      
    } // else if 
  } // if
  else if ( first >= '0' && first <= '9' ) {        // Case2 數字                                                                          
    for ( i = 0 ; i <= 200 ; i++ ) {
      tokky[i] = '\0' ;
    } // for
    Num( tokky, first );
  } // else if 
  
  else if ( first >= 65 && first <= 90 ){
    Ident( first );                                                             // Case 1 底線加英文字
  } // else 
  else if ( first == '_' ) {
    Ident( first ); 
  } // else if 
  else if ( first >= 97 && first <= 122 ) {
    Ident( first ); 
  } // else if 
  
  else if ( look >= 48 && look <= 57 ){
    is2char = false ; 
    Special( is2char, first);
  } // else 
  else if ( look >= 65 && look <= 90 ){
    is2char = false ; 
    Special( is2char, first);
  } // else 
  else if ( look >= 97 && look <= 123 ){
    is2char = false ; 
    Special( is2char, first);
  } // else 
    
} // Token()

/////////////////////////////////

void Ident( char & first ) { // Case1底線 或 英文字母 開頭，之後跟著英文字母或底線或數字
                                               // 讀到換行，特殊符號或空白時停下(用艾希碼的範圍作條件)
  char tokky[200] = {0} ; 
  int i = 0 ; 
  char look = '\0' ; 
  look = cin.peek() ;  
  // cout << look << endl ; 
  bool continu = true ; 
  
  while ( continu ) {   // 先拿起整個token放到字串
    tokky[i] = first ; 
    i = i + 1 ; 
    if ( look == ' ' || look == '\n' || look == '\t' || look == '\0' ) {
       continu = false ; 
    } // if 
    else if ( look >= 33 && look <= 47 ) {
      continu = false ; 
    } // else if 
    else if ( look >= 58 && look <= 64 ) {
      continu = false ; 
    } // else if 
    else if ( look >= 91 && look <= 94 ) {
      continu = false ; 
    } // else 
    else if ( look >= 123 && look <= 126 ) {
      continu = false ; 
    } // else if 
    else {
      scanf( "%c", &first ) ; 
      look = cin.peek() ; 
      continu = true ; 
    } // else 
    
  } // while 
  
  
  
  gTemp = new Token1 ; 
  gTemp->next = NULL ; 
  gTemp->type = IDENTIFIER ; 
  gTemp->tokenStr = new char[ strlen(tokky)+ 6] ; 
  strcpy( gTemp->tokenStr, tokky ) ; 
  gTemp->firstAppearOn = NULL ; 
  gTemp->lastAppearOn = new Line ; 
  gTemp->lastAppearOn->line = gLine ; 
  gTemp->lastAppearOn->firstAppearAt = NULL ; 
  gTemp->lastAppearOn->lastAppearAt = new Column ; 
  gTemp->lastAppearOn->lastAppearAt->next = NULL ; 
  gTemp->lastAppearOn->lastAppearAt->column = gColumn ;
  
   
  
  gColumn = gColumn + strlen( tokky ) - 1 ; 
  // cout << gTemp->tokenStr << endl ;
  // exit (0) ;  
} // Ident()

///////////////////////////////////////

void Num( char tokky[200], char & first ) { // 數字 與 字串 與 字元
                      // tokky沒東西時，讀到非數字停下(用艾希碼的範圍作條件)(數字)
                      // 有東西時，複製那個東西(字串與字元)
  
  int i = 0 ; 
  char look = '\0' ; 
  look = cin.peek() ; 
  bool continu = true ; 
  
  if ( strcmp( tokky, "\0" ) == 0 ) {                                 // 數字
    if ( first == '.' || first >= 48 && first <= 57 ) {
      
      while ( continu ) {
        tokky[i] = first ; 
        i++ ; 
          
        if ( look == '\n' || look == ' ' || look == '\t' || look < 48 || look > 57 ) {
          if ( look == '.' ) {
            continu = true ; 
            scanf( "%c", &first ) ; 
            look = cin.peek() ; 
          } // if 
          else {
            continu = false ; 
          } // else 
        } // if 
        else {
          scanf( "%c", &first ) ; 
          look = cin.peek() ; 
        } // else 
          
        
        
      } // while
    } // if 
  } // if 
    
  gTemp = new Token1 ; 
  gTemp->next = NULL ; 
  gTemp->type = CONSTANT ; 
  gTemp->tokenStr = new char[ strlen(tokky)+ 1] ; 
  strcpy( gTemp->tokenStr, tokky ) ; 
  gTemp->firstAppearOn = NULL ; 
  gTemp->lastAppearOn = new Line ; 
  gTemp->lastAppearOn->line = gLine ; 
  gTemp->lastAppearOn->firstAppearAt = NULL ; 
  gTemp->lastAppearOn->lastAppearAt = new Column ; 
  gTemp->lastAppearOn->lastAppearAt->next = NULL ; 
  gTemp->lastAppearOn->lastAppearAt->column = gColumn ;
 
  // cout << gTemp->tokenStr << endl ; // TEST
  gColumn = gColumn + strlen( tokky ) - 1 ; 
  
} // Num()

//////////////////////////////////////////

void Special( bool is2char, char & first) { // 特殊符號

  int i = 0 ; 
  char tokky[200] = {0} ; 
  
  if ( is2char == true ) {
    tokky[0] = first ; 
    scanf( "%c", &first ) ; 
    tokky[1] = first ; 
  } // if 
  else {
    tokky[0] = first ; 
  } // else 
  
  gTemp = new Token1 ; 
  gTemp->next = NULL ; 
  gTemp->type = SPECIAL ; 
  gTemp->tokenStr = new char[ strlen(tokky)+ 1] ; 
  strcpy( gTemp->tokenStr, tokky ) ; 
  gTemp->firstAppearOn = NULL ; 
  gTemp->lastAppearOn = new Line ; 
  gTemp->lastAppearOn->line = gLine ; 
  gTemp->lastAppearOn->firstAppearAt = NULL ; 
  gTemp->lastAppearOn->lastAppearAt = new Column ; 
  gTemp->lastAppearOn->lastAppearAt->next = NULL ; 
  gTemp->lastAppearOn->lastAppearAt->column = gColumn ;
  
  gColumn = gColumn + strlen( tokky ) - 1 ; 

} // Special()

//////////////////////////////////

void Insert( TokenPtr & gTemp, TokenPtr & gFront, TokenPtr & gRear) { //按ASCII排順序
                                                                                                          
                                                                                                    
  
  if (gFront == NULL && uHas == false) {
    gFront = gTemp;
    gFront->firstAppearOn = gFront->lastAppearOn ; 
    gFront->firstAppearOn->firstAppearAt = gFront->lastAppearOn->lastAppearAt ; 
    gRear = gFront ; 
    gHead = gFront ;
    uHas = true;
    return ; 
  }// if 
  else if (gFront == NULL && uHas == true) {
    gFront = gHead;
    return ; 
  }// else if 
  else if ( uHas == true && strcmp( gFront->tokenStr, gTemp->tokenStr ) == 0 ) { // 同一個token
    if ( gFront->firstAppearOn == NULL ) {
      gFront->firstAppearOn = new Line ; 
      gFront->firstAppearOn->next = NULL ; 
      gFront->firstAppearOn->line = gTemp->lastAppearOn->line ; 
      gFront->firstAppearOn->firstAppearAt = new Column ; 
      gFront->firstAppearOn->firstAppearAt->next = NULL ; 
      gFront->firstAppearOn->firstAppearAt->column = gTemp->lastAppearOn->lastAppearAt->column ; 
      gFront->firstAppearOn->lastAppearAt = gFront->firstAppearOn->firstAppearAt ; 
      gFront->lastAppearOn = gFront->firstAppearOn ; 
    } // if 
    else {
      if ( gFront->lastAppearOn->line == gTemp->lastAppearOn->line ) { // 如果同一line
        gFront->lastAppearOn->lastAppearAt->next = new Column ;
        gFront->lastAppearOn->lastAppearAt->next->next = NULL ; 
        gFront->lastAppearOn->lastAppearAt->next->column = gTemp->lastAppearOn->lastAppearAt->column ;
        gFront->lastAppearOn->lastAppearAt = gFront->lastAppearOn->lastAppearAt->next ;
      } // if 
      else {                                                                                           // 如果不同line 
        gFront->lastAppearOn->next = new Line ;  
        gFront->lastAppearOn->next->next = NULL ; 
        gFront->lastAppearOn->next->line = gTemp->lastAppearOn->line ; 
        gFront->lastAppearOn->next->firstAppearAt = new Column ; 
        gFront->lastAppearOn->next->firstAppearAt->column = gTemp->lastAppearOn->lastAppearAt->column ; 
        gFront->lastAppearOn->next->firstAppearAt->next = NULL ; 
        gFront->lastAppearOn->next->lastAppearAt = gFront->lastAppearOn->next->firstAppearAt ; 
      } // else 
      
    } // else 
    
    return ; 
  } // else if 
  else if (uHas == true && gRear->next == NULL && strcmp( gRear->tokenStr, gTemp->tokenStr ) < 0 ) { // 接到尾巴
    gRear->next = gTemp;
    gRear->next->firstAppearOn = gRear->next->lastAppearOn ; 
    gRear->next->firstAppearOn->firstAppearAt = gRear->next->lastAppearOn->lastAppearAt ; 
    gRear = gRear->next ; 
    return ; 
  }// else if 
  else if (uHas == true && strcmp( gFront->tokenStr, gTemp->tokenStr ) > 0 ) { // 接到頭
    gTemp->next = gFront;
    gHead = gTemp;
    gFront = gTemp;
    gFront->firstAppearOn = gFront->lastAppearOn ; 
    gFront->firstAppearOn->firstAppearAt = gFront->firstAppearOn->lastAppearAt ;
    // gFront = gFront->next ; 
    return ; 
  }// else if 
  else if (uHas == true && gFront->next != NULL && strcmp( gFront->next->tokenStr, gTemp->tokenStr ) > 0 &&
                 strcmp( gFront->tokenStr, gTemp->tokenStr ) < 0 ) { // 接到中間
    gTemp->next = gFront->next;
    gFront->next = gTemp;
    gFront->next->firstAppearOn = gFront->next->lastAppearOn ; 
    gFront->next->firstAppearOn->firstAppearAt = gFront->next->firstAppearOn->lastAppearAt ; 
    // gFront = gFront->next ; 
    return ; 
  }// else if 
  else if (uHas == true && gFront->next != NULL && strcmp( gFront->next->tokenStr, gTemp->tokenStr ) <= 0 ) {
    Insert( gTemp, gFront->next, gRear );
  } // else if 
  
} // Insert()

/////////////////////////////////////////////////////////////////////////////////

void React(){ // 互動介面
  
  
} // React()

//////////////////////////////////////////////////////////////////////////////

void Out() { // 印出整個linked list
  // cout << "bitch" << endl ;  
  int i = 0 ; 
  while ( gFront != NULL ) {
    
    cout << gFront->tokenStr << ' ' ; 
    
    while ( gFront->firstAppearOn != NULL ) {
      
      
      while ( gFront->firstAppearOn->firstAppearAt != NULL  ) {
        cout << '(' << gFront->firstAppearOn->line << ',' << gFront->firstAppearOn->firstAppearAt->column ; 
        cout << ')' ;
        gFront->firstAppearOn->firstAppearAt = gFront->firstAppearOn->firstAppearAt->next ; 
      } // while 
      
      gFront->firstAppearOn = gFront->firstAppearOn->next ; 
    } // while 
    
    cout << endl ; 
     gFront = gFront->next ;
  } // while
  
} // Out()
