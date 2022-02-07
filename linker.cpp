#include <cstdlib>
#include<sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//static const string& file_name;
static int line_num;
const int Max_Memory = 512;
const int Max_Symbol_Characters = 16;
int offSet;
int relative_offSet;
string memory; 
vector<string> use_list;
int absolute_address[1];






class ErrorWarning {
	int ruleBroken;
	int module;
	int address;
	int max;
	string sym;
public:
	void setMod(int m) {
		module = m;
	}
	void setAddr(int a) {
		address = a;
	}
	void setMax(int m) {
		max = m;
	}
	void setSym(string s) {
		sym = s;
	}
	string getName() {
		return sym;
	}
	void setRule(int r) {
		ruleBroken = r;
	}
	int getRule() {
		return ruleBroken;
	}
	string msgString() {
		if (ruleBroken == 8) {
			return  "Error: Absolute address exceeds machine size; zero used";
		}
		if (ruleBroken == 9) {
			return "Error: Relative address exceeds module size; zero used";
		}
		if (ruleBroken == 6) {
			return "Error: External address exceeds length of uselist; treated as immediate";
		}
		if (ruleBroken==3) {
			return ("Error: " + sym + " is not defined; zero used");
		}
		if (ruleBroken==2) {
			return "Error: This variable is multiple times defined; first value used";
		}
		if (ruleBroken==10) {
			return "Error: Illegal immediate value; treated as 9999";
		}
		if (ruleBroken==11) {
			return "Error: Illegal opcode; treated as 9999";
		}
		if (ruleBroken==5) {
			return ("Warning: Module "+to_string(module+1)+": "+sym+" to big "+to_string(address)+" (max="+to_string(max)+") assume zero relative");
		}
		if (ruleBroken==7) {
			return ("Warning: Module "+to_string(module+1)+": "+sym+" appeared in the uselist but was not actually used");
		}
		if (ruleBroken==4) {
			return ("Warning: Module "+to_string(module+1)+": "+sym+" was defined but never used");
		}
		return "NONE";
	}
	
};

bool isWhiteSpace(const char* v, int i){
    if (v[i] == ' ' | v[i] == '\t' | v[i] == '\v' | v[i] == '\f'){
        return true;
    }
    return false;
}

// class Module {
// 	int start;
// 	int end;
// 	int len;
// 	//list of defs
// 	vector<Symbol> defList;
// 	//list of uses
// 	vector<string> useList;
// int symbol(){
//     ifstream f;
//     string buffer;
//     int offset;
//     int i = 1;

// }

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

// Pass1() {
//       while (!eof) {
//             createModule();
//             int defcount = readInt();
//             for (int i=0;i<defcount;i++) {
//                  Symbol sym = readSym();
//                  int val = readInt();
//                  createSymbol(sym,val);
//             }
//             int usecount = readInt();
//             for (int i=0;i<usecount;i++) {
//                   Symbol sym = readSym();
//             }
//             int instcount = readInt();
//             for (int i=0;i<instcount;i++) {
//                  char addressmode = ReadIEAR();
//                  int  operand = ReadInt();
//             }
//       }

// } 

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
// void parser1(const string& filename){
//     char str[128];
//     char *tok;
//     int i = 0;
//     int module_size;
//     ifstream myfile;
//     char delim[] = " \n\t";
//     myfile.open (filename);
//     while(myfile.getline(str, 128)){
//         tok = strtok(str, delim);
//         if (i == 0){
//             int module_size = tok[0];
//             int offSet = getOffset(tok[0], filename);

//         }
//     }
//     myfile.close();

//     tok = strtok(str, delim);
//     printf("str<%s> tok<%s>\n",str,tok);


// }
void update_memory(){
    int temp = stoi(memory);
    temp++;
    memory = to_string(temp);
    if (temp < 100){
        if (temp < 10){
            string add_zero = "00";
            memory.insert(0, add_zero);
        }
        else{
            memory.insert(0, 1, '0');
        }
    }
}
int assign_symbol_value(int symbolVal){
    int symbol = symbolVal + offSet;
    return symbol;
}
int replace(int operand, int absolute_address){
    int temp = operand / 1000;
    return ((temp * 1000) + absolute_address);
}
void R_Instruction(int operand){
    int num = operand + relative_offSet;
    cout<< memory<< " : " << num<< endl;
    update_memory();
}
void I_Instruction(int operand){
    cout<< memory<< " : " << operand<< endl;
    update_memory();
}
void A_Instruction(int operand){
    cout<< memory<< " : " << operand<< endl;
    update_memory();
}
void E_Instruction(int operand){
    int opcode = operand / 1000;
    int i = (opcode * 1000) % operand;
    int num = replace(operand, absolute_address[i]);
    cout<<memory<<" : " << num<< endl;
    update_memory();
}



int main(int argc, char** argv){
    //file_name = argv[1];
    line_num = 0;
    //parser1(argv[1]);
    int symbolVal = 2;
    offSet = 6;
    memory = "020";
    int result = assign_symbol_value(symbolVal);
    cout<< "symbol = "<< result<< endl;

    int operand = 7001;
    relative_offSet = 5;
    R_Instruction(operand); 


    absolute_address[0] = 15;
    E_Instruction(7000);
    I_Instruction(1004);
   // cout<<memory<<endl;
    // for(int i=1; i < argc; i++) // i=1, assuming files arguments are right after the executable
    // {
    //     int offset = getOffset("a", argv[1]);
    //     int line = linePos("a", argv[1]);
    //     cout<< "offset "<< offset<< "line "<< line<< endl;
    //     offset = getOffset("b", argv[1]);
    //     line = linePos("b", argv[1]);
    //     cout<< "offset "<< offset<< "line "<< line<< endl;
    //     offset = getOffset("c", argv[1]);
    //     line = linePos("c", argv[1]);
    //     cout<< "offset "<< offset<< "line "<< line<< endl;
    //     // cout<< offset<<endl;
    //     offset = getOffset("g", argv[1]);
    //     line = linePos("g", argv[1]);
    //     cout<< "offset "<< offset<< "line "<< line<< endl;
    //     // cout<< offset;
    //     return 0;
    // }
    
    return 0;
    // } 
}
