#ifndef UIUPDATE_H
#define UIUPDATE_H

#include <memory>
#include <string>
#include "MainUI.hpp"

struct UIUpdate {
  enum class Type { Progress, Done } type;
  std::size_t progress;
  std::size_t total;
  std::string message;
  std::weak_ptr<DupDetectWindow> window;
  std::shared_ptr<DupDetectWindow::DuplicateFilesCollection> duplicates;
};

void ui_scan_update_cb(void *data);

#endif 
