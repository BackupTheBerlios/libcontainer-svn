#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libcontainer.h>
#include "extra.h"

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

/* A simple bubble sort for the list. */
void list_bubble_sort(List_header *header,
					  List_data_compare_func compare_func) {
  List_node *i_node = header->head;
  List_node *j_node;

  while (i_node) {
	j_node = header->head;

	while (j_node) {
	  /* compare and swap if need be */
	  if (compare_func(i_node->data, j_node->data) < 0)
		list_swap_nodes(i_node, j_node);

	  j_node = j_node->next;
	}
	i_node = i_node->next;
  }  
}

/* swap two nodes */
void list_swap_nodes(List_node *node1, List_node *node2) {
  List_data *tmp_data;

  tmp_data = node1->data;

  /* just swap the data TODO: do this proper like */
  node1->data = node2->data;
  node2->data = tmp_data;
}

/* Grab a node by its index. This will be sllloooowwww. */
List_node *list_node_by_index(List_header *header, int index) {
  int i;
  List_node *node = header->head;

  /* check we are within bounds */
  if (index < 0 || index > header->count - 1)
	return (LIST_NULL_NODE);

  /* only loop index amount of times */
  for (i = 0; i < index; i++)
	node = node->next;

  return (node);
}

/* Delete any dups from the list. Actually attempts to free the memory
   so the data will be gone forever. */
int list_remove_dups(List_header *header,
					 List_data_compare_func compare_func,
					 List_data_free_func free_func) {
  int del_count = 0;
  List_node *i_node = header->head;
  List_node *j_node, *tmp_node;

  while (i_node) {
	j_node = header->head;
	while (j_node) {

	  /* don't compare against self */
	  if (j_node == i_node) {
		j_node = j_node->next;
		continue;
	  }

	  /* compare the nodes */
	  if (compare_func(i_node->data, j_node->data) == 0) {
		tmp_node = j_node->next;

		/* del the node (and its data) */
		list_del_node(header, j_node, free_func);
		j_node = tmp_node;
		del_count++;
		continue;
	  }
	  j_node = j_node->next;
	}
	i_node = i_node->next;
  }

  return (del_count);
}

void list_randomise(List_header *header) {

}

/* Create a new List_header object */
List_header *list_init(void) {
  List_header *header;
  
  if (!(header = (List_header *)malloc(sizeof(List_header)))) {
#ifdef DEBUG
	extra_warn("Memory allocation failure in list_init()");
#endif
	return ((List_header *)0);
  }
  
  header->head = LIST_NULL_NODE;
  header->tail = LIST_NULL_NODE;

  header->count = 0;
  
  return (header);
}

/* Reverse the list in place */
void list_reverse(List_header *header) {
  List_node *cur_node = header->head;
  List_node *tmp_node = header->tail;

  /* swap head and tail first */
  header->tail = header->head;
  header->head = tmp_node;

  /* now loop swapping nodes partners */
  while (cur_node) {
	tmp_node = cur_node;
	cur_node = tmp_node->next;

	tmp_node->next = tmp_node->prev;
	tmp_node->prev = cur_node;
  }
}

/* Free the list and all of the data */
void list_destroy(List_header *header,
				  List_data_free_func free_func) {
  List_node *current_node;
  List_node *next_node;
  
  current_node = header->head;
  
  /* loop the nodes calling free() */
  while (current_node) {
	next_node = current_node->next;

#ifdef DEBUG
	extra_warn("Freeing node and its data at: %p\n", current_node);
#endif

	/* use the users free func */
	free_func(current_node->data);

	free(current_node);
	current_node = next_node;
  }
  free(header);
  
  header = ((List_header *)0);
}

/* Remove a single node from the list */
int list_del_node(List_header *header, List_node *node,
				  List_data_free_func free_func) {
  /* my head hurts */
  if (node->prev != LIST_NULL_NODE) {
	if (node->next != LIST_NULL_NODE) {
	  node->prev->next = node->next;
	  node->next->prev = node->prev;
	} else {
	  node->prev->next = LIST_NULL_NODE;
	  header->tail = node->prev;
	}
  } else {
	if (node->next != LIST_NULL_NODE) {
	  node->next->prev = LIST_NULL_NODE;
	  header->head = node->next;
	} else {
	  header->head = LIST_NULL_NODE;
	  header->tail = LIST_NULL_NODE;
	}
  }

  /* if free_func is defined then use it */
  free_func(node->data);
  free(node);
  
  header->count--;

  return (1);
}

static List_node *get_new_node(List_header *header, List_data *data) {
  List_node *node;
  
  /* allocate space for the actual node */
  if (!(node = (List_node *)malloc(sizeof(List_node)))) {
#ifdef DEBUG
	extra_warn("Memory allocation failure in list_init()");
#endif
	return (LIST_NULL_NODE);
  }
  
  node->data = data;
  node->prev = LIST_NULL_NODE;
  node->next = LIST_NULL_NODE;

#ifdef DEBUG
  extra_warn("New node created at %p\n", node);
#endif

  return (node);
}

/* Add a node to the end of the list */
List_node *list_append_node(List_header *header, List_data *data) {
  return (list_insert_node(header, data, header->count));
}

/* Add a node to the beginning of the list */
List_node *list_prepend_node(List_header *header, List_data *data) {
  return (list_insert_node(header, data, 0));
}

/* Insert a node at place index:
   list_insert_node(hdr, "hello", 0) will insert at start
   list_insert_node(hdr, "hello", 3) will insert after 2
   
   Will return LIST_NULL_NODE if index is out of range
*/
List_node *list_insert_node(List_header *header, List_data *data, int index) {
  List_node *node, *prev_node, *next_node;
  
  /* get our new node first */
  if ((node = get_new_node(header, data)) == LIST_NULL_NODE)
	return (LIST_NULL_NODE);
  
  if (index > header->count || index < 0)
	return (LIST_NULL_NODE);

  /* grab the pre and post nodes */
  prev_node = list_node_by_index(header, index - 1);
  next_node = list_node_by_index(header, index);
  
  node->prev = prev_node;
  node->next = next_node;
  
  if (prev_node == LIST_NULL_NODE)
	header->head = node;
  else 
	prev_node->next = node;
  
  if (next_node == LIST_NULL_NODE)
	header->tail = node;
  else 
	next_node->prev = node;
  
  header->count++;

  return (node);
}
