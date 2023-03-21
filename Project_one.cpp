# include <iostream>
# include <string>
# include <cstdio>
# include <stdexcept>
# include <stack>

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

struct Token {
  string token_string ;
  int colnum = 0 ;
  int line = 0 ;
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
    }

    return false ;

  } // IsFloat()

  protected: virtual string AddZero_Back( string token ) {
    for ( int i = token.size() ; i < 3 ; i ++ ) {
      token = token + string( 1, '0' ) ;
    } // for

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
    else if ( token.compare( "()" ) == 0 ) {
      return "nil" ;
    } // else if 
    else if ( token.compare( "#f" ) == 0 ) {
      return "nil" ;
    } // else if 
    else if ( token.at( 0 ) == '\"' ) {
      return token ;
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
  protected: bool GetToken( Token & token ) ;
  public: bool GetNextToken( Token &Out_token ) ;

  public: GetTokenMachine() {
    try {
      m_isFile = false ;
      m_token.token_string = "" ;
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

  protected: virtual bool GetChar( char &ch ) {

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

      return true ;
    } // if
    else {
      return false ;
    } // else

  } // GetChar()

  protected: string ErrorMessage( string type, int line, int column ) {
    while ( GetChar( m_nextChar ) && m_nextChar != '\n' ) {
      ;
    } // while

    return "ERROR (" + type + ") : END-OF-LINE encountered at Line "+ To_String( line ) 
    + " Column " + To_String( column ) ;
  } // ErrorMessage()

  protected: string ErrorMessage( string type ) {
    while ( GetChar( m_nextChar ) && m_nextChar != '\n' ) {
      ;
    } // while

    return "ERROR (" + type + ") : END-OF-FILE encountered" ;
  } // ErrorMessage()

  protected: virtual string DelimiterDeal( string token ) {
    token = token + string( 1, m_bufferDelimiter ) ;
    m_bufferDelimiter = '\0' ;
    return token ;
  } // DelimiterDeal()
  
  protected: virtual string CheckDelimiter() {
    // 型別要不要改成void function名要不要改名為SaveDelimiterToBuffer 或 SaveDelimiter
    // 不要改成void 名字是OK
    m_bufferDelimiter = m_nextChar ;
    return "" ; 
  } // CheckDelimiter()

  public: virtual string ReadWholeLine() {
    // 型別要不要改成void
    // 不要
    while ( m_nextChar != '\n' && ! cin.eof() ) {
      GetChar( m_nextChar );
    }  // while

    return "" ;
  } // ReadWholeLine()

  protected: virtual void UpdateToken( string &token ) {
  /*
    如果進到這個function 他會先看有沒有特別的東西 ex \n \t \0 這類的
    如果有的話 就要讓他變成它的功能 如果沒有特別的 就進else代表他的東西要保留
    \Y \X那些的 要保留下來
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
        not_end = GetChar( m_nextChar );
        UpdateToken( token ) ; // 如果進到這個function裡的else 那會是啥情況
                               // 我放在那個function裡面解釋
        not_end = GetChar( m_nextChar );  
        continue_choise = true ;            
      } // if 

      if ( ! continue_choise ) { // 所以是continue_choise == false才會進這if?
        /*
        對 我跟你說他的命名由來:c o n t i n u e , 但老大不給用所以我只能這樣 沒其他原因
        你知道為什麼我要這樣空格嗎 因為GetToken沒寫好 如果打那個東西在註解裡也會被夏式語法擋住
        */
        token = token + string( 1, m_nextChar ) ;
        not_end = GetChar( m_nextChar );
      } // if

    } while ( m_nextChar != '\"' && m_nextChar != '\n' && not_end ) ; // do-while()
    if ( m_nextChar == '\n' || ! not_end ) {
      return ErrorMessage( "no closing quote", m_line, m_lastColumn ) ;
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
    } // while

    return sub_token_front + string( 1, '.' ) + sub_token_back ;

  } // ReadDot()

  protected: virtual string ReadPAREN( char ch ) {
    return string( 1, ch )  ;
  } // ReadPAREN()

  protected: virtual string DealDelimiter( string token, char ch ) {

    if ( ch == ';' ) {
      return ReadWholeLine() ;
    } // if
    else if ( ch == '\"' ) {
      return ReadString() ;
    } // else if
    else if ( ch == '.' ) {
      return ReadDot( token ) ;
    } // else if
    else if ( ch == '(' || ch == ')' ) {
      return ReadPAREN( ch ) ;
    } // else if

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
  
  public: void Reload() {

    m_reload_line = true ;
    m_column = 1 ;

  } // Reload() 

} 
g_getTokenMachine ;

bool GetTokenMachine :: GetToken( Token & token ) {
  try {
    if ( IsDelimiter( m_nextChar ) ) {
      token.colnum = m_column ;
      token.line = m_line ;
      token.token_string = DealDelimiter( token.token_string, m_nextChar ) ;
      if ( ! GetChar( m_nextChar ) ) {
        m_nextChar = '\0' ;
      } // if

      return true ;
    } // if

    if ( ! IsDelimiter( m_nextChar ) && 
         m_nextChar != '\n' && m_nextChar != '\t' && 
         m_nextChar != ' ' && m_nextChar != '\0' ) {
      token.colnum = m_column ;
      token.line = m_line ;
      token.token_string = token.token_string + string( 1, m_nextChar ) ;
    } // if
    bool get_info = false ;
    while ( GetChar( m_nextChar ) && ! IsDelimiter( m_nextChar ) && 
            m_nextChar != '\n' && m_nextChar != '\t' && m_nextChar != ' '  ) {
      if ( ! get_info ) {
        token.colnum = m_column ;
        token.line = m_line ;
        get_info = true ;        
      } // token
      token.token_string = token.token_string + string( 1, m_nextChar ) ;
    } // while 

    if ( m_nextChar == '.' ) {
      if ( ! get_info ) {
        token.colnum = m_column ;
        token.line = m_line ;        
      } // token
      token.token_string = ReadDot( token.token_string ) ;
      return true ;
    } // if

    if ( token.token_string.size() != 0 ) {
      return true ;
    } // if

    if ( cin.eof() ) {
      m_nextChar = '\0' ;
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
  the function is we can get the token but we need to chiose Out_token != "" 
  */
  try {
    m_token.token_string = "" ;
    if ( m_notend ) {
      do {                
        m_notend = GetToken( m_token ) ;
        if ( m_notend == false ) {
          Out_token.token_string = ErrorMessage( "no more input" ) ; 
          return false ;
        } // if        
      } while ( m_token.token_string.length() == 0 ) ;
      Out_token = m_token ;
      return true ;
      
    } // if
    
    return false ;        
    
  } // try
  catch ( exception &e ) {
    cout << e.what() << endl  ;
    Out_token.token_string = "" ;
    if ( !cin.eof() ) {
      return ( GetNextToken( Out_token ) ) ;
    } // if
    else {
      return false ;
    } // else
  } // catch
  
} // GetTokenMachine::GetNextToken()

class Statement {

  protected : GetTokenMachine m_pl_tokenGetter ;
  protected : TokenClassCategory m_tokenCategorier ;
  protected : bool m_not_end ;
  protected : string m_nextToken ;
  protected : stack<Token> m_get_token ;
  protected : stack<string> m_errorMessage ;

  protected : Token GetToken() ;
  protected: string ErrorMessage( string type, int line, int column, string token ) ;
  protected : Token GetNextToken( stack<Token> &token_wait_stack ) ;
  protected : void CheckTheS_EXP_WHILE( stack<Token> &token_wait_stack ) ; 
  protected : bool CheckTheDOT_AND_S_EXP(  stack<Token>  &token_wait_stack ) ; 
  protected : bool IsATOM( Token token, stack<Token> &token_wait_stack ) ;
  protected : bool IsS_EXP( Token token, stack<Token> &token_wait_stack ) ;
  protected : string GetStatement() ;
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
  m_not_end = m_pl_tokenGetter.GetNextToken( token ) ; 
  return token ;

} // Statement::GetToken()

string Statement :: ErrorMessage( string type, int line, int column, string token ) {
  m_pl_tokenGetter.ReadWholeLine() ;
  return "ERROR (" + type + ") : atom or '(' expected when token at Line "
  + To_String( line ) + " Column " + To_String( column ) + " is >>" + token + "<<" ;
} // ErrorMessage()

Token Statement :: GetNextToken( stack<Token> &token_wait_stack ) {
/*
  從輸入的文件或stack裡面拿一個token出來 如果stack有東西就先用stack的 這樣才不會打架
*/
  if ( ! token_wait_stack.empty() ) {
    Token token = token_wait_stack.top() ;
    token_wait_stack.pop() ;
    return token ;
  } // if
  else {
    return GetToken() ;
  } // else

} // Statement::GetNextToken()

bool Statement :: CheckTheDOT_AND_S_EXP( stack<Token>  &token_wait_stack ) {
  Token token = GetNextToken( token_wait_stack ) ;
  if ( m_tokenCategorier.GetThisTokenType( token.token_string ) == DOT ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack ) ) {
      return true ;
    } // if
  } // if

  token_wait_stack.push( token ) ;
  return false ;
} // Statement::CheckTheDOT_AND_S_EXP()

void Statement :: CheckTheS_EXP_WHILE( stack<Token> &token_wait_stack ) {

  Token token = GetNextToken( token_wait_stack ) ;
  Token temp_wait_token ;
  while ( IsS_EXP( token, token_wait_stack ) ) {
    token = GetNextToken( token_wait_stack ) ;
  } // while



} // Statement::CheckTheS_EXP_WHILE()

bool Statement :: IsATOM( Token token, stack<Token> &token_wait_stack ) {

  int type = m_tokenCategorier.GetThisTokenType( token.token_string ) ;
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
    if ( m_tokenCategorier.GetThisTokenType( nextToken.token_string ) == RIGHT_PAREN ) {
      return true ;
    } // if
    else {
      token_wait_stack.push( nextToken ) ;
      return false ;
    } // else 
  } // else if 

  return false ;

} // Statement::IsATOM()

