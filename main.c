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
  
  Vector2 ringPosition = {200, 200};
  float angleStart = 180.0f;
  
  budgetMax = 1000;
  budgetCurrent = budgetMax;
  
  int budgetStrWidth = MeasureText(TextFormat("$%d", budgetCurrent), 40);
  
  bool budgetMaxActive = true;

  expensesCount = 0;
  expenses = malloc(expensesCount * sizeof(Expense));

  bool expenseCategoryActive = false;
  int expenseCategory = 0;
  
  bool expenseValueActive = false;
  int expenseValue = 0;
  
  Rectangle panelRec = {0, 430, screenWidth, 170};
  Rectangle panelContentRec = {0, 0, screenWidth - 12, 300};
  Vector2 panelScroll = { 0, 0 };
  
  // populate expenses
  for (int i = 0; i < expensesCount; ++i)  {
    expenses[i].category = 0;
    expenses[i].value = 50 * i;
  }
  
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
        budgetCurrent = budgetMax;
        budgetStrWidth = MeasureText(TextFormat("$%d", budgetMax), 40);
      }
      
      DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd, 0, Fade(GREEN, 0.3));           // Available
      DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd - 360.0f, 0, Fade(MAROON, 0.3)); // Expenses
      DrawText(TextFormat("$%d", budgetCurrent), ringPosition.x - (budgetStrWidth / 2), ringPosition.y - 15, 40, GRAY);
      
      if (GuiValueBox((Rectangle){550, 180, 200, 50}, "Expense", &expenseValue, 0, budgetMax, expenseValueActive)) {
        expenseValueActive = !expenseValueActive;
      }
      
      if (GuiButton((Rectangle){550, 240, 200, 50}, "Apply")) {
        budgetCurrent -= expenseValue;
        Expense *temp = realloc(expenses, sizeof(Expense) * (expensesCount + 1));
        
        if (temp != NULL) {
          expenses = temp;
          ++expensesCount;
          expenses[expensesCount - 1].category = expenseCategory;
          expenses[expensesCount - 1].value = expenseValue;
        }
      }
      
      // Draw dropdown last
      if (expenseCategoryActive) {GuiUnlock();}
      if (GuiDropdownBox((Rectangle){550, 120, 200, 50}, "Essentials;Entertainment;Investing;Shopping", &expenseCategory, expenseCategoryActive)) {
        expenseCategoryActive = !expenseCategoryActive;
        printf("CATEGORY: %d\n", expenseCategory);
      }
      
      // Print expenses
      Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
      
      //GuiLine((Rectangle){550, 350, 200, 70}, "Hello!");
      BeginScissorMode(view.x, view.y, view.width, view.height);
        
        for (int i = 0; i < expensesCount; ++i) {
          GuiPanel((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + (40 * i), panelContentRec.width, 40});
          
          // TO-DO: Simplify (array of strings with categories then simple for loop?)
          switch (expenses[i].category) {
            case ESSENTIALS:
              DrawText("Essentials", panelRec.x + panelScroll.x + 2, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
              break;
            case ENTERTAINMENT:
              DrawText("Entertainment", panelRec.x + panelScroll.x + 2, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
              break;
            case INVESTMENT:
              DrawText("Investing", panelRec.x + panelScroll.x + 2, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
              break;
            case SHOPPING:
              DrawText("Shopping", panelRec.x + panelScroll.x + 2, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
              break;
            default:
              break;
          }
          DrawText(TextFormat("Value: %d", expenses[i].value), panelRec.x + panelScroll.x + 5 + 250, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
          
          if (GuiButton((Rectangle){750, panelRec.y + 5 + panelScroll.y + (40 * i), 30, 30}, "X")) {
            expenseRemove(i);
          }
        }
      EndScissorMode();
      
      if (GuiButton((Rectangle){550, 380, 90, 50}, "save")) {
        save();
      }
      
      if (GuiButton((Rectangle){660, 380, 90, 50}, "load")) {
        load();
      }
      
      
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  CloseWindow();

  return 0;
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

