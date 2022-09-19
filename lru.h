#ifndef LRU_H_
#define LRU_H_

typedef struct lru_cache LRUCache;

LRUCache *lRUCacheCreate(int capacity);
void lRUCacheFree(LRUCache *obj);
int lRUCacheGet(LRUCache *obj, int key);
void lRUCachePut(LRUCache *obj, int key, int value);

#endif