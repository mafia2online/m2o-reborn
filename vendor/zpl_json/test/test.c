#define ZPL_IMPLEMENTATION
#define ZPLJ_IMPLEMENTATION
#include <zpl.h>
#include <zpl_json.h>

int main(void) {

    zpl_file_t file;
    zpl_file_open(&file, "test.json");
    isize file_size = zpl_file_size(&file);
    char *content = zpl_malloc(file_size+1);
    zpl_file_read(&file, content, file_size);
    content[file_size] = 0;
    zpl_file_close(&file);


    zplj_object_t root = {0};

    zplj_parse(&root, file_size, content, zpl_heap_allocator(), true);

    zplj_free(&root);

    zpl_mfree(content);
    return 0;
}
