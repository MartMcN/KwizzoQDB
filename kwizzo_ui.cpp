#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "kwizzo_xml.h"
#include "kwizzo.h"
#include "kwizzo_ui.h"

// Rating
#define KIWZZO_EASY     0
#define KWIZZO_MEDIUM   1
#define KWIZZO_HARD     2


// BUTTONS FOR SELECTION
#define COLOR_STD_BUTTON ((ImVec4)ImColor::HSV(0.57f, 0.5f, 0.3f))
#define COLOR_STD_HOVER ((ImVec4)ImColor::HSV(0.57f, 0.5f, 0.7f))

#define COLOR_SELECTED_BUTTON ((ImVec4)ImColor::HSV(0.28f, 1.0f, 0.6f))
#define COLOR_SELECTED_HOVER ((ImVec4)ImColor::HSV(0.28f, 1.0f, 0.7f))
#define COLOR_SELECTED_ACTIVE ((ImVec4)ImColor::HSV(0.28f, 1.0f, 1.0f))

// BUTTON OTHER STYLE
#define COLOR_OTHER_BUTTON ((ImVec4)ImColor::HSV(0.3f, 0.4f, 0.4f))
#define COLOR_OTHER_HOVER ((ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f))
#define COLOR_OTHER_ACTIVE ((ImVec4)ImColor::HSV(0.3f, 0.8f, 0.8f))

kwizzo_window::kwizzo_window(kwizzo_question *ptr, int width)
{
    ptr_kwizzo_question = ptr;
    window_width = width;
    window();
}

kwizzo_window::~kwizzo_window()
{

}


void kwizzo_window::window()
{
    //
    bool p_open;
    // Set window position

    ImVec2 window_postion;
    window_postion.x = 0;
    window_postion.y = 0;
    ImGui::SetNextWindowPos(window_postion);

    ImGui::Begin("KWIZZO!", &p_open, ImGuiWindowFlags_NoDecoration);

    ImGui::SetWindowSize(ImVec2(window_width, 800));


    kwizzo_windows_next_prev();
    ImGui::Separator();

    // Question and Answer
    kwizzo_window_question();
    ImGui::Separator();

    // Ratings
    kwizzo_window_rating();
    ImGui::Separator();

    // Catagory
    kwizzo_window_catagory();

    ImGui::End();
}

void kwizzo_window::kwizzo_windows_next_prev()
{
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_OTHER_BUTTON);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_OTHER_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_OTHER_ACTIVE);

    ImGui::Button("PREV");
    if(ImGui::IsItemActivated())
    {
        ptr_kwizzo_question->load_prev_question();
    }

    ImGui::SameLine();
    
    ImGui::Button("NEXT");
    if(ImGui::IsItemActivated())
    {
        ptr_kwizzo_question->load_next_question();
    }

    ImGui::PopStyleColor(3);
}

void kwizzo_window::kwizzo_window_question()
{
    // Q and A
    //ImGui::InputTextMultiline("QUESTION", ptr_kwizzo_question->current_edited.question, IM_ARRAYSIZE(ptr_kwizzo_question->current_edited.question), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4) );
    
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("QUESTION"))
    {
        ImGui::TextWrapped(ptr_kwizzo_question->current_edited.question);
        
        ImGui::TreePop();
    }
    
    ImGui::Separator();

    //ImGui::InputTextMultiline("ANSWER", ptr_kwizzo_question->current_edited.answer, IM_ARRAYSIZE(ptr_kwizzo_question->current_edited.answer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4) );
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("ANSWER"))
    {
        ImGui::TextWrapped(ptr_kwizzo_question->current_edited.answer);
        ImGui::TreePop();
    }

}


void kwizzo_window::kwizzo_window_rating()
{
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_SELECTED_ACTIVE);

    for( uint8_t index = 0; index < MAX_RATINGS; index++)
    {
        if(index != 0)
            ImGui::SameLine();

        if(ptr_kwizzo_question->current_edited.rating[index])
        {
            // rating is active
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_SELECTED_BUTTON);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_SELECTED_HOVER);
        }
        else
        {
            // rating is inactive
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_STD_BUTTON);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_STD_HOVER);         
        }
        
        switch(index)
        {
            case KIWZZO_EASY:
            ImGui::Button("EASY");
            break;

            case KWIZZO_MEDIUM:
            ImGui::Button("MEDIUM");
            break;
            
            case KWIZZO_HARD:
            ImGui::Button("HARD");
            break;

            default:
                break;
        }

        ImGui::PopStyleColor(2);

        if(ImGui::IsItemActivated())
        {
            ptr_kwizzo_question->current_edited.rating[KIWZZO_EASY] = false;
            ptr_kwizzo_question->current_edited.rating[KWIZZO_MEDIUM] = false;
            ptr_kwizzo_question->current_edited.rating[KWIZZO_HARD] = false;
            ptr_kwizzo_question->current_edited.rating[index] = true;
        }

    }


    ImGui::PopStyleColor(1);
}

void kwizzo_window::kwizzo_window_catagory()
{
    uint8_t catagory_count;
    uint8_t index = 0;
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_SELECTED_ACTIVE);

    catagory_count = ptr_kwizzo_question->get_catagory_count();

    while(catagory_count--)
    {
        if(index%4)
            ImGui::SameLine();

        if(ptr_kwizzo_question->current_edited.catagory[index])
        {
            // Catagory is active
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_SELECTED_BUTTON);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_SELECTED_HOVER);
        }
        else
        {
            // Catagory is inactive
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_STD_BUTTON);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_STD_HOVER);         
        }
        
        const char * catagorytext = ptr_kwizzo_question->get_catagory_text(index);
        ImGui::Button(catagorytext);
        if(ImGui::IsItemActivated())
        {
           if(ptr_kwizzo_question->current_edited.catagory[index])
                ptr_kwizzo_question->current_edited.catagory[index] = false;
           else
                ptr_kwizzo_question->current_edited.catagory[index] = true;
 
        }

        ImGui::PopStyleColor(2);

        index++;
    }

    ImGui::PopStyleColor(1);
}