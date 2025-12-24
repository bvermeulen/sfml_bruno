#ifndef DOUBLE_PENDULUM_VIEW
#define DOUBLE_PENDULUM_VIEW

#include <double_pendulum_view.h>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

class BobObject
{
    public:
        BobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color);
        ~BobObject();
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

class RodObject
{
    public:
        RodObject(
            sf::RenderWindow& windowRef, 
            sf::Vector2f p1, float r1, 
            sf::Vector2f p2, float r2, 
            float width, sf::Color color);
        ~RodObject();
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

class TraceLine
{
    public:
        TraceLine(
            sf:: RenderWindow& windowRef,
            float traceWidth,
            sf::Color traceColor
        );
        ~TraceLine();
        void addPoint(sf::Vector2f point);
        void draw();
        void setTraceOn();
        void resetTraceOn();
        
    private:
        sf::RenderWindow& window;
        float traceWidth;
        sf::Color traceColor;
        vector<sf::Vector2f> tracePoints;
        sf::RectangleShape traceShape;
        bool traceOn;

    };

class DoublependulumViewObject
{
    public:
        DoublependulumViewObject(
            sf::RenderWindow& windowRef, 
            float rod1Length, float rod2Length,
            float theta1, float theta2,
            float bob1Radius, float bob2Radius,
            sf::Color bob1Color, sf::Color bob2Color 
        );
        ~DoublependulumViewObject();
        void update(sf::Event& event);
        void updateThetas(float t1, float t2);
        sf::Vector2f getThetas();
        void draw();

    private:
        sf::RenderWindow& window;
        float rod1Length, rod2Length;
        float theta1, theta2;
        BobObject* hingePoint;
        BobObject* bob1;
        BobObject* bob2;
        RodObject* rod1;
        RodObject* rod2;
        void setBobPositions();
        void setRodPositions();
        TraceLine* trace;
};

#endif // DOUBLE_PENDULUM_VIEW
