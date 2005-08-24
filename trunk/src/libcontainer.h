#ifndef __LIB_CONTAINER_837284789237489723894798237
#define __LIB_CONTAINER_837284789237489723894798237

/* tree */

#define TREE_NULL_NODE ((Tree_node *)0)

#define TREE_GET_ROOT_NODE(header) header->root
#define TREE_GET_DATA_FROM_NODE(node) node->data
#define TREE_GET_COUNT(header) header->count

typedef void Tree_data;

typedef void (* Tree_data_free_func)(Tree_data *);
typedef int (* Tree_data_compare_func)(Tree_data *, Tree_data *);

typedef struct __TREE_NODE {
  Tree_data *data;

  struct __TREE_NODE *parent;

  struct __TREE_NODE *left;
  struct __TREE_NODE *right;
} Tree_node;

typedef struct __TREE_HEADER {
  Tree_node *root;
  unsigned int count;

  Tree_data_compare_func compare_func;
} Tree_header;

Tree_header *tree_init(Tree_data_compare_func compare_func);
Tree_node *tree_add_node(Tree_header *header, Tree_data *data);
int tree_del_node(Tree_header *header, Tree_node *node,
				  Tree_data_free_func free_func);
Tree_node *tree_find_by_data(Tree_header *header, Tree_data *needle,
							 Tree_data_compare_func compare_func);
void tree_destroy(Tree_header *header, Tree_data_free_func free_func,
				  Tree_node *start_node);

/* list */

#define LIST_NULL_NODE ((List_node *)0)

/* some simple helpers */
#define LIST_GET_HEAD_NODE(header) header->head
#define LIST_GET_TAIL_NODE(header) header->tail

#define LIST_GET_DATA_FROM_NODE(node) node->data
#define LIST_GET_COUNT(header) header->count
#define LIST_FOREACH(header, node) for (node = header->head;		\
										node != ((List_node *)0);	\
										node = node->next)

typedef void List_data;

/* functions for callbacks */
typedef void (* List_data_free_func)(List_data *);
typedef int (* List_data_compare_func)(List_data *, List_data *);

/* a node */
typedef struct __LIST_NODE {
  List_data *data;

  struct __LIST_NODE *next;
  struct __LIST_NODE *prev;
} List_node;

/* the header (think of it as the list) */
typedef struct __LIST_HEADER {
  unsigned int count;

  List_node *head;
  List_node *tail;
} List_header;

List_header *list_init(void);
void list_reverse(List_header *header);

void list_swap_nodes(List_node *node1, List_node *node2);
void list_bubble_sort(List_header *header,
					  List_data_compare_func compare_func);
int list_remove_dups(List_header *header,
					 List_data_compare_func compare_func,
					 List_data_free_func free_func);
void list_destroy(List_header *header,
				  List_data_free_func free_func);
int list_del_node(List_header *header, List_node *node,
				  List_data_free_func free_func);

List_node *list_node_by_index(List_header *header, int index);
List_node *list_append_node(List_header *header, List_data *data);
List_node *list_prepend_node(List_header *header, List_data *data);
List_node *list_insert_node(List_header *header, List_data *data, int index);

/* queue */

/* some simple helpers */
#define QUEUE_GET_DATA_FROM_NODE(node) node->data
#define QUEUE_GET_COUNT(header) header->list->count
#define QUEUE_EXTRACT_LIST(header) header->list
#define QUEUE_FOREACH(header, node) for (node = header->cur_pointer; \
										 node != ((List_node *)0);	 \
										 node = node->next)

typedef struct __QUEUE_HEADER {
  List_header *list;
  List_node *cur_pointer;
} Queue_header;

Queue_header *queue_init(void);
List_node *queue_dequeue(Queue_header *header);
List_node *queue_enqueue(Queue_header *header, List_data *data);
void queue_destroy(Queue_header *header, List_data_free_func free_func);

/* stack */

/* some simple helpers */
#define STACK_GET_DATA_FROM_NODE(node) node->data
#define STACK_GET_COUNT(header) header->list->count
#define STACK_EXTRACT_LIST(header) header->list
#define STACK_FOREACH(header, node) for (node = header->cur_pointer; \
										 node != ((List_node *)0);	 \
										 node = node->next)

typedef struct __STACK_HEADER {
  List_header *list;
  List_node *cur_pointer;
} Stack_header;

Stack_header *stack_init(void);
List_node *stack_pop(Stack_header *header);
List_node *stack_push(Stack_header *header, List_data *data);
void stack_destroy(Stack_header *header, List_data_free_func free_func);

/* hash table */

#define HASH_NULL_NODE ((Hashtable_node *)0)

#define HASH_GET_COUNT(header) header->list_header->count

typedef void Hashtable_data;

typedef unsigned long (* Hashing_function)(const unsigned char *);

typedef struct __HASHTABLE_HEADER {
  Tree_header *tree_header;
  Hashing_function hash_function;
} Hashtable_header;

typedef struct __HASHTABLE_NODE {
  unsigned long hash;
  Hashtable_data *data;
} Hashtable_node;

/* some hash functions to choose from */
unsigned long hash_medium(const unsigned char *str);
unsigned long hash_strong(const unsigned char *str);

Hashtable_header *hashtable_init(Hashing_function hash_function);
Hashtable_node *hashtable_insert_node(Hashtable_header *header,
									  const unsigned char *hash,
									  Hashtable_data *data);

#endif
