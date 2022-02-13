#include <cstdlib>
#include<sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
#include<tuple>
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
vector<int> flag_uselist;
vector<string> deflist;

void error_message(int ruleBroken){
		if (ruleBroken == 8) {
			cout<<"Error: Absolute address exceeds machine size; zero used"<<endl;
		}
		if (ruleBroken == 9) {
			cout<< "Error: Relative address exceeds module size; zero used"<<endl;
		}
		if (ruleBroken == 6) {
			cout<< "Error: External address exceeds length of uselist; treated as immediate"<<endl;
		}
		// if (ruleBroken==3) {
		// 	cout<< ("Error: " + sym + " is not defined; zero used")<<endl;
		// }
		if (ruleBroken==2) {
			cout<< "Error: This variable is multiple times defined; first value used"<<endl;
		}
		if (ruleBroken==10) {
			cout<< "Error: Illegal immediate value; treated as 9999"<<endl;
		}
		if (ruleBroken==11) {
			cout<< "Error: Illegal opcode; treated as 9999"<<endl;
		}

		// if (ruleBroken==5) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" to big "+to_string(address)+" (max="+to_string(max)+") assume zero relative")<<endl;
		// }
		// if (ruleBroken==7) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" appeared in the uselist but was not actually used")<<endl;
		// }
		// if (ruleBroken==4) {
		// 	return ("Warning: Module "+to_string(module+1)+": "+sym+" was defined but never used")<<endl;
		// 
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
bool contains(vector<int> vec, int elem)
{
    bool result = false;
    if( find(vec.begin(), vec.end(), elem) != vec.end() )
    {
        result = true;
    }
    return result;
}
int find_symbol_index(string symb){

    std::vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), symb);
    return distance(symbol_name.begin(), itr);
}
void print_symbol_table(){
    //int num = symbolVal + offSet;
    cout<< "Symbol Table"<< endl;
    for (int i = 0; i < symbol_value.size(); i++){
        if(find(deflist.begin(), deflist.end(), symbol_name.at(i)) != deflist.end()) {
            cout<< symbol_name.at(i) << "=" << symbol_value.at(i) << " ";
            error_message(2);
        }
        else if (symbol_value.at(i) == "-"){
            ;
        }
        else{
            cout<< symbol_name.at(i) << "=" << symbol_value.at(i) << endl;
        }
    }
    cout<<'\n';
}
int replace(int operand, int absolute_address){
    int temp = operand / 1000;
    return ((temp * 1000) + absolute_address);
}
void R_Instruction(int operand, int mod){
    int address = operand % 1000;
    if (operand > 9999){
        operand = 9999;
        cout<< memory<< ": " << operand<< " ";       
        error_message(10);
    }
    else if (mod < address ){
        int opcode = operand / 1000;
        int num = (opcode * 1000) + relative_offSet; 
        cout<< memory<< ": " << num << " ";
        error_message(9);
    }
    else{
        int num = operand + relative_offSet;
        cout<< memory<< ": " << num<< endl;
    }
    update_memory();
}
void I_Instruction(int operand){
    if (operand > 9999){
        operand = 9999;
        cout<< memory<< ": " << operand<< " ";       
        error_message(11);
    }
    else{
        cout<< memory<< ": " << operand<< endl;
    }
    update_memory();
}
void A_Instruction(int operand){
    int num = operand % 1000;
    if (operand > 9999){
        cout<< memory<< ": " << "9999 ";
        error_message(10);
    }
    else if(num > Max_Memory){
        int opcode = operand / 1000;
        operand = opcode * 1000;
        cout<< memory<< ": " << operand<< " ";
        error_message(8);
    }
    else{
        cout<< memory<< ": " << operand<< endl;
    }
    update_memory();
}
void E_Instruction(int operand){
    if (operand > 9999){
        operand = 9999;
        cout<< memory<< ": " << operand<< " ";       
        error_message(10);
    }
    else{
        int index = operand % 1000;
        if (index > use_list.size() - 1){
            cout<<memory<<": " << operand<< " ";
            error_message(6);
        }
        else{
            flag_uselist.at(index) = 1;
            index = find_symbol_index(use_list.at(index));
            if (symbol_value.at(index) == "-"){
                int absolute_address = 0;
                int num = replace(operand, absolute_address);
                cout<<memory<<": " << num<< " Error: "<< symbol_name.at(index)<< " is not defined; zero used"<<endl;
            }
            else{
                int absolute_address = stoi(symbol_value.at(index));
                int num = replace(operand, absolute_address);
                cout<<memory<<": " << num<< endl;
            }
            
        }
    }
    update_memory();
}
void set_symbolvalue(string str, int address){
    vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), str);
    int index = distance(symbol_name.begin(), itr);
    if (itr != symbol_name.cend()) {
        if (symbol_value.at(index) != "-"){
            deflist.push_back(symbol_name.at(index));
        }
        else{
            symbol_value.at(index) = to_string(address);
        }
        
    }
    else {
        ;
        //cout<< "Error: "<< str<<" is not defined; zero used"<<endl;
        // symbol_value.at(index) = to_string(0);
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
void warning_5(vector<string> flag, int count, int module_size, int memory, int address){
    for(int i = 0; i  < flag.size(); i++){
        if(find(symbol_name.begin(), symbol_name.end(), flag.at(i)) != symbol_name.end()) {
            int index = find_symbol_index(flag.at(i));
            if(symbol_value.at(index) != "-" && stoi(symbol_value.at(index)) > module_size){
                // cout<< "Warning: Module " << count << ": "<< flag.at(i) << " to big " << memory << " (max=" << module_size - 1 << ") assume zero relative"<< endl;
                // cout<< "khar" << address<< endl;
                //symbol_value.at(index) = "0";
                // set_symbolvalue(flag.at(i), memory);
                ;
            }
        } 
        
    }

}

void pass1(){
    string str_tmp = "";
    char delim[] = " \t\n";
    char *tok_bck;
    char *tok;
    char *next_tok_char;
    // string last_tok = "gooz_init";
    string next_tok = "empty";
    string last_tok = "empty";
    char *dup;
    char *dup1;
    int module_size = -100;
    int module = 0; 
    int flag_module_start = -1;
    int count_module = 0;
    //int flag_symb = -1;
    vector<string> flag_symb_val;
    int khar = -100;
    int gav = -100;
    // int module_size_holder = 0;
    // int warning = 0;
  
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
        if (module_size == -100){
            // if (!(isdigit(tok))){
            //     ; // Do nothing
            // }
            module_size = stoi(str_tok);
        }
        if (module_size > 0){
            if (str_tok != "E" && str_tok != "A" && str_tok != "R" && str_tok != "I"){
                // if (is_digits(str_tok)  && symbol_name.size() > 0 && (!is_digits(next_tok))){
                //     if (last_tok == symbol_name.at(symbol_name.size() - 1)){
                //         cout<< "last_tok "<< last_tok<<endl;
                //         cout<< str_tok<<endl;
                //         int number = stoi(str_tok) + stoi(memory);
                //         set_symbolvalue(last_tok, number);
                //     }
                if (is_digits(str_tok) && (!is_digits(next_tok) && str_tok != "E" && str_tok != "A" && str_tok != "R" && str_tok != "I")){
                    int number = stoi(str_tok) + stoi(memory);
                    if (!(is_digits(last_tok))){
                        set_symbolvalue(last_tok, number);
                        //flag_symb = find_symbol_index(last_tok);
                        flag_symb_val.push_back(last_tok);
                    }   
                    
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
                //cout << "alan: " << str_tok <<" next_tok: " << next_tok <<" rel: " << relative_offSet <<endl;
                update_memory();
                str = str_tmp;
                if (flag_module_start == 1){
                    module = module_size;
                    flag_module_start = -1;
                    count_module++;
                    if(flag_symb_val.size() > 0){
                        warning_5(flag_symb_val, count_module, module, khar, gav);
                        flag_symb_val.clear();
                    }

                }
                module_size -= 1;
            }
            std::string s;
            for (const auto &piece : symbol_name) s += piece;
        }
        else{
            //cout << "module_size" << module_size << endl;
            relative_offSet = stoi(memory);
            if (is_digits(str_tok)){// && str_tok != "0"){ //////////////////////////////////////////(input 5 isn't correct with this condition. not sure why it's been here)
                if (is_digits(next_tok)){
                    int tmp = stoi(str_tok) + stoi(memory);
                    khar = stoi(memory);
                    gav = stoi(str_tok);
                    flag_symb_val.push_back(last_tok);
                    //symbol_value.push_back(to_string(tmp));
                    
                    set_symbolvalue(last_tok, tmp);
                    // cout<< "symb name: "<< last_tok <<  " symb val " << tmp << endl;
                    // cout<< "str_tok: "<< str_tok << endl;
                    // cout<< "next_tok: "<< next_tok << endl;
                    module_size = -100;
                    //flag_module_start = -1;
                }
                else{
                    module_size = stoi(str_tok);
                    flag_module_start = 1;
                    //module_size_holder = module_size;
                    
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
    int module_size = -100;
    int flag = 0;
    string possible_uselist = "";
    int module_size_holder = 0;
    int count = 0;
    int flag_module = 0;
    int flag_E = 0;
    vector<tuple <int, string> > check_uselist;
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
        //cout<< "next tok "<< next_tok<< endl;
///////////////////////////////////////////////////
        if (module_size == -100){
            // if (!(isdigit(tok))){
            //     ; // Do nothing
            // }
            module_size = stoi(str_tok);
            module_size_holder = module_size;

        }
        if (module_size > 0){
            if (str_tok != "E" && str_tok != "A" && str_tok != "R" && str_tok != "I"){
                
                if (is_digits(str_tok)){
                    ;
                }

                else {
                    
                    if (!(is_digits(next_tok))){
                        
                        use_list.push_back(str_tok);
                        flag_uselist.push_back(0);
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
                if (module_size == module_size_holder){
                    flag_module = 1;
                    count++;
                }
                module_size -= 1;
                //cout << "alan: " << str_tok <<" next_tok: " << next_tok <<" rel: " << relative_offSet <<endl;

                if (str_tok == "E"){
                    E_Instruction(stoi(next_tok));
                    flag_E = 1;
                }
                else if(str_tok == "R"){
                    //cout<< "module_size_holder" << module_size_holder<< endl;
                    R_Instruction(stoi(next_tok), module_size_holder);
                }
                else if(str_tok == "I"){
                    I_Instruction(stoi(next_tok));
                }
                else{
                    A_Instruction(stoi(next_tok));
                }
                str = str_tmp;
               
            }
            if (module_size == 0 && str.length() == 2){
                cout<< "khar1.0";
                if (contains(flag_uselist, 0)){
                    cout<< "khar1";
                    for (int i = 0; i < flag_uselist.size(); i++){
                        if(flag_uselist.at(i) == 0){
                            int index = find_symbol_index(use_list.at(i));
                            //string symbol_not_used = symbol_name.at(index);
                            tuple<int,string> foo (count,symbol_name.at(index));
                            check_uselist.push_back(foo);
                        }
                    }
                }
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
                    //int tmp = stoi(str_tok) + relative_offSet;
                    // cout<< "------------------------------ \n"<<endl;
                    // cout<<"flag_uselist_size: "<<flag_uselist.size()<<endl;
                    // cout<<"uselist_size: "<<use_list.size()<<endl;
                    // for (int i = 0; i < flag_uselist.size(); i++){
                        
                    //     cout<<"flag_uselist_"<< i<< ": "<<flag_uselist.at(i)<<endl;
                    //     cout<<"uselist_"<< i<< ": "<<use_list.at(i)<<endl;
                    // }
                    // cout<< "------------------------------- \n"<<endl;


                    if (possible_uselist != ""){
                        possible_uselist = "";
                    }
                    //symbol_value.push_back(to_string(tmp));
                    //------------------------------------------------------------- commented the next line to see if it's necessary in pass2
                    //set_symbolvalue(last_tok, tmp);
                    //module_size = -1;
                }
                else{
                    // cout<< "khar2.0";
                    module_size = stoi(str_tok);
                    module_size_holder = module_size;
                    // cout<<"flag_uselist_size: "<<flag_uselist.size()<<endl;
                    // cout<<"uselist_size: "<<use_list.size()<<endl;
                    // for (int i = 0; i < flag_uselist.size(); i++){
                    //     cout<<"flag_uselist_"<< i<< ": "<<flag_uselist.at(i)<<endl;
                    //     cout<<"uselist_"<< i<< ": "<<use_list.at(i)<<endl;
                    // }
                    //+++old
                    // if (contains(flag_uselist, 0) && flag_module == 1){
                    //     cout<< "khar2";
                    //     for (int i = 0; i < flag_uselist.size(); i++){
                    //         if(flag_uselist.at(i) == 0){
                    //             int index = find_symbol_index(use_list.at(i));
                    //             //string symbol_not_used = symbol_name.at(index);
                    //             tuple<int,string> foo (count,symbol_name.at(index));
                    //             check_uselist.push_back(foo);
                    //         }
                    //     }
                    // }
                    if (possible_uselist != ""){
                        use_list.push_back(possible_uselist);
                        flag_uselist.push_back(0);
                        possible_uselist = "";
                    }
                    //^^^^^^^^old
                    // cout<< "+++++++++++++++++++++++++++++++ \n"<<endl;
                    // cout<<"flag_uselist_size: "<<flag_uselist.size()<<endl;
                    // cout<<"uselist_size: "<<use_list.size()<<endl;
                    // for (int i = 0; i < flag_uselist.size(); i++){
                        
                    //     cout<<"flag_uselist_"<< i<< ": "<<flag_uselist.at(i)<<endl;
                    //     cout<<"uselist_"<< i<< ": "<<use_list.at(i)<<endl;
                    // }
                    // cout<< "+++++++++++++++++++++++++++++++ \n"<<endl;
                    //cout<<"flag_module: "<<flag_module<<endl;
                    if (contains(flag_uselist, 0) && flag_E == 1){
                        
                        for (int i = 0; i < flag_uselist.size(); i++){
                            if(flag_uselist.at(i) == 0){
                                int index = find_symbol_index(use_list.at(i));
                                //string symbol_not_used = symbol_name.at(index);
                                tuple<int,string> foo (count,symbol_name.at(index));
                                check_uselist.push_back(foo);
                            }
                        }
                    }
                    flag_E = 0;
                    if (flag == 1){
                        //cout<< "use_list"<< use_list.at(0)<< endl;
                        use_list.clear();
                        flag_uselist.clear();
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
    if (contains(flag_uselist, 0) && flag_E == 1){
    
        for (int i = 0; i < flag_uselist.size(); i++){
            if(flag_uselist.at(i) == 0){
                int index = find_symbol_index(use_list.at(i));
                //string symbol_not_used = symbol_name.at(index);
                tuple<int,string> foo (count,symbol_name.at(index));
                check_uselist.push_back(foo);
            }
        }
    }
    // cout<< "count"<< count<< endl;
    // cout << get<0>(check_uselist.at(0))<<endl;
    // for (int i = 0; i < check_uselist.size() - 1; i++){
    //     cout<<"Warning: Module "<< get<0>(check_uselist.at(i))<< ": "<< get<1>(check_uselist.at(i)) <<"appeared in the uselist but was not actually used"<<endl;
    // }
    
     for(const auto &i : check_uselist){
         cout<<"Warning: Module "<< get<0>(i)<< ": "<< get<1>(i) <<" appeared in the uselist but was not actually used"<<endl;
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
