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

    if (canopen_Dictionary_fill_device(this)) {
        corto_throw("Failed to map device info configuration.");
        goto error;
    }

    if (canopen_Dictionary_fill_pdo(this)) {
        corto_throw("Failed to map pdo configuration.");
        goto error;
    }
    
    return 0;
error:
    return -1;
}

static
corto_equalityKind corto_compareInt(
    corto_type this,
    const void* o1,
    const void* o2)
{
    corto_assert_object(this);
    CORTO_UNUSED(this);

    uint32_t lhs = *(uint32_t*)o1;
    uint32_t rhs = *(uint32_t*)o2;
    if (lhs == rhs) {
        return CORTO_EQ;
    } else if (lhs < rhs) {
        return CORTO_LT;
    } else {
        return CORTO_GT;
    }

    return CORTO_NEQ;
}

int16_t canopen_Dictionary_fill_device(
    canopen_Dictionary this)
{
    if (!this->device) {
        this->device = corto_rb_new((corto_equals_cb)corto_compareInt, NULL);
    }

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
            if (canopen_Dictionary_add_entry(this, this->device, entry)) {
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
    if (!this->pdo) {
        this->pdo = corto_rb_new((corto_equals_cb)corto_compareInt, NULL);
    }

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
            if (canopen_Dictionary_add_entry(this, this->pdo, entry)) {
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
        if (!sub) {
            /* Create new IndexMap entry for register Index */
            sub = corto_rb_new((corto_equals_cb)corto_compareInt, NULL);
            corto_rb_set(map, &entry->meta->index, &sub);
            corto_rb_set(sub, &entry->meta->subindex, &entry);
        } else {
            if (!corto_rb_find(sub, &entry->meta->subindex)) {
                /* Create new SubIndexMap entry for register subindex */
                corto_rb_set(sub, &entry->meta->subindex, &entry);
            } else {
                /* Notify of Update (future) - SubIndex exists
                   Decrease refcount on previously mapped entry */
                corto_release(entry);
            }
        }
    }

    return 0;
}
