#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class bobObject
{

    private:
        sf::RenderWindow& window;
        sf::CircleShape weightShape;
        sf::Color bobColor;
        float bobRadius;
        bool moving;
        bool selected;
    
        void selectObject(sf::Event& event)
        {
            if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonReleased->position);
                    sf::Vector2f weightCenter = weightShape.getPosition() + sf::Vector2f(bobRadius, bobRadius);
                    sf::Vector2f distVector = mousePos - weightCenter;
                    float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
                    bool withinWeight = distanceSquare <= bobRadius * bobRadius;

                    if (!selected && withinWeight)
                    {
                        printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, weightCenter.x, weightCenter.y);
                        weightShape.setFillColor(sf::Color::Red);
                        selected = true;
                    }
                    else if (selected && withinWeight)
                    {
                        printf("object deselected! %.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, weightCenter.x, weightCenter.y);
                        weightShape.setFillColor(bobColor);
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
                    printf("object selected! (%.2f, %.2f)\n", mousePos.x, mousePos.y);

                    sf::Vector2f weightCenter = weightShape.getPosition() + sf::Vector2f(bobRadius, bobRadius);
                    sf::Vector2f distVector = mousePos - weightCenter;
                    float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
                    if (distanceSquare <= bobRadius * bobRadius)
                    {
                        moving = true;
                    }
                }
            }

            if (moving && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2 mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::Vector2f position = mousePos - sf::Vector2f(bobRadius, bobRadius);
                weightShape.setPosition(position);
                printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, position.x, position.y);

            }
            else
            {
                moving = false;
            }
        }

    public:
        bobObject(sf::RenderWindow& windowRef, float x, float y, float radius, sf::Color color): window(windowRef)
        {
            bobColor = color;
            bobRadius = radius;
            sf::Vector2f objectCenter = sf::Vector2f(x, y) - sf::Vector2f(bobRadius, bobRadius);
            weightShape.setPosition(objectCenter);
            weightShape.setRadius(bobRadius);
            weightShape.setFillColor(bobColor);
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
    float width = 1250;
    float height = 750;
    float xRange = 20;
    float viewScale = xRange / width;
    printf("width = %.2f, height = %.2f, viewScale = %.2f\n", width, height, viewScale);
    
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(width, height)), "Hello weights!!");
    sf::Color background = sf::Color(100,100,100);
    sf::View weightView;
    weightView.setSize(viewScale * sf::Vector2f(width, -height));
    weightView.setCenter({0, 0});
    window.setView(weightView);

    vector<bobObject> weights {
        bobObject(window, 5.0, -2.5, 0.4, sf::Color::Yellow),
        bobObject(window, -3.5, 2.0, 0.8, sf::Color::Green),
        bobObject(window, 0.0, 0.0, 0.3, sf::Color::Blue),

    };


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