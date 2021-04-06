#include "main.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* 
  TO-DO:
    - Add expense category color (on wheel and scrollable panel)
    - Add the ability to add category
    - Add the ability to change expense category and value in the expense list
    - Fix text alignment and GUI element placement
    - Change saving / loading system 
    - Remove debug printf()
    - Refactor
*/

int main() {   
  // Initialization
  //---------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 600;
  
  InitWindow(screenWidth, screenHeight, "RayBudget");
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  
  // Values
  categories = malloc(categoriesCount * sizeof(*categories));
  strcpy(categories[0], "Essentials");
  strcpy(categories[1], "Entertainment");
  strcpy(categories[2], "Investing");
  strcpy(categories[3], "Shopping");
  
  categoriesDropdown = getCategoryString(categories, categoriesCount, ';');
  
  int budgetStrWidth  = MeasureText(TextFormat("$%d", budgetCurrent), 40);
  
  int expenseCategory = 0;
  int expenseValue    = 0;
  
  // Booleans
  bool budgetMaxActive       = true;
  bool expenseCategoryActive = false;
  bool expenseValueActive    = false;
  
  // Widget Properties
  float   angleStart   = 180.0f;
  Vector2 ringPosition = {200, 200};
  
  Rectangle panelRec        = {0, 430, 500, 170};
  Rectangle panelContentRec = {0, 0, screenWidth - 12, 300};
  Vector2   panelScroll     = { 0, 0 };
  
  SetTargetFPS(60);
  //---------------------------------------------------------------------------------------
  
  // Main loop
  while (!WindowShouldClose()) {
    // Update
    //----------------------------------------------------------------------------------
    float angleEnd = 180.0f + (((float)budgetCurrent / (float)budgetMax) * 360.0f);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    
      ClearBackground(RAYWHITE);
      
      if (expenseCategoryActive) {GuiLock();}
      
      if (GuiValueBox((Rectangle){600, 50, 100, 50}, "Budget", &budgetMax, 0, 10000, budgetMaxActive))  {
        budgetMaxActive = !budgetMaxActive;
        budgetCalculate();
        budgetStrWidth = MeasureText(TextFormat("$%d", budgetCurrent), 40);
      }
      
      DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd, 0, Fade(GREEN, 0.3));           // Available
      DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd - 360.0f, 0, Fade(MAROON, 0.3)); // Expenses
      DrawText(TextFormat("$%d", budgetCurrent), ringPosition.x - 50, ringPosition.y - 15, 40, GRAY);
      
      if (GuiValueBox((Rectangle){550, 160, 200, 40}, "Expense", &expenseValue, 0, budgetMax, expenseValueActive)) {
        expenseValueActive = !expenseValueActive;
      }
      
      if (GuiButton((Rectangle){550, 210, 200, 40}, "Apply")) {
        expenseAdd(expenseCategory, expenseValue);
        budgetCalculate();
      }
      
      // Draw dropdown last
      if (expenseCategoryActive) {GuiUnlock();}
      if (GuiDropdownBox((Rectangle){550, 110, 200, 40}, categoriesDropdown, &expenseCategory, expenseCategoryActive)) {
        expenseCategoryActive = !expenseCategoryActive;
      }
      
      // Print expenses
      Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
      
      // Draw list of expenses
      BeginScissorMode(view.x, view.y, view.width, view.height);
        
        for (int i = 0; i < expensesCount; ++i) {
          GuiPanel((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + (40 * i), panelContentRec.width, 40});

          DrawText(categories[(expenses[i].category)], panelRec.x + panelScroll.x + 5, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
          DrawText(TextFormat("-$%d", expenses[i].value), panelRec.x + panelScroll.x + 5 + 250, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
          
          if (GuiButton((Rectangle){panelRec.width - 45, panelRec.y + 5 + panelScroll.y + (40 * i), 30, 30}, "X")) {
            expenseRemove(i);
          }
        }
      EndScissorMode();
      
      
      // Save
      if (GuiButton((Rectangle){505, 560, 140, 30}, "save")) {save();}
      
      // Load
      if (GuiButton((Rectangle){655, 560, 140, 30}, "load")) {load();}
      
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  CloseWindow();

  return 0;
}

void budgetCalculate() {
  budgetCurrent = budgetMax;
  
  for (int i = 0; i < expensesCount; ++i) {
    budgetCurrent -= expenses[i].value;
  }
}

void expenseAdd(int category, int value) {
  Expense *temp = realloc(expenses, sizeof(Expense) * (expensesCount + 1));
        
  if (temp != NULL) {
    expenses = temp;
    expenses[expensesCount].category = category;
    expenses[expensesCount].value = value;
    ++expensesCount;
  }
}

void expenseRemove(int index) {
  budgetCurrent += expenses[index].value;
  
  --expensesCount;
  for (int i = index; i < expensesCount; ++i) {
    expenses[i] = expenses[i + 1];
  }
}

// TO-DO: Simplify
void save() {
  FILE *fp = fopen("expenses.txt", "w");
  fprintf(fp, "%d\n%d\n", budgetMax, expensesCount);
  fclose(fp);
  
  fp = fopen("expenses.txt", "a");
  for (int i = 0; i < expensesCount; ++i) 
  {
    fprintf(fp, "%d\n%d\n", expenses[i].category, expenses[i].value);
  }
  fclose(fp);
}

// TO-DO: Simplify
void load() {
  FILE *fp = fopen("expenses.txt", "r");
  
  fseek(fp, 0, SEEK_END);
  int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  char *content = malloc(fsize + 1);
  
  fread(content, 1, fsize, fp);
  fclose(fp);
  
  int lines;
  const char **values = TextSplit(content, '\n', &lines);

  budgetMax = TextToInteger(values[0]);
  budgetCurrent = budgetMax;
  expensesCount = TextToInteger(values[1]);

  expenses = malloc(sizeof(Expense) * expensesCount);
  
  int j = 0;
  for (int i = 1; i < lines - 2; i += 2) {
    printf("%d) %s: %s\n", i, values[i + 1], values[i + 2]);
    expenses[j].category = TextToInteger(values[i + 1]);
    expenses[j].value = TextToInteger(values[i + 2]);
    
    budgetCurrent -= expenses[j].value;
    ++j;
  }
}

// Join dynamic array of fixed length strings
char *getCategoryString(char (*textList)[MAX_CATEGORY_LENGTH], int count, char delimiter) {
  static char result[MAX_TEXT_BUFFER] = { 0 };
  memset(result, 0, MAX_TEXT_BUFFER);
  
  int pos = 0;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < MAX_CATEGORY_LENGTH; ++j) {
      if (textList[i][j] == '\0') {break;}
      
      result[pos++] = textList[i][j];
    }
    
    result[pos++] = delimiter;
  }
  
  result[pos - 1] = '\0';
  
  return result;
}
