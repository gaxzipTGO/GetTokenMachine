# include <iostream>
# include <string>
# include <cstdio>
# include <stdexcept>
# include <stack>
# include <queue>
# include <vector>
# include <stdio.h>
# include <string.h>

//    // ***************************************************************************** //
//    //                                                                               //
//    //               !!!!!!!!!!!!!!!!!!警急任務!!!!!!!!!!!!!!!!!!!!!                  //
//    //                          WARRING WARRING WARRING                              //
//    //                                                                               //
//    //                            找出runtimeError!!!                                //
//    //                               卡隱藏囉卡隱藏                                   //
//    //                                                                               //
//    //                                                                               //
//    //                                                                               //
//    //                                                                               //
//    // ***************************************************************************** //
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                                        ///////
//                         /////////////////////////////////////               
                                    
using namespace std ;

enum G_Category {
  LEFT_PAREN = 2344,
  RIGHT_PAREN = 2346,
  INT = 2134,
  STRING = 3245,
  DOT = 2345,
  FLOAT = 5467,
  NIL = 1234,
  T = 5432,
  QUOTE = 6543,
  SYMBOL = 3228
};

class Token {
  public : string m_token_string ;
  public : int m_colnum ;
  public : int m_line ;


  public : Token() {
    m_token_string = "" ;
    m_colnum = 0 ;
    m_line = 0 ;
  } // Token()

  public : Token( string str, int colnum, int line ) {

    m_token_string = str ;
    m_colnum = colnum ;
    m_line = line ;

  } // Token()
} 
;

string To_String( int num ) {

  char buffer[16];
  sprintf( buffer, "%d", num ) ;
  string strNumber( buffer ) ;
  return strNumber ;

} // To_String()

bool HaveWord( char ch ) {
  if ( ( ch >= 65 && ch <= 90 ) || ( ch >= 97 && ch <= 122 ) ) {
    return true ;
  } // if
  
  return false ;
} // HaveWord()



class TokenClassCategory {
  
  protected: virtual bool IsInt( string token ) {
    if ( token.size() == 0 ) {
      return false ;
    } // if 

    for ( int i = 0 ; i < token.size() ; i ++ ) {
      if ( token.at( i ) < '0' || token.at( i ) > '9' ) {
        return false ; 
      } // if
    } // for

    return true ;
  } // IsInt() 

  protected: virtual bool IsInt( string token, bool noSize ) {

    for ( int i = 0 ; i < token.size() ; i ++ ) {
      if ( token.at( i ) < '0' || token.at( i ) > '9' ) {
        return false ; 
      } // if
    } // for

    return true ;
  } // IsInt()   

  protected: virtual bool IsIntMissOperate( string token, bool noSize ) {
  /*
    確認給進來的這一項是不是數字 可以無視一次+ or - 是的話回傳一個 true
  */
    bool miss = true ;
    for ( int i = 0 ; i < token.size() ; i ++ ) {
      if ( ( token.at( i ) < '0' || token.at( i ) > '9' ) ) {
        if ( miss == false ) {
          return false ;
        } // if

        if ( token.at( i ) == '+' || token.at( i ) == '-' ) {
          miss = false ;
        } // if
        else {
          return false ;
        } // else
       
      } // if
    } // for

    return true ;

  } // IsIntMissOperate()

  protected: virtual bool IsIntMissOperate( string token ) {
  /*
    確認給進來的這一項是不是數字 可以無視一次+ or - 是的話回傳一個 true
  */
    if ( token.size() == 0 ) {
      return false ;
    } // if

    bool miss = true ;
    for ( int i = 0 ; i < token.size() ; i ++ ) {
      if ( ( token.at( i ) < '0' || token.at( i ) > '9' ) ) {
        if ( miss == false ) {
          return false ;
        } // if

        if ( token.at( i ) == '+' || token.at( i ) == '-' ) {
          miss = false ;
        } // if
        else {
          return false ;
        } // else
       
      } // if
    } // for

    return true ;

  } // IsIntMissOperate()

  protected: virtual bool IsFloat( string token ) {
    size_t find = token.find( '.' ) ;
    if ( token.npos != find ) {
      string sub_string_head = token.substr( 0, find ) ;
      string sub_string_tail = token.substr( find+1, token.size()-find ) ;
      if ( IsIntMissOperate( sub_string_head, false ) &&
           IsInt( sub_string_tail, false ) ) {
        return true ;
      } // if
    } // if
    else {
      return false ;
    } // else

    return false ;

  } // IsFloat()

  protected: virtual string AddZero_Back( string token ) {
    
    if ( IsInt( token ) || token == "" ) {
      for ( int i = token.size() ; i < 3 ; i ++ ) { 
        token = token + string( 1, '0' ) ; 
      } // for
    } // if

    return token ;

  } // AddZero_Back()

  protected: virtual string RoundingUP( string token ) {
    if ( token.at( 3 ) > '4' ) {
      token.at( 2 ) = token.at( 2 ) + 1 ;
    } // if

    return token.substr( 0, 3 ) ;
  } // RoundingUP()

