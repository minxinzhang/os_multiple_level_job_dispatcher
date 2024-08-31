/* Include Files */
#include "mab.h"

/*******************************************************
 * PcbPtr createMab(int offset, int size) 
 *  - create a memory block of `size` 
 *  - mem address starts from `offset`.
 *
 * returns:
 *    The pointer of the memory block alloc'ed
 *    NULL if malloc failed
 ******************************************************/
MabPtr createMab(int offset, int size)
{   MabPtr newMab = (MabPtr)malloc(sizeof(Mab));
    if (newMab == NULL) {
        fprintf(stderr, "ALERT: Memory allocation failed\n");
        exit(1);
    }
    newMab->offset = offset;
    newMab->size = size;
    newMab->allocated = FALSE;
    newMab->parent = NULL;
    newMab->left_child = NULL;
    newMab->right_child = NULL;
    return newMab;
}

//debug helper
void printMabInfo(MabPtr m) {
    if (!m) {
        printf("not allocated\n");
        return;
    }
    printf("offset: %d size: %d allocated: %d parent: ", m -> offset, m -> size, m -> allocated);
    if (m -> parent) printf("yes"); else printf("no");
    printf(" left child: ");
    if (m -> left_child) printf("yes"); else printf("no");
    printf(" right child: ");
    if (m -> right_child) printf("yes"); else printf("no"); 
    printf("\n");
}

/*******************************************************
 * MabPtr memMerge(MabPtr m) 
 * - recursively merge free memory blocks 
 * - bottom up from `m`
 *
 * returns:
 * the pointer of the freed memory block with 
 * the highest order
 ******************************************************/
MabPtr memMerge(MabPtr m) {
    // `m` is the root, no further merging can be done
    if (m->parent == NULL) {
        return m; 
    }

    //set `m`'s sibling
    MabPtr buddy;
    if (m == m->parent->left_child) {
        buddy = m->parent->right_child;
    } else {
        buddy = m->parent->left_child;
    }

    // buddy block is free so merge `m` and `buddy`
    if (buddy->allocated == FALSE && !buddy->left_child && !buddy->right_child) {
        // free `buddy` and `m` memory
        free(buddy);
        free(m);
        //delete the sibling nodes
        m->parent->left_child = NULL;
        m->parent->right_child = NULL;
        // attempt to merge higher order blocks
        return memMerge(m->parent);
    }

    return m;
}; 

/*******************************************************
 * MabPtr memSplit(MabPtr m, int size) 
 * - split `m` 
 * - top down search recursively
 * - left child is prefered
 * 
 * returns:
 * - NULL if not possible
 * - the memory block pointer if a mem block could
 * wrap `size`, i.e. mab->size / 2 <= size <= mab->size
 ******************************************************/
MabPtr memSplit(MabPtr m, int size) {

    //termination criteria

    //in use
    if (m->allocated) return NULL;
    //free but need to check its descendents
    if (m->size / 2 < size) {
        //it's already split thus there must be
        //a descendent alloc'ed
        if (m -> left_child) return NULL;
        //not alloc'ed and can't split further
        else return m;
    }

    if (m->left_child != NULL) {
        // This block is already split, go down the tree
        MabPtr leftSplit = memSplit(m->left_child, size);
        if (leftSplit != NULL) {
            return leftSplit;
        }
        return memSplit(m->right_child, size);
    }

    // not split
    if (m->size / 2 >= size) {
        // Split the memory block
        m->left_child = createMab(m->offset, m->size / 2);
        m->left_child->parent = m;
        m->right_child = createMab(m->offset + m->size / 2, m->size / 2);
        m->right_child->parent = m;
        return memSplit(m->left_child, size);
    }

    return NULL;
}; // split a memory block

/*******************************************************
 * MabPtr memAlloc(MabPtr memBlock) 
 *      - allocate a memory block
 *
 * returns:
 *    MabPtr of newly initialised Mab
 *    NULL if malloc failed
 ******************************************************/
MabPtr memAlloc(MabPtr m, int size) {
    MabPtr freeBlock;
    if (size < MAB_MIN) freeBlock = memSplit(m,MAB_MIN);
    else freeBlock = memSplit(m, size);
    if (freeBlock != NULL) {
        freeBlock->allocated = TRUE;
        return freeBlock;
    } else {
        return NULL; // No suitable block found
    }
};

/*******************************************************
 * MabPtr memFree(MabPtr m) 
 *      - free the memory block `m`
 *
 * returns:
 *    a free block with the possible highest order
 ******************************************************/
// Free memory block
MabPtr memFree(MabPtr m) {
    // `m` is not allocated
    // there's nothing to free
    if (m->allocated == FALSE) {
        return NULL;
    }

    // `m` is free now
    m->allocated = FALSE;
    // try to merge free blocks
    return memMerge(m);
};