/***********************
 *Samuel Pupke
 *spupke@clemson.edu
 *Lab 5
 *Lab Section 004
 **********************/

#include <stdio.h>

struct NODE{
    int a;
    struct NODE *b;
    struct NODE *c;
}nodes[5] = { // a      b          c
                {15,    nodes + 2, nodes+1},
                {30,    nodes + 3, NULL},
                {46,    nodes + 1,  nodes + 4},
                {95,    nodes + 4,  nodes},
                {123,   NULL, nodes + 2}
    };
struct NODE* np = nodes + 4;
struct NODE** npp = &nodes[2].b;

int main()
{ 
    // sizeof node = 24 bytes because of 4 byte padding between int and pointer
    //printf("sizeof NODE\t%lu\n", sizeof(struct NODE));
    // sizeof pointer = 8 bytes
    //printf("sizeof pointer\t%lu\n", sizeof(struct NODE*));
    // sizeof int = 4 bytes
    //printf("sizeof int\t%lu\n", sizeof(int));

    //printf("\n\n\nValue testings\n");

    // i want all memory address outputs to be relative to the start of the array but with a fake value of this variable input:
    int fake = 0x120;
    int start = (int)nodes;

    // then from here on out instead of using the real addresses, shift the addresses from the start of the array relative to the fake value

    //print value of "nodes"
    printf("nodes\t%p\n", nodes);
    //print value of "nodes.a"
    //printf("nodes.a\t%d\n", nodes.a); // ILLEGAL
    //printf("nodes.a\tILLEGAL\n");
    //print value of "nodes[3].a"
    printf("nodes[3].a\t%d\n", nodes[3].a);
    //print value of "nodes[3].c"
    printf("nodes[3].c\t%p\n", nodes[3].c);
    //print value of "nodes[3].c->a"
    printf("nodes[3].c->a\t%d\n", nodes[3].c->a);
    //print value of "*nodes.a"
    //printf("*nodes.a\t%d\n", *nodes.a); // ILLEGAL
    //printf("*nodes.a\tILLEGAL\n");
    //print value of "(*nodes).a"
    printf("(*nodes).a\t%d\n", (*nodes).a);
    //print value of "nodes->a"
    printf("nodes->a\t%d\n", nodes->a);
    //print value of "nodes[3].b->b"
    printf("nodes[3].b->b\t%p\n", nodes[3].b->b);
    //&nodes[3].a
    printf("&nodes[3].a\t%p\n", &nodes[3].a);
    //&nodes[3].c
    printf("&nodes[3].c\t%p\n", &nodes[3].c);
    //&nodes[3].c->a
    printf("&nodes[3].c->a\t%p\n", &nodes[3].c->a);
    //&nodes->a
    printf("&nodes->a\t%p\n", &nodes->a);
    //np
    printf("np\t%p\n", np);
    //np->a
    printf("np->a\t%d\n", np->a);
    //np->c->c->a
    printf("np->c->c->a\t%d\n", np->c->c->a);
    //npp
    printf("npp\t%p\n", npp);
    //npp->a
    //printf("npp->a\t%d\n", npp->a); // ILLEGAL
    //printf("npp->a\tILLEGAL\n");
    //*npp
    printf("*npp\t%p\n", *npp);
    //*npp->a
    //printf("*npp->a\t%d\n", *npp->a); // ILLEGAL
    //printf("*npp->a\tILLEGAL\n");
    //(*npp)->a
    printf("(*npp)->a\t%d\n", (*npp)->a);
    //&np
    printf("&np\t%p\n", &np);
    //&np->a
    printf("&np->a\t%p\n", &np->a);
    //&np->c->c->a
    printf("&np->c->c->a\t%p\n", &np->c->c->a);




    return 0;

}