  protected: virtual string DealWithOperater( string token ) {
  /*
    看第一項是不是加號 如果是的話並且後面是數字的話把他給清除
  */
    if ( token.at( 0 ) == '+' ) {
      if ( token.at( 1 ) >= '0' && token.at( 1 ) <= '9' ) {
        token.erase( 0, 1 ) ;
      } // if
    } // if

    return token ;
  } // DealWithOperater()

  protected: virtual string DealWithDot_Back( string token ) {
    bool int_is = true ;
    for ( int i = 0 ; i < token.size() && int_is == true ; i ++ ) {
      if ( token.at( i ) == '.' ) {
        string sub_token_head = token.substr( 0, i ) ;
        string sub_token_tail = token.substr( i+1, token.size() - i ) ;
        if ( ! IsIntMissOperate( sub_token_head ) ) {
          return token ;
        } // if

        if ( sub_token_tail.size() == 3 ) {
          return token ;
        } // if 
        else if ( sub_token_tail.size() < 3 ) {
          if ( sub_token_head.at( sub_token_head.size()-1 ) >= '0' && 
               sub_token_head.at( sub_token_head.size()-1 ) <= '9' ) {
            return sub_token_head + '.' + AddZero_Back( sub_token_tail ) ;
          } // if
        } // else if 要做補數
        else if ( sub_token_tail.size() > 3 ) {
          return sub_token_head + '.' + RoundingUP( sub_token_tail ) ;
        } // else if 做四捨五入
      } // if
      else if ( HaveWord( token.at( i ) ) ) {
        int_is = false ;
      } // if 
    } // for

    return token ;
  } // DealWithDot_Back()

  protected: virtual string AddZero_Front( string token ) {
  /*
    確認給進來的這一項是不是空的 是的話就可以在最尾巴加上 '0' (可以無視一次+ or - )
  */
    if ( token.size() == 0 ) {
      return "0" ;
    } // if

    if ( token.size() == 1 ) {
      if ( token.at( 0 ) == '+' || token.at( 0 ) == '-' ) {
        return token + string( 1, '0' ) ;
      } // if
    } // if 

    return token ;
  } // AddZero_Front()

  protected: virtual string DealWithDot_Front( string token ) {
    for ( int i = 0 ; i < token.size()  ; i ++ ) {
      if ( token.at( i ) == '.' ) {
        string sub_token_head = token.substr( 0, i ) ;
        string sub_token_tail = token.substr( i+1, token.size() - i ) ;
        if ( IsInt( sub_token_tail ) ) {
          return AddZero_Front( sub_token_head ) + string( 1, '.' ) + sub_token_tail ;
        } // if
        else {
          return token ;
        } // else
      } // if
    } // for  

    return token ;
  } // DealWithDot_Front()

  public: virtual string ChangeToken( string token ) {
    if ( token.compare( "t" ) == 0 ) {
      return "#t" ;
    } // if
    else if ( token.compare( "#f" ) == 0 ) {
      return "nil" ;
    } // else if 
    else if ( GetThisTokenType( token ) == STRING || GetThisTokenType( token ) == SYMBOL ||
              GetThisTokenType( token ) == LEFT_PAREN || GetThisTokenType( token ) == LEFT_PAREN ||
              GetThisTokenType( token ) == DOT ) {
      return token ;
    } // else if
    else if ( token.compare( "\'" ) == 0 ) {
      return "quote" ;
    } // else if

    token = DealWithDot_Front( token ) ;  // 在小數點前補0
    token = DealWithDot_Back( token ) ;  // 在小數點後補0 四捨五入 有字母不會做改變
    token = DealWithOperater( token ) ;  // 如果是+就把+刪掉 其餘不動
    return token ;
  } // ChangeToken()

  protected: virtual bool CheckSymbol( string token ) {
    if ( token.npos == token.find( "\"" ) && token.npos == token.find( ";" ) 
         && token.npos == token.find( " " ) && token.npos == token.find( "\t" ) 
         && token.npos == token.find( "\n" ) ) {
      return true ; 
    } // if
    else
      return false;
  } // CheckSymbol()
  
  public : virtual int GetThisTokenType( string token ) {
    if ( token == "(" ) {
      return LEFT_PAREN ;
    } // if
    else if ( token == ")" ) {
      return RIGHT_PAREN ;
    } // else if 
    else if ( token == "t" || token == "#t" ) {
      return T ;
    } // else if
    else if ( token == "nil" || token == "#f" ) {
      return NIL ;
    } // else if
    else if ( token == "." ) {
      return DOT ;
    } // else if
    else if ( token == "\'" ) {
      return QUOTE ;
    } // else if
    else if ( IsFloat( token ) ) {
      return FLOAT ;
    } // else if
    else if ( IsIntMissOperate( token ) ) { // 只有純數字的int ex: 123 456
      return INT ;
    } // else if 
    else if ( token.at( 0 ) == '+' || token.at( 0 ) == '-' ) { // +123 -456
      if ( IsIntMissOperate( token.substr( 1, token.size() ) ) ) { 
        return INT ;
      } // if
      else return STRING ;
    } // else if
    else if ( CheckSymbol( token ) ) {
      return SYMBOL ;
    } // else if
    else {
      return STRING ;
    } // else
  } // GetThisTokenType() 
   
} 
g_classCategory ;

