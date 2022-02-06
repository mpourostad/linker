#include <cstdlib>
#include<sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

static int line_num;
const int Max_Memory = 512;
const int Max_Symbol_Characters = 16;
vector<string> readFileToVector(const string& filename)
{
    ifstream source;
    source.open(filename);
    vector<string> lines;
    string line;
    while (getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}
bool isWhiteSpace(const char* v, int i){
    if (v[i] == ' ' | v[i] == '\t' | v[i] == '\v' | v[i] == '\f'){
        return true;
    }
    return false;
}

// void __parseerror(int errcode) {
//     static char* errstr[] = {
//         "NUM_EXPECTED",
//         "SYM_EXPECTED",
//         "ADDR_EXPECTED",
//         "SYM_TOO_LONG",
//         "TOO_MANY_DEF_IN_MODULE",
//         "TOO_MANY_USE_IN_MODULE",
//         "TOO_MANY_INSTR", };
//     printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]); 
// }

Pass1() {
      while (!eof) {
            createModule();
            int defcount = readInt();
            for (int i=0;i<defcount;i++) {
                 Symbol sym = readSym();
                 int val = readInt();
                 createSymbol(sym,val);
            }
            int usecount = readInt();
            for (int i=0;i<usecount;i++) {
                  Symbol sym = readSym();
            }
            int instcount = readInt();
            for (int i=0;i<instcount;i++) {
                 char addressmode = ReadIEAR();
                 int  operand = ReadInt();
            }
      }

} 

int linePos(string str, const string& filename){
    ifstream f;
    string buffer;
    int offset;
    int i = 1;
    
    f.open(filename);
    while(getline(f, buffer)){
        //cout<<buffer;
        offset = buffer.find(str);

        if (offset != -1){
            return i;
        }
        else{
            //getline(f, buffer);
            i++;
            continue;
        }

    }
    return -1;
}
int getOffset(string str, const string& filename){
    ifstream f;
    string buffer;
    int offset;
    //int i = 1;
    
    f.open(filename);
    while(getline(f, buffer)){
        //cout<<buffer;
        offset = buffer.find(str);

        if (offset != -1){
            return offset + 1;
        }
        else{
            //getline(f, buffer);
            //i++;
            continue;
        }

    }
    return -1;
}
int main(int argc, char** argv){

    line_num = 0;
    for(int i=1; i < argc; i++) // i=1, assuming files arguments are right after the executable
    {
        int offset = getOffset("a", argv[1]);
        int line = linePos("a", argv[1]);
        cout<< "offset "<< offset<< "line "<< line<< endl;
        offset = getOffset("b", argv[1]);
        line = linePos("b", argv[1]);
        cout<< "offset "<< offset<< "line "<< line<< endl;
        offset = getOffset("c", argv[1]);
        line = linePos("c", argv[1]);
        cout<< "offset "<< offset<< "line "<< line<< endl;
        // cout<< offset<<endl;
        offset = getOffset("g", argv[1]);
        line = linePos("g", argv[1]);
        cout<< "offset "<< offset<< "line "<< line<< endl;
        // cout<< offset;
        return 0;
    }
    
    return 0;
    // } 
}
