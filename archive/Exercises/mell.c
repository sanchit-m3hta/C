#include <stdio.h>

/* Task:
Create a structure type to represent a single node of the list
*/

/*
typedef struct{
    int element;
    Node* next;

} Node;
 */

typedef struct Node {
    int element;
    struct Node* next;
 }Node;



int main(void){

    //node2 = 10, node1 = 6, node3 = 17

    Node node1;
    Node node2;
    Node node3;

    node1.element = 6;
    node1.next = &node3;

    node2.element = 10;
    node2.next = &node1;

    node3.element = 17;
    node3.next = NULL;

    Node* head = &node2;

    printf("First element: %d\n", head->element);

    printf("second element: %d\n", head->next->element);

    printf("third element: %d\n", head->next->next->element);

    printf("Print list (1): ");
    Node* current_node = head;

    while(current_node != NULL){
        //print the element in the current node
        printf("%d ", current_node->element);
        //adjust the current node pointer to point at the next node
        current_node = current_node->next;
    }
    /*Hard coded printing of LL
    printf("%d ", current_node->element);
    //adjust the current node pointer to point at the next node
    current_node = current_node->next;

    printf("%d ", current_node->element);
    //adjust the current node pointer to point at the next node
    current_node = current_node->next;
    */

    printf("\n");

    //remove 6 from the list
    node2.next = &node3;

    current_node = head;
    
    printf("With 6 removed\nPrint list (2): ");
    while(current_node != NULL){
        //print the element in the current node
        printf("%d ", current_node->element);
        //adjust the current node pointer to point at the next node
        current_node = current_node->next;
    }

    printf("\n");
    return 0;
}