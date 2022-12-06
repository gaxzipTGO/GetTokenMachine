#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std ;

class GetTokenMachine {
    protected: fstream fin ;
    protected: string token = "" ;
    protected: bool end = false ;
    protected: char nextChar = '\0' ; // used to cheak
    protected: char bufferDelimiter = '\0' ; // used to save delimiter

    public: GetTokenMachine(string file) {
        this->fin.open(file) ;
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

    protected: bool GetFileToken( fstream &file,string & token ) {
        // if read delimite we need to save the token and return
        bool isEmpty = false ;
        if ( bufferDelimiter != '\0' ) {
            token = token + string(1,bufferDelimiter) ;
            bufferDelimiter = '\0' ;
            return isEmpty ;
        } // this means our buffer have a delimiter we need to use it first 
        file.get(nextChar) ;
        while ( nextChar != '\t' &&nextChar !=' ' && nextChar != '\n' && ! IsDelimiter(nextChar) && ! file.eof() ) {
            token = token + string(1,nextChar) ;
            file.get(nextChar) ;
        }
        if ( IsDelimiter(nextChar) ) {
            bufferDelimiter = nextChar ;
        } // if nextChar is delimiter, it will leave while and go to here

        if ( ! file.eof() ) {
            return isEmpty  ;
        }
        else return true ;

    }

    public: void Test() {
        char continueTime = 'y' ;
        while ( continueTime == 'y' && ! end ) {
            end = GetFileToken(fin,this->token) ;
            if ( token.length() > 0 ) {
                cout << token << endl ;
                token = "" ;             
            }
            //cout << "do yo countinue?" << endl ;
            //cin >> continueTime ; 
        } 
        cin >> token ;
    }

    public: bool GetNextToken(string &Out_token) {
        /*
        the function is we can get the token but we need to chiose Out_token != "" 
        */
        token = "" ;
        if ( ! end ) {
            do {
                end = GetFileToken(fin,token) ;
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

    public: char ReturnNextChar() {
        return nextChar ;
    }

    public: bool IsEnterChar() {
        if( nextChar == '\n' || end ) { 
            return true ;
        }
        else return false ;
    }
} ;


int main(int argc,char **argv) {
    cout << "hello world" << endl ;
}