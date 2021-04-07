#include "main.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* 
  - Fix text alignment and GUI element placement
  - Refactor
*/

int main() {   
  // Initialization
  //---------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 600;
  
  InitWindow(screenWidth, screenHeight, "RayBudget");
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
  
  // Values
  categories = malloc(4 * sizeof(*categories));
  
  // Initialize
  load();
  categoryDropdownUpdate();
  
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
  Rectangle panelContentRec = {0, 0, panelRec.width - 12, 300};
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
      
      // Draw categories
      DrawText("Categories", 715, 410, 15, GRAY);
      for (int i = 0; i < 4; ++i) {
        if (GuiTextBox((Rectangle){505, 430 + (43 * i), 290, 38}, categories[i], 20, categoryStates[i])) {
          categoryDropdownUpdate();
          categoryStates[i] = !categoryStates[i];
        }
      }

      // Draw list of expenses
      Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
      
      BeginScissorMode(view.x, view.y, view.width, view.height);
        
        for (int i = 0; i < expensesCount; ++i) {
          GuiPanel((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + (40 * i), panelContentRec.width, 40});

          DrawText(categories[(expenses[i].category)], panelRec.x + panelScroll.x + 5, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
          DrawText(TextFormat("-$%d", expenses[i].value), panelRec.x + panelScroll.x + 340, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
          
          if (GuiButton((Rectangle){panelRec.width - 45, panelRec.y + 5 + panelScroll.y + (40 * i), 30, 30}, "X")) {
            expenseRemove(i);
          }
        }
        
      EndScissorMode();
      
      // Save
      if (GuiButton((Rectangle){screenWidth - 240, 10, 70, 20}, "save")) {save();}
      
      // Load
      if (GuiButton((Rectangle){screenWidth - 160, 10, 70, 20}, "load")) {load();}
      
      // Reset
      if (GuiButton((Rectangle){screenWidth - 80, 10, 70, 20}, "reset")) {reset();}
      
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  CloseWindow();

  return 0;
}

void save() {
  FILE *fp = fopen("expenses.txt", "w");
  fclose(fp);
  
  fp = fopen("expenses.txt", "a");
  fprintf(fp, "%d\n", budgetMax);
  
  for (int i = 0; i < 4; ++i) {
    if (TextLength(categories[i]) >= 1) {
      fprintf(fp, "%s\n", categories[i]);
    } else {
      fprintf(fp, " \n");
    }
  }
  
  fprintf(fp, "%d\n", expensesCount);
  for (int i = 0; i < expensesCount; ++i) {
    fprintf(fp, "%d\n%d\n", expenses[i].category, expenses[i].value);
  }
  fclose(fp);
}

void load() {
  if (!FileExists("expenses.txt")) {
    reset(); 
    return;
  }
  
  char *data = LoadFileText("expenses.txt");
  
  int count;
  const char **lines = TextSplit(data, '\n', &count);
  
  if (count >= 5) {
    // Budget max
    budgetMax = TextToInteger(lines[0]);
    
    // Categories
    for (int i = 1; i < 5; ++i) {
      strcpy(categories[i-1], lines[i]);
    }
    
    // Expenses
    expensesCount = TextToInteger(lines[5]);
    expenses = malloc(expensesCount * sizeof(Expense));
    
    for (int i = 6, j = 0; (i < count) && (j < expensesCount); i += 2, ++j) {
      expenses[j].category = TextToInteger(lines[i]);
      expenses[j].value = TextToInteger(lines[i+1]);
      
      budgetCurrent -= expenses[j].value;
    }
    
    categoryDropdownUpdate();
  } else {
    reset();
  }
}

void reset() {
  expenses = realloc(expenses, sizeof(Expense));
  expensesCount = 0;
  budgetMax = 1000;
  budgetCurrent = 1000;
  
  const char *categoriesDefault[] = {"Essentials", "Investing", "Entertainment", "Shopping"};
  
  for (int i = 0; i < 4; ++i) {
    strcpy(categories[i], categoriesDefault[i]);
  }
  
  categoryDropdownUpdate();
}

void categoryAdd(int index, char *name) {
  strcpy(categories[index], name);
}

void categoryDropdownUpdate() {
  strcpy(categoriesDropdown, TextFormat("%s;%s;%s;%s", categories[0], categories[1], categories[2], categories[3]));
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

