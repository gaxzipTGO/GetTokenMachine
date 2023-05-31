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
# include <cmath>
# include <sstream>
# include <cstdlib>
//    // ***************************************************************************** //
//    //                                                                               //
//    //                               開始 project 3                                  //
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

const char* g_ARUGMENTERROR = "Argument Error" ;

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
  BOOL = 9876,
  FUNCTION = 21555
};

/*
  這堆東西是一開始就要用的字串
  每個都有特定的意義
  如果遇到這些東西 就要確認他們的狀態了
*/

int g_level = 0 ;

string To_string( int value ) {
  stringstream ss ;
  ss << value ;
  return ss.str() ;
} // To_string()

string To_string( double value ) {
  stringstream ss ;
  ss << value ;
  return ss.str() ;
} // To_string()

void ErrorSymBol( string object ) {
  cout << "ERROR (unbound symbol) : " << object << endl ;
  if ( 1 ) throw invalid_argument( "Type Error" ) ;
} // ErrorSymBol() 

void ReadExit() {
  if ( 1 ) throw invalid_argument( "Normal Exit" ) ;
} // ReadExit()

void NoReturnError() {
  if ( 1 ) throw invalid_argument( "No Return" ) ;
} // NoReturnError()

void ArgumentError( string function_name ) {
  cout << "ERROR (incorrect number of arguments) : " << function_name << endl ;
  if ( 1 ) throw invalid_argument( "Argument Error" ) ;
} // ArgumentError()



void ErrorFormat( string function ) {
  cout << "ERROR (" << function << " format) : " ; 
  if ( 1 ) throw invalid_argument( "Format Error" ) ;
} // ErrorFormat()

void ErrorLevel( string function ) {
  cout << "ERROR (level of " << function << ")" << endl ; 
  if ( 1 ) throw invalid_argument( "Level Error" ) ;
} // ErrorLevel()

class Token {
  public : string m_token_string ;
  public : int m_colnum ;
  public : int m_line ;
  public : int m_type ;
  public : bool m_dont_exp ;

  public : Token() {
    m_token_string = "" ;
    m_colnum = 0 ;
    m_line = 0 ;
    m_type = 0 ;
    m_dont_exp = false ;
  } // Token()

  public : Token( string str, int colnum, int line, int type ) {

    m_token_string = str ;
    m_colnum = colnum ;
    m_line = line ;
    m_type = type ;
    m_dont_exp = false ;

  } // Token()
} 
;

// ------------------------------------------------------------NEW-------------------------------------------

class Func {
  public :
  string m_function_name ;
  string m_operater ;
  int m_argument ;
  Func() {
    m_function_name = "" ;
    m_operater = "" ;
    m_argument = -9999 ;
  } // Func()

  Func( string name, string op, int argv ) {
    m_function_name = name ;
    m_operater = op ;
    m_argument = argv ;
  } // Func()

}
;

typedef Func Function ; 

Function SetFunctionStruct( string function_name,  string operater, int argument ) {
  Function temp( function_name, operater, argument ) ;
  return temp ;
} // SetFunctionStruct()

vector<Function> g_predef_function ;
vector<Function> g_custom_function ;

bool FunctionNULL( Function function_1 ) {
  return function_1.m_function_name == "" && 
         function_1.m_argument == -9999 &&
         function_1.m_operater == "" ; 
} // FunctionNULL()

bool FunctionEqual( Function function_1, Function function_2 ) {
  return function_1.m_function_name == function_2.m_function_name && 
         function_1.m_argument == function_2.m_argument &&
         function_1.m_operater == function_2.m_operater ;
} // FunctionEqual()

void LoadPreDefFunction() {
  // Constructors
  g_predef_function.push_back( SetFunctionStruct( "cons", "==", 2 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "list", ">=", 0 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "quote", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "'", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "define", "==", 2 ) ) ;
  // Part accessors
  g_predef_function.push_back( SetFunctionStruct( "car", "==", 1 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "cdr", "==", 1 ) ) ;
  // Primitive predicates (all functions below can only take 1 m_argument)
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

  // 這不知道為什麼進無窮欸
  g_predef_function.push_back( SetFunctionStruct( "let", ">=", 0 ) ) ;
  g_predef_function.push_back( SetFunctionStruct( "lambda", ">=", 0 ) ) ;
  // g_predef_function.push_back( temp ) ;
  // Bypassing the default evaluation

  // The binding of a symbol to an S-expression
} // LoadPreDefFunction()

void LoadPreDefFunctionTwo() {


} // LoadPreDefFunctionTwo()

bool SearchFromFunction( vector<Function> function_vector, string symbol ) {
    // 在vector中查找特定符號
  for ( vector<Function>::iterator it = function_vector.begin() ; it != function_vector.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return true;
    } // if
  } // for

  // 沒有找到該符號
  
  return false ;
} // SearchFromFunction()


Function* GetFromFunction_PTR( string symbol ) {
  // 在vector中查找特定符號
  for ( vector<Function>::iterator it = g_predef_function.begin() ; it != g_predef_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return new Func( it->m_function_name, it->m_operater, it->m_argument ) ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; it != g_custom_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return new Func( it->m_function_name, it->m_operater, it->m_argument ) ;
    } // if
  } // for

  // 沒有找到該符號
  // cout << "ERROR (unbound symbol) : " << symbol << endl ;
  throw invalid_argument( "unbound symbol" ) ;

} // GetFromFunction_PTR()

int Getm_argument( string symbol ) {
    // 在vector中查找特定符號
  for ( vector<Function>::iterator it = g_predef_function.begin() ; 
        it != g_predef_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return it->m_argument ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; 
        it != g_custom_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return it->m_argument ;
    } // if
  } // for

  // 沒有找到該符號，返回預設值0
  return 0;
} // Getm_argument()

class TreeNode {
  public:
  Token* m_left_token ;
  Token* m_right_token ;
  TreeNode* m_left ;
  TreeNode* m_right ;
  int m_type ;
  int m_right_type ;
  bool m_left_ok ;
  bool m_right_ok ;
  bool m_can_read ;
  bool m_dont_exp ;

  TreeNode() {
    m_type = 0 ;
    m_right_type = 0 ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = NULL ;
    m_right_token = NULL ;
    m_left_ok = true ;
    m_can_read = true ;
    m_dont_exp = false ;
  } // TreeNode()

  TreeNode( bool dontread ) {
    m_type = 0 ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = NULL ;
    m_right_token = NULL ;
    m_left_ok = true ;
    m_can_read = dontread ;
    m_dont_exp = false ;
  } // TreeNode()

  TreeNode( int type ) {
    m_type = type ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = NULL ;
    m_right_token = NULL ;
    m_dont_exp = false ;
    if ( type == T ) {
      m_type = TOKEN ;
      m_left_token = new Token( "#t", 0, 0, T ) ;
    } // if
    else if ( type == NIL ) {
      m_type = TOKEN ;
      m_left_token = new Token( "nil", 0, 0, NIL ) ;
    } // if
  } // TreeNode()

  TreeNode( Token* token ) {

    m_type = TOKEN ;
    m_left = NULL ;
    m_right = NULL ;
    m_left_token = token ;
    m_right_token = NULL ;
    m_dont_exp = false ;
    m_can_read = true ;

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
    if ( m_left == NULL && m_right == NULL && m_left_token == NULL && m_right_token == NULL ) {
      return true ;
    } // if
    else if ( m_type == NIL ) return true ; 
    return false ;
  } // IsNIL()
} // TreeNode
;

struct Object {
  string object_name ;
  TreeNode* object_ptr ;
  Function* object_function ;
  int type ;
}
;

vector<Object> g_def_object ;
stack< vector<Object> > g_stack_object ;

bool IsFunction( string symbol ) {

  for ( vector<Object>::iterator it = g_def_object.begin() ; it != g_def_object.end() ; ++it ) {
    if ( it->object_name == symbol ) {
      if ( it->type == FUNCTION ) {
        return true ;
      } // if
    } // if
  } // for

  for ( vector<Function>::iterator it = g_predef_function.begin() ; it != g_predef_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return true ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; it != g_custom_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return true ;
    } // if
  } // for

  return false ;
} // IsFunction()

Function GetFromFunction( string symbol ) {
  // 在vector中查找特定符號
  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == symbol ) {
          return *it->object_function ;
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; it != g_def_object.end() ; ++it ) {
    if ( it->object_name == symbol ) {
      if ( it->type == FUNCTION ) {
        return *it->object_function ;
      } // if
    } // if
  } // for

  for ( vector<Function>::iterator it = g_custom_function.begin() ; it != g_custom_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return *it ;
    } // if
  } // for

  for ( vector<Function>::iterator it = g_predef_function.begin() ; it != g_predef_function.end() ; ++it ) {
    if ( it->m_function_name == symbol ) {
      return *it ;
    } // if
  } // for



  // 沒有找到該符號
  // cout << "ERROR (unbound symbol) : " << symbol << endl ;
  throw invalid_argument( "Function Error" ) ;

} // GetFromFunction()

int CountRightNodes( TreeNode* root ) {
  if ( root == NULL ) {
    return 0 ;
  } // if

  int count = 0 ;
  if ( root->m_right ) {
    if ( root->m_right->m_type == TOKEN ) {
      if (  root->m_right->m_left_token->m_type != QUOTE ) {
        count ++ ;
      } // if
    } // if
    else {
      count ++ ;
    } // else
  } // if
  
  count += CountRightNodes( root->m_right ) ;
  return count ;
} // CountRightNodes()

bool ComPare( string str1, string str2, string op ) {
  double num1, num2 ;
  stringstream ss1( str1 ) ;
  ss1 >> num1 ;
  stringstream ss2( str2 ) ;
  ss2 >> num2 ;

  if ( op == "==" ) {
    return num1 == num2 ;
  } // if 
  else if ( op == "!=" ) {
    return num1 != num2 ;
  } // else if
  else if ( op == "<" ) {
    return num1 < num2 ;
  } // else if
  else if ( op == "<=" ) {
    return num1 <= num2 ;
  } // else if
  else if ( op == ">" ) {
    return num1 > num2 ;
  } // else if
  else if ( op == ">=" ) {
    return num1 >= num2 ;
  } // else if
  else {
    cout << "Invalid operator!" << endl ;
    return false ;
  } // else 
} // ComPare()

bool ComPareString( string str1, string str2, string op ) {
  int result = strcmp( str1.c_str(), str2.c_str() ) ;
  if ( op == "string=?" ) {
    return result == 0 ;
  } // if 
  else if ( op == "string>?" ) {
    return result > 0 ;
  } // else if
  else if ( op == "string<?" ) {
    return result < 0 ;
  } // else if
  else {
    cout << "Invalid operator!" << endl ;
    return false ;
  } // else 
} // ComPareString()


int GetDefinedType( string object ) ;

bool CompareTwoTokenNode( Token* token1, Token* token2 ) {

  if ( token1 && token2 && token1->m_token_string == token2->m_token_string ) {
    if ( GetDefinedType( token1->m_token_string ) == FUNCTION ) {
      if ( token1->m_dont_exp != token2->m_dont_exp ) {
        return false ;
      } // if
    } // if

    return true ;
  } // if
  else if ( token1 == token2 ) {
    return true ;
  } // else if

  return false ;

} // CompareTwoTokenNode()

bool CompareTwoTreeNode( TreeNode* inputPtr_1, TreeNode* inputPtr_2 ) {

  if ( ! inputPtr_1 && ! inputPtr_2 ) {
    return true ;
  } // if

  if ( inputPtr_1 && inputPtr_2 && ( inputPtr_1->m_type == inputPtr_2->m_type ) ) {

    if ( CompareTwoTokenNode( inputPtr_1->m_left_token, inputPtr_2->m_left_token ) &&
         CompareTwoTokenNode( inputPtr_1->m_right_token, inputPtr_2->m_right_token ) && 
         CompareTwoTreeNode( inputPtr_1->m_left, inputPtr_2->m_left ) && 
         CompareTwoTreeNode( inputPtr_1->m_right, inputPtr_2->m_right ) ) {
      return true ;
    } // if
  } // if
  else {
    return false ;
  } // else

  return false ;
} // CompareTwoTreeNode()

bool CheckRightNodeCount( TreeNode* now_TreePtr, int m_argument, const string& m_operater ) {
  int right_node_count = CountRightNodes( now_TreePtr );
  if ( m_operater == "<" ) {
    return right_node_count < m_argument;
  } // if
  else if ( m_operater == ">" ) {
    return right_node_count > m_argument;
  } // else if
  else if ( m_operater == "<=" ) {
    return right_node_count <= m_argument;
  } // else if
  else if ( m_operater == ">=" ) {
    return right_node_count >= m_argument;
  } // else if
  else if ( m_operater == "==" ) {
    return right_node_count == m_argument;
  } // else if
  else {
    // 非法的 m_operater 符號，可以選擇回傳 false 或拋出異常等處理方式
    throw invalid_argument( "incorrect number of arguments" ) ;
  } // else
} // CheckRightNodeCount()

bool CheckNodeValidity( TreeNode* now_ptr ) {
  string symbol = now_ptr->m_left_token->m_token_string ;
  // 如果符號不在vector中，拋出錯誤
  Function function = GetFromFunction( symbol ) ;
  int m_argument = function.m_argument ;
  string m_operater = function.m_operater ;
  if ( CheckRightNodeCount( now_ptr, m_argument, m_operater ) ) {
    return true ;
  } // if

  if ( function.m_function_name == "if" ) {
    if ( CheckRightNodeCount( now_ptr, 2, m_operater ) ) {
      return true ;
    } // if

    if ( CheckRightNodeCount( now_ptr, 3, m_operater ) ) {
      return true ;
    } // if
  } // if

  return false ;
} // CheckNodeValidity()

bool IsAtomType( int type ) {
  if ( type == SYMBOL || type == INT || type == FLOAT || type == STRING || type == NIL || type == T ) {
    return true ;
  } // if
  else if ( type == LEFT_PAREN || type == RIGHT_PAREN ) {
    return true ;
  } // else if
  else return false ;
} // IsAtomType()

