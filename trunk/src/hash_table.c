#include <stdio.h>
#include <stdlib.h>

#include <libcontainer.h>
#include "extra.h"

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

unsigned long hash_medium(const unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  
  while ((c = *str++))
	hash = ((hash << 5) + hash) + c;

  return (hash);
}

unsigned long hash_strong(const unsigned char *str) {
  unsigned long hash = 0;
  int c;
  
  while ((c = *str++))
	hash = c + (hash << 6) + (hash << 16) - hash;
  
  return (hash);
}

static int node_compare_func(Tree_data *data1, Tree_data *data2) {
  return (0);
  //return (data1->hash != data2->hash);
}

Hashtable_node *hashtable_insert_node(Hashtable_header *header,
									  const unsigned char *hash,
									  Hashtable_data *data) {
  Hashtable_node *node;
  unsigned long converted_hash;

  /* generate the hash */
  converted_hash = header->hash_function(hash);

#ifdef DEBUG
  libcontainer_warn(WARN_ARGS, "Hash for %s has become %lu.\n",
					hash, converted_hash);
#endif
  
  return (node);
}

Hashtable_header *hashtable_init(Hashing_function hash_function) {
  Hashtable_header *table;

  if (!(table = (Hashtable_header *)malloc(sizeof(Hashtable_header)))) {
#ifdef DEBUG
	libcontainer_warn(WARN_ARGS, "Memory allocation failure in "
					  "hashtable_init()");
#endif
	return ((Hashtable_header *)0);
  }

  /* fill in the header */
  table->hash_function = hash_function;
  table->tree_header = tree_init(node_compare_func);

  /* If the list init fails then return null */
  if (table->tree_header == ((Tree_header *) 0))
	return ((Hashtable_header *) 0);

  return (table);
}
