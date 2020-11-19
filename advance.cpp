# include <stdio.h>
# include <iostream>
# include <math.h>
# include <list>
# include <string.h>
# include <string>
# include <cstring>
# include <stdlib.h>
# include <cstdlib>
# include <vector>
# include <sstream>

# include <iomanip>

using namespace std ;

struct WatWat
{ // a token and its type
  string tokName ;
  bool isInt ;
  bool isFloat ;
  bool isString ;
  bool isNil ;
  bool isT ;
  bool isDot ;
  bool isQuote ;
  bool isLeftPan ;
  bool isRightPan ;
  bool isSymbol ;
  bool isComment ;
} ;

struct Node
{
  WatWat token ;
  Node *left ;
  Node *right ;
} ;

// ///////////////////  global variables  // ///////////////////

int gTestNum = -1 ;

char g_nextChar = '\0' ; // the next char
bool g_isEOF = false ;
WatWat g_nextToken ; // the next token
int g_Line = 1 ;
int g_Col = 0 ;

WatWat g_peekToken ; // buffer the next token for peek
int g_PeekLine = -1 ;
int g_PeekCol = -1 ;
bool g_hasOuterLefPan = false ;
bool g_hasOuterRighPan = false ;
Node *g_root ;

vector<WatWat> g_tokenList ; // every time GetNextToken() is called, push the token to this list

string g_error_message = "" ;
bool g_hasCommentBe4 = false ;

// ////////////////////////////////////////////////////////////////

bool GetNextChar()
{ // EOF global bool
  if ( scanf( "%c", &g_nextChar ) != EOF )
  {
    g_Col++ ;
    if ( g_nextChar == '\n' ) {
      g_Line++ ;
      g_Col = 0 ;
    } // if

    return true ;
  } // if
  else
  {
    g_isEOF = true ;
    return false ;
  } // else
} // GetNextChar()

void DumpWholeList()
{
  cout << " the list of token : " << endl ;
  for ( int a = 0 ; a < g_tokenList.size() ; a++ )
  {
    cout << g_tokenList.at( a ).tokName << endl ;
  } // for
} // DumpWholeList()

void DebugTokenCheck()
{ // 用來檢查現在的token是不是被我設了兩個以上的true 判斷成兩種不童的terminal
  int a = 0 ;
  string a_str = "g_nextToken >>" + g_nextToken.tokName + "<< is a " ;
  int b = 0 ;
  string b_str = "g_peekToken >>" + g_peekToken.tokName + "<< is a " ;

  if ( g_nextToken.isComment )
  {
    a++ ;
    a_str = a_str + "Comment " ;
  } // if

  if ( g_nextToken.isDot )
  {
    a++ ;
    a_str = a_str + "Dot " ;
  } // if

  if ( g_nextToken.isFloat )
  {
    a++ ;
    a_str = a_str + "Float " ;
  } // if

  if ( g_nextToken.isInt )
  {
    a++ ;
    a_str = a_str + "Int " ;
  } // if

  if ( g_nextToken.isLeftPan )
  {
    a++ ;
    a_str = a_str + "LeftPan " ;
  } // if

  if ( g_nextToken.isNil )
  {
    a++ ;
    a_str = a_str + "Nil " ;
  } // if

  if ( g_nextToken.isQuote )
  {
    a++ ;
    a_str = a_str + "Quote " ;
  } // if

  if ( g_nextToken.isRightPan )
  {
    a++ ;
    a_str = a_str + "RightPan " ;
  } // if

  if ( g_nextToken.isString )
  {
    a++ ;
    a_str = a_str + "String " ;
  } // if

  if ( g_nextToken.isSymbol )
  {
    a++ ;
    a_str = a_str + "Symbol " ;
  } // if

  if ( g_nextToken.isT )
  {
    a++ ;
    a_str = a_str + "True " ;
  } // if

  // ----------------------------

  if ( g_peekToken.isComment )
  {
    b++ ;
    b_str = b_str + "Comment " ;
  } // if

  if ( g_peekToken.isDot )
  {
    b++ ;
    b_str = b_str + "Dot " ;
  } // if

  if ( g_peekToken.isFloat )
  {
    b++ ;
    b_str = b_str + "Float " ;
  } // if

  if ( g_peekToken.isInt )
  {
    b++ ;
    b_str = b_str + "Int " ;
  } // if

  if ( g_peekToken.isLeftPan )
  {
    b++ ;
    b_str = b_str + "LeftPan " ;
  } // if

  if ( g_peekToken.isNil )
  {
    b++ ;
    b_str = b_str + "Nil " ;
  } // if

  if ( g_peekToken.isQuote )
  {
    b++ ;
    b_str = b_str + "Quote " ;
  } // if

  if ( g_peekToken.isRightPan )
  {
    b++ ;
    b_str = b_str + "RightPan " ;
  } // if

  if ( g_peekToken.isString )
  {
    b++ ;
    b_str = b_str + "String " ;
  } // if

  if ( g_peekToken.isSymbol )
  {
    b++ ;
    b_str = b_str + "Symbol " ;
  } // if

  if ( g_peekToken.isT )
  {
    b++ ;
    b_str = b_str + "True " ;
  } // if

  cout << a_str << endl ;

} // DebugTokenCheck()

