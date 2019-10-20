#ifndef START_PROMPT_H
#define START_PROMPT_H

#include <string>

class StartPrompt {
   public:
    void show_window();

    std::string player_name{};

   private:
    void show_setup_page();
    
    bool show{true};
};

#endif