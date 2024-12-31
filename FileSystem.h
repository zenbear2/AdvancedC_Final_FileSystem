#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define BLOCK_SIZE 4096   // Size of each block
#define MAX_BLOCKS 1024   // Maximum number of blocks
#define MAX_INODES 128    // Maximum number of inodes
#define MAX_FILENAME 255  // Maximum filename length
#define DIRECT_POINTERS 12 // Number of direct pointers in an inode

// Block structure
typedef struct Page{
    uint8_t data[BLOCK_SIZE];
} Block;

// Inode structure
typedef struct metadata{
    uint32_t size;                   // File size in bytes
    uint32_t blocks[DIRECT_POINTERS]; // Direct block pointers
    uint32_t indirect_block;         // Pointer to indirect block
    uint16_t permissions;            // File permissions
    uint32_t creation_time;          // File creation timestamp
    uint32_t modification_time;      // Last modification timestamp
    bool is_directory;               // Is this inode a directory?
    char filename[MAX_FILENAME];     // Filename
} Inode;

// File system metadata
typedef struct file_manager{
    bool block_bitmap[MAX_BLOCKS];   // Tracks free/used blocks
    bool inode_bitmap[MAX_INODES];   // Tracks free/used inodes
    Block blocks[MAX_BLOCKS];        // Array of all blocks
    Inode inodes[MAX_INODES];        // Array of all inodes
} FileSystem;


void initialize_file_system(FileSystem *fs);

int allocate_block(FileSystem *fs) ;

void free_block(FileSystem *fs, int block_index);

int allocate_inode(FileSystem *fs);

void free_inode(FileSystem *fs, int inode_index);

void write_to_file(FileSystem *fs, int inode_index, const uint8_t *data, uint32_t size);

void read_from_file(FileSystem *fs, int inode_index, uint8_t *buffer, uint32_t size);