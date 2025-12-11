#include <cmath>
#include <dp_view.h>

using namespace std;

bobObject::bobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color): window(windowRef)
{
    bobColor = color;
    bobRadius = radius;
    bobCenter = center;
    bobShape.setOrigin(sf::Vector2f(bobRadius, bobRadius));
    bobShape.setPosition(bobCenter);
    bobShape.setRadius(bobRadius);
    bobShape.setFillColor(bobColor);
    moving = false;
    selected = false;
}

void bobObject::selectObject(sf::Event& event)
{
    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Right)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonReleased->position);
            sf::Vector2f distVector = mousePos - bobCenter;
            float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
            bool withinBob = distanceSquare <= bobRadius * bobRadius;

            if (!selected && withinBob)
            {
                printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", bobCenter.x, bobCenter.y, mousePos.x, mousePos.y);
                bobShape.setFillColor(sf::Color::Red);
                selected = true;
            }
            else if (selected && withinBob)
            {
                printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", bobCenter.x, bobCenter.y, mousePos.x, mousePos.y);
                bobShape.setFillColor(bobColor);
                selected = false;
            }
        }
    }
}

void bobObject::moveObject(sf::Event& event)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);
            sf::Vector2f distVector = mousePos - bobCenter;
            float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
            if (distanceSquare <= bobRadius * bobRadius)
            {
                moving = true;
            }
        }
    }

    if (moving && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        bobCenter = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bobShape.setPosition(bobCenter);
        printf("object selected! (%.2f, %.2f)\n", bobCenter.x, bobCenter.y);
    }
    else
    {
        moving = false;
    }
}

void bobObject::update(sf::Event& event)
{
    selectObject(event);
    moveObject(event);
}

const float bobObject::getBobRadius() 
{
    return bobRadius;
}

const sf::Vector2f bobObject::getBobCenter()
{
    return bobCenter;
}

void bobObject::drawObject()
{
    window.draw(bobShape);
}

rodObject::rodObject(
    sf::RenderWindow& windowRef, 
    sf::Vector2f p1, float r1, 
    sf::Vector2f p2, float r2, 
    float width, sf::Color color): 
    window(windowRef)
{
    rodColor = color;
    rodWidth = width;
    rodPoint1 = p1;
    radius1 = r1;
    rodPoint2 = p2;
    radius2 = r2;
    rodShape.setFillColor(rodColor);
}

void rodObject::drawObject()
{   
    sf::Vector2f direction = rodPoint2 - rodPoint1;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y) - radius1 - radius2;
    float angle = atan2(direction.y, direction.x);
    rodShape.setSize(sf::Vector2f(length, rodWidth));
    rodShape.setOrigin(sf::Vector2f(length * 0.5, rodWidth * 0.5));
    sf::Vector2f p1 = rodPoint1 + sf::Vector2f(cos(angle), sin(angle)) * radius1;
    sf::Vector2f p2 = rodPoint2 + sf::Vector2f(cos(PI + angle), sin(PI + angle)) * radius2;
    sf::Vector2f center = (p1 + p2) * 0.5f;
    rodShape.setPosition(center);
    rodShape.setRotation(sf::radians(angle));
    
    rodObject::window.draw(rodShape);
}

void rodObject::update(sf::Vector2f p1, float r1, sf::Vector2f p2, float r2)
{
    rodPoint1 = p1;
    rodPoint2 = p2;
    radius1 = r1;
    radius2 = r2;
}

dpViewObject::dpViewObject(sf::RenderWindow& windowRef, float rod1Length, float rod2Length): window(windowRef)
{
    sf::Vector2f hingePointCenter = sf::Vector2f(0.0, 0.0);
    float hingePointRadius = 0.3;
    sf::Vector2f bob1Center = sf::Vector2f(0.5, -2.5);
    float bob1Radius = 0.6;
    sf::Vector2f bob2Center = sf::Vector2f(-3.5, 2.0);
    float bob2Radius = 1.0;

    hingePoint = new bobObject(window, hingePointCenter, hingePointRadius, sf::Color::Blue);
    bob1 = new bobObject(window, bob1Center, bob1Radius, sf::Color::Yellow);
    bob2 = new bobObject(window, bob2Center, bob2Radius, sf::Color::Green);
    rod1 = new rodObject(window, hingePointCenter, hingePointRadius, bob1Center, bob1Radius, 0.1, sf::Color::Black);
    rod2 = new rodObject(window, bob1Center, bob1Radius, bob2Center, bob2Radius, 0.1, sf::Color::Black);
}

void dpViewObject::update(sf::Event& event)
{
    hingePoint->update(event);
    bob1->update(event);
    bob2->update(event);
    sf::Vector2f hingePointCenter = hingePoint->getBobCenter();
    float hingePointRadius = hingePoint->getBobRadius();
    sf::Vector2f bob1Center = bob1->getBobCenter();
    float bob1Radius = bob1->getBobRadius();
    sf::Vector2f bob2Center = bob2->getBobCenter();
    float bob2Radius = bob2->getBobRadius();
    rod1->update(hingePointCenter, hingePointRadius, bob1Center, bob1Radius);
    rod2->update(bob1Center, bob1Radius, bob2Center, bob2Radius);
}

void dpViewObject::draw()
{
    rod1->drawObject();
    rod2->drawObject();
    hingePoint->drawObject();
    bob1->drawObject();
    bob2->drawObject();
}