void Function_CheckAtom( vector<Token> &input_token, vector<Token> &output_token ) {
/*
進來的一定都是通過文法的!!
*/
  Token null = Token( "NIL", 0, 0, NIL )  ;
  int num = input_token.size() ;

  if ( num == 1 ) { // 參數簡單又正確
    if ( IsAtomType( input_token.at( 0 ).m_type ) ) {
      output_token.push_back( input_token.at( 0 ) ) ;
    } // if
    else output_token.push_back( null ) ;
  } // if
  else if ( num == 2 ) { // 參數數量為2 可能是()
    if ( input_token.at( 0 ).m_type == LEFT_PAREN && input_token.at( 1 ).m_type == RIGHT_PAREN ) {
      output_token.push_back( input_token.at( 0 ) ) ;
      output_token.push_back( input_token.at( 1 ) ) ;
    } // if
    else ;
      // PRINT ERROR
  } // else if 
  else { // 參數數量爆多qq

  } // else
} // Function_CheckAtom()

void Function_CheckPair( vector<Token> &input_token, vector<Token> &output_token ) {
  Token null = Token( "NIL", 0, 0, NIL )  ;
  int num = input_token.size() ;

  if ( num == 1 ) { // 只有一個參數絕對錯 掰掰!
    output_token.push_back( null ) ;
  } // if
  else {

  } // else

} // Function_CheckPair()

void Function_CheckList( vector<Token> &input_token, vector<Token> &output_token ) {
  Token null = Token( "NIL", 0, 0, NIL )  ;
  vector<Token> temp ;
  temp.reserve( 100 ) ;
  for ( int i = 0 ; i < temp.size() ; i++ ) { // 只要list中有非ATOM的就一定不是LIST (應該是吧?)
    if ( IsAtomType( temp.at( i ).m_type == false ) ) {
      output_token.push_back( null ) ;
      return ;
    } // if
  } // for
    
    // '(3 4)
  if ( input_token.at( 0 ).m_type == QUOTE && input_token.at( 1 ).m_type == LEFT_PAREN ) {
    temp.assign( input_token.begin()+2, input_token.end()-1 ) ; // 除了最後一個屬於上一層的右括不要留
    // 剩下的理論上是list內的數字或'( (3 4) 5 ) 這種
    
  } // if
} // Function_CheckList()


Token* CopyToken( Token* inputToken ) {
  if ( inputToken != NULL ) {
    Token* temp =  new Token( inputToken->m_token_string, inputToken->m_colnum,
                              inputToken->m_line, inputToken->m_type ) ;
    temp->m_dont_exp = inputToken->m_dont_exp ;
    return temp ;
  } // if

  return NULL ;
} // CopyToken()

TreeNode* CopyObject( TreeNode* inputPtr ) {

  if ( inputPtr == NULL ) {
    return NULL ;
  } // if
  else {
    TreeNode* copy_ptr = new TreeNode() ;
    copy_ptr->m_left_ok = inputPtr->m_left_ok ;
    copy_ptr->m_right_ok = inputPtr->m_right_ok ;
    copy_ptr->m_can_read = inputPtr->m_can_read ;
    copy_ptr->m_type = inputPtr->m_type ;
    copy_ptr->m_right_type = inputPtr->m_right_type ;
    copy_ptr->m_dont_exp = inputPtr->m_dont_exp ;

    copy_ptr->m_left = CopyObject( inputPtr->m_left ) ;
    copy_ptr->m_right = CopyObject( inputPtr->m_right ) ;
    copy_ptr->m_left_token = CopyToken( inputPtr->m_left_token ) ;
    copy_ptr->m_right_token = CopyToken( inputPtr->m_right_token ) ;
    return copy_ptr ;
  } // else

} // CopyObject()

TreeNode* Get_DefObject_Ptr( string Object_name ) ;
TreeNode* Get_RealObject_Ptr( string object_name ) ;
Function* Get_DefObject_Function( string object_name ) ;
Object GetDefObject( string object_name ) ;
void DefineObject( Object object ) ;


TreeNode* Get_RealObject_Ptr( string object_name ) {


  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      return it->object_ptr ;
    } // if
  } // for

  throw invalid_argument( "no define" ) ;
} // Get_RealObject_Ptr()

Function* Get_DefObject_Function( string object_name ) {
  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object_name ) {
          return it->object_function ;
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      return it->object_function ;
    } // if
  } // for

  throw invalid_argument( "no define" ) ;
} // Get_DefObject_Function()

int Get_DefObject_type( string object_name ) {
  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object_name ) {
          return it->type ;
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      return it->type ;
    } // if
  } // for

  throw invalid_argument( "no define" ) ;
} // Get_DefObject_type()

Object GetDefObject( string object_name ) {

  Object temp_object ;
  temp_object.object_name = object_name ;
  temp_object.object_ptr = Get_DefObject_Ptr( object_name ) ;
  temp_object.object_function = Get_DefObject_Function( object_name ) ;
  temp_object.type = Get_DefObject_type( object_name ) ;
  return temp_object ;

} // GetDefObject()


Object GetObject( string object_name ) {

  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object_name ) {
          return *it ;
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      return *it ;
    } // if
  } // for

  throw invalid_argument( "no define" ) ;
} // GetObject()

void DefineObject( Object object ) {

  try {
    for ( vector<Object>::iterator it = g_def_object.begin() ; 
          it != g_def_object.end() ; ++it ) {
      if ( it->object_name == object.object_name ) {
        it->type = object.type ;
        it->object_ptr = NULL ;
        it->object_function = NULL ;
        it->object_ptr = object.object_ptr ;
        it->object_function = object.object_function ;
        if ( 1 ) throw invalid_argument( "defined" ) ;
      } // if
    } // for

    if ( 1 ) throw invalid_argument( "no define" ) ;
  } // try
  catch ( exception &e ) {
    if ( strcmp( e.what(), "no define" ) == 0 )
      g_def_object.push_back( object ) ;
  } // catch 

} // DefineObject()

bool IsDefinedOrNot( string object ) {

  if ( IsFunction( object ) )
    return true ;
  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object ) {
          return true ;
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object ) {
      return true ;
    } // if
  } // for

  return false ;
} // IsDefinedOrNot()

int GetDefinedType( string object ) {


  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object ) {
          return SYMBOL ;
        } // if
      } // for
    } // for
  } // if
  
  if ( g_def_object.size() != 0 ) {
    for ( vector<Object>::iterator it = g_def_object.begin() ; 
          it != g_def_object.end() ; ++it ) {
      if ( it->object_name == object ) {
        return SYMBOL ;
      } // if
    } // for
  } // if

  if ( g_custom_function.size() != 0 ) {
    for ( vector<Function>::iterator it = g_custom_function.begin() ; 
          it != g_custom_function.end() ; ++it ) {
      if ( it->m_function_name == object ) {
        return FUNCTION ;
      } // if
    } // for
  } // if

  if ( g_predef_function.size() != 0 ) {
    for ( vector<Function>::iterator it = g_predef_function.begin() ; 
          it != g_predef_function.end() ; ++it ) {
      if ( it->m_function_name == object ) {
        return FUNCTION ;
      } // if
    } // for
  } // if

  return 0 ;

} // GetDefinedType()

