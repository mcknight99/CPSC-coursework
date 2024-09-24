typedef struct {

   char a;

   struct SELF_REF *b;

   int c;

}SELF_REF;

// this is broken. fix it

typedef struct {

   char a;

   struct SELF_REF *b;

   int c;

}SELF_REF_FIX;

// the problem that was fixed was that the struct SELF_REF was not defined before it was used in the struct SELF_REF.