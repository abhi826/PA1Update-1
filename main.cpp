#include <iostream>
#include "lex.h"
#include <fstream>
#include <regex>
#include <set>
using namespace std;

int main(int argc, char* argv[] ) {
    //every  token  is  printed  when  it  is  seen followed  by  its lexeme between parentheses.
    bool vflag=false;
    //prints out all the unique integer constants in numeric order.
    bool inconstsflag=false;
    set<int> ints;
    //prints out  all  the  unique real constants  in  numeric order.
    bool rconstsflag=false;
    set<float> reals;
    //prints  out  all  the  unique string constants  in alphabetical order
    bool sconstsflag=false;
    set<string> strings;
    //prints out  all  of  the  unique  identifiers  in  alphabetical order.
    set<string> idents;
    bool idsflag=false;
    bool filePassed=false;
    string filename;

//argument parsing
    for(int i=1;i<argc;i++){
        string arg=argv[i];
        regex fileReg("[a-zA-Z0-9.]+");
        if(arg[0]=='-'){
            if(arg.compare("-v")==0){
                vflag=true;
            }
            else if( arg.compare("-iconsts")==0){
                inconstsflag=true;
            }
            else if( arg.compare("-rconsts")==0){
                rconstsflag=true;
            }
            else if(arg.compare("-sconsts")==0){
                sconstsflag=true;
            }
            else if(arg.compare("-ids")==0){
                idsflag=true;
            }
            else{
                cout<<"UNRECOGNIZED FLAG "<<arg<<endl;
                exit(1);
            }

        }
        else{
            if(regex_match(arg,fileReg)){
                if(filePassed==false){
                    filePassed=true;
                    filename=arg;
                }
                else{
                    cout<<"ONLY ONE FILE NAME ALLOWED"<<endl;
                    exit(1);
                }
            }
        }
    }
    if(filePassed==false){
       cout<<"No Specified Input File Name."<<endl;
        exit(1);
    }
    ifstream file;
    file.open(filename);
    if(!file){
        cout<<"CANNOT OPEN THE FILE "<<filename<<endl;
        exit(1);
    }
    int linenum=1;
    int tokenCount=0;
    LexItem tok;
    while((tok=getNextToken(file,linenum))!=DONE&&tok!=ERR){
        tokenCount++;
        if(vflag){
            cout<<tok<<endl;
        }
        if(tok==SCONST){
            strings.insert(tok.GetLexeme());
        }
        if(tok==ICONST){
            int num=stoi(tok.GetLexeme());
            ints.insert(num);
        }
        if(tok==RCONST){
            float num=stof(tok.GetLexeme());
            reals.insert(num);
        }
        if(tok==IDENT){
            idents.insert(tok.GetLexeme());
        }
    }
    if(tok==ERR){
        cout<<"Error in line "<<linenum<<" "<<"("<<tok.GetLexeme()<<")"<<endl;
        return 0;
    }
    cout<<"Lines: "<<linenum-1<<endl;
    cout<<"Tokens: "<<tokenCount<<endl;
    if(sconstsflag&&strings.size()>0){
        cout<<"STRINGS:"<<endl;
        for(string str:strings){
            cout<<str<<endl;
        }
    }
    if(inconstsflag&&ints.size()>0){
        cout<<"INTEGERS:"<<endl;
        for(int i:ints){
            cout<<i<<endl;
        }
    }
    if(rconstsflag&&reals.size()>0){
        cout<<"REALS:"<<endl;
        for(float f:reals){
            cout<<f<<endl;
        }
    }
    if(idsflag&&idents.size()>0){
        cout<<"IDENTIFIERS:"<<endl;
        int count=0;
        for(string id:idents){
            cout<<id;
            if(count<idents.size()-1){
                cout<<", ";
            }
            count++;
        }
        cout<<endl;
    }

    return 0;
}
