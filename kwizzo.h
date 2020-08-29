#define MAX_TEXT        1024
#define MAX_RATINGS     4
#define MAX_CATAGORIES  12

#define XML_FILE_NAME             "./resources/quiz.xml"
#define XML_FILE_NAME_PARTIAL     "./resources/quiz_backup_"    

typedef struct {
    char            question[MAX_TEXT];
    char            answer[MAX_TEXT];
    bool            rating[MAX_RATINGS];
    bool            catagory[MAX_CATAGORIES];
} kwizzo_question_t;

class kwizzo_question 
{
public:
    kwizzo_question(kwizzo_xml *kwizzo_question_db);
    ~kwizzo_question();


    kwizzo_question_t *get_current_question();

    void load_next_question();
    void load_prev_question();
    void reload_question();                             // undo any changes to a question
    void clear_question();                              // Clears edited data                             
    void save_question();                               // Saves any changes to the xml
    void new_question();

    // File Operations
    void save_file();

    // ui support
    uint8_t get_catagory_count();                       // Return the number of catagories
    const char *get_catagory_text(uint8_t index);       // Returns the indicated catagory text


    kwizzo_question_t current_edited;                   // question data for editing
private:
    kwizzo_question_t current;                          // question data read from the xml/db
    kwizzo_xml *kwizzo_db;

    // xml support
    void decode_xml_rating(bool *rating_array);
    void decode_xml_catagory(bool *catagory_array);
    void encode_xml_rating(bool *rating_array);
    void encode_xml_catagory(bool *catagory_array);
};

