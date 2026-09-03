/*
Doubly Linked list implementation
*/
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

struct Node* head; //global variable - pointer to head node.

struct Node* get_new_node(int x){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = x;
    new_node->prev = NULL;
    new_node ->next = NULL;

}

void insert_at_head(int x){
    struct Node* new_node = get_new_node(x);
    if (head == NULL){
        //if list is empty
        head = new_node;
        return;
    }
    head->prev = new_node;
    new_node->next = head;
    head = new_node;


}
void insert_at_tail(int x);
void print_list(int x);
void reverse_print_list(int x);



int main(void){

    return 0;
}