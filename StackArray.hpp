#pragma once
#include <stddef.h>
#include <algorithm>
#include <stdexcept>
#include "Stack.hpp"

//default konstruktor - O(capacitiy)
template <typename T>
StackArray<T>::StackArray() : capacity_{10}, size_{0}, arr_{new T[capacity_]} {}

//copy konstruktor linearna  vremenska slozenost O(n)
template <typename T>
StackArray<T>::StackArray(const StackArray& other)
    : capacity_{other.capacity_}, size_{other.size_}, arr_{new T[capacity_]} {
  std::copy(other.arr_, other.arr_ + size_, arr_);
}

//move konstruktor konstantna vremenska slozenost O(1)
template <typename T>
StackArray<T>::StackArray(StackArray&& other)
    : capacity_{std::move(other.capacity_)},
      size_{std::move(other.size_)},
      arr_{other.arr_} {
  other.arr_ = nullptr;
}

//operator=  linearna  vremenska slozenost O(n)
template <typename T>

StackArray<T>& StackArray<T>::operator=(const StackArray& other) {
  if (this != &other) {
    delete[] arr_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    arr_ = new T[capacity_];
    std::copy(other.arr_, other.arr_ + size_, arr_);
  }
  return *this;
}

//operator= konstantna   vremenska slozenost O(1)
template <typename T>
StackArray<T>& StackArray<T>::operator=(StackArray&& other) {
  delete[] arr_;
  capacity_ = std::move(other.capacity_);
  size_ = std::move(other.size_);
  arr_ = other.arr_;
  other.arr_ = nullptr;

  return *this;
}

//destruktor linearna  vremenska slozenost O(n)
template <typename T>
StackArray<T>::~StackArray() {
  delete[] arr_;
}
//metod pop konstantna vremenska slozenost O(1)
template <typename T>
T StackArray<T>::pop() {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  T value = arr_[--size_];
  return value;
}

//metod top konstantna vremenska slozenost O(1)
template <typename T>
T& StackArray<T>::top() {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  return arr_[size_ - 1];
}

template <typename T>

const T& StackArray<T>::top() const {
  if (empty()) throw std::out_of_range{"Stack is empty"};
  return arr_[size_ - 1];
}

//metod push konstantna vremenska slozenost O(1)
template <typename T>
template <typename U>
StackArray<T>& StackArray<T>::push(U&& item) {
  if (full()) realoc();
  arr_[size_++] = std::forward<U>(item);
  return *this;
}

//metod size konstantna vremenska slozenost O(1)
template <typename T>
size_t StackArray<T>::size() const {
  return size_;
}

//metod capacity konstantna vremenska slozenost O(1)
template <typename T>
size_t StackArray<T>::capacity() const {
  return capacity_;
}

//metod empty konstantna vremenska slozenost O(1)
template <typename T>
bool StackArray<T>::empty() const {
  return size_ == 0;
}

//metod full konstantna vremenska slozenost O(1)
template <typename T>
bool StackArray<T>::full() const {
  return size_ == capacity_;
}

//metod realoc linearna vremenska slozenost O(n)
template <typename T>
void StackArray<T>::realoc() {
  auto tmp = arr_;
  capacity_ = 2 * capacity_;
  arr_ = new T[capacity_];
  std::copy(tmp, tmp + size_, arr_);
  delete[] tmp;
}
//operator== linearna vremenska slozenost O(n)
template<typename T>
bool StackArray<T>::operator==(const StackArray& other)const
{
  if( size_ != other.size_)
    return false;
  StackArray stacktmp1=*this;
  StackArray stacktmp2=other;
  while( !(stacktmp1.empty()))
  {
    if( stacktmp1.top() != stacktmp2.top())
      return false;
    stacktmp1.pop();
    stacktmp2.pop();
  }
  return true;
}


//operator!= linearna vremenska slozenost O(n)
template<typename T>
bool StackArray<T>::operator!=(const StackArray& other)const{
  return !(*this == other);
}

//Zadnji element sa stack-a moguce je dohvatiti ukoliko koristimo dodatni stack 
//za privremeno cuvanje elemenata koji se nalaze u stack-u prije zadnjeg elementa.
//Sve operacije nad postojucim stack-om moraju imati konstantnu vremensku slozenost O(1),
//zbog cega ovakva implementacija metoda za dohvatanje zadnjeg elementa iz stack-a 
//nije preporucljiva jer ima linearnu vremensku slozenost.
template<typename T>
T StackArray<T>::getend(){
  if(empty())
    throw std::out_of_range{"Stack is empty"};
  if(size_==1)
  {
    return top();
  }
  else{

  StackArray tmp;
  while( size_>1)
  {
    tmp.push(this->top());
    this->pop();
        
  }
  T endvalue=this->top();
  while(!(tmp.empty()))
  {
    push(tmp.top());
    tmp.pop();
  }
  return endvalue;
  }
  }

