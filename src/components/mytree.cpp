#include "mytree.hpp"

MyTree::MyTree(int X, int Y, int W, int H, const char* L)
    : Fl_Tree(X, Y, W, H, L)
{
}

MyTreeItem* MyTree::add(const char* label)
{
    auto* item = new MyTreeItem(this, label);
    Fl_Tree::add(label, item);
    return item;
}

MyTreeItem* MyTree::add(MyTreeItem* parent, const char* label)
{
    auto* item = new MyTreeItem(this, label);
    parent->add(prefs(), label, item);
    return item;
}

void MyTree::remove(MyTreeItem* item)
{
    // is this needed?
    Fl_Tree::remove(item);
    // delete item;
}

MyTree::~MyTree() = default;
