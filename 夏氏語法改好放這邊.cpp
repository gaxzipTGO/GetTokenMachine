# include <iostream>
# include <string>
# include <cstdio>
# include <stdexcept>

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

  protected: virtual string AddZero_Back ( string token ) {
    for ( int i = token.size() ; i < 3 ; i ++ ) {
      token = token + string( 1, '0' ) ;
    } // for

    return token ;
  } // AddZero_Back ()

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
            return sub_token_head + '.' + AddZero_Back ( sub_token_tail ) ;
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
  
} 
g_classCategory ;

class GetTokenMachine {
  protected: bool m_isFile ;
  protected: string m_token ;
  protected: bool m_end ;
  protected: char m_nextChar ; // used to Check
  protected: bool m_bufferDelimiter  ; // used to check m_nextChar is delimiter
  protected: string m_bufferToken ;
  protected: int m_line ;
  protected: int m_column ;
  protected: int m_lastColumn ;
  public: GetTokenMachine() {
    try {
      m_isFile = false ;
      m_token = "" ;
      m_end = false ;
      m_nextChar = '\0' ;
      m_bufferDelimiter = '\0' ;
      m_bufferToken = "" ;
      m_line = 1 ;
      m_column = 0 ;
      m_lastColumn = 0 ;
    } // try
    catch( exception &e ) {
      // cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
      throw invalid_argument( e.what() )  ;            
    } // catch
    
  } // GetTokenMachine()
  
  protected: bool IsDelimiter( char ch ) {
    if ( ch == '\"' || ch == '\'' || 
         ch == '(' || ch == ')' || ch == ';' ) {
      
      return true ;
    } // if
    
    return false ;
  } // IsDelimiter()

  /*
  protected: bool IsDelimiter( char ch ) {
    if ( ch == '\"' || ch == '\'' || ch == '.' || ch == ',' || ch == '*' ||
         ch == '@' || ch == '#' || ch == '$' || ch == '%' ||
         ch == '^' || ch == '&' || ch == '(' || ch == ')' || ch == '[' ||
         ch == ']' || ch == '{' || ch == '}' || ch == '|' || ch == ';' ||
         ch == ':' || ch == ';' || ch == '/' || ch == '?' || ch == '<' || 
         ch == '=' || ch == '>' || ch == '+' || ch == '-' ) {
      
      return true ;
    } // if
    
    return false ;
  } // IsDelimiter()
  */
  protected: virtual bool GetChar( char &ch ) {

    if ( cin.get( ch ) ) {
      m_column += 1 ;
      if ( ch == '\n' ) {
        m_lastColumn = m_column ;
        m_column = 0 ;
      } // if

      return true ;
    } // if
    else {
      return false ;
    } // else

  } // GetChar()

  protected: virtual string DelimiterDeal( string token ) {
    token = token + string( 1, m_bufferDelimiter ) ;
    m_bufferDelimiter = '\0' ;
    return token ;
  } // DelimiterDeal()
  
  protected: virtual string CheckDelimiter() {
    // 型別要不要改成void function名要不要改名為SaveDelimiterToBuffer 或 SaveDelimiter
    m_bufferDelimiter = m_nextChar ;
    return "" ; 
  } // CheckDelimiter()

  protected: virtual string ReadWholeLine() {
    // 型別要不要改成void
    while ( m_nextChar != '\n' && ! cin.eof() ) {
      GetChar( m_nextChar );
    }  // while

    return "" ;
  } // ReadWholeLine()

  protected: virtual void ChangeToken( string &token ) {
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
  } // ChangeToken()

  protected: virtual string ReadString() {
    string token = "" ;
    do {
      bool continue_choise  = false ;
      if ( m_nextChar == '\\' ) {
        GetChar( m_nextChar );
        ChangeToken( token ) ;// 如果進到這個function裡的else 那會是啥情況
        GetChar( m_nextChar );  
        continue_choise = true ;            
      } // if 

      if ( ! continue_choise ) { //所以是continue_choise == false才會進這if?
        token = token + string( 1, m_nextChar ) ;
        GetChar( m_nextChar );
      } // if

    } while ( m_nextChar != '\"' && m_nextChar != '\n' ) ; // do-while()
    if ( m_nextChar == '\n' ) {
      return "ERROR (no closing quote) : END-OF-LINE encountered at Line "+ To_String( m_line ) 
      + " Column " + To_String( m_lastColumn ) ;
    } // if

    token = token + string( 1, m_nextChar ) ;
    return token ;
  } // ReadString()

  protected: virtual string DealDelimiter( char ch ) {

    if ( ch == ';' ) {
      m_line += 1 ;
      return ReadWholeLine() ;
    } // if
    else if ( ch == '\"' ) {
      return ReadString() ;
    } // else if

    return string( 1, ch ) ;
  } // DealDelimiter()

  protected: bool GetToken( string & token ) {
    try {
      bool delimiter_is = false ;
      while ( GetChar( m_nextChar ) && ! delimiter_is && m_nextChar != '\n' &&
              m_nextChar != '\t' && m_nextChar != ' '  ) {
        if ( ! IsDelimiter( m_nextChar ) ) {
          token = token + string( 1, m_nextChar ) ;
        } // if
        else {
          token = token + DealDelimiter( m_nextChar ) ;
        } // else
      } // while 

      if ( cin.eof() ) {
        m_nextChar = '\0' ;
        return true ;
      } // if 

      return false ;
    } // try 
    catch( exception &e ) {
      throw invalid_argument( e.what() ) ;
    } // catch
  } // GetToken()
  
  
  
  public: bool GetNextToken( string &Out_token ) {
    /*
    the function is we can get the token but we need to chiose Out_token != "" 
    */
    try {
      m_token = "" ;
      if ( ! m_end ) {
        do {                
          m_end = GetToken( m_token ) ;
          if ( m_end == true ) {
            Out_token = m_token ; 
            return false ;
          } // if        
        } while ( m_token.length() == 0 && m_nextChar != '\n' ) ;
        
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
    
  } // GetNextToken()
  
  public: char ReturnNextChar() {
    return m_nextChar ;
  } // ReturnNextChar()
  
  public: bool IsEnterChar() {
    if ( m_nextChar == '\n' ) { 
      return true ;
    } // if 
    
    else return false ;
    
  } // IsEnterChar()
  
  public: void ReloadLine() {
    m_line = 1 ;
  } // ReloadLine() 

} 
getTokenMachine ;


int main() {

  cout << "Welcome to OurScheme!" << endl ;
  char test_number = 0 ;
  do {
    cin.get( test_number ) ;
  } while ( test_number != '\n' ) ; // while
  cout << endl  ;

  string token = "" ;
  bool end ;
  bool normal_end = false ;
  GetTokenMachine pl_GetToken ;
  TokenClassCategory tokenCategory ;
  do {
    end = pl_GetToken.GetNextToken( token ) ; // end is a bool means the page is EOF if end is false

    if ( token != "" ) {
      cout << "> " ;

      if ( token.compare( "(exit)" ) != 0 ) {
        cout << tokenCategory.ChangeToken( token ) << endl << endl  ;
        pl_GetToken.ReloadLine() ;
      } // if
      else {
        end = false ;
        normal_end = true ;
      } // else
      
    } // if the compare is very important!!! that can Check the token does read something   

    if ( end == false && ! normal_end == true ) {
      cout << "> ERROR (no more input) : END-OF-FILE encountered" << endl ;
    } // if 
  } while ( end ) ;   // this while is we can loading all token of page
  cout << "Thanks for using OurScheme!" ;  

} // main()
