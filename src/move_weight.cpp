#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class weightObject
{

    private:
        sf::RenderWindow& window;
        sf::CircleShape weightShape;
        sf::Color objectColor;
        bool moving;
        bool selected;
    
    public:
        weightObject(sf::RenderWindow& windowRef, float x, float y, float radius, sf::Color color): window(windowRef)
        {
            sf::Vector2f position = sf::Vector2f(x, y);
            objectColor = color;
            weightShape.setPosition(position);
            weightShape.setRadius(radius);
            weightShape.setFillColor(color);
            moving = false;
            selected = false;
        }

        void update()
        {
        }

        void draw()
        {
            window.draw(weightShape);
        }

        void move()
        {
        }

        void select(sf::Event& event)
        {
            if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonReleased->position);
                    float radius = weightShape.getRadius();
                    sf::Vector2f weightCenter = weightShape.getPosition() + sf::Vector2f(radius, radius);
                    sf::Vector2f distVector = mousePos - weightCenter;
                    float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
                    bool withinWeight = distanceSquare <= radius * radius;

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
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Moving a weight!");
    vector<weightObject> weights {
        weightObject(window, 40, 50, 20.0, sf::Color::Yellow),
        weightObject(window, 140, 350, 40.0, sf::Color::Green)
    };

    printf("hello\n");
    while (window.isOpen())
    {
        while (optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        
                
            window.clear(sf::Color(100,100,100));
            weights[0].select(*event);
            weights[1].select(*event);
            weights[0].update();
            weights[0].draw();
            weights[1].draw();
            window.display();
        }
    }
}