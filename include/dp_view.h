#ifndef DP_VIEW
#define DP_VIEW

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
        const bool isMoving();
        void setBobCenter(sf::Vector2f point);

    private:
        sf::RenderWindow& window;
        sf::CircleShape bobShape;
        sf::Color bobColor;
        // sf::Vector2f bobCenter;
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

class dpViewObject
{
    public:
        dpViewObject(sf::RenderWindow& windowRef, 
            float rod1Length, float rod2Length,
            float theta1, float theta2,
            float bob1Radius, float bob2Radius,
            sf::Color bob1Color, sf::Color bob2Color 
        );
        void update(sf::Event& event);
        void draw();

    private:
        sf::RenderWindow& window;
        float rod1Length, rod2Length;
        float theta1, theta2;
        bobObject* hingePoint;
        bobObject* bob1;
        bobObject* bob2;
        rodObject* rod1;
        rodObject* rod2;
        void setBobPositions();
        void setRodPositions();
};

#endif // DP_VIEW
