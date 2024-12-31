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

    // Add the file to the root directory
    add_to_directory(&fs, root_inode, file_inode, "file1.txt");

    // Create a subdirectory
    int subdir_inode = create_directory(&fs, "subdir");
    add_to_directory(&fs, root_inode, subdir_inode, "subdir");

    // Add a file to the subdirectory
    int file2_inode = allocate_inode(&fs);
    strncpy(fs.inodes[file2_inode].filename, "file2.txt", MAX_FILENAME);
    fs.inodes[file2_inode].is_directory = false;
    add_to_directory(&fs, subdir_inode, file2_inode, "file2.txt");

    // List root directory contents
    list_directory(&fs, root_inode);

    // List subdirectory contents
    list_directory(&fs, subdir_inode);



    // Print the total number of blocks
    printf("Total blocks: %u\n", fs.total_blocks);


    // Clean up
    cleanup_file_system(&fs);
    return 0;
}

// int main() {
//     FileSystem fs;
//     initialize_file_system(&fs);

//     // Create a root directory
//     int root_inode = create_directory(&fs, "root");

//     // Create a file
//     int file_inode = allocate_inode(&fs);
//     strncpy(fs.inodes[file_inode].filename, "file1.txt", MAX_FILENAME);
//     fs.inodes[file_inode].is_directory = false;

//     // Add the file to the root directory
//     add_to_directory(&fs, root_inode, file_inode, "file1.txt");

//     // Create a subdirectory
//     int subdir_inode = create_directory(&fs, "subdir");
//     add_to_directory(&fs, root_inode, subdir_inode, "subdir");

//     // Add a file to the subdirectory
//     int file2_inode = allocate_inode(&fs);
//     strncpy(fs.inodes[file2_inode].filename, "file2.txt", MAX_FILENAME);
//     fs.inodes[file2_inode].is_directory = false;
//     add_to_directory(&fs, subdir_inode, file2_inode, "file2.txt");

//     // List root directory contents
//     list_directory(&fs, root_inode);

//     // List subdirectory contents
//     list_directory(&fs, subdir_inode);

//     return 0;
// }
