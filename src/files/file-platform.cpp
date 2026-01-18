#include "file-platform.hpp"

PlatformUnavailableCallback on_platform_unavailable;

void set_on_platform_unavailable(PlatformUnavailableCallback callback)
{
    on_platform_unavailable = callback;
}
