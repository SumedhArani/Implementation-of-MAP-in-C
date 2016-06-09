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
	type##_node* temp = *head; \
\
	nnode -> data = data; \
	nnode -> next = NULL; \
\
	if(*head == NULL) \
		*head = nnode; \
\
	else \
	{ \
		while(temp->next!=NULL) \
			temp = temp->next; \
		temp->next = nnode; \
	} \
}

#define map_declare(keytype, valuetype) \
list_declare(valuetype)\
list_define(valuetype)\
typedef struct keytype##_##valuetype##_mnode \
{ \
	keytype *key; \
	valuetype##_node *vhead; \
	struct keytype##_##valuetype##_mnode *next; \
}keytype##_##valuetype##_node; \
typedef struct hash \
{ \
	keytype##_##valuetype##_node *head; \
}hash_cell; \
\
void map_##keytype##_##valuetype##_insert(keytype##_##valuetype##_node** head, keytype *k, valuetype *v);\
void keytype##_##valuetype##hash_sfind(char* str);\
void keytype##_##valuetype##hash_uinsert(char* str);\
\
void map_keytype##_valuetype##_delete(keytype* k, int index);\
void keytype##_##valuetype##_hash_sdelete(char* str);\
\
void map_keytype##_valuetype##_display(keytype* k, int index);\
void keytype##_##valuetype##_hash_sdisplay(char* str);\
\
hash_cell keytype##_valuetype##_hash_table[16];\
void map_keytype##_valuetype##_initialize();\
void map_keytype##_valuetype##_initialize() \
{ \
	for(int i =0; i<16; i++) \
		keytype##_valuetype##_hash_table[i].head = NULL; \
}

int hashcode_char(char* key);
int hashcode_int(int* key);
int hash_char_Index(int h, char* a);
int hash_int_Index(int h, int* a);
int hash_func(int hashcode);
int hash_user_func(char* key);

int hash_user_func(char* key)
{
	int i;
	int code = 0;
	int len = 0;
	len = strlen(key)-1;
	for(i = 0; i <= len; ++i)
	{
		if(key[i] >= 65 && key[i] <= 90)
			code += (key[i] - 65 + 1);
		else if(key[i] >= 97 && key[i] <= 122)
			code += (key[i] - 97 + 1);
		else if(key[i] >= 48 && key[i] <=57)
			code += (key[i] - 48 + 1);
	}
	
	code = ((code+12)*91)-95;
	return code % 16;
}

int hashcode_char(char *key)
{
	int i;
	int code = 0;
	int len = 0;
	len = strlen(key)-1;
	
	/*for(i = 0; i <= len; ++i)
	{
		code = code + key[i]*pow(31, len-i);
	}*/
	for(i = 0; i <= len; ++i)
	{
		if(key[i] >= 65 && key[i] <= 90)
			code += (key[i] - 65 + 1);
		else if(key[i] >= 97 && key[i] <= 122)
			code += (key[i] - 97 + 1);
		else if(key[i] >= 48 && key[i] <=57)
			code += (key[i] - 48 + 1);
	}
	
	return code % 16;
}

int hashcode_int(int *key)
{
	
	int code = 0;
	//code = 31 * (*key) + (int) ((*key) >> 32);
	code = ((*key) * 9) % 16; 
	
	return code;
} 
int hash_char_Index(int h, char* a)
{
	/*int length;
	length = strlen(a);
	return h & (length - 1);*/
	return h;
}

int hash_int_Index(int h, int* a)
{
	/*int num_digits = 0;
	int temp;
	temp = *a;
	while(temp != 0)
	{
		num_digits = num_digits + 1;
		temp = temp/10;
	}
	return h & (num_digits - 1);*/
	return h;
}
	
int hash_func(int hashcode)
{
	/*hashcode ^= (hashcode >> 20) ^ (hashcode >> 12);
	return hashcode ^ (hashcode >> 7) ^ (hashcode >> 4);*/
	return hashcode;
}

