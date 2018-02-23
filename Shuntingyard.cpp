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
        char* ch;
        TreeNode *next1;
        TreeNode *next2;
        TreeNode *parent;
        bool visited;
};

void pushOp(char ch, Stack* &top);
char* popOperator(Stack* &top);
char* reverse(char* string);
void printPrefix(TreeNode*, int);

int main(){
    Stack* top = NULL;
    TreeNode* root = NULL;
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
     TreeNode* current = NULL;
     cout << postfix << endl;
     int nodeCount = 0;
     while(*(postfix) != NULL){
          //I arbitrarily decided that 80 digits was the highest worth supporting but this could go higher
          char operate[80];
          //Deal with the whole two digit numbers thing
          int counter = 0;
          if(isdigit(*(postfix))){
               while(*(postfix) != NULL){
                    operate[counter] = *(postfix);
                    postfix++;
                    counter++;
                    //If it gets to a space then it means its the end of a number
                    if(*(postfix) == ' '){
                         operate[counter] = '\0';
                         counter = 0;
                         break;
                    }
                    else if(precedence.find(*(postfix)) != precedence.end()){
                         operate[counter] = '\0';
                         counter = 0;
                         postfix--;
                         break;
                     }
               
               }
          }
          //If its an operator than put it into the array to append
          else if(precedence.find(*(postfix)) != precedence.end()){
               operate[0] = *(postfix);
               operate[1] = '\0';
          }
          //For the very first character in the expression, it must become the root
          if(root == NULL){
               root = new struct TreeNode();
               root->ch = operate;
               root -> next1 = NULL;
               root -> next2 = NULL;
               root -> parent = NULL;
               current = root;
               ++nodeCount;     
          }
          //Otherwise if must move through the rest of the logic normally
          else{
               //Create a new node for the element in the list
               TreeNode* newnode = new struct TreeNode();
               ++nodeCount;
               newnode -> ch = &(*(postfix));
               newnode -> next1 = NULL;
               newnode -> next2 = NULL;
               newnode -> parent = NULL;
               //Check if the current node can have more children and which slot is available 
               if(current -> next1 != NULL){
                    current -> next1 = newnode;
                    newnode -> parent = current;
                    current = newnode;
               }
               else if(current->next2 != NULL){
                    current -> next2 = newnode;
                    newnode -> parent = current;
                    current = newnode;
               }
               else{
                    //It has to search up the list for the first parent that can have a child
                    while(current->parent != NULL){
                         //Check if the new current node can have children and which slot is open    
                              
                    if(current -> next1 != NULL){
                         current -> next1 = newnode;
                         newnode -> parent = current;
                         current = newnode;
                    }
                    else if(current->next2 != NULL){
                         current -> next2 = newnode;
                         newnode -> parent = current;
                         current = newnode;
                    }
                    //Set the current to its parent if the node has no potential children
                     else{
                         current = current->parent;
                       }
                    }
               }
          }
     }

     //Check what kind of notation the user would like to print out
     cout << "Enter what kind of notation you would like for output: Prefix(1) Postfix(2) Infix(3)" << endl;
     printPrefix(root, nodeCount);
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
     while(true){
          if(*(string) =='\0'){
               //Once it reaches the end of the string then it needs to reverse through to the beginning
               while(*(string) != firstChar){
                    char append[2];
                    append[0] = *(string);
                    append[1] = '\0';
                    strcat(array, append);
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

void printPrefix(TreeNode* root, int nodecount){
     char* output = new char[80];
     TreeNode* current = root;
     //While there are still nodes that have not been read
     while(nodecount != 0){
          //If the node has already been visited it has to go through the children
          if(current->visited == true){
               //Check if the first child has been visited
               if(current->next1->visited != true){
                    current = current->next1;
               }
               //Check if the second child has been visited
               else if(current->next2->visited != true){
                    current = current->next2;
               }
               //If both children have been visited then it has to go up in the tree
               else if(current->parent != NULL){
                    current = current->parent;
               }
               //Something has gone horribly wrong
               else{
                    cout << "ERROR";
                    return;
               }
          }
          //Otherwise the node hasn't been visited then it has to add the characters to the output quene
          else{
               strcat(output, current->ch);
               current->visited = true;
               nodecount--;
               //Check if the first child has been visited
               if(current->next1->visited != true){
                    current = current->next1;
               }
               //Check if the second child has been visited
               else if(current->next2->visited != true){
                    current = current->next2;
               }
               //If both children have been visited then it has to go up in the tree
               else if(current->parent != NULL){
                    current = current->parent;
               }
               //Something has gone horribly wrong
               else{
                    cout << "ERROR";
                    return;
               }
          }
     }
     cout << output;
}
