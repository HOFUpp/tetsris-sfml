//Game-sfml 
#include <SFML/Graphics.hpp>    //SFML SIMPLE FAST MULTIMEDIA LIBRARY
#include <time.h>               //for "srand(time(NULL))""
#include <cstdlib>              //for "rand()"

//////// virtual field resolution ////////
#define WIDTH 160
#define HEIGHT 320

//////// field dimension //////// 
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

//////// block dimension ////////
#define BLOCK_DIMENSION 16








//////// field declaration ////////
int field[FIELD_HEIGHT][FIELD_WIDTH] = {0};

//I,J,L,O,S,T,Z
class Game
{
private:
    sf::Sprite tile, ghost_tile, preview_tile;
    sf::Texture texture;
    float scale = 1.f;
    int rotation_state = 0;
    int tetramino_current_num;
    int tetramino_future_num;
    sf::Vector2f main[4];
    int rotation_reference[7] = {1, 2, 2, 3, 3, 2, 2};
    int tetramino_setup[7][4][2] = {
        {{3,1},{4,1},{5,1},{6,1}},      //I
        {{4,0},{4,1},{5,1},{6,1}},      //J
        {{5,0},{3,1},{4,1},{5,1}},      //L
        {{4,0},{5,0},{4,1},{5,1}},      //O
        {{4,0},{5,0},{3,1},{4,1}},      //S
        {{4,0},{3,1},{4,1},{5,1}},      //T
        {{4,0},{5,0},{5,1},{6,1}}       //Z
    }; 
    int points = 0;   
private:
    sf::Clock time_for_stoping;
    sf::Time elapsed_for_stoping_start;
    sf::Time elapsed_for_stoping_finish;
    bool stoping = false;
    float stoping_time = 0.3f;
    int lines_cleard = 0;
public:
//constructor and destructor
    Game();
    void setup(float sca);
//window functions
    void draw(sf::RenderWindow& window);
//points functions
    int get_points();
//piece functions
    void new_piece();
    void move(float dx, float dy);
    void hard_drop();
    void ghost_piece(sf::RenderWindow& window);
    void preview_piece(sf::RenderWindow& window);
    void rotate();
    void stop_piece();
//lines functions
    int check_lines();
    void remove_lines(int line);
    int get_lines_cleard();
//generale game functions
    bool check_game_over();
};

Game::Game()
{
    srand(time(NULL));
    texture.loadFromFile("src/tetraminos.png");
    tetramino_future_num = rand() % 7;
}

void Game::setup(float sca)
{
    scale = sca;
    tile.setTexture(texture);
    tile.setScale(scale, scale);

    ghost_tile.setTexture(texture);
    ghost_tile.setScale(scale, scale);

    preview_tile.setTexture(texture);
    preview_tile.setScale(scale, scale);

    points = 0;
    lines_cleard = 0;

    for(int i = 0; i < FIELD_HEIGHT; ++i)
        for(int j = 0; j < FIELD_WIDTH; ++j)
            field[i][j] = 0;
}

int Game::get_lines_cleard()
{
    return lines_cleard;
}

int Game::get_points()
{
    return points;
}

void Game::new_piece()
{
    tetramino_current_num = tetramino_future_num;
    tetramino_future_num = rand() % 7;

    for(int i = 0; i < 4; i++) {
        main[i].x = tetramino_setup[tetramino_current_num][i][0];
        main[i].y = tetramino_setup[tetramino_current_num][i][1];
    }
    move(0, -1);
    stoping_time = 0.3f;
}

bool Game::check_game_over()
{
    for(int j = 0; j < FIELD_WIDTH; j++)
        if(field[0][j] != 0) return true;
    return false;
}

void Game::hard_drop()
{
    for(int i = 0; i < FIELD_HEIGHT -1 ; i++) {
        move(0, 1);        
    }
    stoping_time = 0;
}

int Game::check_lines()
{
    int check_lines_num = 0;
    for(int i = 0; i < FIELD_HEIGHT; i++) {
        int block_counter = 0; 
        for(int j = 0; j < FIELD_WIDTH; j++)
            if(field[i][j] != 0) ++block_counter;
        if(block_counter == FIELD_WIDTH) {
            remove_lines(i);
            check_lines_num++;
        }
    }
    lines_cleard += check_lines_num;
    switch(check_lines_num) {
        case 1:
            points += 40;
            break;
        case 2:
            points += 100;
            break;
        case 3:
            points += 300;
            break;
        case 4:
            points += 1200;
            break;
    }
    if(check_lines_num > 0) return 1;
    else return 0;
}

void Game::remove_lines(int line)
{
    for(int i = (FIELD_HEIGHT-1); i > 0; i--)
        for(int j = 0; j < FIELD_WIDTH; j++)
            if(i <= line) field[i][j] = field[i-1][j];
}

void Game::stop_piece()
{
    
    for(int i = 0; i < 4; i++) {
        if((main[i].y + 1 >= FIELD_HEIGHT || field[static_cast<int>(main[i].y) + 1][static_cast<int>(main[i].x)] != 0)) {
            elapsed_for_stoping_finish = time_for_stoping.getElapsedTime();
            if(!stoping) {
                elapsed_for_stoping_start = time_for_stoping.getElapsedTime();
                stoping = true;
            }
            if(elapsed_for_stoping_finish.asSeconds() - elapsed_for_stoping_start.asSeconds() > stoping_time) {
                for(int i = 0; i < 4; i++)
                    field[static_cast<int>(main[i].y)][static_cast<int>(main[i].x)] = tetramino_current_num + 1;
                stoping = false;
                new_piece();
                break;
            }
        }
    }
}