bool GetNextToken()
{
  string temp = "" ;
  g_nextToken.tokName = "" ;       // buffer is now empty
  g_nextToken.isComment = false ;  // buffer is now empty
  g_nextToken.isDot = false ;      // buffer is now empty
  g_nextToken.isFloat = false ;    // buffer is now empty
  g_nextToken.isInt = false ;      // buffer is now empty
  g_nextToken.isLeftPan = false ;  // buffer is now empty
  g_nextToken.isNil = false ;      // buffer is now empty
  g_nextToken.isQuote = false ;    // buffer is now empty
  g_nextToken.isRightPan = false ; // buffer is now empty
  g_nextToken.isString = false ;   // buffer is now empty
  g_nextToken.isSymbol = false ;   // buffer is now empty
  g_nextToken.isT = false ;        // buffer is now empty

  if ( strcmp( g_peekToken.tokName.c_str(), "" ) == 0 )
  { // if theres no token in buffer
    while ( ( g_nextChar == '\0' || g_nextChar == ' ' || g_nextChar == '\t' || g_nextChar == '\n' ) &&
            g_isEOF == false )
    {
      // skip white space
      g_nextChar = '\0' ;

      GetNextChar() ;

    } // while

    if ( g_isEOF )
    { // no more next token left
      return false ;
    } // if
    else if ( g_nextChar == '(' )
    {
      temp.push_back( g_nextChar ) ;
      g_nextToken.tokName = temp ;
      g_nextToken.isLeftPan = true ;
      g_nextChar = '\0' ; // 每次推進去之後這個char歸零
      return true ;
      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == ')' )
    {
      temp.push_back( g_nextChar ) ;
      g_nextToken.tokName = temp ;
      g_nextToken.isRightPan = true ;
      g_nextChar = '\0' ;
      return true ;
      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == '\'' )
    { // QUOTE
      temp.push_back( g_nextChar ) ;
      g_nextToken.tokName = temp ;
      g_nextToken.isQuote = true ;
      g_nextChar = '\0';
      return true;
      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == ';' )
    { // COMMENT
      while ( g_nextChar != '\n' && g_isEOF == false )
      {
        temp.push_back( g_nextChar );
        GetNextChar() ;
      } // while

      g_nextToken.tokName = temp ;
      g_nextToken.isComment = true ;
      g_nextChar = '\0';

      return true ;
      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == '\"' )
    { // STRING
      temp.push_back( g_nextChar );
      int saveBe4_Line = g_Line ;
      int saveBe4_Col = g_Col ;
      GetNextChar() ;
      while ( g_nextChar != '\n' && g_nextChar != '\"' && g_isEOF == false )
      {
        temp.push_back( g_nextChar ) ;
        if ( g_nextChar == '\\' ) {
          if ( GetNextChar() && g_nextChar != '\n' && g_isEOF == false ) {
            temp.push_back( g_nextChar ) ;
          } // if
        } // if

        saveBe4_Line = g_Line ;
        saveBe4_Col = g_Col ;
        GetNextChar() ;
      } // while

      if ( g_nextChar != '\"' )
      { // if the last one is not closing quote
        char s11[64] ;
        char s22[64] ;
        sprintf( s11, "%d", saveBe4_Line ) ;
        sprintf( s22, "%d", saveBe4_Col ) ;
        string s1( s11 ) ;
        string s2( s22 ) ;
        g_error_message = "ERROR (no closing quote) : END-OF-LINE encountered at Line " +
                          s1 + " Column " + s2 ;
        return false ;
      } // if
      else
      {
        temp.push_back( g_nextChar ) ; // 把最後的 " 推進去
        g_nextToken.tokName = temp ;
        g_nextToken.isString = true ;
        g_nextChar = '\0' ;
        return true ;
      } // else

      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == '+' || g_nextChar == '-' || g_nextChar == '.' ||
              ( g_nextChar >= 48 && g_nextChar <= 57 ) )
    { // INT or FLOAT or DOT

      int hasDot = 0 ;
      int hasSign = 0 ;
      bool sym = false;
      bool hasNum = false ;

      // check the first char
      if ( g_nextChar == '+' || g_nextChar == '-' )
      {
        hasSign ++ ;
        temp.push_back( g_nextChar ) ;
        if ( GetNextChar() )
        {
          if ( g_nextChar == '.' )
          {
            hasDot++ ;
          } // if
          else if ( ( g_nextChar >= 48 && g_nextChar <= 57 ) ) {
            hasNum = true ;
          } // else if
          else {
            sym = true ;
          } // else
        } // if
        else
        { // 只有一個加記號
          g_nextToken.tokName = temp ;
          g_nextToken.isSymbol = true ;
          g_nextChar = '\0' ;
          return true ;
        } // else
      }   // if
      else if ( g_nextChar == '.' )
      {
        hasDot++ ;
        hasSign ++ ; // 之後也不能有sign
        temp.push_back( g_nextChar ) ;
        if ( GetNextChar() )
        {
          if ( g_nextChar == ' ' || g_nextChar == '\t' || g_nextChar == '\n' || g_nextChar == '(' ||
               g_nextChar == ')' || g_nextChar == '\'' || g_nextChar == ';' || g_nextChar == '\"' )
          { // 遇到terminal 切

            g_nextToken.tokName = temp ;
            g_nextToken.isDot = true ;

            return true;
          } // if
        }   // if
        else
        {
          g_nextToken.tokName = temp ;
          g_nextToken.isDot = true ;
          g_nextChar = '\0' ;
          return true ;
        } // else
      }   // else if
      else {
        hasNum = true ;
      } // else

      temp.push_back( g_nextChar );
      GetNextChar() ;
      // 再來 只能接受數字或點(點要在之前沒點的情況下接受) 否則就是symbol
      while ( g_isEOF == false && g_nextChar != ' ' && g_nextChar != '\t' && 
              g_nextChar != '\n' && g_nextChar != '(' &&
              g_nextChar != ')' && g_nextChar != '\'' &&
              g_nextChar != ';' && g_nextChar != '\"' )
      {
        if ( g_nextChar >= 48 && g_nextChar <= 57 )
        {
          hasNum = true ;
        } // if
        else if ( g_nextChar == '.' )
        {
          hasDot++ ;
        } // else if
        else if ( g_nextChar == '+' || g_nextChar == '-' ) {
          hasSign++ ;
        } // else if
        else {
          sym = true ;
        } // else

        temp.push_back( g_nextChar ) ;
        GetNextChar() ;
      } // while

      if ( hasDot > 1 || hasSign > 1 ) {
        sym = true ;
      } // if
      else if ( hasNum != true ) {
        sym = true ;
      } // else if

      if ( sym == true )
      { // 這是symbol
        g_nextToken.tokName = temp ;
        g_nextToken.isSymbol = true ;
        return true ;
      } // if
      else if ( hasDot == 1 )
      { // 這是float
        g_nextToken.tokName = temp ;
        g_nextToken.isFloat = true ;
        return true;
      } // else if
      else
      { // 這是 int
        g_nextToken.tokName = temp ;
        g_nextToken.isInt = true ;
        return true ;
      } // else

      // -----------------------------------------------------------------------------------
    } // else if
    else if ( g_nextChar == '#' || g_nextChar == 'n' || g_nextChar == 't' )
    { // nil or T
      temp.push_back( g_nextChar );
      if ( GetNextChar() )
      {
        while ( g_isEOF == false && g_nextChar != ' ' && g_nextChar != '\t' && g_nextChar != '\n' &&
                g_nextChar != ';' && g_nextChar != '(' && g_nextChar != ')' && g_nextChar != '\'' &&
                g_nextChar != '\"' )
        { // 到遇到terminal為止
          temp.push_back( g_nextChar );
          GetNextChar() ;
        }   // while

        if ( strcmp( temp.c_str(), "nil" ) == 0 || strcmp( temp.c_str(), "#f" ) == 0 )
        {
          g_nextToken.tokName = temp ;
          g_nextToken.isNil = true ;
          return true ;
        } // if
        else if ( strcmp( temp.c_str(), "t" ) == 0 || strcmp( temp.c_str(), "#t" ) == 0 )
        {
          g_nextToken.tokName = temp ;
          g_nextToken.isT = true ;
          return true ;
        } // else if
        else
        {
          g_nextToken.tokName = temp ;
          g_nextToken.isSymbol = true ;
          return true ;
        } // else

      } // if
      else if ( g_nextChar == 't' )
      { // 果只有一個t 表示是true
        g_nextToken.tokName = temp ;
        g_nextToken.isT = true ;
        g_nextChar = '\0';
        return true ;
      } // else if
      else
      { //      如果只有一個# 表示是symbol
        g_nextToken.tokName = temp ;
        g_nextToken.isSymbol = true ;
        g_nextChar = '\0' ;
        return true ;
      } // else
    } // else if
    else
    { // SYMBOL
      temp.push_back( g_nextChar );
      GetNextChar() ;
      while ( g_isEOF == false && g_nextChar != '(' && g_nextChar != ')' && g_nextChar != '\'' &&
              g_nextChar != '\"' && g_nextChar != ';' && g_nextChar != ' ' && g_nextChar != '\t' &&
              g_nextChar != '\n' )
      {
        temp.push_back( g_nextChar ) ;
        GetNextChar() ;
      } // while

      g_nextToken.tokName = temp ;
      g_nextToken.isSymbol = true ;
      if ( g_nextChar == ' ' || g_nextChar == '\t' || g_nextChar == '\n' )
      {
        g_nextChar = '\0' ;
      } // if

      return true ;
    } // else
  } // if
  else
  {                            // if theres a token in buffer
    g_nextToken = g_peekToken ; // send the token from buffer to nextToken
    g_Line = g_PeekLine ;
    g_Col = g_PeekCol ;

    g_peekToken.tokName = "" ;       // buffer is now empty
    g_peekToken.isComment = false ;  // buffer is now empty
    g_peekToken.isDot = false ;      // buffer is now empty
    g_peekToken.isFloat = false ;    // buffer is now empty
    g_peekToken.isInt = false ;      // buffer is now empty
    g_peekToken.isLeftPan = false ;  // buffer is now empty
    g_peekToken.isNil = false ;      // buffer is now empty
    g_peekToken.isQuote = false ;    // buffer is now empty
    g_peekToken.isRightPan = false ; // buffer is now empty
    g_peekToken.isString = false ;   // buffer is now empty
    g_peekToken.isSymbol = false ;   // buffer is now empty
    g_peekToken.isT = false ;        // buffer is now empty
    g_PeekLine = -1 ;
    g_PeekCol = -1 ;

    return true ;
  } // else

  return false ;

} // GetNextToken()

