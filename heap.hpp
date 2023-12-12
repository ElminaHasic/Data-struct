#pragma once
#include <iostream>

template <typename T>
class Heap {
  public:
  Heap();
  Heap(const Heap&);
  Heap(Heap&&);
  Heap& operator=(const Heap&);
  Heap& operator=(Heap&&);
  ~Heap();

  template <typename F>
  Heap& insert(F&&);

  T& min();
  const T& min() const;
  T& max();
  const T& max() const;

  Heap& removeMax();
  Heap& removeMin();

  friend std::ostream& operator<<(std::ostream& os, const Heap& heap) {
    os << "[ ";
    heap.InOrder(0);
    os << " ]" << std::endl;
  }

  private:
  void InOrder(size_t) const;
  void realloc();
  void balanceUp();
  void balance_down(size_t);
  size_t capacity_;
  size_t size_;
  T* arr_;
};

template <typename T>
Heap<T>::Heap() : capacity_{10}, size_{0}, arr_{new T[capacity_]} {}

template <typename T>
Heap<T>::Heap(const Heap& other)
    : capacity_{other.capacity_}, size_{other.size_}, arr_{new T[capacity_]} {
  std::copy(other.arr_, other.arr_ + size_, arr_);
}

template <typename T>
Heap<T>::Heap(Heap&& other)
    : capacity_{other.capacity_}, size_{other.size_}, arr_{other.arr_} {
  other.arr_ = nullptr;
  other.capacity_ = other.size_ = 0;
}

template <typename T>
Heap<T>& Heap<T>::operator=(const Heap& other) {
  if (this != &other) {
    delete[] arr_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    arr_ = new T[capacity_];
    std::copy(other.arr_, other.arr_ + size_, arr_);
  }
  return *this;
}

template <typename T>
Heap<T>& Heap<T>::operator=(Heap&& other) {
  delete[] arr_;
  capacity_ = other.capacity_;
  size_ = other.size_;
  arr_ = other.arr_;
  other.arr_ = nullptr;
  other.capacity_ = other.size_ = 0;

  return *this;
}

template <typename T>
Heap<T>::~Heap() {
  delete[] arr_;
  capacity_ = size_ = 0;
}

template <typename T>
void Heap<T>::realloc() {
  auto temp = arr_;
  capacity_ *= 1.5;
  arr_ = new T[capacity_];
  std::copy(temp, temp + size_, arr_);
  delete[] temp;
}

template <typename T>
void Heap<T>::balanceUp() {
  auto current_index = size_ - 1;
  while (current_index > 0) {
    size_t parent_index = (current_index - 1) / 2;
    if (arr_[current_index] > arr_[parent_index]) {
      std::swap(arr_[current_index], arr_[parent_index]);
      current_index = (current_index - 1) / 2;
    } else
      break;
  }
}

template <typename T>
void Heap<T>::balance_down(size_t index) {
  if (index < size_) {
    size_t left = index * 2 + 1;
    size_t right = index * 2 + 2;
    if (left < size_ && right < size_ &&
        (arr_[index] < arr_[right] || arr_[index] < arr_[left])) {
      size_t max_index = arr_[left] < arr_[right] ? right : left;
      std::swap(arr_[index], arr_[max_index]);
      balance_down(max_index);
    } else if (left < size_ && right >= size_ && arr_[index] < arr_[left]) {
      std::swap(arr_[index], arr_[left]);
      balance_down(left);
    } else if (left >= size_ && right < size_ && arr_[index] < arr_[right]) {
      std::swap(arr_[index], arr_[right]);
      balance_down(right);
    }
  }
}

template <typename T>
void Heap<T>::InOrder(size_t index) const {
  if (index >= size_) return;
  InOrder(2 * index + 1);
  std::cout << arr_[index] << " ";
  InOrder(2 * index + 2);
}

template <typename T>
T& Heap<T>::min() {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};
  if (size_ == 1) return arr_[0];
  size_t min_index = 0;
  for (auto i = 1; i < size_; ++i) {
    if (arr_[i] < arr_[min_index]) min_index = i;
  }
  return arr_[min_index];
}

template <typename T>
const T& Heap<T>::min() const {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};
  if (size_ == 1) return arr_[0];
  size_t min_index = 0;
  for (auto i = 1; i < size_; ++i) {
    if (arr_[i] < arr_[min_index]) min_index = i;
  }
  return arr_[min_index];
}

template <typename T>
T& Heap<T>::max() {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};
  return arr_[0];
}

template <typename T>
const T& Heap<T>::max() const {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};
  return arr_[0];
}

template <typename T>
template <typename F>
Heap<T>& Heap<T>::insert(F&& element) {
  if (size_ == capacity_) realloc();
  arr_[size_++] = std::forward<F>(element);
  balanceUp();
  return *this;
}

template <typename T>
Heap<T>& Heap<T>::removeMax() {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};

  if (size_ == 1) {
    size_ = 0;
  } else {
    std::swap(arr_[0], arr_[--size_]);
    balance_down(0);
    return *this;
  }
}

template <typename T>
Heap<T>& Heap<T>::removeMin() {
  if (size_ == 0) throw std::out_of_range{"The heap is empty"};
  if (size_ == 1)
    size_ = 0;

  else {
    auto min_index = 1;
    for (auto i = 2; i < size_; ++i) {
      if (arr_[i] < arr_[min_index]) min_index = i;
    }
    if (min_index == size_ - 1)
      --size_;
    else {
      std::swap(arr_[min_index], arr_[size_ - 1]);
      size_t parent_index = (min_index - 1) / 2;
      auto current_index = min_index;
      while (arr_[current_index] > arr_[parent_index]) {
        std::swap(arr_[current_index], arr_[parent_index]);
        current_index = parent_index;
        parent_index = (current_index - 1) / 2;
      }
      --size_;
    }
  }
  return *this;
}

