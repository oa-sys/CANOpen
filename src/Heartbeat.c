/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>
#include <include/nmt.h>

void* canopen_Heartbeat_thread(void* arg);

static corto_time canopen_Heartbeat_doubleToTime(double frequency) {
    corto_time result;
    result.sec = frequency;
    frequency -= result.sec;
    if (frequency >= 0) {
        result.nanosec = frequency * 1000000000.0;
    }

    return result;
}

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

    if (this->consumer || this->producer) {
        if (canopen_Heartbeat_start(this)) {
            corto_throw("Failed to start heartbeat thread.");
            goto error;
        }
    }

    return 0;
error:
    return -1;
}

int16_t canopen_Heartbeat_start(
    canopen_Heartbeat this)
{
    this->thread = (corto_word)corto_thread_new(
            canopen_Heartbeat_thread,
            this);
    return 0;
}

int16_t canopen_Heartbeat_stop(
    canopen_Heartbeat this)
{
    if (this->thread) {
        corto_thread_join((corto_thread)this->thread, NULL);
    }

    return 0;
}

void canopen_Heartbeat_destruct(
    canopen_Heartbeat this)
{
    canopen_Heartbeat_stop(this);
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
        CANOPEN_HEARTBEAT_CONSUMER_SUBINDEX
    );

    if (!entry) {
        corto_throw("Failed to lookup [%d]:[%d] config.",
            CANOPEN_HEARTBEAT_CONSUMER_INDEX,
            CANOPEN_HEARTBEAT_CONSUMER_SUBINDEX);
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
    canopen_Entry entry = canopen_Dictionary_lookup(
        this->dictionary,
        CANOPEN_HEARTBEAT_PRODUCER_INDEX,
        CANOPEN_HEARTBEAT_PRODUCER_SUBINDEX
    );

    if (!entry) {
        corto_throw("Failed to lookup [%d]:[%d] config.",
            CANOPEN_HEARTBEAT_PRODUCER_INDEX,
            CANOPEN_HEARTBEAT_PRODUCER_SUBINDEX);
        goto error;
    }

    uint32_t value = *(uint32_t*)entry->v;
    if (value > 0) {
        return true;
    }

    return false;
error:
    return false;
}

void* canopen_Heartbeat_thread(void* arg)
{
    corto_mount this = arg;
    corto_float64 frequency = 1.0;
    corto_time interval = canopen_Heartbeat_doubleToTime(1.0 / frequency);
    corto_time next, current, sleep = {0, 0}, lastSleep = {0, 0};
    corto_time_get(&next);
    next = corto_time_add(next, interval);
    while (!this->quit) {
        ///TODO DO WORK

        corto_time_get(&current);
        lastSleep = sleep;
        sleep = corto_time_sub(next, current);
        if (lastSleep.sec || lastSleep.nanosec) {
            /* Attempt to limit the amount of oscillation in a fully loaded system */
            if (corto_time_compare(lastSleep, sleep) == CORTO_LT) {
                double tmp = (corto_time_toDouble(sleep) + corto_time_toDouble(lastSleep)) / 2;
                sleep = canopen_Heartbeat_doubleToTime(tmp);
            }

        }

        if (sleep.sec >= 0) {
            corto_sleep(sleep.sec, sleep.nanosec);
        } else {
            sleep = corto_time_sub(current, next);
            corto_warning(
                "processing events took [%d.%.9d] longer than sampleRate interval",
                sleep.sec, sleep.nanosec);
            corto_time_get(&next);
        }

        next = corto_time_add(next, interval);
    }

    return NULL;
}
