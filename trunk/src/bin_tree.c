#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include <libcontainer.h>
#include "extra.h"

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

Tree_header *tree_init(Tree_data_compare_func compare_func) {
  Tree_header *header;
  
  if (!(header = (Tree_header *)malloc(sizeof(Tree_header))))
	return ((Tree_header *)0);

  header->compare_func = compare_func;
  header->root = TREE_NULL_NODE;
  header->count = 0;
  
  return (header);
}

Tree_node *tree_find_by_data(Tree_header *header, Tree_data *needle,
							 Tree_data_compare_func compare_func) {
  int compare_res;
#ifdef DEBUG
  int hops = 0;
#endif
  Tree_node *node = header->root;

  while (node) {
	/* compare the data */
	compare_res = header->compare_func(needle, node->data);

	if (compare_res > 0) {
	  node = node->right;
	} else if (compare_res < 0) {
	  node = node->left;
	} else {
#ifdef DEBUG
	  /* keep our subscribed viewers up to date */
	  extra_warn("Found node in tree_find_by_data() at %p after %d hops\n",
				 node, hops);
#endif
	  return (node);
	}
#ifdef DEBUG
	/* update the amount of times we have hopped */
	hops++;
#endif
  }

  /* no node was found */
#ifdef DEBUG
  extra_warn("No matching node found using tree_find_by_data()\n");
#endif

  return (TREE_NULL_NODE);
}

int tree_del_node(Tree_header *header, Tree_node *node,
				  Tree_data_free_func free_func) {
  int compare_res;
  Tree_node *parent;

  if (!header->root || !node)
	return (0);

  parent = node->parent;

  compare_res = header->compare_func(node->data, parent->data);

  /* data musn't match here */
  assert(compare_res != 0);

  if (compare_res > 0) {
	/* node is right of its parent */
	parent->right = node->right;
  } else {
	/* node is left of its parent */
	parent->left = node->left;
  }

  free_func(node->data);
  header->count--;

  return (1);
}

static Tree_node *get_new_node(Tree_header *header, Tree_data *data) {
  Tree_node *node;
  
  /* allocate space for the actual node */
  node = (Tree_node *)malloc(sizeof(Tree_node));
  
  /* if malloc fail inform the user and return null */
  if (node == TREE_NULL_NODE)
	return (TREE_NULL_NODE);
  
  node->data = data;

  node->left = TREE_NULL_NODE;
  node->right = TREE_NULL_NODE;
  node->parent = TREE_NULL_NODE;

  return (node);
}

Tree_node *tree_add_node(Tree_header *header, Tree_data *data) {
  Tree_node *new_node = get_new_node(header, data);
  Tree_node *tmp_node;

  if (new_node == TREE_NULL_NODE)
	return (TREE_NULL_NODE);

  /* if the list is empty then root becomes the new node */
  if (!header->count) {
	header->root = new_node;
	header->count++;
	return (new_node);
  }

  /* set the initial node to the root */
  tmp_node = header->root;

  while (tmp_node) {
	int compare_res = header->compare_func(data, tmp_node->data);

	/* data can't be duplicated for the time being */
	assert(compare_res != 0);

	if (compare_res < 0) {
	  if (!tmp_node->left) {
		/* we have reached the end. Add the node here */
		tmp_node->left = new_node;
		new_node->parent = tmp_node;
		header->count++;
		return (new_node);
	  }

	  tmp_node = tmp_node->left;
	} else {
	  if (!tmp_node->right) {
		/* we have reached the end. Add the node here */
		tmp_node->right = new_node;
		new_node->parent = tmp_node;
		header->count++;
		return (new_node);
	  }

	  tmp_node = tmp_node->right;
	}
  }

  return (new_node);
}

void tree_destroy(Tree_header *header, Tree_data_free_func free_func,
				  Tree_node *start_node) {
  Tree_node *tmp_node = start_node;
 
  if (!header || !header->root || !start_node)
	return;

  free_func(start_node->data);
  free(start_node);

  tree_destroy(header, free_func, tmp_node->left);
  tree_destroy(header, free_func, tmp_node->right);
}
