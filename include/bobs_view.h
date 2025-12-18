#ifndef BOBS_VIEW
#define BOBS_VIEW

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

const float PI = acos(-1.0);
const float deg_rad = 180.0 / PI;

class bobObject
{
    public:
        bobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color);
        void update(sf::Event& event);
        void drawObject();
        const sf::Vector2f getBobCenter();
        const float getBobRadius();

    private:
        sf::RenderWindow& window;
        sf::CircleShape bobShape;
        sf::Color bobColor;
        sf::Vector2f bobCenter;
        float bobRadius;
        bool moving;
        bool selected;
    
        void selectObject(sf::Event& event);
        void moveObject(sf::Event& event);
};

class rodObject
{
    public:
        rodObject(
            sf::RenderWindow& windowRef, 
            sf::Vector2f p1, float r1, 
            sf::Vector2f p2, float r2, 
            float width, sf::Color color);
        void update(sf::Vector2f p1, float r1, sf::Vector2f p2, float r2);
        void drawObject();

    private:
        sf::RenderWindow& window;
        sf::Color rodColor;
        float rodWidth;
        sf::Vector2f rodPoint1;
        float radius1;
        sf::Vector2f rodPoint2;
        float radius2;
        sf::RectangleShape rodShape;
};

class bobsViewObject
{
    public:
        bobsViewObject(sf::RenderWindow& windowRef);
        void update(sf::Event& event);
        void draw();

    private:
        sf::RenderWindow& window;
        bobObject* hingePoint;
        bobObject* bob1;
        bobObject* bob2;
        rodObject* rod1;
        rodObject* rod2;
};

#endif // BOBS_VIEW
