#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>

class InsertScore
{
private:
    sf::Font font;
    sf::Text text_in_textbox;
    sf::Text suggestion;
    sf::Text message;
    sf::RectangleShape text_box;
    std::string username = "";
    float scale;
public:
    void setup(float sca);
    void update(sf::RenderWindow& window, int score);
    void draw(sf::RenderWindow& window);
    void saveOnFile(int score);
    void handleText(sf::Event e, std::string& name);
    bool isEnterPressed();
    bool enterPressed = false;
};

void InsertScore::setup(float sca)
{
    scale = sca;
    enterPressed = false;
    username = "";

    float x_dimension = (scale * 180);
    float y_dimension = (scale * 40);
    sf::FloatRect textRect;

    text_box.setSize(sf::Vector2f(x_dimension, y_dimension));
    text_box.setPosition(((260*scale)/2) - (x_dimension/2), 170*scale);

    font.loadFromFile("src/pixelmix.ttf");

    message.setFont(font); 
    message.setString("GAME OVER");
    message.setCharacterSize(70);
    message.setFillColor(sf::Color::White);
    textRect = message.getLocalBounds();
    message.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    message.setPosition(sf::Vector2f((260*scale)/2.0f, 50*scale));

    text_in_textbox.setFont(font); 
    text_in_textbox.setString("");
    text_in_textbox.setCharacterSize(45);
    text_in_textbox.setFillColor(sf::Color::Black);
    textRect = text_in_textbox.getLocalBounds();
    text_in_textbox.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text_in_textbox.setPosition(sf::Vector2f((260*scale)/2.0f, 180*scale));

    suggestion.setFont(font); 
    suggestion.setString("ENTER YOUR NAME");
    suggestion.setCharacterSize(35);
    suggestion.setFillColor(sf::Color::White);
    textRect = suggestion.getLocalBounds();
    suggestion.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    suggestion.setPosition(sf::Vector2f((260*scale)/2.0f,140*scale));
}

void InsertScore::update(sf::RenderWindow& window, int score)
{
    sf::Event event;
    sf::FloatRect textRect;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::TextEntered:
                if(event.text.unicode == 13 && username != "") {
                    saveOnFile(score);
                    enterPressed = true;
                }
                if(event.text.unicode < 128)
                    handleText(event, username);
                break;
        }
    }

    textRect = text_in_textbox.getLocalBounds();
    text_in_textbox.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text_in_textbox.setPosition(sf::Vector2f((260*scale)/2.0f,190*scale));

    text_in_textbox.setString(username);
}

void InsertScore::saveOnFile(int score)
{
    std::ofstream file("score.bin", std::ios::out | std::ios::app | std::ios::binary);
    file << username << std::endl;
    file << score << std::endl;
    file.close();
}

void InsertScore::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(message);
    window.draw(text_box);
    window.draw(text_in_textbox);
    window.draw(suggestion);
    window.display();
}

void InsertScore::handleText(sf::Event e, std::string& name)
{
    if(e.text.unicode < 128 && e.text.unicode != 27 && e.text.unicode != 13 && e.text.unicode != 32) {
        if(e.text.unicode == 8 && name.length() > 0) {
            std::string name2;
            for(int i = 0; i < name.length() - 1; i++)
                name2 += name[i];
            name = name2;
        }else if(!(e.text.unicode == 8) && name.length() < 8) name += (char)e.text.unicode;
    }
}