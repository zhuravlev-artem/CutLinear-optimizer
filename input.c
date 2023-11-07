#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>

int disk_thickness;
int lenght_parts=0;
int lenght_boards=0;

struct Board
{
    int length;
    int part_counter;
    int old_remnat;
    bool used;
    int* combination;
    int* parts_buffer;
    int* best_combination;
};

struct Part
{
    int length;
    bool used;
};

void Pprint(struct Part* x)
{
    printf("\tlength: %d\n\tused: %d", x->length, x->used);
}

void Bprint(struct Board* x)
{
    printf("\tlength: %d\n\tpart_counter: %d\n\told_remnat: %d\n\tused: %d\n\tcombination:", x->length, x->part_counter, x->old_remnat, x->used);
    for(int i=0; i<lenght_boards; i++)
    {
        printf(" %d,", x->combination[i]);
    }
    printf("\b\n\tparts_buffer:");

    for(int i=0; i<lenght_boards; i++)
    {
        printf(" %d,", x->parts_buffer[i]);
    }
    printf("\b\n\tbest_combination:");

    for(int i=0; i<lenght_boards; i++)
    {
        printf(" %d,", x->best_combination[i]);
    }
    printf("\b");
}

struct Node
{
    int data;
    struct Node* next;
};

int n_read(struct Node* Hd, unsigned int id)
{
    struct Node* ptr = Hd;
    for(int i=0; (i < id)&&(ptr != NULL); i++)
    {
        ptr=ptr->next;
    }
    return ptr->data;
}

struct Node* NewNode(int Data)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data=Data;
    node->next=NULL;
    return node;
}

void n_append(struct Node* Hd, int Data)
{
    struct Node* node_ptr=Hd;

    while(node_ptr->next != NULL)
    {
        node_ptr = node_ptr->next;
    }

    node_ptr->next = NewNode(Data);

}

void n_print(struct Node* Hd)
{

    struct Node* node_ptr=Hd;

    while(node_ptr->next != NULL)
    {
        printf("%d->", node_ptr->data);
        node_ptr=node_ptr->next;
    }
    printf("%d->NULL", node_ptr->data);
}

int main()
{
    struct Node parts = {0, NULL};
    int scanb;
    while(1)
    {
        scanf("%d", &scanb);
        if(scanb==0)
        {
            break;
        }
        n_append(&parts, scanb);
        lenght_parts++;
    }
    scanf("%d", &scanb);
    struct Node boards = {scanb, NULL};
    lenght_boards++;

    while(1)
    {
        scanf("%d", &scanb);
        if(scanb==0)
        {
            break;
        }
        n_append(&boards, scanb);
        lenght_boards++;
    }

    struct Part* part = (struct Part*)malloc(sizeof(struct Part)*lenght_parts);

    for(int i = 0; i<lenght_parts; i++){
        part[i] = {n_read(&parts, i), 0};
    }

    struct Board* board = (struct Board*)malloc(sizeof(struct Board)*lenght_boards);

    for(int i = 0; i<;)



    return 0;
}
