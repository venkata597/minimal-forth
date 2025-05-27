#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <variant>

typedef enum STACK_ERROR_CODE{
STACK_EMPTY,
STACK_OVERFLOW, // Just writing it, this stack is dynamic so we won't need an overflow error code
};

typedef enum INSTRUCTION{
// IO
DOT,
NEW_LINE,
// LITERALS
INT,
// STACK MANIPULATION
DUP,
DROP,
SWAP,
ROT,
NIP,
// ARITHMETIC
ADD,
SUBTRACT,
MULTIPLY,
DIVIDE,
MODULO,
ABS,
NEGATE,
MIN,
MAX,
// COMPARISION OR LOGIC
EQUALS,
LESS_THAN,
GREATER_THAN,

// UNRECOGNIZED COMMAND
UNKNOWN,
};


class Node{
public:
    int data;
    Node* next;
};

// STACK
class Stack{
private:
    Node* tos = nullptr;
    Node* temp = nullptr; //for stack traversals
public:
    void push(int ele){
        Node* newn = new Node;
        newn->data = ele;
        newn->next = nullptr;
        if(tos==nullptr){
            tos = newn;
        }
        else{
            newn->next = tos;
            tos = newn;
        }
    }

    int pop(){
       if(tos==nullptr){
        std::cout << "Stack is empty" << std::endl;
        return STACK_EMPTY;
       }
       else{
        temp = tos;
        tos = tos->next;
        temp->next = nullptr;
        int p = temp->data;
        delete temp;
        return p;
       }
    }

    void peek(){
        std::cout << tos->data;
    }

    void display(){
        temp = tos;
        while(temp!=nullptr){
            std::cout << temp->data << "-> ";
            temp = temp->next;
        }
        std::cout << "END OF STACK" << std::endl;
    }

    int getntop(int n){
        if(tos==nullptr){
            std::cout << "STACK EMPTY" << std::endl;
            return -1;
        }
        else{
            temp = tos;
            for(int i = 0;i<n-1;i++){
                temp = temp->next;
            }
            int nth = temp->data;
            return nth;
        }
    }

    void remove(int n){
        if(tos==nullptr){
            return;
        }
        else{
            temp = tos;
            Node* temp2;
            while(temp->next->data!=n&&temp->next!=NULL){
                temp = temp->next;
            }
            temp2 = temp->next;
            temp->next = temp2->next;
            temp2->next = nullptr;
            delete temp2;
        }
        
    }
};


class parsed_token{
public:
    std::vector<INSTRUCTION> insts;
    std::vector<int> integer_data;
    int intvect_ptr = 0;
};

Stack stack;

bool isnum(std::string n){
    bool flag = true;
    for(int i = 0;i<n.length();i++){
        if(isdigit(n[i])){
            continue;
        }
        else{
            flag = false;
        }
    }
    return flag;
}


int make_num(std::string n){
    std::string genval;
    for(int i = 0;i<n.length();i++){
        if(isdigit(n[i])){
            genval.push_back(n[i]);
        }
    }
    return std::stoi(genval);
}

std::vector<std::string> read_all_lines(std::ifstream& file){
    std::vector<std::string> lines;
    std::string buff;
    while(getline(file,buff)){
        lines.push_back(buff);
    }
    return lines;
}

std::vector<std::string> split_whitespace(std::string line){
    std::vector<std::string> tokens;
    line.push_back(' ');
    std::string buff;
    int i = 0;
    while(line[i]!='\0'){
        if(line[i]==' '){
            if(!buff.empty()) tokens.push_back(buff);
            buff.clear();
        }
        else{
            buff.push_back(line[i]);
        }
        i++;
    }
    return tokens;
}

std::vector<parsed_token> parse(std::vector<std::vector<std::string>> lex_tokens){
    std::vector<parsed_token> parsedtoks;
    int s = lex_tokens.size();
    for(int i = 0;i<s;i++){
        int size = lex_tokens[i].size();
        parsed_token pars;
        for(int j = 0;j<size;j++){
            std::string token = lex_tokens[i][j];
            if(token[0]=='('){
                if(token[token.length()-1]==')'){
                    continue;
                }
                else{
                    std::cout << "Comment intialized incorrectly" << std::endl;
                }
            }
            else{
                if(isnum(token)==true){pars.insts.push_back(INT);int n = make_num(token);pars.integer_data.push_back(n);}
                else if(token=="DROP") pars.insts.push_back(DROP);
                else if(token=="SWAP") pars.insts.push_back(SWAP);
                else if(token=="ROT") pars.insts.push_back(ROT);
                else if(token=="NIP") pars.insts.push_back(NIP);
                else if(token=="DUP")pars.insts.push_back(DUP);
                else if(token=="CR") pars.insts.push_back(NEW_LINE);
                else if(token==".") pars.insts.push_back(DOT);
                else if(token=="+") pars.insts.push_back(ADD);
                else if(token=="-") pars.insts.push_back(SUBTRACT);
                else if(token=="*") pars.insts.push_back(MULTIPLY);
                else if(token=="/") pars.insts.push_back(DIVIDE);
                else if(token=="MOD") pars.insts.push_back(MODULO);
                else if(token=="ABS") pars.insts.push_back(ABS);
                else if(token=="NEG") pars.insts.push_back(NEGATE);
                else if(token=="MIN") pars.insts.push_back(MIN);
                else if(token=="MAX") pars.insts.push_back(MAX);
                else if(token=="=") pars.insts.push_back(EQUALS);
                else if(token==">") pars.insts.push_back(GREATER_THAN);
                else if(token=="<") pars.insts.push_back(LESS_THAN);
                else pars.insts.push_back(UNKNOWN);
            }
        }
        parsedtoks.push_back(pars);
    }
    return parsedtoks;
}

