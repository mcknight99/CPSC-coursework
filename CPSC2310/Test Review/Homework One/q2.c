
struct simple{

   int a;

   char b; 

   float c;

};

struct Init{

   int a;

   short b[10];

   struct simple c;

};

// does it need to be by decreasing size? struct, int, short?

//Rewrite struct Init, such that during the creation of the struct it will initialize an instance of Init using an initialization list. 

// struct Init{
//    int a;
//    short b[10];
//    struct simple c;
// };

// struct Init init = {1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {1, 'a', 1.0}};