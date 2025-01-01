#include "FileSystem.h"

int main() {
    FileSystem fs;
    uint32_t num_blocks;

    // Ask user for the number of blocks
    printf("Enter the number of blocks for the file system: ");
    if (scanf("%u", &num_blocks) != 1 || num_blocks == 0) {
        printf("Invalid input! Number of blocks must be a positive integer.\n");
        return 1;
    }

    // Initialize file system
    initialize_file_system(&fs, num_blocks);

    // Create a root directory
    int root_inode = create_directory(&fs, "root");

    // Create a file
    int file_inode = allocate_inode(&fs);
    strncpy(fs.inodes[file_inode].filename, "file1.txt", MAX_FILENAME);
    fs.inodes[file_inode].is_directory = false;

    // Write some data to the file
    char *data = "Hello, FileSystem! This is a test file. In file1.txt";
    write_to_file(&fs, file_inode, (const uint8_t *)data, strlen(data));

    // Add the file to the root directory
    add_to_directory(&fs, root_inode, file_inode, "file1.txt");

    //Read from the file
    uint8_t buffer[BLOCK_SIZE] = {0};
    read_from_file(&fs, file_inode, buffer, strlen(data));
    printf("\n--------------------------Test context begin-------------------------\n\n");
    printf("Read from file: %s\n", buffer);
    printf("\n---------------------------Test context end--------------------------\n\n\n");

    // Create a subdirectory
    int subdir_inode = create_directory(&fs, "subdir");
    add_to_directory(&fs, root_inode, subdir_inode, "subdir");

    // Add a file to the subdirectory
    int file2_inode = allocate_inode(&fs);
    strncpy(fs.inodes[file2_inode].filename, "file2.txt", MAX_FILENAME);
    fs.inodes[file2_inode].is_directory = false;
    add_to_directory(&fs, subdir_inode, file2_inode, "file2.txt");


    // Write data from an external file to the file system
    int file3_inode = read_file_to_fs(&fs, "test_text.txt", "file3.txt");
    add_to_directory(&fs, subdir_inode, file3_inode, "file3.txt");
    read_from_file(&fs, file3_inode, buffer, fs.inodes[file3_inode].size);
    printf("\n--------------------------Test context begin-------------------------\n\n");
    printf("Read from file: %s\n", buffer);
    printf("\n---------------------------Test context end--------------------------\n\n");


    int total_write = write_file_to_host(&fs, file3_inode, "file3.txt"); 


    // List root directory contents
    list_directory(&fs, root_inode);

    // List subdirectory contents
    list_directory(&fs, subdir_inode);


    // Clean up
    cleanup_file_system(&fs);
    return 0;
}
