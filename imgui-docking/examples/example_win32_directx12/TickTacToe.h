#pragma once

class TickTacToe {

public:
    TickTacToe();
    ~TickTacToe();
    void UpdateGame();
    char CheckGanador();
    bool CheckEmpate();
    void ResetTicTackToe();
private:
    char tablero[3][3] = { {'_','_','_'},{'_','_','_'},{'_','_','_'} };
    bool endGame;
    bool turno = false;
    char jugador;
    char ganador;
};