void Game::rotate()
{
    if(tetramino_current_num != 3)
    {
        if(rotation_state == 3) rotation_state = 0;
        else rotation_state++;

        sf::Vector2f temp[4];
        bool successful = true;
        
        for(int i = 0; i < 4; i++) temp[i] = main[i];
        sf::Vector2f p = sf::Vector2f(main[rotation_reference[tetramino_current_num]].x, main[rotation_reference[tetramino_current_num]].y);

        for(int i = 0; i < 4; i++)
            main[i] = sf::Vector2f(p.x - (main[i].y-p.y), p.y + (main[i].x-p.x));

        for(int i = 0; i < 4; i++) {
            if(main[i].x > FIELD_WIDTH) {move(-2, 0);}
            if(main[i].x > FIELD_WIDTH - 1) {move(-1, 0);}
            if(main[i].x < -1) {move(2, 0);} 
            if(main[i].x < 0) {move(1, 0);}
            if(main[i].y > FIELD_HEIGHT - 1) {move(0, -1);}
            if(field[static_cast<int>(main[i].y)][static_cast<int>(main[i].x)] != 0) {move(0, -1);}
        }

        for(int i = 0; i < 4; i++) if(field[static_cast<int>(main[i].y)][static_cast<int>(main[i].x)] != 0) successful = false;
        if(!successful) for(int i = 0; i < 4; i++) main[i] = temp[i];
    }
}

void Game::move(float dx, float dy)
{
    for(int i = 0; i < 4; i++) {
        if(main[i].x > FIELD_WIDTH-2 && dx == 1) dx = 0;
        if(main[i].x < 1 && dx == -1) dx = 0;
        if(field[static_cast<int>(main[i].y)][static_cast<int>(main[i].x) + 1] != 0 && dx == 1) dx = 0;
        if(field[static_cast<int>(main[i].y)][static_cast<int>(main[i].x) - 1] != 0 && dx == -1) dx = 0;
        if(main[i].y > FIELD_HEIGHT-2 && dy == 1) dy = 0;
        //if(main[i].y < 1 && dy == -1) dy = 0;
        if(field[static_cast<int>(main[i].y) + 1][static_cast<int>(main[i].x)] != 0 && dy == 1) dy = 0;
        if(field[static_cast<int>(main[i].y) - 1][static_cast<int>(main[i].x)] != 0 && dy == -1) dy = 0;
    }
    for(int i = 0; i < 4; i++) {
        main[i].x += dx;
        main[i].y += dy;
    }
}

void Game::ghost_piece(sf::RenderWindow& window)
{
    sf::Vector2f ghost_main[4];
    ghost_tile.setTextureRect(sf::IntRect(BLOCK_DIMENSION * 7, 0, BLOCK_DIMENSION, BLOCK_DIMENSION));
    for(int i = 0; i < 4; i++) ghost_main[i] = main[i];

    int vely = 1;
    for(int i = 0; i < FIELD_HEIGHT - 1; i++) {
        for(int j = 0; j < 4; j++)
            if(ghost_main[j].y > 18) vely = 0;
            else if(field[static_cast<int>(ghost_main[j].y) + 1][static_cast<int>(ghost_main[j].x)] != 0) vely = 0;
        for(int j = 0; j < 4; j++) ghost_main[j].y += vely;
    }

    for(int i = 0; i < 4; i++) {
        ghost_tile.setPosition(ghost_main[i].x * BLOCK_DIMENSION * scale, ghost_main[i].y * BLOCK_DIMENSION * scale);
        //std::cout << ghost_main[i].y << std::endl;
        window.draw(ghost_tile);
    }
}

void Game::preview_piece(sf::RenderWindow& window)
{
    sf::Vector2f preview_main[4];
    preview_tile.setTextureRect(sf::IntRect(BLOCK_DIMENSION * tetramino_future_num, 0, BLOCK_DIMENSION, BLOCK_DIMENSION));

    for(int i = 0; i < 4; i++) {
        preview_main[i].x = tetramino_setup[tetramino_future_num][i][0];
        preview_main[i].y = tetramino_setup[tetramino_future_num][i][1];
    }

    if(tetramino_future_num == 2 || tetramino_future_num == 4 || tetramino_future_num == 5) 
        for(int i = 0; i < 4; i++) preview_main[i].x++;
    
    for(int i = 0; i < 4; i++) {
        preview_tile.setPosition(((preview_main[i].x * BLOCK_DIMENSION) + (8*BLOCK_DIMENSION))* scale, ((preview_main[i].y * BLOCK_DIMENSION) + (2*BLOCK_DIMENSION)) * scale);
        window.draw(preview_tile);
    }
}

void Game::draw(sf::RenderWindow& window) 
{
    ghost_piece(window);
    preview_piece(window);
    for(int i = 0; i < 4; i++) {
        tile.setPosition(main[i].x * BLOCK_DIMENSION * scale, main[i].y * BLOCK_DIMENSION * scale);
        tile.setTextureRect(sf::IntRect(BLOCK_DIMENSION * tetramino_current_num, 0, BLOCK_DIMENSION, BLOCK_DIMENSION));
        window.draw(tile);
    }

    for(int i = 0; i < FIELD_HEIGHT; i++) {
        for(int j = 0; j < FIELD_WIDTH; j++) {
            if(field[i][j] == 0) continue;
            tile.setPosition(j * BLOCK_DIMENSION * scale, i * BLOCK_DIMENSION * scale);
            tile.setTextureRect(sf::IntRect(BLOCK_DIMENSION * (field[i][j] - 1), 0, BLOCK_DIMENSION, BLOCK_DIMENSION));
            window.draw(tile);
        }
    }
}