void dup(){
    // ( a → a a )
    int a = stack.pop();
    stack.push(a);
    stack.push(a);
}

void drop(){
    // ( a → )
    stack.pop();
}

void swap(){
    // ( a b → b a )
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(b);
    stack.push(a);
}

void rot(){
    // ( a b c → b c a )
    int num = stack.getntop(3);
    stack.remove(num);
    stack.push(num);
}

void nip(){
    // ( a b → b )
    stack.remove(stack.getntop(2));
}

void dot(){
    stack.peek();
}

void new_line(){std::cout << std::endl;}

void add(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(a+b);
}

void subtract(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(b-a);
}

void multiply(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(a*b);
}

void divide(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(b/a);
}

void mod(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    stack.push(b%a);
}

void negate(){
    int a,b;
    a = stack.pop();
    b = -a;
    stack.push(a);
    stack.push(b);
}

void min(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    if(a>b){
        stack.push(b);
    }
    else{
        stack.push(a);
    }
}

void max(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    if(a>b){
        stack.push(a);
    }
    else{
        stack.push(b);
    }
}

void equals(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    if(a==b){
        stack.push(-1);
    }
    else{
        stack.push(0);
    }
}

void lessthan(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    if(a<b){
        stack.push(-1);
    }
    else{
        stack.push(0);
    }
}

void greaterthan(){
    int a,b;
    a = stack.pop();
    b = stack.pop();
    if(a>b){
        stack.push(-1);
    }
    else{
        stack.push(0);
    }
}

void execute(std::vector<parsed_token> parsed){
    for(int i = 0;i<parsed.size();i++){
        parsed_token line = parsed[i];
        for(int j = 0;j<line.insts.size();j++){
            switch(line.insts[j]){
                case INT: stack.push(line.integer_data[(line.intvect_ptr)++]); break;
                case DUP: dup(); break;
                case DROP: drop(); break;
                case SWAP: swap(); break;
                case ROT: rot(); break;
                case NIP: nip(); break;
                case DOT: dot(); break;
                case NEW_LINE: new_line(); break;
                case ADD: add(); break;
                case SUBTRACT: subtract(); break;
                case MULTIPLY: multiply(); break;
                case DIVIDE: divide(); break;
                case MODULO: mod(); break;
                case NEGATE: negate(); break;
                case MIN: min(); break;
                case MAX: max(); break;
                case EQUALS: equals(); break;
                case GREATER_THAN: greaterthan(); break;
                case LESS_THAN: lessthan(); break;
                case UNKNOWN: std::cout << "Unknown Command encountered at line: " << i+1 << std::endl;exit(0);break;
            }
        }
    }
}

void read_check(std::vector<parsed_token> parsed){
    for(int i = 0;i<parsed.size();i++){
        parsed_token p = parsed[i];
        for(int j = 0;j<p.insts.size();j++){
            switch(p.insts[j]){
                case DUP: std::cout << "DUP "; break;
                case DROP: std::cout << "DROP "; break;
                case SWAP: std::cout << "SWAP "; break;
                case ROT: std::cout << "ROT "; break;
                case NIP: std::cout << "NIP "; break;
                case DOT: std::cout << "DOT"; break;
                case INT: std::cout << "INT "; break;
                case ADD: std::cout << "ADD "; break;
                case SUBTRACT: std::cout << "SUBTRACT "; break;
                case MULTIPLY: std::cout << "MULTIPLY "; break;
                case DIVIDE: std::cout << "DIVIDE "; break;
                case MODULO: std::cout << "MODULO "; break;
                case ABS: std::cout << "ABS "; break;
                case NEGATE: std::cout << "NEG "; break;
                case MIN: std::cout << "MIN "; break;
                case MAX: std::cout << "MAX "; break;
                case UNKNOWN: std::cout << "UNKNOWN "; break;
            }
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    if(argc<2){
        std::cout << "Error: Correct command is <filename.fth>" << std::endl;
    }
    std::ifstream prog_file(argv[1]);
    std::vector<std::string> file_content = read_all_lines(prog_file);
    std::vector<std::vector<std::string>> lexed;
    for(int i =0;i<file_content.size();i++){
        std::vector<std::string> line_toks = split_whitespace(file_content[i]);
        lexed.push_back(line_toks);
    }
    std::vector<parsed_token> parsed = parse(lexed);
    execute(parsed);
}