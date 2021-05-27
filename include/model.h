#include "standard.h"

#define MAX_TEXT_BUFFER 100
#define MAX_CATEGORY_NAME_LENGTH 25

// Structs
typedef struct {
  u08 category;
  u32 value;
} Expense;

// Variables
Expense *expenses;

// Category
chr (*categories)[MAX_CATEGORY_NAME_LENGTH];
chr categoriesDropdown[(MAX_CATEGORY_NAME_LENGTH * 4) + 3];
bit categoryStates[4];

// Expense
u32 expenseCategory       = 0;
u32 expenseCount          = 0;
u32 expenseValue          = 0;
bit expenseCategoryActive = false;
bit expenseValueActive    = false;

// Budget
u32 budgetMax       = 1000;
i32 budgetCurrent   = 1000;
bit budgetMaxActive = true;

