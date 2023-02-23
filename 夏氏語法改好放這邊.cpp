# include <iostream>
# include <string>
# include <fstream>
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
        for ( int i = 0 ; i < str.length() ; i++) {
            if ( str.at(i) < 48 || str.at(i) > 57 ) {
                return false ;
            }
        }
        return true ;
    }

    protected: bool HaveDot( string str ) {
        for ( int i = 0 ; i < str.length() ; i++ ) {
            if ( str.at(i) =='.' ) {
                return true ;
            }
        }
        return false ;        
    }

    protected: bool HaveOperate( string str ) {
        for ( int i = 0 ; i < str.length() ; i++  ) {
            if ( str.at(i) =='+' || str.at(i) == '-' ) {
                return true ;
            }
        }
        return false ;        
    }

    protected: bool HaveWord( string str ) {
        for ( int i = 0 ; i < str.length() ; i++  ) {
            if ( ( str.at(i) >= 65 && str.at(i) <= 90) || ( str.at(i) >= 97 && str.at(i) <= 122) ) {
                return true ;
            }
        }
        return false ;
    }

    public: int CheakCateGory( string token ) {
        if ( token == "nil" || token == "#f" ) {
            return NIL ;
        }
        else if ( token == "t" || token == "#t" ) {
            return T ;
        }

        bool haveOperate = HaveOperate(token) ; 
        bool haveDot =  HaveDot(token) ;
        bool haveWord = HaveWord(token) ;

        if ( haveDot && haveWord ) {
            return SYMBOL ;
        }

        for ( int i = 0 ; i < token.length() ; i++ ) {

            if ( token.at(i) == '(' ) {
                return LEFT_PAREN ;
            }
            else if ( token.at(i) == ')' ) {
                return RIGHT_PAREN ;
            }
            else if ( token.at(i) == '\'' ) {
                return QUOTE ;
            }

        }

        return 0 ;
    } // CheakCateGory()

} g_classCategory ;

class GetTokenMachine {
    protected: bool isFile ;
    protected: fstream fin ;
    protected: string token ;
    protected: bool end ;
    protected: char nextChar ; // used to cheak
    protected: char bufferDelimiter  ; // used to save delimiter
    protected: string bufferToken ;

    public: GetTokenMachine() {
        try{
            isFile = false ;
            token = "" ;
            end = false ;
            nextChar = '\0' ;
            bufferDelimiter = '\0' ;
            bufferToken = "" ;
        }
        catch( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
            throw invalid_argument(e.what())  ;            
        }
    }

    public: GetTokenMachine(string file) {
        try {
            isFile = true ;
            token = "" ;
            nextChar = '\0' ;
            bufferDelimiter = '\0' ;
            bufferToken = "" ;
            this->fin.open(file.c_str()) ;
            if ( !fin ) {
                throw invalid_argument("the file not open")  ;
            }
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
            throw invalid_argument(e.what())  ;
        }
    }

    protected: bool IsDelimiter(char ch) {
        if ( ch == '\"' || ch == '\'' || ch == '.' || ch == ',' || ch == '*' ||
            ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' ||
            ch == '^' || ch == '&' || ch == '(' || ch == ')' || ch == '[' ||
            ch == ']' || ch == '{' || ch == '}' || ch == '|' || ch == ';' ||
            ch == ':' || ch == ';' || ch == '/' || ch == '?' || ch == '<' || 
            ch == '=' || ch == '>' || ch == '+' || ch == '-' ) {
            return true ;
        }
        return false ;
    }

    protected: virtual string DelimiterDeal(string token) {
        token = token + string(1,bufferDelimiter) ;
        bufferDelimiter = '\0' ;
        return token ;
    }

    protected: virtual string CheakDelimiter() {
        this->bufferDelimiter = nextChar ;
        return "" ; 
    }


    protected: bool GetToken( string & token ) {
        try {
            bool isEmpty = false ;
            if ( bufferToken != "" ) {
                token = bufferToken ;
                bufferToken = "" ;
                return isEmpty ;
            }
            // if read delimite we need to save the token and return
            if ( bufferDelimiter != '\0' ) {
                token = DelimiterDeal(token) ;
                return isEmpty ;
            } // this means our buffer have a delimiter we need to use it first 
            cin.get(nextChar) ;
            while ( nextChar != '\t' &&nextChar !=' ' && nextChar != '\n' && ! IsDelimiter(nextChar) && ! cin.eof() ) {
                token = token + string(1,nextChar) ;
                cin.get(nextChar) ;
            }
            if ( IsDelimiter(nextChar) ) {
                token += CheakDelimiter() ;
            } // if nextChar is delimiter, it will leave while and go to here

            if ( ! cin.eof() ) {
                return isEmpty  ;
            }
            else return true ;
        }
        catch( exception &e ) {
            throw invalid_argument(e.what()) ;
        }

    }

