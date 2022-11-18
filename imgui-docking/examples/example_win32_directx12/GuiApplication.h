#pragma once
#include <stddef.h>
#include "TickTacToe.h"

namespace GUIApp {
    class GuiApplication
    {   
    public:
        GuiApplication();
        ~GuiApplication();

         
        float value;
        bool flag;
        float MAX_Range;
        float MIN_Range;
        int rows, columns;

        TickTacToe* TickTacGame;

        void DrawAppGUI();
        void DockAndMainMenu();
        void TickTacToeGameWindow(bool* p_open = NULL);
        void ConsoleWindow(float* value);
        void ConsolePanel(float* value); 
        void AlignForWidth(float width, float alignment );

        
 
        
        //int x[3][2] = { {0,1}, {2,3}, {4,5} };
    }; 
}
