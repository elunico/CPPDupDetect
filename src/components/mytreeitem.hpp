#ifndef MYTREEITEM_HPP
#define MYTREEITEM_HPP

#include <FL/Fl_Tree.H>
#include "files/filedata.hpp"

class MyTreeItem : public Fl_Tree_Item {
   public:
    FileData file_data;

    MyTreeItem(Fl_Tree* tree, const char* label_);
    void set_file_data(const std::string& path,
                       const std::string& hash,
                       FileData::Type     type,
                       bool               survivor);

    std::string get_path() const;
    std::string get_hash() const;

    FileData* get_file_data() const;

    bool is_directory() const;

    bool is_file() const;

    bool is_root() const;

    bool is_survivor() const;

    virtual ~MyTreeItem();
};

#endif