// ///////////////////////////////////////////////////////////

bool PeekNextToken()
{
  WatWat temp = g_nextToken;
  int peLine = g_Line;
  int peCol = g_Col;

  if ( strcmp( g_peekToken.tokName.c_str(), "" ) != 0 )
  { // if buffer already has a token
    return true;
  } // if
  else if ( GetNextToken() )
  {
    g_peekToken = g_nextToken;
    g_nextToken = temp;

    g_PeekCol = g_Col;
    g_Col = peCol;

    g_PeekLine = g_Line;
    g_Line = peLine;

    return true;
  } // else if
  else
  {
    return false ; // encounter EOF
  } // else

} // PeekNextToken()

// ///////////////////////////////////////////////////////////

bool Atom( WatWat tok )
{ // Atom() 不會get token 只能peek

  if ( tok.isFloat == true || tok.isInt == true || tok.isSymbol == true || tok.isString == true ||
       tok.isNil == true || tok.isT == true || tok.isComment == true )
  {

    return true;
  } // if
  else if ( tok.isLeftPan == true )
  {
    if ( PeekNextToken() )
    {
      if ( g_peekToken.isRightPan == true )
      { // 把()直接變nil
        string tt = "()";
        g_tokenList.at( g_tokenList.size() - 1 ).tokName = tt;
        g_tokenList.at( g_tokenList.size() - 1 ).isNil = true;
        g_tokenList.at( g_tokenList.size() - 1 ).isLeftPan = false;

        g_peekToken.tokName = "";       // buffer is now empty
        g_peekToken.isComment = false;  // buffer is now empty
        g_peekToken.isDot = false;      // buffer is now empty
        g_peekToken.isFloat = false;    // buffer is now empty
        g_peekToken.isInt = false;      // buffer is now empty
        g_peekToken.isLeftPan = false;  // buffer is now empty
        g_peekToken.isNil = false;      // buffer is now empty
        g_peekToken.isQuote = false;    // buffer is now empty
        g_peekToken.isRightPan = false; // buffer is now empty
        g_peekToken.isString = false;   // buffer is now empty
        g_peekToken.isSymbol = false;   // buffer is now empty
        g_peekToken.isT = false;        // buffer is now empty
        g_PeekLine = -1;
        g_PeekCol = -1;
        return true;
      } // if
      else
      {
        return false;
      } // else
    }   // if
  }     // else if

  return false; // everything else
} // Atom()

