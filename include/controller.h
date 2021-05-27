#include "raylib.h"
#include <stdlib.h>

// Declarations
nil save();
nil load();
nil reset();

nil categoryAdd();
nil categoryDropdownUpdate();

nil budgetCalculate();

nil expenseAdd(u08 category, u32 value);
nil expenseRemove(u32 index);


// Definitions

// Save expenses
nil save() {
  FILE *fp = fopen("expenses.txt", "w");

  if (fp == NULL) {
  	fclose(fp);
  	exit(1);
  }
  
  fprintf(fp, "%d\n", budgetMax);

  for (u08 i = 0; i < 4; ++i) {
    if (TextLength(categories[i]) >= 1) {
      fprintf(fp, "%s\n", categories[i]);
    } else {
      fprintf(fp, " \n");
    }
  }
  
  fprintf(fp, "%d\n", expenseCount);
  for (u32 i = 0; i < expenseCount; ++i) {
    fprintf(fp, "%d\n%d\n", expenses[i].category, expenses[i].value);
  }
  
  fclose(fp);
}

// Load expenses
nil load() {
  if (!FileExists("expenses.txt")) {
    reset(); 
    return;
  }
  
  str data = LoadFileText("expenses.txt");
  
  i32 count;
  const str *lines = TextSplit(data, '\n', &count);
  
  if (count < 5) {
    reset();
    return;
  }
  
  // Budget max
  budgetMax = TextToInteger(lines[0]);
  budgetCurrent = budgetMax;
  
  // Categories
  for (u08 i = 1; i < 5; ++i) {
    strcpy(categories[i-1], lines[i]);
  }
  
  // Expenses
  expenseCount = TextToInteger(lines[5]);
  expenses = malloc(expenseCount * sizeof(Expense));
  
  for (u32 i = 6, j = 0; (i < count) && (j < expenseCount); i += 2, ++j) {
    expenses[j].category = TextToInteger(lines[i]);
    expenses[j].value = TextToInteger(lines[i+1]);
    
    budgetCurrent -= expenses[j].value;
  }
  
  categoryDropdownUpdate();
}

// Reset expenses
nil reset() {
  expenses = realloc(expenses, sizeof(Expense));
  expenseCount = 0;
  budgetMax = 1000;
  budgetCurrent = 1000;
  
  const str categoriesDefault[] = {"Essentials", "Investing", "Entertainment", "Shopping"};
  
  for (u08 i = 0; i < 4; ++i) {
    strcpy(categories[i], categoriesDefault[i]);
  }
  
  categoryDropdownUpdate();
}

// Update category string (used for category dropdown menu)
nil categoryDropdownUpdate() {
  strcpy(categoriesDropdown, TextFormat("%s;%s;%s;%s", categories[0], categories[1], categories[2], categories[3]));
}

// Calculate budget from expenses
nil budgetCalculate() {
  budgetCurrent = budgetMax;
  
  for (u32 i = 0; i < expenseCount; ++i) {
    budgetCurrent -= expenses[i].value;
  }
}

// Add expense
nil expenseAdd(u08 category, u32 value) {
  Expense *temp = realloc(expenses, sizeof(Expense) * (expenseCount + 1));
        
  if (temp != NULL) {
    expenses = temp;
    expenses[expenseCount].category = category;
    expenses[expenseCount].value = value;
    ++expenseCount;
  }
}

// Remove expense
nil expenseRemove(u32 index) {
  budgetCurrent += expenses[index].value;
  
  --expenseCount;
  for (u32 i = index; i < expenseCount; ++i) {
    expenses[i] = expenses[i + 1];
  }
}

