#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "jsmn.h"

#define MAX_TEXT_BUFFER 100
#define MAX_CATEGORY_NAME_LENGTH 25

// Functions
void save();
void load();
void loadJSON();
void reset();

void categoryAdd();
void categoryDropdownUpdate();

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
char categoriesDropdown[(MAX_CATEGORY_NAME_LENGTH * 4) + 3];

int expensesCount = 0;

int budgetMax = 1000;
int budgetCurrent = 1000;

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}