TreeNode* Get_DefObject_Ptr( string object_name ) {

  if ( IsDefinedOrNot( object_name ) ) {
    for ( vector<Function>::iterator it = g_custom_function.begin() ; 
          it != g_custom_function.end() ; ++it ) {
      if ( it->m_function_name == object_name ) {
        Token* token = new Token( it->m_function_name, 0, 0, SYMBOL ) ;
        return new TreeNode( token ) ;
      } // if
    } // for
      
    for ( vector<Function>::iterator it = g_predef_function.begin() ; 
          it != g_predef_function.end() ; ++it ) {
      if ( it->m_function_name == object_name ) {
        Token* token = new Token( it->m_function_name, 0, 0, SYMBOL ) ;
        return new TreeNode( token ) ;
      } // if
    } // for
  } // if


  stack< vector<Object> > temp_stack = g_stack_object ;
  if ( temp_stack.empty() == false ) {
    for ( ; ! temp_stack.empty() ; temp_stack.pop() ) {
      for ( vector<Object>::iterator it = temp_stack.top().begin() ; 
            it != temp_stack.top().end() ; ++it ) {
        if ( it->object_name == object_name ) {
          if ( it->type == SYMBOL )
            return CopyObject( it->object_ptr ) ;
          else {
            Token* token = new Token( it->object_function->m_function_name, 0, 0, SYMBOL ) ;
            return new TreeNode( token ) ;
          } // else
        } // if
      } // for
    } // for
  } // if

  for ( vector<Object>::iterator it = g_def_object.begin() ; 
        it != g_def_object.end() ; ++it ) {
    if ( it->object_name == object_name ) {
      if ( it->type == SYMBOL )
        return CopyObject( it->object_ptr ) ;
      else {
        Token* token = new Token( it->object_function->m_function_name, 0, 0, SYMBOL ) ;
        return new TreeNode( token ) ;
      } // else
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

  protected: virtual bool IsIntMissm_operater( string token, bool noSize ) {
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

  } // IsIntMissm_operater()

  protected: virtual bool IsIntMissm_operater( string token ) {
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

  } // IsIntMissm_operater()

  protected: virtual bool IsFloat( string token ) {
    size_t find = token.find( '.' ) ;
    if ( token.npos != find ) {
      string sub_string_head = token.substr( 0, find ) ;
      string sub_string_tail = token.substr( find+1, token.size()-find ) ;
      if ( ( sub_string_head == "+" || sub_string_head == "-" ||  
             IsIntMissm_operater( sub_string_head, false ) ) && IsInt( sub_string_tail, false ) ) {
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

  protected: virtual string DealWithm_operater( string token ) {
  /*
    看第一項是不是加號 如果是的話並且後面是數字的話把他給清除
  */
    if ( token.at( 0 ) == '+' ) {
      if ( token.at( 1 ) >= '0' && token.at( 1 ) <= '9' ) {
        token.erase( 0, 1 ) ;
      } // if
    } // if

    return token ;
  } // DealWithm_operater()

  public: virtual string DealWithDot_Back( string token ) {
    bool int_is = true ;
    for ( int i = 0 ; i < token.size() && int_is == true ; i ++ ) {
      if ( token.at( i ) == '.' ) {
        string sub_token_head = token.substr( 0, i ) ;
        string sub_token_tail = token.substr( i+1, token.size() - i ) ;
        if ( ! IsIntMissm_operater( sub_token_head ) ) {
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

  protected: string RoundFinal( string token ) {

    for ( int i = token.size()-1 ; i > 0 ; i -- ) {
      if ( token[i] == ':' ) {
        token[i] = '0' ;
        if ( token[i-1] == '.' )
          i -- ;
        if ( token[i-1] == '-' )
          token.insert( token.begin()+1, '1' ) ;
        else {
          token[i-1] = token[i-1] + 1 ;
        } // else
      } // if
    } // for


    if ( token[0] == ':' ) {
      token[0] = '0' ;
      token.insert( token.begin(), '1' ) ;
    } // if

    return token ;
  } // RoundFinal()

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
    token = DealWithm_operater( token ) ;  // 如果是+就把+刪掉 其餘不動
    token = RoundFinal( token ) ;
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
    else if ( IsIntMissm_operater( token ) && 
              ( token != "+" && token != "-" && token != "*" && token != "/" ) ) { // 只有純數字的int ex: 123 456
      return INT ;
    } // else if
    else if ( token == "+" || token == "-" || token == "*" || token == "/" ) {
      return SYMBOL ;
    } // else if
    else if ( token.at( 0 ) == '+' || token.at( 0 ) == '-'  ) { // +123 -456
      if ( IsIntMissm_operater( token ) ) { 
        return INT ;
      } // if
      else return SYMBOL ;
    } // else if
    else if ( CheckSymbol( token ) ) {
      return SYMBOL ;
    } // else if
    else if ( token.at( 0 ) == '\"' && token.at( token.size()-1 ) == '\"' ) {
      return STRING ;
    } // else if
    else {
      return SYMBOL ;
    } // else
  } // GetThisTokenType() 
   
} 
g_classCategory ;

double StringToFloat( string str ) {
  double f ;
  stringstream ss( str ) ;
  ss >> f ;
  return f ;
} // StringToFloat()

void ConvertString( string s1, string s2, int &i1, int &i2, double &f1, double &f2, bool &b1, bool &b2 ) {
  if ( s1.npos == s1.find( "." ) ) 
    b1 = true ;

  if ( s2.npos == s2.find( "." ) ) 
    b2 = true ;

  f1 = StringToFloat( s1 ) ;
  f2 = StringToFloat( s2 ) ;
  i1 = atoi( s1.c_str() ) ;
  i2 = atoi( s2.c_str() ) ;
} // ConvertString()

string AddNum( string s1, string s2, int &type ) {
  TokenClassCategory tokenClass ;
  bool first_is_int = false ;
  bool second_is_int = false ;
  int first_int = 0 ;
  int second_int = 0 ;
  double first_float = 0.0 ;
  double second_float = 0.0 ;

  ConvertString( s1, s2, first_int, second_int, first_float, second_float, first_is_int, second_is_int ) ;
  if ( first_is_int && second_is_int ) { // both int
    type = INT ;
    int sum = first_int + second_int ;
    string temp = To_string( sum ) ;
    return temp ;
  } // if both int
  else {
    type = FLOAT ;
    double sum = first_float + second_float ;
    string temp = To_string( sum ) ;
    if ( tokenClass.GetThisTokenType( temp ) == INT )
      temp = temp + string( ".", 1 ) ;
    return tokenClass.DealWithDot_Back( temp ) ; // 我想直接call那個四捨五入的function ~ ~
  } // else
} // AddNum() 

string MinusNum( string s1, string s2, int &type ) {
  TokenClassCategory tokenClass ;
  bool first_is_int = false ;
  bool second_is_int = false ;
  int first_int = 0 ;
  int second_int = 0 ;
  double first_float = 0.0 ;
  double second_float = 0.0 ;

  ConvertString( s1, s2, first_int, second_int, first_float, second_float, first_is_int, second_is_int ) ;
  if ( first_is_int && second_is_int ) { // both int
    type = INT ;
    int sum = first_int - second_int ;
    string temp = To_string( sum ) ;
    return temp ;
  } // if both int
  else {
    type = FLOAT ;
    double sum = first_float - second_float ;
    string temp = To_string( sum ) ;
    if ( tokenClass.GetThisTokenType( temp ) == INT )
      temp = temp + string( ".", 1 ) ;
    return tokenClass.DealWithDot_Back( temp ) ; // 我想直接call那個四捨五入的function ~ ~
  } // else

} // MinusNum() 

string MultiplyNum( string s1, string s2, int &type ) {
  TokenClassCategory tokenClass ;
  bool first_is_int = false ;
  bool second_is_int = false ;
  int first_int = 0 ;
  int second_int = 0 ;
  double first_float = 0.0 ;
  double second_float = 0.0 ;

  ConvertString( s1, s2, first_int, second_int, first_float, second_float, first_is_int, second_is_int ) ;

  if ( first_is_int && second_is_int ) { // both int
    type = INT ;
    int sum = first_int * second_int ;
    string temp = To_string( sum ) ;
    return temp ;
  } // if both int
  else {
    type = FLOAT ;
    double sum = first_float * second_float ;
    string temp = To_string( sum ) ;
    if ( tokenClass.GetThisTokenType( temp ) == INT )
      temp = temp + string( ".", 1 ) ;
    return tokenClass.DealWithDot_Back( temp ) ; // 我想直接call那個四捨五入的function ~ ~
  } // else

} // MultiplyNum() 

string DividedNum( string s1, string s2, int &type ) {
  TokenClassCategory tokenClass ;
  bool first_is_int = false ;
  bool second_is_int = false ;
  int first_int = 0 ;
  int second_int = 0 ;
  double first_float = 0.0 ;
  double second_float = 0.0 ;

  ConvertString( s1, s2, first_int, second_int, first_float, second_float, first_is_int, second_is_int ) ;
  try {
    if ( first_is_int && second_is_int ) { // both int
      type = INT ;
      if ( first_int == 0 || second_int == 0 ) throw invalid_argument( "division zero" ) ;
      int sum = first_int / second_int ;
      string temp = To_string( sum ) ;
      return temp ;
    } // if both int
    else {
      type = FLOAT ;
      if ( first_float == 0 || second_float == 0 ) throw invalid_argument( "division zero" ) ;
      double sum = first_float / second_float ;
      string temp = To_string( sum ) ;
      if ( tokenClass.GetThisTokenType( temp ) == INT )
        temp = temp + string( ".", 1 ) ;
      return tokenClass.DealWithDot_Back( temp ) ; // 我想直接call那個四捨五入的function ~ ~
    } // else
  } // try
  catch( exception &e ) {
    cout << "ERROR (division by zero) : /" << endl ;
    throw invalid_argument( "Division zero" ) ;
  } // catch

} // DividedNum() 

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
      cout << endl << "> " ;
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

void CheckEXP( Token* inputToken ) {

  if ( inputToken && inputToken->m_dont_exp == false ) {
    if ( inputToken->m_type == SYMBOL ||
         inputToken->m_token_string == "+" ||
         inputToken->m_token_string == "-" || 
         inputToken->m_token_string == "*" ||
         inputToken->m_token_string == "/" ) {
      if ( IsDefinedOrNot( inputToken->m_token_string ) ) {
        int type = GetDefinedType( inputToken->m_token_string ) ;
        if ( type == FUNCTION ) {
          if ( inputToken->m_token_string == "new lambda" ) inputToken->m_token_string = "lambda" ;
          inputToken->m_token_string = "#<procedure " + inputToken->m_token_string + ">" ;
        } // if
      } // if
      else {
        ErrorSymBol( inputToken->m_token_string ) ;
      } // else
    } // if
  } // if

} // CheckEXP()

void CheckEXP( TreeNode* inputPtr ) {

  if ( inputPtr ) {
    CheckEXP( inputPtr->m_left_token ) ;
    CheckEXP( inputPtr->m_right_token ) ;

    if ( ! inputPtr->m_dont_exp )
      CheckEXP( inputPtr->m_left ) ;
    CheckEXP( inputPtr->m_right ) ;
  } // if


} // CheckEXP()

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
  protected : TreeNode* Exe_S_EXP( TreeNode* inputPtr ) ;

  public : void PrintAllOfStatement() ;
  public : void PrintLisp( TreeNode* now_TreePtr, int level, bool &enter ) ;

  public : Statement() {
    m_nextToken = "" ;
    m_nowTreePtr = new TreeNode( false ) ;
  } // Statement()

  public : Statement( GetTokenMachine token_get, TokenClassCategory token_category ) {
    m_not_end = true ;
    m_pl_tokenGetter = token_get ;
    m_tokenCategorier = token_category ;
    m_nextToken = "" ;
    m_nowTreePtr = new TreeNode( false ) ;
  } // Statement()

  public : void ErrorType( string function, TreeNode* inputPtr ) {
    cout << "ERROR (" << function << " with incorrect argument type) : " ;
    bool enter ;
    inputPtr->m_can_read = false ;
    CheckEXP( inputPtr ) ;
    PrintLisp( inputPtr, 0, enter ) ;
    if ( 1 ) throw invalid_argument( "Type Error" ) ;
  } // ErrorType()

  public : void ErrorFunction( TreeNode* inputPtr ) {

    cout << "ERROR (attempt to apply non-function) : "  ;
    bool enter ;
    if ( inputPtr->m_right_token ) {
      TreeNode* newPtr = new TreeNode( false ) ;
      newPtr->m_type = LISP ;
      newPtr->m_left = inputPtr ;
      inputPtr = newPtr ;
    } // if

    inputPtr->m_right = NULL ;
    inputPtr->m_can_read = false ;
    CheckEXP( inputPtr ) ;
    PrintLisp( inputPtr, 0, enter ) ;
    if ( 1 ) throw invalid_argument( "Non Function" ) ;

  } // ErrorFunction()
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
    Token temp = Token( "(", 0, 0, LEFT_PAREN ) ;
    token_wait_vector.insert( token_wait_vector.end()-1, temp ) ;
    Token temp_token = GetNextToken( token_wait_stack ) ;
    if ( IsS_EXP( temp_token, token_wait_stack, token_wait_vector ) ) {
      Token temp = Token( ")", 0, 0, RIGHT_PAREN ) ;
      token_wait_vector.insert( token_wait_vector.end(), temp ) ;
      return true ;
    } // if
    else {
      ErrorMessage( "atom or '('", token_wait_stack.top().m_line, 
                    token_wait_stack.top().m_colnum,
                    token_wait_stack.top().m_token_string ) ;
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
      if ( wait_token_vector[1].m_type != RIGHT_PAREN ) {
        now_TreePtr->AddLeft( tempNode ) ;
        wait_token_vector.erase( wait_token_vector.begin() ) ;
        CreateLisp( now_TreePtr->m_left, wait_token_vector, false ) ;
        if ( wait_token_vector.size() != 0 ) {
          CreateLisp( now_TreePtr, wait_token_vector, skip_paren ) ;
        } // if 如果往下的都結束了 回到這邊 要確定有沒有東西要放在右邊
      } // if 
      else {
        now_TreePtr->AddLeft( new Token( "nil", 0, 0, NIL ) ) ;
        wait_token_vector.erase( wait_token_vector.begin() ) ;
        wait_token_vector.erase( wait_token_vector.begin() ) ;
        CreateLisp( now_TreePtr, wait_token_vector, false ) ;
      } // else
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
      else if ( wait_token_vector[1].m_type == NIL ) {
        wait_token_vector.erase( wait_token_vector.begin(), wait_token_vector.begin() + 2 ) ;
      } // else if
      else if ( IsAtomType( wait_token_vector[1].m_type )  ) {
        Token* tempToken = 
        new Token( m_tokenCategorier.ChangeToken( wait_token_vector[1].m_token_string ), 
                                                  wait_token_vector[1].m_colnum, 
                                                  wait_token_vector[1].m_line,
                                                  wait_token_vector[1].m_type ) ;
        wait_token_vector.erase( wait_token_vector.begin(), wait_token_vector.begin() + 2 ) ;
        now_TreePtr->m_right_token = tempToken ;
      } // else if
    } // if
    
    if ( wait_token_vector.front().m_type == RIGHT_PAREN ) {
      wait_token_vector.erase( wait_token_vector.begin() ) ;
    } // if 如果是 ")" 
    else {
      if ( now_TreePtr->m_left_ok == true ) {
        Token* tempToken = 
        new Token( m_tokenCategorier.ChangeToken( wait_token_vector.front().m_token_string ), 
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
  if ( level >= 0 ) {
    if ( now_TreePtr != NULL && ( now_TreePtr->IsNIL() ) && now_TreePtr->m_type != NIL ) {
      if ( enter )
        PrintWhiteSpaceWithLevel( level ) ;    
      if ( level == 0 || level == 1 ) {
        cout << "nil" << endl ;
        enter = true ;
      } // if 

      PrintLisp( now_TreePtr->m_right, level-1, enter ) ;

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
          if ( enter )
            PrintWhiteSpaceWithLevel( level ) ;
          cout << "( " ;
          enter = false ;
          PrintLisp( now_TreePtr->m_left, level+1, enter ) ;
        } // else if
        else if ( now_TreePtr->m_type == TOKEN ) {
          if ( enter ) {
            PrintWhiteSpaceWithLevel( level ) ;
            enter = false ;
          } // if

          cout << now_TreePtr->m_left_token->m_token_string << endl ;

          enter = true ;
        } // if
        else if ( now_TreePtr->m_type == NIL ) {
          if ( enter )
            PrintWhiteSpaceWithLevel( level ) ;
          cout << "nil" << endl ;
          enter = true ;          
        } // else if

        if ( now_TreePtr->m_can_read ) {
          if ( now_TreePtr->m_right_token && now_TreePtr->m_right_token->m_type != NIL ) {
            PrintWhiteSpaceWithLevel( level ) ;
            cout << "." << endl ;
            PrintWhiteSpaceWithLevel( level ) ;
            cout << now_TreePtr->m_right_token->m_token_string << endl ;
          } // if
          
          PrintLisp( now_TreePtr->m_right, level, enter ) ;
        } // if
        
      } // else

    } // else
  } // if
} // Statement::PrintLisp()


// ------------------------------------------------------------更新-------------------------------------------
TreeNode* ReadFunction( TreeNode* inputPtr, Function nowFunction ) ;
TreeNode* ReadQuote( TreeNode* inputPtr ) ;
TreeNode* ReadCons( TreeNode* inputPtr, int time ) ;
TreeNode* ReadLeftDefine( TreeNode* inputPtr ) ;
TreeNode* ReadLeft( TreeNode* inputPtr ) ;
TreeNode* ReadList( TreeNode* inputPtr ) ;
TreeNode* ReadDefine( TreeNode* inputPtr ) ;
TreeNode* CheckFinal( TreeNode* inputPtr, bool right ) ;
TreeNode* CreateToken( TreeNode* inputPtr ) ;
TreeNode* ReadCar( TreeNode* inputPtr ) ;
TreeNode* ReadCdr( TreeNode* inputPtr ) ;
TreeNode* ReadATOM( TreeNode* inputPtr ) ;
TreeNode* ReadDefineTwo( TreeNode* inputPtr ) ;
TreeNode* ReadBegin( TreeNode* inputPtr ) ;

TreeNode* CreateToken( Token token ) {
  TokenClassCategory m_token_change ;
  TreeNode* now_TreePtr = new TreeNode( false ) ;
  Token* tempToken = new Token( m_token_change.ChangeToken( token.m_token_string ), 
                                token.m_colnum,
                                token.m_line,
                                token.m_type ) ;
  now_TreePtr->AddLeft( tempToken ) ;
  now_TreePtr->m_left_ok = false ;
  now_TreePtr->m_type = TOKEN ;
  return now_TreePtr ;
}  // CreateToken() 

void ChangeToDontExp( Token* inputToken ) {
  if ( inputToken ) {
    inputToken->m_dont_exp = true ;
  } // if
} // ChangeToDontExp()

void ChangeToDontExp( TreeNode* inputPtr ) {

  if ( inputPtr ) {
    inputPtr->m_dont_exp = true ;
    ChangeToDontExp( inputPtr->m_left ) ;
    ChangeToDontExp( inputPtr->m_right ) ;
    ChangeToDontExp( inputPtr->m_left_token ) ;
    ChangeToDontExp( inputPtr->m_right_token ) ;
  } // if

} // ChangeToDontExp()

TreeNode* ReadQuote( TreeNode* inputPtr ) {

  inputPtr = CopyObject( inputPtr->m_right ) ;
  inputPtr->m_dont_exp = true ;
  if ( inputPtr->m_left )
    ChangeToDontExp( inputPtr->m_left ) ;
  if ( inputPtr->m_left_token )
    ChangeToDontExp( inputPtr->m_left_token ) ;
  if ( inputPtr->m_right_token )
    ChangeToDontExp( inputPtr->m_right_token ) ;
  return inputPtr ;

} // ReadQuote()

TreeNode* ReadCons( TreeNode* inputPtr, int time ) {
  TreeNode* nowPtr = NULL ;
  if ( time == 2 ) {
    return nowPtr ;
  } // if

  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
    if ( nowPtr == NULL ) {
      nowPtr = new TreeNode( NIL ) ;
    } // if 如果沒東西 要自動補給他 不然會出問題
    // 如果他是lisp 要指向他所回傳的pointer
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr = ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if 如果他是quote 那我們就直接指向他所回傳的東西
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) )  { // 在這邊如果沒有定義過 他會直接拋出error
        int type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ;
        if ( type == FUNCTION ) {
          Token* token = new Token( inputPtr->m_left_token->m_token_string, 0, 0, SYMBOL ) ;
          nowPtr = new TreeNode( token ) ;
        } // if 如果他的type是FUNCTION 那就給他做function的處理 
        else if ( type == SYMBOL ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // else if 如果今天是symbol 那我們要做的是把它symbol所指向的東西給接起來 
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else 
    } // else if 如果是symbol的話就要確認有沒有定義過 但這邊我們只管有沒有定義 不管它是甚麼 他是什麼是後面的工作
    else if ( inputPtr->m_left_token->m_type == NIL ) {
      nowPtr = new TreeNode( NIL ) ;
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 如果他什麼都不是 就直接接起來

  } // else 如果他是token的話 先確認有沒有被定義過 如果有定義過 確定她是function還是symbol

  // 進入cons第二次 這次是要直接找下一個的m_left
  if ( time == 0 ) {
    TreeNode* tempNode = ReadCons( inputPtr->m_right, time+1 ) ;
    if ( tempNode != NULL ) {
      if ( tempNode->m_type == LISP ) {
        nowPtr->m_right = tempNode->m_left ;
      } // if
      else {
        nowPtr->m_right = NULL ;
        nowPtr->m_right_token = tempNode->m_left_token ;
        if ( nowPtr->m_right_token && nowPtr->m_right_token->m_type == NIL )
          nowPtr->m_right_token = NULL ;
      } // else 
    } // if
  } // if
  else if ( time == 1 ) {
    if ( nowPtr != NULL )
      nowPtr->m_right = ReadCons( inputPtr, time+1 ) ;
  } // else if

  nowPtr->m_can_read = true ;
  return nowPtr ;

} // ReadCons()

TreeNode* ReadList( TreeNode* inputPtr ) {

  TreeNode* nowPtr = NULL ;
  if ( inputPtr == NULL ) {
    return NULL ;
  } // if

  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
    if ( nowPtr == NULL ) {
      nowPtr = new TreeNode( NIL ) ;
    } // if 如果沒東西 要自動補給他 不然會出問題
    // 如果他是lisp 要指向他所回傳的pointer
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      Function quote = GetFromFunction( inputPtr->m_left_token->m_token_string ) ;
      nowPtr = ReadFunction( inputPtr, quote ) ;
      inputPtr = inputPtr->m_right ;
    } // if 如果他是quote 那我們就直接指向他所回傳的東西
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) { // 在這邊如果沒有定義過 他會直接拋出error
        int type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ;
        if ( type == FUNCTION ) {
          Token* token = new Token( inputPtr->m_left_token->m_token_string, 0, 0, SYMBOL ) ;
          nowPtr = new TreeNode( token ) ;
        } // if 如果他的type是FUNCTION 那就給他做function的處理 
        else if ( type == SYMBOL ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // else if 如果今天是symbol 那我們要做的是把它symbol所指向的東西給接起來 
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else 
    } // else if 如果是symbol的話就要確認有沒有定義過 但這邊我們只管有沒有定義 不管它是甚麼 他是什麼是後面的工作
    else if ( inputPtr->m_left_token->m_type == NIL ) {
      nowPtr = new TreeNode( NIL ) ;
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 如果他什麼都不是 就直接接起來

  } // else 如果他是token的話 先確認有沒有被定義過 如果有定義過 確定她是function還是symbol

  if ( nowPtr != NULL )
    nowPtr->m_right = ReadList( inputPtr->m_right ) ;
  nowPtr->m_can_read = true ;
  return nowPtr ;

} // ReadList()

struct Costum_Function {
  string function_name ;
  TreeNode* function_ptr ;
  vector<TreeNode*> function_argument ;
} ;

stack<Costum_Function> g_stack_lambda ;
vector<Costum_Function> g_costum_def_function ;

TreeNode* ReadDefine( TreeNode* inputPtr ) {

  // 遇到define會有2種狀況 後面是lisp或著是後面是token 後面是token的情況下又有三種狀況
  // symbol notsymbol quote 這三種 要分別處理
  // 基本上 如果有遇到define 要做出一個struct名為object 並且把他放進名為g_def_object的vector裡面
  if ( inputPtr->m_type == TOKEN ) {
    if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      string object_name = inputPtr->m_left_token->m_token_string ;
      if ( GetDefinedType( object_name ) == FUNCTION ) {
        ErrorFormat( "DEFINE" ) ;
      } // if

      Object temp_object ;
      temp_object.object_name = object_name ;
      temp_object.type = SYMBOL ;
      temp_object.object_function = NULL ;
      temp_object.object_ptr = new TreeNode( false ) ;
      inputPtr = inputPtr->m_right ; // 往下一個看
      if ( inputPtr->m_type == LISP ) {
        temp_object.object_ptr->m_type = LISP ;
        temp_object.object_ptr = ReadLeftDefine( inputPtr->m_left ) ;
        if ( CountRightNodes( temp_object.object_ptr ) > 0 ) {
          temp_object.object_function = NULL ;
          DefineObject( temp_object ) ;
          cout << temp_object.object_name << " defined" << endl ;
          throw invalid_argument( "OK" ) ;
        } // if

        if ( temp_object.object_ptr->m_dont_exp == false && temp_object.object_ptr->m_type == TOKEN ) {
          if ( temp_object.object_ptr->m_left_token->m_type == SYMBOL ) {
            if ( IsDefinedOrNot( temp_object.object_ptr->m_left_token->m_token_string ) ) {
              int type = GetDefinedType( temp_object.object_ptr->m_left_token->m_token_string ) ;
              if ( type == SYMBOL ) {
                temp_object = GetObject( temp_object.object_ptr->m_left_token->m_token_string ) ;
                temp_object.object_name = object_name ;
              } // if 
              else if ( type == FUNCTION ) {
                temp_object.object_function = 
                GetFromFunction_PTR( temp_object.object_ptr->m_left_token->m_token_string ) ;
                temp_object.object_ptr = NULL ;
                temp_object.type = FUNCTION ;
              } // else if
            } // if
            else {
              ErrorSymBol( temp_object.object_ptr->m_left_token->m_token_string ) ;
            } // else
          } // if 如果他是symbol 要先確定他有沒有被定義過 如果沒被定義要拋出錯誤 如果有定義了直接連上去就好
        } // if
      } // if 這邊代表他是一個LISP 那就用LISP的標準去看他
      else if ( inputPtr->m_type == TOKEN ) {
        if ( inputPtr->m_left_token->m_type == SYMBOL ) {
          if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
            int type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ;
            if ( type == SYMBOL ) {
              temp_object = GetObject( inputPtr->m_left_token->m_token_string ) ;
              temp_object.object_name = object_name ;
            } // if 
            else if ( type == FUNCTION ) {
              temp_object.object_ptr = NULL ;
              temp_object.object_function = GetFromFunction_PTR( inputPtr->m_left_token->m_token_string ) ;
              temp_object.type = FUNCTION ;
            } // else if
          } // if
          else {
            ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
          } // else
        } // if 如果他是symbol 要先確定他有沒有被定義過 如果沒被定義要拋出錯誤 如果有定義了直接連上去就好
        else if ( inputPtr->m_left_token->m_type == QUOTE ) {
          temp_object.object_ptr->m_type = LISP ;
          temp_object.object_ptr = ReadQuote( inputPtr ) ;
          inputPtr = inputPtr->m_right ;
        } // else if 
        else {
          temp_object.object_ptr = CreateToken( *inputPtr->m_left_token ) ;
        } // else 
      } // else if
      else {
        Statement statement ;
        statement.ErrorType( "define", inputPtr ) ;
      } // else


      DefineObject( temp_object ) ;
      cout << temp_object.object_name << " defined" << endl ;
      throw invalid_argument( "OK" ) ;
    } // if 要先確定第一項是symbol 才可以繼續下去
    else {
      ErrorFormat( "DEFINE" ) ;
    } // else
  } // if
  else if ( inputPtr->m_type == LISP ) {
    ReadDefineTwo( inputPtr ) ;
  } // else if
  else {
    ErrorFormat( "DEFINE" ) ;
  } // else

  return NULL ;
} // ReadDefine()

void FunctionReDefine( string function_name ) {

  for ( int i = 0 ; i < g_costum_def_function.size() ; i ++ ) {
    if ( g_costum_def_function[i].function_name == function_name ) {
      g_costum_def_function.erase( g_costum_def_function.begin()+i ) ;
    } // if
  } // for

  for ( int i = 0 ; i < g_custom_function.size() ; i ++ ) {
    if ( g_custom_function[i].m_function_name == function_name ) {
      g_custom_function.erase( g_custom_function.begin()+i ) ;
    } // if
  } // for
} // FunctionReDefine()

void ReadFunctionDefine( TreeNode* inputPtr ) {

  for ( TreeNode* i_ptr = inputPtr ; i_ptr ; i_ptr = i_ptr->m_right ) {
    if ( i_ptr->m_type != TOKEN || i_ptr->m_left_token->m_type != SYMBOL ) {
      ErrorFormat( "DEFINE" ) ;
    } // if
  } // for
  
  int count = CountRightNodes( inputPtr ) ;
  string function_name = inputPtr->m_left_token->m_token_string ;
  FunctionReDefine( function_name ) ;
  int argument = 0 ;
  Costum_Function temp ;
  temp.function_name = function_name ;
  inputPtr = inputPtr->m_right ;
  while ( inputPtr ) {
    if ( inputPtr->m_left_token && inputPtr->m_left_token->m_type == SYMBOL ) {
      argument ++ ;
      TreeNode* temp_input = CopyObject( inputPtr ) ;
      temp_input->m_right = NULL ;
      temp.function_argument.push_back( temp_input ) ;
    } // if
    else {
      ErrorFormat( "LAMBDA" ) ;
    } // else

    inputPtr = inputPtr->m_right ;
  } // while 

  g_costum_def_function.push_back( temp ) ;
  g_custom_function.push_back( SetFunctionStruct( function_name, "==", count ) ) ;  
  cout << function_name << " defined" << endl ;

} // ReadFunctionDefine()

TreeNode* ReadDefineTwo( TreeNode* inputPtr ) {

  ReadFunctionDefine( inputPtr->m_left ) ;
  g_costum_def_function.back().function_ptr = inputPtr->m_right ;
  throw invalid_argument( "OK" ) ;

} // ReadDefineTwo()


TreeNode* ReadCleanEnviroment() {
  vector<Object> empty_object ;
  empty_object.reserve( 100 ) ;
  g_def_object = empty_object ;
  cout << "environment cleaned" << endl ;
  throw invalid_argument( "OK" ) ;
} // ReadCleanEnviroment()

TreeNode* ReadCar( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      Statement statement ;
      statement.ErrorType( "car", inputPtr ) ;
    } // else 
  } // else

  nowPtr = CopyObject( nowPtr ) ;
  if ( nowPtr->m_type == LISP ) {
    if ( nowPtr->m_right == NULL ) nowPtr = nowPtr->m_left ;
    nowPtr->m_right = NULL ;
    nowPtr->m_can_read = false ;
    if ( nowPtr->m_type == TOKEN && nowPtr->m_left_token->m_type == SYMBOL ) {
      if ( nowPtr->m_dont_exp == false )
        nowPtr = Get_DefObject_Ptr( nowPtr->m_left_token->m_token_string ) ;
    } // if

    return nowPtr ;
  } // if
  else {
    Statement statement ;
    statement.ErrorType( "car", nowPtr ) ;
  } // else 

  return new TreeNode( NIL ) ;

} // ReadCar()

TreeNode* CheckToken( TreeNode* nowPtr ) {

  if ( nowPtr->m_type == TOKEN && nowPtr->m_left_token->m_type == SYMBOL ) {
    nowPtr = Get_DefObject_Ptr( nowPtr->m_left_token->m_token_string ) ;
    return nowPtr ;
  } // if

  return nowPtr ;
} // CheckToken()

TreeNode* ReadCdr( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr = ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
      if ( nowPtr->m_type == LISP ) {
        nowPtr = nowPtr->m_left ;
        if ( nowPtr->m_right_token ) {
          nowPtr->m_left_token = nowPtr->m_right_token ;
          nowPtr->m_left = NULL ;
          nowPtr->m_right = NULL ;
          nowPtr->m_right_token = NULL ;
          return nowPtr ;
        } // if
        else {
          nowPtr = nowPtr->m_right ;
          if ( nowPtr && nowPtr->m_left_token && nowPtr->m_left_token->m_type == DOT ) {
            nowPtr = nowPtr->m_right ;
            return nowPtr ;
          } // if
          
          TreeNode* temp_Tree = new TreeNode( LISP ) ;
          temp_Tree->m_can_read = true ;
          temp_Tree->m_left = nowPtr ;
          temp_Tree->m_dont_exp = true ;
          return temp_Tree ;
        } // else
      } // if
      else {
        Statement statement ;
        statement.ErrorType( "car", nowPtr ) ;
      } // else
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        if ( inputPtr->m_left_token->m_dont_exp == false )
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        else nowPtr = inputPtr ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      Statement statement ;
      statement.ErrorType( "cdr", inputPtr ) ;
    } // else 
  } // else

  nowPtr = CopyObject( nowPtr ) ;
  bool dont_exp = false ;
  if ( nowPtr->m_dont_exp ) dont_exp = true ;
  if ( nowPtr->m_type == LISP ) {
    if ( nowPtr->m_right == NULL ) nowPtr = nowPtr->m_left ;
    if ( nowPtr->m_right_token ) {
      Token* temp = nowPtr->m_right_token ;
      nowPtr = new TreeNode( TOKEN ) ;
      nowPtr->m_can_read = true ;
      nowPtr->m_left_token = temp ;
      return nowPtr ;
    } // if

    if ( nowPtr->m_right == NULL ) return new TreeNode( NIL ) ;
    if ( nowPtr->m_right != NULL ) nowPtr = nowPtr->m_right ;
    if ( nowPtr->m_left_token != NULL && 
         nowPtr->m_left_token->m_token_string == "." ) {
      nowPtr = nowPtr->m_right ;
      if ( not_experance )
        return nowPtr ;
      return CheckToken( nowPtr ) ;
    } // if
    else if ( dont_exp ) {
      TreeNode* temp_Tree = new TreeNode( LISP ) ;
      temp_Tree->m_can_read = true ;
      temp_Tree->m_left = nowPtr ;
      temp_Tree->m_dont_exp = true ; 
      return temp_Tree ;     
    } // else if
    else {
      if ( nowPtr->m_right_token != NULL ) {
        nowPtr->m_left_token = nowPtr->m_right_token ;
        nowPtr->m_right_token = NULL ;
        nowPtr->m_left = NULL ;
        nowPtr->m_right = NULL ;
        nowPtr->m_type = TOKEN ;
        if ( not_experance )
          return nowPtr ;
        return CheckToken( nowPtr ) ;
      } // if

    } // else
    
    if ( nowPtr->m_type == TOKEN && nowPtr->m_left_token->m_type == SYMBOL ) {
      if ( not_experance )
        return nowPtr ;
      TreeNode* temp_Tree = new TreeNode( LISP ) ;
      temp_Tree->m_can_read = true ;
      temp_Tree->m_left = nowPtr ;
      return temp_Tree ;
    } // if

    TreeNode* tempNode = new TreeNode( false ) ;
    tempNode->m_type = LISP ;
    tempNode->m_left = nowPtr ;
    nowPtr = tempNode ;   
    return nowPtr ;
  } // if
  else {
    Statement statement ;
    statement.ErrorType( "cdr", nowPtr ) ;
  } // else 

  return new TreeNode( NIL ) ;
} // ReadCdr()

