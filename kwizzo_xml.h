#include "tinyxml2.h"

class kwizzo_xml
{
public:
    kwizzo_xml();
    ~kwizzo_xml();

    bool kwizzo_xml_next_quiz();            // updates the quiz object pointer
    bool kwizzo_xml_prev_quiz();            // update the quiz object pointer
    bool kwizz_xml_first_quiz();            // get the first quiz object
    bool kwizzo_xml_question(char *buffer); // Returns text of the qestion element
    bool kwizzo_xml_answer(char *buffer);   // Returns text of the answer element
    bool kwizzo_xml_catagory(char *buffer); // Returns the text of the catagory
    bool kwizzo_xml_rating(char *buffer);   // Returns the text of rating

private:
    tinyxml2::XMLDocument *doc;             // points to the doc
    tinyxml2::XMLElement *quizz_root;       // points to the root node
    tinyxml2::XMLElement *quizz;            // points to the xml question
};

