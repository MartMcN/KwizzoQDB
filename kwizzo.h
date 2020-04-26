#define MAX_TEXT        1024
#define MAX_RATINGS     3
#define MAX_CATAGORIES  6

typedef struct {
    char            question[MAX_TEXT];  
    char            answer[MAX_TEXT];
    bool            rating[MAX_RATINGS];
    bool            catagory[MAX_CATAGORIES];
} kwizzo_question_t;


class kwizzo_xml;

class kwizzo_question 
{
public:
    kwizzo_question(kwizzo_xml *kwizzo_db);
    ~kwizzo_question();

    kwizzo_question_t *get_current_question();
    void update_current_question();

    void load_next_question();
    void load_prev_question();
    void save_changes();
    void undo_changes();

    kwizzo_question_t current_edited;
private:
    kwizzo_question_t current;
    kwizzo_xml *kwizzo_db;
};

///////////
///////////
///////////


class kwizzo_window
{
public:
    kwizzo_window(kwizzo_question *ptr, int width);
    ~kwizzo_window();

    void window();

private:
    // methods to draw components of the window
    void kwizzo_windows_next_prev();
    void kwizzo_window_question();
    void kwizzo_window_rating();
    void kwizzo_window_catagory();

    // Pointer to the kwizzo question object
    kwizzo_question *ptr_kwizzo_question;

    // Windows width for dynamic resize
    int  window_width;
};


