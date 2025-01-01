#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define BLOCK_SIZE 4096   // Size of each block
#define MAX_FILENAME 255  // Maximum filename length
#define DIRECT_POINTERS 12 // Number of direct pointers in an inode
#define MAX_DIR_ENTRIES 16 // Maximum entries in a single directory
#define INODE_BLOCK_RATIO 4


// Block structure
typedef struct Page{
    uint8_t data[BLOCK_SIZE];
} Block;

typedef struct {
    uint32_t inode_index;      // Index of the child inode
    char name[MAX_FILENAME];  // Name of the file or subdirectory
} DirectoryEntry;

// Inode structure
typedef struct {
    uint32_t size;                   // File size in bytes (not applicable for directories)
    uint32_t blocks[DIRECT_POINTERS]; // Direct block pointers
    uint32_t indirect_block;         // Pointer to indirect block (if implemented)
    uint16_t permissions;            // File permissions
    uint32_t creation_time;          // File creation timestamp
    uint32_t modification_time;      // Last modification timestamp
    bool is_directory;               // True if this is a directory
    uint32_t dir_entry_count;        // Number of entries (only for directories)
    DirectoryEntry entries[MAX_DIR_ENTRIES]; // Entries in the directory
    char filename[MAX_FILENAME];     // Filename or directory name
} Inode;


// File system metadata
typedef struct file_manager{
    bool *block_bitmap;   // Dynamic array to track free/used blocks
    bool *inode_bitmap;   // Tracks free/used inodes
    Block *blocks;        // Dynamically allocated array of blocks
    Inode *inodes;
    uint32_t total_blocks;    // Total number of blocks
    uint32_t total_inodes;    // Total number of inodes
} FileSystem;


void initialize_file_system(FileSystem *fs, uint32_t num_blocks);
void cleanup_file_system(FileSystem *fs);

int allocate_block(FileSystem *fs) ;
void free_block(FileSystem *fs, int block_index);
int allocate_inode(FileSystem *fs);
void free_inode(FileSystem *fs, int inode_index);

void write_to_file(FileSystem *fs, int inode_index, const uint8_t *data, uint32_t size);
void read_from_file(FileSystem *fs, int inode_index, uint8_t *buffer, uint32_t size);

int create_directory(FileSystem *fs, const char *name);
int add_to_directory(FileSystem *fs, int dir_inode_index, int child_inode_index, const char *name);
void list_directory(FileSystem *fs, int dir_inode_index);

int read_file_to_fs(FileSystem *fs, const char *external_filename, const char *internal_filename);
int write_file_to_host(FileSystem *fs, int inode_index, const char *external_filename);