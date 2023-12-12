#include <iostream>
#include <string>
#include "SortedSinglyLinkedList.hpp"
#include "sortedsinglylinkedlist.hpp"
using namespace Types;
struct ToDoTask {
  int id;
  std::string title;
  std::string description;
  int priority;
  bool operator>=(const ToDoTask& task) { return priority >= task.priority; }
  bool operator<=(const ToDoTask& task) { return priority <= task.priority; }
  bool operator<(const ToDoTask& task) { return priority < task.priority; }
  bool operator==(int id2) const { return id == id2; }
  friend std::ostream& operator<<(std::ostream& os, const ToDoTask& task) {
    std::string task2 = "* Task *";
    auto tasksize = task2.size();
    std::string linija(tasksize, '*');
    std::cout << linija << std::endl
              << task2 << std::endl
              << linija << std::endl
              << std::endl;

    os << "Title: " << task.title << std::endl;
    os << "Description: " << task.description << std::endl;
    os << "Priority: " << task.priority << "\n" << std::endl;
    return os;
  }
  void printtask() const {
    std::cout << "Title: " << title << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Priority: " << priority << std::endl;
    std::cout << "Id: " << id << std::endl;
  }
};

void print(const SortedSinglyLinkedList<ToDoTask>& lista) {
  std::cout << std::string(40, '*') << std::endl;

  auto it = lista.begin();
  while (it != lista.end()) {
    lista.getdata(it).printtask();
    std::cout << std::endl;
    ++it;
  }
  std::cout << std::string(40, '*') << std::endl;
}

int generateId() {
  static int counter = 0;
  return ++counter;
}
size_t choice() {
  std::cout << "Choose one of the options\n";
  std::cout << std::string(40, '*') << std::endl;
  std::cout << "\n1. Display the current list of ToDoTask\n";
  std::cout << "2. Enter a new ToDoTask\n";
  std::cout << "3. Mark ToDoTask as done\n";
  size_t choice;
  std::cout << "Enter your choice: ";
  std::cin >> choice;
  std::cout << std::string(40, '*') << std::endl;
  return choice;
}
void provjeriizbor(size_t, SortedSinglyLinkedList<ToDoTask>&);
void foo(SortedSinglyLinkedList<ToDoTask>& lista) {
  size_t izbor2;
  std::cout << "Enter number 0 to exit program, or something else continue:\n";
  std::cin >> izbor2;
  if (izbor2 == 0) {
    return;
  } else {
    size_t izbor3 = choice();
    provjeriizbor(izbor3, lista);
  }
}
void provjeriizbor(size_t izbor, SortedSinglyLinkedList<ToDoTask>& lista) {
  if (izbor == 1) {
    if (lista.empty()) {
      std::cout << "Your notes is empty\n";
      foo(lista);
    } else {
      std::cout << lista;
      foo(lista);
    }

  } else if (izbor == 2) {
    std::cout << "Enter a new ToDoTask:" << std::endl;
    ToDoTask newtask;
    std::cout << "Enter the title of the task: ";
    std::cin.ignore(1024, '\n');
    std::getline(std::cin, newtask.title);
    std::cout << "Enter the description of the task: ";
    std::getline(std::cin, newtask.description);
    std::cout << "Enter a priority (>0): ";
    std::cin >> newtask.priority;
    newtask.id = generateId();
    lista.add(newtask);
    foo(lista);
  } else if (izbor == 3) {
    if (lista.empty()) {
      std::cout << "Your list is empty\n";
      foo(lista);
    } else {
      print(lista);
      std::cout << "Enter id of the task which you want to mark as done: ";
      int id2;
      std::cin >> id2;
      std::function<bool(ToDoTask task)> predicate = [id2](ToDoTask task) {
        if (task.id == id2) return true;
        return false;
      };
      auto it = lista.find(predicate);
      if (it == lista.end()) {
        std::cout << "Task with id = " << id2 << " does not exist\n";
        size_t izbor3 = choice();
        provjeriizbor(izbor3, lista);

      } else {
        lista.remove(it);

        foo(lista);
      }
    }
  }
}

int main(void) {
  Types::SortedSinglyLinkedList<ToDoTask> lista;
  size_t izbor = choice();
  provjeriizbor(izbor, lista);

  return 0;
}

