#include "mytreeitem.hpp"

MyTreeItem::MyTreeItem(Fl_Tree* tree, char const* label_) : Fl_Tree_Item(tree)
{
    label(label_);
}

void MyTreeItem::set_file_data(const std::string& path,
                               const std::string& hash,
                               FileData::Type     type,
                               bool               survivor)
{
    file_data.path     = path;
    file_data.hash     = hash;
    file_data.type     = type;
    file_data.survivor = survivor;
    user_data(&file_data);
}

std::string MyTreeItem::get_path() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data ? data->path : "";
}

std::string MyTreeItem::get_hash() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data ? data->hash : "";
}

FileData* MyTreeItem::get_file_data() const
{
    return static_cast<FileData*>(user_data());
}

bool MyTreeItem::is_directory() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data && data->type == FileData::Type::DIRECTORY;
}

bool MyTreeItem::is_file() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data && data->type == FileData::Type::FILE;
}

bool MyTreeItem::is_root() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data && data->type == FileData::Type::ROOT;
}

bool MyTreeItem::is_survivor() const
{
    auto* data = static_cast<FileData*>(user_data());
    return data && data->survivor;
}

MyTreeItem::~MyTreeItem() = default;