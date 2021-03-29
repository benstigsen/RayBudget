#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


static void DrawStyleEditControls(void);  // Draw and process scroll bar style edition controls

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raygui - GuiScrollPanel()");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    
    /* Widgets */
    //Vector2 center = {(GetScreenWidth() - 100) / 2, GetScreenHeight() / 2};
    Vector2 ringPosition = {200, 200};
    float angleStart = 0.0f;
    float angleEnd = 360.0f;  
    
    char expenseStr[20] = "Entertainment";
    
    int budgetMax = 1000;
    int budgetStrWidth = MeasureText(TextFormat("$%d", budgetMax), 40);
    int expenseValue = 0;
    
    bool budgetMaxActive = true;
    bool expenseCategoryActive = false;
    bool expenseValueActive = false;
    
    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        if (GuiValueBox((Rectangle){600, 50, 100, 50}, "Budget", &budgetMax, 0, 10000, budgetMaxActive)) 
        {
          budgetMaxActive = !budgetMaxActive;
          budgetStrWidth = MeasureText(TextFormat("$%d", budgetMax), 40);
        }
        
        DrawRing(ringPosition, 80.0f, 190.0f, angleStart, angleEnd, 0, Fade(MAROON, 0.3));
        DrawText(TextFormat("$%d", budgetMax), ringPosition.x - (budgetStrWidth / 2), ringPosition.y - 15, 40, GRAY);
        
        if (GuiTextBox((Rectangle){550, 120, 200, 50}, expenseStr, 40, expenseCategoryActive))
        {
          expenseCategoryActive = !expenseCategoryActive;
        }
        
        if (GuiValueBox((Rectangle){550, 180, 200, 50}, "Expense", &expenseValue, 0, budgetMax, expenseValueActive)) 
        {
          expenseValueActive = !expenseValueActive;
        }
        
        if (GuiButton((Rectangle){550, 240, 200, 50}, "Apply")) 
        {
          budgetMax -= expenseValue;
        } 
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

