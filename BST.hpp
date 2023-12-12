#pragma once
#include <iostream>
#include <queue>
#include <vector>

#include <iostream>
namespace Types {
template <typename T>
struct Node {
  Node* left{nullptr};
  Node* right{nullptr};
  T data;
  Node(T& element) : data{element} {}
  Node(T&& element) : data{element} {}
};

template <typename T>
class BST {
  public:
  BST() = default;
  BST(const BST& bst) { copy(root_, bst.root_); }
  BST(BST&& bst) : root_{bst.root_} { bst.root_ = nullptr; }
  BST& operator=(BST&& bst) {
    destroy(root_);
    root_ = bst.root_;
    bst.root_ = nullptr;
    return *this;
  }
  BST& operator=(const BST& bst) {
    if (&bst != this) {
      destroy(root_);
      copy(root_, bst.root_);
    }
    return *this;
  }
  ~BST() { destroy(root_); }
  int sum() { return sum(root_); }

  int sum(Node<T>* cvor) {
    if (cvor == nullptr) return 0;
    int sumica = 0;
    if (cvor->data % 2 == 0) {
      sumica = cvor->data;
    }
    sumica = sumica + sum(cvor->left);
    sumica = sumica + sum(cvor->right);
    return sumica;
  }

  template <typename U>
  BST& add(U&& element) {
    auto* newNode = new Node<T>(std::forward<U>(element));
    if (!root_) {
      root_ = newNode;
    } else {
      auto* current = root_;
      auto* parent = current;
      while (current) {
        parent = current;
        if (current->data > element) {
          current = current->left;
        } else if (current->data < element) {
          current = current->right;
        } else {
          return *this;
        }
      }
      if (parent->data > element) {
        parent->left = newNode;
      } else {
        parent->right = newNode;
      }
    }
    return *this;
  }

  void preorder() const {
    std::cout << "[";
    preorder(root_);
    std::cout << "]" << std::endl;
  }
  void inorder() const {
    std::cout << "[";
    inorder(root_);
    std::cout << "]" << std::endl;
  }
  void postorder() const {
    std::cout << "[";
    postorder(root_);
    std::cout << "]" << std::endl;
  }
  Node<T>* get_previous(Node<T>* cvor, const T& kljuc);

  Node<T>* search(const T& element) { return (search(root_, element)); }
  // TODO: Implementirati koristenjem petlje
  Node<T>* loopSearch(const T& element);
  // TODO: Implementirati metod koji uklanja cvor koji ima vrijednost element
  bool remove(const T& element);
  // TODO: Metod koji vrsi zamjenu dva cvora u stablu
  void swapNodes(Node<T>* first, Node<T>* second);

  // Metod koji mijenja vrijednost node na element
  void replace(Node<T>* node, const T& element);

  private:
  void cvor_nema_djece(Node<T>* cvor, const T& kljuc);
  void cvor_ima_1_dijete(Node<T>* cvor, const T& kljuc);
  Node<T>* get_NSP(Node<T>* cvor);

  Node<T>* search(Node<T>* node, const T& element) {
    if (!node) return nullptr;
    if (node->data == element) return node;
    if (node->data > element) {
      return search(node->left, element);
    } else {
      return search(node->right, element);
    }
  }
  void preorder(const Node<T>* const current) const {
    if (!current) return;
    std::cout << current->data << ", ";
    preorder(current->left);
    preorder(current->right);
  }
  void inorder(const Node<T>* const current) const {
    if (!current) return;
    inorder(current->left);
    std::cout << current->data << ", ";
    inorder(current->right);
  }
  void postorder(const Node<T>* const current) const {
    if (!current) return;
    postorder(current->left);
    postorder(current->right);
    std::cout << current->data << ", ";
  }

