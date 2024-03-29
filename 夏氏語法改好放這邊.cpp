# include <iostream>
# include <string>
# include <cstdio>
# include <stdexcept>
# include <stack>
# include <queue>
# include <vector>
# include <map>
# include <stdio.h>
# include <string.h>

//    // ***************************************************************************** //
//    //                                                                               //
//    //                               開始 project 2                                  //
//    //                                                                               //
//    //                                                                               //
//    //                                                                               //
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
  SYMBOL = 3228,
  TOKEN = 5556,
  LISP = 7788,
  ATOM = 6655,
  CONS = 7789,
  BOOL = 9876
};

/*
  這堆東西是一開始就要用的字串
  每個都有特定的意義
  如果遇到這些東西 就要確認他們的狀態了
*/

class Token {
  public : string m_token_string ;
  public : int m_colnum ;
  public : int m_line ;
  public : int m_type ;

  public : Token() {
    m_token_string = "" ;
    m_colnum = 0 ;
    m_line = 0 ;
    m_type = 0 ;
  } // Token()

  public : Token( string str, int colnum, int line, int type ) {

    m_token_string = str ;
    m_colnum = colnum ;
    m_line = line ;
    m_type = type ;

  } // Token()
} 
;

// ------------------------------------------------------------NEW-------------------------------------------

struct Function {
  string function_name ;
  string operater ;
  int argument ;
}
;

Function SetFunctionStruct( string function_name,  string operater, int argument ) {
  Function temp ;
  temp.function_name = function_name ;
  temp.operater = operater ;
  temp.argument = argument ;
  return temp ;
} // SetFunctionStruct()

vector<Function> g_predef_function ;
vector<Function> g_custom_function ;