// -------------------------------------------------------------

bool ReadSExp()
{
  if ( GetNextToken() )
  {
    // DebugTokenCheck() ; // test
    g_tokenList.push_back( g_nextToken ); // 每次叫GetNextToken後都要把Token推進去
    // DebugTokenCheck();                  // test
    WatWat nowTok = g_nextToken;
    if ( Atom( nowTok ) )
    { // <ATOM>
      return true;
    } // if
    else if ( nowTok.isLeftPan )
    { // Left-Paren <S-exp> { <S-exp> } [ DOT <S-exp> ] Right-Paren
      if ( ReadSExp() )
      { // <S-exp>
        
        nowTok = g_nextToken ;
        while ( ReadSExp() )
        { // { <S-exp> } // 0 ~ 多個
          nowTok = g_nextToken ;
        } // while
        
        nowTok = g_nextToken ;

        if ( nowTok.isDot && g_error_message.compare( "" ) == 0 )
        { // [ DOT <S-exp> ] // 0~1個
          if ( ReadSExp() )
          {
            nowTok = g_nextToken ;
            if ( GetNextToken() ) {
              // DebugTokenCheck() ; // test
              g_tokenList.push_back( g_nextToken ); // 每次叫GetNextToken後都要把Token推進去
              nowTok = g_nextToken;
            } // if
            else {
              if ( g_error_message.compare( "" ) == 0 ) {
                g_error_message = "ERROR (no more input) : END-OF-FILE encountered" ;
              } // if
            } // else

            // proceed to the if below : Right-Pan

          } // if
          else
          {
            nowTok = g_nextToken ;
            if ( g_error_message.compare( "" ) == 0 ) {
              char s11[64] ;
              char s22[64] ;
              sprintf( s11, "%d", g_Line ) ;
              sprintf( s22, "%d", g_Col ) ;
              string s1( s11 ) ;
              string s2( s22 ) ;
              g_error_message = "ERROR (unexpected token) : ')' expected when token at Line " + s1 +
                                " Column " + s2 + " is >>" + nowTok.tokName + "<<" ;
            } // if

            return false;
          } // else

          nowTok = g_nextToken;
        }   // if

        if ( nowTok.isRightPan && g_error_message.compare( "" ) == 0 )
        { // Right-Paren
          return true;
        } // else if
        else
        {
          if ( g_error_message.compare( "" ) == 0 ) {
            char s11[64] ;
            char s22[64] ;
            sprintf( s11, "%d", g_Line ) ;
            sprintf( s22, "%d", g_Col ) ;
            string s1( s11 ) ;
            string s2( s22 ) ;
            g_error_message = "ERROR (unexpected token) : ')' expected when token at Line " + s1 +
                              " Column " + s2 + " is >>" + nowTok.tokName + "<<" ;
          } // if

          return false;
        } // else

      } // if
      else
      {
        nowTok = g_nextToken ;
        if ( g_error_message.compare( "" ) == 0 ) {
          char s11[64] ;
          char s22[64] ;
          sprintf( s11, "%d", g_Line ) ;
          sprintf( s22, "%d", g_Col ) ;
          string s1( s11 ) ;
          string s2( s22 ) ;
          g_error_message = "ERROR (unexpected token) : atom or '(' expected when token at Line " + s1 +
                            " Column " + s2 + " is >>" + nowTok.tokName + "<<" ;
        } // if

        return false;
      } // else
    } // else if
    else if ( nowTok.isQuote )
    { // QUOTE <S-exp>
      if ( ReadSExp() )
      {
        nowTok = g_nextToken ;
        return true;
      } // if
      else
      {
        nowTok = g_nextToken ;
        if ( g_error_message.compare( "" ) == 0 ) {
          char s11[64] ;
          char s22[64] ;
          sprintf( s11, "%d", g_Line );
          sprintf( s22, "%d", g_Col );
          string s1( s11 );
          string s2( s22 );
          g_error_message = "ERROR (unexpected token) : atom or '(' expected when token at Line " + s1 +
                           " Column " + s2 + " is >>" + nowTok.tokName + "<<" ;
        } // if

        return false;
      } // else
    }   // else if
    else {
      nowTok = g_nextToken ;
      return false ;
    } // else
  } // if
  else
  { // EOF or no closing "
    if ( g_isEOF ) {
      g_error_message = "ERROR (no more input) : END-OF-FILE encountered" ;
      g_isEOF = true ;
    } // if

    return false;
  } // else

} // ReadSExp()

