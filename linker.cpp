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
string filename = "0";
int offSet;
int relative_offSet;
string memory; 
vector<string> use_list;
vector<string> symbol_name;
vector<string> symbol_value;
//int absolute_address[1];






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

int linePos(string str){
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
int parser(string str){
    ifstream f;
    string buffer;
    int offset;
    //int i = 1;
    
    f.open(filename);
    while(getline(f, buffer)){
        //cout<<buffer;
        offset = buffer.find(str);
        cout<< buffer<<endl;
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
void assign_symbol_value(string symbol, int symbolVal){
    int num = symbolVal + offSet;
    cout<< symbol<< " = "<< num<< endl;
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
    int num = replace(operand);
    cout<<memory<<" : " << num<< endl;
    update_memory();
}
void insert_symbolvalue(string str, int address){
    vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), str);
 
    if (itr != symbol_name.cend()) {
        int index = distance(symbol_name.begin(), itr);
        symbol_value.at(index - 1) = to_string(address);
        
    }
    else {
        std::cout << "Element not found"<< endl;
    }
}
// void pass1(){
//     ifstream f;
//     string buffer;
//     int offset;
    
//     f.open(filename);
//     while(getline(f, buffer)){
//         for (int i = 0; i < (buffer.length() - 1); i++){
//             if (isalnum(buffer[i])){
//                 offSet = getOffset(buffer[i]);
//             }
//         }
//     }
// }
/////////////////////////////////////////////////////////////////////////////////////////////////

bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

/*
 * Erase First Occurrence of given  substring from main string.
 */
void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

///////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
    // filename = argv[1];
    // line_num = 0;
    // //parser1(argv[1]);
    // int symbolVal = 2;
    // offSet = 6;
    memory = "000";
    relative_offSet = 0;
    // string sym = "xy";
    // assign_symbol_value(sym, symbolVal);
    // //cout<< "symbol = "<< result<< endl;
    // // cout<< "symbol = "<< endl;

    // int operand = 7001;
    // relative_offSet = 5;
    // R_Instruction(operand); 


    // absolute_address[0] = 15;
    // E_Instruction(7000);
    // I_Instruction(1004);

    // int test = parser(sym);
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





    string str = "1 xy 2 2 z xy 5 R 1004 I 5678      E 2000 R 8002 E 7001 0      1 z 6   R \n \n 8001 E 1000 E 1000 E 3000 R 1002 A 1010 0 1 z 2 R 5001 E 4000 1 z 2 2 xy z 3 A 8000 E 1001 E 2000";
    string str_tmp = "";
    char delim[] = " \t\n";
    char *tok_bck;
    char *tok;
    char *next_tok_char;
    // string last_tok = "gooz_init";
    string next_tok = "gooz_init";
    string last_tok = "whatever";
    char *dup;
    char *dup1;
    int module_size = -1;
    //int relative_offset = 0;

    cout << str << endl;
    while(str.length() > 1){
        str_tmp = str;
        dup = strdup(str.c_str());
        tok = strtok(dup, delim);
        std::string str_tok = std::string(tok);
        free(dup);

        eraseSubStr(str_tmp, str_tok);
        dup1 = strdup(str_tmp.c_str());
        next_tok_char = strtok(dup1, delim);
        std::string next_tok = std::string(next_tok_char);
        free(dup1);
///////////////////////////////////////////////////
        if (module_size == -1){
            // if (!(isdigit(tok))){
            //     ; // Do nothing
            // }
            module_size = stoi(str_tok);
        }
        if (module_size > 0){
            if (str_tok != "E" && str_tok != "A" && str_tok != "R" && str_tok != "I"){
                if (is_digits(str_tok)){
                    ;
                }
                else {
                    if (std::find(symbol_name.begin(), symbol_name.end(), str_tok) != symbol_name.end()){
                        ;
                    }
                    else{
                        symbol_name.push_back(str_tok);
                        symbol_value.push_back("-");
                    }
                    module_size -= 1;
                }
            }
            if (str_tok == "E" || str_tok == "A" || str_tok == "R" || str_tok == "I"){
                module_size -= 1;
                //cout << "alan: " << str_tok <<" next_tok: " << next_tok <<" rel: " << relative_offSet <<endl;

                if (str_tok == "E"){
                    E_Instruction(stoi(next_tok));
                }
                else if(str_tok == "R"){
                    R_Instruction(stoi(next_tok));
                }
                else if(str_tok == "I"){
                    I_Instruction(stoi(next_tok));
                }
                else{
                    A_Instruction(stoi(next_tok));
                }
                str = str_tmp;
            }
            std::string s;
            for (const auto &piece : symbol_name) s += piece;
        }
        else{
            relative_offSet = stoi(memory);
            if (is_digits(str_tok) && str_tok != "0"){
                if (is_digits(next_tok)){
                    int tmp = stoi(str_tok) + relative_offSet;
                    //symbol_value.push_back(to_string(tmp));
                    insert_symbolvalue(last_tok, tmp);
                    module_size = -1;
                }
                else{
                    module_size = stoi(str_tok);
                }
            }
        }
        //insert_symbolvalue("z");
/////////////////////////////////////////////////////
        dup = strdup(str.c_str());
        tok = strtok(dup, delim);
        int start_position_to_erase = str.find(tok);
        str.erase(start_position_to_erase, strlen(tok));
        last_tok = str_tok;
    }

    
    return 0;
    // } 
}
