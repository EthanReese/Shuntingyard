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
        TreeNode *right;
        TreeNode *left;
        TreeNode *parent;
};

struct TreeStack{
     TreeNode* tn;
     TreeStack* next;
};

struct node{
        char* ch;
        node *next;
        node *previous;
};

void pushOp(char ch, Stack* &top);
void pushTree(TreeNode* tn, TreeStack* &top);
char* popOperator(Stack* &top);
TreeNode* popTree(TreeStack* &top);
void LinkedList(node* &head, char* string);
void printPrefixBetter(TreeNode* &current);
void printPostfix(TreeNode* &current);

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
    cin.getline(input, 80);
    char* infix = new char[80];
    infix = input;
    char* outputQuene = new char[80];
    //Keep going while there are still characters in the input buffer
    while(*(input) != '\0'){
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
        else if(' ' == *(input)){
               //Ignore the space because it gets incorporated later
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
     //Anything else will have to be a normal operator
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
     node* head = NULL;
     LinkedList(head, outputQuene);
     node* now = head;
     TreeStack* lead = NULL;
     //Loop through each node and turn it into a binary expansion tree
     while(now != NULL){
          //If its a digit push it onto the stack
          if(isdigit((now->ch)[0])){
               TreeNode* newNode = new struct TreeNode();
               newNode->ch = new char[80];
               strcpy(newNode->ch, now->ch);
               pushTree(newNode, lead);
          }
          else{
               TreeNode* newNode = new struct TreeNode();
               newNode->ch = new char[80];
               strcpy(newNode->ch, now->ch);
               newNode->left = popTree(lead);
               newNode->right = popTree(lead);
               pushTree(newNode, lead);
          }
          now = now->next;
     }
     //Check what kind of notation the user would like to print out
     cout << "Enter what kind of notation you would like for output: Prefix(1) Postfix(2) Infix(3)" << endl;
     char* input_2 = new char[2];
     cin >> input_2;
     if(strcmp(input_2, "1") == 0){
          printPrefixBetter(lead->tn);
     }
     else if(strcmp(input_2, "2") == 0){
          printPostfix(lead->tn);
     }
     else if(strcmp(input_2, "3") == 0){
          cout << infix <<  endl;
     }
     else{
             cout << "There was an error" << endl;
     }
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

//Function to add an operator to the stack
void pushTree(TreeNode* tn, TreeStack* &top){
    TreeStack* newnode = new struct TreeStack();
    newnode -> tn = tn;
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
//Pop an operator off of the top of the stack
TreeNode* popTree(TreeStack* &top){
    TreeStack* node = top;
    TreeNode* content = top->tn;
    top = top->next;
    delete node;
    return content;
}
//Convert an out
void LinkedList(node* &head, char* string){
     //Loop over the whole string
     node* current = NULL;
     while(*(string) != NULL){
          //Create the first node in the list
          if(*(string) == ' '){
               string++;
          }
          else if(head == NULL && *(string)!=' '){
               char* append = new char[80];
               head = new struct node;
               head->next = NULL;
               head->previous = NULL;
               int counter = 0;
               //If its a number it honeslty becomes kind of a pain
               if(isdigit(*(string))){
                       while(isdigit(*(string))){
                            append[counter] = *(string);
                            counter++;
                            string++;
                       }
                       append[counter] = '\0';
               }
               else{
                    append[0] = *(string);
                    append[1] = '\0';
                    string++;
               }
               head->ch = new char[80];
               strcpy(head->ch, append);
               delete[] append;
               current = head;
          }
          else{
               int counter = 0;
               node* newnode = new struct node();
               newnode->previous = current;
               current->next = newnode;
               char* append = new char[80];
               //If its a number it honeslty becomes kind of a pain
               if(isdigit(*(string))){
                       while(isdigit(*(string))){
                            append[counter] = *(string);
                            counter++;
                            string++;
                       }
                       append[counter] = '\0';
               }
               else{
                    append[0] = *(string);
                    append[1] = '\0';
                    string++;
               }
               newnode->ch = new char[80];
               strcpy(newnode->ch, append);
               delete[] append;
               newnode->next = NULL;
               current = newnode;
               //cout << newnode->ch;
               //cout << *(string);
          }
     }
}
void printPrefixBetter(TreeNode* &current){
      if(current->ch != NULL){       
          cout << current->ch << ' ';
             if(current->right != NULL){
                  printPrefixBetter(current->right);
             }
             if(current->left != NULL){
                  printPrefixBetter(current->left);
             }
     }
}
void printPostfix(TreeNode* &current){
     if(current->ch != NULL){
        if(current->right != NULL){
                  printPostfix(current->right);
             }
          if(current->left != NULL){
                  printPostfix(current->left);
             } 
          cout << current->ch << ' '; 
     }
}
