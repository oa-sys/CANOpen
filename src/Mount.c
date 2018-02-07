/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>

int16_t canopen_Mount_construct(
    canopen_Mount this)
{
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

    if (!canopen_nmt__create(this, "NMT", this->dictionary)) {
        corto_throw("Failed to create network management service.");
        goto error;
    }
    return 0;
error:
    return -1;
}
