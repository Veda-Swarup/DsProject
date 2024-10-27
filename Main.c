#include<stdio.h>
#include<string.h>
#include<stdlib.h>
enum nodeType {File, Folder};
typedef struct node{
    enum nodeType type;
    char* name;
    int numberOfItems;
    size_t size;
    char* content;
    struct node* previous;
    struct node* parent;
    struct node* next;
    struct node* child;
}node;
char *getString(){
    size_t size = 10;
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(stdin)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

node* getNode(node *currentFolder, char* name, enum nodeType type) {

    if (currentFolder->child != NULL) {

        node *currentNode = currentFolder->child;

        while (currentNode->next != NULL) {

            if (strcmp(name, currentNode->name) == 0 && currentNode->type == type) {
                return currentNode;
            }
            currentNode = currentNode->next;
        }

        if (strcmp(name, currentNode->name) == 0 && currentNode->type == type) {
            return currentNode;
        } else return NULL;

    } else return NULL;
}

node* getNodeTypeless(node *currentFolder, char* name) {

    if (currentFolder->child != NULL) {

        node *currentNode = currentFolder->child;

        while (currentNode != NULL) {

            if (strcmp(name, currentNode->name) == 0) {
                return currentNode;
            }
            currentNode = currentNode->next;
        }

    } else return NULL;
}

void mkdir(node *currentFolder, char *command) {
    if (strtok(command, " ") != NULL) {
        char* folderName = strtok(NULL, " ");
        if (folderName != NULL) {
            if (getNodeTypeless(currentFolder, folderName) == NULL) {

                currentFolder->numberOfItems++;
                node *newFolder = (node*) malloc(sizeof(node));

                if (currentFolder->child == NULL) {
                    currentFolder->child = newFolder;
                    newFolder->previous = NULL;
                    newFolder->parent = currentFolder;
                } else {

                    node *currentNode = currentFolder->child;

                    while (currentNode->next != NULL) {
                        currentNode = currentNode->next;
                    }
                    currentNode->next = newFolder;
                    newFolder->previous = currentNode;
                    newFolder->parent = NULL;
                }

                char* newFolderName = (char*) malloc(sizeof(char)*(strlen(folderName)+1));
                strcpy(newFolderName, folderName);

                newFolder->name = newFolderName;
                newFolder->type = Folder;
                newFolder->numberOfItems = 0;
                newFolder->size = 0;
                newFolder->content = NULL;
                newFolder->next = NULL;
                newFolder->child = NULL;

                printf("Folder '%s' added\n", newFolder->name);
            } else {
                fprintf(stderr, "'%s' is already exist in current directory!\n",  folderName);
            }
        }
    }
}

void mkfile(node *currentFolder, char *command) {

    if (strtok(command, " ") != NULL) {
        char *fileName = strtok(NULL, " ");
        if (fileName != NULL) {
            if (getNodeTypeless(currentFolder, fileName) == NULL) {

                currentFolder->numberOfItems++;

                node *newFile = (node *) malloc(sizeof(node));

                if (currentFolder->child == NULL) {
                    currentFolder->child = newFile;
                    newFile->previous = NULL;
                    newFile->parent = currentFolder;
                } else {

                    node *currentNode = currentFolder->child;

                    while (currentNode->next != NULL) {
                        currentNode = currentNode->next;
                    }
                    currentNode->next = newFile;
                    newFile->previous = currentNode;
                    newFile->parent = NULL;
                }

                char* newFileName = (char*) malloc(sizeof(char)*(strlen(fileName)+1));
                strcpy(newFileName, fileName);

                newFile->name = newFileName;
                newFile->type = File;
                newFile->numberOfItems = 0;
                newFile->size = 0;
                newFile->content = NULL;
                newFile->next = NULL;
                newFile->child = NULL;

                printf("File '%s' added\n", newFile->name);
            } else {
                fprintf(stderr, "'%s' is already exist in current directory!\n", fileName);
            }
        }
    }
}

void edit(node *currentFolder, char *command) {

    if (strtok(command, " ") != NULL) {
        char *fileName = strtok(NULL, " ");
        if (fileName != NULL) {
            node * editingNode = getNode(currentFolder, fileName, File);
            if (editingNode != NULL) {
                printf("%s\n", "Enter text to overwrite the file content:\n");
                if (editingNode->content != NULL){
                    free(editingNode->content);
                }
                char* content = getString();
                if (content != NULL) {
                    editingNode->content = content;
                    editingNode->size = strlen(editingNode->content);
                }
            }
        }
        else printf("Flie doesn't exist");
    }
}

void ls(node *currentFolder) {
    if (currentFolder->child == NULL) {
        printf("Empty Folder\n");
    } else {

        node *currentNode = currentFolder->child;

        while (currentNode->next != NULL) {

            
           
                printf("%s\t",  currentNode->name);

            currentNode = currentNode->next;
        }

        printf("\n");
    }
}

node* cd(node *currentFolder, char *command, char **path) {

    if (strtok(command, " ") != NULL) {
        char* folderName = strtok(NULL, " ");
        if (folderName != NULL) {

            node *destinationFolder = getNode(currentFolder, folderName, Folder);

            if ( destinationFolder != NULL) {

                size_t newPathLength = strlen(*path) + strlen(destinationFolder->name) + 2;

                *path = (char *) realloc(*path, sizeof(char)* newPathLength);

                strcat(strcat(*path, destinationFolder->name), "/");

                return destinationFolder;
            } else {
                fprintf(stderr, "There is no '%s' folder in current directory!\n",  folderName);
                return currentFolder;
            }
        } else {
            return currentFolder;
        }
    } else {
        return currentFolder;
    }
}

void pwd(char *path) {
    if (strlen(path) != 1){

        for (int i = 0; i < strlen(path)-1 ; ++i) {
            printf("%c", path[i]);
        }
        printf("\n");
    } else {
        printf("%s\n", path);
    }
}
int main() {

    node *root = (node*) malloc(sizeof(node));

    char *rootName = (char *) malloc(sizeof(char)*2);
    strcpy(rootName, "/");
    root->type = Folder;
    root->name = rootName;
    root->numberOfItems = 0;
    root->size = 0;
    root->content =NULL;
    root->previous = NULL;
    root->parent = NULL;
    root->next = NULL;
    root->child = NULL;

    node *currentFolder = root;

    char *path = (char *) malloc(sizeof(char)*2);
    strcpy(path, "/");

    while (1) {

        printf("> ");
        char *command = getString();

        if (strncmp(command, "mkdir", 5) == 0) {
            mkdir(currentFolder, command);
        } else if (strncmp(command, "mkflie", 6) == 0) {
            mkfile(currentFolder, command);
        } else if (strcmp(command, "ls") == 0) {
            ls(currentFolder);
        }  else if (strncmp(command, "edit", 4) == 0 ) {
            edit(currentFolder, command);
        } else if (strcmp(command, "pwd") == 0) {
            pwd(path);
        }  else if (strncmp(command, "cd", 2) == 0){
            currentFolder = cd(currentFolder, command, &path);
        } 
         else if (strcmp(command, "exit") == 0){
            break;
        }
    }

    return 0;
}
