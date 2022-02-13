// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"

using std::cout;
using std::endl;
using std::domain_error;
using namespace std;

JQueue::JQueue(prifn_t priFn) {
  
  //setting the priority
  priority = priFn;
  //initially the heap is empty
  _heap = nullptr;
  //initially the size is zero
  _size = 0; 

}

JQueue::~JQueue() {

  if (_heap != nullptr){
  clear(); //clears the heap
  }

}

JQueue::JQueue(const JQueue& rhs) {

  _heap = copy(rhs._heap); //copying the heap
  _size = rhs._size;
  priority = rhs.priority;

}

Node* JQueue::copy(Node* ptr){

  //rhs doesn't exist
  if (ptr == nullptr){
    return nullptr;
  }
  //rhs exists therefore making copy of the heap
  else{
    //creating a new node w/ job info
    Node* newNode = new Node(ptr -> _job);

    //copying the right and the left pointers
    newNode -> _left = copy(ptr -> _left);
    newNode -> _right = copy(ptr -> _right);

    return newNode;
  }


}

JQueue& JQueue::operator=(const JQueue& rhs) {

  //the objects are different
  if (this != &rhs){

    //RemoveHeap(_heap); //deleting the heap before assigning
    clear(); //deleting the heap before assigning 
    _heap = copy(rhs._heap); //copying the heap
    _size = rhs._size;
    priority = rhs.priority;

  }

  return *this;

}

void JQueue::insertJob(const job_t& input) {

  //creating the new tree
  Node* newNode = new Node(input);
  //merging the tree with the older tree and setting the heap to the root of the new tree
  _heap = merge(_heap, newNode);
  _size++;

}

job_t JQueue::getNextJob() {

  job_t el;
  Node* ptr;

  if (_heap == nullptr){
    throw domain_error("Error: Index Out of Range");
  }

  else{
    ptr = _heap;
    el = _heap -> _job;
    _heap = merge(_heap -> _left, _heap -> _right);

    _size = _size - 1;
    delete ptr;
    ptr = nullptr;
  }

  return el;
  

}

void JQueue::mergeWithQueue(JQueue& rhs) {

  if (priority != rhs.priority){
    throw domain_error("Error: Priority does not match");
  }
  else{
    _heap = merge(_heap, rhs._heap);
    rhs._heap = nullptr;
    _size += rhs._size;
    rhs._size = 0;
  }

}

Node* JQueue::merge(Node* tree1, Node* tree2){

  // if one of the heaps is empty then return the other
  if (tree1 == nullptr){
    return tree2;
  }
  if (tree2 == nullptr){
    return tree1;
  }

  //skew heap merging algorithm
  if (priority(tree1->_job) <= priority(tree2->_job)){
    swap(tree1, tree2);
  }

  swap(tree1 -> _left, tree1 -> _right);
  tree1 -> _left = merge(tree1 -> _left, tree2);
  
  return tree1;
}

void JQueue::clear() {

  //deleting the tree
  if (_heap != nullptr){
    int count = 0;
    Node* newNode;
    while (count < _size){
      newNode = _heap;
      _heap = merge(_heap -> _left, _heap -> _right);
      delete newNode;
      newNode = nullptr;
      count += 1;
    }
    _size = _size - count;
  }
}

int JQueue::numJobs() const {
  return _size;
}

void JQueue::printJobQueue() const {

  if (_heap != nullptr) {
    //call the recursive print function
    printQueue(_heap);
  }

}

void JQueue::printQueue(Node* tree) const {

  if (tree == nullptr) {
    return;
  }

  //level order traversal
  cout << "[" << priority(tree->_job) << "]" << tree->_job << endl;
  printQueue(tree->_left);
  printQueue(tree->_right);

}

prifn_t JQueue::getPriorityFn() const {
  return priority;
}

void JQueue::setPriorityFn(prifn_t priFn) {

  //rebuilding the heap
  priority = priFn;
  if (_heap != nullptr){

    JQueue copy(*this);
    clear();

    job_t el;
    Node* ptr;
    int size = 0;

    while(size < copy.numJobs()){

      el = copy._heap -> _job;
      insertJob(el);
      ptr = copy._heap;
      copy._heap = merge(copy._heap -> _left, copy._heap -> _right);
      size = size + 1;
      delete ptr;
      ptr = nullptr;

    }

  }

}

void JQueue::dump() const {
  if (_heap != nullptr) {
    dumper(_heap); //calling recursive dump
  }
}

void JQueue::dumper(Node* tree) const {

  if(tree == nullptr) {
    return;
  }

  //inorder traversal
  cout << "(";
  dumper(tree->_left);
  cout <<  priority(tree->_job);
  dumper(tree->_right);
  cout << ")";

}

ostream& operator<<(ostream& sout, const job_t& job) {
  sout << "Job: " << job._name << ", pri: " << job._priority
       << ", u: " << job._user << ", g: " << job._group << ", proc: "
       << job._proc << ", mem: " << job._mem << ", time: " << job._time;
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node._job;
  return sout;
}

