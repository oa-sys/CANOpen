/* This is a managed file. Do not delete this comment. */

// TODO Add config observer
#include <driver/mnt/canopen/canopen.h>
#include <driver/mnt/canopen/dictionary.h>

const uint16_t CANOPEN_DICTIONARY_SUB_MAX = 255;

int16_t canopen_Dictionary_construct(
    canopen_Dictionary this)
{
    if (!this->from) {
        corto_throw("[from] uninitialized.");
        goto error;
    }

    if (!this->map) {
        corto_throw("[map] uninitialized.");
        goto error;
    }

    if (canopen_Dictionary_fill_device(this)) {
        corto_throw("Failed to map device info configuration.");
        goto error;
    }

    return 0;
error:
    return -1;
}

int16_t canopen_Dictionary_fill_device(
    canopen_Dictionary this)
{
    corto_iter it;
    corto_string from = corto_asprintf("%s/%s", this->from, "device");
    if (corto_select("//").from(from).iter(&it)) {
        corto_throw("Corto select failed at [%s]", from);
        goto error;
    }

while(corto_iter_hasNext(&it) != 0)
{
    corto_record *r = (corto_record*)corto_iter_next(&it);
    canopen_Entry entry = (canopen_Entry)r->object;
    /* Verify that config node exists */
    if (entry) {
        if (canopen_Dictionary_add_entry(this, this->map, entry)) {
            corto_throw("Failed to add entry to device dictionary.");
            goto error;
        }
    }
}

    corto_dealloc(from);
    return 0;
error:
    corto_dealloc(from);
    return -1;
}

int16_t canopen_Dictionary_fill_pdo(
    canopen_Dictionary this)
{
    corto_iter it;
    corto_string from = corto_asprintf("%s/%s", this->from, "pdo");
    if (corto_select("//").from(from).iter(&it)) {
        corto_throw("Corto select failed at [%s]", from);
        goto error;
    }

    while(corto_iter_hasNext(&it) != 0)
    {
        corto_record *r = (corto_record*)corto_iter_next(&it);
        corto_string path = corto_asprintf("%s/%s", from, r->id);
        canopen_Entry entry = (canopen_Entry)corto_lookup(root_o, path);
        corto_dealloc(path);
        /* Verify that config node exists */
        if (entry) {
            if (canopen_Dictionary_add_entry(this, this->map, entry)) {
                corto_throw("Failed to add entry to pdo dictionary.");
                goto error;
            }

        }

    }

    corto_dealloc(from);
    return 0;
error:
    corto_dealloc(from);
    return -1;
}

int16_t canopen_Dictionary_add_entry(
    canopen_Dictionary this,
    canopen_IndexMap map,
    canopen_Entry entry)
{
    if (corto_instanceof((corto_type)canopen_Entry_o, entry) == true) {
        uint32_t index = canopen_Dictionary_encode_index(
            entry->meta->index,
            entry->meta->subindex);
        if (!corto_rb_find(this->map, &index)) {
            corto_rb_set(this->map, (void *)(intptr_t)index, entry);
        } else {
            /* Index exists */
        }
    } else {
        corto_error("Not an ENTRY.");
    }

    return 0;
}

canopen_Entry canopen_Dictionary_lookup(
    canopen_Dictionary this,
    uint16_t index,
    uint16_t sub)
{
    if (!this) {
        corto_throw("Invalid dictionary handle.");
        goto error;
    }

    uint32_t encoded = canopen_Dictionary_encode_index(
        index,
        sub);

    canopen_Entry result = (canopen_Entry)corto_rb_find(
        this->map,
        (void *)(intptr_t)encoded);

    if (!result) {
        corto_throw("Entry not found for index [%d]:[%d] = [%lu]",
            index, sub, encoded);
        goto error;
    }

    return result;
error:
    return NULL;
}

int16_t canopen_Dictionary_traverse(
    canopen_Dictionary this,
    uintptr_t callback,
    uintptr_t data)
{
    if (!this) {
        corto_throw("Invalid dictionary handle.");
        goto error;
    }

    corto_rb_walk(this->map, (corto_elementWalk_cb)callback, (void*)data);
    return 0;
error:
    return -1;
}

uint32_t canopen_Dictionary_encode_index(
    uint16_t index,
    uint16_t sub)
{
    return (index * CANOPEN_DICTIONARY_SUB_MAX) + sub;
}
