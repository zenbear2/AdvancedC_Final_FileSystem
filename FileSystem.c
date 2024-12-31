#include "FileSystem.h"

void initialize_file_system(FileSystem *fs) {
    memset(fs, 0, sizeof(FileSystem));
    for (int i = 0; i < MAX_BLOCKS; i++) {
        fs->block_bitmap[i] = false; // All blocks are free
    }
    for (int i = 0; i < MAX_INODES; i++) {
        fs->inode_bitmap[i] = false; // All inodes are free
    }
}

int allocate_block(FileSystem *fs) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!fs->block_bitmap[i]) {
            fs->block_bitmap[i] = true; // Mark block as used
            return i;                   // Return block index
        }
    }
    return -1; // No free blocks
}

void free_block(FileSystem *fs, int block_index) {
    if (block_index >= 0 && block_index < MAX_BLOCKS) {
        fs->block_bitmap[block_index] = false; // Mark block as free
    }
}

int allocate_inode(FileSystem *fs) {
    for (int i = 0; i < MAX_INODES; i++) {
        if (!fs->inode_bitmap[i]) {
            fs->inode_bitmap[i] = true; // Mark inode as used
            return i;                   // Return inode index
        }
    }
    return -1; // No free inodes
}

void free_inode(FileSystem *fs, int inode_index) {
    if (inode_index >= 0 && inode_index < MAX_INODES) {
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