TreeNode* ReadATOM( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_type == TOKEN ) {
    if ( ! nowPtr->m_right_token ) {
      if (  IsAtomType( nowPtr->m_left_token->m_type ) ) 
        return new TreeNode( T ) ;
    } // if
  } // if

  return new TreeNode( NIL ) ;
} // ReadATOM()

TreeNode* ReadPair( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_type == LISP ) {
    return new TreeNode( T ) ;
    /* 
    nowPtr = nowPtr->m_left ;
    if ( nowPtr->m_left_token && IsAtomType( nowPtr->m_left_token->m_type ) ) {
      if ( nowPtr->m_right_token && IsAtomType( nowPtr->m_right_token->m_type ) ) {
        return new TreeNode( T ) ;
      } // if
      else {
        if ( nowPtr->m_right ) {
          nowPtr = nowPtr->m_right ;
          if ( nowPtr->m_left_token && IsAtomType( nowPtr->m_left_token->m_type ) ) {
            return new TreeNode( T ) ;
          } // if
          else if ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == DOT ) {
            nowPtr = nowPtr->m_right ;
            if ( nowPtr->m_left_token && IsAtomType( nowPtr->m_left_token->m_type ) ) {
              return new TreeNode( T ) ;
            } // if
          } // else if
          else if ( nowPtr->m_left ) {
          } // else if
        } // if
      } // else
    } // if
    */
  } // if

  return new TreeNode( NIL ) ;
} // ReadPair()

