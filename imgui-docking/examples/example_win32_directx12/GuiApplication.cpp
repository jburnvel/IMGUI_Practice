#include "imgui.h"
#include <string>
#include <vector>
#include <set>
#include <list>
#include "GuiApplication.h"
#include <iostream>




//-----------------------------------------------------------------------------
// [SECTION] Helpers
//-----------------------------------------------------------------------------

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpAppMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
} 



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
    ganador = ' ';
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


void GUIApp::GuiApplication::AlignForWidth(float width, float alignment = 0.5f)
{
    //ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
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

//ImGuiStyle& style = ImGui::GetStyle();
float width = 300.0f;
AlignForWidth(width,0.5f);

ImGui::BeginGroup();
    char buf[32];
    sprintf_s(buf, "Turno de las %c", jugador);
    ImGui::Text(buf);

        for (auto i = 0; i < 3; i++)
        {
            string num = " a ";

            for (auto j = 0; j < 3; j++)
            {
                num = tablero[i][j];//to_string(i) + " - " + to_string(j);


                ImGui::PushID(i*3+j);
                if (num == "_") {
                    
                    if (!endGame){
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
                        ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
                    }

                }
                else if(num == "X") {
                    ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
                }
                else if (num == "O") {
                    ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
                }
                string actual = turno? "X" : "O";
                if (endGame)actual = " ";
                if (ImGui::Button(num == "_"? actual.c_str() :(num).c_str(), ImVec2(100, 100)))
                {
                    if (endGame==false && tablero[i][j] == '_') {
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
            if (ImGui::Button("Reset Tick Tac Toe Game",ImVec2(350,50)))
            {
                ResetTicTackToe();
            }
        }
    ImGui::EndGroup();
    ImGui::End();
    
}
void GUIApp::GuiApplication::ConsolePanel(float* _value)
{
    ImGui::Begin("Console");
    //IMGUI_APP_MARKER("Tables/Basic");
    HelpAppMarker("Iterators are used to point at the memory addresses of STL containers. They are primarily used in sequences of numbers, characters etc. They reduce the complexity and execution time of the program.");
    if (ImGui::TreeNode("Iterators  -----------"))
    {
        
        ImGui::Text("Vector int iterator");
        std::vector<int> vect;

        for (int contador = 1; contador <= 6; ++contador)
            vect.push_back(contador);

        std::vector<int>::const_iterator it; // declara un iterator de solo lectura
        it = vect.cbegin(); // 7 lo asigna al comienzo del vector
        while (it != vect.cend()) // Mientras no ha llegado al final
        {
            char buf[32];
            sprintf_s(buf, "Vect %d ", *it);
            //ImGui::SameLine();
            ImGui::Text(buf); // imprime el valor al que apunta tras indireccionarlo
            ++it; // y pasamos al siguiente elemento
        }

        ImGui::Separator();
        ImGui::Text("List int iteratdor");
        std::list<int> li;
        for (int contador = 1; contador <= 6; ++contador)
            li.push_back(contador);

        std::list<int>::const_iterator itl; // declara un iterator de solo lectura
        itl = li.cbegin(); // 7 lo asigna al comienzo del vector
        while (itl != li.cend()) // Mientras no ha llegado al final
        {
            char buf[32];
            sprintf_s(buf, "List %d ", *itl);
            //ImGui::SameLine();
            ImGui::Text(buf); // imprime el valor al que apunta tras indireccionarlo
            ++itl; // y pasamos al siguiente elemento
        }

        ImGui::Separator();
        ImGui::Text("Set int iteratdor");
        std::set<int> miset;
        miset.insert(2);
        miset.insert(33);
        miset.insert(63);
        miset.insert(21);
        miset.insert(16);
        miset.insert(88);

        std::set<int>::const_iterator its; // declara un iterator de solo lectura
        its = miset.cbegin(); // 7 lo asigna al comienzo del vector
        while (its != miset.cend()) // Mientras no ha llegado al final
        {
            char buf[32];
            sprintf_s(buf, "Set %d - ", *its);
            //ImGui::SameLine();
            ImGui::Text(buf); // imprime el valor al que apunta tras indireccionarlo
            ++its; // y pasamos al siguiente elemento
        }
        ImGui::TreePop();
    }



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


