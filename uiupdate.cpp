#include "uiupdate.hpp"
#include "MainUI.hpp"

/**
  * This should run on the main thread. It is a callback for Fl::awake()
  * Do NOT call Fl::lock() or use FLLock in this function
  */
void ui_scan_update_cb(void* data)
{
    auto* msg = static_cast<UIUpdate*>(data);
    if (msg == nullptr) {
        return;
    }

    std::unique_ptr<UIUpdate> cleaner{msg};

    auto win_weak = msg->window;
    auto win = win_weak.lock();
    if (win == nullptr) {
        return;
    }

    switch (msg->type) {
        case UIUpdate::Type::Done: {
            if (msg->duplicates) {
                auto local = std::move(*msg->duplicates);
                win->updateTable(local);
            } else {
                win->updateTable(DupDetectWindow::DuplicateFilesCollection{});
            }

                win->display_not_scanning();
                win->reset_progress(0, 1);
                win->My_currentTargetFile->value(msg->message.c_str());
            break;
        }
        case UIUpdate::Type::Progress: {
            win->reset_progress(0, msg->total);
            win->My_scanProgressBar->value(msg->progress);
            win->My_currentTargetFile->value(msg->message.c_str());
            break;
        }
    }
}