class GetTokenMachine {
  protected: bool m_isFile ;
  protected: Token m_token ;
  protected: bool m_notend ;
  protected: char m_nextChar ; // used to Check
  protected: bool m_bufferDelimiter  ; // used to check m_nextChar is delimiter
  protected: string m_bufferToken ;
  protected: int m_line ;
  protected: int m_column ;
  protected: int m_lastColumn ;
  protected: bool m_reload_line ;
  protected: virtual bool GetChar( char &ch ) ; 
  protected: virtual bool GetChar( char &ch, bool skipEOF ) ; 
  protected: bool GetToken( Token & token ) ;
  public: bool GetNextToken( Token &Out_token ) ;

  public: GetTokenMachine() {
    try {
      m_isFile = false ;
      m_token.m_token_string = "" ;
      m_notend = true ;
      m_nextChar = '\0' ;
      m_bufferDelimiter = '\0' ;
      m_bufferToken = "" ;
      m_line = 1 ;
      m_column = 0 ;
      m_lastColumn = 0 ;
      m_reload_line = false ;
    } // try
    catch( exception &e ) {
      // cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
      throw invalid_argument( e.what() )  ;            
    } // catch
    
  } // GetTokenMachine()

  protected: bool IsDelimiter( char ch ) {
    if ( ch == '\"' || ch == '\'' || ch == '.' ||
         ch == '(' || ch == ')' || ch == ';' ) {
      
      return true ;
    } // if
    
    return false ;
  } // IsDelimiter()

  protected: void ErrorMessage( string type, int line, int column ) {
    while ( m_nextChar != '\n' && m_nextChar != EOF ) {
      cin.get( m_nextChar ) ;
    } // while

    m_line = 1 ;
    m_column = 0 ;
    m_reload_line = false ;
    cout << "ERROR (" + type + ") : END-OF-LINE encountered at Line "+ To_String( line ) + 
            " Column " + To_String( column ) << endl ;
    throw invalid_argument( "String error" ) ;
  } // ErrorMessage()

  protected: void ErrorMessage( string type ) {
    cout << "ERROR (" + type + ") : END-OF-FILE encountered" << endl ;
    throw invalid_argument( "EOF error" ) ;
  } // ErrorMessage()

  protected: virtual string DelimiterDeal( string token ) {
    token = token + string( 1, m_bufferDelimiter ) ;
    m_bufferDelimiter = '\0' ;
    return token ;
  } // DelimiterDeal()
  
  protected: virtual string SaveDelimiterToBuffer() {
    m_bufferDelimiter = m_nextChar ;
    return "" ; 
  } // SaveDelimiterToBuffer()

  public: virtual string ReadWholeLine() {
    while ( m_nextChar != '\n' && m_nextChar != EOF ) {
      GetChar( m_nextChar, true );
    }  // while

    return "" ;
  } // ReadWholeLine()

  protected: virtual void UpdateToken( string &token ) {
  /*
    重點 : 有特殊意義的存功能 沒有的就照存
  */
    if ( m_nextChar == 'n' ) {
      token = token + string( 1, '\n' ) ;
    } // if
    else if ( m_nextChar == 't' ) {
      token = token + string( 1, '\t' ) ;
    } // if
    else if ( m_nextChar == '0' ) {
      token = token + string( 1, '\0' ) ;
    } // if
    else if ( m_nextChar == '\\' ) {
      token = token + string( 1, '\\' ) ;
    } // if
    else if ( m_nextChar == '\'' ) {
      token = token + string( 1, '\'' ) ;
    } // if
    else if ( m_nextChar == '\"' ) {
      token = token + string( 1, '\"' ) ;
    } // if
    else {
      token = token + string( 1, '\\' ) ;
      token = token + string( 1, m_nextChar ) ;
    } // else
  } // UpdateToken()

  protected: virtual string ReadString() {
    string token = "" ;
    bool not_end ;
    do { 
      bool continue_choise  = false ;
      if ( m_nextChar == '\\' ) {
        not_end = GetChar( m_nextChar );  // 讀一個char (false代表後面沒東西了)
        UpdateToken( token ) ; // 有特殊意義的存功能 沒有的就照存
        not_end = GetChar( m_nextChar );  
        continue_choise = true ;
      } // if 

      if ( ! continue_choise ) { // continue_choise == false才會進這if
        token = token + string( 1, m_nextChar ) ;
        not_end = GetChar( m_nextChar );
      } // if

    } while ( m_nextChar != '\"' && m_nextChar != '\n' && not_end ) ; // do-while()
    // 讀到 換行 或 一個string結束 或 EOF
    

    if ( m_nextChar == '\n' || ! not_end ) {
      if ( m_nextChar == '\n' )
        ErrorMessage( "no closing quote", m_line-1, m_lastColumn ) ;
      ErrorMessage( "no closing quote", m_line, m_lastColumn ) ;
    } // if

    token = token + string( 1, m_nextChar ) ;
    return token ;
  } // ReadString()

