#include <string.h>
#include <stdio.h>
#include "standard.h"

#define MAX_TEXT_BUFFER 100
#define MAX_CATEGORY_NAME_LENGTH 25

// Functions
nil save();
nil load();
nil reset();

nil categoryAdd();
nil categoryDropdownUpdate();

nil budgetCalculate();

nil expenseAdd(u08 category, u32 value);
nil expenseRemove(u32 index);