// -------------------------------------------------------------

bool AtomForTree( WatWat tok )
{
  if ( tok.isFloat == true || tok.isInt == true || tok.isSymbol == true || tok.isString == true ||
       tok.isNil == true || tok.isT == true || tok.isComment == true )
  {
    return true;
  } // if
  else
  {
    return false; // everything else
  }               // else
} // AtomForTree()

// --------------------------------------------------------------------------------------------

void BuildTokenTree( int &currentIndex, vector<WatWat> tokenList, Node *current )
{
  
  if ( currentIndex >= tokenList.size() )
  {
    return ;
  } // if
  else if ( tokenList.at( currentIndex ).isLeftPan )
  {
    Node *temp = new Node;
    temp->token.tokName = "Boner"; // internal node
    temp->token.isComment = false ;
    temp->token.isDot = false ;
    temp->token.isFloat = false ;
    temp->token.isInt = false ;
    temp->token.isLeftPan = false ;
    temp->token.isNil = false ;
    temp->token.isQuote = false ;
    temp->token.isRightPan = false ;
    temp->token.isString = false ;
    temp->token.isSymbol = false ;
    temp->token.isT = false ;
    temp->right = NULL ;
    temp->left = NULL ;
    if ( current != NULL ) {
      current->left = temp ;
      current = current->left ;
    } // if
    else {
      current = temp ;
    } // else

    
    if ( currentIndex == 0 ) {
      g_root = current ;
      g_hasOuterLefPan = true ;
      g_hasOuterRighPan = true ;
    } // if

    currentIndex = currentIndex + 1;

    BuildTokenTree( currentIndex, tokenList, current );

    if ( currentIndex-1 >= 0 && current->right == NULL ) {
      BuildTokenTree( currentIndex, tokenList, current );
    } // if
    
    return ;
  } // else if
  else if ( AtomForTree( tokenList.at( currentIndex ) ) )
  {
    if ( currentIndex == 0 ) {
      Node *temp = new Node;
      temp->token.tokName = "Boner"; // internal node
      temp->token.isComment = false ;
      temp->token.isDot = false ;
      temp->token.isFloat = false ;
      temp->token.isInt = false ;
      temp->token.isLeftPan = false ;
      temp->token.isNil = false ;
      temp->token.isQuote = false ;
      temp->token.isRightPan = false ;
      temp->token.isString = false ;
      temp->token.isSymbol = false ;
      temp->token.isT = false ;
      current = temp ;
      current->right = NULL ;
      current->left = NULL ;
      g_root = current ;
    } // if
    
    Node *temp = new Node;
    temp->token = tokenList.at( currentIndex );
    temp->left = NULL;
    temp->right = NULL;
    current->left = temp ;
    currentIndex++ ;
    
    if ( currentIndex < tokenList.size() && AtomForTree( tokenList.at( currentIndex ) ) ) {
      current->right = new Node ;
      current->right->right = NULL ;
      current->right->left = NULL ;
      current->right->token.tokName = "Boner"; // internal node
      current->right->token.isComment = false ;
      current->right->token.isDot = false ;
      current->right->token.isFloat = false ;
      current->right->token.isInt = false ;
      current->right->token.isLeftPan = false ;
      current->right->token.isNil = false ;
      current->right->token.isQuote = false ;
      current->right->token.isRightPan = false ;
      current->right->token.isString = false ;
      current->right->token.isSymbol = false ;
      current->right->token.isT = false ;
      BuildTokenTree( currentIndex, tokenList, current->right );
    } // if
    else if ( currentIndex < tokenList.size() && tokenList.at( currentIndex ).isRightPan ) {
      BuildTokenTree( currentIndex, tokenList, current ) ;
    } // else if
    else {
      BuildTokenTree( currentIndex, tokenList, current );
    } // else

    return ;
  } // else if
  else if ( tokenList.at( currentIndex ).isRightPan )
  {
    currentIndex++;
    return ;
  } // else if
  else if ( tokenList.at( currentIndex ).isDot )
  {
    currentIndex ++ ;
    
    if ( currentIndex < tokenList.size() && tokenList.at( currentIndex ).isLeftPan ) {
      Node *temp = new Node;
      temp->token.tokName = "Boner"; // internal node
      temp->token.isComment = false ;
      temp->token.isDot = false ;
      temp->token.isFloat = false ;
      temp->token.isInt = false ;
      temp->token.isLeftPan = false ;
      temp->token.isNil = false ;
      temp->token.isQuote = false ;
      temp->token.isRightPan = false ;
      temp->token.isString = false ;
      temp->token.isSymbol = false ;
      temp->token.isT = false ;
      temp->left = NULL ;
      temp->right = NULL ;
      current->right = temp ;
      currentIndex++ ;
      BuildTokenTree( currentIndex, tokenList, current->right ) ;
      return ;
    } // if
    else if ( currentIndex < tokenList.size() && AtomForTree( tokenList.at( currentIndex ) ) ) {
      Node *temp = new Node;
      temp->token = tokenList.at( currentIndex );
      temp->left = NULL ;
      temp->right = NULL ;
      current->right = temp ;
      
      currentIndex++ ;
      BuildTokenTree( currentIndex, tokenList, current ) ;
    } // else if
    else {
      cout << "Weird encounter after DOT when building a tree !" << endl ;
    } // else

    return ;
  } // else if
  else if ( tokenList.at( currentIndex ).isQuote ) {
    if ( currentIndex == 0 ) {
      Node *temp = new Node;
      temp->left = NULL ;
      temp->right = NULL ;
      temp->token.tokName = "Boner"; // internal node
      temp->token.isComment = false ;
      temp->token.isDot = false ;
      temp->token.isFloat = false ;
      temp->token.isInt = false ;
      temp->token.isLeftPan = false ;
      temp->token.isNil = false ;
      temp->token.isQuote = false ;
      temp->token.isRightPan = false ;
      temp->token.isString = false ;
      temp->token.isSymbol = false ;
      temp->token.isT = false ;
      current = temp ;
      g_root = current ;
    } // if
    
    Node *temp = new Node;
    current->left = temp ;
    temp->token = tokenList.at( currentIndex );
    currentIndex++ ;
    temp->left = NULL;
    temp->right = NULL;

    current->right = new Node ;
    current->right->token.tokName = "Boner"; // internal node
    current->right->token.isComment = false ;
    current->right->token.isDot = false ;
    current->right->token.isFloat = false ;
    current->right->token.isInt = false ;
    current->right->token.isLeftPan = false ;
    current->right->token.isNil = false ;
    current->right->token.isQuote = false ;
    current->right->token.isRightPan = false ;
    current->right->token.isString = false ;
    current->right->token.isSymbol = false ;
    current->right->token.isT = false ;
    current->right->left = NULL ;
    current->right->right = NULL ;
    BuildTokenTree( currentIndex, tokenList, current->right );
  } // else if
  else
  { // Debug
    cout << "cant decide where to put >>" << tokenList.at( currentIndex ).tokName <<
            "<< at list " << currentIndex << endl;
  } // else

  return ;
} // BuildTokenTree()