TreeNode* ReadIsList( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_type == LISP ) {
    nowPtr = nowPtr->m_left ;

    for ( ; nowPtr ; nowPtr = nowPtr->m_right ) {
      if ( nowPtr->m_right_token )
        return new TreeNode( NIL ) ;
    } // for

    return new TreeNode( T ) ;
  } // if

  return new TreeNode( NIL ) ;
} // ReadIsList()

TreeNode* ReadNULL( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( !nowPtr || nowPtr->m_type == NIL || nowPtr->IsNIL() || 
       ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == NIL ) ||
       ( nowPtr->m_left && nowPtr->m_left->IsNIL() ) )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadNULL()

TreeNode* ReadInteger( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == INT )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadInteger()

TreeNode* ReadReal( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_left_token && ( nowPtr->m_left_token->m_type == INT || 
                                 nowPtr->m_left_token->m_type == FLOAT ) )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadReal()

TreeNode* ReadString( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == STRING )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadString()

TreeNode* ReadBool( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( !nowPtr || nowPtr->IsNIL() || 
       ( nowPtr->m_left_token && 
         ( nowPtr->m_left_token->m_type == NIL || nowPtr->m_left_token->m_type == T ) ) )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadBool()


TreeNode* ReadSymBol( TreeNode* inputPtr ) {

  TreeNode* nowPtr ;
  bool not_experance = false ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else

  if ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == SYMBOL  )
    return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;
} // ReadSymBol()

TreeNode* ReadPlus( TreeNode* inputPtr ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 

    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == INT || nowPtr->m_left_token->m_type == FLOAT ) {
        if ( time != 0 )
          sum_ptr->m_left_token->m_token_string = 
          AddNum( token_change.ChangeToken( sum_ptr->m_left_token->m_token_string ),
                  token_change.ChangeToken( nowPtr->m_left_token->m_token_string ),
                  sum_ptr->m_left_token->m_type ) ; // 要用他已經changeToken的來做運算
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( "+", nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( "+", nowPtr ) ;
    } // else
  } // for

  return sum_ptr ;

} // ReadPlus()

TreeNode* ReadSUB( TreeNode* inputPtr ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == INT || nowPtr->m_left_token->m_type == FLOAT ) {
        if ( time != 0 )
          sum_ptr->m_left_token->m_token_string = 
          MinusNum( token_change.ChangeToken( sum_ptr->m_left_token->m_token_string ),
                    token_change.ChangeToken( nowPtr->m_left_token->m_token_string ),
                    sum_ptr->m_left_token->m_type ) ; 
                                                        // 要用他已經changeToken的來做運算
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( "-", nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( "-", nowPtr ) ;
    } // else
  } // for

  return sum_ptr ;

} // ReadSUB()

TreeNode* ReadMUX( TreeNode* inputPtr ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == INT || nowPtr->m_left_token->m_type == FLOAT ) {
        if ( time != 0 )
          sum_ptr->m_left_token->m_token_string = 
          MultiplyNum( token_change.ChangeToken( sum_ptr->m_left_token->m_token_string ),
                       token_change.ChangeToken( nowPtr->m_left_token->m_token_string ),
                       sum_ptr->m_left_token->m_type ) ; 
                                                        // 要用他已經changeToken的來做運算
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( "*", nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( "*", nowPtr ) ;
    } // else
  } // for

  return sum_ptr ;

} // ReadMUX()

TreeNode* ReadDIV( TreeNode* inputPtr ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == INT || nowPtr->m_left_token->m_type == FLOAT ) {
        if ( time != 0 )
          sum_ptr->m_left_token->m_token_string = 
          DividedNum( token_change.ChangeToken( sum_ptr->m_left_token->m_token_string ),
                      token_change.ChangeToken( nowPtr->m_left_token->m_token_string ),
                      sum_ptr->m_left_token->m_type ) ; 
                                                          // 要用他已經changeToken的來做運算
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( "/", nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( "/", nowPtr ) ;
    } // else
  } // for

  return sum_ptr ;

} // ReadDIV()

TreeNode* ReadNOT( TreeNode* inputPtr ) {

  TreeNode* nowPtr = NULL ;
  if ( inputPtr->m_type == LISP ) {
    nowPtr = ReadLeft( inputPtr->m_left ) ;
  } // if
  else {
    if ( inputPtr->m_left_token->m_type == QUOTE ) {
      nowPtr =  ReadQuote( inputPtr ) ;
      inputPtr = inputPtr->m_right ;
    } // if
    else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      nowPtr = inputPtr ;
    } // else 
  } // else 

  if ( ! nowPtr || ( nowPtr && nowPtr->IsNIL() ) || 
       ( nowPtr->m_left_token && nowPtr->m_left_token->m_type == NIL ) ) {
    return new TreeNode( T ) ;
  } // if

  return new TreeNode( NIL ) ;

} // ReadNOT()

TreeNode* ReadOperator( TreeNode* inputPtr, string op ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  TreeNode* final_ptr = new TreeNode( T ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == INT || nowPtr->m_left_token->m_type == FLOAT ) {
        if ( time != 0 ) {
          if ( ComPare( token_change.ChangeToken( sum_ptr->m_left_token->m_token_string ),
                        token_change.ChangeToken( nowPtr->m_left_token->m_token_string ),
                        op ) ) {
            sum_ptr->m_left_token = nowPtr->m_left_token ;
          } // if
          else {
            delete final_ptr ;
            final_ptr =  new TreeNode( NIL ) ;
          } // else
        } // if
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( op, nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( op, nowPtr ) ;
    } // else
  } // for

  return final_ptr ;

} // ReadOperator()

TreeNode* ReadStringAppend( TreeNode* inputPtr ) {
  TokenClassCategory token_change ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == STRING ) {
        if ( time != 0 ) {
          string str1 = sum_ptr->m_left_token->m_token_string.
          substr( 0, sum_ptr->m_left_token->m_token_string.size()-1 ) ;
          string str2 = nowPtr->m_left_token->m_token_string.
          substr( 1, nowPtr->m_left_token->m_token_string.size()-1 ) ;
          sum_ptr->m_left_token->m_token_string = str1+str2 ;
        } // if
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( "string-append", nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( "string-append", nowPtr ) ;
    } // else
  } // for

  return sum_ptr ;

} // ReadStringAppend()

TreeNode* ReadStringCompare( TreeNode* inputPtr, string op ) {
  TokenClassCategory token_change ;
  TreeNode* final_ptr = new TreeNode( T ) ;
  TreeNode* sum_ptr = new TreeNode( TOKEN ) ;
  sum_ptr->m_left_token = new Token( "0", 0, 0, INT ) ;
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    if ( nowPtr && nowPtr->m_type == TOKEN && nowPtr->m_left_token ) {
      if ( nowPtr->m_left_token->m_type == STRING ) {
        if ( time != 0 ) {
          string str1 = sum_ptr->m_left_token->m_token_string ;
          string str2 = nowPtr->m_left_token->m_token_string ;
          if ( ComPareString( str1, str2, op ) ) {
            sum_ptr->m_left_token = nowPtr->m_left_token ;
          } // if
          else {
            delete final_ptr ;
            final_ptr = new TreeNode( NIL ) ;
          } // else
        } // if
        else sum_ptr->m_left_token = nowPtr->m_left_token ;
      } // if
      else {
        Statement state ;
        state.ErrorType( op, nowPtr ) ;
      } // else
    } // if
    else {
      Statement state ;
      state.ErrorType( op, nowPtr ) ;
    } // else
  } // for

  return final_ptr ;

} // ReadStringCompare()

TreeNode* ReadEQV( TreeNode* inputPtr ) {

  TreeNode* nowPtr_1 = NULL ;
  TreeNode* nowPtr_2 = NULL ;
  int time = 0 ;
  if ( inputPtr->m_type == TOKEN ) {
    if ( IsAtomType( inputPtr->m_left_token->m_type ) ) {
      if ( inputPtr->m_left_token->m_type == STRING ) {
        return new TreeNode( NIL ) ;
      } // if
      else {
        nowPtr_1 = inputPtr ;
      } // else
    } // if
  } // if
  else if ( inputPtr->m_type == LISP ) {
    nowPtr_1 = ReadLeft( inputPtr->m_left ) ;
  } // else if
  else {
    return new TreeNode( NIL ) ; 
  } // else

  inputPtr = inputPtr->m_right ;
  if ( inputPtr->m_type == TOKEN ) {
    if ( IsAtomType( inputPtr->m_left_token->m_type ) ) {
      if ( inputPtr->m_left_token->m_type == STRING ) {
        return new TreeNode( NIL ) ;
      } // if
      else {
        nowPtr_2 = inputPtr ;
      } // else
    } // if
  } // if
  else if ( inputPtr->m_type == LISP ) {
    nowPtr_2 = ReadLeft( inputPtr->m_left ) ;
  } // else if
  else {
    return new TreeNode( NIL ) ; 
  } // else
  
  if ( nowPtr_1->m_type == LISP || nowPtr_2->m_type == LISP ) {
    return new TreeNode( NIL ) ;
  } // if 如果有出現一個list 那就不能用, list之間的記憶體位置不同

  if ( ( nowPtr_1->m_left_token && nowPtr_1->m_left_token->m_type == STRING ) ||
       ( nowPtr_2->m_left_token && nowPtr_2->m_left_token->m_type == STRING ) ) {
    return new TreeNode( NIL ) ;
  } // if 如果是string 也要處理掉

  if ( ( nowPtr_1->m_type == NIL || ( nowPtr_1->m_left_token && nowPtr_1->m_left_token->m_type == NIL ) ) &&
       ( nowPtr_2->m_type == NIL || ( nowPtr_2->m_left_token && nowPtr_2->m_left_token->m_type == NIL ) ) ) {
    return new TreeNode( T ) ;
  } // if 如果兩個都是如果兩個都是nil 那可以直接算對

  /*
    下面要注意的是 可能會有兩種 一種是symbol 一種是非symbol 
    如果是非symbol比較簡單 可以直接處理
    比對後只要OK就沒事了
  */
  if ( nowPtr_1->m_left_token->m_type != SYMBOL && nowPtr_2->m_left_token->m_type != SYMBOL ) {
    if ( nowPtr_1->m_left_token->m_token_string == nowPtr_2->m_left_token->m_token_string )
      return new TreeNode( T ) ;
  } // if

  TreeNode* real_ptr_1 = NULL ;
  TreeNode* real_ptr_2 = NULL ;
  Function function_1 ;
  Function function_2 ;
  // 如果有一個是symbol 那要先把它處理起來
  if ( nowPtr_1->m_left_token->m_type == SYMBOL && nowPtr_1->m_dont_exp != true ) {
    int type = GetDefinedType( nowPtr_1->m_left_token->m_token_string ) ; 
    if ( type == SYMBOL ) {
      Object object = GetDefObject( nowPtr_1->m_left_token->m_token_string ) ;
      if ( object.type == SYMBOL ) {
        real_ptr_1 = Get_RealObject_Ptr( object.object_name ) ;
      } // if
      else {
        function_1 = GetFromFunction( object.object_function->m_function_name ) ;
      } // else
    } // if
    else if ( type == FUNCTION ) {
      function_1 = GetFromFunction( nowPtr_1->m_left_token->m_token_string ) ;
    } // else if
    else {
      ErrorSymBol( nowPtr_1->m_left_token->m_token_string ) ;
    } // else 
  } // if 先確定有沒有定義過 這樣才可以做後面的事情


  if ( nowPtr_2->m_left_token->m_type == SYMBOL && nowPtr_2->m_dont_exp != true ) {
    int type = GetDefinedType( nowPtr_2->m_left_token->m_token_string ) ; 
    if ( type == SYMBOL ) {
      Object object = GetDefObject( nowPtr_2->m_left_token->m_token_string ) ;
      if ( object.type == SYMBOL ) {
        real_ptr_2 = Get_RealObject_Ptr( object.object_name ) ;
      } // if
      else {
        function_2 = GetFromFunction( object.object_function->m_function_name ) ;
      } // else
    } // if
    else if ( type == FUNCTION ) {
      function_2 = GetFromFunction( nowPtr_2->m_left_token->m_token_string ) ;
    } // else if
    else {
      ErrorSymBol( nowPtr_2->m_left_token->m_token_string ) ;
    } // else 
  } // if 先確定有沒有定義過 這樣才可以做後面的事情


  if ( !FunctionNULL( function_1 ) || ! FunctionNULL( function_2 ) ) {
    if ( FunctionEqual( function_1, function_2 ) ) {
      return new TreeNode( T ) ;
    } // if
    else return new TreeNode( NIL ) ;
  } // if

  if ( !real_ptr_1 && !real_ptr_2 ) return new TreeNode( NIL ) ;

  if ( real_ptr_1 && !real_ptr_2 ) {
    if ( ( real_ptr_1->m_left_token && real_ptr_1->m_left_token->m_type ) == 
         ( nowPtr_2->m_left_token && nowPtr_2->m_left_token->m_type ) ) {
      if ( real_ptr_1->m_left_token->m_token_string == nowPtr_2->m_left_token->m_token_string )
        return new TreeNode( T ) ;
    } // if
  } // if

  if ( !real_ptr_1 && real_ptr_2 ) {
    if ( ( nowPtr_1->m_left_token && nowPtr_1->m_left_token->m_type ) == 
         ( real_ptr_2->m_left_token && real_ptr_2->m_left_token->m_type ) ) {
      if ( nowPtr_1->m_left_token->m_token_string == real_ptr_2->m_left_token->m_token_string )
        return new TreeNode( T ) ;
    } // if
  } // if

  if ( real_ptr_1 && real_ptr_2 ) {
    if ( real_ptr_1 == real_ptr_2 ) return new TreeNode( T ) ;
    if ( real_ptr_1->m_type == TOKEN && real_ptr_2->m_type == TOKEN ) {
      if ( ( real_ptr_1->m_left_token && IsAtomType( real_ptr_1->m_left_token->m_type ) ) &&
           ( real_ptr_2->m_left_token && IsAtomType( real_ptr_2->m_left_token->m_type ) ) && 
           ( real_ptr_1->m_left_token->m_type != SYMBOL && real_ptr_2->m_left_token->m_type != SYMBOL ) && 
           ( real_ptr_1->m_left_token->m_type != STRING && real_ptr_2->m_left_token->m_type != STRING ) ) {
        if ( real_ptr_1->m_left_token->m_token_string == real_ptr_2->m_left_token->m_token_string )
          return new TreeNode( T ) ;
      } // if
    } // if
  } // if

  return new TreeNode( NIL ) ;

} // ReadEQV() 


