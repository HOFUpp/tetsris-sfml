#include <SFML/Graphics.hpp>

class MainMenu
{
private:
    sf::RectangleShape button_start;
    sf::RectangleShape button_points;
    sf::RectangleShape button_option;
    sf::Text text_start;
    sf::Text text_points;
    sf::Text text_option;
    sf::Text title;
    sf::Font font;
    sf::Sprite titleImage;
    sf::Texture texture;
    sf::Clock time_for_option;
    float scale;
public:
    void setup(float sca);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool start_pressed = false;
    bool points_pressed = false;
    bool option = false;
};

void MainMenu::setup(float sca)
{
    scale = sca;

    float x_dimension = 200*scale;
    float y_dimension = 40*scale;

    start_pressed = false;
    points_pressed = false;
    sf::FloatRect textRect;
    texture.loadFromFile("src/title.png");

    button_start.setSize(sf::Vector2f(x_dimension, y_dimension));
    button_start.setPosition(((260*scale)/2) - (x_dimension/2), 150*scale);

    button_points.setSize(sf::Vector2f(x_dimension, y_dimension));
    button_points.setPosition(((260*scale)/2) - (x_dimension/2), 200*scale);

    button_option.setSize(sf::Vector2f(x_dimension, y_dimension));
    button_option.setPosition(((260*scale)/2) - (x_dimension/2), 250*scale);

    font.loadFromFile("src/pixelmix.ttf");

    text_option.setFont(font);
    if(option == false) text_option.setString("CONTROL SCHEME 1");
    else text_option.setString("CONTROL SCHEME 2");
    text_option.setCharacterSize(31);
    text_option.setFillColor(sf::Color::Black);
    textRect = text_option.getLocalBounds();
    text_option.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text_option.setPosition(sf::Vector2f((260*scale)/2.0f,270*scale));

    text_start.setFont(font);
    text_start.setString("START GAME");
    text_start.setCharacterSize(38);
    text_start.setFillColor(sf::Color::Black);
    textRect = text_start.getLocalBounds();
    text_start.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text_start.setPosition(sf::Vector2f((260*scale)/2.0f,170*scale));

    text_points.setFont(font);
    text_points.setString("SCORE");
    text_points.setCharacterSize(38);
    text_points.setFillColor(sf::Color::Black);
    textRect = text_points.getLocalBounds();
    text_points.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text_points.setPosition(sf::Vector2f((260*scale)/2.0f,220*scale));

    titleImage.setTexture(texture);
    titleImage.setScale(scale*2, scale*2);
    textRect = titleImage.getLocalBounds();
    titleImage.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    titleImage.setPosition(sf::Vector2f((260*scale)/2.0f,80*scale));
}

void MainMenu::update(sf::RenderWindow& window)
{
    float x_dimension = 200*scale;
    float y_dimension = 40*scale;

    if(sf::Mouse::getPosition(window).x > ((260*scale)/2) - (x_dimension/2) && 
    sf::Mouse::getPosition(window).x < (((260*scale)/2) - (x_dimension/2) + x_dimension) &&
    sf::Mouse::getPosition(window).y > 150*scale && 
    sf::Mouse::getPosition(window).y < (150*scale + y_dimension))
    {
        button_start.setFillColor(sf::Color::Green);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            start_pressed = true;
        } 
    }
    else button_start.setFillColor(sf::Color::White);

    if(sf::Mouse::getPosition(window).x > ((260*scale)/2) - (x_dimension/2) && 
    sf::Mouse::getPosition(window).x < (((260*scale)/2) - (x_dimension/2) + x_dimension) &&
    sf::Mouse::getPosition(window).y > 200*scale && 
    sf::Mouse::getPosition(window).y < (200*scale + y_dimension))
    {
        button_points.setFillColor(sf::Color::Green);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            points_pressed = true;
        } 
    }
    else button_points.setFillColor(sf::Color::White);

    sf::Time elapsed_for_option = time_for_option.getElapsedTime();
    if(sf::Mouse::getPosition(window).x > ((260*scale)/2) - (x_dimension/2) && 
    sf::Mouse::getPosition(window).x < (((260*scale)/2) - (x_dimension/2) + x_dimension) &&
    sf::Mouse::getPosition(window).y > 250*scale && 
    sf::Mouse::getPosition(window).y < (250*scale + y_dimension))
    {
        button_option.setFillColor(sf::Color::Green);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && (option == true) && elapsed_for_option.asSeconds() > 0.4) {
            option = false;
            text_option.setString("CONTROL SCHEME 1");
            time_for_option.restart();
            goto end;
        } 
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && (option == false) && elapsed_for_option.asSeconds() > 0.4) {
            option = true;
            text_option.setString("CONTROL SCHEME 2");
            time_for_option.restart();
            goto end;
        } 
    }
    else button_option.setFillColor(sf::Color::White);
    end:;
    //(option) ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(button_points);
    window.draw(button_start);
    window.draw(button_option);
    window.draw(text_start);
    window.draw(text_points);
    window.draw(text_option);
    window.draw(titleImage);
    window.display();
}