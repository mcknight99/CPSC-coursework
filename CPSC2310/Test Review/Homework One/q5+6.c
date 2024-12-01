struct First{
    short a;
    float b;
    char c;
    int d;
    int e;
};

// size of each:
// short: 2
// float: 4
// char: 1
// int: 4
// int: 4

// total size: 15
// size with padding: 16
// padding size: 1

//rewritten to save space
// go from largest to smallest

struct Second{
    int d;
    int e;
    float b;
    short a;
    char c;
};