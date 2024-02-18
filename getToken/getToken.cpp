/**
   author : gaxzipTGO
   date   : 2024/02/15
   propose: 此function的目的是得到下一個token 
            他會回傳一個int 代表有沒有得到東西 
            如果有的話 那會return 0
            如果沒有 則是return 1
            參數內擺一個char *得到的字串會在那邊
*/

#include <cstdio>
#include <cstring>
#include <map>

using namespace std ;
typedef function<int(char*, char)> GetTokenFunction ;

const int g_keywordSize = 12 ;
char keyword[] = {'(', ')', '+', '-', '*', ',', '/', EOF, '=', ';', '[', ']', ':', '<', '>'} ;
char nextChar = '\0' ;

void append(char* str, char newChar) {
    // 找到字符串的結尾位置
    int length = std::strlen(str);
    char* endOfString = str + length;

    // 在結尾位置添加新的字元
    *endOfString = newChar; // 添加新字元
    *(endOfString + 1) = '\0'; // 添加新的結束符
}

char getNextChar() {
    /**
     * 可以讓nextChar先偷看
     * 然後得到nextChar原本的那個
    */
    char tempChar = nextChar ;
    nextChar = getchar() ;
    return tempChar ;
}

bool isKeyword( char ch ) {
    /**
     * 確認是不是keyword
    */
   for ( int i = 0 ; i < g_keywordSize ; i ++ ) {
    if ( ch == keyword[i] ) return true ;
   }
   return false ;

}  // isKeyword()

bool isWhiteSpace( char ch ) {
    /**
     * 確認是不是whiteSpace
    */
    if ( ch == '\t' || ch == '\n' || ch == ' ' ) return true ;
    else return false ;
}

bool isComment( char ch ) {
    /**
     * @param ch ->準備要被放進去的char 要用nextChar確定是不是註解
    */
    if ( ch == '/' && nextChar == '*' ) {
        char commentEnd = '\0' ;
        do {
            commentEnd = getNextChar() ;
        } while( commentEnd != '*' || nextChar != '/' ) ;
        commentEnd = getNextChar() ;
        return true ;
    } // 一路讀到沒東西可以讀為止
    else if ( ch == '/' && nextChar == '/' ) {
        char commentEnd = '\0' ;
        do {
            commentEnd = getNextChar() ;
        } while( commentEnd != '\n' ) ;
        return true ;
    }
    else return false ;
}

int getDivideSign( char *str, char ch ) {
    if ( !isComment( ch ) ) {
        append( str, ch ) ;
        return 1 ;
    } // if

    else return 0 ;
} // dividesign()

int getComputeSign( char *str, char ch ) {
    /**
     * @param ch ->進來一定會是+ or - 就看後面是甚麼 
     * @return 得到此token的狀態
    */
    append( str, ch ) ;
    if ( nextChar == ch || nextChar == '=' ) {
        ch = getNextChar() ;
        append( str, ch ) ;
    } // if

    return 1 ;
}

int getColonSign( char *str, char ch ) {
    /**
     * @param ch ->進來一定是':'
     * @return 得到此token的狀態
    */
    append( str, ch ) ; 
    if ( nextChar == '=' ) {
        ch = getNextChar() ;
        append( str, ch ) ;
    } // if

    return 1 ;

} // getColonSign()

int getSmallerSign( char *str, char ch ) {
    /**
     * @param ch ->進來一定是'<'
     * @return 得到此token的狀態
    */
    append( str, ch ) ;
    if ( nextChar == '>' || nextChar == '=' ) {
        ch = getNextChar() ;
        append( str, ch ) ;
    } // if

    return 1 ;

} // getSmallerSign()

int getBiggerSign( char *str, char ch ) {
    /**
     * @param ch ->進來一定是'>'
     * @return 得到此token的狀態
    */
    append( str, ch ) ;
    if ( nextChar == '=' ) {
        ch = getNextChar() ;
        append( str, ch ) ;
    } // if

    return 1 ;
} // getSmallerSign()

typedef map<char, GetTokenFunction> tokenFunction ;
tokenFunction g_TokenFunctionMap ;

void initialTokenFunctionMap() {
    g_TokenFunctionMap.emplace( '/', getDivideSign ) ;
    g_TokenFunctionMap.emplace( '+', getComputeSign ) ;
    g_TokenFunctionMap.emplace( '-', getComputeSign ) ;
    g_TokenFunctionMap.emplace( ':', getColonSign ) ;
    g_TokenFunctionMap.emplace( '<', getSmallerSign ) ;
    g_TokenFunctionMap.emplace( '>', getBiggerSign ) ;
}

int keywordAppendChar( char *str, char ch ) {
    /**
     * @returns ->各種不同的狀態 根據不同的狀態代表不同的反應
     *          0->正常情況
     *          1->不能加進去
     *          2->特殊狀況
     * @param   str->一定會有資料 長度必定 == 1
     * @param   ch ->準備要被添加的 要與str做比對
    */
    if ( isKeyword( ch ) ) {
        try
        {
            return g_TokenFunctionMap[ch](str, ch) ;
        }
        catch(const std::exception& e)
        {
            append( str, ch ) ;
            return 1 ;
        }
    } // if
    else {
        append( str, ch ) ;
    } // else
    return 0 ;
}

int appendChar( char *str,  char ch ) {
    /**
     * 確認是不是whiteSpace 如果不是的話 就可以往後補進去
     * @return ->確認是否要繼續做下去 或是需要被跳過
     *         ->0:要繼續做
     *         ->1:跳過，讓這個token先被送出
    */
    if ( isKeyword( ch ) ) {
        return keywordAppendChar( str, ch ) ;
    } // if 如果他的第一項是keyword 那要特殊處理, 要先偷看準備要進來的是什麼 根據那個要進來的東西做不同的動作
    if ( ! isWhiteSpace( ch ) ) {
        append( str, ch ) ;
    } 
    return 0 ;

} // appendChar()

int getNextToken( char *token ) {
    /**
     * 得到一個token並且把它送回去 根據狀況回傳0 or 1
    */
    if ( nextChar == EOF ) return 1 ;
    char nextToken[255] = "" ;
    do { 
        char curChar = getNextChar() ;
        if ( appendChar( nextToken, curChar ) == 1 ) {
            strcpy( token, nextToken ) ;
            return 0 ;
        } // if 空白會被略過，如果在appendChar == 1 的情況 代表要先將此token送出去

    } while( ! isKeyword( nextChar ) && ! isWhiteSpace( nextChar ) ) ;


    strcpy( token, nextToken ) ;
    return 0 ;    
}

int main() {
    initialTokenFunctionMap() ;
    char token[255] = "" ;
    while ( getNextToken( token ) != 1 ) {
        if ( strcmp( token, "" ) != 0 ) {
            printf( "%s\n", token ) ;
        }
    }
    printf("bye\n") ;
} // main