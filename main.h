#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_TEXT_BUFFER 100
#define MAX_CATEGORY_NAME_LENGTH 25

// Functions
void save();
void load();
void reset();

void categoryAdd();
char *categoryString();

void budgetCalculate();

void expenseAdd(int category, int value);
void expenseRemove(int index);

// Structs
typedef struct {
  int category;
  int value;
} Expense;

// Variables
Expense *expenses;

bool categoryStates[4];

char (*categories)[MAX_CATEGORY_NAME_LENGTH];
char *categoriesDropdown;

int expensesCount = 0;

int budgetMax = 1000;
int budgetCurrent = 1000;

