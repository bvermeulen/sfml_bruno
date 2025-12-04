#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

using namespace std;

class weightObject
{

    private:
        sf::RenderWindow& window;
        sf::CircleShape weightShape;
        sf::Color objectColor;
        float objectRadius;
        bool moving;
        bool selected;
    
        void selectObject(sf::Event& event)
        {
            if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonReleased->position);
                    sf::Vector2f weightCenter = weightShape.getPosition() + sf::Vector2f(objectRadius, objectRadius);
                    sf::Vector2f distVector = mousePos - weightCenter;
                    float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
                    bool withinWeight = distanceSquare <= objectRadius * objectRadius;

                    if (!selected && withinWeight)
                    {
                        // printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, weightCenter.x, weightCenter.y);
                        weightShape.setFillColor(sf::Color::Red);
                        selected = true;
                    }
                    else if (selected && withinWeight)
                    {
                        // printf("object deselected! %.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, weightCenter.x, weightCenter.y);
                        weightShape.setFillColor(objectColor);
                        selected = false;
                    }
                }
            }
        }

        void moveObject(sf::Event& event)
        {

            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);
                    sf::Vector2f weightCenter = weightShape.getPosition() + sf::Vector2f(objectRadius, objectRadius);
                    sf::Vector2f distVector = mousePos - weightCenter;
                    float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
                    if (distanceSquare <= objectRadius * objectRadius)
                    {
                        moving = true;
                    }
                }
            }

            if (moving && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2 mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                sf::Vector2f position = mousePos - sf::Vector2f(objectRadius, objectRadius);
                weightShape.setPosition(position);
            }
            else
            {
                moving = false;
            }
        }

    public:
        weightObject(sf::RenderWindow& windowRef, float x, float y, float radius, sf::Color color): window(windowRef)
        {
            objectColor = color;
            objectRadius = radius;
            sf::Vector2f objectCenter = sf::Vector2f(x, y) - sf::Vector2f(objectRadius, objectRadius);
            weightShape.setPosition(objectCenter);
            weightShape.setRadius(objectRadius);
            weightShape.setFillColor(objectColor);
            moving = false;
            selected = false;
        }

        void update(sf::Event& event)
        {
            selectObject(event);
            moveObject(event);
            drawObject();
        }

        void drawObject()
        {
            window.draw(weightShape);
        }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Hello weights!!");
    vector<weightObject> weights {
        weightObject(window, 40, 50, 20.0, sf::Color::Yellow),
        weightObject(window, 140, 350, 40.0, sf::Color::Green),
        weightObject(window, 1000, 0, 200.0, sf::Color::Blue),

    };
    sf::Color background = sf::Color(100,100,100);

    printf("Hello weights!!\n");
    while (window.isOpen())
    {
        window.clear(background);
        for (unsigned int i=0; i < weights.size(); i++) {
            weights[i].drawObject();
        }
        window.display();

        while (optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            window.close();
            
            window.clear(background);
            for (unsigned int i=0; i < weights.size(); i++) {
                weights[i].update(*event);
            }
            window.display();
        }
    }
}