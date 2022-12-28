/**
 * b_plus_tree_internal_page.cpp
 */
#include <iostream>
#include <sstream>

#include "common/exception.h"
#include "page/b_plus_tree_internal_page.h"

namespace cmudb {
/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/
/*
 * Init method after creating a new internal page
 * Including set page type, set current size, set page id, set parent id and set
 * max page size
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
Init(page_id_t page_id, page_id_t parent_id) {
 this->SetPageType(IndexPageType::INTERNAL_PAGE); //internal page
        this->SetSize(0);
        this->SetParentPageId(parent_id);
        this->SetPageId(page_id);
        //numbers of keys: bytes/(bytes for a key)
        SetMaxSize((PAGE_SIZE - sizeof(BPlusTreeInternalPage)) / sizeof(MappingType) - 1);
}

/*
 * Helper method to get/set the key associated with input "index"(a.k.a
 * array offset)
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
KeyType BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
KeyAt(int index) const {
  // replace with your own code
  return array[index].first;
}

template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
SetKeyAt(int index, const KeyType &key) {
  array[index].first = key;
}

/*
 * Helper method to find and return array index(or offset), so that its value
 * equals to input "value"
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
int BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
ValueIndex(const ValueType &value) const {
  for (int i = 0; i < GetSize(); ++i) {
    if (value == ValueAt(i)) {
      return i;
    }
  }
  return GetSize();
}

/*
 * Helper method to get the value associated with input "index"(a.k.a array
 * offset)
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
ValueType BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
ValueAt(int index) const {
  return array[index].second;
}

/*
 * Helper method to get the value associated with input "index"(a.k.a array
 * offset)
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
SetValueAt(int index, const ValueType &value) {
  array[index].second = value;
}

/*****************************************************************************
 * LOOKUP
 *****************************************************************************/
/*
 * Find and return the child pointer(page_id) which points to the child page
 * that contains input "key"
 * Start the search from the second key(the first key should always be invalid)
 *
 * //sequential scan
 * ValueType dir = array[0].second;
 * for (int i = 1; i < GetSize(); ++i) {
 *   if (comparator(key, array[i].first) < 0) {
 *     break;
 *   }
 *   dir = array[i].second;
 * }
 * return dir;
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
ValueType BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
Lookup(const KeyType &key, const KeyComparator &comparator) const {
  int end = GetSize() - 1;
        int target;
        for(int i=1;i<=end;i++)
        {
            if(comparator(array[i].first, key)==0)
                {
                    target=i;
                    break;
                }
            else if(comparator(array[i].first, key)>0)
                {   //can not find
                    target=i-1;
                    break;
                }
            else 
                continue;
        }
        if (target >= GetSize())
        {
            return array[GetSize() - 1].second;
        }
        if (comparator(array[target].first, key) == 0)
        {
            return array[target].second;
        }
        else
        {
            return array[target].second;
        }
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/
/*
 * Populate new root page with old_value + new_key & new_value
 * When the insertion cause overflow from leaf page all the way upto the root
 * page, you should create a new root page and populate its elements.
 * NOTE: This method is only called within InsertIntoParent()(b_plus_tree.cpp)
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
PopulateNewRoot(const ValueType &old_value, const KeyType &new_key,
                const ValueType &new_value) {
  // must be an empty page
  assert(GetSize() == 1);
  array[0].second = old_value;
  array[1] = {new_key, new_value};
  IncreaseSize(1);
}

/*
 * Insert new_key & new_value pair right after the pair with its value ==
 * old_value
 * @return:  new size after insertion
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
int BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
InsertNodeAfter(const ValueType &old_value, const KeyType &new_key,
                const ValueType &new_value) {
  for (int i = GetSize(); i > 0; --i) {
    if (array[i - 1].second == old_value) {
      array[i] = {new_key, new_value};
      IncreaseSize(1);
      break;
    }
    array[i] = array[i - 1];
  }
  return GetSize();
}

/*****************************************************************************
 * SPLIT
 *****************************************************************************/
/*
 * Remove half of key & value pairs from this page to "recipient" page
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
MoveHalfTo(BPlusTreeInternalPage *recipient,
           BufferPoolManager *buffer_pool_manager) {
  auto half = (GetSize() + 1)/2;
  recipient->CopyHalfFrom(array + GetSize() - half, half, buffer_pool_manager);

  // update parent page id of all children
  for (auto index = GetSize() - half; index < GetSize(); ++index) {
    auto *page = buffer_pool_manager->FetchPage(ValueAt(index));
    if (page == nullptr) {
      throw Exception(EXCEPTION_TYPE_INDEX,
                      "all page are pinned while CopyLastFrom");
    }
    auto child = reinterpret_cast<BPlusTreePage *>(page->GetData());
    child->SetParentPageId(recipient->GetPageId());

    assert(child->GetParentPageId() == recipient->GetPageId());
    buffer_pool_manager->UnpinPage(child->GetPageId(), true);
  }
  IncreaseSize(-1*half);
}

template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
CopyHalfFrom(MappingType *items, int size,
             BufferPoolManager *buffer_pool_manager) {
  // must be a new page
  assert(!IsLeafPage() && GetSize() == 1 && size > 0);
  for (int i = 0; i < size; ++i) {
    array[i] = *items++;
  }
  IncreaseSize(size - 1);
}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
/*
 * Remove the key & value pair in internal page according to input index(a.k.a
 * array offset)
 * NOTE: store key&value pair continuously after deletion
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
Remove(int index) {
  assert(0 <= index && index < GetSize());
  for (int i = index; i < GetSize() - 1; ++i) {
    array[i] = array[i + 1];
  }
  IncreaseSize(-1);
}

/*
 * Remove the only key & value pair in internal page and return the value
 * NOTE: only call this method within AdjustRoot()(in b_plus_tree.cpp)
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
ValueType BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
RemoveAndReturnOnlyChild() {
  IncreaseSize(-1);
  assert(GetSize() == 1);
  return ValueAt(0);
}

/*****************************************************************************
 * MERGE
 *****************************************************************************/
/*
 * Remove all of key & value pairs from this page to "recipient" page, then
 * update relevant key & value pair in its parent page.
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
MoveAllTo(BPlusTreeInternalPage *recipient, int index_in_parent,
          BufferPoolManager *buffer_pool_manager) {
  // first find parent
  auto *page = buffer_pool_manager->FetchPage(GetParentPageId());
  if (page == nullptr) {
    throw Exception(EXCEPTION_TYPE_INDEX,
                    "all page are pinned while MoveAllTo");
  }
  auto *parent = reinterpret_cast<BPlusTreeInternalPage *>(page->GetData());

  // the separation key from parent
  SetKeyAt(0, parent->KeyAt(index_in_parent));

  // assumption: current page is at the right hand of recipient
  assert(parent->ValueAt(index_in_parent) == GetPageId());

  // unpin parent page
  buffer_pool_manager->UnpinPage(parent->GetPageId(), true);

  recipient->CopyAllFrom(array, GetSize(), buffer_pool_manager);

  // update parent page id of all children
  for (auto index = 0; index < GetSize(); ++index) {
    auto *page = buffer_pool_manager->FetchPage(ValueAt(index));
    if (page == nullptr) {
      throw Exception(EXCEPTION_TYPE_INDEX,
                      "all page are pinned while CopyLastFrom");
    }
    auto child = reinterpret_cast<BPlusTreePage *>(page->GetData());
    child->SetParentPageId(recipient->GetPageId());

    assert(child->GetParentPageId() == recipient->GetPageId());
    buffer_pool_manager->UnpinPage(child->GetPageId(), true);
  }
}

template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
CopyAllFrom(MappingType *items, int size,
            BufferPoolManager *buffer_pool_manager) {
  assert(GetSize() + size <= GetMaxSize());
  int start = GetSize();
  for (int i = 0; i < size; ++i) {
    array[start + i] = *items++;
  }
  IncreaseSize(size);
}

/*****************************************************************************
 * REDISTRIBUTE
 *****************************************************************************/
/*
 * Remove the first key & value pair from this page to tail of "recipient"
 * page, then update relevant key & value pair in its parent page.
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
MoveFirstToEndOf(BPlusTreeInternalPage *recipient,
                 BufferPoolManager *buffer_pool_manager) {
  assert(GetSize() > 1);
  MappingType pair{KeyAt(1), ValueAt(0)};
  page_id_t child_page_id = ValueAt(0);
  SetValueAt(0, ValueAt(1));
  Remove(1);

  // delegate to helper function
  recipient->CopyLastFrom(pair, buffer_pool_manager);

  // update child parent page id
  auto *page = buffer_pool_manager->FetchPage(child_page_id);
  if (page == nullptr) {
    throw Exception(EXCEPTION_TYPE_INDEX,
                    "all page are pinned while CopyLastFrom");
  }
  auto child = reinterpret_cast<BPlusTreePage *>(page->GetData());
  child->SetParentPageId(recipient->GetPageId());

  assert(child->GetParentPageId() == recipient->GetPageId());
  buffer_pool_manager->UnpinPage(child->GetPageId(), true);
}

template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
CopyLastFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager) {
  assert(GetSize() + 1 <= GetMaxSize());

  auto *page = buffer_pool_manager->FetchPage(GetParentPageId());
  if (page == nullptr) {
    throw Exception(EXCEPTION_TYPE_INDEX,
                    "all page are pinned while CopyLastFrom");
  }
  auto parent = reinterpret_cast<BPlusTreeInternalPage *>(page->GetData());

  auto index = parent->ValueIndex(GetPageId());
  auto key = parent->KeyAt(index + 1);

  array[GetSize()] = {key, pair.second};
  IncreaseSize(1);
  parent->SetKeyAt(index + 1, pair.first);

  // unpin when we are done
  buffer_pool_manager->UnpinPage(parent->GetPageId(), true);
}

/*
 * Remove the last key & value pair from this page to head of "recipient"
 * page, then update relevant key & value pair in its parent page.
 */
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
MoveLastToFrontOf(BPlusTreeInternalPage *recipient, int parent_index,
                  BufferPoolManager *buffer_pool_manager) {
  assert(GetSize() > 1);
  IncreaseSize(-1);
  MappingType pair = array[GetSize()];
  page_id_t child_page_id = pair.second;

  // delegate
  recipient->CopyFirstFrom(pair, parent_index, buffer_pool_manager);

  // update child parent page id
  auto *page = buffer_pool_manager->FetchPage(child_page_id);
  if (page == nullptr) {
    throw Exception(EXCEPTION_TYPE_INDEX,
                    "all page are pinned while CopyLastFrom");
  }
  auto child = reinterpret_cast<BPlusTreePage *>(page->GetData());
  child->SetParentPageId(recipient->GetPageId());

  assert(child->GetParentPageId() == recipient->GetPageId());
  buffer_pool_manager->UnpinPage(child->GetPageId(), true);
}

template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
CopyFirstFrom(const MappingType &pair, int parent_index,
              BufferPoolManager *buffer_pool_manager) {
  assert(GetSize() + 1 < GetMaxSize());

  auto *page = buffer_pool_manager->FetchPage(GetParentPageId());
  if (page == nullptr) {
    throw Exception(EXCEPTION_TYPE_INDEX,
                    "all page are pinned while CopyFirstFrom");
  }
  auto parent = reinterpret_cast<BPlusTreeInternalPage *>(page->GetData());

  auto key = parent->KeyAt(parent_index);

  // set parent key to the last of current page
  parent->SetKeyAt(parent_index, pair.first);

  InsertNodeAfter(array[0].second, key, array[0].second);
  array[0].second = pair.second;

  buffer_pool_manager->UnpinPage(parent->GetPageId(), true);
}

/*****************************************************************************
 * DEBUG
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
void BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
QueueUpChildren(std::queue<BPlusTreePage *> *queue,
                BufferPoolManager *buffer_pool_manager) {
  for (int i = 0; i < GetSize(); i++) {
    auto *page = buffer_pool_manager->FetchPage(array[i].second);
    if (page == nullptr) {
      throw Exception(EXCEPTION_TYPE_INDEX,
                      "all page are pinned while printing");
    }
    auto *child = reinterpret_cast<BPlusTreePage *>(page->GetData());
    assert(child->GetParentPageId() == GetPageId());
    queue->push(child);
  }
}

template <typename KeyType, typename ValueType, typename KeyComparator>
std::string BPlusTreeInternalPage<KeyType, ValueType, KeyComparator>::
ToString(bool verbose) const {
  if (GetSize() == 0) {
    return "";
  }
  std::ostringstream os;
  if (verbose) {
    os << "[" << GetPageId() << "-"
       << GetParentPageId() << "]";
  }
  int entry = verbose ? 0 : 1;
  int end = GetSize();
  bool first = true;
  while (entry < end) {
    if (first) {
      first = false;
    } else {
      os << " ";
    }
    os << std::dec << " " << array[entry].first.ToString();
    if (verbose) {
      os << "(" << array[entry].second << ")";
    }
    ++entry;
    os << " ";
  }
  return os.str();
}

// valuetype for internalNode should be page id_t
template class BPlusTreeInternalPage<GenericKey<4>, page_id_t, GenericComparator<4>>;
template class BPlusTreeInternalPage<GenericKey<8>, page_id_t, GenericComparator<8>>;
template class BPlusTreeInternalPage<GenericKey<16>, page_id_t, GenericComparator<16>>;
template class BPlusTreeInternalPage<GenericKey<32>, page_id_t, GenericComparator<32>>;
template class BPlusTreeInternalPage<GenericKey<64>, page_id_t, GenericComparator<64>>;

} // namespace cmudb
