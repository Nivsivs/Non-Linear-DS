#include <iostream>
#include <string>
#include <stdexcept>
#include "HybridList.h"
using namespace std;

HybridList::HybridList() {}

HybridList::HybridList(int blockSize) {
	if (blockSize <= 0)
		throw out_of_range("Invalid bloc k size " + to_string(blockSize));
	this->blockSize = blockSize;
}
HybridList:: HybridList(const HybridList& h){
    operator = (h);
}


HybridList::~HybridList() {
	clear();
}

int HybridList::size() const {
	return numElements;
}

int HybridList::capacity() const {
	return numBlocks * blockSize;
}

int HybridList::block_size() const {
	return blockSize;
}

HybridListNode* HybridList::front() const {
	return head;
}

HybridListNode* HybridList::back() const {
	return tail;
}

double& HybridList::at(int index) const {
	HybridListNode* curNode = head;
	int elementsSearched = 0;

	if (index < 0 || index >= numElements)
		throw out_of_range("Invalid index " + to_string(index));

	// Iterate through all blocks to identify block containing the index
	while (curNode != nullptr) {
		if (index < elementsSearched + curNode->size()) {
			// Element is in this block so just return the correct element from
			// this block
			return curNode->at(index - elementsSearched);
		}
		// Element is not in this block so add the number of elements in the
		// block to the number of elements searched
		elementsSearched += curNode->size();
		curNode = curNode->next;
	}

	// Iterator went beyond last block so something went horribly wrong
	abort();
}

HybridList& HybridList::operator=(const HybridList& h){
    HybridListNode* curNode = head, * nextNode = nullptr;

    while (curNode != nullptr) {
        nextNode = curNode->next;
        delete curNode;
        curNode = nextNode;
    }
    head = tail = nullptr;
    numElements = numBlocks = 0;

    curNode = h.front();
    blockSize = h.block_size();

    while (curNode != nullptr) {
        for (int i = 0; i < h.block_size(); i++){
            push_back(curNode->at(i));
        }
    }

    return *this;
}

void HybridList::push_back(double value) {
	if (numBlocks == 0) {
		// Hybrid list is empty so creating a new node that will be both the head
		// and tail and append the element to it
		HybridListNode* newTail = new HybridListNode(blockSize);
		newTail->push_back(value);
		tail = newTail;
		head = newTail;
		numBlocks = 1;
	}
	else if (tail->size() == blockSize) {
		// Tail node is full so create a new tail node and copy the back half of
		// the old tail node to the new tail node
		HybridListNode* newTail = new HybridListNode(blockSize);

		// Copy just under half of elements from old tail to new tail
		for (int i = blockSize / 2 + 1; i < blockSize; i++)
			newTail->push_back(tail->at(i));
		tail->resize(blockSize / 2 + 1);
		//cout << tail->size() << endl;
		// Append new item to new tail
		newTail->push_back(value);
		tail->next = newTail;
		//cout << newTail->size() << endl;
		//cout << "Tail: " << tail << endl;
		//cout << "New tail: " << newTail << endl;
		tail = newTail;
		numBlocks++;
	}
	else
		tail->push_back(value);	// Tail isn't full so just append to tail
	
	numElements++;
}

void HybridList::pop_back(){
    if (numElements == 0)
        throw out_of_range("list is empty");

    if (tail->size()>1){
        tail->pop_back();
    }
    else {
        if (numBlocks == 1){
            delete tail;
            head = tail = nullptr;
        }
        else{
            HybridListNode* curNode = head;
            while (curNode->next != tail){
                curNode = curNode->next;
            }
            delete tail;
            tail = curNode;
            tail->next = nullptr;
        }
        numBlocks--;
    }
    numElements--;
}

void HybridList::insert(int index, double value) {
    if (index < 0 || index > numElements)
        throw out_of_range("Invalid Index " + to_string(index));
    if (index == numElements) {
        push_back(value);
        return;
    }
    HybridListNode* curNode = head;
    int elementsSearched = 0;

    while (curNode != nullptr) {
        if (index < elementsSearched + curNode->size()) {
            curNode->insert(index - elementsSearched, value);
            break;
        }

        elementsSearched += curNode->size();
        curNode = curNode->next;
    }
    numElements++;
 
}

void HybridList::erase(int index) {
    HybridListNode* curNode = head;
    int elementSearched = 0;

    if (index < 0 || index >= numElements)
    throw out_of_range("Invalid index " + to_string(index));

    while(curNode != nullptr){
        if (index < elementSearched + curNode->size()){
            if (curNode->size() <= 1){
                HybridListNode* nextNode = curNode->next;
                delete curNode;
                if (curNode == head) {
                    head = nextNode;
                }
                else {
                    HybridListNode* prevNode = head;
                    while (prevNode->next != curNode) {
                        prevNode = prevNode->next;
                    }
                    prevNode->next = nextNode;
                }
                numBlocks--;
            }
            else {
                curNode->erase(index - elementSearched);
            }
            break;
        }
        elementSearched += curNode ->size();
        curNode = curNode->next;
    }
    numElements--;
}
 
void HybridList::clear() {
	HybridListNode* curNode = head, * nextNode = nullptr;
	// Iterate through each node starting from the head and delete it
	while (curNode != nullptr) {
		nextNode = curNode->next;
		delete curNode;
		curNode = nextNode;
	}
	head = tail = nullptr;
	numElements = numBlocks = 0;
}