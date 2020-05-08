//Tetris-sfml 
#include <SFML/Graphics.hpp>    //SFML SIMPLE FAST MULTIMEDIA LIBRARY
#include <iostream>             //for "std::cout"
#include <time.h>               //for "srand(time(NULL))""
#include <cstdlib>              //for "rand()"
#include <cmath>
#include <SFML/Audio.hpp>

#include "game.cpp"
#include "mainmenu.cpp"
#include "insertscore.cpp"
#include "scoreboard.cpp"

//////// virtual field resolution ////////
#define WIDTH 160
#define HEIGHT 320

class Window
{
private:
//programm scale
    float scale;
    Game game;
//mWindows attributes
    int wx;
    int wy;
    sf::RenderWindow mWindow;
//gameplay variables
    float delta_down = 1.0f;
    float defaul_delta_down = 1.0f;
private:
    sf::Texture t;
    sf::Texture t2;
    sf::Sprite frame;
    sf::Sprite background;
    sf::Font font;
    sf::Text points;
    sf::Text lines;
    sf::Text instruction1;
    sf::Text instruction2;
    sf::Text textNext;
private:
    sf::Clock time_for_rotating;
    sf::Clock time_for_moving_x;
    sf::Clock time_for_moving_y;
    sf::Clock time_for_moving_drop;
private:
    MainMenu menu;
    InsertScore insert;
    ScoreBoard score;
    sf::Sprite frame_piece;
    sf::Texture t3;
    sf::SoundBuffer buffer1, buffer2, buffer3;
    sf::Sound rotate, clear, fall;
    sf::Music music;
    int start_music = 0;
public:
    Window(float sca);
    void setup();
    void start_menu();
    void insert_score();
    void score_board();
    void loop();
    void show();
    bool isStartPressed();
    bool isPointsPressed();
    bool isEnterPressed();
    float calc_time();
public:
    bool isWindowOpen();
    bool game_finished = false;
};

Window::Window(float sca)
{
    scale = sca;
    wx = (WIDTH + 100) * scale;
    wy = (HEIGHT) * scale;

    mWindow.create(sf::VideoMode(wx, wy), "TETRIS", sf::Style::Titlebar | sf::Style::Close);
    mWindow.setFramerateLimit(60);
    sf::FloatRect textRect;
    font.loadFromFile("src/pixelmix.ttf");

    t.loadFromFile("src/frame.png");
    frame.setTexture(t);
    frame.setScale(scale, scale);

    //t2.loadFromFile("background.png");
    //background.setTexture(t2);

    buffer1.loadFromFile("src/rotate.wav");
    buffer2.loadFromFile("src/clear.wav");
    buffer3.loadFromFile("src/fall.wav");
    rotate.setBuffer(buffer1);
    clear.setBuffer(buffer2);
    fall.setBuffer(buffer3);

    music.openFromFile("src/theme.wav");
    music.setLoop(true);

    t3.loadFromFile("src/frame1.png");
    frame_piece.setTexture(t3);
    frame_piece.setScale(scale, scale);
    frame_piece.setPosition((167.5)*scale, 7.5*scale);

    points.setFont(font);
    points.setString("POINTS: 0");
    points.setCharacterSize(20);
    points.setFillColor(sf::Color::White);
    textRect = points.getLocalBounds();
    points.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    points.setPosition(sf::Vector2f((195*scale),300*scale));

    lines.setFont(font);
    lines.setString("LINES: 0");
    lines.setCharacterSize(20);
    lines.setFillColor(sf::Color::White);
    textRect = points.getLocalBounds();
    lines.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    lines.setPosition(sf::Vector2f((195*scale),270*scale));

    instruction1.setFont(font);
    instruction1.setString("Q   quit");
    instruction1.setCharacterSize(23);
    instruction1.setFillColor(sf::Color::White);
    textRect = points.getLocalBounds();
    instruction1.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    instruction1.setPosition(sf::Vector2f((205*scale),150*scale));

    instruction2.setFont(font);
    instruction2.setString("ESC menu");
    instruction2.setCharacterSize(23);
    instruction2.setFillColor(sf::Color::White);
    textRect = points.getLocalBounds();
    instruction2.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    instruction2.setPosition(sf::Vector2f((205*scale),170*scale));

    textNext.setFont(font); 
    textNext.setString("next piece");
    textNext.setCharacterSize(20);
    textNext.setFillColor(sf::Color::White);
    textRect = textNext.getLocalBounds();
    textNext.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    textNext.setPosition(sf::Vector2f((205*scale), 100*scale));
}

