//
// Created by 17323 on 2/22/2021.
//
#include "lex.h"
#include <map>
using namespace std;

const map<string,Token> tokenMap={{"program",PROGRAM},{"PROGRAM",PROGRAM},
                                  {"print",PRINT},{"PRINT",PRINT},{"read",READ},
                                  {"READ",READ},{"integer",INTEGER},{"INTEGER",INTEGER},{"end",END},{"END",END},
                                  {"IF",IF},{"if",IF},
                                  {"THEN",THEN},{"then",THEN},{"REAL",REAL},{"real",REAL},
                                  {"char",CHAR},{"CHAR",CHAR},{"+",PLUS},{"-",MINUS},{"*",MULT},
                                  {"/",DIV},{"//",CONCAT},{"=",ASSOP},{"(",LPAREN},
                                  {")",RPAREN},{"==",EQUAL},{"<",LTHAN},{":",COLON},{",",COMA} };


LexItem getNextToken(istream& in, int& linenum){

    enum LexState{
        START,INID,INSTRING,ININT,INREAL,INOP
    };
    LexState lexstate=START;
    string lexeme="";
    char ch;
    int stringStart=-1; //stringstart =0 if single quote and 1 for double quote
    while(in.get(ch)){

        switch(lexstate){
            case START:
                if(ch=='\n'){

                    linenum++;
                }
                if(isspace(ch)){
                    continue;
                }
                else if(isalpha(ch)){
                    lexstate=INID;
                    lexeme+=ch;
                }
                else if(ch=='"'||ch=='\''){
                    lexstate=INSTRING;
                    lexeme+=ch;
                    if(ch=='"'){
                        stringStart=1;
                    }
                    else{
                        stringStart=0;
                    }
                }
                else if(isdigit(ch)){
                    lexstate=ININT;
                    lexeme+=ch;
                }
                else if(ch=='.'){
                    lexstate=INREAL;
                    lexeme+=ch;
                }
                else if(ch=='!'){
                    while(ch!='\n'){
                        ch=in.get();
                    }
                    linenum++;
                }
                else if(ch==':'||ch==','){
                    lexeme+=ch;
                    if(ch==':'){
                        return LexItem(COLON,lexeme,linenum);
                    }
                    return LexItem(COMA,lexeme,linenum);
                }
                else if(ispunct(ch)){
                    lexstate=INOP;
                    lexeme+=ch;
                }
                break;

            case INID:
                if(isalnum(ch)){
                    lexeme+=ch;
                }
                else{
                    in.putback(ch);
                    map<string,Token>::const_iterator iter=tokenMap.find(lexeme);
                    if(iter!=tokenMap.end()){

                        return LexItem(iter->second,lexeme,linenum);
                    }
                    else{
                        string copy="";
                        for(int i=0;i<lexeme.length();i++){
                            char upper;
                            upper=toupper(lexeme[i]);
                            copy+=upper;
                        }

                        return LexItem(IDENT,copy,linenum);

                    }

                }
                break;

            case INSTRING:
                if(ch=='"'){
                    if(stringStart==1){
                        string copy="";
                        for(int i=1;i<lexeme.length();i++){
                            copy+=lexeme[i];
                        }
                        return LexItem(SCONST,copy,linenum);
                    }
                    else{
                        lexeme+=ch;
                    }

                }
                else if(ch=='\''){
                    if(stringStart==0){
                        string copy="";
                        for(int i=1;i<lexeme.length();i++){
                            copy+=lexeme[i];
                        }
                        return LexItem(SCONST,copy,linenum);
                    }
                    else{
                        lexeme+=ch;
                    }
                }
                else if(ch=='\n'){
                    return LexItem(ERR,lexeme,linenum);
                }
                else{
                    lexeme+=ch;
                }
                break;

            case ININT:
                if(ch=='.'){
                    char c;
                    c=in.peek();
                    if(isdigit(c)){
                        lexstate=INREAL;
                        lexeme+=ch;
                    }
                    else{
                        return LexItem(ERR,lexeme,linenum);
                    }
                }
                else if(!isdigit(ch)){
                    in.putback(ch);
                    return LexItem(ICONST,lexeme,linenum);
                }
                else if(isdigit(ch)){
                    lexeme+=ch;
                }

                break;

            case INREAL:
                if(isalpha(ch)){
                    lexeme+=ch;
                    return LexItem(ERR,lexeme,linenum);
                }
                if(isdigit(ch)){
                    lexeme+=ch;
                }
                else{
                    in.putback(ch);
                    return LexItem(RCONST,lexeme,linenum);
                }
                break;

            case INOP:
                char tempNext=ch;
                map<string,Token>::const_iterator iter=tokenMap.find(lexeme+tempNext);
                if(iter!=tokenMap.end()){
                    return LexItem(iter->second,lexeme+tempNext,linenum);
                }
                else{
                    in.putback(ch);
                    iter=tokenMap.find(lexeme);
                    if(iter!=tokenMap.end()){
                        return LexItem(iter->second,lexeme,linenum);
                    }
                    else{
                        return LexItem(ERR,lexeme,linenum);
                    }

                }

                break;
        }
    }
    return LexItem(DONE,"DONE",linenum);
}

 ostream& operator<<(ostream& out,const LexItem& tok){
    Token t=tok.GetToken();
    if(tok==IDENT){
        out<<tokenPrint[t]<<"("<<tok.GetLexeme()<<")";
    }
    else{
        out<<tokenPrint[t];
    }
    return out;

}
LexItem id_or_kw(const string& lexeme,int linenum){
    map<string,Token>::iterator iter=kwmap.find(lexeme);
    if(iter!=kwmap.end()){
        return LexItem(iter->second,lexeme,linenum);
    }
    else{
        return LexItem(IDENT,lexeme,linenum);
    }

}