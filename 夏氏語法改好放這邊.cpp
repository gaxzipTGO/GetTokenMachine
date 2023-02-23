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

class TokenClassCategory {

    public: bool IsNum( string str ) {
    for ( int i = 0 ; i < str.length() ; i++ ) {
      if ( str.at( i ) < 48 || str.at( i ) > 57 ) {
        return false ;
      } // if
    } // for

    return true ;
  } // IsNum()

    protected: bool HaveDot( string str ) {
    for ( int i = 0 ; i < str.length() ; i++ ) {
      if ( str.at( i ) == '.' ) {
        return true ;
      } // if
    } // for
    
    return false ; 

  } // HaveDot()

    protected: bool HaveOperate( string str ) {
    for ( int i = 0 ; i < str.length() ; i++  ) {
      if ( str.at( i ) == '+' || str.at( i ) == '-' ) {
        return true ;
      } // if
    } // for

    return false ;

  } // HaveOperate()

    protected: bool HaveWord( string str ) {
    for ( int i = 0 ; i < str.length() ; i++  ) {
      if ( ( str.at( i ) >= 65 && str.at( i ) <= 90 ) || ( str.at( i ) >= 97 && str.at( i ) <= 122 ) ) {
        return true ;
      } // if
    } // for

    return false ;
  } // HaveWord()

    public: int CheakCateGory( string token ) {
    if ( token == "nil" || token == "#f" ) {
      return NIL ;
    } // if
    else if ( token == "t" || token == "#t" ) {
      return T ;
    } // else if 

    bool haveOperate = HaveOperate( token ) ; 
    bool haveDot =  HaveDot( token ) ;
    bool haveWord = HaveWord( token ) ;

    if ( haveDot && haveWord ) {
      return SYMBOL ;
    } // if 

    for ( int i = 0 ; i < token.length() ; i++ ) {

      if ( token.at( i ) == '(' ) {
        return LEFT_PAREN ;
      } // if
      else if ( token.at( i ) == ')' ) {
        return RIGHT_PAREN ;
      } // else if
      else if ( token.at( i ) == '\'' ) {
        return QUOTE ;
      } // else if

    } // for

    return 0 ;
  } // CheakCateGory()

} g_classCategory 
;

class GetTokenMachine {
    protected: bool m_isFile ;
    protected: string m_token ;
    protected: bool m_end ;
    protected: char m_nextChar ; // used to cheak
    protected: char m_bufferDelimiter  ; // used to save delimiter
    protected: string m_bufferToken ;

    public: GetTokenMachine() {
    try {
      m_isFile = false ;
      m_token = "" ;
      m_end = false ;
      m_nextChar = '\0' ;
      m_bufferDelimiter = '\0' ;
      m_bufferToken = "" ;
    } // try
    catch( exception &e ) {
      // cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
      throw invalid_argument( e.what() )  ;            
    } // catch

  } // GetTokenMachine()

    protected: bool IsDelimiter( char ch ) {
    if ( ch == '\"' || ch == '\'' || ch == '.' || ch == ',' || ch == '*' ||
         ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' ||
         ch == '^' || ch == '&' || ch == '(' || ch == ')' || ch == '[' ||
         ch == ']' || ch == '{' || ch == '}' || ch == '|' || ch == ';' ||
         ch == ':' || ch == ';' || ch == '/' || ch == '?' || ch == '<' || 
         ch == '=' || ch == '>' || ch == '+' || ch == '-' ) {

      return true ;
    } // if

    return false ;
  } // IsDelimiter()

    protected: virtual string DelimiterDeal( string token ) {
    token = token + string( 1, m_bufferDelimiter ) ;
    m_bufferDelimiter = '\0' ;
    return token ;
  } // DelimiterDeal()

    protected: virtual string CheakDelimiter() {
    m_bufferDelimiter = m_nextChar ;
    return "" ; 
  } // CheakDelimiter()


    protected: bool GetToken( string & token ) {
    try {
      bool isEmpty = false ;
      if ( m_bufferToken != "" ) {
        token = m_bufferToken ;
        m_bufferToken = "" ;
        return isEmpty ;
      } // if
      // if read delimite we need to save the token and return
      if ( m_bufferDelimiter != '\0' ) {
        token = DelimiterDeal( token ) ;
        return isEmpty ;
      } // if
      // this means our buffer have a delimiter we need to use it first 

      cin.get( m_nextChar ) ;
      while ( m_nextChar != '\t' && m_nextChar != ' ' && m_nextChar != '\n' &&
              ! IsDelimiter( m_nextChar ) && ! cin.eof() ) {
        token = token + string( 1, m_nextChar ) ;
        cin.get( m_nextChar ) ;
      } // while

      if ( IsDelimiter( m_nextChar ) ) {
        token += CheakDelimiter() ;
      } // if m_nextChar is delimiter, it will leave while and go to here

      if ( ! cin.eof() ) {
        return isEmpty  ;
      } // if
      else return true ;
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
    if ( m_nextChar == '\n' || m_end ) { 
      return true ;
    } // if 

    else return false ;

  } // IsEnterChar()

} getTokenMachin
;

class PL_GetToken : public GetTokenMachine {