// -------------------------------------------------------------

void PrintSExp( Node *g_root, int &indent )
{ // 記得先判斷這個token是不是null
  if ( g_root == NULL ) {
    return ;
  } // if
  else {
    if ( AtomForTree( g_root->token ) || g_root->token.isQuote ) {
      if ( g_root->token.isFloat == true ) {
        float te = 0.0 ;
        stringstream ss ;
        ss << g_root->token.tokName ;
        ss >> te ;
        printf( "%.3f", te ) ;
        cout << endl ;
      } // if
      else if ( g_root->token.isInt )
      {
        int te = 0 ;
        stringstream ss ;
        ss << g_root->token.tokName ;
        ss >> te ;
        printf( "%d", te ) ;
        cout << endl ;
      } // else if
      else if ( g_root->token.isQuote ) {
        // suppose do nothing in project 1
      } // else if
      else if ( g_root->token.isT ) {
        cout << "#t" << endl ;
      } // else if
      else if ( g_root->token.isComment ) {
        g_hasCommentBe4 = true ;
      } // else if
      else if ( g_root->token.isString ) {
        int a = 0 ;
        bool escap = false ;
        while ( a < g_root->token.tokName.length() )
        {
          if ( g_root->token.tokName.at( a ) == '\\' ) {
            if ( a+1 < g_root->token.tokName.length() ) {
              if ( g_root->token.tokName.at( a+1 ) == '\\' ) {
                if ( escap ) {
                  escap = false ;
                  cout << '\\' ;
                  a = a + 2 ;
                } // if
                else {
                  escap = true ;
                  a++ ;
                } // else
              } // if
              else if ( g_root->token.tokName.at( a+1 ) == 'n' ) {
                if ( escap ) {
                  escap = false ;
                  cout << '\\' << 'n' ;
                } // if
                else {
                  cout << '\n' ;
                } // else

                a = a + 2 ;
              } // else if
              else if ( g_root->token.tokName.at( a+1 ) == 't' ) {
                if ( escap ) {
                  escap = false ;
                  cout << '\\' << 't' ;
                } // if
                else {
                  cout << '\t' ;
                } // else

                a = a + 2 ;
              } // else if
              else if ( g_root->token.tokName.at( a+1 ) == '\"' ) {
                if ( escap ) {
                  escap = false ;
                  cout << '\"' ;
                } // if
                else {
                  cout << '\"' ;
                } // else
                
                a = a + 2 ;
              } // else if
              
            } // if
          } // if

          cout << g_root->token.tokName.at( a ) ;
          a++ ;
        } // while
        
        cout << endl ;
      } // else if
      else if ( g_root->token.isNil != true ) {
        cout << g_root->token.tokName << endl ;
      } // else if
      
      return ;
    } // if
    else {
      if ( g_root->left != NULL && g_root->left->token.isNil ) {
        cout << "nil" << endl ;
      } // if
      else if ( g_root->left->token.tokName.compare( "Boner" ) == 0 ) {
        cout << "( " ;
        indent++ ;
      } // else if
      else if ( g_hasOuterLefPan ) {
        cout << "( " ;
        indent++ ;
        g_hasOuterLefPan = false ;
      } // else if
      
      PrintSExp( g_root->left, indent ) ;

      if ( g_root->right == NULL ) {
        indent-- ;
      } // if

      if ( g_root->token.tokName.compare( "Boner" ) == 0 ) {
        int a = 0 ;
        while ( a < indent )
        {
          cout << " " ;
          a++;
        } // while
      } // if

      if ( g_root->right == NULL && indent >= 0 ) {
        cout << ")" << endl ;
      } // if
      else if ( g_root->right != NULL &&
                AtomForTree( g_root->right->token ) ) {
        
        if ( g_root->right->token.isNil == false ) {
          cout << "." << endl ;
        } // if

        int a = 0 ;
        while ( a < indent )
        {
          cout << " " ;
          a++;
        } // while

        PrintSExp( g_root->right, indent ) ;
        indent-- ;
        a = 0 ;
        while ( a < indent )
        {
          cout << " " ;
          a++;
        } // while

        cout << ")" << endl ;
      } // else if
      else if ( g_root->right != NULL && g_root->right->token.tokName.compare( "Boner" ) == 0 ) {
        PrintSExp( g_root->right, indent ) ;
      } // else if
      
      return ;
    } // else
    
    return ;
  } // else

  return ;
} // PrintSExp()

