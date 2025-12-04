#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class weightObject
{

    private:
        sf::RenderWindow& window;
        sf::CircleShape weightShape;
        bool moving;
    
    public:
        weightObject(sf::RenderWindow& windowRef, float x, float y, float radius, sf::Color color): window(windowRef)
            {
                sf::Vector2f position = sf::Vector2f(x, y);
                weightShape.setPosition(position);
                weightShape.setRadius(radius);
                weightShape.setFillColor(color);
                moving = false;
            }

        void update()
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                printf("Left mouse button is pressed!!");
            }
        }

        void draw()
            {
                window.draw(weightShape);
            }

        void move()
            {
            }

        void select()
            {
            }

};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Moving a weight!");
    vector<weightObject> weights {
        weightObject(window, 40, 50, 20.0, sf::Color::Yellow),
        weightObject(window, 140, 350, 40.0, sf::Color::Green)
    };

    printf("hello");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            cout << "Left mouse button is pressed!" << endl;
        }

        window.clear(sf::Color(100,100,100));
        weights[0].update();
        weights[0].draw();
        weights[1].draw();
        window.display();
    }
}