TreeNode* ReadEQUAL( TreeNode* inputPtr ) {

  TreeNode* nowPtr_1 = NULL ;
  TreeNode* nowPtr_2 = NULL ;
  int time = 0 ;
  if ( inputPtr->m_type == TOKEN ) {
    nowPtr_1 = CopyObject( inputPtr ) ;
    nowPtr_1->m_right = NULL ;
  } // if
  else if ( inputPtr->m_type == LISP ) {
    nowPtr_1 = ReadLeft( inputPtr->m_left ) ;
  } // else if
  else {
    return new TreeNode( NIL ) ; 
  } // else

  inputPtr = inputPtr->m_right ;
  if ( inputPtr->m_type == TOKEN ) {
    nowPtr_2 = CopyObject( inputPtr ) ;
    nowPtr_2->m_right = NULL ;
  } // if
  else if ( inputPtr->m_type == LISP ) {
    nowPtr_2 = ReadLeft( inputPtr->m_left ) ;
  } // else if
  else {
    return new TreeNode( NIL ) ; 
  } // else


  if ( nowPtr_1->m_left_token && nowPtr_1->m_left_token->m_type == SYMBOL && 
       nowPtr_1->m_dont_exp != true ) {
    int type = GetDefinedType( nowPtr_1->m_left_token->m_token_string ) ; 
    if ( type == SYMBOL || type == FUNCTION ) {
      nowPtr_1 = Get_DefObject_Ptr( nowPtr_1->m_left_token->m_token_string ) ;
    } // if
    else {
      ErrorSymBol( nowPtr_1->m_left_token->m_token_string ) ;
    } // else 
  } // if 先確定有沒有定義過 這樣才可以做後面的事情


  if ( nowPtr_2->m_left_token && nowPtr_2->m_left_token->m_type == SYMBOL && 
       nowPtr_2->m_dont_exp != true ) {
    int type = GetDefinedType( nowPtr_2->m_left_token->m_token_string ) ; 
    if ( type == SYMBOL || type == FUNCTION ) {
      nowPtr_2 = Get_DefObject_Ptr( nowPtr_2->m_left_token->m_token_string ) ;
    } // if
    else {
      ErrorSymBol( nowPtr_2->m_left_token->m_token_string ) ;
    } // else 
  } // if 先確定有沒有定義過 這樣才可以做後面的事情
  
  if ( CompareTwoTreeNode( nowPtr_1, nowPtr_2 ) ) return new TreeNode( T ) ;
  return new TreeNode( NIL ) ;


} // ReadEQUAL()

TreeNode* ReadAnd( TreeNode* inputPtr ) {
  // 要嘛就是回傳第一個是NIL 要嘛就回傳最後一個
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    nowPtr = CopyObject( nowPtr ) ;
    nowPtr->m_right = NULL ;
    if ( !inputPtr->m_right ) {
      return nowPtr ;
    } // if

    if ( nowPtr->IsNIL() || ( nowPtr && nowPtr->m_left_token && nowPtr->m_left_token->m_type == NIL ) ) {
      return new TreeNode( NIL ) ;
    } // if
  } // for

  return inputPtr ;

} // ReadAnd() 

TreeNode* ReadOR( TreeNode* inputPtr ) {
  // 回傳第一個不是NIL的 不然就回傳最後一個
  int time = 0 ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;

    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    nowPtr = CopyObject( nowPtr ) ;
    nowPtr->m_right = NULL ;
    if ( !inputPtr->m_right ) {
      return nowPtr ;
    } // if

    if ( ! nowPtr->IsNIL() && ( nowPtr && nowPtr->m_left_token && nowPtr->m_left_token->m_type != NIL ) ) {
      return nowPtr ;
    } // if
  } // for

  return inputPtr ;
} // ReadOR()

TreeNode* ReadIF( TreeNode* inputPtr ) {


  int time = 0 ;
  TreeNode* ptr_1 = NULL ;
  TreeNode* ptr_2 = NULL ;
  TreeNode* ptr_3 = NULL ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right, time ++ ) {
    
    TreeNode* nowPtr = NULL ;
    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeftDefine( inputPtr->m_left ) ;
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    nowPtr = CopyObject( nowPtr ) ;
    nowPtr->m_right = NULL ;
    if ( time == 0 ) {
      ptr_1 = nowPtr ;
      if ( ptr_1->IsNIL() || ( ptr_1 && ptr_1->m_left_token && ptr_1->m_left_token->m_type == NIL ) ) {
        time ++ ;
        inputPtr = inputPtr->m_right ;
      } // if 回傳第三個
    } // if
    else if ( time == 1 ) ptr_2 = nowPtr ;
    else if ( time == 2 ) ptr_3 = nowPtr ;
  } // for

  if ( ptr_1->IsNIL() || ( ptr_1 && ptr_1->m_left_token && ptr_1->m_left_token->m_type == NIL ) ) {
    if ( ptr_3 ) return ptr_3 ;
    else NoReturnError() ;
  } // if 回傳第三個
  else {
    return ptr_2 ;
  } // else

  NoReturnError() ;
  return new TreeNode( NIL ) ;
} // ReadIF() 

bool IsList( TreeNode* inputPtr ) {
  if ( inputPtr ) {
    if ( inputPtr->m_right_token ) return false ;
    if ( !IsList( inputPtr->m_right ) ) return false ;
  } // if

  return true ;

} // IsList()

void DeletePtr( TreeNode* inputPtr ) ;

TreeNode* ReadLEFT_TO_COND( TreeNode* inputPtr ) {
  if ( !IsList( inputPtr ) ) {
    cout << "ERROR (non-list) : " ;
    bool enter = false ;
    Statement printer ;
    TreeNode* newPtr = new TreeNode( false ) ;
    newPtr->m_type = LISP ;
    newPtr->m_left = inputPtr ;
    printer.PrintLisp( newPtr, 0, enter ) ;
    DeletePtr( newPtr ) ;
    throw invalid_argument( "Error (non-list)" ) ; 
  } // if

  if ( inputPtr->m_type == TOKEN || inputPtr->m_type == NIL ) { 
    if ( inputPtr->IsNIL() || 
         ( inputPtr && inputPtr->m_left_token && inputPtr->m_left_token->m_type == NIL ) )
      return new TreeNode( NIL ) ;
    else if ( inputPtr->m_dont_exp == false && inputPtr->m_left_token->m_type == SYMBOL ) {
      if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
        int type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ;
        if ( type == SYMBOL ) {
          TreeNode* tempNode = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
          tempNode->m_right = inputPtr->m_right ;
          return tempNode ;
        } // if
        
        return inputPtr ;
      } // if
      else {
        ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
      } // else
    } // else if
    else {
      return inputPtr ;
    } // else
  } // if
  else if ( inputPtr->m_type == LISP ) {
    TreeNode* tempNode  = ReadLeft( inputPtr->m_left ) ;
    tempNode->m_right = inputPtr->m_right ;
    return tempNode ;
  } // else if

  return new TreeNode( NIL ) ;
} // ReadLEFT_TO_COND()

TreeNode* ReadLEFTANDELSE( TreeNode* inputPtr ) {

  if ( !IsList( inputPtr ) ) {
    Statement printer ;
    cout << "ERROR (non-list) : " ;
    bool enter = false ;
    TreeNode* newPtr = new TreeNode( false ) ;
    newPtr->m_type = LISP ;
    newPtr->m_left = inputPtr ;
    printer.PrintLisp( newPtr, 0, enter ) ;
    DeletePtr( newPtr ) ;
    throw invalid_argument( "Error (non-list)" ) ; 
  } // if

  if ( inputPtr->m_left_token && inputPtr->m_left_token->m_token_string == "else" ) {
    TreeNode* tempNode = new TreeNode( T ) ;
    tempNode->m_right = inputPtr->m_right ;
    return tempNode ;
  }  // if
  else {
    return ReadLEFT_TO_COND( inputPtr ) ;
  } // else

  return new TreeNode( NIL ) ;

} // ReadLEFTANDELSE()


TreeNode* ReadCOND( TreeNode* inputPtr ) {

  vector<TreeNode*> condition_ptr_vector ;
  condition_ptr_vector.reserve( 100 ) ;
  for ( ; inputPtr ; inputPtr = inputPtr->m_right ) {
    
    TreeNode* nowPtr = NULL ;
    if ( inputPtr->m_type == LISP ) {
      if ( inputPtr->m_right )
        nowPtr = ReadLEFT_TO_COND( inputPtr->m_left ) ;
      else {
        nowPtr = ReadLEFTANDELSE( inputPtr->m_left ) ;
      } // else
    } // if

    nowPtr = CopyObject( nowPtr ) ;
    if ( nowPtr->m_type == LISP || 
         ( ! nowPtr->IsNIL() && nowPtr->m_left_token->m_type != NIL ) ) {
      TreeNode* resultPtr = NULL ;
      nowPtr = nowPtr->m_right ;
      for ( ; nowPtr ; ) {
        if ( nowPtr->m_type == LISP ) {
          resultPtr = ReadLeft( nowPtr->m_left ) ;
        } // if
        else {
          if ( nowPtr->m_left_token->m_type == QUOTE ) {
            resultPtr =  ReadQuote( nowPtr ) ;
            nowPtr = nowPtr->m_right ;
          } // if
          else if ( nowPtr->m_left_token->m_type == SYMBOL ) {
            if ( IsDefinedOrNot( nowPtr->m_left_token->m_token_string ) ) {
              resultPtr = Get_DefObject_Ptr( nowPtr->m_left_token->m_token_string ) ;
            } // if
            else {
              ErrorSymBol( nowPtr->m_left_token->m_token_string ) ;
            } // else
          } // else if
          else {
            resultPtr = nowPtr ;
          } // else 
        } // else

        nowPtr = nowPtr->m_right ;
      } // for

      return CopyObject( resultPtr ) ;
    } // if
  } // for

  NoReturnError() ;
  return new TreeNode( NIL ) ;
} // ReadCOND() 