bool Statement :: IsS_EXP( Token token, stack<Token> &token_wait_stack ) {
    
  if ( IsATOM( token, token_wait_stack ) ) {
    return true ;
  } // if
  else if ( m_tokenCategorier.GetThisTokenType( token.token_string ) == LEFT_PAREN ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack ) ) {
      CheckTheS_EXP_WHILE( token_wait_stack ) ; 
      CheckTheDOT_AND_S_EXP( token_wait_stack ) ;
      temp_token = GetNextToken( token_wait_stack ) ;
      if ( m_tokenCategorier.GetThisTokenType( temp_token.token_string ) == RIGHT_PAREN ) {
        return true ;
      } // if 
      else {
        token_wait_stack.push( temp_token ) ;
      } // else
    } // if
    else {
      token_wait_stack.push( temp_token ) ;
    } // else
  } // else if 
  else if ( m_tokenCategorier.GetThisTokenType( token.token_string ) == QUOTE ) {
  } // else if  

  
  token_wait_stack.push( token ) ;    
  return false ;

} // Statement::IsS_EXP()

string Statement :: GetStatement() {

  Token token ;
  stack<Token> wait_token_stack ;
  token = GetNextToken( wait_token_stack ) ;
  
  if ( IsS_EXP( token,  wait_token_stack ) ) {
    token.token_string = m_tokenCategorier.ChangeToken( token.token_string ) ;
  } // if
  else {
    return ErrorMessage( "unexpected token", wait_token_stack.top().line, 
                         wait_token_stack.top().colnum, wait_token_stack.top().token_string ) ;
  } // else

  return token.token_string ;

} // Statement::GetStatement() 

void Statement :: PrintAllOfStatement() {
  string statement = "" ;
  bool normal_end = false ;
  do {
    statement = GetStatement() ;
    cout << endl ;
    // end is a bool means the page is EOF ,if end is false
    cout << "> " ;
    if ( statement.compare( "(exit)" ) != 0 ) {
      cout << statement << endl  ;
      m_pl_tokenGetter.Reload() ;
    } // if
    else {
      m_not_end = true ;
      normal_end = true ;
    } // else
  } while ( m_not_end ) ;   // this while is we can loading all token of page
  cout << "Thanks for using OurScheme!" ;  

} // Statement::PrintAllOfStatement()

int main() {

  cout << "Welcome to OurScheme!" << endl ;
  char testNum[10] ;
  cin.getline( testNum, sizeof( testNum ) )  ;
  GetTokenMachine getToken ;
  TokenClassCategory tokenclass ;
  Statement statement( getToken, tokenclass ) ;
  statement.PrintAllOfStatement() ;
  
} // main()
