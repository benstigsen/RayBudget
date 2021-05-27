#include "standard.h"

#define MAX_CATEGORY_LENGTH 25

/// Category
chr (*categories)[MAX_CATEGORY_LENGTH]; // Array of category names
chr categoriesDropdown[(MAX_CATEGORY_LENGTH * 4) + 3]; // String for raylib dropdown string "category1;category2;category3;category4"

bit categoryStates[4]; // Keeps track of 4 input boxes to see if user is typing in one of them

/// Expense

// Expense structure
typedef struct {
  u08 category; // The expense category index (0-3)
  u32 value;    // The expense value
} Expense;

Expense *expenses; // Pointer to expenses

u32 expenseCategory = 0; // The selected expense category in the dropdown
u32 expenseCount    = 0; // Amount of expenses
u32 expenseValue    = 0; // Contains the value in the expense textbox
 
bit expenseCategoryActive = false; // Whether the expense category dropdown is expanded
bit expenseValueActive    = false; // Whether the expense textbox is focused

// Budget
u32 budgetMax       = 1000; // Max available budget
i32 budgetCurrent   = 1000; // Currently available budget

bit budgetMaxActive = true; // Whether the max budget value box is active

