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
        bool visited;
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
void printPrefix(TreeNode* &root, int);
void LinkedList(node* &head, char* string);
void printPrefixBetter(TreeNode* &current);

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
    char* infix = new char[80];
    infix = input;
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
    cout << "Postfix: " << outputQuene << endl;
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
     /*while(now != NULL){
          //I arbitrarily decided that 80 digits was the highest worth supporting but this could go higher
          char* operate = new char[80];
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
                    //It also means its the end of the number if the next character is an operator
                    else if(precedence.find(*(postfix)) != precedence.end()){
                         operate[counter] = '\0';
                         counter = 0;
                         --postfix;
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
          if(nodeCount == 0){
               root = new struct TreeNode();
               root->ch = new char[80];
               strcpy(root->ch, now->ch);
               root -> next1 = NULL;
               root -> next2 = NULL;
               root -> parent = NULL;
               root -> visited = false;
               current = root;
               nodeCount++;
          }
          //Otherwise it must move through the rest of the logic normally
          else{
               //Create a new node for the element in the list
               TreeNode* newnode = new struct TreeNode();
               ++nodeCount;
               newnode->ch = new char[80];
               strcpy(newnode -> ch, now->ch);
               newnode -> next1 = NULL;
               newnode -> next2 = NULL;
               newnode -> parent = NULL;
               newnode -> visited = false;
               //Check if the current node can have more children and which slot is available 
               if(current -> next2 == NULL){
                    current -> next2 = newnode;
                    newnode -> parent = current;
                    current = newnode;
               }
               else if(current->next1 == NULL){
                    current -> next1 = newnode;
                    newnode -> parent = current;
                    current = newnode;
               }
               else{
                    //It has to search up the list for the first parent that can have a child
                    while(current->parent != NULL){
                         //Check if the new current node can have children and which slot is open     
                         if(current -> next2 == NULL){
                              current -> next2 = newnode;
                              newnode -> parent = current;
                              current = newnode;
                         }
                         else if(current->next1 == NULL){
                              current -> next1 = newnode;
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
          now = now->previous;
     }*/
     //Check what kind of notation the user would like to print out
     cout << "Enter what kind of notation you would like for output: Prefix(1) Postfix(2) Infix(3)" << endl;
     char* input_2 = new char[2];
     cin >> input_2;
     if(strcmp(input_2, "1") == 0){
          printPrefixBetter(lead->tn);
     }
     else if(strcmp(input_2, "2") == 0){
          cout << outputQuene;
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

/*void printPrefix(TreeNode* &root, int nodecount){
     TreeNode* current = root;
     //While there are still nodes that have not been read
     while(nodecount != 0){
          //If the node has already been visited it has to go through the children
          if(current->visited == true){
               //Check if the first child has been visited 
               if(current->next1 != NULL){
                    if(current->next1->visited != true){
                         current = current->next1;
                    }
               }
               //Check if the second child has been visited
               else if(current->next2 != NULL){
                   if(current->next2->visited != true){ 
                         current = current->next2;
                   }
               }
               //If both children have been visited then it has to go up in the tree
               else if(current->parent != NULL){
                    current = current->parent;
               }
          }
          //Otherwise the node hasn't been visited then it has to add the characters to the output quene
          else{
          //     cout << current->ch << ' ';
               current->visited = true;
               nodecount--;
               //Check if the first child has been visited
               if(current->next1 != NULL){
                    if(current->next1->visited != true){
                         current = current->next1;
                    }
               }
               //Check if the second child has been visited
               else if(current->next2 != NULL){
                   if(current->next2->visited != true){ 
                         current = current->next2;
                   }
               }
               //If both children have been visited then it has to go up in the tree
               else if(current->parent != NULL){
                    current = current->parent;
               }
          }
     }
}*/
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
