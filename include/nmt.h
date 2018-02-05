#ifndef __DRIVER_MNT_CANOPEN__NMT_H__
#define __DRIVER_MNT_CANOPEN__NMT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CANOPEN_HEARTBEAT_ID_BASE 1792
const uint16_t CANOPEN_HEARTBEAT_CONSUMER_INDEX = 4118;         // 0x1016
const uint16_t CANOPEN_HEARTBEAT_CONSUMER_MAX_SUB_INDEX = 0;    // 0x00
const uint16_t CANOPEN_HEARTBEAT_PRODUCER_INDEX = 4119;         // 0x1017

#ifdef __cplusplus
}
#endif
#endif //__DRIVER_MNT_CANOPEN__NMT_H__
