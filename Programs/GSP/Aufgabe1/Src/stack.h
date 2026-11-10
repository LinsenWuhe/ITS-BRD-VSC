
//Konstanten
#define STACK_SIZE 10       //Stackgröße festgelegt
#define STACK_OK    0       
#define STACK_ERROR -1
#define STACK_OVERFLOW 1
#define STACK_EMPTY -2
#define STACK_NUR1  -11

// Funktionen für Stack

//Pusht eine Zahl auf den Stack
int  stack_push(int value);

//Holt eine Zahl vom Stack
int  stack_pop(int *result);

//Löscht den Stack 
int stack_clear(void);


