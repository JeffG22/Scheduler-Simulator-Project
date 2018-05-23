#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int data;
    struct node* next;
} node;

void add(node** headP, int data) {
    node* new_node = (node*) malloc(sizeof(node));

    new_node->data = data;
    new_node->next = NULL;

    if(*headP == NULL)
        *headP = new_node;
    else{
        node* current = *headP;
        while (current->next != NULL)
            current = current->next;

        current->next = new_node;
    }
}

void push(node** headP, int data) {
    node* new_node = malloc(sizeof(node));

    new_node->next = *headP;
    new_node->data = data;

    *headP = new_node;
}

int pop(node** headP) {
    if (*headP != NULL) {
        node* previous_head = *headP;

        *headP = previous_head->next;
        int data = previous_head->data;

        free(previous_head);

        return data;
    }
    else {
        printf("List is already empty.\n");
        return -1;
    }
}

int remove_last(node** headP) {
    if (*headP != NULL) {
        if ((*headP)->next == NULL) {
            return pop(headP);
        }
        else {
            node *current = (*headP)->next;
            node *previous = *headP;

            while (current->next != NULL) {
                previous = current;
                current = current->next;
            }

            int data = current->data;
            previous->next = NULL;

            free(current);

            return data;
        }
    }
    else {
        printf("List is already empty.\n");
        return -1;
    }
}

int remove_by_index(node** headP, int index) {
    if (*headP != NULL) {
        if (index == 0) {
            return pop(headP);
        }
        else if ((*headP)->next == NULL) {
            printf("Error: There's no element at index %d.\n", index);
            return -1;
        }
        else {
            node *current = (*headP)->next;
            node *previous = *headP;

            int i = 0;

            while ((i < index-1) && (current->next != NULL)) {
                previous = current;
                current = current->next;
                i++;
            }

            if (i != index-1) {
                printf("Error: There's no element at index %d.\n", index);
                return -1;
            }

            int data = current->data;
            previous->next = current->next;

            free(current);

            return data;
        }
    }
    else {
        printf("List is already empty.\n");
        return -1;
    }
}

int remove_by_value(node** headP, int value) {
    if (*headP != NULL) {
        if((*headP)->data == value)
            return pop(headP);
        else if ((*headP)->next == NULL) {
            printf("Oops! It looks like the value %d is not in the list!\n", value);
            return -1;
        }
        else {
            node* current = (*headP)->next;
            node* previous = *headP;

            int i = 0;

            while ((current->data != value) && (current->next != NULL)) {
                previous = current;
                current = current->next;
                i++;
            }

            if (current->data != value) {
                printf("Oops! It looks like the value %d is not in the list!\n", value);
                return -1;
            }

            int data = current->data;
            previous->next = current->next;

            free(current);

            return data;
        }
    }
    else {
        printf("List is already empty.\n");
        return -1;
    }
}

void printList(node* head) {
    if (head == NULL)
        printf("Empty list.\n");
    else {
        node* current = head;
        while (current != NULL) {
            printf("%d ", current->data);
            current = current -> next;
        }
        printf("\n");
    }
}

int main() {
    node *aList = NULL;
    add(&aList, 3);
    add(&aList, 12);
    add(&aList, 4);
    printList(aList);
    push(&aList, 22);
    printList(aList);
    printf("Removed %d\n", remove_by_index(&aList, 5));
    printList(aList);
    printf("Removed %d\n", remove_by_value(&aList, 22));
    printf("Removed %d\n", remove_by_value(&aList, 23));
    printList(aList);
    printf("Removed %d\n", remove_by_value(&aList, 4));
    printList(aList);
    printf("Removed %d\n", remove_by_value(&aList, 3));
    printList(aList);
    printf("Removed %d\n", remove_by_value(&aList, 32));
    printf("Removed %d\n", remove_by_value(&aList, 12));
    printList(aList);
    printf("Removed %d\n", remove_by_value(&aList, 32));


}

/*
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node* next;
} node_t;

void print_list(node_t* head) {
    node_t* current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

void add(node_t* head, int value) {
    node_t* current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    current->next->val = value;
    current->next->next = NULL;
}


int main() {
    node_t* list = NULL;
    list = malloc(sizeof(node_t));
    list->val = 13;

    add(list, 24);
    add(list, 33);
    add(list, 212);
    print_list(list);
}
*/