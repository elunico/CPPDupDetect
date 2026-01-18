#ifndef MYTREE_HPP
#define MYTREE_HPP

#include <FL/Fl_Tree.H>
#include "mytreeitem.hpp"

class MyTree : public Fl_Tree {
   public:
    MyTree(int X, int Y, int W, int H, const char* L = 0);

    MyTreeItem* add(const char* label);

    MyTreeItem* add(MyTreeItem* parent, const char* label);

    void remove(MyTreeItem* item);

    virtual ~MyTree();
};

#endif  // MYTREE_HPP