  protected: virtual string ReadDot( string sub_token_front ) {

    string sub_token_back = "" ;
    int asset_line = 0 ;
    if ( m_reload_line ) {
      asset_line = m_reload_line ;
    } // if 這個代表 雖然已經有東西印出來了 但實際上他還在原本這一行 因此不能被reload影響

    while ( GetChar( m_nextChar ) && ! IsDelimiter( m_nextChar ) && m_nextChar != '\n' &&
            m_nextChar != '\t' && m_nextChar != ' '  ) {
      sub_token_back = sub_token_back + string( 1, m_nextChar ) ;
    } // while  把.後面的數字讀完

    if ( m_nextChar == '.' ) {
      sub_token_back = ReadDot( sub_token_back ) ;
    } // if

    return sub_token_front + string( 1, '.' ) + sub_token_back ;

  } // ReadDot()

  protected: virtual string ReadPAREN( char ch ) {
    return string( 1, ch )  ;
  } // ReadPAREN()

  protected: virtual string DealDelimiter( string token, char ch ) {

    if ( ch == ';' ) {
      while ( m_nextChar != EOF && m_nextChar != '\n' ) {
        cin.get( m_nextChar ) ;
        m_column += 1 ;
        if ( m_nextChar == '\n' ) {
          m_line += 1 ;
          m_column = 0 ;
        } // if
      } // while

      if ( m_nextChar == EOF ) {
        ErrorMessage( "no more input" ) ;
      } // if

      if ( m_reload_line ) {
        m_column = 0 ;
        m_line = 1 ;
        m_reload_line = false ;
      } // if

      return "" ;
    } // if
    else if ( ch == '\"' ) {
      string str = ReadString() ;
      GetChar( m_nextChar ) ;
      return str ;
    } // else if
    else if ( ch == '.' ) {
      return ReadDot( token ) ; // 把.後面的數字讀完
    } // else if
    else if ( ch == '(' || ch == ')' ) {
      GetChar( m_nextChar, true ) ;
      return ReadPAREN( ch ) ; // return 這個括號回去
    } // else if

    GetChar( m_nextChar ) ;
    return string( 1, ch ) ;
  } // DealDelimiter()
  
  public: char ReturnNextChar() {
    return m_nextChar ;
  } // ReturnNextChar()
  
  public: bool IsEnterChar() {
    if ( m_nextChar == '\n' ) { 
      return true ;
    } // if 
    
    else return false ;
    
  } // IsEnterChar()
  
  public: void ReloadError() {

    while ( m_nextChar != '\n' && m_nextChar != EOF ) {

      cin.get( m_nextChar ) ;

    } // while 

    m_column = 0 ;
    m_line = 1 ;
    m_reload_line = false ;

  } // Reload() 

  public: void ReadReload() {

    m_reload_line = true ;
    m_line = 1 ;
    m_column = 1 ;
    
  } // ReadReload()

} 
g_getTokenMachine ;

bool GetTokenMachine :: GetToken( Token & token ) {
  try {
    if ( m_nextChar == EOF ) {
      ErrorMessage( "no more input" ) ;
      return false ;
    } // if

    if ( IsDelimiter( m_nextChar ) ) { // 是Delimiter的話就去處理他
      token.m_colnum = m_column ;
      token.m_line = m_line ;
      token.m_token_string = DealDelimiter( token.m_token_string, m_nextChar ) ;
      return true ;
    } // if

    if ( m_nextChar != EOF && ! IsDelimiter( m_nextChar ) && 
         m_nextChar != '\n' && m_nextChar != '\t' && 
         m_nextChar != ' ' && m_nextChar != '\0' ) { // 非Delimeter 就是純字元
      token.m_colnum = m_column ;
      token.m_line = m_line ;
      token.m_token_string = token.m_token_string + string( 1, m_nextChar ) ;
    } // if

    bool get_info = false ;
    while ( m_nextChar != EOF && GetChar( m_nextChar ) && ! IsDelimiter( m_nextChar ) && 
            m_nextChar != '\n' && m_nextChar != '\t' && m_nextChar != ' '  ) {
      if ( ! get_info ) {
        token.m_colnum = m_column ;
        token.m_line = m_line ;
        get_info = true ;        
      } // if 

      token.m_token_string = token.m_token_string + string( 1, m_nextChar ) ;
    } // while 把純字元的部分全部讀完

    if ( m_nextChar == '.' ) {
      if ( ! get_info ) {
        token.m_colnum = m_column ;
        token.m_line = m_line ;        
      } // if 

      token.m_token_string = ReadDot( token.m_token_string ) ;
      return true ;
    } // if

    if ( token.m_token_string.size() != 0 ) {
      return true ;
    } // if

    if ( m_nextChar == EOF ) {
      return false ;
    } // if 

    return true ;
  } // try 
  catch( exception &e ) {
    throw invalid_argument( e.what() ) ;
  } // catch
} // GetTokenMachine::GetToken()

