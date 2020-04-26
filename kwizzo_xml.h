#include "tinyxml2.h"

class kwizzo_xml
{
public:
    kwizzo_xml();
    ~kwizzo_xml();

    bool kwizzo_xml_next_quiz();
    bool kwizzo_xml_prev_quiz();
    bool kwizz_xml_first_quiz();
    bool kwizzo_xml_question(char *buffer);
    bool kwizzo_xml_answer(char *buffer);
    bool kwizzo_xml_catagory(char *buffer);

private:
    tinyxml2::XMLDocument *doc;
    tinyxml2::XMLElement *quizz_root;
    tinyxml2::XMLElement *quizz;
};

