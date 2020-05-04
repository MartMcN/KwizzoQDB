class kwizzo_window
{
public:
    kwizzo_window(kwizzo_question *ptr, int width);
    ~kwizzo_window();

    void window();

private:
    // methods to draw components of the window
    void kwizzo_menubar();
    void kwizzo_windows_next_prev();
    void kwizzo_window_question();
    void kwizzo_window_rating();
    void kwizzo_window_catagory();
    void kwizzo_edit_text();

    // Pointer to the kwizzo question object
    kwizzo_question *ptr_kwizzo_question;

    // Windows width for dynamic resize
    int  window_width;
};
