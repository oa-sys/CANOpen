/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>
int16_t canopen_SocketCAN_construct(
    canopen_SocketCAN this)
{
    if (this->id <= 0) {
        corto_throw("Invalid device id [%d].", this->id);
        goto error;
    }

    return 0;
error:
    return -1;
}

int16_t canopen_SocketCAN_receive(
    canopen_SocketCAN this)
{
    return 0;
}

int16_t canopen_SocketCAN_transmit(
    canopen_SocketCAN this,
    canopen_TxData data)
{

    return 0;
}
