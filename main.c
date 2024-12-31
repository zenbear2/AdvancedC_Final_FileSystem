#include "FileSystem.h"

int main() {

    printf("Hello, FileSystem!\n");
    FileSystem fs;
    printf("Initializing file system...\n");
    initialize_file_system(&fs);

    int inode_index = allocate_inode(&fs);
    if (inode_index == -1) {
        printf("No free inodes available!\n");
        return 1;
    }

    const char *data = "Hello, FileSystem!";
    write_to_file(&fs, inode_index, (const uint8_t *)data, strlen(data));
    printf("index: %d\n", inode_index);
    printf("lenght of data: %d\n", strlen(data));

    uint8_t buffer[BLOCK_SIZE] = {0};
    read_from_file(&fs, inode_index, buffer, strlen(data));
    printf("Read from file: %s\n", buffer);

    free_inode(&fs, inode_index);
    return 0;
}