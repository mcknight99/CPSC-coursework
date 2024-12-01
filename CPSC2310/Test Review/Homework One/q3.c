struct  test{

   int a

   struct test b;

   char c;

};

//this is broken. fix it

struct test_fix{

   int a;

   struct test *b;

   char c;

};

int main() {
    struct test_fix;
}