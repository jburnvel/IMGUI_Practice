#include "imgui.h"
#include <string>
#include <iostream>
#include "TickTacToe.h" 

using namespace std;

TickTacToe::TickTacToe()
{
    turno = true;
    jugador = turno ? 'X' : 'O';
    endGame = false;
    ganador = ' ';
}

TickTacToe::~TickTacToe()
{

}

void TickTacToe::UpdateGame()
{
    char buf[32];
    sprintf_s(buf, "Turno de las %c", jugador);
    ImGui::Text(buf);

    for (auto i = 0; i < 3; i++)
    {
        string num = " a ";
        for (auto j = 0; j < 3; j++)
        {
            num = tablero[i][j];//to_string(i) + " - " + to_string(j);
            ImGui::PushID(i * 3 + j);
            if (num == "_") {

                if (!endGame) {
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
                    if (turno) {
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
                    }
                    else {
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
                    }
                }
                else {
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                }

            }
            else if (num == "X") {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
            }
            else if (num == "O") {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
            }

            string actual = turno ? "X" : "O";

            if (endGame)actual = " ";

            if (ImGui::Button(num == "_" ? actual.c_str() : (num).c_str(), ImVec2(100, 100)))
            {
                if (endGame == false && tablero[i][j] == '_') {
                    tablero[i][j] = jugador;
                    turno = !turno;
                    jugador = turno ? 'X' : 'O';
                }
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            if (j < 3 - 1)
                ImGui::SameLine();
        }
    }

    ganador = CheckGanador();

    if (ganador != '_') {
        //char buf[32];
        sprintf_s(buf, " Ganador %c", ganador);
        ImGui::Text(buf);
        endGame = true;
    }

    if (CheckEmpate() == true && ganador == '_') {
        //char buf[32];
        sprintf_s(buf, " Empate ");
        ImGui::Text(buf);
        endGame = true;
    }
    if (endGame) {
        if (ImGui::Button("Reset Tick Tac Toe Game", ImVec2(350, 50)))
        {
            ResetTicTackToe();
        }
    }
}

char TickTacToe::CheckGanador()
{
    for (auto i = 0; i < 3; i++)
    {
        if (tablero[i][0] != '_' && tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2])
        {
            return tablero[i][0];
        }
    }
    for (auto i = 0; i < 3; i++)
    {
        if (tablero[0][i] != '_' && tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i])
        {
            return tablero[0][i];
        }
    }
    if (tablero[0][0] != '_' && tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2])
    {
        return tablero[0][0];
    }
    if (tablero[0][2] != '_' && tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0])
    {
        return tablero[0][2];
    }
    return '_';
}

bool TickTacToe::CheckEmpate()
{
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            if (tablero[i][j] == '_')
            {
                return false;
            }
        }
    }
    return true;
}

void TickTacToe::ResetTicTackToe()
{
    endGame = false;
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            tablero[i][j] = '_';
        }
    }
}