void Window::setup()
{
    game.setup(scale);
    game.new_piece();
    menu.setup(scale);
    insert.setup(scale);
    score.setup(scale);
    game_finished = false;
    start_music = 0;
}

bool Window::isStartPressed()
{
    return menu.start_pressed;
}

bool Window::isPointsPressed()
{
    return menu.points_pressed;
}

void Window::start_menu()
{
    menu.draw(mWindow);
    menu.update(mWindow);
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            mWindow.close();
    }
}

void Window::insert_score()
{
    insert.draw(mWindow);
    insert.update(mWindow, game.get_points());
    if(insert.enterPressed) setup();
}

void Window::score_board()
{
    score.draw(mWindow);
    score.update(mWindow);
    sf::Event event;
    if(score.back_pressed) setup();
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            mWindow.close();
    }
}

bool Window::isEnterPressed()
{
    return insert.enterPressed;
}

bool Window::isWindowOpen()
{
    return mWindow.isOpen();
}

float Window::calc_time()
{
    int num = float(game.get_lines_cleard()/10);
    switch(num) {
        case 0:
            return 1.0f;
            break;
        case 1:
            return 0.9f;
            break;
        case 2:
            return 0.8f;
            break;
        case 3:
            return 0.7f;
            break;
        case 4:
            return 0.6f;
            break;
        case 5:
            return 0.5f;
            break;
        case 6:
            return 0.35f;
            break;
        case 7:
            return 0.24f;
            break;
        case 8:
            return 0.19f;
            break;
        case 9:
            return 0.13f;
            break;
        default:
            return 0.05f;
            break;
    }
}

void Window::loop()
{
    if(start_music == 0) {music.play(); start_music = 1;}

    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            mWindow.close();
    }

    defaul_delta_down = calc_time();
    sf::Time elapsed_for_moving_y = time_for_moving_y.getElapsedTime();
    sf::Time elapsed_for_rotating = time_for_rotating.getElapsedTime();
    sf::Time elapsed_for_moving_x = time_for_moving_x.getElapsedTime();
    sf::Time elapsed_for_moving_drop = time_for_moving_drop.getElapsedTime();

    if(mWindow.hasFocus()) {
        //std::cout << "time: " << defaul_delta_down << "\t" << "lines: "  << game.get_lines_cleard() << std::endl;

        if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsed_for_rotating.asSeconds() > 0.20 && !(menu.option))
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && elapsed_for_rotating.asSeconds() > 0.20 && (menu.option)) {
            game.rotate();
            rotate.play();
            time_for_rotating.restart();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && elapsed_for_moving_x.asSeconds() > 0.08) {
            game.move(-1, 0);
            time_for_moving_x.restart();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && elapsed_for_moving_x.asSeconds() > 0.08) {
            game.move(1, 0);
            time_for_moving_x.restart();
        }

        if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && elapsed_for_moving_drop.asSeconds() > 0.2 && !(menu.option))
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsed_for_moving_drop.asSeconds() > 0.2 && (menu.option)) {
            fall.play();
            game.hard_drop();
            time_for_moving_drop.restart();
        }
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delta_down = 0.05;
    }

    if(elapsed_for_moving_y.asSeconds() > delta_down) {
            game.move(0, 1);
            time_for_moving_y.restart();
            delta_down = defaul_delta_down;
    }

    game.stop_piece();
    if(game.check_lines()) clear.play();

    points.setString("POINTS: " + std::to_string(game.get_points()));
    lines.setString("LINES: " + std::to_string(game.get_lines_cleard()));

    if(game.check_game_over() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {game_finished = true; music.stop();}

    show();
}

void Window::show()
{
    mWindow.clear(sf::Color::Black);
    //mWindow.draw(background);
    mWindow.draw(frame_piece);
    mWindow.draw(frame);
    mWindow.draw(textNext);
    game.draw(mWindow);
    mWindow.draw(points);
    mWindow.draw(lines);
    mWindow.draw(instruction1);
    mWindow.draw(instruction2);
    mWindow.display();
}