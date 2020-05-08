#include "window.cpp"

int main()
{
    float scale = 2.0f;
    Window window(scale);
    window.setup();

    while(window.isWindowOpen())
    {
        if(!(window.isStartPressed()) && !(window.isPointsPressed())) {
            window.start_menu();
        }
        else if(!(window.isEnterPressed()) && window.game_finished) {
            window.insert_score();
        }
        else if(window.isStartPressed()) {
            window.loop();
            window.show();
        }else if(window.isPointsPressed()) {
            window.score_board();
        }
    }

    return 0;
}