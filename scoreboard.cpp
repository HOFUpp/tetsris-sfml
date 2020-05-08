#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>

class ScoreBoard
{
private:
    sf::Text podium_names[3];
    sf::Text podium_points[3];
    sf::Text no_score;
    sf::Text title;
    sf::Font font;
    sf::Sprite back_button;
    sf::Texture t1, t2;
    std::vector<int> points;
    std::vector<std::string> names;    
    float scale;
public:
    void setup(float sca);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int num_lines();
    void sort_scoreboard();
    bool back_pressed = false;
};

void ScoreBoard::setup(float sca)
{
    scale = sca;

    points.clear();
    names.clear();

    float x_dimension = 180*scale;
    float y_dimension = 40*scale;

    back_pressed = false;
    sf::FloatRect textRect;
    font.loadFromFile("src/pixelmix.ttf");

    t1.loadFromFile("src/back1.png");
    t2.loadFromFile("src/back2.png");
    back_button.setTexture(t1);
    back_button.setScale(scale*0.8, scale*0.8);
    back_button.setPosition(10*scale, 12*scale);

    no_score.setFont(font);
    no_score.setString("no score board\n   was found");
    no_score.setCharacterSize(30);
    no_score.setFillColor(sf::Color::White);
    textRect = no_score.getLocalBounds();
    no_score.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    no_score.setPosition(sf::Vector2f((260/2.0f)*scale, (320/2.0f)*scale));

    title.setFont(font);
    title.setString("PODIUM");
    title.setCharacterSize(45);
    title.setFillColor(sf::Color::White);
    textRect = title.getLocalBounds();
    title.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    title.setPosition(sf::Vector2f((260/2.0f)*scale - 0*scale, 25*scale));

    std::ifstream file("score.bin", std::ios::binary);
    int num = num_lines();
    while(file) {
        std::string string;
        getline(file, string);
        num--; if(num == -1) break;
        if((num-1) % 2) points.push_back(atoi(string.c_str()));
        else names.push_back(string);
    }//for(int i = 0; i < points.size(); i++) {std::cout << names[i] << "\t" << points[i] << std::endl;}
    file.close();
    sort_scoreboard();

    for(int i = 0; i < 3; i++) {
        if((num_lines()/2) < 1 && i == 0) break;
        if((num_lines()/2) < 2 && i == 1) break;
        if((num_lines()/2) < 3 && i == 2) break;
        podium_names[i].setFont(font);
        podium_names[i].setString(std::to_string(i+1) + ". " + names[i]);
        podium_names[i].setCharacterSize(35);
        podium_names[i].setFillColor(sf::Color::White);
        //textRect = podium[i].getLocalBounds();
        //podium_names[i].setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
        podium_names[i].setPosition(sf::Vector2f((260/2.0f - 120.f)*scale,(35*scale*(i+1)) + (35*scale)));
        
        podium_points[i].setFont(font);
        podium_points[i].setString(std::to_string(points[i]));
        podium_points[i].setCharacterSize(35);
        podium_points[i].setFillColor(sf::Color::White);
        //textRect = podium_points[i].getLocalBounds();
        //podium_points[i].setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
        podium_points[i].setPosition(sf::Vector2f((260/2.0f + 20.f)*scale,35*scale*(i+1) + (35*scale)));
    }
}

void ScoreBoard::update(sf::RenderWindow& window)
{
    float x_dimension = 32*scale*0.8;
    float y_dimension = 32*scale*0.8;

    if((sf::Mouse::getPosition(window).x > 10*scale && 
    sf::Mouse::getPosition(window).x < (10*scale) + x_dimension) && 
    sf::Mouse::getPosition(window).y > 10*scale && 
    sf::Mouse::getPosition(window).y < (10*scale) + y_dimension)
    {
        back_button.setTexture(t2);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            back_pressed = true;
        } 
    }
    else back_button.setTexture(t1);
}

void ScoreBoard::sort_scoreboard()
{
    int itemp; std::string stemp;
    for(int i = 0; i < points.size(); i++) {
        for(int j = 0; j < points.size() - i - 1; j++) {
            if(points[j + 1] > points[j]) {
                itemp = points[j];
                points[j] = points[j + 1];
                points[j + 1] = itemp;
                stemp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = stemp;
            }
        }
    }
}

int ScoreBoard::num_lines()
{
    std::ifstream file("score.bin", std::ios::binary);
    int num = 0;
    while(file) {
        std::string string;
        getline(file, string);
        num++;
    }
    return num-1;
}

void ScoreBoard::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    for(int i = 0; i < 3; i++) {
        window.draw(podium_names[i]);
        window.draw(podium_points[i]);
    }
    if(num_lines() == 0) window.draw(no_score);
    else window.draw(title);
    window.draw(back_button);
    window.display();
}