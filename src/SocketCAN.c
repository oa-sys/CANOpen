/* This is a managed file. Do not delete this comment. */

#include <include/canopen.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

int16_t canopen_SocketCAN_construct(
    canopen_SocketCAN this)
{
    if (this->id < 0) {
        corto_throw("Invalid device id [%d].", this->id);
        goto error;
    }

    /* Create and bind socket */
    struct sockaddr_can addr;

    this->fd = socket(AF_CAN, SOCK_RAW, CAN_RAW);
    if (this->fd < 0) {
        corto_throw("Device ID [%d] Failed to CREATE [AF_CAN] socket ID [%d]",
            this->id, this->fd);
        goto error;
    } else {
        addr.can_family = AF_CAN;
        addr.can_ifindex = this->id;
        int ret = bind(this->fd, (struct sockaddr*) &addr, sizeof(addr));
        if (ret) {
            corto_throw("Device ID [%d] Failed to BIND [AF_CAN] socket ID [%d] | %s",
                this->id,
                this->fd,
                strerror(ret));
            goto error;
        }
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
