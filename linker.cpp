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

const int Max_Memory = 512;
string filename = "0";
int relative_offSet;
int unused_index = -1;
string memory; 
vector<string> use_list;
vector<string> symbol_name;
vector<int> symbol_name_duplicated_flag;
vector<string> symbol_value;
vector<string> symbol_value_absolute;
vector<string> symbol_module_number;
vector<string> unused_symbol_name;
vector<string> unused_symbol_module_number;
vector<int> flag_uselist;

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
		if (ruleBroken==2) {
			cout<< "Error: This variable is multiple times defined; first value used"<<endl;
		}
		if (ruleBroken==10) {
			cout<< "Error: Illegal immediate value; treated as 9999"<<endl;
		}
		if (ruleBroken==11) {
			cout<< "Error: Illegal opcode; treated as 9999"<<endl;
		}
}

void print_vector_string(vector<string> vec){
    for (int i = 0; i < vec.size(); i++){
        cout<<"vec_"<< i<< ": "<<vec.at(i)<<endl;
    }
}

void print_vector_int(vector<int> vec){
    for (int i = 0; i < vec.size(); i++){
        cout<<"vec_"<< i<< ": "<<to_string(vec.at(i))<<endl;
    }
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
int find_symbol_index(vector<string> vec_var ,string symb){

    std::vector<string>::iterator itr = find(vec_var.begin(), vec_var.end(), symb);
    return distance(vec_var.begin(), itr);
}
void print_symbol_table(){
    cout<< "Symbol Table"<< endl;
    for (int i = 0; i < symbol_name.size(); i++){
        if(symbol_name_duplicated_flag.at(i) == 1) {
            cout<< symbol_name.at(i) << "=" << symbol_value.at(i) << " ";
            error_message(2);
        } else if (symbol_name_duplicated_flag.at(i) == 0) {
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
        error_message(11);
    }
    else if (mod < address ){
        int opcode = operand / 1000;
        int num = (opcode * 1000) + relative_offSet; 
        cout<< memory<< ": " << num << " ";
        error_message(9);
    }
    else{
        int num = operand + relative_offSet;
        if (num < 10){
            string s_num = to_string(num);
            string add_zero = "000";
            s_num.insert(0, add_zero);
            cout<< memory<< ": " << s_num<< endl;
        }
        else if (operand < 100){
            string s_num = to_string(num);
            string add_zero = "00";
            s_num.insert(0, add_zero);
            cout<< memory<< ": " << s_num<< endl;
        }
        else if (operand < 1000){
            string s_num = to_string(num);
            string add_zero = "0";
            s_num.insert(0, add_zero);
            cout<< memory<< ": " << s_num<< endl;
        }
        else{
             cout<< memory<< ": " << num << endl;
        }
    }
    update_memory();
}
void I_Instruction(int operand){
    if (operand > 9999){
        operand = 9999;
        cout<< memory<< ": " << operand<< " ";       
        error_message(10);
    }
    else{
        if (operand < 10){
            string s_operand = to_string(operand);
            string add_zero = "000";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else if (operand < 100){
            string s_operand = to_string(operand);
            string add_zero = "00";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else if (operand < 1000){
            string s_operand = to_string(operand);
            string add_zero = "0";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else{
             cout<< memory<< ": " << operand<< endl;
        }

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
        if (operand < 10){
            string s_operand = to_string(operand);
            string add_zero = "000";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else if (operand < 100){
            string s_operand = to_string(operand);
            string add_zero = "00";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else if (operand < 1000){
            string s_operand = to_string(operand);
            string add_zero = "0";
            s_operand.insert(0, add_zero);
            cout<< memory<< ": " << s_operand<< endl;
        }
        else{
             cout<< memory<< ": " << operand<< endl;
        }
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
            if(std::find(symbol_name.begin(), symbol_name.end(), use_list.at(index)) == symbol_name.end()) {
                int absolute_address = 0;
                int num = replace(operand, absolute_address);
                cout<<memory<<": " << num<< " Error: "<< use_list.at(index)<< " is not defined; zero used"<<endl;
            }
            else{
                index = find_symbol_index(symbol_name, use_list.at(index));
                int absolute_address = stoi(symbol_value.at(index));
                int num = replace(operand, absolute_address);
                cout<<memory<<": " << num<< endl;

                if(!unused_symbol_name.empty()){
                    if(std::find(unused_symbol_name.begin(), unused_symbol_name.end(), symbol_name.at(index)) != unused_symbol_name.end()) {
                        unused_index = find_symbol_index(unused_symbol_name, symbol_name.at(index));
                        if (unused_index != -1){
                            unused_symbol_name.erase(unused_symbol_name.begin() + unused_index);
                            unused_symbol_module_number.erase(unused_symbol_module_number.begin() + unused_index);
                        }
                    }
                }
            }
            
        }
    }
    update_memory();
}
void set_symbolvalue(string str, int address){
    vector<string>::iterator itr = find(symbol_name.begin(), symbol_name.end(), str);
    int index = distance(symbol_name.begin(), itr);
    if (itr != symbol_name.cend()) {
        if (index == symbol_value.size()-1){
            symbol_value.at(index) = to_string(address);
        }
        else {
            symbol_name.erase(symbol_name.begin() + index);
            symbol_value.erase(symbol_value.begin() + index);
            symbol_name.push_back(str);
            symbol_value.push_back(to_string(address));
        }
    }
        
    else {
        ;
    }
}



bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}



int line_num(string str){
    ifstream f;
    string buffer;
    int offset;
    int i = 1;
    
    f.open(filename);
    while(getline(f, buffer)){
        
        offset = buffer.find(str);

        if (offset != -1){
            return i;
        }
        else{
        
            i++;
            continue;
        }

    }
    return -1;
}
int getOffset(string str){
    ifstream f;
    string buffer;
    int offset;
    
    
    f.open(filename);
    while(getline(f, buffer)){
        
        offset = buffer.find(str);
        
        if (offset != -1){
            return offset + 1;
        }
        else{
           
            continue;
        }

    }
    return -1;
}
void print_parse_error(string str, string error_type){
    int offset = getOffset(str);
    int line = line_num(str);
    cout << "Parse Error line " << line << " offset " << offset << ": " << error_type << endl;
    exit(1);
}

void pass1(){
    string next_tok;
    string last_tok;
    string str_tok;
    
    vector<vector <string> > flag_symb_val;
    vector<string > str;
    vector<string > str_tmp;
    
    int part_num = 0;
    int part_size = 0;
  
    ifstream f;
    string buffer;
    f.open(filename);


    copy(istream_iterator<string>(f),
    istream_iterator<string>(),
    back_inserter(str));

    f.close();
    // Edge case:
    // if (str.length() == 2){
    //     str_tmp = str;
    //     dup = strdup(str.c_str());
    //     tok = strtok(dup, delim);
    //     std::string str_tok = std::string(tok);
    //     free(dup);
    //     if(!is_digits(str_tok)){
    //         print_parse_error(str_tok, "NUM_EXPECTED");
    //     }
    //     else{
    //         cout << "Parse Error line " << 1 << " offset " << 2 << ": " << "SYM_EXPECTED" << endl;
    //         exit(1);
    //     }
    // }
    while(str.size() > 0){
 
        str_tok = str.at(0);
        next_tok = str.at(1);

        if (part_size == 0){
            if(!is_digits(str_tok)){
                print_parse_error(str_tok, "NUM_EXPECTED");
            }
            else{
                part_size = stoi(str_tok);
                part_num++;
                str.erase(str.begin());
                if (part_size > 16){
                    if (part_num % 3 == 1){
                        print_parse_error(str_tok, "TOO_MANY_DEF_IN_MODULE");   
                    }
                    else if (part_num % 3 == 2){
                        print_parse_error(str_tok, "TOO_MANY_USE_IN_MODULE");
                    }
                    else{
                        if (part_size + stoi(memory) > Max_Memory){
                            print_parse_error(str_tok, "TOO_MANY_INSTR");
                        }
                    }
                }
                if (part_num % 3 == 0){
                    for(int i = 0; i < symbol_value.size(); i++){
                        if (stoi(symbol_module_number.at(i)) == part_num / 3){
                            if (stoi(symbol_value_absolute.at(i)) > part_size - 1){
                                if (symbol_name_duplicated_flag.at(i) != 2){
                                    cout<< "Warning: Module " << part_num / 3 << ": "<< symbol_name.at(i)  << " too big " << symbol_value_absolute.at(i) << " (max=" << part_size - 1 << ") assume zero relative"<< endl;
                                    symbol_value.at(i) = to_string(stoi(symbol_value.at(i)) - stoi(symbol_value_absolute.at(i)));
                                }
                            }
                        }
                    }
                }
            }
        }
        else{
            if (part_num % 3 == 1){
                // we are in the def list
                if(is_digits(str_tok)){
                    print_parse_error(str_tok, "SYM_EXPECTED");
                }
                if (str_tok.length() > 17){
                    print_parse_error(str_tok, "SYM_TOO_LONG");
                }
                if(std::find(symbol_name.begin(), symbol_name.end(), str_tok) != symbol_name.end()) {
                    symbol_name_duplicated_flag.push_back(2);
                    int index = find_symbol_index(symbol_name, str_tok);
                    symbol_name_duplicated_flag.at(index) = 1;
                }
                else{
                    symbol_name_duplicated_flag.push_back(0);
                }
                symbol_name.push_back(str_tok);
                
                if(!is_digits(next_tok)){
                    print_parse_error(next_tok, "NUM_EXPECTED");
                }
                symbol_value.push_back(to_string(stoi(next_tok) + stoi(memory)));
                symbol_value_absolute.push_back(to_string(stoi(next_tok)));
                symbol_module_number.push_back(to_string(part_num / 3 + 1));

                
                str.erase(str.begin());
                str.erase(str.begin());
                part_size--;
            }
            else if (part_num % 3 == 2){
                // we are in the use list
                if(is_digits(str_tok)){
                    print_parse_error(str_tok, "SYM_EXPECTED");
                }
                if (str_tok.length() > 17){
                    print_parse_error(str_tok, "SYM_TOO_LONG");
                }
                str.erase(str.begin());
                part_size--;
            }
            else {
                // we are in the prog list
                if(str_tok == "E" || str_tok == "A" || str_tok == "R" || str_tok == "I"){
                    update_memory();
                }
                else{
                    print_parse_error(str_tok, "ADDR_EXPECTED");
                }
                
                if(!is_digits(next_tok)){
                    print_parse_error(next_tok, "NUM_EXPECTED");
                }
                str.erase(str.begin());
                str.erase(str.begin());
                part_size--;
            }
        }

    }
    unused_symbol_name = symbol_name;
    unused_symbol_module_number = symbol_module_number;

    print_symbol_table();
}

void pass2(){
    string str_tok;
    string next_tok; 
    string last_tok; 
    
    int module_size_holder = 0;
    
    vector<string> str;
    int part_num = 0;
    int part_size = 0;
  
    ifstream f;
    string buffer;
    //string str;
    f.open(filename);
    copy(istream_iterator<string>(f),
    istream_iterator<string>(),
    back_inserter(str));
   
    f.close();

    while(str.size() > 0){
         
        str_tok = str.at(0);
        next_tok = str.at(1);

        if (part_size == 0){
            part_size = stoi(str_tok);
            relative_offSet = stoi(memory);
            module_size_holder = part_size;
            part_num++;
            str.erase(str.begin());
            
        }       
        else{
            if (part_num % 3 == 1){
                // we are in the def list
                // str = str_tmp;
                str.erase(str.begin());
                str.erase(str.begin());
                part_size--;
            }
            else if (part_num % 3 == 2){
                // we are in the use list
                use_list.push_back(str_tok);
                flag_uselist.push_back(0);
                str.erase(str.begin());
                part_size--;
            }
            else {
                // we are in the prog list
                if(str_tok == "E" || str_tok == "A" || str_tok == "R" || str_tok == "I"){
                    if (str_tok == "E"){
                        E_Instruction(stoi(next_tok));
                        
                    }
                    else if(str_tok == "R"){
                        R_Instruction(stoi(next_tok), module_size_holder);
                    }
                    else if(str_tok == "I"){
                        I_Instruction(stoi(next_tok));
                    }
                    else{
                        A_Instruction(stoi(next_tok));
                    }
                }
                
                str.erase(str.begin());
                str.erase(str.begin());
                part_size--;
                if (part_size == 0){
                    
                    if (!flag_uselist.empty()){
                        for (int i = 0; i < flag_uselist.size(); i++){
                            if(flag_uselist.at(i) == 0){
                               
                                cout<< '\n';
                                cout<<"Warning: Module "<< part_num / 3 << ": "<< use_list.at(i) <<" appeared in the uselist but was not actually used"<<endl;
                                cout<< '\n';
                            }
                        }
                    }
                    use_list.clear();
                    flag_uselist.clear();
                


                }
            }
        }
        
    }

    if (unused_symbol_name.size()>0){
        for (int i = 0; i < unused_symbol_name.size(); i++){
            cout<<"Warning: Module "<< unused_symbol_module_number.at(i) << ": "<<unused_symbol_name.at(i)<< " was defined but never used"<<endl;
        }
    }
}

int main(int argc, char** argv){
    filename = argv[1];

    memory = "000";
    relative_offSet = 0;
    
    pass1();
    memory = "000";
    relative_offSet = 0;
    cout<<"Memory Map"<< endl;
    pass2();
    return 0;
}
