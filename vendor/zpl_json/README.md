# ZPL - JSON5 parser module
[![npm version](https://badge.fury.io/js/zpl_json.c.svg)](https://badge.fury.io/js/zpl_json.c)

A simple module used for parsing JSON5 files. It offers a simple interface that is JSON and JSON5-standard friendly.
Thanks to the "at place" approach of parsing JSON tokens, the parser offers high speed advantage and low memory footprint.

## Usage
```c
#define ZPL_IMPLEMENTATION
#define ZPLJ_IMPLEMENTATION
#include <zpl.h>
#include <zpl_json.h>

#define source                          \
    "{\n"                               \
    "   hostname: \"my m2o server\",\n" \
    "   password: \"\",\n"              \
    "   port: 27010,\n\n"               \
                                        \
    "   /* settings for amounts */\n"   \
    "   max_entities: 16384,\n"         \
    "   max_connections: 128,\n"        \
    "   resources: [],\n"               \
    "}\n"

zplj_object_t *data_get(zplj_object_t *root, char *name) {
    for (isize i = 0; i < zpl_array_count(root->nodes); ++i) {
        zplj_object_t *node = (root->nodes + i);
        if (node && !zpl_strcmp(node->name, name)) {
            return node;
        }
    }
    return NULL;
}

int main(void) {

    zplj_object_t root = {0};

    // copy the source to heap
    isize di = zpl_strlen(source);
    char *d = zpl_malloc(di+1);
    zpl_strncpy(d, source, di);
    d[di] = 0;

    // parse JSON5 document, comment strip enabled
    u8 error_code;
    zplj_parse(&root, di, d, zpl_heap_allocator(), true, &error_code);

    // example usage
    zplj_object_t *hostname = data_get(&root, "hostname");
    ZPL_ASSERT(hostname);
    zpl_printf("The hostname is: %s\n", hostname->string);

    // free the resources
    zplj_free(&root);
    zpl_mfree(d);

    return 0;
}
```

See test/ folder for more examples.