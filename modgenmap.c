#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

#define map_declare(keytype, valuetype) \
typedef struct keytype##_##valuetype##_mnode \
{ \
	keytype *key; \
	valuetype##_node *vhead;  \
	struct keytype##_##valuetype##_mnode *next; \
}keytype##_##valuetype##_node; \
typedef struct hash \
{ \
	keytype##_##valuetype##_node *head; \
}hash_cell; \
\
void map_##keytype##_##valuetype##_insert(keytype##_##valuetype##_node** head, keytype *k, valuetype *v);

int hashcode_char(char* key);
int hashcode_int(int* key);
int hash_char_Index(int h, char* a);
int hash_int_Index(int h, int* a);
int hash_func(int hashcode);

int hashcode_char(char* key)
{
	int i;
	int code = 0;
	int len = 0;
	len = strlen(key)-1;
	
	for(i = 0; i <= len; ++i)
	{
		code = code + key[i]*pow(31, len-i);
	}
	
	return code;
}

int hashcode_int(int* key)
{
	int code = 0;
	code = 31 * (*key) + (int) ((*key) >> 32);
	return code;
} 
int hash_char_Index(int h, char* a)
{
	int length;
	length = strlen(a);
	return h & (length - 1);
}

int hash_int_Index(int h, int* a)
{
	int num_digits = 0;
	int temp;
	temp = *a;
	while(temp != 0)
	{
		num_digits = num_digits + 1;
		temp = temp/10;
	}
	return h & (num_digits - 1);
}
	
int hash_func(int hashcode)
{
	hashcode ^= (hashcode >> 20) ^ (hashcode >> 12);
	return hashcode ^ (hashcode >> 7) ^ (hashcode >> 4);
}

#define map_define(keytype, valuetype) \
void map_##keytype##_##valuetype##_insert(keytype##_##valuetype##_node** head, keytype *k, valuetype *v) \
{\
	keytype##_##valuetype##_node* nnode = (keytype##_##valuetype##_node*)malloc(sizeof(keytype##_##valuetype##_node)); \
	nnode -> key = k; \
	nnode -> vhead = NULL; \
	nnode -> next = NULL; \
	list_insert(valuetype, &(nnode->vhead), v); \
	if(*head == NULL) \
		*head = nnode; \
\
	else \
	{ \
		keytype##_##valuetype##_node* temp = *head; \
		while(temp->next!=NULL) \
			temp = temp->next; \
		temp->next = nnode; \
	} \
}


#define hash_Index(type, h, a) hash_##type##_Index(h, a)
#define hashcode(type, key) hashcode_##type(key)

#define list_type(type) type##_node 
#define map_type(keytype, valuetype) keytype##_##valuetype##_node

#define list_insert(type, list, data) list_##type##_insert(list, data)
#define map_insert(keytype, valuetype, list, key, value) map_##keytype##_##valuetype##_insert(list, key, value)

list_declare(char)
list_define(char)
map_declare(int, char)
map_define(int, char)

int main()
{ 
	char* a = "Vandana";
	char* b = "Srilakshmi";
	list_type(char)* head1 = NULL;
	list_insert(char, &head1, a); 
	list_type(char)* head2 = NULL;
	list_insert(char, &head2, b);

	printf("%s\n", (head1->data));

	hash_cell hash_table[16];
	int i;
	
	for(i = 0; i < 16; ++i)
	{
		hash_table[i].head = NULL;
	}

	int key1 = 47245;
	int key2 = 74;

	int h1 = hashcode(int, &key1);
	int hash1 = hash_func(h1);
	int index1 = hash_Index(int, hash1, &key1);

	int h2 = hashcode(int, &key2);
	int hash2 = hash_func(h2);
	int index2 = hash_Index(int, hash2, &key2);

	printf("%d\n", index1);
	printf("%d\n", index2);

	//write  structure to hold the head variable and initialize the array with this struct kind
	map_insert(int, char, &hash_table[index1].head, &key1, a);
	map_insert(int, char, &hash_table[index1].head, &key2, b);

	printf("%s\n", (((hash_table[index1].head)->vhead)->data));
	return 0;

}