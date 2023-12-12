#pragma once
#include <iostream>
#include "sortedsinglylinkedlist.hpp"
namespace Types {
template <typename T>
SortedSinglyLinkedList<T>::~SortedSinglyLinkedList() {
  dealoc();
}
template <typename T>
void SortedSinglyLinkedList<T>::dealoc() {
  while (head_ != nullptr) {
    auto tmp = head_->next_;
    delete head_;
    head_ = tmp;
  }
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

template <typename T>
SortedSinglyLinkedList<T>::SortedSinglyLinkedList(
    const SortedSinglyLinkedList& other) {
  copyList(other);
}
template <typename T>
void SortedSinglyLinkedList<T>::copyList(const SortedSinglyLinkedList& lista) {
  auto tmp = lista.head_;
  while (tmp != nullptr) {
    add(tmp->data_);
    tmp = tmp->next_;
  }
}

template <typename T>
SortedSinglyLinkedList<T>& SortedSinglyLinkedList<T>::add(const T& element) {
  Node<T>* newnode = new Node<T>(element);

  if (head_ == nullptr) { 
    head_ = newnode;
    tail_ = newnode;
  } else {
    auto tmp = head_;
    if (head_->data_ >= element) {  
      newnode->next_ = head_;
      head_ = newnode;
    } else if (tail_->data_ <= element) {  
      newnode->next_ = nullptr;
      tail_->next_ = newnode;
      tail_ = newnode;
    }

    else {
      while (tmp->next_->data_<element) { 
        tmp = tmp->next_;
      }
      newnode->next_ = tmp->next_;
      tmp->next_ = newnode;
    }
  }
  ++size_;
  return *this;
}

template <typename T>
SortedSinglyLinkedList<T>::SortedSinglyLinkedList(
    SortedSinglyLinkedList&& other) {
  moveList(other);
}

template <typename T>
void SortedSinglyLinkedList<T>::moveList(SortedSinglyLinkedList&& otherlist) {
  head_ = otherlist.head_;
  tail_ = otherlist.tail_;
  size_ = otherlist.size_;
  otherlist.head_ = nullptr;
  otherlist.tail_ = nullptr;
  otherlist.size_ = 0;
}

template <typename T>
SortedSinglyLinkedList<T>& SortedSinglyLinkedList<T>::operator=(
    const SortedSinglyLinkedList& lista) {
  if (this == &lista) return *this;
  dealoc();
  copyList(lista);
  return *this;
}

template <typename T>
SortedSinglyLinkedList<T>& SortedSinglyLinkedList<T>::operator=(
    SortedSinglyLinkedList&& lista) {
  dealoc();
  moveList(lista);
  return *this;
}
template <typename T>
typename SortedSinglyLinkedList<T>::Iterator SortedSinglyLinkedList<T>::end()
    const {
  return {tail_->next_};
}
template <typename T>
typename SortedSinglyLinkedList<T>::Iterator SortedSinglyLinkedList<T>::begin()
    const {
  return {head_};
}

template <typename T>
bool SortedSinglyLinkedList<T>::Iterator::operator==(const Iterator& it) const {
  return ptr_ == it.ptr_;
}

template <typename T>
bool SortedSinglyLinkedList<T>::Iterator::operator!=(const Iterator& it) const {
  return ptr_ != it.ptr_;
}
template <typename T>
typename SortedSinglyLinkedList<T>::Iterator&
SortedSinglyLinkedList<T>::Iterator::operator++() {
  ptr_ = ptr_->next_;
  return *this;
}
template <typename T>
void SortedSinglyLinkedList<T>::remove(
    const Iterator& it) {
  if (it == end()) throw std::out_of_range{"Greska"};
  if (it.ptr_ == head_) {
    auto temp = head_->next_;
    delete head_;
    head_ = temp;
  } else {
    auto tmp = head_;
    while (tmp->next_ != it.ptr_) {
      tmp = tmp->next_;
    }
    tmp->next_ = it.ptr_->next_;
    delete it.ptr_;
  }

  --size_;
  
}
template<typename T>
bool SortedSinglyLinkedList<T>::empty()const{
  return head_==nullptr;}

template<typename T>
typename SortedSinglyLinkedList<T>::Iterator SortedSinglyLinkedList<T>::find(std::function<bool(T)> predicate){
    auto temp=head_;
    while(temp!= nullptr)
    {
      if(predicate(temp->data_))
      {
        return Iterator(temp);
      }
      temp=temp->next_;
    }
    return end();
}

template<typename T>
 T SortedSinglyLinkedList<T>::getdata(Iterator it )const{
   return it.ptr_->data_;
 }


}  // namespace Types
