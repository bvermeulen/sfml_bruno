#ifndef DOUBLE_PENDULUM
#define DOUBLE_PENDULUM

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

const float PI = acos(-1.0);
const float deg_rad = 180.0 / PI;

class bobObject
{
    private:
        sf::RenderWindow& window;
        sf::CircleShape bobShape;
        sf::Color bobColor;
        float bobRadius;
        bool moving;
        bool selected;
    
        void selectObject(sf::Event& event);
        sf::Vector2f moveObject(sf::Event& event);

    public:
        bobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color);
        sf::Vector2f update(sf::Event& event);
        void drawObject();
};

class rodObject
{
    private:
        sf::RenderWindow& window;
        sf::Color rodColor;
        float rodWidth;
        sf::Vector2f rodPoint1;
        sf::Vector2f rodPoint2;
        sf::RectangleShape rodShape;
        
    public:
        rodObject(sf::RenderWindow& windowRef, sf::Vector2f p1, sf::Vector2f p2, float width, sf::Color color);
        void update(sf::Vector2f p1, sf::Vector2f p2);
        void drawObject();
};

class doublePendulum
{
    private:
        sf::RenderWindow& window;
        bobObject* hingePoint;
        bobObject* bob1;
        bobObject* bob2;
        rodObject* rod1;
        rodObject* rod2;

    public:
        doublePendulum(sf::RenderWindow& windowRef);
        void update(sf::Event& event);
        void draw();
};

#endif // DOUBLE_PENDULUM 