bool GetTokenMachine :: GetNextToken( Token &Out_token ) {
  /*
  結論:可以得到一個token,但要自行決定return哪個
  the function is we can get the token but we need to chiose Out_token != "" 
  */
  try {
    m_token.m_token_string = "" ;
    if ( m_notend ) {
      do {
        m_notend = GetToken( m_token ) ; // 得到一個token ( false表示 eof 或 讀完才eof )
        if ( m_notend == false ) {
          return false ;
        } // if        
      } while ( m_token.m_token_string.length() == 0 && m_token.m_token_string == "" ) ;
      Out_token = m_token ;
      return true ;
      
    } // if
    
    return false ;        
    
  } // try
  catch ( exception &e ) {
    throw invalid_argument( e.what() ) ;  
  } // catch
  
} // GetTokenMachine::GetNextToken()

bool GetTokenMachine::GetChar( char &ch ) {
  /*
  讀一個char 讀到EOF ( false代表後面沒東西了 )
  */
  if ( ch == EOF ) {
    ErrorMessage( "no more input" ) ;
    return false ;
  } // if

  if ( cin.get( ch ) ) {
    m_column += 1 ;
    if ( ch == '\n' ) {
      m_line += 1 ;
      m_lastColumn = m_column ;
      m_column = 0 ;
      if ( m_reload_line == true ) {
        m_line = 1 ;
        m_reload_line = false ;
      } // if
    } // if

    if ( ch != ' ' && ch != '\t' && ch != '\n' ) {
      m_reload_line = false ;
    } // if

    return true ;
  } // if
  else {
    ch = EOF ;
    ErrorMessage( "no more input" ) ;
    return false ;
  } // else

} // GetTokenMachine::GetChar()

bool GetTokenMachine::GetChar( char &ch, bool skipEOF ) {
  /*
  讀一個char 讀到EOF ( false代表後面沒東西了 )
  */
  if ( ch == EOF ) {
    ErrorMessage( "no more input" ) ;
    return false ;
  } // if

  if ( cin.get( ch ) ) {
    m_column += 1 ;
    if ( ch == '\n' ) {
      m_line += 1 ;
      m_lastColumn = m_column ;
      m_column = 0 ;
      if ( m_reload_line == true ) {
        m_line = 1 ;
        m_reload_line = false ;
      } // if
    } // if

    if ( ch != ' ' && ch != '\t' && ch != '\n' ) {
      m_reload_line = false ;
    } // if

    return true ;
  } // if
  else {
    ch = EOF ;
    if ( ! skipEOF )
      ErrorMessage( "no more input" ) ;
    return false ;
  } // else

} // GetTokenMachine::GetChar()

class Statement {

  protected : GetTokenMachine m_pl_tokenGetter ;
  protected : TokenClassCategory m_tokenCategorier ;
  protected : bool m_not_end ;
  protected : string m_nextToken ;



  protected : Token GetToken() ;
  protected : Token GetNextToken( stack<Token> &token_wait_stack ) ;
  protected : void ErrorMessage( string type, int line, int column, string token ) ;
  protected : void CheckTheS_EXP_WHILE( stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) ; 
  protected : bool CheckTheDOT_AND_S_EXP( stack<Token>  &token_wait_stack,
                                          vector<Token> &token_wait_vector ) ; 
  protected : bool IsATOM( Token token, stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) ;
  protected : bool IsS_EXP( Token token, stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) ;
  protected : void GetStatement() ;
  protected : void PopStackToLast( stack<Token> &token_stack ) ;
  protected : void PopvectorToLast( vector<Token> &token_queue ) ;
  protected : bool IsDOTANDPAREN( vector<Token> &token_wait_vector, int level ) ;
  protected : void PrintWhiteSpaceWithLevel( int level ) ;
  protected : void PrintTotalTokenNoPAREN( vector<Token> &token_wait_vector, int level, bool &new_line ) ;
  protected : void PrintTotalTokenInPAREN( vector<Token> &token_wait_vector, int level, bool &new_line ) ;
  protected : void PrintTotalTokenAtvector( vector<Token> &token_wait_vector, int level, bool &new_line ) ;
  protected : void PrintTotalTokenAtvectorFirst( vector<Token> &token_wait_vector, 
                                                 int level, bool new_line ) ;
  protected : void PrintFunction( vector<Token> &token_function_vector ) ;
  public : void PrintAllOfStatement() ;
  public : Statement( GetTokenMachine token_get, TokenClassCategory token_category ) {


    m_not_end = true ;
    m_pl_tokenGetter = token_get ;
    m_tokenCategorier = token_category ;
    m_nextToken = "" ;

  } // Statement()

} 
; // Statemant

