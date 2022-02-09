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
void error_message(int ruleBroken){
		if (ruleBroken == 8) {
			cout<<"Error: Absolute address exceeds machine size; zero used";
		}
		if (ruleBroken == 9) {
			cout<< "Error: Relative address exceeds module size; zero used";
		}
		if (ruleBroken == 6) {
			cout<< "Error: External address exceeds length of uselist; treated as immediate";
		}
		// if (ruleBroken==3) {
		// 	cout<< ("Error: " + sym + " is not defined; zero used");
		// }
		if (ruleBroken==2) {
			cout<< "Error: This variable is multiple times defined; first value used";
		}
		if (ruleBroken==10) {
			cout<< "Error: Illegal immediate value; treated as 9999";
		}
		if (ruleBroken==11) {
			cout<< "Error: Illegal opcode; treated as 9999";
		}
		// if (ruleBroken==5) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" to big "+to_string(address)+" (max="+to_string(max)+") assume zero relative");
		// }
		// if (ruleBroken==7) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" appeared in the uselist but was not actually used");
		// }
		// if (ruleBroken==4) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" was defined but never used");
		// 
}
	



// int linePos(string str){
//     ifstream f;
//     string buffer;
//     int offset;
//     int i = 1;
    
//     f.open(filename);
//     while(getline(f, buffer)){
//         //cout<<buffer;
//         offset = buffer.find(str);

//         if (offset != -1){
//             return i;
//         }
//         else{
//             //getline(f, buffer);
//             i++;
//             continue;
//         }

//     }
//     return -1;
// }
// int parser(string str){
//     ifstream f;
//     string buffer;
//     int offset;
//     //int i = 1;
    
//     f.open(filename);
//     while(getline(f, buffer)){
//         //cout<<buffer;
//         offset = buffer.find(str);
//         cout<< buffer<<endl;
//         if (offset != -1){
//             return offset + 1;
//         }
//         else{
//             //getline(f, buffer);
//             //i++;
//             continue;
//         }

//     }
//     return -1;
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
void print_symbol_table(){
    //int num = symbolVal + offSet;
    cout<< "Symbol Table: "<< endl;
    for (int i = 0; i < symbol_value.size(); i++){
        cout<< symbol_name.at(i) << " = " << symbol_value.at(i) << endl;
    }
}
int replace(int operand, int absolute_address){
    int temp = operand / 1000;
    return ((temp * 1000) + absolute_address);
}
void R_Instruction(int operand){
    int num = operand + relative_offSet;
    cout<< memory<< ": " << num<< endl;
    update_memory();
}
void I_Instruction(int operand){
    if (operand > 9999){
        operand = 9999;
        error_message(10);
    }
    cout<< memory<< ": " << operand<< endl;
    update_memory();
}
void A_Instruction(int operand){
    int num = operand % 1000;
    if (num > Max_Memory){
        ;
    }
    cout<< memory<< ": " << operand<< endl;
    update_memory();
}
void E_Instruction(int operand){
    // int opcode = operand / 1000;
    // int i = (opcode * 1000) % operand;
    // int num = replace(operand, stoi(symbol_value.at(i)));
    int index = operand % 1000;
    //cout<< "index " << index << endl;
    //cout<< "use_list" << use_list.at(index)<< endl;
    //cout<< "use_list " << use_list.at(index) << endl;
    std::vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), use_list.at(index));
    int index1 = distance(symbol_name.begin(), itr);
    //cout<< "index " << index1 << endl;
    int absolute_address = stoi(symbol_value.at(index1));
    int num = replace(operand, absolute_address);
    cout<<memory<<": " << num<< endl;
    update_memory();
}
void insert_symbolvalue(string str, int address){
    vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), str);
 
    if (itr != symbol_name.cend()) {
        int index = distance(symbol_name.begin(), itr);
        symbol_value.at(index) = to_string(address);
        
    }
    else {
        std::cout << "Element not found"<< endl;
    }
}
std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string reduce(const std::string& str,
                   const std::string& fill = " ",
                   const std::string& whitespace = " \t\n")
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

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

void pass1(){
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
    ifstream f;
    string buffer;
    string str;
    //int relative_offset = 0;
    f.open(filename);
    while(getline(f, buffer)){
        //cout<<buffer;
        for(int i = 0; i < buffer.length(); i ++){
            str.push_back(buffer.at(i));
        }
        str.push_back(' ');
    }
    f.close();
    //cout << str << endl;
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
                update_memory();

 
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
        str = reduce(str);

    }
    print_symbol_table();
}

void pass2(){
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
    int flag = 0;
    string possible_uselist = "";
    ifstream f;
    string buffer;
    string str;
    //int relative_offset = 0;
    f.open(filename);
    while(getline(f, buffer)){
        //cout<<buffer;
        for(int i = 0; i < buffer.length(); i ++){
            str.push_back(buffer.at(i));
        }
        str.push_back(' ');
    }
    f.close();
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
                    
                    if (!(is_digits(next_tok))){
                        
                        use_list.push_back(str_tok);
                        //cout<< use_list.at(0)<<endl;
                    }
                    else if (module_size == 1){
                        possible_uselist = str_tok;
                    }
                    module_size -= 1;
                }
            }
            if (str_tok == "E" || str_tok == "A" || str_tok == "R" || str_tok == "I"){
                flag = 1;
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
        // else if (module_size == 1){

        // }
        else{
            relative_offSet = stoi(memory);
            if (is_digits(str_tok) && str_tok != "0"){
                if (is_digits(next_tok)){
                    int tmp = stoi(str_tok) + relative_offSet;
                    if (possible_uselist != ""){
                        possible_uselist = "";
                    }
                    //symbol_value.push_back(to_string(tmp));
                    insert_symbolvalue(last_tok, tmp);
                    module_size = -1;
                }
                else{
                    module_size = stoi(str_tok);
                    if (possible_uselist != ""){
                        use_list.push_back(possible_uselist);
                    }
                    if (flag == 1){
                        //cout<< "use_list"<< use_list.at(0)<< endl;
                        use_list.clear();
                        flag = 0;
                    }
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
        str = reduce(str);
    }
}

///////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
    filename = argv[1];

    memory = "000";
    relative_offSet = 0;
    
    pass1();
//     return 0;
// }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ pass 2
    memory = "000";
    relative_offSet = 0;
    cout<<"Memory Map"<< endl;
    pass2();
     return 0;
}