// -------------------------------------------------------------

void AllCleanUp()
{
  g_nextChar = '\0'; // the next char
  g_error_message = "" ;
  g_nextToken.tokName = "";       // the next token
  g_nextToken.isComment = false;  // buffer is now empty
  g_nextToken.isDot = false;      // buffer is now empty
  g_nextToken.isFloat = false;    // buffer is now empty
  g_nextToken.isInt = false;      // buffer is now empty
  g_nextToken.isLeftPan = false;  // buffer is now empty
  g_nextToken.isNil = false;      // buffer is now empty
  g_nextToken.isQuote = false;    // buffer is now empty
  g_nextToken.isRightPan = false; // buffer is now empty
  g_nextToken.isString = false;   // buffer is now empty
  g_nextToken.isSymbol = false;   // buffer is now empty
  g_nextToken.isT = false;        // buffer is now empty

  g_peekToken.tokName = "";       // buffer the next token for peek
  g_peekToken.isComment = false;  // buffer is now empty
  g_peekToken.isDot = false;      // buffer is now empty
  g_peekToken.isFloat = false;    // buffer is now empty
  g_peekToken.isInt = false;      // buffer is now empty
  g_peekToken.isLeftPan = false;  // buffer is now empty
  g_peekToken.isNil = false;      // buffer is now empty
  g_peekToken.isQuote = false;    // buffer is now empty
  g_peekToken.isRightPan = false; // buffer is now empty
  g_peekToken.isString = false;   // buffer is now empty
  g_peekToken.isSymbol = false;   // buffer is now empty
  g_peekToken.isT = false;        // buffer is now empty
  g_PeekLine = -1;
  g_PeekCol = -1;

  g_tokenList.clear(); // every time GetNextToken() is called, push the token to this list

} // AllCleanUp()