Token Statement :: GetToken() {
  
  Token token ;
  m_not_end = m_pl_tokenGetter.GetNextToken( token ) ; // 得到一個token ( false表示eof token是空的 )
  return token ;

} // Statement::GetToken()

void Statement :: ErrorMessage( string type, int line, int column, string token ) {
  cout << "ERROR (unexpected token) : " + type + " expected when token at Line "
          + To_String( line ) + " Column " + To_String( column ) + " is >>" + token + "<<" << endl ;
  
  m_pl_tokenGetter.ReloadError() ;
  throw invalid_argument( "Token Error" ) ;
} // Statement::ErrorMessage()



Token Statement :: GetNextToken( stack<Token> &token_wait_stack ) {
/*
  結論 : 得到一個token,無論是從哪裡拿的
  從輸入的文件或stack裡面拿一個token出來 ( 如果stack有東西就先用stack的 這樣才不會打架 )
*/
  if ( ! token_wait_stack.empty() ) {
    Token token = token_wait_stack.top() ;
    token_wait_stack.pop() ;
    return token ;
  } // if
  else {
    return GetToken() ; // 如果eof 會是空的
  } // else

} // Statement::GetNextToken()

bool Statement :: CheckTheDOT_AND_S_EXP( stack<Token>  &token_wait_stack,
                                         vector<Token> &token_wait_vector ) {
  Token token = GetNextToken( token_wait_stack ) ;
  token_wait_vector.push_back( token ) ;
  if ( m_tokenCategorier.GetThisTokenType( token.m_token_string ) == DOT ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
      return true ;
    } // if
    else {
      ErrorMessage( "atom or '('", temp_token.m_line, temp_token.m_colnum, temp_token.m_token_string ) ;
    } // else
  } // if

  token_wait_vector.pop_back() ;
  token_wait_stack.push( token ) ;
  return false ;
} // Statement::CheckTheDOT_AND_S_EXP()

void Statement :: CheckTheS_EXP_WHILE( stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) {

  Token token = GetNextToken( token_wait_stack ) ;
  while ( IsS_EXP( token, token_wait_stack, token_wait_vector ) ) {
    token = GetNextToken( token_wait_stack ) ;
  } // while



} // Statement::CheckTheS_EXP_WHILE()

bool Statement :: IsATOM( Token token, stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) {

  int type = m_tokenCategorier.GetThisTokenType( token.m_token_string ) ;
  if ( type == SYMBOL ||
       type == INT ||
       type == FLOAT ||
       type == STRING ||
       type == NIL ||
       type == T ) {
    return true ;
  } // if
  else if ( type == LEFT_PAREN ) {
    Token nextToken ;
    nextToken = GetNextToken( token_wait_stack ) ;
    if ( m_tokenCategorier.GetThisTokenType( nextToken.m_token_string ) == RIGHT_PAREN ) {
      token_wait_vector.push_back( nextToken ) ;
      return true ;
    } // if
    else {
      token_wait_stack.push( nextToken ) ;
      return false ;
    } // else 
  } // else if 

  return false ;

} // Statement::IsATOM()

void Statement :: PrintWhiteSpaceWithLevel( int level ) {

  for ( int i = 0 ; i < level*2 ; i ++ ) {
    cout << ' ' ;
  } // for

} // Statement::PrintWhiteSpaceWithLevel()

void Statement :: PopStackToLast( stack<Token> &token_stack ) {

  for ( ; token_stack.size() > 1 ; ) {
    token_stack.pop();
  } // for

} // Statement::PopStackToLast()

void Statement :: PopvectorToLast( vector<Token> &token_vector ) {

  for ( ; token_vector.size() > 0 ; ) {
    token_vector.pop_back();
  } // for

} // Statement::PopvectorToLast()

