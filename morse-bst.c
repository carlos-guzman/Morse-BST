//TODO: Complete if found in toMorse


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MORSE_LEN 10
#define DEBUG 0

typedef struct tree_node{
	struct tree_node* left;
	struct tree_node* right;
	char data;
}tree_node;

typedef struct stack_item{
	tree_node* data;
	struct stack_item* below;
}stack_item;

tree_node *tree_insert(char);

tree_node* root;
stack_item* top = NULL;
// char input[20];

void tree_init(){
	root = tree_insert(0);
	root->left = tree_insert('T');
	root->right = tree_insert('E');
	root->left->left = tree_insert('M');
	root->left->right = tree_insert('N');
	root->left->left->left = tree_insert('O');
	root->left->left->right = tree_insert('G');
	root->left->left->left->left = tree_insert(0);
	root->left->left->left->right = tree_insert(0);
	root->left->left->left->left->left = tree_insert('0');
	root->left->left->left->left->right = tree_insert('9');
	root->left->left->left->right->right = tree_insert('8');
	root->left->left->right->left = tree_insert('Q');
	root->left->left->right->right = tree_insert('Z');
	root->left->left->right->right->right = tree_insert('7');
	root->left->right->left = tree_insert('K');
	root->left->right->right = tree_insert('D');
	root->left->right->left->left = tree_insert('Y');
	root->left->right->left->right = tree_insert('C');
	root->left->right->right->left = tree_insert('X');
	root->left->right->right->right = tree_insert('B');
	root->left->right->right->right->right = tree_insert('6');
	root->right->left = tree_insert('A');
	root->right->right = tree_insert('I');
	root->right->left->left = tree_insert('W');
	root->right->left->right = tree_insert('R');
	root->right->left->left->left = tree_insert('J');
	root->right->left->left->right = tree_insert('P');
	root->right->left->left->left->left = tree_insert('1');
	root->right->left->right->right = tree_insert('L');
	root->right->right->left = tree_insert('U');
	root->right->right->right = tree_insert('S');
	root->right->right->left->left = tree_insert(0);
	root->right->right->left->right = tree_insert('F');
	root->right->right->left->left->left = tree_insert('2');
	root->right->right->right->left = tree_insert('V');
	root->right->right->right->right = tree_insert('H');
	root->right->right->right->left->left = tree_insert('3');
	root->right->right->right->right->left = tree_insert('4');
	root->right->right->right->right->right = tree_insert('5');
}

tree_node* tree_insert(char data){
	tree_node* new_tree_node = malloc(sizeof(tree_node));
	new_tree_node->data = data;
	new_tree_node->left = NULL;
	new_tree_node->right = NULL;
	return new_tree_node;
}

void tree_print(tree_node* current, int level){
	level++;
	if(current==NULL)
		return;
	int i;
	for(i=0;i<level-1;i++)
		printf("\t");

	printf("%c\n", current->data);
	tree_print(current->left, level);
	tree_print(current->right, level);
}

void tree_clear_rec(tree_node* current){
	if(current==NULL)
		return;
	tree_clear_rec(current->left);
	tree_clear_rec(current->right);
	free(current);
}

void tree_clear(){
	if(DEBUG) printf("Clearing tree...");
	tree_clear_rec(root);
}

void stack_push(tree_node* t){
	stack_item* new_item = malloc(sizeof(stack_item));
	new_item->below = (top != NULL)? top : NULL;
	new_item->data = t;
	top = new_item;
}

tree_node* stack_pop(){
	if(top==NULL)
		return NULL;
	tree_node* t = top->data;
	stack_item* temp = top;
	top = top->below;
	free(temp);
	return t;
}

tree_node* stack_peek(){
	if(top==NULL)
		return NULL;
	return top->data;
}

void stack_reverse(){
	if(DEBUG) printf("reversing\n");
	if(top == NULL || top->below == NULL)
		return;
	stack_item* new_top = NULL;
	while(top!=NULL){
		stack_item* temp = malloc(sizeof(stack_item));
		temp->data = stack_pop();
		temp->below = new_top;
		new_top = temp;
	}
	top = new_top;
}

void stack_print(){
	printf("stack\n");
	stack_item* current = top;
	while(current != NULL){
		printf("%c\t%p\t%p\n", current->data->data, current, current->data);
		current = current->below;
	}
}

