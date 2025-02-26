#pragma once
#include "SortedBag.h"

class SortedBagIterator {
private:
    const SortedBag& bag;
    int current;
    int* stack;
    int stackSize;

public:
    SortedBagIterator(const SortedBag &b);
    ~SortedBagIterator();
    void first();
    void next();
    bool valid() const;
    TComp getCurrent() const;
};