#define map_define(keytype, valuetype) \
void map_##keytype##_##valuetype##_insert(keytype##_##valuetype##_node** head, keytype *k, valuetype *v) \
{\
\
	keytype##_##valuetype##_node* temp;\
	temp = *head;\
	while(temp != NULL && strcmp(temp->key,k)!=0)\
	{\
			temp = temp->next; \
	}\
	if(temp == NULL)\
	{\
		keytype##_##valuetype##_node* nnode = (keytype##_##valuetype##_node*)malloc(sizeof(keytype##_##valuetype##_node)); \
		nnode -> key = k; \
		nnode -> vhead = NULL; \
		nnode -> next = NULL; \
		list_insert(valuetype, &(nnode->vhead), v); \
		\
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
	}\
	else\
	{\
		list_insert(valuetype, &(temp->vhead), v);\
	}\
}\
\
void keytype##_##valuetype##hash_sfind(char* str)\
{\
	char *token, *string, *tofree, *tokk, *tokv;\
	char *temptok, *ttok, *tok1;\
\
	keytype *k;\
	valuetype *v;\
\
	tofree = string = strdup(str);\
	\
	while((token = strsep(&string , ";"))!= NULL)\
	{	\
		tokk = strsep(&token, ":");\
		k = (keytype*)tokk;\
		temptok =strsep(&token, ":");\
		int h1 = hashcode(keytype, k);\
		int hash1 = hash_func(h1);\
		int index1 = hash_Index(keytype, hash1, k);\
\
			while((tokv = strsep(&temptok , ","))!= NULL)\
			{\
				v = (valuetype*)tokv;\
				map_##keytype##_##valuetype##_insert(&keytype##_valuetype##_hash_table[index1].head, k, v);\
			}\
	}\
	free(tofree);\
\
}\
void keytype##_##valuetype##hash_uinsert(char* str)\
{\
	char *token, *string, *tofree, *tokk, *tokv;\
	char *temptok, *ttok, *tok1;\
\
	keytype *k;\
	valuetype *v;\
\
	tofree = string = strdup(str);\
	\
	while((token = strsep(&string , ";"))!= NULL)\
	{	\
		tokk = strsep(&token, ":");\
		k = (keytype*)tokk;\
		temptok =strsep(&token, ":");\
		int index1 = hash_user_func(k);\
\
			while((tokv = strsep(&temptok , ","))!= NULL)\
			{\
				v = (valuetype*)tokv;\
				map_##keytype##_##valuetype##_insert(&keytype##_valuetype##_hash_table[index1].head, k, v);\
			}\
	}\
	free(tofree);\
\
}\
void map_keytype##_valuetype##_delete(keytype* k, int index)\
{\
	keytype##_##valuetype##_node* curr;\
	curr = keytype##_valuetype##_hash_table[index].head;\
	keytype##_##valuetype##_node* prev;\
	prev = NULL;\
	\
	if(keytype##_valuetype##_hash_table[index].head == NULL)\
	{\
		printf("Key not found\n");\
	}\
	else\
	{\
		while(curr != NULL && *(curr->key) != *k)\
		{\
			prev = curr;\
			curr = curr->next; \
		}\
		if(curr == NULL)\
		{\
			printf("Key not found\n");\
		}\
		else if(prev == NULL)\
		{\
			keytype##_valuetype##_hash_table[index].head = curr->next;\
		}\
		else\
			prev->next = curr->next;	}\
}\
void keytype##_##valuetype##_hash_sdelete(char* str)\
{\
		keytype* k;\
		k = (keytype*)str;\
		int h1 = hashcode(keytype, k);\
		int hash1 = hash_func(h1);\
		int index1 = hash_Index(keytype, hash1, k);\
		map_keytype##_valuetype##_delete(k, index1);\
}\
\
void map_keytype##_valuetype##_display(keytype* k, int index)\
{\
	keytype##_##valuetype##_node* temp;\
	temp = keytype##_valuetype##_hash_table[index].head;\
	if(keytype##_valuetype##_hash_table[index].head == NULL)\
	{\
		printf("Key not found\n");\
	}\
	else\
	{\
		while(temp != NULL && strcmp(temp->key,k)!=0)\
		{\
			temp = temp->next; \
		}\
		if(temp == NULL)\
		{\
			printf("Key not found\n");\
		}\
		else\
		{\
			valuetype##_node *t;\
			t = temp->vhead;\
			while(t != NULL)\
			{\
				printf("%s -> ", (char*)t->data);\
				t = t->next;\
			}\
			printf("\n");\
		}\
	}\
}\
\
void keytype##_##valuetype##_hash_sdisplay(char* str)\
{\
		keytype* k;\
		k = (keytype*)str;\
		int h1 = hashcode(keytype, k);\
		int hash1 = hash_func(h1);\
		int index1 = hash_Index(keytype, hash1, k);\
		map_keytype##_valuetype##_display(k, index1);\
}\
void keytype##_valuetype##_disp()\
{\
	int i;\
	for(i = 0; i < 16; ++i)\
	{\
		if(keytype##_valuetype##_hash_table[i].head != NULL)\
		{\
			keytype##_##valuetype##_node* temp;\
			temp = keytype##_valuetype##_hash_table[i].head;\
			while(temp != NULL)\
			{\
				printf("%d.  %s :", i,(temp -> key));\
				valuetype##_node *t;\
				t = temp->vhead;\
				while(t != NULL)\
				{\
					printf("%s -> ", (char*)t->data);\
					t = t->next;\
				}\
				temp = temp -> next;\
				printf("\n");\
			}\
		}\
	}\
}\


#define hash_Index(type, h, a) hash_##type##_Index(h, a)
#define hashcode(type, key) hashcode_##type(key)

#define list_type(type) type##_node 
#define list_insert(type, list, data) list_##type##_insert(list, data)

#define map_type(keytype, valuetype) keytype##_##valuetype##_node
#define map_initialize(keytype, valuetype) map_keytype##_valuetype##_initialize()
#define map_insert(keytype, valuetype, string) keytype##_##valuetype##hash_sfind(string)
#define map_delete(keytype, valuetype, delete_key) keytype##_##valuetype##_hash_sdelete(delete_key);
#define map_display(keytype, valuetype, search_key) keytype##_##valuetype##_hash_sdisplay(search_key);
#define map_uinsert(keytype, valuetype, string) keytype##_##valuetype##hash_uinsert(string);

#define table(keytype, valuetype) keytype##_valuetype##_hash_table
#define display(keytype, valuetype) keytype##_valuetype##_disp()

//!-------------------------------------x

map_declare(char, int)
map_define(char, int)
int main()
{ 
	char *k1 = (char*)malloc(100*sizeof(char));
	char* a = (char*)malloc(11000*sizeof(char));
	int k2, res;


	map_initialize(char, int);

	int input = 1;
	while(input)
	{
		printf("Enter the case\n1: Large files\n2: Composite key\n");
		scanf("%d", &input);

		switch(input)
		{
			case 1:	scanf("%s", a);
					map_insert(char, int, a);
					display(char, int);
					break;

			case 2: res = 0;
					char *key = (char*)malloc(100*sizeof(char));
					char *keytemp;
					char temp[100];
					int val1;
					char temp_val[100];

					printf("Enter the first key\n");
					scanf("%s", k1);

					int h = hashcode(char, k1);\
					int hash = hash_func(h);\
					int index = hash_Index(char, hash, k1);\

					printf("Enter the second key\n");
					scanf("%d", &k2);

					h = hashcode(int, &k2);
					hash = hash_func(h);
					index = hash_Index(int, hash, &k2);

					sprintf(temp,"%d",k2);
					key = strcat(k1, temp);
					keytemp = strdup(key); 

					printf("Enter the value\n");
					scanf("%d", &val1);
					sprintf(temp_val,"%d",val1);

					char *a;
					a = strcat(key, ":");
					a = strcat(key, temp_val);
					map_insert(char, int, a);

					display(char,int);
					break;

			case 0: exit(0);
					break;

		    default: break;
		}
	}
	
	return 0;
}