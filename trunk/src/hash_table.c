#include <stdio.h>
#include <stdlib.h>

#include <libcontainer.h>

unsigned long hash_djb2(const unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  
  while ((c = *str++))
	hash = ((hash << 5) + hash) + c;

  return (hash);
}

unsigned long hash_sdbm(const unsigned char *str) {
  unsigned long hash = 0;
  int c;
  
  while ((c = *str++))
	hash = c + (hash << 6) + (hash << 16) - hash;
  
  return (hash);
}

