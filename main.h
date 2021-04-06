#include <string.h>
#include <stdio.h>

#define MAX_TEXT_BUFFER 256
#define MAX_CATEGORY_LENGTH 25

// Functions
void save();
void load();
void budgetCalculate();
void expenseAdd(int category, int value);
void expenseRemove(int index);
char *getCategoryString(char (*textList)[MAX_CATEGORY_LENGTH], int count, char delimiter);

// Enums
enum ExpenseType {ESSENTIALS, ENTERTAINMENT, INVESTMENT, SHOPPING};

// Structs
typedef struct {
  int category;
  int value;
} Expense;


// Variables
Expense *expenses;

char (*categories)[MAX_CATEGORY_LENGTH];
char *categoriesDropdown;
int  categoriesCount = 4;

int expensesCount = 0;

int budgetMax = 1000;
int budgetCurrent = 1000;

