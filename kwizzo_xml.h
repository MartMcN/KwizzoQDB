#include "tinyxml2.h"

class kwizzo_xml
{
public:
    kwizzo_xml();
    ~kwizzo_xml();

    bool kwizzo_xml_next_quiz();            // updates the quiz object pointer
    bool kwizzo_xml_prev_quiz();            // update the quiz object pointer
    bool kwizz_xml_first_quiz();            // get the first quiz object

    bool kwizzo_xml_question(char *buffer); // Returns text of the question element
    bool kwizzo_xml_answer(char *buffer);   // Returns text of the answer element
    bool kwizzo_xml_catagory(char *buffer); // Returns the text of the catagory element
    bool kwizzo_xml_rating(char *buffer);   // Returns the text of rating element

    bool kwizzo_xml_update_question(char *buffer);      // Update the text of the question elment
    bool kwizzo_xml_update_answer(char *buffer);        // Update the text of the question elment
    bool kwizzo_xml_update_catagory(char *buffer);      // Update the text of the question elment
    bool kwizzo_xml_update_rating(char *buffer);        // Update the text of the question elment

private:
    tinyxml2::XMLDocument *doc;             // points to the doc
    tinyxml2::XMLElement *quizz_root;       // points to the root node
    tinyxml2::XMLElement *quizz;            // pointer to the quiz xml object
};