void LoadPreDefFunction() {
  // Constructors
  g_predef_function.push_back( SetFunctionStruct( "cons", "==", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "list", ">=", 0 ) ) ;

  // Bypassing the default evaluation
  g_predef_function.push_back( SetFunctionStruct( "quote", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "'", "==", 1 ) ) ;

  // The binding of a symbol to an S-expression
  g_predef_function.push_back( SetFunctionStruct( "define", "==", 2 ) ) ;

  // Part accessors
  g_predef_function.push_back( SetFunctionStruct( "car", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "cdr", "==", 1 ) ) ;

  // Primitive predicates (all functions below can only take 1 argument)
  g_predef_function.push_back( SetFunctionStruct( "atom?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "pair?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "list?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "null?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "integer?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "real?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "number?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "string?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "boolean?", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "symbol?", "==", 1 ) ) ;

  // Basic arithmetic, logical and string operations
  g_predef_function.push_back( SetFunctionStruct( "+", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "-", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "*", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "/", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "not", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "and", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "or", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( ">", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( ">=", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "<", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "<=", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "=", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "string-append", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "string>?", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "string<?", ">=", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "string=?", ">=", 2 ) ) ;

  // Eqivalence tester
  g_predef_function.push_back( SetFunctionStruct( "eqv?", "==", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "equal?", "==", 2 ) ) ;

  // Sequencing and functional composition
  g_predef_function.push_back( SetFunctionStruct( "begin", ">=", 1 ) ) ;

  // Conditionals
  g_predef_function.push_back( SetFunctionStruct( "if", "==", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "if", "==", 3 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "cond", ">=", 1 ) ) ;
  // clean-environment
  g_predef_function.push_back( SetFunctionStruct( "clean-environment", "==", 0 ) ) ;
  // exit
  g_predef_function.push_back( SetFunctionStruct( "exit", "==", 0 ) ) ;
} // LoadPreDefFunction()

bool SearchFromFunction( vector<Function> function_vector, string symbol ) {
    // 在vector中查找特定符號
  for ( vector<Function>::iterator it = function_vector.begin() ; it != function_vector.end() ; ++it ) {
    if ( it->function_name == symbol ) {
      return true;
    } // if
  } // for

  // 沒有找到該符號
  
  return false ;
} // SearchFromFunction()

Function GetFromFunction( string symbol ) {
  // 在vector中查找特定符號
  for ( vector<Function>::iterator it = g_predef_function.begin() ; it != g_predef_function.end() ; ++it ) {
    if ( it->function_name == symbol ) {
      return *it ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; it != g_custom_function.end() ; ++it ) {
    if ( it->function_name == symbol ) {
      return *it ;
    } // if
  } // for

  // 沒有找到該符號
  cout << "ERROR (unbound symbol) : " << symbol << endl ;
  throw invalid_argument( "unbound symbol" ) ;

} // GetFromFunction()

int GetArgument( string symbol ) {
    // 在vector中查找特定符號
  for ( vector<Function>::iterator it = g_predef_function.begin() ; 
        it != g_predef_function.end() ; ++it ) {
    if ( it->function_name == symbol ) {
      return it->argument ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; 
        it != g_custom_function.end() ; ++it ) {
    if ( it->function_name == symbol ) {
      return it->argument ;
    } // if
  } // for

  // 沒有找到該符號，返回預設值0
  return 0;
} // GetArgument()

class TreeNode {
  public:
  Token* m_left_token ;
  TreeNode* m_left ;
  TreeNode* m_right ;
  int m_type ;
  bool m_left_ok ;
  bool m_can_read ;


  TreeNode() {
    m_type = 0 ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = NULL ;
    m_left_ok = true ;
    m_can_read = true ;
  } // TreeNode()

  TreeNode( bool dontread ) {
    m_type = 0 ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = NULL ;
    m_left_ok = true ;
    m_can_read = dontread ;
  } // TreeNode()

  void AddLeft( Token* token ) {
    m_left_token = token ;
    m_left_ok = false ;
    m_type = TOKEN ;
  } // AddLeft()

  void AddLeft( TreeNode* t ) {
    m_left = t ;
    m_left_ok = false ;
    m_type = LISP ;
  } // AddLeft()

  void AddRight( TreeNode* t ) {
    m_right = t ;
  } // AddRight()

  bool IsNIL() {
    if ( m_left == NULL && m_right == NULL && m_left_token == NULL ) {
      return true ;
    } // if
    
    return false ;
  } // IsNIL()
} // TreeNode
;

int CountRightNodes( TreeNode* root ) {
  if ( root == NULL ) {
    return 0 ;
  } // if

  int count = 0 ;
  if ( root->m_right ) {
    count++ ;
  } // if
  
  count += CountRightNodes( root->m_right ) ;
  return count ;
} // CountRightNodes()

bool CheckRightNodeCount( TreeNode* now_TreePtr, int argument, const string& operater ) {
  int right_node_count = CountRightNodes( now_TreePtr );
  if ( operater == "<" ) {
    return right_node_count < argument;
  } // if
  else if ( operater == ">" ) {
    return right_node_count > argument;
  } // else if
  else if ( operater == "<=" ) {
    return right_node_count <= argument;
  } // else if
  else if ( operater == ">=" ) {
    return right_node_count >= argument;
  } // else if
  else if ( operater == "==" ) {
    return right_node_count == argument;
  } // else if
  else {
    // 非法的 operater 符號，可以選擇回傳 false 或拋出異常等處理方式
    throw invalid_argument( "incorrect number of arguments" ) ;
  } // else
} // CheckRightNodeCount()

bool CheckNodeValidity( TreeNode* now_ptr, vector<Function>& function_vector ) {
  string symbol = now_ptr->m_left_token->m_token_string ;
  // 如果符號不在vector中，拋出錯誤
  Function function = GetFromFunction( symbol ) ;
  int argument = function.argument ;
  string operater = function.operater ;
  if ( CheckRightNodeCount( now_ptr, argument, operater ) ) {
    return true ;
  } // if

  return false ;
} // CheckNodeValidity()

bool IsAtomType( int type ) {
  if ( type == SYMBOL || type == INT || type == FLOAT || type ==STRING || type == NIL || type == T ) {
    return true ;
  } // if
  else if ( type == LEFT_PAREN || type == RIGHT_PAREN ) {
    return true ;
  } // else if
  else return false ;
} // IsAtomType()

void Function_CheckAtom( vector<Token> &input_token, vector<Token> &output_token ) {
// ex: (cons 3 nil ) 丟進來這裡的是: 3 nil ) 
  Token null = Token( "NIL", 0, 0, NIL )  ;
  int num = input_token.size() ;

  if ( num == 1 ) {
    if ( IsAtomType( input_token.at( 0 ).m_type ) ) {
      output_token.push_back( input_token.at( 0 ) ) ;
    } // if
    else output_token.push_back( null ) ;
  } // if
  else if ( num == 2 ) {
    

  } // else if 
} // Function_CheckAtom()

void Function_CheckPair( vector<Token> &input_token, vector<Token> &output_token ) {


} // Function_CheckPair()

void Function_CheckList( vector<Token> &input_token, vector<Token> &output_token ) {

} // Function_CheckList()

struct Object {
  string object_name ;
  TreeNode* object_ptr ;
}
;

vector<Obect> g_def_object ;

TreeNode* Get_DefObject_Ptr( string object_name ) {

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      return it->object_ptr ;
    } // if
  } // for

  throw invalid_argument( "Not Define" ) ;

} // Get_DefObject_Ptr()


// ------------------------------------------------------------NEW-------------------------------------------

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

  protected: virtual bool IsIntMissoperater( string token, bool noSize ) {
  /*
    確認給進來的這一項是不是數字 可以無視一次+ or - 是的話回傳一個 true
  */
    bool miss = true ;
    if ( token == "+" || token == "-" )
      return false ;

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

  } // IsIntMissoperater()

  protected: virtual bool IsIntMissoperater( string token ) {
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

  } // IsIntMissoperater()

  protected: virtual bool IsFloat( string token ) {
    size_t find = token.find( '.' ) ;
    if ( token.npos != find ) {
      string sub_string_head = token.substr( 0, find ) ;
      string sub_string_tail = token.substr( find+1, token.size()-find ) ;
      if ( IsIntMissoperater( sub_string_head, false ) &&
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
        if ( ! IsIntMissoperater( sub_token_head ) ) {
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
    else if ( token == "+" || token == "-" || token == "*" ) {
      return token ;
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
    else if ( IsIntMissoperater( token ) ) { // 只有純數字的int ex: 123 456
      return INT ;
    } // else if 
    else if ( token.at( 0 ) == '+' || token.at( 0 ) == '-' ) { // +123 -456
      if ( IsIntMissoperater( token.substr( 1, token.size() ) ) ) { 
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
      m_nextChar = getchar() ;
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
        m_nextChar = getchar() ;
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

      m_nextChar = getchar() ;

    } // while 

    if ( m_nextChar == EOF ) {
      ErrorMessage( "no more input" ) ;
    } // if

    m_column = 0 ;
    m_line = 1 ;
    m_reload_line = false ;

  } // ReloadError() 

  public: void ReadReload() {

    m_reload_line = true ;
    m_line = 1 ;
    m_column = 1 ;
    
  } // ReadReload()

  public: void Reload() {
    if ( m_nextChar == '\n' ) {
      m_column = 0 ;
      m_reload_line = false ;
    } // if
    else {
      m_column = 1 ;
      m_reload_line = true ;
    } // else

    m_line = 1 ;
  } // Reload()

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

    if ( ch != ' ' && ch != '\t' && ch != '\n' && ch != ';' ) {
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
  protected : TreeNode* m_nowTreePtr ;


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
  protected : void PrintFunction( vector<Token> &token_function_vector ) ;
  protected : void CreateLisp( TreeNode* now_TreePtr, vector<Token> &wait_token_vector, bool skip_paren ) ;
  protected : void PrintStatementResult( vector<Token> &wait_token_vector ) ;
  protected : void Exe_S_EXP( TreeNode* now_TreePtr ) ;

  public : void PrintAllOfStatement() ;
  public : void PrintLisp( TreeNode* now_TreePtr, int level, bool &enter ) ;

  public : Statement( GetTokenMachine token_get, TokenClassCategory token_category ) {
    m_not_end = true ;
    m_pl_tokenGetter = token_get ;
    m_tokenCategorier = token_category ;
    m_nextToken = "" ;
    m_nowTreePtr = new TreeNode( false ) ;
  } // Statement()

} 
; // Statemant

Token Statement :: GetToken() {
  
  Token token ;
  m_not_end = m_pl_tokenGetter.GetNextToken( token ) ; // 得到一個token ( false表示eof token是空的 )
  token.m_type = m_tokenCategorier.GetThisTokenType( token.m_token_string ) ;
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
  Token tempToken = Token( token.m_token_string, token.m_colnum, token.m_line, token.m_type ) ;
  token_wait_vector.push_back( tempToken ) ; 
     
  if ( IsATOM( token, token_wait_stack, token_wait_vector ) ) {
    return true ;
  } // if
  else if ( m_tokenCategorier.GetThisTokenType( token.m_token_string ) == LEFT_PAREN ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
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
      ErrorMessage( "atom or '('", token_wait_stack.top().m_line, 
                    token_wait_stack.top().m_colnum,
                    token_wait_stack.top().m_token_string ) ;      
    } // else
  } // else if 
  else if ( m_tokenCategorier.GetThisTokenType( token.m_token_string ) == QUOTE ) {
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
      return true ;
    } // if
    else {
      token_wait_vector.erase( token_wait_vector.end() ) ;
      return false ;
    } // else

  } // else if  

  token_wait_vector.pop_back() ;
  token_wait_stack.push( token ) ;    
  return false ;

} // Statement::IsS_EXP()

void Statement :: CreateLisp( TreeNode* now_TreePtr, vector<Token> &wait_token_vector, bool skip_paren ) {
/*
進來之後要開始做樹 基本上在一開始應該是有一個TreeNode的 所以在外面要先準備好
*/
  if ( wait_token_vector.front().m_type == LEFT_PAREN ) {
    if ( now_TreePtr->m_left_ok == true ) {
      TreeNode* tempNode = new TreeNode() ;
      now_TreePtr->AddLeft( tempNode ) ;
      wait_token_vector.erase( wait_token_vector.begin() ) ;
      CreateLisp( now_TreePtr->m_left, wait_token_vector, false ) ;
      if ( wait_token_vector.size() != 0 ) {
        CreateLisp( now_TreePtr, wait_token_vector, skip_paren ) ;
      } // if 如果往下的都結束了 回到這邊 要確定有沒有東西要放在右邊
    } // if 可以直接加
    else {
      TreeNode* tempNode = new TreeNode() ;
      now_TreePtr->AddRight( tempNode ) ;
      CreateLisp( now_TreePtr->m_right, wait_token_vector, false ) ;
    } // else 不能直接加
  } // if 如果今天遇到"(" 那我們要先確認現在指向的這個TreeNode的left是否有東西 
    // 如果沒有的話可以直接在left加一個新的TreeNode 並且處理掉第一項
    // 如果有東西的話要先在右邊加一個TreeNode 然後進到那個function裡面
  else {
    if ( wait_token_vector.front().m_type == DOT ) {
      if ( wait_token_vector[1].m_type == LEFT_PAREN  ) {
        wait_token_vector.erase( wait_token_vector.begin(), wait_token_vector.begin() + 2 ) ;
        CreateLisp( now_TreePtr, wait_token_vector, true ) ;
      } // if
      else if ( wait_token_vector[1].m_token_string == "0" || 
                wait_token_vector[1].m_type == NIL ) {
        wait_token_vector.erase( wait_token_vector.begin(), wait_token_vector.begin() + 2 ) ;
      } // else if

    } // if
    
    if ( wait_token_vector.front().m_type == RIGHT_PAREN ) {
      wait_token_vector.erase( wait_token_vector.begin() ) ;
    } // if 如果是 ")" 
    else {
      if ( now_TreePtr->m_left_ok == true ) {
        Token* tempToken = new Token( wait_token_vector.front().m_token_string, 
                                      wait_token_vector.front().m_colnum,
                                      wait_token_vector.front().m_line,
                                      wait_token_vector.front().m_type ) ;
        now_TreePtr->AddLeft( tempToken ) ;
        wait_token_vector.erase( wait_token_vector.begin() ) ;
        CreateLisp( now_TreePtr, wait_token_vector, false ) ;
      } // if
      else {
        TreeNode* tempNode = new TreeNode() ;
        now_TreePtr->AddRight( tempNode ) ;
        CreateLisp( now_TreePtr->m_right, wait_token_vector, false ) ;        
      } // else
    } // else 如果今天遇到 atom 那我們要先確認現在指向的這個TreeNode的left是否有東西 
      // 如果沒有的話可以直接在left加一個新的Token 並且處理掉第一項
      // 如果有東西的話要先在右邊加一個TreeNode 然後進到那個function裡面
  } // else

} // Statement::CreateLisp()

void Statement :: PrintLisp( TreeNode* now_TreePtr, int level, bool &enter ) {

  if ( now_TreePtr != NULL && now_TreePtr->IsNIL() ) {
    if ( enter )
      PrintWhiteSpaceWithLevel( level-1 ) ;
    cout << "nil" << endl ;
    enter = true ;
  } // if
  else {
    if ( now_TreePtr == NULL ) {
      if ( enter )
        PrintWhiteSpaceWithLevel( level-1 ) ;
      cout << ")" << endl ;
      enter = true ;
    } // if
    else {
      if ( now_TreePtr->m_type == LISP ) {
        if ( ! now_TreePtr->m_left->IsNIL() ) {
          if ( enter )
            PrintWhiteSpaceWithLevel( level ) ;

          if ( level == 0 ) {
            if ( now_TreePtr->m_left->m_type == TOKEN ) {
              if ( now_TreePtr->m_left->m_left_token->m_token_string == "exit" ) {
                if ( CountRightNodes( now_TreePtr->m_left ) == 0 ) {
                  throw invalid_argument( "Normal Exit" ) ;
                } // if
              } // if
            } // if 這裡是判斷有沒有遇到離開 基本上只會用到第一次而已
          } // if

          cout << "( " ;
          enter = false ;
        } // if

        PrintLisp( now_TreePtr->m_left, level+1, enter ) ;
      } // else if
      else if ( now_TreePtr->m_type == TOKEN ) {

        if ( enter )
          PrintWhiteSpaceWithLevel( level ) ;
        cout << m_tokenCategorier.ChangeToken( now_TreePtr->m_left_token->m_token_string ) << endl ;
        enter = true ;
      } // if

      if ( now_TreePtr->m_can_read )
        PrintLisp( now_TreePtr->m_right, level, enter ) ;
    } // else

  } // else

} // Statement::PrintLisp()


// ------------------------------------------------------------NEW-------------------------------------------
void Statement :: Exe_S_EXP( TreeNode* inputPtr, TreeNode* outputPtr ) {
/*
這邊比較複雜 我寫多一點註解以免大家搞不懂這裡的構想 每次看到這裡都覺得寫這個的人是個天才
我們在做這部分之前就會先建好一個 tree了 但是這個樹只是一個初步的雛型 我們要先對他進行預處理
最後我們要使用的是outputPtr那個pointer 由於他已經是指向記憶體位置 因此我們不用加"&"號
*/  
  if ( inputPtr->m_type == LISP ) {

  }  // if 如果inputPtr指向的是一個LISP 那它代表的就是"("這東西 因此我們要進入ReadLEFT來確認他新的pointer 我給你的東西
     // 一樣是一個vector 那邊不會變 所以你可以放心地寫 不用怕
  else if ( inputPtr->m_type == TOKEN ) {

  } // else if 在他是一個TOKEN的情況下 我們要先知道她有沒有被宣告過 有一個全域變數會專門處理這件事情 我很高機率會把這東西
    // 名字設定為 g_def_object ( 取名由來為: 全域的 定義的 物件 ( 這裡處理的其實不算是變數 比較像是物件 ) ),
    // 那這邊應該會要有一個funciton 是一個 Get_DefObject_Ptr( 取名由來: 得到 定義的 物件的 指向 ) 他回傳的會是一個TreeNode *
    // 所以說 我們還會有一個struct 名為 Object 他裡面會放兩種東西 一個是該物件的名稱 我們用string 
    // 一個是他所指向的記憶體位置TreeNode * 那這樣這個Get_DefObject_ptr 就可以工作了 這樣就不用擔心 定義的部分該怎麼辦
    // 差不多在370行左右的地方
  else{
    cout << "Type error please Check" << endl ;
    throw ( "TYPE Error" ) ;
  } // else 這邊正常來說是不可能進入的 如果進入 一定是前面有寫錯 要回去看!
} // Statement::Exe_S_EXP()
// ------------------------------------------------------------NEW-------------------------------------------


void Statement :: PrintStatementResult( vector<Token> &wait_token_vector ) {

  bool enter = false ;  
  if (  wait_token_vector.front().m_type == LEFT_PAREN ) {
    m_nowTreePtr->m_left = new TreeNode() ;
    m_nowTreePtr->m_left_ok = false ;
    m_nowTreePtr->m_type = LISP ;
    wait_token_vector.erase( wait_token_vector.begin() ) ;
    CreateLisp( m_nowTreePtr->m_left, wait_token_vector, false ) ;
    /*
    這邊要開始對statement做計算
    */
  } // if
  else if ( wait_token_vector.size() == 1 ) {
    Token* tempToken = new Token( wait_token_vector.front().m_token_string, 
                                  wait_token_vector.front().m_colnum,
                                  wait_token_vector.front().m_line,
                                  wait_token_vector.front().m_type ) ;
    m_nowTreePtr->AddLeft( tempToken ) ;
    m_nowTreePtr->m_left_ok = false ;
    m_nowTreePtr->m_type = TOKEN ;
    wait_token_vector.erase( wait_token_vector.begin() ) ;
  } // else if
  
  PrintLisp( m_nowTreePtr, 0, enter ) ;  


} // Statement::PrintStatementResult()

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
        PrintStatementResult( wait_token_vector ) ; 
        m_pl_tokenGetter.Reload() ;
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

  } // try
  catch( exception &e ) {
    if ( strcmp( e.what(), "EOF error" ) == 0 ) {
      throw invalid_argument( e.what() ) ;
    } // if
    else if ( strcmp( e.what(), "Normal Exit" ) == 0 ) {
      cout << endl ;
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
  LoadPreDefFunction() ;
  char testNum[10] ;
  cin.getline( testNum, 10 ) ;
  GetTokenMachine getToken ;
  TokenClassCategory tokenclass ;
  Statement statement( getToken, tokenclass ) ;
  statement.PrintAllOfStatement() ;


} // main()
