#include<stdio.h>
#include<stdlib.h>
typedef struct node{
    int data;
    struct node* child;
    struct node* sibling;
}node;
node* createNode(int data){
    node* temp = (node*)malloc(sizeof(node));
    temp->data = data;
    temp->child = NULL;
    temp->sibling = NULL;
}
node* addSibling(node* parent,int data){
    node* temp = createNode(data);
    if(parent->child == NULL){
        parent->child = temp;
        return parent;
    }
    node* q = parent->child;
    while(q->sibling){
        q = q->sibling;
    }
    q->sibling = temp;
    return parent;

}
void ls(node* curDir){
    node* temp= curDir->child;
    while(temp){
        printf("%d ",temp->data);
        temp = temp->sibling;
    }
}
node* cd(node* curDir,int data){
    node* temp = curDir->child;
    while(temp){
        if(temp->data == data){
            return temp;
        }
        temp= temp->sibling;
    }

}
int main(){
    node* root = createNode(99);
    node* curDir = root;
    curDir = addSibling(curDir,5);
    curDir = addSibling(curDir,6);
    ls(curDir);
    printf("\n");
    curDir = cd(curDir,5);
    curDir = addSibling(curDir,7);
    curDir = addSibling(curDir,8);
    ls(curDir);

    //root = addSibling()
    return 0;
}