    protected: string ReadString( string token ) {
    token += m_bufferDelimiter ;
    m_bufferDelimiter = '\0' ;
    cin.get( m_nextChar ) ;
    while ( m_nextChar != '\n' && m_nextChar != '\"' && cin.eof() != true ) {
      token += m_nextChar ;
      cin.get( m_nextChar ) ;
    } // while

    if ( m_nextChar == '\"' && ! cin.eof() ) {
      token += m_nextChar ;
    } // if
    else if ( m_nextChar == '\n' || cin.eof() == true  ) {
      string errorMessage = token + "->the token not exist"  ;
      throw invalid_argument( errorMessage.c_str() )  ;
    } // else if

    return token ;
  } // ReadString()

    protected: string ReadOperate( string token ) {
    char tempDelimiter = m_bufferDelimiter ;
    m_bufferDelimiter = '\0' ;
    string tempToken = "" ;
    m_end = GetToken( token ) ;
    if ( token != "" ) {
      return tempDelimiter + token ;
    } // if
    else {
      return string( 1, tempDelimiter ) ;
    } // else
  } // ReadOperate()

    protected: string ReadPoundSign( string token ) {
    char tempDelimiter = m_bufferDelimiter ;
    m_bufferDelimiter = '\0' ;
    string tempToken = "" ;
    m_end = GetToken( token ) ;
    if ( token != "" ) {
      return tempDelimiter + token ;
    } // if
    else {
      return string( 1, tempDelimiter ) ;
    } // else  
  } // ReadPoundSign()

    protected: string ReadDot() {
    string token = "" ;
    char lastChar = '\0' ;
    cin.get( m_nextChar ) ;
    while ( m_nextChar != '\t' && m_nextChar != ' ' && m_nextChar != '\n' && ! IsDelimiter( m_nextChar ) 
            && ! cin.eof() ) {
      token = token + string( 1, m_nextChar ) ;
      lastChar = m_nextChar ;
      cin.get( m_nextChar ) ;
    } // while

    cin.putback( m_nextChar ) ;
    m_nextChar = lastChar ;
    if ( token != "" ) {
      if ( IsNum( token ) ) {
        return "." + token ;
      } // if
      else {
        cin.putback( m_nextChar ) ;
        m_nextChar = '\0' ;
        for ( int i = token.size() ; i >= 0 ; i -- ) {
          cin.putback( token[i] ) ;
        } // for

        return "." ;
      } // else
    } // if
    else {
      m_bufferDelimiter = '.' ;
      return "" ;
    } // else
  } // ReadDot()

    protected: string PutDelimiterToBuffer() {
    m_bufferDelimiter = m_nextChar ;
    return "" ; 
  } // PutDelimiterToBuffer()

    protected: virtual string CheakDelimiter() {

    if ( m_nextChar == '.' ) {
      return ReadDot() ; 
    } // if
    else {
      return PutDelimiterToBuffer() ;
    } // else

  } // CheakDelimiter()

    protected: virtual string DelimiterDeal( string token ) {
    try {
      if ( m_bufferDelimiter == '\"' ) {
        token = ReadString( token ) ;
      } // if
      else if ( m_bufferDelimiter == '+' || m_bufferDelimiter == '-' ) {
        token = ReadOperate( token ) ;
      } // else if
      else if ( m_bufferDelimiter == '#' ) {
        token = ReadPoundSign( token ) ;
      } // else if 
      else {
        token = token + string( 1, m_bufferDelimiter ) ;
        m_bufferDelimiter = '\0' ;
      } // else

      return token ;

    } // try
    catch( exception &e ) {
      throw invalid_argument( e.what() ) ;
    } // catch
    
  } // DelimiterDeal()

    protected: bool IsNum( string str ) {
    for ( int i = 0 ; i < str.length() ; i++ ) {
      if ( str.at( i ) < 48 || str.at( i ) > 57 ) {
        return false ;
      } // if
    } // for

    return true ;
  } // IsNum()

} g_pl_GetToken
;

int main() {
  string token = "" ;
  bool end ;
  PL_GetToken pl_GetToken ;
  do {
    end = pl_GetToken.GetNextToken( token ) ; // end is a bool means the page is EOF if end is false

    if ( token != "" ) {
    // here we can put some choise to get token like's string
      cout << token << ' ';
    } // if the compare is very important!!! that can cheak the token does read something   

    if ( pl_GetToken.IsEnterChar() ) {
      cout << '\n' ;
    } // if     
  } while ( end ) ;   // this while is we can loading all token of page    
} // main()
