#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define DESC_SIZE 8

typedef struct node_t *pNode;
typedef struct node_t {
    char* desc;
    pNode prev;
    pNode next;
} Node;


typedef struct queue_t *pQueue;
typedef struct queue_t {
    pNode first;
    pNode last;
} Queue;


void print_queue(pQueue p)
{
    int i = 0;
    printf("v~~top\n");
    pNode tmp = p->first;
    while (NULL != tmp) {
        printf("%d: %s\n", ++i, tmp->desc);
        tmp = tmp->next;
    }
    printf("---bottom\n");
    i = 0;
    tmp = p->last;
    while (NULL != tmp) {
        printf("%d: %s\n", ++i, tmp->desc);
        tmp = tmp->prev;
    }
    printf("^~~top\n\n");
}


void insert_bottom(pQueue p, pNode n)
{
    n->next = n->prev = NULL;
    if (NULL == p->first) {
        p->first = n;
        p->last = n;
        return;
    }
    p->last->next = n;
    n->prev = p->last;
    p->last = n;
}


pNode search_node_by_desc(pQueue p, char* desc)
{
    pNode tmp = p->first;
    while (NULL != tmp) {
        if (strcmp(desc, tmp->desc) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}


void delete_node(pQueue p, char* desc)
{
    pNode del = search_node_by_desc(p, desc);
    if (NULL == del) return;
    if (del == p->first)
        p->first = del->next;
    if (del == p->last)
        p->last = del->prev;
    if (NULL != del->next)
        del->next->prev = del->prev;
    if (NULL != del-> prev)
        del->prev->next = del->next; //1 aggiorno il puntatore al successivo del nodo precedente

    free(del->desc); //2 libero byte allocati per descrizione, vediamo che automaticamente il contenuto della stringa non rimane scritto nel heap
    del->next = NULL; //3 per sicurezza cancello indirizzi che rimarrebbero scritti nell'heap
    del->prev = NULL; //3
    free(del);
}


pNode create_node(char* desc)
{
    pNode new_node = (pNode) malloc(sizeof(Node));
    if (new_node == NULL) exit(-1); //4 controllo su malloc
    new_node->desc = (char*) malloc(sizeof(char)*DESC_SIZE);
    if (new_node->desc == NULL) exit(-1); //4 controllo su malloc
    
    strncpy(new_node->desc, desc, sizeof(char) * DESC_SIZE ); //5 istruzione di copia di grandezza definita (overflow safe)
    //strcpy(new_node->desc, desc);

    return new_node;
}


int main()
{
    Queue queue;

    queue.first = queue.last = NULL; //0 inizializzare a NULL i puntatori della coda

    insert_bottom(&queue, create_node("a1"));
    insert_bottom(&queue, create_node("b2"));
    insert_bottom(&queue, create_node("c3"));
    insert_bottom(&queue, create_node("d4"));
    insert_bottom(&queue, create_node("e5"));
    print_queue(&queue);

    delete_node(&queue, "e5");
    delete_node(&queue, "a1");
    print_queue(&queue);

    delete_node(&queue, "b2");
    delete_node(&queue, "c3");
    print_queue(&queue);

    delete_node(&queue, "d4");
    print_queue(&queue);

    return 0;
}
