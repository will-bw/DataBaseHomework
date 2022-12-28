/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace cmudb {

template <typename T> LRUReplacer<T>::LRUReplacer() 
{
    this->eleNum=0;
}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}


/*
 * Insert value into LRU
 */
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    this->DelTrace(value);
    auto node = std::make_shared<Node>(value);
    if (node != nullptr) 
    {
        node->pre = nullptr;
        node->next = this->first;
        if(this->eleNum==0)
        {
            this->first=node;
            this->last=node;
        }
        else
        {
            this->first->pre = node;
            this->first = node;
        } 
    }
    eleNum++;
    eleIndex[node->value] = node;
}


/* If LRU is non-empty, pop the heabd member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
template <typename T> bool LRUReplacer<T>::Victim(T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    if (this->eleNum == 0) {
        return false;
    }
    else if (this->eleNum==1) 
    {
        value = last->value;
        first = nullptr;
        last = nullptr;
    
        eleIndex.erase(value);
        eleNum--;
        return true;
    }
    else
    {
        value = last->value;
        std::shared_ptr<Node> temp = last;

        last->pre->next=nullptr;
        last = last->pre;
        temp->pre = nullptr;
        
        eleIndex.erase(value);
        eleNum--;
        return true;
    }
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
    std::lock_guard<std::mutex> guard(mutex);

    return DelTrace(value);
}

template <typename T> size_t LRUReplacer<T>::Size()
 { 
     return this->eleNum; 
     }

template <typename T> bool LRUReplacer<T>::DelTrace(const T &value) {
    auto test = eleIndex.find(value);
    //can not find
    if (test == eleIndex.end()) 
    {
        return false;
    }
    else
    {
        auto ptr = test->second;
        //
        if(ptr!=first && ptr!=last)
        {
            ptr->pre->next = ptr->next;
            ptr->next->pre = ptr->pre;
        }
        else if(ptr == first && ptr!=last)
        {
            ptr->next->pre = nullptr;
            first = ptr->next;
        }
        else if(ptr != first && ptr==last)
        {
            ptr->pre->next = nullptr;
            last = ptr->pre;
        }
        else{
            first = nullptr;
            last = nullptr;
        }
        //
        ptr->pre = nullptr;
        ptr->next = nullptr;

        eleNum--;
        eleIndex.erase(value);  
    return true;
    }
}

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace cmudb
