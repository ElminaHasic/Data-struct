#pragma once
#include <iostream>
#include <iterator>
#include <functional>
namespace Types {
   template <typename T>
struct Node {
   T data_;
  Node* next_;
  Node(const T& element) : data_{element}, next_{nullptr} {}
};

template <typename T>
class SortedSinglyLinkedList {
  public:
  class Iterator;
  SortedSinglyLinkedList() = default;
  SortedSinglyLinkedList(const SortedSinglyLinkedList&);
  SortedSinglyLinkedList(SortedSinglyLinkedList&&);
  SortedSinglyLinkedList& operator=(const SortedSinglyLinkedList&);
  SortedSinglyLinkedList& operator=(SortedSinglyLinkedList&&);
  ~SortedSinglyLinkedList();
  SortedSinglyLinkedList& add(const T&);
  void remove(const Iterator&);
  Iterator end()const;
  Iterator begin()const;
   Iterator find(std::function<bool(T)> );
  T getdata(Iterator )const;
  bool empty() const;
  friend std::ostream& operator<<(std::ostream& o,
                                  const SortedSinglyLinkedList& lista) {
    auto tmp = lista.head_;
    while (tmp != nullptr) {
      o << tmp->data_;
      tmp = tmp->next_;
    }
    o << std::endl;
    return o;
  }

  private:
  void copyList(const SortedSinglyLinkedList&);
  void moveList(SortedSinglyLinkedList&&);
  void dealoc();
  Node<T>* head_{nullptr};
  Node<T>* tail_{nullptr};
  size_t size_{0};
};

template <typename T>
class SortedSinglyLinkedList<T>::Iterator : public std::forward_iterator_tag {
  public:
    friend SortedSinglyLinkedList<T>;
  Iterator() = default;
  Iterator(const Iterator& it) : ptr_{it.ptr_} {}
  Iterator(Iterator&& it) : ptr_{it.ptr_} {it.ptr_ = nullptr;}
  Iterator& operator=(const Iterator& it) {
    if (this != &it) {
      ptr_ = it.ptr_;
    }
    return *this;
  }
  Iterator& operator=(Iterator&& it) {
    ptr_ = it.ptr_;
    it.ptr_ = nullptr;

    return *this;
  }
   ~Iterator(){ptr_=nullptr;}
   Iterator(Node<T>* ptr):ptr_{ptr}{}
   bool operator==(const Iterator&)const;
   bool operator!=(const Iterator&)const;
   Iterator& operator++();
  private:
  Node<T>* ptr_{nullptr};
};


}  // namespace Types
