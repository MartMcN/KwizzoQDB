#include <cstddef>
#include <iostream>
#include <string.h>
#include "kwizzo.h"
#include "kwizzo_xml.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"


// Rating
#define KIWZZO_EASY     0
#define KWIZZO_MEDIUM   1
#define KWIZZO_HARD     2


struct catagory_entry{
    uint8_t cat_index;
    const char* cat_string;
};

catagory_entry catagorys[MAX_CATAGORIES] = 
{
    {0,  "Geography"},
    {1,  "Food & Drink"},
    {2,  "History"},
    {3,  "Music & Lyrics"},
    {4,  "TV, Cinema & Radio"},
    {5,  "Sport"},
    {6,  "Maritime"},
    {7,  "Animals, Plants & Botany"},
    {8,  "Literature"},
    {9,  "Science & Medicine"},
    {10, "Arts"},
    {11, "General Knowledge"}
};

//
// "Geography"},
// "Food &amp; Drink"bool kwizzo_xml::kwizzo_xml_rating(char *buffer)
// "Animals, Plants &amp; Botany"
// "Literature"
// "Science &amp; Medicine"
// "Arts"
// "General Knowledge"
//
// catagory_entry catagorys[MAX_CATAGORIES] = 
// {
//     {0, "ARTS & LIT"},
//     {1, "ENTS"},
//     {2, "HISTORY"},
//     {3, "GEOGRAPHY"},
//     {4, "SCIENCE"},
//     {5, "SPORT"}
// };
//


kwizzo_question::kwizzo_question(kwizzo_xml *kwizzo_db)
{
    // KWIZZO KWIZZO
    // Initalise the curent question
    kwizzo_question::kwizzo_db = kwizzo_db;

    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // Stick in some data now
    kwizzo_db->kwizzo_xml_next_quiz();
    kwizzo_db->kwizzo_xml_question(current.question);
    kwizzo_db->kwizzo_xml_answer(current.answer);
    decode_xml_rating(current.rating);
    decode_xml_catagory(current.catagory);
    
    // Copy over to the edited version
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}


kwizzo_question::~kwizzo_question()
{

}

kwizzo_question_t *kwizzo_question::get_current_question()
{
    return &current;
}


void kwizzo_question::load_next_question()
{
    // Initalise the curent question
    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // Stick in some data now
    kwizzo_db->kwizzo_xml_next_quiz();

    // Get the question and answer text
    kwizzo_db->kwizzo_xml_question(current.question);
    kwizzo_db->kwizzo_xml_answer(current.answer);

    decode_xml_rating(current.rating);
    decode_xml_catagory(current.catagory);

    // Copy over to the edited version
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}


void kwizzo_question::decode_xml_rating(bool *rating_array)
{
    // Clear the array
    for (uint8_t index = 0; index < MAX_RATINGS; index++)
    {
        rating_array[index] = false;
    }

    // Attempt to read the XML rating element
    char rating_txt[32];
    memset(rating_txt, 0, 32);
    
    // Update the rating array
    if(kwizzo_db->kwizzo_xml_rating(rating_txt))
    {
        if(strncmp(rating_txt, "EASY", 4))
        {
            rating_array[0] = true;
        }
        else if(strncmp(rating_txt, "MEDIUM", 4))
        {
            rating_array[1] = true;
        }
        else if(strncmp(rating_txt, "MEDIUM", 4))
        {
            rating_array[2] = true;
        }
    }

}

void kwizzo_question::decode_xml_catagory(bool *catagory_array)
{
    // Clear the array
    for (uint8_t index = 0; index < MAX_CATAGORIES; index++)
    {
        catagory_array[index] = false;
    }

    // Attempt to read the XML catagory element
    char catagory_txt[64];
    memset(catagory_txt, 0, 64);
    
    // Update the rating array
    if(kwizzo_db->kwizzo_xml_catagory(catagory_txt))
    {
        for(uint8_t index = 0; index < MAX_CATAGORIES; index++)

        if(strncmp(catagory_txt, catagorys[index].cat_string, 4) == 0)
        {
            catagory_array[index] = true;
        }
        else
        {
            catagory_array[index] = false;
        }
    }
}

void kwizzo_question::load_prev_question()
{
    // Initalise the curent question
    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // Stick in some data now
    kwizzo_db->kwizzo_xml_prev_quiz();
    kwizzo_db->kwizzo_xml_question(current.question);
    kwizzo_db->kwizzo_xml_answer(current.answer);

    // Copy over to the edited version
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}

//////////

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
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_SELECTED_ACTIVE);

    for( uint8_t index = 0; index < MAX_CATAGORIES; index++)
    {
        if(index != 0)
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
        

        ImGui::Button(catagorys[index].cat_string);
        if(ImGui::IsItemActivated())
        {
           if(ptr_kwizzo_question->current_edited.catagory[index])
                ptr_kwizzo_question->current_edited.catagory[index] = false;
           else
                ptr_kwizzo_question->current_edited.catagory[index] = true;
 
        }

        ImGui::PopStyleColor(2);
    }


    ImGui::PopStyleColor(1);
}