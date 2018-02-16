//A program that can use the 
//Created by: Ethan Reese
//February 8, 2017
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cctype>
#include <map>

using namespace std;

struct Stack{
    char ch;
    Stack *next;
};

struct TreeNode{
        char ch;
        TreeNode *next1;
        TreeNode *next2;
};

void pushOp(char ch, Stack* &top);
char* popOperator(Stack* &top);
char* reverse(char* string);

int main(){
    Stack* top = NULL;
    //Define the precedence for the various operators.
    map<char, int> precedence;
    precedence['+'] = 2;
    precedence['-'] = 2;
    precedence['*'] = 3;
    precedence['/'] = 3;
    precedence['^'] = 4;
    //Prompt the user to enter their expression
    cout << "Please enter your expression: ";
    char* input = new char[80];
    cin >> input;
    char* outputQuene = new char[80];
    //Keep going while there are still characters in the input buffer
    while(*(input) != NULL){
        //If the input is a number
        if(isdigit(*(input))){
            //Add the digit to the end of the output quene
            char append[2];
            append[0] = *(input);
            append[1] = '\0';
            strcat(outputQuene, append);
            //strcat(outputQuene, ' ');
        }
            //If its a left bracket just drop it onto the stack
        else if('(' == *(input)){
            pushOp(*(input), top);
        }
        else if(*(input) == ')'){
            //Pop operators off of the stack until it finds the corresponding left parenthesis
            while(top != NULL && top -> ch != '('){
                strcat(outputQuene, popOperator(top));
            }
            if(top -> ch == '('){
                //Take the left bracket off and throw it away
                popOperator(top);
            }
            else{
                cout << "The brackets don't match up";
                return 5;
            }
        }
            //Anything else will have to be an operator
        else{
          //Add a space to the output quene to clarify double digit numbers
          char add[2];
          add[0] = ' ';
          add[1] = '\0';
          strcat(outputQuene, add);
            //Assuming there's actually something on the stack and the input is an operator
            if(precedence.find(*(input)) != precedence.end()){
                if(top != NULL) {
                    //If the precedence on the stack is greater or the precednece is equal and its a left operator
                    while ((precedence[*(input)] < precedence[top->ch]) ||
                           ((precedence[*(input)] == precedence[top->ch]) && (*(input) != '^'))) {
                        //Make sure the top of the stack isn't a left bracket, ik it should go in the while but it was getting unmanageable
                        if (top->ch == '(') {
                            break;
                        }
                        //Take the operator off of the stack and put it in the output quene
                        char append[2];
                        append[0] = *popOperator(top);
                        append[1] = '\0';
                        strcat(outputQuene, append);
                    }
                }
                //Put the read in operator at the top of the stack.
                pushOp(*(input), top);
            }
        }
        //cout << outputQuene;
        input++;
    }
    //Once there are no more tokens on the input buffer take everything off of the stack and put it on the output quene
    while(top != NULL){ 
           char append[2];
           append[0] = *popOperator(top);
           append[1] = '\0';
           strcat(outputQuene, append);
    }
    cout << outputQuene << endl;
     //Take the postfix notation and turn it into a binary expression tree
     char* postfix = reverse(outputQuene);


    delete[] outputQuene;
    return 0;
}

//Function to add an operator to the stack
void pushOp(char ch, Stack* &top){
    Stack* newnode = new struct Stack();
    newnode -> ch = ch;
    newnode -> next = top;
    top = newnode;
}

//Pop an operator off of the top of the stack
char* popOperator(Stack* &top){
    Stack* node = top;
    char* op = &(top -> ch);
    top = top->next;
    delete node;
    return op;
}

char* reverse(char* string){
     //Create an array to save the reverse order
     char* array = new char[80];
     char firstChar = *(string);
     //Go until the string ends to find the last character
     while(*(string) != NULL){
          if(*(string) =='\0'){
               //Once it reaches the end of the string then it needs to reverse through to the beginning
               while(*(string) != firstChar){
                    strcat(array, *(string));
                    string--;
               }
               //Append the first character back onto the string.
               char append[2];
               append[0] = firstChar;
               append[1] = '\0';
               strcat(array, append);
               break;
          }
          string++;
     }
     return array;
}
