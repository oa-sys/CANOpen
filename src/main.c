/* This is a managed file. Do not delete this comment. */

#include <driver/mnt/canopen/canopen.h>

int cortomain(int argc, char *argv[]) {

    // canopen_ValueType *t = canopen_ValueTypeCreate_b(CANOPEN_TYPE_BOOLEAN, true);
    // canopen_Entry o = canopen_EntryCreate(
    //     0x55,
    //     0x00,
    //     "TestName",
    //     CANOPEN_DEFTYPE,
    //     CANOPEN_TYPE_BOOLEAN,
    //     false,
    //     t
    // );
    // char *str = corto_serialize(o, "text/corto");
    // corto_warning("Value: [%s]", str);
    if (corto_use(
        "/home/hendren/dev/oasys/driver-mnt-canopen/config/device_info.json",
        0,
        NULL) != 0)
    {
        return false;
    }
    if (corto_use(
        "/home/hendren/dev/oasys/driver-mnt-canopen/config/mount.json",
        0,
        NULL) != 0)
    {
            return false;
    }

    if (corto_use(
        "/home/hendren/dev/oasys/driver-mnt-canopen/config/ws-server.json",
        0,
        NULL) != 0)
    {
        return false;
    }

    if (corto_use(
        "/home/hendren/dev/oasys/driver-mnt-canopen/config/file-server.json",
        0,
        NULL) != 0)
    {
        return false;
    }



    while(1) {
        usleep(1000);
    }
    return 0;
}