  void destroy(Node<T>* node) {
    if (node == nullptr) {
      return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
    node = nullptr;
  }

  void copy(Node<T>* dest, Node<T>* source) {
    if (source == nullptr) return;
    dest = new Node<T>(source->data);
    copy(dest->left, source->left);
    copy(dest->right, source->right);
  }

  public:
  Node<T>* root_{nullptr};
};

template <typename KeyType>
bool BST<KeyType>::remove(const KeyType& kljuc) {
  auto cvor = search(root_, kljuc);
  if (cvor == nullptr)
    return false;
  else {
    if (cvor->left == nullptr && cvor->right == nullptr) {
      cvor_nema_djece(cvor, kljuc);
      return true;
    } else if ((cvor->left == nullptr && cvor->right != nullptr) ||
               (cvor->left != nullptr && cvor->right == nullptr)) {
      cvor_ima_1_dijete(cvor, kljuc);
      return true;
    } else {
      auto* NSP = get_NSP(cvor);
      std::swap(NSP->data, cvor->data);

      if (NSP->left == nullptr && NSP->right == nullptr) {
        auto prev = get_previous(NSP, kljuc);
        cvor_nema_djece(NSP, kljuc);
        return true;
      } else {
        cvor_ima_1_dijete(NSP, kljuc);

        return true;
      }
    }
  }
}

template <typename T>
void BST<T>::cvor_nema_djece(Node<T>* cvor, const T& kljuc) {
  if (cvor == root_) {
    delete cvor;
    root_ = nullptr;
  } else {
    auto prev = get_previous(cvor, kljuc);
    if (prev->left == cvor) {
      delete cvor;
      prev->left = nullptr;
    } else {
      delete cvor;
      prev->right = nullptr;
    }
  }
}

template <typename KeyType>
void BST<KeyType>::cvor_ima_1_dijete(Node<KeyType>* cvor,
                                     const KeyType& kljuc) {
  if (cvor == root_) {
    if (root_->left != nullptr) {
      auto tmp = root_;
      root_ = root_->left;
      delete tmp;
      return;
    } else {
      auto tmp = root_;
      root_ = root_->right;
      delete tmp;
      return;
    }

  } else {
    auto prev = get_previous(cvor, kljuc);
    if (prev->left == cvor) {
      if (cvor->left != nullptr) {
        prev->left = cvor->left;
        cvor->left = nullptr;
        delete cvor;
        return;
      } else {
        prev->left = cvor->right;
        cvor->right = nullptr;
        delete cvor;
        return;
      }
    }

    else {
      if (cvor->left != nullptr) {
        prev->right = cvor->left;

        cvor->left = nullptr;
        delete cvor;
        return;
      } else {
        prev->right = cvor->right;
        cvor->right = nullptr;
        delete cvor;
        return;
      }
    }
  }
}

template <typename KeyType>
Node<KeyType>* BST<KeyType>::get_NSP(Node<KeyType>* cvor) {
  auto current = cvor->right;
  while (current->left != nullptr) {
    current = current->left;
  }
  return current;
}

template <typename KeyType>
Node<KeyType>* BST<KeyType>::get_previous(Node<KeyType>* cvor,
                                          const KeyType& kljuc) {
  auto* previous = root_;
  while (previous->left != cvor && previous->right != cvor) {
    if (previous->data > kljuc)
      previous = previous->left;
    else
      previous = previous->right;
  }
  return previous;
}

template <typename T>
int br_desnih(Node<T>* korijen) {
  auto br = 0;
  if (korijen == nullptr) return br;
  br = br + br_desnih(korijen->left);
  br = br + br_desnih(korijen->right);
  if (korijen->left == nullptr && korijen->right != nullptr) ++br;

  return br;
}

template <typename T>
int br_lijevih(Node<T>* korijen) {
  auto br = 0;
  if (korijen == nullptr) return br;
  br = br + br_lijevih(korijen->left);
  br = br + br_lijevih(korijen->right);
  if (korijen->left != nullptr && korijen->right == nullptr) ++br;
  return br;
}

template <typename T>
int br_grana(Node<T>* korijen) {
  auto br = 0;
  if (korijen == nullptr) return br;

  if (korijen->left != nullptr || korijen->right != nullptr) ++br;

  br = br + br_grana(korijen->left);
  br = br + br_grana(korijen->right);
  return br;
}

template <typename T>
int suman(T n, Node<T>* c) {
  auto suma = 0;
  if (c == nullptr) return 0;
  if (c->data == n) suma = n;
  suma = suma + suman(n, c->left);
  suma = suma + suman(n, c->right);
  return suma;
}


template<typename T>
std::vector<T> get_inorder(Node<T>* korijen)
{
  std::vector<T> vektor;
  std::vector<T> desnivektor;
  if(korijen->left!=nullptr)
    vektor=get_inorder(korijen->left);
  vektor.push_back(korijen->data);
  desnivektor.push_back(korijen->data);
  if(korijen->right != nullptr)
  {
    desnivektor=get_inorder(korijen->right);
    for(auto i=0; i<desnivektor.size(); ++i)
      vektor.push_back(desnivektor.at(i));
  }
  return vektor;
    
    
}

// template <typename T>
// std::vector<T> get_inorder(Node<T>* korijen) {
//   std::vector<T> vek{};
//   std::vector<T> vekd{};
//   if (korijen->left != nullptr) vek = get_inorder(korijen->left);
//
//   vek.push_back(korijen->data);
//   vekd.push_back(korijen->data);
//
//   if (korijen->right != nullptr) {
//     vekd = get_inorder(korijen->right);
//
//     for (auto item : vekd) vek.push_back(item);
//   }
//   return vek;
// }

template <typename T>
void dealociraj(Node<T>* c) {
  if (c == nullptr) return;
  dealociraj(c->left);
  dealociraj(c->right);
  delete c;
  c = nullptr;
}

//
template <typename T>
int br_listova(Node<T>* korijen) {
  auto br = 0;
  if (korijen == nullptr) return br;
  if (korijen->left == nullptr && korijen->right == nullptr) ++br;

  br = br + br_listova(korijen->left);
  br = br + br_listova(korijen->right);
  return br;
}

template <typename T>
void ispis_po_nivoima(Node<T>* cvor) {
  if (cvor == nullptr) return;
  std::queue<Node<T>*> red;
  red.push(cvor);
  while (!(red.empty())) {
    auto item = red.front();

    std::cout << item->data << std::endl;
    if (item->left != nullptr) red.push(item->left);
    if (item->right != nullptr) red.push(item->right);
    red.pop();
  }
}

template <typename T>
int visina_odlicno(Node<T>* c) {
  if (c == nullptr) return 0;
  std::queue<Node<T>*> red;
  auto broj = 0;
  if (c->left != nullptr && c->right != nullptr)
    ++broj;
  else
    return 0;
  red.push(c->left);
  red.push(c->right);
  auto count = 2;
  while (!(red.empty())) {
    auto provjera = 0;
    for (auto i = 0; i < count; ++i) {
      auto tmp = red.front();
      if (tmp->left != nullptr && tmp->right != nullptr) {
        ++provjera;
        red.pop();
        red.push(tmp->left);
        red.push(tmp->right);
      }
    }
    if (provjera == count)
      ++broj;
    else
      break;
    count = count * 2;
  }

  return broj;
}

}  // namespace Types