TreeNode* ReadBegin( TreeNode* inputPtr ) {

  TreeNode* nowPtr = NULL ;
  for ( ; inputPtr ; ) {
    if ( inputPtr->m_type == LISP ) {
      nowPtr = ReadLeft( inputPtr->m_left ) ;
      /*
      if ( nowPtr->m_left_token && nowPtr->m_left_token->m_token_string == "new lambda" ) {
        g_custom_function.pop_back() ;
        g_stack_lambda.pop() ;
      } // if
      */
    } // if
    else {
      if ( inputPtr->m_left_token->m_type == QUOTE ) {
        nowPtr =  ReadQuote( inputPtr ) ;
        inputPtr = inputPtr->m_right ;
      } // if
      else if ( inputPtr->m_left_token->m_type == SYMBOL ) {
        if ( IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          nowPtr = Get_DefObject_Ptr( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // else
      } // else if
      else {
        nowPtr = inputPtr ;
      } // else 
    } // else

    inputPtr = inputPtr->m_right ;
  } // for

  nowPtr = CopyObject( nowPtr ) ;

  return nowPtr ;
} // ReadBegin()

void CheckCondFormat( TreeNode* inputPtr ) {

  for ( ; inputPtr ; inputPtr = inputPtr->m_right ) {
    if ( inputPtr->m_type != LISP ) {
      ErrorFormat( "COND" ) ;
    } // if
    else if ( CountRightNodes( inputPtr->m_left )+1 < 2 ) ErrorFormat( "COND" ) ; 
  } // for

} // CheckCondFormat()

bool CheckLetFormat( TreeNode* inputPtr ) {

  if ( inputPtr->m_type != LISP && inputPtr->m_type == NIL ) {
    ErrorFormat( "LET" ) ;
  } // if
  else {
    if ( CountRightNodes( inputPtr ) > 0 ) {
      return true ;
    } // if
    else {
      ErrorFormat( "LET" ) ;
    } // else
  } // else

  return false ;

} // CheckLetFormat()

Object Define_Local_Vairable( TreeNode* inputPtr ) {
  // 簡單來說 把右邊出現的值送進object裡面
  if ( CountRightNodes( inputPtr ) != 1 ) {
    ErrorFormat( "LET" ) ;
  } // if

  Object temp_object ;
  temp_object.object_name = inputPtr->m_left_token->m_token_string ;
  inputPtr = inputPtr->m_right ;
  temp_object.object_ptr = ReadBegin( inputPtr ) ;
  if ( temp_object.object_ptr->m_dont_exp == false &&
       temp_object.object_ptr->m_left_token &&
       GetDefinedType( temp_object.object_ptr->m_left_token->m_token_string ) == FUNCTION ) {
    temp_object.type = FUNCTION ;
    temp_object.object_function = 
    GetFromFunction_PTR( temp_object.object_ptr->m_left_token->m_token_string ) ;
    temp_object.object_ptr = NULL ;
  } // if
  else {
    temp_object.type = SYMBOL ;
    temp_object.object_function = NULL ;
  } // else
  
  return temp_object ;  

} // Define_Local_Vairable()

TreeNode* ReadCostumFunction( TreeNode* inputPtr, string function_name ) {

  // 把那些東西組裝進來
  int count = 0 ;
  vector<Object> tempvector ;
  tempvector.reserve( 100 ) ;
  Costum_Function tempFunction ;
  for ( vector<Costum_Function>::iterator it = g_costum_def_function.begin() ; 
        it != g_costum_def_function.end() ; ++it ) {
    if ( it->function_name == function_name ) {
      tempFunction = *it ;
    } // if
  } // for

  while ( inputPtr ) {
    TreeNode* tempNode = CopyObject( inputPtr ) ;
    tempNode->m_right = NULL ;
    tempFunction.function_argument[count]->m_right = tempNode ;
    tempvector.push_back( Define_Local_Vairable( tempFunction.function_argument[count] ) ) ;
    count ++ ;
    inputPtr = inputPtr->m_right ;
  } // while 

  g_stack_object.push( tempvector ) ;

  TreeNode* nowPtr = ReadBegin( tempFunction.function_ptr ) ;
  g_stack_object.pop() ;
  return nowPtr ;

} // ReadCostumFunction()

vector<Object> ReadletVairable( TreeNode* inputPtr ) {
  // 用完這個東西 可以得到區域變數
  // 在這一區進來之後 他的裡面還會有其他的() 每看到一個就要定義一次
  // 他裡面只能有() 不能出現其他東西
  // 在check format時會確認 
  for ( TreeNode* i_ptr = inputPtr ; i_ptr != NULL ; i_ptr = i_ptr->m_right ) {
    if ( i_ptr->m_type != LISP ) {
      ErrorFormat( "LET" ) ;
    } // if
  } // for

  vector<Object> temp_object ;
  temp_object.reserve( 100 ) ;
  while ( inputPtr ) {
    temp_object.push_back( Define_Local_Vairable( inputPtr->m_left ) ) ;
    inputPtr = inputPtr->m_right ;
  } // while

  return temp_object ;
} // ReadletVairable()

TreeNode* Readlet( TreeNode* inputPtr ) {
  
  vector<Object> local_object ;
  local_object.reserve( 100 ) ;
  CheckLetFormat( inputPtr ) ;
  local_object = ReadletVairable( inputPtr->m_left ) ;
  g_stack_object.push( local_object ) ;
  inputPtr = inputPtr->m_right ;
  TreeNode* now_ptr = ReadBegin( inputPtr ) ;
  g_stack_object.pop() ;
  return now_ptr ;

} // Readlet()


void SaveThelambdaToStack( TreeNode* inputPtr ) {
  g_stack_lambda.top().function_ptr = inputPtr ;
} // SaveThelambdaToStack()

void ReadLambdaSymbol( TreeNode* inputPtr ) {
  int argument = 0 ;
  Costum_Function temp ;
  while ( inputPtr ) {
    if ( inputPtr->m_left_token && inputPtr->m_left_token->m_type == SYMBOL ) {
      argument ++ ;
      TreeNode* temp_input = CopyObject( inputPtr ) ;
      temp_input->m_right = NULL ;
      temp.function_argument.push_back( temp_input ) ;
    } // if
    else {
      ErrorFormat( "LAMBDA" ) ;
    } // else

    inputPtr = inputPtr->m_right ;
  } // while

  g_custom_function.push_back( SetFunctionStruct( "new lambda", "==", argument ) ) ;
  g_stack_lambda.push( temp ) ;
} // ReadLambdaSymbol()

TreeNode* ReadLambda( TreeNode* inputPtr ) {
  ReadLambdaSymbol( inputPtr->m_left ) ;
  g_stack_lambda.top().function_ptr = inputPtr->m_right ;
  Token* token = new Token() ;
  token->m_token_string = "new lambda" ;
  token->m_type = SYMBOL ;
  TreeNode* newPtr = new TreeNode( token ) ;
  return newPtr ;
} // ReadLambda()

TreeNode* ReadNewLambda( TreeNode* inputPtr ) {
  // 把那些東西組裝進來
  int count = 0 ;
  vector<Object> tempvector ;
  tempvector.reserve( 100 ) ;
  while ( inputPtr ) {
    TreeNode* tempNode = CopyObject( inputPtr ) ;
    tempNode->m_right = NULL ;
    g_stack_lambda.top().function_argument[count]->m_right = tempNode ;
    tempvector.push_back( Define_Local_Vairable( g_stack_lambda.top().function_argument[count] ) ) ;
    count ++ ;
    inputPtr = inputPtr->m_right ;
  } // while 

  g_stack_object.push( tempvector ) ;
  TreeNode* nowPtr = ReadBegin( g_stack_lambda.top().function_ptr ) ;
  g_stack_object.pop() ;
  g_stack_lambda.pop() ;
  bool ok = false ;
  for ( int i = 0 ; i < g_custom_function.size() ; i ++ ) {
    if ( g_custom_function[i].m_function_name == "new lambda" ) {
      g_custom_function.erase( g_custom_function.begin()+i ) ;
    } // if

  } // for

  return nowPtr ;

} // ReadNewLambda()


TreeNode* ReadFunction( TreeNode* inputPtr, Function nowFunction ) {
  Statement printer ;
  g_level ++ ;
  cout << "" ;
  if ( !IsList( inputPtr ) ) {
    cout << "ERROR (non-list) : " ;
    bool enter = false ;
    TreeNode* newPtr = new TreeNode( false ) ;
    newPtr->m_type = LISP ;
    newPtr->m_left = inputPtr ;
    printer.PrintLisp( newPtr, 0, enter ) ;
    DeletePtr( newPtr ) ;
    throw invalid_argument( "Error (non-list)" ) ; 
  } // if

  if ( nowFunction.m_function_name == "define" ) {
    if ( g_level != 1 ) {
      ErrorLevel( "DEFINE" ) ;
    } // if
  } // if
  else if ( nowFunction.m_function_name == "clean-environment" ) {
    if ( g_level != 1 ) {
      ErrorLevel( "CLEAN-ENVIRONMENT" ) ;
    } // if
  } // else if
  else if ( nowFunction.m_function_name == "exit" ) {
    if ( g_level != 1 ) {
      ErrorLevel( "EXIT" ) ;
    } // if
  } // else if

  if ( CheckNodeValidity( inputPtr ) ) { // 這裡如果沒找到function的話會跳出error
    if ( nowFunction.m_function_name == "'" || nowFunction.m_function_name == "quote" ) 
      return ReadQuote( inputPtr ) ;
    if ( nowFunction.m_function_name == "cons" ) {
      TreeNode* nowPtr = new TreeNode( false ) ;
      nowPtr->m_type = LISP ;
      nowPtr->m_left = ReadCons( inputPtr->m_right, 0 ) ; // 先進入該死的function
      return nowPtr ;
    } // if
    else if ( nowFunction.m_function_name == "list" ) {
      TreeNode* nowPtr = new TreeNode( false ) ;
      nowPtr->m_type = LISP ;
      nowPtr->m_left = ReadList( inputPtr->m_right ) ;
      return nowPtr ;
    } // else if
    else if ( nowFunction.m_function_name == "define" ) {
      if ( g_level != 1 ) {
        ErrorLevel( "DEFINE" ) ;
      } // if
      else {
        return ReadDefine( inputPtr->m_right ) ;
      } // else
    } // else if
    else if ( nowFunction.m_function_name == "clean-environment" ) {
      if ( g_level != 1 ) {
        ErrorLevel( "CLEAN-ENVIRONMENT" ) ;
      } // if
      else
        return ReadCleanEnviroment() ;
    } // else if
    else if ( nowFunction.m_function_name == "car" ) {
      return ReadCar( inputPtr->m_right ) ; 
    } // else if
    else if ( nowFunction.m_function_name == "cdr" ) {
      return ReadCdr( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "exit" ) {
      if ( g_level != 1 ) {
        ErrorLevel( "EXIT" ) ;
      } // if
      else
        ReadExit() ;
    } // else if
    else if ( nowFunction.m_function_name == "atom?" ) {
      return ReadATOM( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "pair?" ) {
      return ReadPair( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "list?" ) {
      return ReadIsList( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "null?"  ) {
      return ReadNULL( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "integer?"  ) {
      return ReadInteger( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "real?"  ) {
      return ReadReal( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "number?"  ) {
      return ReadReal( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "string?"  ) {
      return ReadString( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "boolean?"  ) {
      return ReadBool( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "symbol?" ) {
      return ReadSymBol( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "+" ) {
      return ReadPlus( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "-" ) {
      return ReadSUB( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "*" ) {
      return ReadMUX( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "/" ) {
      return ReadDIV( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "not" ) {
      return ReadNOT( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == ">" ) {
      return ReadOperator( inputPtr->m_right, ">" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "<" ) {
      return ReadOperator( inputPtr->m_right, "<" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "<=" ) {
      return ReadOperator( inputPtr->m_right, "<=" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "=" ) {
      return ReadOperator( inputPtr->m_right, "==" ) ;
    } // else if
    else if ( nowFunction.m_function_name == ">=" ) {
      return ReadOperator( inputPtr->m_right, ">=" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "string-append" ) {
      return ReadStringAppend( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "string=?" ) {
      return ReadStringCompare( inputPtr->m_right, "string=?" ) ;
    }  // else if
    else if ( nowFunction.m_function_name == "string>?" ) {
      return ReadStringCompare( inputPtr->m_right, "string>?" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "string<?" ) {
      return ReadStringCompare( inputPtr->m_right, "string<?" ) ;
    } // else if
    else if ( nowFunction.m_function_name == "eqv?" ) {
      return ReadEQV( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "equal?" ) {
      return ReadEQUAL( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "and" ) {
      return ReadAnd( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "or" ) {
      return ReadOR( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "if" ) {
      return ReadIF( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "cond" ) {
      CheckCondFormat( inputPtr->m_right ) ;
      return ReadCOND( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "begin" ) {
      return ReadBegin( inputPtr->m_right ) ;
    } // else if
    else if ( nowFunction.m_function_name == "let" ) {
      return Readlet( inputPtr->m_right ) ;
    } // else if 
    else if ( nowFunction.m_function_name == "lambda" ) {
      return ReadLambda( inputPtr->m_right ) ;
    } // else if 
    else if ( nowFunction.m_function_name == "new lambda" ) {
      return ReadNewLambda( inputPtr->m_right ) ;
    } // else if
    else {
      return ReadCostumFunction( inputPtr->m_right, nowFunction.m_function_name ) ;
    } // else
  } // if
  else {
    if ( nowFunction.m_function_name == "define" ) {
      ErrorFormat( "DEFINE" ) ;
    } // if
    else if ( nowFunction.m_function_name == "cond" ) {
      ErrorFormat( "COND" ) ;
    } // else if
    else
      ArgumentError( nowFunction.m_function_name ) ;
  } // else 

  return new TreeNode( NIL ) ;
} // ReadFunction()

TreeNode* ReadLeftDefine( TreeNode* inputPtr ) {
  Statement printer ;
  try {

    if ( inputPtr->m_type == LISP ) {
      if ( inputPtr->m_dont_exp )
        if ( 1 ) throw invalid_argument( "Function Error" ) ;
      TreeNode* temp = ReadLeft( inputPtr->m_left ) ;
      temp->m_right = inputPtr->m_right ;
      if ( temp->m_type == LISP ) {
        printer.ErrorFunction( temp ) ;
      } // if

      temp = ReadLeft( temp ) ;
      return temp ;
    } // if 如果他還是一個leftparn 那就要先下一個function所輸出的結果回傳出來 
    else if ( inputPtr->m_type == TOKEN ) {
      if ( inputPtr->m_right_token ) if ( 1 ) throw invalid_argument( "Function Error" ) ;
      Function nowFunction ;
      int type = 0 ;
      if ( inputPtr->m_dont_exp == false && 
           ( inputPtr->m_left_token->m_type == SYMBOL || inputPtr->m_left_token->m_type == QUOTE ) ) {
        if ( !IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          int token_type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ; 
          if ( token_type == SYMBOL ) {
            Object object = GetDefObject( inputPtr->m_left_token->m_token_string ) ;
            if ( object.type == FUNCTION ) {
              nowFunction = GetFromFunction( object.object_function->m_function_name ) ;
              return ReadFunction( inputPtr, nowFunction ) ; 
              // 如果有定義過就可以進來 會根據function的名字以及他的m_argument做出相對應的動作
            } // if
            else { 
              inputPtr = Get_DefObject_Ptr( object.object_name ) ;
            } // else
          } // if
          else if ( token_type == FUNCTION ) {
            nowFunction = GetFromFunction( inputPtr->m_left_token->m_token_string ) ;
            if ( nowFunction.m_function_name == "lambda" ) {
              TreeNode* tempPtr = new TreeNode( LISP ) ;
              tempPtr->m_left = inputPtr ;
              return tempPtr ;
            } // if
            else  return ReadFunction( inputPtr, nowFunction ) ; 
            // 如果有定義過就可以進來 會根據function的名字以及他的m_argument做出相對應的動作
          } // else if

        } // else
      } // if 如果他是symbol的話要先確定有沒有定義過 如果沒有定義過要直接拋出錯誤

      if ( 1 ) throw invalid_argument( "Function Error" ) ;
      return new TreeNode( NIL ) ;
      // 如果這邊沒有定義過 那會拋出一個error 自然就會被整個丟出去
      // 由於left後面一定要接一個function 所以如果找不到function就代表這個東西不對了
    } // else if
    else if ( ! inputPtr->m_left ) {
      return new TreeNode( NIL ) ;
    } // else if
    else {
      cout << "Type error please Check" << endl ;
      throw ( "Type Error" ) ;
    } // else
  } // try
  catch( exception &e ) {
    if ( strcmp( e.what(), "No Return" ) == 0 ) {
      bool enter ;
      TreeNode* newPtr = new TreeNode( false ) ;
      newPtr->m_type = LISP ;
      newPtr->m_left = inputPtr ;
      cout << "ERROR (no return value) : " ;
      printer.PrintLisp( newPtr, 0, enter ) ;
      DeletePtr( newPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Format Error" ) == 0 ) {
      bool enter ;
      TreeNode* newPtr = new TreeNode( false ) ;
      newPtr->m_type = LISP ;
      newPtr->m_left = inputPtr ;
      printer.PrintLisp( newPtr, 0, enter ) ;
      DeletePtr( newPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Function Error" ) == 0 ) {
      printer.ErrorFunction( inputPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Normal Exit" ) == 0 ) {
      if ( 1 ) throw invalid_argument( e.what() ) ;
    } // else if

    if ( 1 ) throw invalid_argument( "Error" ) ;
  } // catch

  return new TreeNode( NIL ) ;
} // ReadLeftDefine()

TreeNode* ReadLeft( TreeNode* inputPtr ) {
  Statement printer ;
  try {
    if ( inputPtr->m_type == LISP ) {
      if ( inputPtr->m_dont_exp )
        if ( 1 ) throw invalid_argument( "Function Error" ) ;
      TreeNode* temp = ReadLeft( inputPtr->m_left ) ;
      temp->m_right = inputPtr->m_right ;
      // if ( temp->m_type == LISP ) {
      //  printer.ErrorFunction( temp ) ;
      // } // if

      temp = ReadLeft( temp ) ;
      return temp ;
    } // if 如果他還是一個leftparn 那就要先下一個function所輸出的結果回傳出來 
    else if ( inputPtr->m_type == TOKEN ) {
      if ( inputPtr->m_right_token ) if ( 1 ) throw invalid_argument( "Function Error" ) ;
      Function nowFunction ;
      int type = 0 ;
      if ( inputPtr->m_dont_exp == false && 
           ( inputPtr->m_left_token->m_type == SYMBOL || inputPtr->m_left_token->m_type == QUOTE ) ) {
        if ( !IsDefinedOrNot( inputPtr->m_left_token->m_token_string ) ) {
          ErrorSymBol( inputPtr->m_left_token->m_token_string ) ;
        } // if
        else {
          int token_type = GetDefinedType( inputPtr->m_left_token->m_token_string ) ; 
          if ( token_type == SYMBOL ) {
            Object object = GetDefObject( inputPtr->m_left_token->m_token_string ) ;
            if ( object.type == FUNCTION ) {
              nowFunction = GetFromFunction( object.object_function->m_function_name ) ;
              return ReadFunction( inputPtr, nowFunction ) ; 
              // 如果有定義過就可以進來 會根據function的名字以及他的m_argument做出相對應的動作
            } // if
            else {
              TreeNode* rightptr = inputPtr->m_right ;
              inputPtr = Get_DefObject_Ptr( object.object_name ) ;
              inputPtr->m_right = rightptr ;
              return ReadLeft( inputPtr ) ;
            } // else
          } // if
          else if ( token_type == FUNCTION ) {
            nowFunction = GetFromFunction( inputPtr->m_left_token->m_token_string ) ;
            return ReadFunction( inputPtr, nowFunction ) ; 
            // 如果有定義過就可以進來 會根據function的名字以及他的m_argument做出相對應的動作
          } // else if

        } // else
      } // if 如果他是symbol的話要先確定有沒有定義過 如果沒有定義過要直接拋出錯誤

      if ( 1 ) throw invalid_argument( "Function Error" ) ;
      return new TreeNode( NIL ) ;
      // 如果這邊沒有定義過 那會拋出一個error 自然就會被整個丟出去
      // 由於left後面一定要接一個function 所以如果找不到function就代表這個東西不對了
    } // else if
    else if ( ! inputPtr->m_left ) {
      return new TreeNode( NIL ) ;
    } // else if
    else {
      cout << "Type error please Check" << endl ;
      throw ( "Type Error" ) ;
    } // else
  } // try
  catch( exception &e ) {
    if ( strcmp( e.what(), "No Return" ) == 0 ) {
      bool enter ;
      TreeNode* newPtr = new TreeNode( false ) ;
      newPtr->m_type = LISP ;
      newPtr->m_left = inputPtr ;
      cout << "ERROR (no return value) : " ;
      printer.PrintLisp( newPtr, 0, enter ) ;
      DeletePtr( newPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Format Error" ) == 0 ) {
      bool enter ;
      TreeNode* newPtr = new TreeNode( false ) ;
      newPtr->m_type = LISP ;
      newPtr->m_left = inputPtr ;
      printer.PrintLisp( newPtr, 0, enter ) ;
      DeletePtr( newPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Function Error" ) == 0 ) {
      printer.ErrorFunction( inputPtr ) ;
    } // if
    else if ( strcmp( e.what(), "Normal Exit" ) == 0 ) {
      if ( 1 ) throw invalid_argument( e.what() ) ;
    } // else if

    if ( 1 ) throw invalid_argument( "Error" ) ;
  } // catch

  return new TreeNode( NIL ) ;
} // ReadLeft()


TreeNode* CheckFinal( TreeNode* inputPtr, bool right ) {

  if ( ! inputPtr ) {
    return NULL ;
  } // if

  if ( inputPtr->m_can_read && right == false && ! inputPtr->m_dont_exp ) {
    if ( inputPtr->m_left != NULL && inputPtr->m_right == NULL && inputPtr->m_right_token == NULL ) {
      inputPtr = inputPtr->m_left ;
      inputPtr = CheckFinal( inputPtr, false ) ;
    } // if
  } // if

  if ( inputPtr->m_dont_exp ) {
    return inputPtr ;
  } // if

  inputPtr->m_left = CheckFinal( inputPtr->m_left, false ) ;
  inputPtr->m_right = CheckFinal( inputPtr->m_right, true ) ;
  return inputPtr ;

} // CheckFinal()

// ------------------------------------------------------------更新-------------------------------------------

// ------------------------------------------------------------NEW-------------------------------------------
TreeNode* Statement :: Exe_S_EXP( TreeNode* inputPtr ) {
/*
這邊比較複雜 我寫多一點註解以免大家搞不懂這裡的構想 每次看到這裡都覺得寫這個的人是個天才
我們在做這部分之前就會先建好一個 tree了 但是這個樹只是一個初步的雛型 我們要先對他進行預處理
最後我們要使用的是outputPtr那個pointer 由於他已經是指向記憶體位置 因此我們不用加"&"號
*/  
  if ( !IsList( inputPtr->m_left ) ) {
    cout << "ERROR (non-list) : " ;
    bool enter = false ;
    PrintLisp( inputPtr, 0, enter ) ;
    DeletePtr( inputPtr ) ;
    throw invalid_argument( "Error (non-list)" ) ; 
  } // if

  if ( inputPtr->m_type == LISP ) {
    return ReadLeft( inputPtr->m_left ) ; 
  }  // if 如果inputPtr指向的是一個LISP 那它代表的就是"("這東西 因此我們要進入ReadLEFT來確認他新的pointer 我給你的東西
     // 一樣是一個vector 那邊不會變 所以你可以放心地寫 不用怕
  else if ( inputPtr->m_type == QUOTE ) {
    return ReadQuote( inputPtr->m_left ) ; 
  } // else if 在他是一個TOKEN的情況下 我們要先知道她有沒有被宣告過 有一個全域變數會專門處理這件事情 我很高機率會把這東西
    // 名字設定為 g_def_object ( 取名由來為: 全域的 定義的 物件 ( 這裡處理的其實不算是變數 比較像是物件 ) ),
    // 那這邊應該會要有一個funciton 是一個 Get_DefObject_Ptr( 取名由來: 得到 定義的 物件的 指向 ) 他回傳的會是一個TreeNode *
    // 所以說 我們還會有一個struct 名為 Object 他裡面會放兩種東西 一個是該物件的名稱 我們用string 
    // 一個是他所指向的記憶體位置TreeNode * 那這樣這個Get_DefObject_ptr 就可以工作了 這樣就不用擔心 定義的部分該怎麼辦
    // 差不多在370行左右的地方
  else {
    cout << "Type error please Check" << endl ;
    throw ( "TYPE Error" ) ;
  } // else 這邊正常來說是不可能進入的 如果進入 一定是前面有寫錯 要回去看!

  return NULL ;
} // Statement::Exe_S_EXP()
// ------------------------------------------------------------NEW-------------------------------------------

void CheckNIL( TreeNode* inputPtr ) {
  if ( inputPtr->m_left )
    CheckNIL( inputPtr->m_left ) ;
  if ( inputPtr->m_right )
    CheckNIL( inputPtr->m_right ) ;

  if ( inputPtr ) {
    if ( inputPtr->m_right_token ) {
      if ( inputPtr->m_right_token->m_type == NIL ) {
        inputPtr->m_right_token = NULL ;
      } // if
    } // if

    if ( inputPtr->m_left ) {
      if ( inputPtr->m_left->IsNIL() ) {
        inputPtr->m_type = TOKEN ;
        inputPtr->m_left_token = new Token( "nil", 0, 0, NIL ) ;
        inputPtr->m_left = NULL ;
      } // if
    } // if
  } // if
} // CheckNIL()


void DeletePtr( Token* inputToken ) {

  if ( inputToken )
    delete inputToken ;

} // DeletePtr()

void DeletePtr( TreeNode* inputPtr ) {

  if ( inputPtr ) {
    DeletePtr( inputPtr->m_left_token ) ;
    DeletePtr( inputPtr->m_right_token ) ;
    DeletePtr( inputPtr->m_left ) ;
    DeletePtr( inputPtr->m_right ) ;
    delete inputPtr ;
  } // if

} // DeletePtr()

void Statement :: PrintStatementResult( vector<Token> &wait_token_vector ) {


  bool enter = false ;
  bool quote = false ;  
  TreeNode* outputPtr = NULL ;
  if (  wait_token_vector.front().m_type == LEFT_PAREN || wait_token_vector.front().m_type == QUOTE ) {
    m_nowTreePtr = new TreeNode( false ) ;
    m_nowTreePtr->m_left = new TreeNode() ;
    m_nowTreePtr->m_left_ok = false ;
    m_nowTreePtr->m_type = LISP;
    int type = wait_token_vector.front().m_type ;
    wait_token_vector.erase( wait_token_vector.begin() ) ;
    if ( type == LEFT_PAREN ) {
      CreateLisp( m_nowTreePtr->m_left, wait_token_vector, false ) ;
      outputPtr = Exe_S_EXP( m_nowTreePtr ) ;
      while ( outputPtr->m_type == LISP && outputPtr->m_left->m_left_token &&
              outputPtr->m_left->m_left_token->m_token_string == "lambda" ) 
        outputPtr = Exe_S_EXP( outputPtr ) ;
    } // if 
    else if ( type == QUOTE ) {
      if ( wait_token_vector.front().m_type == LEFT_PAREN ) {
        CreateLisp( m_nowTreePtr->m_left, wait_token_vector, false ) ;
        outputPtr = m_nowTreePtr->m_left ;
      } // if
      else {
        outputPtr = CreateToken( wait_token_vector.front() ) ;
        wait_token_vector.erase( wait_token_vector.begin() ) ;
      } // else if

      if ( outputPtr->m_left != NULL && outputPtr->m_left->IsNIL() ) {
        outputPtr->m_left = NULL ;
      } // if

      quote = true ;
    } // else if
  } // if
  else if ( wait_token_vector.size() == 1 ) {
    if ( wait_token_vector.front().m_type == SYMBOL ) {
      if ( IsDefinedOrNot( wait_token_vector.front().m_token_string ) ) {
        outputPtr = Get_DefObject_Ptr( wait_token_vector.front().m_token_string ) ;
        if ( outputPtr->m_type == LISP ) {
          outputPtr = Exe_S_EXP( outputPtr ) ;
        } // if
      } // if
      else ErrorSymBol( wait_token_vector.front().m_token_string ) ;
    } // if
    else {
      outputPtr = CreateToken( wait_token_vector.front() ) ;
    } // else 

    wait_token_vector.erase( wait_token_vector.begin() ) ;
  } // else if

  if ( outputPtr ) {
    outputPtr->m_can_read = false ;
    // if ( quote == false )
    // outputPtr = CheckFinal( outputPtr, true ) ;
  } // if
  else {
    outputPtr = new TreeNode( NIL ) ;
  } // else

  if ( outputPtr->m_left ) 
    CheckNIL( outputPtr->m_left ) ;
  if ( outputPtr->m_right )
    CheckNIL( outputPtr->m_right ) ;
  CheckEXP( outputPtr ) ;
  PrintLisp( outputPtr, 0, enter ) ;
  DeletePtr( outputPtr ) ;

} // Statement::PrintStatementResult()

void Statement :: GetStatement() {
  g_level = 0 ;
  Token token ;
  stack<Token> wait_token_stack ;
  vector<Token> wait_token_vector ;
  wait_token_vector.reserve( 100 ) ;
  try {
    cout << "" ;
    cout << endl << "> " ;
    cout << "" ;
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
    } // else if
    else if ( strcmp( e.what(), "Token Error" ) == 0 ) {
      int x  = 0 ;
    } // else if
    else {
      m_pl_tokenGetter.Reload() ;
    } // else
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

static int uTestNum = 0 ;

int main() {
  
  cout << "Welcome to OurScheme!" << endl ;

  g_costum_def_function.reserve( 100 ) ;
  g_custom_function.reserve( 100 ) ;
  g_def_object.reserve( 100 ) ;
  g_predef_function.reserve( 100 ) ;
  LoadPreDefFunction() ;
  // LoadPreDefFunctionTwo() ; 
  char testNum[10] ;
  // cin.getline( testNum, 10 ) ;
  char ch = '\0' ;
  scanf( "%d%c", &uTestNum, &ch ) ;
  // if ( strcmp( testNum, "1" ) == 0 ) {
  GetTokenMachine getToken ;
  TokenClassCategory tokenclass ;
  Statement statement( getToken, tokenclass ) ;
  statement.PrintAllOfStatement() ;
  // } // if
  
} // main()