    protected: bool GetFileToken( fstream &file,string & token ) {
        // if read delimite we need to save the token and return
        bool isEmpty = false ;
        if ( bufferDelimiter != '\0' ) {
            token = DelimiterDeal(token) ;
            return isEmpty ;
        } // this means our buffer have a delimiter we need to use it first 
        file.get(nextChar) ;
        while ( nextChar != '\t' &&nextChar !=' ' && nextChar != '\n' && ! IsDelimiter(nextChar) && ! file.eof() ) {
            token = token + string(1,nextChar) ;
            file.get(nextChar) ;
        }
        if ( IsDelimiter(nextChar) ) {
            CheakDelimiter() ;
        } // if nextChar is delimiter, it will leave while and go to here

        if ( ! file.eof() ) {
            return isEmpty  ;
        }
        else return true ;

    }


    public: bool GetNextToken(string &Out_token) {
        /*
        the function is we can get the token but we need to chiose Out_token != "" 
        */
       try {
        token = "" ;
        if ( ! end ) {
            do {
                if ( isFile == true ) {
                    end = GetFileToken(fin,token) ;
                }
                else {
                    end = GetToken(token) ;
                } 
                if( end == true ) {
                    Out_token = token ; 
                    return false ;
                }         
            } while ( token.length() == 0 && nextChar != '\n') ;
            Out_token = token ;
            return true ;
        }
        return false ;        
      }
      catch ( exception &e ) {
        cout << e.what() << endl  ;
        Out_token = "" ;
        if ( !cin.eof() ) {
          return (GetNextToken(Out_token)) ;
        }
        else {
          return false ;
        }
      }

    }

    public: char ReturnNextChar() {
        return nextChar ;
    }

    public: bool IsEnterChar() {
        if( nextChar == '\n' || end ) { 
            return true ;
        }
        else return false ;
    }

} getTokenMachine;

class PL_GetToken : public GetTokenMachine {


    protected: string ReadString( string token ) {
        token += bufferDelimiter ;
        bufferDelimiter = '\0' ;
        cin.get(nextChar) ;
        while( nextChar != '\n' && nextChar != '\"' && cin.eof() != true ) {
            token += nextChar ;
            cin.get(nextChar) ;
        }
        if ( nextChar == '\"' && ! cin.eof() ) {
            token += nextChar ;
        }
        else if ( nextChar == '\n' || cin.eof() == true  ) {
            string errorMessage = token + "->the token not exist"  ;
            throw invalid_argument(errorMessage.c_str())  ;
        }
        return token ;
    }

    protected: string ReadOperate(string token) {
        char tempDelimiter = bufferDelimiter ;
        bufferDelimiter = '\0' ;
        string tempToken = "" ;
        end = GetToken(token) ;
        if ( token != "" ) {
            return tempDelimiter + token ;
        }        
        else {
            return string(1,tempDelimiter) ;
        }
    }

    protected: string ReadPoundSign( string token ) {
        char tempDelimiter = bufferDelimiter ;
        bufferDelimiter = '\0' ;
        string tempToken = "" ;
        end = GetToken(token) ;
        if ( token != "" ) {
            return tempDelimiter + token ;
        }        
        else {
            return string(1,tempDelimiter) ;
        }        
    }

    protected: string ReadDot() {
          string token = "" ;
		  char lastChar = '\0' ;
		  cin.get(nextChar) ;
          while ( nextChar != '\t' &&nextChar !=' ' && nextChar != '\n' && ! IsDelimiter(nextChar) && ! cin.eof() ) {
			  token = token + string(1,nextChar) ;
			  lastChar = nextChar ;
			  cin.get(nextChar) ;
          }
		  cin.putback(nextChar) ;
		  nextChar = lastChar ;
          if ( token != "" ) {
            if ( g_classCategory.IsNum(token) ) {
                return "." + token ;
            }
            else {
                cin.putback(nextChar) ;
                nextChar = '\0' ;
                for ( int i = token.size() ; i >=0 ; i -- ) {
                    cin.putback(token[i]) ;
                }
                return "." ;
            }
          }        
          else {
            bufferDelimiter = '.' ;
            return "" ;
          }
    }



    protected: virtual string CheakDelimiter(){

      if ( nextChar == '.' ) {
        return ReadDot() ; 
      } // if
      else {
        return GetTokenMachine::CheakDelimiter() ;
      }
    } // 

    protected: virtual string DelimiterDeal( string token ) {
        try {
            if ( bufferDelimiter == '\"' ) {
                token = ReadString(token) ;
            }
            else if ( bufferDelimiter == '+' || bufferDelimiter == '-' ) {
                token = ReadOperate(token) ;
            } // else if
            else if ( bufferDelimiter == '#' ) {
                token = ReadPoundSign(token) ;
            }
            else {
                token = token + string(1,bufferDelimiter) ;
                bufferDelimiter = '\0' ;
            }
            return token ;
        }
        catch(exception &e) {
            throw invalid_argument(e.what()) ;
        }
    }

} pl_GetToken;

int main() {
    string token = "" ;
    bool end ;
    do {
        end = pl_GetToken.GetNextToken(token) ; // end is a bool means the page is EOF if end is false
        if ( token != "" ) {
            // here we can put some choise to get token like's string
            cout << token << ' ';
        } // the compare is very important!!! that can cheak the token does read something   
        if ( pl_GetToken.IsEnterChar() ) {
            cout << '\n' ;
        }     
    } while ( end ) ;   // this while is we can loading all token of page    
}

