/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>
int16_t canopen_Mount_construct(
    canopen_Mount this)
{
    if (this->timing == CANOPEN_TIMING_UNKNOWN) {
        corto_error("[timing] undefined.");
        goto error;
    }

    if (canopen_Mount_init_driver(this)) {
        corto_throw("Failed to Initialize CAN Socket.");
        goto error;
    }

    if (!this->dictionary) {
        this->dictionary = canopen_Dictionary__create(
            NULL,
            NULL,
            this->id,
            corto_fullpath(NULL, this));
        if (!this->dictionary) {
            corto_throw("Failed to construct dictionary.");
            goto error;
        }
    }

    canopen_nmt nmt = canopen_nmt__create(
        this,
        "NMT",
        this->driver,
        this->dictionary);
    if (!nmt) {
        corto_throw("Failed to create network management service.");
        goto error;
    }

    return 0;
error:
    return -1;
}

int16_t canopen_Mount_init_driver(
    canopen_Mount this)
{
    if (this->socketType == CANOPEN_DRIVER_UNKNOWN) {
        corto_error("[socketType] undefined");
        goto error;
    }

    if (this->socketType == CANOPEN_DRIVER_SOCKET_CAN) {
        this->driver = canopen_SocketCAN__create(
            NULL,
            NULL,
            this->id);
    }

    if (!this->driver) {
        corto_throw("Failed to create socket [%d]", this->socketType);
        goto error;
    }

    return 0;
error:
    return -1;
}
