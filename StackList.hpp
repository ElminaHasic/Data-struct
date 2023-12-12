#pragma once
#include "stacklist.hpp"

// default konstruktor konstantna vremenska slozenost O(1)
template <typename T>
StackList<T>::StackList() : head_{nullptr}, size_{0} {}

// copy konstruktor linearna  vremenska slozenost O(n)
template <typename T>
StackList<T>::StackList(const StackList& other) {
  copystack(other);
}
// move konstruktor konstantna vremenska slozenost O(1)
template <typename T>
StackList<T>::StackList(StackList&& other) {
  movestack(other);
}

// metod clear() linearna  vremenska slozenost O(n)
template <typename T>
void StackList<T>::clear() {
  auto temp = head_;
  while (temp != nullptr) {
    head_ = head_->next_;
    delete temp;
    temp = head_;
  }
  size_ = 0;
}

// metod copystack() linearna  vremenska slozenost O(n)
template <typename T>
void StackList<T>::copystack(const StackList& other) {
  auto temp = other.head_;
  head_ = new Node{temp->data};
  auto tmp2 = head_;
  temp = temp->next_;
  while (temp != nullptr) {
    auto newnode = new Node{temp->data};
    tmp2->next_ = newnode;
    tmp2 = tmp2->next_;
    temp = temp->next_;
  }
  size_ = other.size_;
}

// operator= linearna  vremenska slozenost O(n)
template <typename T>
StackList<T>& StackList<T>::operator=(const StackList& other) {
  if (this == &other) return *this;
  clear();
  copystack(other);
  return *this;
}

// metod movestack() konstantna vremenska slozenost O(1)
template <typename T>
void StackList<T>::movestack(StackList& other) {
  head_ = std::move(other.head_);
  size_ = std::move(other.size_);
  other.head_ = nullptr;
  other.size_ = 0;
}

template <typename T>
StackList<T>& StackList<T>::operator=(StackList&& other) {
  clear();
  movestack(other);
  return *this;
}

// Destruktor linearna vremenska slozenost O(n)
template <typename T>
StackList<T>::~StackList() {
  clear();
}

// metod push() konstantna vremenska slozenost O(1)
template <typename T>
template <typename U>
StackList<T>& StackList<T>::push(U&& item) {
  Node* newnode = new Node(std::forward<U>(item));
  ++size_;
  if (empty()) {
    head_ = newnode;
  } else {
    newnode->next_ = head_;
    head_ = newnode;
  }
  return *this;
}

// metod pop() konstantna vremenska slozenost O(1)
template <typename T>
T StackList<T>::pop() {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  auto temp = head_;
  head_ = head_->next_;
  T value = std::move(temp->data);
  delete temp;
  --size_;
  return value;
}

// metod empty() konstantna vremenska slozenost O(1)
template <typename T>
bool StackList<T>::empty() const {
  return size_ == 0;
}

// metod size() konstantna vremenska slozenost O(1)
template <typename T>
size_t StackList<T>::size() const {
  return size_;
}

// metod top() konstantna vremenska slozenost O(1)
template <typename T>
T& StackList<T>::top() {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  return head_->data;
}
template <typename T>
const T& StackList<T>::top() const {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  return head_->data;
}

// operator== linearna vremenska slozenost O(n)
template <typename T>
bool StackList<T>::operator==(const StackList& other) const {
  if (size_ != other.size_) return false;
  StackList tmp1 = *this;
  StackList tmp2 = other;
  while (!(tmp1.empty())) {
    if (tmp1.top() != tmp2.top()) return false;
    tmp1.pop();
    tmp2.pop();
  }
  return true;
}

// operator!= linearna vremenska slozenost O(n)
template <typename T>
bool StackList<T>::operator!=(const StackList& other) const {
  return !(*this == other);
}

