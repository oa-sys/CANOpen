/* This is a managed file. Do not delete this comment. */

// TODO Add config observer
#include <driver/mnt/canopen/canopen.h>
/* Default object compare function used as tree comparator in scope rbtree */
int16_t canopen_Dictionary_construct(
    canopen_Dictionary this)
{
    if (!this->from) {
        goto error;
    }

    if (!this->map) {
        this->map = corto_create(NULL, NULL, canopen_IndexMap_o);
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
        corto_result *r = (corto_result*)corto_iter_next(&it);
        corto_string path = corto_asprintf("%s/%s", from, r->id);
        canopen_Entry entry = (canopen_Entry)corto_lookup(root_o, path);
        corto_dealloc(path);
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
        corto_result *r = (corto_result*)corto_iter_next(&it);
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
        canopen_SubIndexMap sub = (canopen_SubIndexMap)corto_rb_find(
            map, &entry->meta->index);
        corto_info("--- Add [%d:%d]",entry->meta->index, entry->meta->subindex);
        if (!sub) {
            /* Create new IndexMap entry for register Index */
            sub = (canopen_SubIndexMap)corto_create(NULL, NULL, canopen_SubIndexMap_o);
            corto_info("1. Pre Sub Count: [%d]", corto_rb_count(sub));
            corto_info("1. Pre Map Count: [%d]", corto_rb_count(map));
            corto_rb_set(sub, &entry->meta->subindex, &entry);
            corto_rb_set(map, &entry->meta->index, &sub);
            corto_info("1. Post Sub Count: [%d]", corto_rb_count(sub));
            corto_info("1. Post Map Count: [%d]", corto_rb_count(map));
        } else {
            if (!corto_rb_find(sub, &entry->meta->subindex)) {
                /* Create new SubIndexMap entry for register subindex */
                corto_info("2: Pre Sub Count: [%d]", corto_rb_count(sub));
                corto_rb_set(sub, &entry->meta->subindex, &entry);
                corto_info("2: Post Sub Count: [%d]", corto_rb_count(sub));
            } else {
                /* SubIndex exists - Decrease refcount on mapped entry */
                corto_release(entry);
            }
        }

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

    corto_info("Lookup Sub Map [%d] Map Size [%lu]", index, corto_rb_count(this->map));
    corto_rb map = (corto_rb)corto_rb_find(this->map, &index);

    if (!map) {
        corto_throw("Sub index not set for index [%d]", index);
        goto error;
    }

    corto_info("Lookup [%d]:[%d] Map Size [%lu]", index, sub, corto_rb_count(map));
    canopen_Entry result = (canopen_Entry)corto_rb_find(map, &sub);

    if (!result) {
        corto_throw("Entry not found for index [%d]:[%d]", index, sub);
        goto error;
    }

    return result;
error:
    return NULL;
}

int16_t canopen_Dictionary_traverse(
    canopen_Dictionary this,
    uint16_t index,
    uintptr_t callback,
    uintptr_t data)
{
    if (!this) {
        corto_throw("Invalid dictionary handle.");
        goto error;
    }

    canopen_SubIndexMap map = (canopen_SubIndexMap)corto_rb_find(
        this->map,
        &index);

    if (!map) {
        corto_throw("Sub index not set for index [%d]", index);
        goto error;
    }

    corto_rb_walk(map, (corto_elementWalk_cb)callback, (void*)data);

    return 0;
error:
    return -1;
}
