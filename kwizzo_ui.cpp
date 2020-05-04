#include <stdlib.h>
#include "iostream"
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

    ImGui::Begin("KWIZZO!", &p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);

    ImGui::SetWindowSize(ImVec2(window_width, 800));

    // Menubar
    kwizzo_menubar();
    ImGui::Separator();

    // Next / Prev / Undo
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

void kwizzo_window::kwizzo_menubar()
{

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("New"))
            {
            }

            if (ImGui::MenuItem("Open"))
            {
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Save"))
            {
                ptr_kwizzo_question->save_file();
            }

            if (ImGui::MenuItem("Quit", "Ctrl+Q"))
            {
				std::exit(0);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
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

    const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
    static float UndoButtonWidth = 50.0f; //The 100.0f is just a guess size for the first frame.

    float pos = UndoButtonWidth + ItemSpacing;
    ImGui::SameLine(ImGui::GetWindowWidth() - pos);

    ImGui::Button("UNDO");
    if(ImGui::IsItemActivated())
    {
        // reload the question
        ptr_kwizzo_question->reload_question();
    }

    // Why this work???
    UndoButtonWidth = ImGui::GetItemRectSize().x; //Get the actual width for next frame.

    ImGui::PopStyleColor(3);
}

static void format_text_for_input(char *in_text, char *out_text, float width)
{
    float pixels_per_char = 9.5;             // This is a bit crap martinmc
    uint16_t max_chars_per_line = 0;

    uint16_t char_count_line = 0;
    uint16_t in_text_postion = 0;
    uint16_t out_text_postion = 0;
    uint16_t in_last_space_postion = 0;
    uint16_t out_last_space_position = 0;

    // Calc max chars per line
    max_chars_per_line = width / pixels_per_char;

    while (1)
    {
        //Looping till we get a null terminator (GOOD LORD !!!!! martinmc)
        if (in_text[in_text_postion] == 0)
        {
            out_text[out_text_postion] = 0;
            break;
        }

        // Copy over the character
        out_text[out_text_postion] = in_text[in_text_postion];

        // Remember the postion of the last space
        if(in_text[in_text_postion] == ' ')
        {
            in_last_space_postion = in_text_postion;
            out_last_space_position = out_text_postion;
        }

        // Update the indexs
        out_text_postion++;
        in_text_postion++;
        char_count_line++;

        if (char_count_line >= max_chars_per_line)
        {
            // Max chars per line reached, should rewind to the last space then add a CR after it.
            char_count_line = 0;

            out_text_postion = out_last_space_position + 1;
            out_text[out_text_postion] = '\n';
            out_text_postion++;

            in_text_postion = in_last_space_postion + 1;
        }
    }
}

static void format_remove_cr_lf(char *out_text)
{
    float pixels_per_char = 10;
    uint16_t max_chars_per_line = 0;

    uint16_t scan_position = 0;
    uint16_t position = 0;

    while (1)   // Another good lord
    {
        if(out_text[scan_position] == '\n' || out_text[scan_position] == '\r')
        {
            // Replace the carrage returns and linefeeds with a space if none before it
            // otherwise skip them.

            if (out_text[scan_position - 1] != ' ' && scan_position)
            {
                out_text[position] = ' ';
                position++;
            }

            scan_position++;
        }
        else
        {
            out_text[position] = out_text[scan_position];
            scan_position++;
            position++;
        }
        
        if(out_text[position] == 0)
            break;
    }
}


void kwizzo_window::kwizzo_window_question()
{
    // Q and A
    static char out_textr[1024];
    static float saved_window_x = ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x;
    float window_x = (ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    //
    // QUESTION
    //
    if (ImGui::TreeNode("QUESTION"))
    {
        ImGui::TextWrapped(ptr_kwizzo_question->current_edited.question);

        if(ImGui::Button("Edit"))
        {
            ImGui::OpenPopup("Question Edit");
            format_text_for_input(ptr_kwizzo_question->current_edited.question, out_textr, window_x);
        }

        // POP UP
        if (ImGui::BeginPopupModal("Question Edit", NULL, 0))
        {
            ImGui::SetCursorPosX(0);

            // Only do this if the window changes size
            if( saved_window_x != window_x)
            {
                saved_window_x = window_x;
                format_text_for_input(ptr_kwizzo_question->current_edited.question, out_textr, window_x);
            }

            ImGui::InputTextMultiline("source", out_textr, 1024, ImVec2(window_x, ImGui::GetTextLineHeight() * 4), 0);

             ImGui::Separator();
            if (ImGui::Button("Update", ImVec2(120, 0)))
            {
                // Remove the formatting for the input text box and copy back
                format_remove_cr_lf(out_textr);

                strncpy(ptr_kwizzo_question->current_edited.question, out_textr, 1024);                

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }
    
    ImGui::Separator();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    //
    // Answer
    //
    if (ImGui::TreeNode("ANSWER"))
    {
        ImGui::TextWrapped(ptr_kwizzo_question->current_edited.answer);
        if(ImGui::Button("Edit"))
        {
            ImGui::OpenPopup("Answer Edit");
            format_text_for_input(ptr_kwizzo_question->current_edited.answer, out_textr, window_x);
        }


        // POP UP
        if (ImGui::BeginPopupModal("Answer Edit", NULL, 0))
        {
            ImGui::SetCursorPosX(0);

            // Only do this if the window changes size
            if( saved_window_x != window_x)
            {
                saved_window_x = window_x;
                format_text_for_input(ptr_kwizzo_question->current_edited.answer, out_textr, window_x);
            }

            ImGui::InputTextMultiline("source", out_textr, 1024, ImVec2(window_x, ImGui::GetTextLineHeight() * 4), 0);

             ImGui::Separator();
            if (ImGui::Button("Update", ImVec2(120, 0)))
            {
                // Remove the formatting for the input text box and copy back
                format_remove_cr_lf(out_textr);

                strncpy(ptr_kwizzo_question->current_edited.answer, out_textr, 1024);                

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

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


void kwizzo_window::kwizzo_edit_text()
{

}