#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    char baseName[64];
    char dirName[64];
    struct NODE *parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }
    if (baseName[0] == '\0') {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE *last = NULL;
    for (struct NODE *child = parent->childPtr; child != NULL;
         child = child->siblingPtr) {
        if (strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        last = child;
    }

    struct NODE *node = malloc(sizeof(*node));
    if (node == NULL) {
        fprintf(stderr, "MKDIR ERROR: unable to allocate memory\n");
        return;
    }
    strcpy(node->name, baseName);
    node->fileType = 'D';
    node->parentPtr = parent;
    node->childPtr = NULL;
    node->siblingPtr = NULL;
    if (last == NULL) {
        parent->childPtr = node;
    } else {
        last->siblingPtr = node;
    }
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    char *lastSlash = strrchr(pathName, '/');
    if (lastSlash == NULL) {
        dirName[0] = '\0';
        strcpy(baseName, pathName);
    } else {
        size_t length = lastSlash - pathName;
        memcpy(dirName, pathName, length);
        dirName[length] = '\0';
        strcpy(baseName, lastSlash + 1);
    }

    struct NODE *parent = pathName[0] == '/' ? root : cwd;
    // Tokenize a copy so the caller keeps the complete directory path.
    char directories[64];
    strcpy(directories, dirName);
    char *part = strtok(directories, "/");
    while (part != NULL) {
        struct NODE *child = parent->childPtr;
        while (child != NULL &&
               (strcmp(child->name, part) != 0 || child->fileType != 'D')) {
            child = child->siblingPtr;
        }
        if (child == NULL) {
            printf("ERROR: directory %s does not exist\n", part);
            return NULL;
        }
        parent = child;
        part = strtok(NULL, "/");
    }
    return parent;
}