void stack_clear(){
	if(DEBUG) printf("Clearing stack...");
	stack_item *next;
	stack_item *to_free = top;
	while(to_free != NULL){
		next = to_free->below;
		free(to_free);
		to_free = next;
	}
	top = NULL;
}

char fromMorse(char* morse){
	char* current = morse;
	tree_node* node = root;
	while(*current != 0){
		// printf("%c  %c\n", tree_node->data, *current);
		if(*current == '-')
			node = node->left;
		else if(*current == '.')
			node = node->right;
		else if(*current!='\n'){
			printf("Invalid.\n");
			exit(1);
		}
		current++;
	}
	return node->data;
}

char* toMorse(char c){
	tree_node* current = root;
	tree_node* popped = NULL;
	char found = 0;
	while(!found){
		if(current != NULL){
			stack_push(current);
			current = current->left;
		}else if (top != NULL){
			current = stack_pop();
			if(current->data==c){
				found = 1;
			}
			current = current->right;
		}else
			break;
	}
	if(found){
		char* str = (char*)malloc(MAX_MORSE_LEN);
		*str = '\0';
		if(DEBUG) stack_print();
		stack_reverse();
		if(DEBUG) stack_print();
		current = root;
		while(current->data != c){
			if(DEBUG){
				printf("\n\n\n");
				printf("current\t%c\t%p\n", current->data, current);
				printf("str\t%s\n", str);
				stack_print();
			}
			if(top == NULL){
				strcat(str, ".");
				current = current->right;
				continue;
			}
			popped = stack_pop();
			if(DEBUG) printf("popped\t%c\t%p\n", popped->data, popped);
			if(current == popped){
				strcat(str, "-");
				current = current->left;
			}else{
				while(current->right != popped){
					strcat(str, ".");
					current = current->right;
				}
				strcat(str, ".-");
				current = current->right->left;
				// strcat(str, ".");
				// current = current->right;
			}
		}
		stack_clear();
		return str;
	}else
		return NULL;
}
void newscreen(){
	int i;
	for(i=0;i<30;i++)
		printf("\n");
}
int displayMenu(){
	int option;
	newscreen();
	printf("\n\nWELCOME TO THE GUZMORSE CONVERTER!!\n\n"\
		"What would you like to do?\n"\
		"\t1. Morse -> Alphanumeric\n"\
		"\t2. Alphanumeric -> Morse\n"\
		"\t3. Credits\n"\
		"\n\t0. Exit\n");
	// printf("f%s\n",fgets(input, 5, stdin));
	// printf("%s\n", input);
	// option = (*input)-'0';
	option = getchar() - '0';
	getchar(); //Dirty way of getting rid of a \n
	// printf("You entered %d\n", option);
	return option;
}

int main(){
	int option = 1;//Avoid it being 0
	tree_init();
	while(option!=0){
		newscreen();
		option = displayMenu();
		newscreen();
		switch(option){
			case 0:
				printf("\nSee you soon! :)\n");
				break;
			case 1:;// Empty statement to make the compiler happy (Label cannot be 
				// followed by declaration)
				char morse[6];
				printf("MORSE TO ALPHANUMERIC CONVERSION\n"\
					"Enter an empty line to exit\n\n");
				while(1){
					//*morse = 0;
					printf("Enter the morse code for the character: ");
					// scanf("%s", morse); //Didn't accept empty string
					fgets(morse, 6, stdin);
					if(*morse == '\n') break;
					printf("\nYour character is %c\n\n\n", fromMorse(morse));
				}
				break;
			case 2:;
				char c[2];
				char* sol;
				printf("ALPHANUMERIC TO MORSE CONVERSION\n"\
					"Enter an empty line to exit\n\n");
				while(1){

					printf("Enter the character to translate: ");
					fgets(c, 3, stdin);
					if(*c == '\n') break;
					sol = toMorse(toupper(*c));
					if(sol == NULL){
						printf("\nInvalid character.\n");
						continue;
					}
					printf("\nThe morse for %c is %s\n\n\n", *c, sol);
					free(sol);
				}
				break;
			case 3:
				printf("Copyright © 2014 by Carlos Guzmán Fernández");
				getchar();
				break;
			default:
				printf("Invalid option\n");
				break;
		}
	}
	tree_clear();
}