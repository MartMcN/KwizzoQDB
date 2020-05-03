#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "kwizzo_xml.h"
#include "kwizzo.h"
#include "kwizzo_ui.h"

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

kwizzo_question::kwizzo_question(kwizzo_xml *kwizzo_question_db)
{
    // KWIZZO KWIZZO
    // Initalise the curent question
    kwizzo_db = kwizzo_question_db;

    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // read in data from the the current quizz element
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
    // save quizz if changes
    save_question();

    std::cout << "\n\n" << __FILE__ << " *** LOADING NEXT QUESION ***\n";

    // Initalise the curent question
    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // Update to the next question
    kwizzo_db->kwizzo_xml_next_quiz();

    // Get the question and answer text
    kwizzo_db->kwizzo_xml_question(current.question);
    kwizzo_db->kwizzo_xml_answer(current.answer);

    decode_xml_rating(current.rating);
    decode_xml_catagory(current.catagory);

    // Copy over to the edited version
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}


void kwizzo_question::load_prev_question()
{
    // save quizz if changes
    save_question();

    std::cout << "\n\n" << __FILE__ << " *** LOADING PREV QUESION ***\n";

    // Initalise the curent question
    memset(&current, 0, sizeof(kwizzo_question_t));
    memset(&current_edited, 0, sizeof(kwizzo_question_t));

    // Stick in some data now
    kwizzo_db->kwizzo_xml_prev_quiz();

    kwizzo_db->kwizzo_xml_question(current.question);
    kwizzo_db->kwizzo_xml_answer(current.answer);

    decode_xml_rating(current.rating);
    decode_xml_catagory(current.catagory);

    // Copy over to the edited version
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}

void kwizzo_question::reload_question()
{
    memset(&current_edited, 0, sizeof(kwizzo_question_t));
    memcpy(&current_edited, &current, sizeof(kwizzo_question_t));
}

void kwizzo_question::save_question()
{
    bool updated = false;

    // check is for changes in rating
    if(memcmp(current_edited.rating, current.rating, sizeof(current_edited.rating)) != 0)
    {
        std::cout << __FILE__<< " !!! rating updated !!!\n";
        updated = true;
    }
    
    // check is for changes in catagory
    if( !updated && memcmp(current_edited.catagory, current.catagory, sizeof(current_edited.catagory)) != 0)
    {
        std::cout << __FILE__<< " !!! catagory updated !!!\n";
        updated = true;
    }

    if(updated)
    {
         std::cout << "updating..\n";
        encode_xml_rating(current_edited.rating);
        encode_xml_catagory(current_edited.catagory);
    }

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
        if(strcmp(rating_txt, "EASY") == 0 )
        {
            rating_array[0] = true;
            std::cout << __FILE__ << " EASY = TRUE\n";
        }
        else if(strcmp(rating_txt, "MEDIUM") == 0 )
        {
            rating_array[1] = true;
            std::cout << __FILE__ << " MEDIUM = TRUE\n";
        }
        else if(strcmp(rating_txt, "HARD") == 0 )
        {
            rating_array[2] = true;
            std::cout << __FILE__ << " HARD = TRUE\n";
        }
    }

    std::cout << __FILE__ << " Rating Array: " << rating_txt << "\n";
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

    
    // Update the rating array
    uint8_t catagory_count;

    for(catagory_count = 0; catagory_count < MAX_CATAGORIES; catagory_count++ )
    {
        memset(catagory_txt, 0, 64);

        if (kwizzo_db->kwizzo_xml_catagory(catagory_count, catagory_txt))
        {
            
            for (uint8_t index = 0; index < MAX_CATAGORIES; index++)
            {
                if (strncmp(catagory_txt, catagorys[index].cat_string, 4) == 0)
                {
                    catagory_array[index] = true;
                    std::cout << __FILE__ << " Element Catagory text: " << catagory_txt << "\n";
                }
            }
        }

    }
}


void kwizzo_question::encode_xml_rating(bool *rating_array)
{
    char rating_txt[16];
    memset(rating_txt, 0, 16);

    for (uint8_t index = 0; index < MAX_RATINGS; index++)
    {
        if(rating_array[index] == true)
        {
            if(index == 0)
            {
                strncpy(rating_txt, "EASY", 5);
            }
            else if (index == 1)
            {
                strncpy(rating_txt, "MEDIUM", 7);
            }
            else if (index == 2)
            {

				strncpy(rating_txt, "HARD", 5);
            }
            
            break;
        }

    }

    std::cout << __FILE__ << " Encoded rating: " << rating_txt << "\n";
    kwizzo_db->kwizzo_xml_update_rating(rating_txt);
}


void kwizzo_question::encode_xml_catagory(bool *catagory_array)
{
    // First delete all catagories
    kwizzo_db->kwizzo_xml_delete_catagory();

    for (uint8_t index = 0; index < MAX_CATAGORIES; index++)
    {
        if(catagory_array[index] == true)
        {
            kwizzo_db->kwizzo_xml_update_catagory(catagorys[index].cat_string);
        }
    }
}


uint8_t kwizzo_question::get_catagory_count()
{
    return MAX_CATAGORIES;
}

const char *kwizzo_question::get_catagory_text(uint8_t index)
{
    if(index >= MAX_CATAGORIES)
        return "error";

    return catagorys[index].cat_string;
}





std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%d_%H-%M-%S",timeinfo);
    return std::string(buffer);
}


bool filecopy(std::string fnamein, std::string fnameout)
{
    std::ifstream fin(fnamein, std::ios::binary);
    if (!fin)
    {
        std::cout << "could not open input: " << fnamein << '\n';
        return false;
    }
    std::ofstream fout(fnameout, std::ios::binary);
    if (!fout)
    {
        std::cout << "could not open output: " << fnameout << '\n';
        return false;
    }    
    fout << fin.rdbuf();
    return bool(fout);
}


void file_backup()
{
    std::string savefile;

    savefile = XML_FILE_NAME_PARTIAL + datetime() + ".xml";

    filecopy(XML_FILE_NAME, savefile);
}

void kwizzo_question::save_file()
{
    // Save any updates to the current question
    save_question();

    // Before writing to the current file backup it up
    file_backup();
    
    std::string filename;
    filename.assign(XML_FILE_NAME);

    // Call the TinyXML2 to write the XML file
    kwizzo_db->kwizzo_xml_file_save(filename);
}