bool Statement :: IsS_EXP( Token token, stack<Token> &token_wait_stack, vector<Token> &token_wait_vector ) {
  Token tempToken = Token( token.m_token_string, token.m_colnum, token.m_line ) ;
  token_wait_vector.push_back( tempToken ) ; 
  // 這裡出現SafeCode的問題 我不知道為甚麼會發生 他不該發生這件事情
  // 你有印象老大可能會有那些地方會有safeCode的嗎
     
  if ( IsATOM( token, token_wait_stack, token_wait_vector ) ) {
    return true ;
  } // if
  else if ( m_tokenCategorier.GetThisTokenType( token.m_token_string ) == LEFT_PAREN ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
      // 從這裡進去之後才會出問題 我不確定跟這個有沒有關係
      // 我用recursive 第二次進入這個function 然後對我class內的vector直接做push
      // 我不知道怎麼解釋這部分 一開始以為是怕記憶體溢出所以我又另外在class做一個tempToken
      // 結果一樣 
      // by the way 測資1除了這問題以外都對了 
      CheckTheS_EXP_WHILE( token_wait_stack, token_wait_vector ) ; 
      CheckTheDOT_AND_S_EXP( token_wait_stack, token_wait_vector ) ;
      temp_token = GetNextToken( token_wait_stack ) ;
      if ( m_tokenCategorier.GetThisTokenType( temp_token.m_token_string ) == RIGHT_PAREN ) {
        token_wait_vector.push_back( temp_token ) ;
        return true ;
      } // if 
      else {
        ErrorMessage( "')'", temp_token.m_line, temp_token.m_colnum, temp_token.m_token_string ) ;
        token_wait_stack.push( temp_token ) ;
      } // else
    } // if
    else {
      token_wait_stack.push( temp_token ) ;
    } // else
  } // else if 
  else if ( m_tokenCategorier.GetThisTokenType( token.m_token_string ) == QUOTE ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    vector<Token>::iterator index = token_wait_vector.end() -1 ; // 這是quote所在位置
    Token putToken = Token( "(", 9999, 9999 ) ;
    token_wait_vector.insert( index, 1, putToken ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
      index = token_wait_vector.end() ;
      Token putToken2 = Token( ")", 9999, 9999 ) ;
      token_wait_vector.insert( index, 1, putToken2 ) ;
      return true ;
    } // if
    else {
      token_wait_vector.erase( index ) ;
      return false ;
    } // else

  } // else if  

  token_wait_vector.pop_back() ;
  token_wait_stack.push( token ) ;    
  return false ;

} // Statement::IsS_EXP()

bool Statement :: IsDOTANDPAREN( vector<Token> &token_wait_vector, int level ) {
  if ( token_wait_vector.size() ) {
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) == DOT ) {
      if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[1].m_token_string ) == 
           LEFT_PAREN ) {
        token_wait_vector.erase( token_wait_vector.begin() ) ;
        bool new_line = true ;
        PrintTotalTokenNoPAREN( token_wait_vector, level, new_line ) ;
      } // if
      else if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[1].m_token_string ) == 
                NIL ) {  
        token_wait_vector.erase( token_wait_vector.begin() ) ;
        token_wait_vector.erase( token_wait_vector.begin() ) ;
        return true ;
      } // else if
      else if ( m_tokenCategorier.ChangeToken( token_wait_vector[1].m_token_string ) == 
                "nil" ) {  
        token_wait_vector.erase( token_wait_vector.begin() ) ;
        token_wait_vector.erase( token_wait_vector.begin() ) ;
        return true ;
      } // else if  
    } // if
  } // if

  return false ;

} // Statement::IsDOTANDPAREN()

void Statement :: PrintTotalTokenNoPAREN( vector<Token> &token_wait_vector, int level, bool &new_line ) {


  if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[1].m_token_string ) == RIGHT_PAREN ) {
    token_wait_vector.erase( token_wait_vector.begin() ) ;
    token_wait_vector.erase( token_wait_vector.begin() ) ;
  } // if
  else if ( token_wait_vector[1].m_token_string == "exit" && level == 1 ) {
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[2].m_token_string ) == 
         RIGHT_PAREN ) {
      cout << endl ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      m_not_end = false ;
    } // if
  } // else if
  else {
    token_wait_vector.erase( token_wait_vector.begin() ) ;
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) ==
         LEFT_PAREN ) {
      PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
    } // if

    for ( ; m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) != 
          RIGHT_PAREN ; ) {
      if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) ==
           LEFT_PAREN  ) {
        if ( new_line ) {
          PrintWhiteSpaceWithLevel( level ) ;
          new_line = false ;
        } // if

        PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
      } // if
      else {
        if ( ! IsDOTANDPAREN( token_wait_vector, level ) )
          PrintTotalTokenAtvector( token_wait_vector, level, new_line ) ;
      } // else

    } // for

    token_wait_vector.erase( token_wait_vector.begin() ) ;
  } // else 

} // Statement::PrintTotalTokenNoPAREN()

void Statement :: PrintTotalTokenInPAREN( vector<Token> &token_wait_vector, int level, bool &new_line ) {


  if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[1].m_token_string ) == RIGHT_PAREN ) {
    if ( new_line ) {
      PrintWhiteSpaceWithLevel( level-1 ) ;
      new_line = false ;
    } // if

    cout << "nil" << endl ;
    new_line = true ;
    token_wait_vector.erase( token_wait_vector.begin() ) ;
    token_wait_vector.erase( token_wait_vector.begin() ) ;
  } // if
  else if ( token_wait_vector[1].m_token_string == "exit" && level == 1 ) {
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector[2].m_token_string ) == 
         RIGHT_PAREN ) {
      m_not_end = false ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      token_wait_vector.erase( token_wait_vector.begin() ) ;
      cout << endl ;
    } // if
  } // else if

  else {
    if ( new_line ) {
      PrintWhiteSpaceWithLevel( level ) ;
    } // if

    cout << "( " ;
    new_line = false ;
    token_wait_vector.erase( token_wait_vector.begin() ) ;
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) ==
         LEFT_PAREN ) {
      PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
    } // if

    for ( ; m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) != 
          RIGHT_PAREN ; ) {
      if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) ==
           LEFT_PAREN  ) {
        if ( new_line ) {
          PrintWhiteSpaceWithLevel( level ) ;
          new_line = false ;
        } // if

        PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
      } // if
      else {
        if ( ! IsDOTANDPAREN( token_wait_vector, level ) )
          PrintTotalTokenAtvector( token_wait_vector, level, new_line ) ;
      } // else
    } // for

    PrintWhiteSpaceWithLevel( level-1 ) ;
    cout << ")" << endl ;
    new_line = true ;
    token_wait_vector.erase( token_wait_vector.begin() ) ;
  } // else
} // Statement::PrintTotalTokenInPAREN()

