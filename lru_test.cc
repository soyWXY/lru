#include <limits.h>
#include <vector>
extern "C" {
    #include "lru.h"
    #include "lru_test_internal.h"
}
#include "gtest/gtest.h"

struct pair {
    pair(int k, int v) : key(k), value(v) {};
    int key, value;
};

/**
 * single_pairs and two_pairs are constucted with random constants.
 * 
 * use them in basic tests (PutOneAndGetOne_GetExpect and  DiffKey_GetBothExpect)
 * and the advance tests. if the advance tests fail due to keys and values, basic
 * tests will fail. if the advance test fail and basic test pass, it's clear keys
 * and values  are irrelevant, so you can focus  on the condition that each case 
 * wanna to test.
 */

/* not equal to any value of single_pairs */
const int kAnotherRandomValue = 819;

using std::vector;
const vector<pair> single_pairs = {
    pair(7991, INT_MAX - 1), pair(1321, 5445), pair(4230, 1321)
};

/**
 * for current hash, some two_pairs have collide keys, while some don't.
 * this makes better coverage.
 */
const vector<vector<pair>> two_pairs = {
    {single_pairs[0], single_pairs[1]},
    {single_pairs[1], single_pairs[2]},
    {single_pairs[1], single_pairs[0]},
};

class LruTestGetAndPut : public testing::TestWithParam<pair> {};
class LruTestDiffKeyGetAndPut : public testing::TestWithParam<vector<pair>> {};

void HelperPutWithPair(LRUCache *cache, const pair &p)
{
    lRUCachePut(cache, p.key, p.value);
}

TEST_P(LruTestGetAndPut, PutOneAndGetOne_GetExpect)
{
    LRUCache *cache = lRUCacheCreate(2);
    pair p = GetParam();
    lRUCachePut(cache, p.key, p.value);
    int expect = p.value,
        get = lRUCacheGet(cache, p.key);
    lRUCacheFree(cache);

    EXPECT_EQ(get, expect);
}

INSTANTIATE_TEST_SUITE_P(PutGetOnce, LruTestGetAndPut,
    testing::Values(
        // pair(INT_MIN, kAnotherRandomValue),
        // pair(INT_MIN + 1, kAnotherRandomValue),
        // pair(-1, kAnotherRandomValue),
        pair(0, kAnotherRandomValue),
        pair(INT_MAX, kAnotherRandomValue),
        pair(INT_MAX - 1, kAnotherRandomValue),
        single_pairs[0], single_pairs[1], single_pairs[2]
    )
);

TEST_P(LruTestDiffKeyGetAndPut, DiffKey_GetBothExpect)
{
    LRUCache *cache = lRUCacheCreate(2);
    vector<pair> p = GetParam();
    lRUCachePut(cache, p[0].key, p[0].value);
    lRUCachePut(cache, p[1].key, p[1].value);
    int expect1 = p[0].value, expect2 = p[1].value,
        get1 = lRUCacheGet(cache, p[0].key),
        get2 = lRUCacheGet(cache, p[1].key);
    lRUCacheFree(cache);

    EXPECT_EQ(get1, expect1);
    EXPECT_EQ(get2, expect2);
}

INSTANTIATE_TEST_SUITE_P(DiffKeyPutGet, LruTestDiffKeyGetAndPut,
    testing::Values(
        two_pairs[0], two_pairs[1], two_pairs[2]
    )
);

TEST(LruTest, InitialCacheCheck_GetMinusOne)
{
    LRUCache *cache = lRUCacheCreate(2);
    int get = lRUCacheGet(cache, single_pairs[1].key);
    lRUCacheFree(cache);

    EXPECT_EQ(get, -1);
}

TEST(LruTest, SameKeyRefreashValue_GetNewValue)
{
    LRUCache *cache = lRUCacheCreate(2);
    const int key = single_pairs[0].key,
              expect = kAnotherRandomValue;
    HelperPutWithPair(cache, single_pairs[0]);
    lRUCachePut(cache, key, expect);
    int get = lRUCacheGet(cache, key);
    lRUCacheFree(cache);

    EXPECT_EQ(get, expect);
}

TEST(LruTest, SameKeyMoveToHead_CountUnchanged)
{ 
    LRUCache *cache = lRUCacheCreate(3);
    HelperPutWithPair(cache, single_pairs[0]);
    HelperPutWithPair(cache, single_pairs[1]);
    lRUCachePut(cache, single_pairs[0].key, kAnotherRandomValue);
    int expect_count = 2, count = lRUCacheCount(cache);
    lRUCacheFree(cache);

    EXPECT_EQ(count, expect_count);
}

TEST(LruTest, SameKeyMoveToHead_HeadKeyIsTheKeyPutTwice)
{
    LRUCache *cache = lRUCacheCreate(3);
    HelperPutWithPair(cache, single_pairs[0]);
    HelperPutWithPair(cache, single_pairs[1]);
    lRUCachePut(cache, single_pairs[0].key, kAnotherRandomValue);

    int head_key,
        expect_key = single_pairs[0].key,
        get = lRUCacheGetHead(cache, &head_key);
    lRUCacheFree(cache);

    ASSERT_NE(get, -1) << "Cache count is zero. Cannot find head.";
    EXPECT_EQ(head_key, expect_key);
}

TEST(LruTest, ExceedCapacityRemoveLru_ReturnMinusOne)
{
    LRUCache *cache = lRUCacheCreate(2);
    HelperPutWithPair(cache, single_pairs[0]);
    HelperPutWithPair(cache, single_pairs[1]);
    HelperPutWithPair(cache, single_pairs[2]);
    int get = lRUCacheGet(cache, single_pairs[0].key);
    lRUCacheFree(cache);

    EXPECT_EQ(get, -1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

