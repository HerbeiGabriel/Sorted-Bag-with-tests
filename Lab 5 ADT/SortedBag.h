#pragma once
#include <utility>
typedef int TComp;
typedef TComp TElem;
typedef bool(*Relation)(TComp, TComp);
#define NULL_TCOMP -11111

class Node {
private:
    TComp info;
    int frequency;
public:
    Node() : info(NULL_TCOMP), frequency(0) {}
    TElem get_info() const { return info; }
    void set_info(TElem i) { info = i; }
    int get_frequency() const { return frequency; }
    void set_frequency(int fr) { frequency = fr; }
    void add_frequency() { frequency++; }
    void cut_frequency() { frequency--; }
};

class SortedBagIterator;

class SortedBag {
    friend class SortedBagIterator;

private:
    Node* array_nodes;
    int* array_left;
    int* array_right;
    int cap;
    int first_empty;
    int sz;
    int sz1;
    Relation rel;
    int root;

    void resize();
    void add2(TComp e, int i);
    void delete_parent(int index);
    int search1(TComp e) const;

public:
    SortedBag(Relation r);
    ~SortedBag();
    void add(TComp e);
    bool remove(TComp e);
    bool search(TComp e) const;
    int nrOccurrences(TComp e) const;
    int size() const;
    bool isEmpty() const;
    SortedBagIterator iterator() const;
    void print() const;
};
