# include <iostream>
# include <string>
# include <cstdio>
# include <stdexcept>
# include <queue>

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
  int type ;
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

  protected: virtual bool IsFloat( string token ) {
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
        if ( ! IsFloat( sub_token_head ) ) {
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

  public: virtual int GetThisTokenType( string token ) {
    return SYMBOL ;
  }

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
  
} 
g_classCategory ;

class GetTokenMachine {
  protected: bool m_isFile ;
  protected: string m_token ;
  protected: bool m_notend ;
  protected: char m_nextChar ; // used to Check
  protected: bool m_bufferDelimiter  ; // used to check m_nextChar is delimiter
  protected: string m_bufferToken ;
  protected: int m_line ;
  protected: int m_column ;
  protected: int m_lastColumn ;
  protected: bool m_reload_line ;
  protected: bool GetToken( string & token ) ;
  public: bool GetNextToken( string &Out_token ) ;

  public: GetTokenMachine() {
    try {
      m_isFile = false ;
      m_token = "" ;
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

  protected: string ErrorMessage( string type, int line, int column, char ch ) {
    while ( GetChar( m_nextChar ) && m_nextChar != '\n' ) {
      ;
    } // while

    return "ERROR (" + type + ") : atom or '(' expected when token at Line "
    + To_String( line ) + " Column " + To_String( column ) + " is >>" + string( 1, ch ) + "<<" ;
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

  protected: virtual string ReadWholeLine() {
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

    if ( sub_token_front.size() == 0 && sub_token_back.size() == 0 ) {
      int buffer_line = m_line ;
      if ( asset_line != 0 ) {
        buffer_line = asset_line ;
      } // if
      else if ( m_nextChar == '\n' ) {
        buffer_line -= 1 ;
      } // if

      int buffer_column = m_lastColumn ;
      return ErrorMessage( "unexpected token", buffer_line, buffer_column, '.' ) ;
    } // if

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
    m_column = 0 ;

  } // Reload() 

} 
g_getTokenMachine ;

bool GetTokenMachine :: GetToken( string & token ) {
  try {

    if ( IsDelimiter( m_nextChar ) ) {
      token = DealDelimiter( token, m_nextChar ) ;
      if ( ! GetChar( m_nextChar ) ) {
        m_nextChar = '\0' ;
      } // if

      return true ;
    } // if

    if ( ! IsDelimiter( m_nextChar ) && 
         m_nextChar != '\n' && m_nextChar != '\t' && 
         m_nextChar != ' ' && m_nextChar != '\0' ) {
      token = token + string( 1, m_nextChar ) ;
    } // if

    while ( GetChar( m_nextChar ) && ! IsDelimiter( m_nextChar ) && 
            m_nextChar != '\n' && m_nextChar != '\t' && m_nextChar != ' '  ) {
      token = token + string( 1, m_nextChar ) ;
    } // while 

    if ( m_nextChar == '.' ) {
      token = ReadDot( token ) ;
      return true ;
    } // if

    if ( token.size() != 0 ) {
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

bool GetTokenMachine :: GetNextToken( string &Out_token ) {
  /*
  the function is we can get the token but we need to chiose Out_token != "" 
  */
  try {
    m_token = "" ;
    if ( m_notend ) {
      do {                
        m_notend = GetToken( m_token ) ;
        if ( m_notend == false ) {
          Out_token = ErrorMessage( "no more input" ) ; 
          return false ;
        } // if        
      } while ( m_token.length() == 0 ) ;
      Out_token = m_token ;
      return true ;
      
    } // if
    
    return false ;        
    
  } // try
  catch ( exception &e ) {
    cout << e.what() << endl  ;
    Out_token = "" ;
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

  public : Statement( GetTokenMachine token_get, TokenClassCategory token_category ) {

    m_not_end = true ;
    m_pl_tokenGetter = token_get ;
    m_tokenCategorier = token_category ;
    m_nextToken = "" ;

  } // Statement()

  protected : string GetToken() {

    string token = "" ;
    m_not_end = m_pl_tokenGetter.GetNextToken( token ) ; 
    return token ;

  } // GetToken()

  protected : bool IsATOM( string token ) {

    if ( m_tokenCategorier.GetThisTokenType( token ) == SYMBOL &&
         m_tokenCategorier.GetThisTokenType( token ) == INT &&
         m_tokenCategorier.GetThisTokenType( token ) == FLOAT &&
         m_tokenCategorier.GetThisTokenType( token ) == STRING &&
         m_tokenCategorier.GetThisTokenType( token ) == NIL &&
         m_tokenCategorier.GetThisTokenType( token ) == T ) {
      return true ;
    } // if
    else if ( m_tokenCategorier.GetThisTokenType( token ) == LEFT_PAREN ) {
      m_nextToken = GetToken() ;
      if ( m_tokenCategorier.GetThisTokenType( token ) == RIGHT_PAREN ) {
        return true ;
      } // if
      else {
        return false ;
      } // else 
    } // else 

    return false ;

  } // IsATOM()

  protected : bool CheckTheS_EXP( string token ) {
    
    if ( IsATOM( token ) ) {
      return true ;
    } // if
    else if ( m_tokenCategorier.GetThisTokenType( token ) == LEFT_PAREN ) {
     if ( CheckTheS_EXP( m_nextToken ) ) {
      
     } // if 
    } // else if
    else if ( m_tokenCategorier.GetThisTokenType( token ) == QUOTE ) {

    } // else if 
    else {
      return false ;
    } // else 

    return false ;


  } // CheckTheS_EXP()

  protected : string GetStatement() {

    string token = "" ;
    token = GetToken() ;
    token = m_tokenCategorier.ChangeToken( token ) ;
    return token ;

  } // GetStatement() 

  public : void PrintAllOfStatement() {

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

  } // PrintAllOfStatement()

} 
; // Statemant


int main() {

  cout << "Welcome to OurScheme!" << endl ;
  char testNum[10] ;
  cin.getline( testNum, sizeof( testNum ) )  ;
  GetTokenMachine getToken ;
  TokenClassCategory tokenclass ;
  Statement statement( getToken, tokenclass ) ;
  statement.PrintAllOfStatement() ;
} // main()
