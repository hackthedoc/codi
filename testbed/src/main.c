#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(void) {
    CFATAL("A test message: %f", 3.14f);
    CERROR("A test message: %f", 3.14f);
    CWARN("A test message: %f", 3.14f);
    CINFO("A test message: %f", 3.14f);
    CDEBUG("A test message: %f", 3.14f);
    CTRACE("A test message: %f", 3.14f);

    platformState state;
    if (platform_Sartup(&state, "Codi Engine Testbed", 100, 100, 1280, 720)) {
        while (TRUE) {
            platform_PumpMessage(&state);
        }
    }

    platform_Shutdown(&state);

    return 0;
}
