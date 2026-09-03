#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int value;
    struct Node* next;
} Node;

/*
The structure below represents the whole linked list (since it is more
readable and convenient to pass around a pointer to a "LinkedList" object
than to pass around the head pointer directly). 
*/
typedef struct {
    Node* head;
} LinkedList;

/* print_list(L)
Given a pointer to a LinkedList object, print the entire list in
forward order, with a newline after the last element. 
*/
void print_list(LinkedList* L){
    Node* current_node = L->head;
    while(current_node != NULL){
        printf("%d ", current_node->value);
        current_node = current_node->next;
    }
    printf("\n");
}

/* add_to_front(L, new_element)
   Given a linked list L and an element to add, 
   add the provided element to the beginning
   of the list
*/
void add_to_front(LinkedList* L, int new_element){
    if (L->head == NULL){
        //Adding the first element to an empty list

        //Create a new node
        Node* new_node = malloc(sizeof(Node));
        //SEt the value of the new node to the new_element
        new_node->value = new_element;
        //set the next pointer of the new node to NULL
        new_node->next = NULL;
        //SEt the head pointer of the list to point to the new node
        L->head = new_node;
    } else {
        //Create a new node
        Node* new_node = malloc(sizeof(Node));
        //SEt the value of the new node to the new_element
        new_node->value = new_element;
        //Set the next pointer of the new node to the current first node in the list
        new_node->next = L->head;
        //Set the head pointer of the list to point to the new node
        L->head = new_node;
    }
}

/*add_to_end(L, new_element)
  Given a linked list L, and an element to add,
  add the provided element to the end
  of the list
*/
void add_to_end(LinkedList* L, int new_element){
    if(L->head == NULL){
        //adding to the end of an empty list is the same as adding to the front
        add_to_front(L, new_element); //CHEATINGGG
    } else {
        //adding an element after the existing contents of a non-empty list
        Node* old_last_node =  L->head;
        while(old_last_node->next != NULL){
            old_last_node = old_last_node->next;
        }

        //Create a new node
        Node* new_node = malloc(sizeof(Node));
        //SEt the value of the new node to the new_element
        new_node->value = new_element;
        //set the next pointer of the new node to NULL
        new_node->next = NULL;
        //set the next pointer of the old last node to point at the new node
        old_last_node->next = new_node;
    }
}

/*delete_first(L)
  Given a linked list L, delete the first
  element of the list and return its value.
*/
int delete_first(LinkedList* L){
    //To do: check if list is already empty and print an error
    //      message and exit the program if so.
    Node* delete_node = L->head;
    L->head = delete_node->next;
    delete_node->next = NULL;

    int return_value = delete_node->value;
    free(delete_node);
    return return_value;
}

int main(void){

    //set up two empty lists L1 and L2
    LinkedList L1;
    L1.head = NULL;

    LinkedList L2;
    L2.head = NULL;

    //Add some elements to L1
    add_to_front(&L1, 6);
    add_to_front(&L1, 10);
    add_to_front(&L1, 17);
    //at this point, L1 should contain 17, 10, 6 (in this order)
    print_list(&L1);

    
    //Add more elements to L1
    add_to_end(&L1, 100);
    add_to_end(&L1, 200);
    //at this point, L should contain 17, 10, 6, 100, 200 (in this order)
    print_list(&L1);

    //Add some elements to L2
    for(int i = 0; i < 10; i++){
        add_to_end(&L2, i*i);
    }
    print_list(&L2);

    delete_first(&L1);
    print_list(&L1);
    delete_first(&L1);
    print_list(&L1);
    

    //Now delete every node in the list
    //(to ensure that free() is called for each
    // allocated node)
    while(L1.head != NULL){
        delete_first(&L1);
    }
    while(L2.head != NULL){
        delete_first(&L2);
    }




    return 0;
}