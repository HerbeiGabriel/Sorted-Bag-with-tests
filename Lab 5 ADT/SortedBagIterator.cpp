#include "SortedBagIterator.h"
#include <stdexcept>

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    stack = new int[bag.cap];
    first();
}

SortedBagIterator::~SortedBagIterator() {
    delete[] stack;
    current = NULL_TCOMP;
    stackSize = 0;
}

void SortedBagIterator::first() {
    stackSize = 0;
    current = bag.root;
    while (current != NULL_TCOMP) {
        stack[stackSize++] = current;
        current = bag.array_left[current];
    }
    if (stackSize > 0) {
        current = stack[--stackSize];
    }
    else {
        current = NULL_TCOMP;
    }
}

void SortedBagIterator::next() {
    if (!valid()) {
        throw std::out_of_range("Iterator out of range");
    }
    int node = bag.array_right[current];
    if (node != NULL_TCOMP) {
        do {
            stack[stackSize++] = node;
            node = bag.array_left[node];
        } while (node != NULL_TCOMP);
    }
    if (stackSize > 0) {
        current = stack[--stackSize];
    }
    else {
        current = NULL_TCOMP;
    }
}

bool SortedBagIterator::valid() const {
    return current != NULL_TCOMP;
}

TComp SortedBagIterator::getCurrent() const {
    if (!valid()) {
        throw std::out_of_range("Iterator out of range");
    }
    return bag.array_nodes[current].get_info();
}
