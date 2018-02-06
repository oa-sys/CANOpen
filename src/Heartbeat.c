/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>
#include <include/nmt.h>
int16_t canopen_Heartbeat_construct(
    canopen_Heartbeat this)
{
    corto_info("Create heartbeat.");
    if (!this->dictionary) {
        corto_throw("Heartbeat [dictionary] invalid");
        goto error;
    }

    if (canopen_Heartbeat_is_consumer(this)) {
        corto_throw("Failed to resolve device heartbeat consumer setting");
        goto error;
    }

    if (canopen_Heartbeat_is_producer(this)) {
        corto_throw("Failed to resolve device heartbeat producer setting");
        goto error;
    }

    return 0;
error:
    return -1;
}

int16_t canopen_Heartbeat_start(
    canopen_Heartbeat this)
{
    return 0;
}

int16_t canopen_Heartbeat_stop(
    canopen_Heartbeat this)
{
    return 0;
}

void canopen_Heartbeat_destruct(
    canopen_Heartbeat this)
{

}

bool canopen_Heartbeat_consumer(
    canopen_Heartbeat this)
{
    return 0;
}

bool canopen_Heartbeat_producer(
    canopen_Heartbeat this)
{
    return 0;
}

// static
// int canopen_Dictionary_traverse2(
//     corto_object object,
//     void *ctx)
// {
//
//     return 0;
// }

bool canopen_Heartbeat_is_consumer(
    canopen_Heartbeat this)
{
    canopen_Entry entry = canopen_Dictionary_lookup(
        this->dictionary,
        CANOPEN_HEARTBEAT_CONSUMER_INDEX,
        CANOPEN_HEARTBEAT_CONSUMER_MAX_SUB_INDEX
    );

    if (!entry) {
        corto_throw("Failed to lookup [%d]:[%d] config.",
            CANOPEN_HEARTBEAT_CONSUMER_INDEX,
            CANOPEN_HEARTBEAT_CONSUMER_MAX_SUB_INDEX);
        goto error;
    }

    uint32_t i = 0;
    for (i = 1; i < *(uint32_t*)entry->v; i++) {
        canopen_Entry consumer = canopen_Dictionary_lookup(
            this->dictionary,
            CANOPEN_HEARTBEAT_CONSUMER_INDEX,
            i
        );
        uint32_t value = *(uint32_t*)consumer->v;
        if (value > 0) {
            return true;
        }
    }

    return false;
error:
    return false;
}

bool canopen_Heartbeat_is_producer(
    canopen_Heartbeat this)
{
    return 0;
}
