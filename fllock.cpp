#include "fllock.hpp"

FLLock::FLLock()
{
    Fl::lock();
}

FLLock::~FLLock()
{
    Fl::unlock();
    Fl::awake();
}
