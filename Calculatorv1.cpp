//This is Advanced Calculator [v 2.0]
//It will use parsing and grammar.

#include <exception>
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <cmath>
using namespace std;

void error(string s){
  throw runtime_error(s);
}

class Token{
  public:
    double value;
    char kind;

    Token(char ch, double val): kind(ch),value(val){}

    Token(char ch): kind(ch){}

    
};


class Token_Stream{
  private:
    Token buffer;
    bool IsFull;
  public:
    Token get();
    void put_back(Token t);
    Token_Stream():IsFull(false),buffer(0){}
};


void Token_Stream::put_back(Token t){
  if(IsFull==true){
    cout<<"error putback";
    return;
  }
  buffer = t;
  IsFull = true;
}


Token Token_Stream::get(){
  if(IsFull==true){
    IsFull = false;
    return buffer;
  }
  char ch = ' ';
  do{
    cin.get(ch);
  }while(ch == ' ');
  switch(ch){

    case '\n':
    case 'q':
    case '!':
    case '*':
    case '{':
    case '}':
    case '%':
    case '/': 
    case '+':
    case '-':
    case '(':
    case ')':
      return Token (ch);
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      cin.putback(ch);
      double val;
      cin>>val;
      return Token('0',val);
    default:
      error(":Invalid Token:");
  }
  return 0;
}


/*
 The Grammar algorithm for the following code is :-
      (1)Primary = can be number
      (2)Term = multiple or division of primaries , can be a number
      (3)Expression = addition or subtraction of terms
 */

double term();
double primary();

Token_Stream ts;


double expression(){
  double left = term();
  Token t = ts.get();

  while (true){
    switch(t.kind){
      case '+':
        left+=term();
        t = ts.get();
        break;
      case '-':
        left-=term();
        t = ts.get();
        break;
      default:
        ts.put_back(t);
        return left;
    }
  }
  
}

double term(){
  double left = primary();
  Token t  = ts.get();

  if (t.kind=='0'){          //1!3 (invalid)
    error("Invalid Syntax");
  }

  while(true){
    switch(t.kind){
      case '*':{
        left*=primary();
        t = ts.get();
        break;
      }

      case '/':{
        double divisor = primary();
        if (divisor == 0){
          error("Divison By 0 error");
        }
        left/=divisor;
        t = ts.get();
        break;
      }
      case '%':{
        int a = int(left);
        left = a%int(primary());
        t = ts.get();
        break;
      }

      default:{
        ts.put_back(t);
        return left;
      }
    }

  }

}

double primary() {
    Token t = ts.get();
    switch(t.kind) {
        case '0': {  // Number token
            double val = t.value;
            // Check for factorial operator
            Token next = ts.get();
            if(next.kind == '!') {
                // Validate and compute factorial
                if(val < 0 || val != floor(val)) {
                    error("Factorial requires non-negative integers");
                }
                double result = 1;
                for(int i = 2; i <= val; ++i) {
                    result *= i;
                }
                return result;
            }
            else {
                ts.put_back(next);
                return val;
            }
        }
        case '(': 
        case '{': {
            char closing = (t.kind == '(') ? ')' : '}';
            double d = expression();
            t = ts.get();
            if(t.kind != closing) {
                error(string("Expected '") + closing + "' not found");//concatenation over a string.
            }
            return d;
        }
        case '-':{
          double a;
          try{
            a = primary();
          }
          catch(exception){
            error("Wrong Syntax");
          }
          return -a;
        }
        case '+':{
          double b;
          try{
            b = primary();
          }
          catch(exception){
            error("Wrong Syntax");
          }
          return b;
        }
        case 'q':
            exit(0);
        default:
            error("Primary expected (number or parenthesized expression)");
    }
    return 0;
}

void Calculate(){
  double val = 0;
  bool prompt = true;

  while(cin){
    if(prompt == true){
      cout<<"> ";
    }
    Token t = ts.get();
    if (t.kind == 'q') break;        // Press q to quit
    if (t.kind=='\n'){              // Press ENTER to end expression
      cout << "= " << val <<endl;   
      prompt=true;
      continue;
    }else {
      ts.put_back(t);
    }
    val = expression();
    prompt = false;
  }
}

int main(){
  cout<<R"(
                                         _.oo.
                 _.u[[/;:,.         .odMMMMMM'
              .o888UU[[[/;:-.  .o@P^    MMM^
             oN88888UU[[[/;::-.        dP^          "WELCOME TO THE ADVANCED CALCULATOR"!!-------{No Erroor Handling yet}
            dNMMNN888UU[[[/;:--.   .o@P^           ------------------------------[v 1.0.1]------------------------------------
           ,MMMMMMN888UU[[/;::-. o@^                 
           NNMMMNN888UU[[[/~.o@P^                      HERE'S WHAT YOU CAN DO :---
           888888888UU[[[/o@^-..
          oI8888UU[[[/o@P^:--..                         (1) (+,-,/,*,!,(),{}) These operations are used.
       .@^  YUU[[[/o@^;::---..                          (2) press 'q' to quit.
     oMP     ^/o@P^;:::---..                            (3) after writing an expression press ENTER to get answer.
  .dMMM    .o@^ ^;::---...                              (4) Only normal parenthisis are supported ().
 dMMMMMMM@^`       `^^^^                               
YMMMUP^   
 ^^

  )"<<endl;

  try{
    Calculate();
  }
  catch(exception& e){
    cerr<<e.what()<<'\n';
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max() , '\n');
    Calculate();
  }

}
