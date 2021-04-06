// Functions
void save();
void load();
void budgetCalculate();
void expenseAdd(int category, int value);
void expenseRemove(int index);

// Enums
enum ExpenseType {ESSENTIALS, ENTERTAINMENT, INVESTMENT, SHOPPING};

// Structs
typedef struct {
  int category;
  int value;
} Expense;


// Variables
Expense *expenses;
int expensesCount = 0;

int budgetMax = 1000;
int budgetCurrent = 1000;
