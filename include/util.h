#ifndef __DRIVER_MNT_CANOPEN__UTIL_H__
#define __DRIVER_MNT_CANOPEN__UTIL_H__

#include <include/canopen.h>

#ifdef __cplusplus
extern "C" {
#endif

DRIVER_MNT_CANOPEN_EXPORT
int canopen_Dictionary_hex2dec(
    int x);

DRIVER_MNT_CANOPEN_EXPORT
int canopen_Dictionary_dec2hex(
    int x);

#ifdef __cplusplus
}
#endif
#endif //__DRIVER_MNT_CANOPEN__UTIL_H__
