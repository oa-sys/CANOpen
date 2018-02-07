#ifndef __DRIVER_MNT_CANOPEN__NMT_H__
#define __DRIVER_MNT_CANOPEN__NMT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CANOPEN_HEARTBEAT_ID_BASE 1792
const uint16_t CANOPEN_HEARTBEAT_CONSUMER_INDEX = 4118;         // 0x1016
const uint16_t CANOPEN_HEARTBEAT_CONSUMER_SUBINDEX = 0;         // 0x00
const uint16_t CANOPEN_HEARTBEAT_PRODUCER_INDEX = 4119;         // 0x1017
const uint16_t CANOPEN_HEARTBEAT_PRODUCER_SUBINDEX = 0;         // 0x00
const uint32_t CANOPEN_HEARTBEAT_COBID_BASE = 1792;             // 0x700

#ifdef __cplusplus
}
#endif
#endif //__DRIVER_MNT_CANOPEN__NMT_H__
