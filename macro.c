#include <stdio.h>
#include <stdlib.h>

#define list_declare(type) \
typedef struct type##_lnode \
{ \
	type *data; \
	struct type##_lnode *next; \
}type##_node; \
\
void list_##type##_insert(type##_node** head, type *data);


#define list_define(type) \
void list_##type##_insert(type##_node** head, type *data) \
{\
	type##_node* nnode = malloc(sizeof(type##_node*)); \
	nnode -> data = data; \
	nnode -> next = NULL; \
\
	if(*head == NULL) \
		*head = nnode; \
\
	else \
	{ \
		type##_node* temp = *head; \
		while(temp->next!=NULL) \
			temp = temp->next; \
		temp->next = nnode; \
	} \
}

#define list_type(type) type##_node 

#define list_insert(type, list, data) list_##type##_insert(list, data)

list_declare(char)
list_define(char)
list_declare(char_node)
list_define(char_node)

int main()
{ 
	char* a = "Vandana";
	char* b = "Srilakshmi";
	list_type(char)* head1 = NULL;
	list_insert(char, &head1, a); 
	list_insert(char, &head1, b);

	list_type(char_node) *head = NULL;
	list_insert(char_node, &head, head1);

	printf("%s\n", ((head->data)->data));

	return 0;

}