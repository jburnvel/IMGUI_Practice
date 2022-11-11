#pragma once
namespace GUIApp {
    class GuiApplication
    {   
    public:
        GuiApplication();
        ~GuiApplication();
        void DrawAppGUI();
        void DockAndMainMenu();
        void ConsoleWindow(float* value);
        void ConsolePanel(float* value);
        char CheckGanador();
        bool CheckEmpate();
        void ResetTicTackToe();
        float value;
        bool flag;
        float MAX_Range;
        float MIN_Range;
        int rows, columns;

        char tablero[3][3] = { {'_','_','_'},{'_','_','_'},{'_','_','_'} };
        bool turno = false;
        char jugador;
        char ganador;
        bool endGame;
        //int x[3][2] = { {0,1}, {2,3}, {4,5} };
    }; 
}
