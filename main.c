#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* 
  TO-DO:
    - Add expense category color
    - Add expense category string to expenses list
    - Add dynamic allocation to the expenses when added

*/

typedef struct {
  int category;
  int value;
} Expense;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{   
    // Initialization
    //---------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "RayBudget");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    
    Vector2 ringPosition = {200, 200};
    float angleStart = 180.0f;
    
    int budgetMax = 1000;
    int budgetCurrent = 1000;
    int budgetStrWidth = MeasureText(TextFormat("$%d", budgetCurrent), 40);
    
    bool budgetMaxActive = true;

    int expensesCount = 0;
    Expense *expenses = malloc(expensesCount * sizeof(Expense));

    bool expenseCategoryActive = false;
    int expenseCategory = 0;
    
    bool expenseValueActive = false;
    int expenseValue = 0;
    
    Rectangle panelRec = {0, 430, screenWidth, 170};
    Rectangle panelContentRec = {0, 0, screenWidth - 12, 300};
    Vector2 panelScroll = { 0, 0 };
    
    // populate expenses
    for (int i = 0; i < expensesCount; ++i) {
      Expense expense;
      expenses[i].category = 0;
      expenses[i].value = 50 * i;
    }
    
    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float angleEnd = 180.0f + (((float)budgetCurrent / (float)budgetMax) * 360.0f);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
          ClearBackground(RAYWHITE);
          
          if (expenseCategoryActive) {GuiLock();}
          
          if (GuiValueBox((Rectangle){600, 50, 100, 50}, "Budget", &budgetMax, 0, 10000, budgetMaxActive)) 
          {
            budgetMaxActive = !budgetMaxActive;
            budgetCurrent = budgetMax;
            budgetStrWidth = MeasureText(TextFormat("$%d", budgetMax), 40);
          }
          
          DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd, 0, Fade(GREEN, 0.3));           // Available
          DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd - 360.0f, 0, Fade(MAROON, 0.3)); // Expenses
          DrawText(TextFormat("$%d", budgetCurrent), ringPosition.x - (budgetStrWidth / 2), ringPosition.y - 15, 40, GRAY);
          
          if (GuiValueBox((Rectangle){550, 180, 200, 50}, "Expense", &expenseValue, 0, budgetMax, expenseValueActive)) 
          {
            expenseValueActive = !expenseValueActive;
          }
          
          if (GuiButton((Rectangle){550, 240, 200, 50}, "Apply")) 
          {
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
          if (GuiDropdownBox((Rectangle){550, 120, 200, 50}, "Entertainment;Essentials;Investing", &expenseCategory, expenseCategoryActive))
          {
            expenseCategoryActive = !expenseCategoryActive;
            printf("CATEGORY: %d\n", expenseCategory);
          }
          
          // Print expenses
          Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
          
          //GuiLine((Rectangle){550, 350, 200, 70}, "Hello!");
          BeginScissorMode(view.x, view.y, view.width, view.height);
            
            for (int i = 0; i < expensesCount; ++i) {
              GuiPanel((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + (40 * i), panelContentRec.width, 40});
              DrawText(TextFormat("Category: %d", expenses[i].category), panelRec.x + panelScroll.x + 2, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
              DrawText(TextFormat("Value: %d", expenses[i].value), panelRec.x + panelScroll.x + 5 + 250, panelRec.y + 5 + panelScroll.y + (40 * i), 30, GRAY);
            }
            // Display Data
            //GuiGrid((Rectangle){panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height}, 16, 3);
          EndScissorMode();
          
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

