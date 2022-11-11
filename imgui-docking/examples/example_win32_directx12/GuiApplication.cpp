#include "imgui.h"
#include <string> 
#include "GuiApplication.h"


using namespace std;

GUIApp::GuiApplication::GuiApplication()
{
    flag = false;
    value = 25.0f;
    MAX_Range = 25.0f;
    MIN_Range = 0.01f;
    rows = 3;
    columns = 3;
    turno = true;
    jugador = turno ? 'X' : 'O';
    endGame = false;
}

GUIApp::GuiApplication::~GuiApplication()
{

}

void GUIApp::GuiApplication::DrawAppGUI()
{    
    DockAndMainMenu();
    ConsolePanel(&value);
    ConsoleWindow(&value);
}

void GUIApp::GuiApplication::DockAndMainMenu()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open", NULL);
            ImGui::MenuItem("Save..", NULL);
            ImGui::MenuItem("Exit", NULL);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

void GUIApp::GuiApplication::ConsoleWindow(float* _value)
{
    ImGui::Begin("Panel");
    if (ImGui::Button("Reset")) { *_value = 0; }
    std::string str = std::to_string(*_value);
    ImGui::Text((str+" \n Result").c_str());


    if (ImGui::Button("- columns")) { columns--; }
    ImGui::SameLine();
    if (ImGui::Button("+ columns")) { columns++; }
    if (ImGui::Button("- rows")) { rows--; }
    ImGui::SameLine();
    if (ImGui::Button("+ rows")) { rows++; }
    //Image = new cSprite(m_pDevice, "C://Image.png");
    //ImGui::ImageButton(Image->GetTexture(), ImVec2(200.f, 200.f)); 
    for (auto i = 0; i < rows; i++)
    {
        string num = " a ";
        
        for (auto j = 0; j < columns; j++)
        {
            num = to_string(i) + " - " + to_string(j);
            
            ImGui::Button(num.c_str(),ImVec2(100,100));
            if(j<columns-1)
                ImGui::SameLine();
        }
    }
    ImGui::Separator();
    
    ImGui::End();

ImGui::Begin("TickTacToe");


//
//string nums = "" + tablero[0][0];
//if (ImGui::Button(nums.c_str(), ImVec2(100, 100)))
//{
//    tablero[2][2] = jugador;
//    jugador = turno ? 'X' : 'O';
//    turno = !turno;
//}

char buf[32];
sprintf(buf, "Turno de las %c", jugador);
ImGui::Text(buf);
    for (auto i = 0; i < 3; i++)
    {
        string num = " a ";

        for (auto j = 0; j < 3; j++)
        {
            num = tablero[i][j];//to_string(i) + " - " + to_string(j);

            ImGui::PushID(i*3+j);
            if (ImGui::Button((num).c_str(), ImVec2(100, 100)))
            {
                if (endGame==false && tablero[i][j] == '_') {
                    tablero[i][j] = jugador;
                    turno = !turno;
                    jugador = turno ? 'X' : 'O';
                }
            }
            ImGui::PopID();
            if (j < 3 - 1)
                ImGui::SameLine();
        }
    }
    char ganador = CheckGanador();
    if (ganador != '_') {
        char buf[32];
        sprintf(buf, " Ganador %c", ganador);
        ImGui::Text(buf);
        endGame = true;
    }

    if (CheckEmpate() == true && ganador == '_') {
        char buf[32];
        sprintf(buf, " Empate ");
        ImGui::Text(buf);
        endGame = true;
    }
    if (endGame) {
        if (ImGui::Button("Reset Tick Tac Toe Game",ImVec2(350,50)))
        {
            ResetTicTackToe();
        }
    }
    ImGui::End();
    
}

void GUIApp::GuiApplication::ConsolePanel(float* _value)
{
    ImGui::Begin("Console");

    if (flag == false)
    {
        *_value -= 0.1f;
        if (*_value < MIN_Range) flag = true;
    }
    else
    {
        *_value += 0.1f;
        if (*_value > MAX_Range) flag = false;
    }

    ImGui::DragFloat(" ", _value);
        ImGui::SameLine();
    if (ImGui::Button("+100")) { *_value += 100; }
        ImGui::SameLine();
    if (ImGui::Button("-100")) { *_value += -100;}

    ImGui::End();
}

char GUIApp::GuiApplication::CheckGanador()
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
            return tablero[i][0];
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

bool GUIApp::GuiApplication::CheckEmpate()
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

void GUIApp::GuiApplication::ResetTicTackToe()
{
    endGame = false;
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
             tablero[i][j]='_';
             
        }
    }
}


