#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

template <typename KeyType, typename ValueType>
class UnorderedMap {
  using bucket_type = std::list<std::pair<KeyType, ValueType>>;

  public:
  UnorderedMap() { bucket_array_.resize(capacity_); }
  class Iterator;
  void emplace(const KeyType&, const ValueType&);
  ValueType& operator[](const KeyType& data);
  bool erase(const KeyType&);
  Iterator find(const KeyType&);
  Iterator begin();
  Iterator end();

  protected:
  typename bucket_type::iterator getLastBucket(
      typename std::vector<bucket_type>::iterator& arr);
  typename bucket_type::iterator getFirstBucket(
      typename std::vector<bucket_type>::iterator& arr);

  private:
  size_t hash_function(const std::string& key) {
    size_t h = 0;
    for (auto i = 0u; i < key.length(); ++i) {
      h = (h << 4) | (h >> 28);
      h += h;
    }
    return h % capacity_;
  }

  size_t hash_function(double key) { return (int)key % capacity_; }

  const size_t capacity_{101};
  size_t size_{0};
  std::vector<bucket_type> bucket_array_;
};

template <typename KeyType, typename ValueType>
class UnorderedMap<KeyType, ValueType>::Iterator {
  public:
  friend class UnorderedMap;
  Iterator() = default;
  ~Iterator() = default;
  Iterator(const std::vector<bucket_type>& bucket_arr,
           typename bucket_type::iterator it,
           typename std::vector<bucket_type>::iterator arr =
               typename std::vector<bucket_type>::iterator())
      : bucket_arr_{&bucket_arr}, it_{it}, arr_{arr} {}

  bool operator!=(const Iterator& other) const {
    if (bucket_arr_ != other.bucket_arr_) return true;
    if (this->it_ != other.it_ || arr_ != arr_) return true;
    return false;
  }

  bool operator==(const Iterator& other) const {
    if (bucket_arr_ != other.bucket_arr_) return false;
    return (this->it_ == other.it_ && arr_ == other.arr_);
  }

  std::pair<KeyType, ValueType>& operator*() { return *it_; }
  std::pair<KeyType, ValueType>* operator->() { return &(*it_); }

  Iterator& operator++();
  Iterator operator++(int);
  Iterator& operator--();
  Iterator operator--(int);

  protected:
  bool last_(const Iterator& other);
  bool first(const Iterator& other);

  private:
  const std::vector<bucket_type>* bucket_arr_;
  typename bucket_type::iterator it_;
  typename std::vector<bucket_type>::iterator arr_;
};

template <typename KeyType, typename ValueType>
ValueType& UnorderedMap<KeyType, ValueType>::operator[](const KeyType& key) {
  auto it = find(key);
  if (it == end()) emplace(key, ValueType{});
  it = find(key);
  return it.it_->second;
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::erase(const KeyType& key) {
  auto index = hash_function(key);
  auto& current_bucket = bucket_array_[index];
  auto it = std::find_if(current_bucket.begin(), current_bucket.end(),
                         [&](const auto& el) { return el.first == key; });

  if (it != current_bucket.end()) {
    current_bucket.erase(it);
    --size_;
    return true;
  }
  return false;
}

template <typename KeyType, typename ValueType>
void UnorderedMap<KeyType, ValueType>::emplace(const KeyType& key,
                                               const ValueType& value) {
  auto index = hash_function(key);
  auto& current_bucket = bucket_array_[index];
  auto it = std::find_if(current_bucket.begin(), current_bucket.end(),
                         [&](const auto& el) { return el.first == key; });

  if (it == current_bucket.end()) {
    current_bucket.push_back(std::make_pair(key, value));
    ++size_;
  } else {
    throw std::invalid_argument{"The key exists"};
  }
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator
UnorderedMap<KeyType, ValueType>::find(const KeyType& key) {
  auto index = hash_function(key);
  auto& current_bucket = bucket_array_[index];
  auto it = std::find_if(current_bucket.begin(), current_bucket.end(),
                         [&](const auto& el) { return el.first == key; });
  if (it != current_bucket.end()) return Iterator(bucket_array_, it);
  return end();
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator
UnorderedMap<KeyType, ValueType>::begin() {
  typename std::vector<bucket_type>::iterator arr;
  return Iterator(bucket_array_, getFirstBucket(arr), arr);
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator
UnorderedMap<KeyType, ValueType>::end() {
  typename std::vector<bucket_type>::iterator arr;
  return Iterator(bucket_array_, getLastBucket(arr), arr);
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator&
UnorderedMap<KeyType, ValueType>::Iterator::operator--() {
  if (!(first(*this))) {
    --it_;

    if (it_ == arr_->begin()) {
      if (!(first(*this))) {
        --arr_;
        while (arr_->empty()) {
          if (arr_ == bucket_arr_->begin()) break;
          --arr_;
        }
        it_ = --(arr_->end());
      }
    }
  }
  return *this;
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator
UnorderedMap<KeyType, ValueType>::Iterator::operator--(int) {
  auto tmp = *this;
  this->operator--();
  return tmp;
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator&
UnorderedMap<KeyType, ValueType>::Iterator::operator++() {
  ++it_;
  if (it_ == arr_->end()) {
    if (last_(*this)) return *this;
    if (arr_ != bucket_arr_->end()) {
      ++arr_;
      while (arr_ != bucket_arr_->end()) {
        if (!(arr_->empty())) break;
        ++arr_;
      }
    }
    it_ = arr_->begin();
  }

  return *this;
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Iterator
UnorderedMap<KeyType, ValueType>::Iterator::operator++(int) {
  auto tmp = *this;
  this->operator++();
  return tmp;
}

template <typename KeyType, typename ValueType>
typename std::list<std::pair<KeyType, ValueType>>::iterator
UnorderedMap<KeyType, ValueType>::getLastBucket(
    typename std::vector<bucket_type>::iterator& arr) {
  auto endofbucketarray = bucket_array_.end();
  --endofbucketarray;
  while (endofbucketarray->empty()) {
    if (endofbucketarray == bucket_array_.begin()) break;
    --endofbucketarray;
  }

  arr = endofbucketarray;
  return endofbucketarray->end();
}

template <typename KeyType, typename ValueType>
typename std::list<std::pair<KeyType, ValueType>>::iterator
UnorderedMap<KeyType, ValueType>::getFirstBucket(
    typename std::vector<bucket_type>::iterator& arr) {
  auto beginOfbarr = bucket_array_.begin();
  while (beginOfbarr != bucket_array_.end()) {
    if (!(beginOfbarr->empty())) break;
    ++beginOfbarr;
  }

  arr = beginOfbarr;
  if (beginOfbarr != bucket_array_.end()) {
    return beginOfbarr->begin();
  }
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::Iterator::last_(const Iterator& other) {
  auto endofbucketarray = bucket_arr_->end();
  --endofbucketarray;
  while (endofbucketarray->empty()) {
    if (endofbucketarray == bucket_arr_->begin()) break;
    --endofbucketarray;
  }

  auto it = endofbucketarray->end();
  if (other.arr_ == endofbucketarray && it == other.it_) return true;
  return false;
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::Iterator::first(const Iterator& other) {
  auto beginofarray = bucket_arr_->begin();
  while (beginofarray != bucket_arr_->end()) {
    if (!(beginofarray->empty())) break;
    ++beginofarray;
  }

  if (other.it_ == beginofarray->begin()) return true;
  return false;
}

