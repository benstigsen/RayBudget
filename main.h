// Functions
void save();
void load();
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
int expensesCount;

int budgetMax;
int budgetCurrent;