void Statement :: PrintTotalTokenAtvector( vector<Token> &token_wait_vector, int level, bool &new_line ) {

  IsDOTANDPAREN( token_wait_vector, level ) ;
  if ( token_wait_vector.size() != 0 ) {
    if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) == RIGHT_PAREN ) {
    } // if
    else {
      if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) == LEFT_PAREN ) {
        PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
      } // if
      else {
        if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) != 
             RIGHT_PAREN ) {
          if ( new_line ) {
            PrintWhiteSpaceWithLevel( level ) ;
          } // if

          cout << m_tokenCategorier.ChangeToken( token_wait_vector.front().m_token_string ) << endl ;
          new_line = true ;
          token_wait_vector.erase( token_wait_vector.begin() ) ; 
        } // if
        else {
          token_wait_vector.erase( token_wait_vector.begin() ) ;
        } // else
      } // else
    } // else
  } // if

} // Statement::PrintTotalTokenAtvector()  

void Statement::PrintTotalTokenAtvectorFirst( vector<Token> &token_wait_vector, int level, bool new_line ) {
  for ( ; token_wait_vector.size() != 0 ; ) {
    IsDOTANDPAREN( token_wait_vector, level ) ;
    if ( token_wait_vector.size() != 0 ) {
      if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) == RIGHT_PAREN ) {
      } // if
      else {
        if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) == LEFT_PAREN ) {
          PrintTotalTokenInPAREN( token_wait_vector, level+1, new_line ) ;
        } // if
        else {
          if ( m_tokenCategorier.GetThisTokenType( token_wait_vector.front().m_token_string ) != 
               RIGHT_PAREN ) {
            if ( new_line ) {
              PrintWhiteSpaceWithLevel( level ) ;
            } // if

            cout << m_tokenCategorier.ChangeToken( token_wait_vector.front().m_token_string ) << endl ;
            token_wait_vector.erase( token_wait_vector.begin() ) ; 
          } // if
          else {
            token_wait_vector.erase( token_wait_vector.begin() ) ;
          } // else
        } // else
      } // else
    } // if
  } // for
} // Statement::PrintTotalTokenAtvectorFirst()  

void Statement :: GetStatement() {

  Token token ;
  stack<Token> wait_token_stack ;
  vector<Token> wait_token_vector ;
  try {
    cout << endl << "> " ;
    token = GetNextToken( wait_token_stack ) ;
    if ( m_not_end ) {
      if ( IsS_EXP( token,  wait_token_stack, wait_token_vector ) ) {
        bool new_line = false ;
        PrintTotalTokenAtvector( wait_token_vector, 0, new_line ) ; 
      } // if
      else {
        PopStackToLast( wait_token_stack ) ; 
        ErrorMessage( "atom or '('", wait_token_stack.top().m_line, 
                      wait_token_stack.top().m_colnum,
                      wait_token_stack.top().m_token_string ) ;
      } // else
    } // if
    else {
      cout << token.m_token_string << endl ;
    } // else 

    m_pl_tokenGetter.ReadReload() ;
  } // try
  catch( exception &e ) {
    if ( strcmp( e.what(), "EOF error" ) == 0 ) {
      throw invalid_argument( e.what() ) ;
    } // if
  } // catch

} // Statement::GetStatement() 

void Statement :: PrintAllOfStatement() {
  try {
    string statement = "" ;
    bool normal_end = false ;
    do {
      GetStatement() ;
    } while ( m_not_end ) ;   // this while is we can loading all token of page
    cout << "Thanks for using OurScheme!" ;  
  } // try
  catch( exception &e ) {
    cout << "Thanks for using OurScheme!" ;
  } // catch

} // Statement::PrintAllOfStatement()

int main() {

  cout << "Welcome to OurScheme!" << endl ;
  char testNum[10] ;
  cin.getline( testNum, 10 ) ;
  GetTokenMachine getToken ;
  TokenClassCategory tokenclass ;
  Statement statement( getToken, tokenclass ) ;
  statement.PrintAllOfStatement() ;


} // main()
