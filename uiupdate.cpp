#include "uiupdate.hpp"
#include <atomic>
#include <memory>
#include "MainUI.hpp"

static std::string make_progress_label(int progress, int total)
{
    return std::format("Scan Progress {:.0f}% ({:Ld}/{:Ld})", ((double(progress) / total) * 100.0), progress, total);
}

/**
 * This should run on the main thread. It is a callback for Fl::awake()
 * Do NOT call Fl::lock() in this function
 */
void ui_scan_update_cb(void* data)
{
    auto* msg = static_cast<UIUpdate*>(data);
    if (msg == nullptr) {
        return;
    }

    std::unique_ptr<UIUpdate> cleaner{msg};

    auto win_weak = msg->window;
    auto win      = win_weak.lock();
    if (win == nullptr) {
        return;
    }

    switch (msg->type) {
        case UIUpdate::Type::Done: {
            if (msg->duplicates) {
                win->updateTable(msg->duplicates);
            } else {
                win->updateTable(std::make_shared<DupDetectWindow::DuplicateFilesCollection>());
            }

            win->display_not_scanning();
            win->reset_progress(0, 1);
            win->My_currentTargetFile->value(msg->message.c_str());
            win->display_not_scanning();
            win->My_scanProgressBar->copy_label(make_progress_label(msg->progress, msg->total).c_str());
            break;
        }
        case UIUpdate::Type::Progress: {
            if (win->scanning.load(std::memory_order_acquire)) {
                // check to see if the program is still scanning, otherwise do
                // not modify the UI in the progress update, it will be usurped
                // the done update
                win->reset_progress(0, msg->total);
                win->My_scanProgressBar->copy_label(make_progress_label(msg->progress, msg->total).c_str());
                win->My_scanProgressBar->value(msg->progress);
                win->My_currentTargetFile->value(msg->message.c_str());
            }
            break;
        }
    }
}