// ％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％

void printSubtree(Node* g_root, const string& prefix)
{
        if (g_root == NULL)
        {
                return;
        }

        bool hasLeft = (g_root->left != NULL);
        bool hasRight = (g_root->right != NULL);

        if (!hasLeft && !hasRight)
        {
          return;
        }

        cout << prefix;
        cout << ((hasLeft  && hasRight) ? "├── " : "");
        cout << ((!hasLeft && hasRight) ? "└── " : "");

        if (hasRight)
        {
          bool printStrand = (hasLeft && hasRight &&
                                 (g_root->right->right != NULL || g_root->right->left != NULL));
          string newPrefix = prefix + (printStrand ? "│   " : "    ");
          cout << g_root->right->token.tokName << endl;
          printSubtree(g_root->right, newPrefix);
        }

        if (hasLeft)
        {
          cout << (hasRight ? prefix : "") << "└── " << g_root->left->token.tokName << endl;
          printSubtree(g_root->left, prefix + "    ");
        }
}

void printTree(Node* g_root)
{
        if (g_root == NULL)
        {
          return;
        }

        cout << g_root->token.tokName << endl;
        printSubtree(g_root, "");
        cout << endl;
}

// ％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％％

int main()
{
  Node *expr = NULL ;
  g_root = expr ;

  int currentIndex = 0;
  char garbageLineEnter ;
  int indent = 0 ;
  scanf( "%d", &gTestNum ) ;
  scanf( "%c", &garbageLineEnter ) ;
  AllCleanUp();
  // if ( gTestNum == 2 ) { // test
  //   cout << " gTestNum : " << gTestNum  << endl ;
  //   // exit( 0 ) ;
  // } // if

  cout << "Welcome to OurScheme!" << endl ;
  bool ex = false;
  
  while ( ex == false && g_isEOF == false )
  { 
    if ( ReadSExp() )
    { 
      // DumpWholeList() ; // test // no error
      
      BuildTokenTree( currentIndex, g_tokenList, expr ) ; // build a tree for pretty print
      // exit( 0 ) ; // test
      
      
      // 這裡要設定ex
      if ( g_root != NULL && g_root->left != NULL && g_root->left->token.tokName.compare( "exit" ) == 0 &&
           g_root->right == NULL && g_hasOuterLefPan )
      { // exit
        ex = true ;
        cout << endl << "> " << endl ;
      } // if

      printTree( g_root ); // test
      // exit( 0 ) ; // test
      if ( ex == false && g_isEOF == false )
      {
        if ( g_root != NULL && g_root->left != NULL && g_root->left->token.isComment == false ) {
          cout << endl << "> ";
        } // if
        
        PrintSExp( g_root, indent ) ; // You must "pretty print" this data structure.
      } // if

    }   // if
    else
    {
      if ( g_error_message.compare( "" ) == 0 ) {
        char s11[64] ;
        char s22[64] ;
        sprintf( s11, "%d", g_Line ) ;
        sprintf( s22, "%d", g_Col ) ;
        string s1( s11 ) ;
        string s2( s22 ) ;
        g_error_message = "ERROR (unexpected token) : atom or '(' expected when token at Line " + s1 +
                          " Column " + s2 + " is >>" + g_nextToken.tokName + "<<" ;
      } // if
      
      cout << endl << "> " << g_error_message << endl ;
      while ( GetNextChar() && g_nextChar != '\n' && g_isEOF == false )
      { // ignore the rest of the error line
      
      } // while
      
      g_error_message = "" ;
      g_nextChar = '\0' ;
    } // else

    g_Col = 0 ;
    if ( g_hasCommentBe4 == true ) {
      // dont reset line count
      g_hasCommentBe4 = false ;
    } // if
    else {
      g_Line = 1 ;
    } // else
    
    currentIndex = 0 ;
    AllCleanUp() ;
    expr = NULL ;
    g_root = expr ;
    indent = 0 ;
  } // while

  cout << "Thanks for using OurScheme!" << endl;
  return 0;
} // main()

