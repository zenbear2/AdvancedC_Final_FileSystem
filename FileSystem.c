#include "FileSystem.h"

void initialize_file_system(FileSystem *fs, uint32_t num_blocks) {
    fs->total_blocks = num_blocks;
    int num_inode = num_blocks / INODE_BLOCK_RATIO;
    fs->total_inodes = num_inode;

    printf("Total blocks: %d\n", num_blocks);
    // Allocate memory for the block bitmap and blocks
    fs->block_bitmap = (bool *)malloc(num_blocks * sizeof(bool));
    if (fs->block_bitmap == NULL) {
        printf("Memory allocation for block bitmap failed!\n");
        exit(1);
    }

    fs->blocks = (Block *)malloc(num_blocks * sizeof(Block));
    if (fs->blocks == NULL) {
        printf("Memory allocation for blocks failed!\n");
        exit(1);
    }

    // Allocate memory for the inode bitmap and inodes
    fs->inode_bitmap = (bool *)malloc(num_blocks * sizeof(bool));
    if (fs->block_bitmap == NULL) {
        printf("Memory allocation for inode bitmap failed!\n");
        exit(1);
    }

    fs->inodes = (Inode *)malloc(num_blocks * sizeof(Inode));
    if (fs->block_bitmap == NULL) {
        printf("Memory allocation for inodes failed!\n");
        exit(1);
    }



    if (fs->block_bitmap == NULL || fs->blocks == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Initialize block bitmap
    printf("Initialize block bitmap !\n");
    for (uint32_t i = 0; i < num_blocks; i++) {
        fs->block_bitmap[i] = false;
    }

    // Initialize inode bitmap
    printf("Initialize inode bitmap !\n");
    for (int i = 0; i < num_inode; i++) {
        fs->inode_bitmap[i] = false;
    }
    printf("File System Memory allocated\n");
}

void cleanup_file_system(FileSystem *fs) {
    free(fs->block_bitmap);
    free(fs->blocks);
}


int allocate_block(FileSystem *fs) {
    for (uint32_t i = 0; i < fs->total_blocks; i++) {
        if (!fs->block_bitmap[i]) {
            fs->block_bitmap[i] = true; // Mark block as used
            return i;                   // Return block index
        }
    }
    return -1; // No free blocks
}

void free_block(FileSystem *fs, int block_index) {
    if (block_index >= 0 && block_index < fs->total_blocks) {
        fs->block_bitmap[block_index] = false; // Mark block as free
    }
}

int allocate_inode(FileSystem *fs) {
    for (int i = 0; i < fs->total_inodes; i++) {
        if (!fs->inode_bitmap[i]) {
            fs->inode_bitmap[i] = true; // Mark inode as used
            return i;                   // Return inode index
        }
    }
    return -1; // No free inodes
}

void free_inode(FileSystem *fs, int inode_index) {
    if (inode_index >= 0 && inode_index < fs->total_inodes) {
        fs->inode_bitmap[inode_index] = false; // Mark inode as free
    }
}

void write_to_file(FileSystem *fs, int inode_index, const uint8_t *data, uint32_t size) {
    Inode *inode = &fs->inodes[inode_index];
    uint32_t remaining = size;
    uint32_t block_index;

    for (int i = 0; i < DIRECT_POINTERS && remaining > 0; i++) {
        if (inode->blocks[i] == 0) {
            block_index = allocate_block(fs);
            if (block_index == -1) {
                printf("No free blocks available!\n");
                return;
            }
            inode->blocks[i] = block_index;
        }

        // Write data to block
        uint32_t to_write = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
        memcpy(fs->blocks[inode->blocks[i]].data, data, to_write);
        if (remaining <= BLOCK_SIZE) {
            fs->blocks[inode->blocks[i]].data[to_write] = '\0'; // Null-terminate
        }
        data += to_write;
        remaining -= to_write;
    }

    inode->size = size; // Update file size
}


void read_from_file(FileSystem *fs, int inode_index, uint8_t *buffer, uint32_t size) {
    Inode *inode = &fs->inodes[inode_index];
    uint32_t remaining = size;
    uint32_t block_index;

    for (int i = 0; i < DIRECT_POINTERS && remaining > 0; i++) {
        if (inode->blocks[i] == 0) {
            printf("Block not allocated! i=%d\n",i);
            //return;
        }

        // Read data from block
        uint32_t to_read = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
        memcpy(buffer, fs->blocks[inode->blocks[i]].data, to_read);
        buffer += to_read;
        remaining -= to_read;
    }

    *buffer = '\0'; // Null-terminate the read data
}

int create_directory(FileSystem *fs, const char *name) {
    int inode_index = allocate_inode(fs);
    if (inode_index == -1) {
        printf("No free inodes available!\n");
        return -1;
    }

    Inode *inode = &fs->inodes[inode_index];
    inode->is_directory = true;
    inode->dir_entry_count = 0;
    strncpy(inode->filename, name, MAX_FILENAME);
    inode->size = 0; // Directory size is 0 (data stored in entries)
    return inode_index;
}

int add_to_directory(FileSystem *fs, int dir_inode_index, int child_inode_index, const char *name) {
    Inode *dir_inode = &fs->inodes[dir_inode_index];
    if (!dir_inode->is_directory) {
        printf("Inode is not a directory!\n");
        return -1;
    }

    if (dir_inode->dir_entry_count >= MAX_DIR_ENTRIES) {
        printf("Directory is full!\n");
        return -1;
    }

    // Add a new entry
    DirectoryEntry *entry = &dir_inode->entries[dir_inode->dir_entry_count];
    entry->inode_index = child_inode_index;
    strncpy(entry->name, name, MAX_FILENAME);

    dir_inode->dir_entry_count++;
    return 0;
}

void list_directory(FileSystem *fs, int dir_inode_index) {
    Inode *dir_inode = &fs->inodes[dir_inode_index];
    if (!dir_inode->is_directory) {
        printf("Inode is not a directory!\n");
        return;
    }

    printf("Contents of directory '%s':\n", dir_inode->filename);
    for (uint32_t i = 0; i < dir_inode->dir_entry_count; i++) {
        DirectoryEntry *entry = &dir_inode->entries[i];
        Inode *child = &fs->inodes[entry->inode_index];
        printf("  %s (inode %d, %s)\n", entry->name, entry->inode_index,
               child->is_directory ? "directory" : "file");
    }
}

