#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <iostream>

SortedBag::SortedBag(Relation r) {
    rel = r;
    cap = 10;
    array_nodes = new Node[cap];
    array_left = new int[cap];
    array_right = new int[cap];
    for (int i = 0; i < cap; i++) {
        array_left[i] = NULL_TCOMP;
        array_right[i] = NULL_TCOMP;
    }
    root = NULL_TCOMP;
    first_empty = 0;
    sz = 0;
    sz1 = 0;
}

SortedBag::~SortedBag() {
    delete[] array_nodes;
    delete[] array_left;
    delete[] array_right;
}

void SortedBag::resize() {
    int oldcap = cap;
    cap = cap * 2;
    Node* array_nodes_new = new Node[cap];
    int* array_left_new = new int[cap];
    int* array_right_new = new int[cap];
    for (int i = 0; i < cap; i++) {
        array_left_new[i] = NULL_TCOMP;
        array_right_new[i] = NULL_TCOMP;
    }
    for (int i = 0; i < oldcap; i++) {
        array_left_new[i] = array_left[i];
        array_right_new[i] = array_right[i];
        array_nodes_new[i] = array_nodes[i];
    }
    delete[] array_nodes;
    delete[] array_right;
    delete[] array_left;
    array_nodes = array_nodes_new;
    array_left = array_left_new;
    array_right = array_right_new;
}

void SortedBag::add(TComp e) {
    if (sz1 == cap)
        resize();
    if (root == NULL_TCOMP)
        root = first_empty;
    add2(e, root);
}

void SortedBag::add2(TComp e, int i) {
    if (array_nodes[i].get_info() == NULL_TCOMP) {
        array_nodes[i].set_info(e);
        array_nodes[i].add_frequency();
        first_empty++;
        sz++;
        sz1++;
    }
    else if (array_nodes[i].get_info() == e) {
        array_nodes[i].add_frequency();
        sz1++;
    }
    else if (rel(e, array_nodes[i].get_info())) {
        if (array_left[i] == NULL_TCOMP) {
            array_left[i] = first_empty;
            add2(e, first_empty);
        }
        else {
            add2(e, array_left[i]);
        }
    }
    else {
        if (array_right[i] == NULL_TCOMP) {
            array_right[i] = first_empty;
            add2(e, first_empty);
        }
        else {
            add2(e, array_right[i]);
        }
    }
}

void SortedBag::delete_parent(int index) {
    for (int i = 0; i < cap; i++) {
        if (array_left[i] == index) {
            array_left[i] = NULL_TCOMP;
        }
        if (array_right[i] == index) {
            array_right[i] = NULL_TCOMP;
        }
        if (index < first_empty) {
            first_empty = index; // Update first_empty if needed
        }
    }
}

bool SortedBag::remove(TComp e) {
    int index = search1(e);
    if (index == -1) {
        return false; // element not found
    }

    if (array_nodes[index].get_frequency() > 1) {
        array_nodes[index].cut_frequency();
        sz1--;
    }
    else {
        // Case when the root is the element to be removed
        if (index == root) {
            if (array_left[root] == NULL_TCOMP && array_right[root] == NULL_TCOMP) {
                // If root has no children
                array_nodes[root].set_info(NULL_TCOMP);
                array_nodes[root].set_frequency(0);
                root = NULL_TCOMP;
                first_empty = root;
                sz--;
                sz1--;
            } 
            else if (array_left[root] != NULL_TCOMP && array_right[root] == NULL_TCOMP) {
                // Root has only left child
                root = array_left[root];
                array_nodes[index].set_info(NULL_TCOMP);
                array_nodes[index].set_frequency(0);
                first_empty = index;
                sz--;
                sz1--;
            } 
            else if (array_left[root] == NULL_TCOMP && array_right[root] != NULL_TCOMP) {
                // Root has only right child
                root = array_right[root];
                array_nodes[index].set_info(NULL_TCOMP);
                array_nodes[index].set_frequency(0);
                first_empty = index;
                sz--;
                sz1--;
            } 
            else {
                // Root has two children
                int successor = array_right[root];
                while (array_left[successor] != NULL_TCOMP) {
                    successor = array_left[successor];
                }
                array_nodes[root].set_info(array_nodes[successor].get_info());
                array_nodes[root].set_frequency(array_nodes[successor].get_frequency());
                remove(array_nodes[successor].get_info()); // Recursively remove the successor
            }
        } 
        else {
            // Handle the case where the node to be removed is not the root
            if (array_left[index] == NULL_TCOMP && array_right[index] == NULL_TCOMP) {
                array_nodes[index].set_info(NULL_TCOMP);
                array_nodes[index].set_frequency(0);
                delete_parent(index); // delete the right or left of the parent
                first_empty = index;
                sz--;
                sz1--;
            }
            else if (array_left[index] != NULL_TCOMP && array_right[index] != NULL_TCOMP) {
                int successor = array_right[index];
                while (array_left[successor] != NULL_TCOMP) {
                    successor = array_left[successor];
                }
                array_nodes[index].set_info(array_nodes[successor].get_info());
                array_nodes[index].set_frequency(array_nodes[successor].get_frequency());
                remove(array_nodes[successor].get_info()); // Recursively remove the successor
            }
            else {
                int childIndex = (array_left[index] != NULL_TCOMP) ? array_left[index] : array_right[index];
                array_nodes[index].set_info(array_nodes[childIndex].get_info());
                array_nodes[index].set_frequency(array_nodes[childIndex].get_frequency());
                array_left[index] = array_left[childIndex];
                array_right[index] = array_right[childIndex];
                array_nodes[childIndex].set_info(NULL_TCOMP);
                array_nodes[childIndex].set_frequency(0);
                delete_parent(childIndex);
                first_empty = childIndex;
                sz--;
                sz1--;
            }
        }
    }
    return true;
}


bool SortedBag::search(TComp elem) const {
    for (int i = 0; i < cap; i++) {
        if (array_nodes[i].get_info() == elem && array_nodes[i].get_info()!=NULL_TCOMP)
            return true;
    }
    return false;
}

int SortedBag::search1(TComp elem) const {
    for (int i = 0; i < cap; i++) {
        if (array_nodes[i].get_info() == elem)
            return i;
    }
    return -1;
}

int SortedBag::nrOccurrences(TComp elem) const {
    int t = search1(elem);
    if (t != -1) {
        return array_nodes[t].get_frequency();
    }
    else return 0;
}

int SortedBag::size() const {
    return sz1;
}

bool SortedBag::isEmpty() const {
    return (sz == 0);
}

SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}

void SortedBag::print() const {
    for (int i = 0; i < cap; i++) {
        std::cout << i << ". " << "info: " << array_nodes[i].get_info() << " frequency: " << array_nodes[i].get_frequency() << " left: " << array_left[i] << " right: " << array_right[i] << std::endl;
    }
}
