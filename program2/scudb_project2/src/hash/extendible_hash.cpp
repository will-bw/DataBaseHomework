#include <list>

#include "hash/extendible_hash.h"
#include "page/page.h"

namespace cmudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size)
{
    this->globalDepth=0;
    this->bucketMaxSize=size;
    this->numBuckets=0;
    bucketTable.push_back(std::make_shared<Bucket>(0));
}

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) const
{
  return std::hash<K>{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const
{
  return globalDepth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const
{
  return bucketTable[bucket_id]->localDepth;
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const 
{
  return numBuckets;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) 
{  
  std::lock_guard<std::mutex> guard(mutex);
  auto index = HashKey(key) & ((1 << globalDepth) - 1);
  std::shared_ptr<Bucket> bucket = bucketTable[index];
  if (bucket != nullptr && bucket->items.find(key) != bucket->items.end()) {
    value = bucket->items[key];
    return true;
  }
  return false;
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) 
{
  std::lock_guard<std::mutex> guard(mutex);
  auto index =HashKey(key) & ((1 << globalDepth) - 1);
  std::shared_ptr<Bucket> bucket = bucketTable[index];
  //can not find
  if (bucket == nullptr || bucket->items.find(key) == bucket->items.end()) 
  {
    return false;
  }
  else
  {
  bucket->items.erase(key);
  return true;
  }
}


/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) 
{
  std::lock_guard<std::mutex> guard(mutex);

  auto index = HashKey(key) & ((1 << globalDepth) - 1);
  std::shared_ptr<Bucket> targetBucket = bucketTable[index];

  // 桶满了
  while (targetBucket->items.size() == bucketMaxSize)
   {
    if (targetBucket->localDepth == globalDepth) 
    {
      size_t length = bucketTable.size();
      for (size_t i = 0; i < length; i++) 
      {
        bucketTable.push_back(bucketTable[i]);
      }
      globalDepth++;
    }
   

  //divide into two Bucket
    auto zeroBucket = std::make_shared<Bucket>(targetBucket->localDepth + 1);
    auto oneBucket = std::make_shared<Bucket>(targetBucket->localDepth + 1);
    
    //flag for bucket
    int flag = 1 << targetBucket->localDepth;

    for (auto item : targetBucket->items)
     {
      size_t hashkey = HashKey(item.first);
      if (hashkey & flag)
      {
        oneBucket->items.insert(item);
      } 
      else 
      {
        zeroBucket->items.insert(item);
      }
    }

    for (size_t i = 0; i < bucketTable.size(); i++) {
      if (bucketTable[i] == targetBucket) {
        if (i & flag) {
          bucketTable[i] = oneBucket;
        } else {
          bucketTable[i] = zeroBucket;
        }
      }
    }

    //
    index = HashKey(key) & ((1 << globalDepth) - 1);
    targetBucket = bucketTable[index];
  } //

  targetBucket->items[key] = value;
}

template class ExtendibleHash<page_id_t, Page *>;
template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
} // namespace cmudb
