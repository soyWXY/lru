#ifndef LRU_TEST_INTERNAL_H_
#define LRU_TEST_INTERNAL_H_

/**
 * Functions declared here are only for test.
 */

/**
 * I reveal these functions because their logic is simple and they are
 * behavior known against calling code. If this action does more harm
 * than good, I make the method private again.
 * 
 * If the method is not simple (more than two logic), it might need test
 * for itself, and revealing them is not feasible.
 * 
 * A desire to use a method shows it might move to public in the future.
 */

typedef struct lru_cache LRUCache;

int lRUCacheGetHead(LRUCache *obj, int *key);
int lRUCacheCount(LRUCache *obj);

#endif