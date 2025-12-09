#include <double_pendulum.h>
#include <cmath>

using namespace std;

rodObject::rodObject(sf::RenderWindow& windowRef, sf::Vector2f p1, sf::Vector2f p2, float width, sf::Color color): 
    window(windowRef)
{
    rodColor = color;
    rodWidth = width;
    rodPoint1 = p1;
    rodPoint2 = p2;
    rodShape.setFillColor(rodColor);
}

void rodObject::drawObject()
{
    sf::Vector2f direction = rodPoint2 - rodPoint1;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = atan2(direction.y, direction.x);
    rodShape.setSize(sf::Vector2f(length, rodWidth));
    rodShape.setOrigin(sf::Vector2f(length * 0.5, rodWidth * 0.5));
    sf::Vector2f center = (rodPoint1 + rodPoint2) / 2.0f;
    rodShape.setPosition(center);
    rodShape.setRotation(sf::radians(angle));
    
    rodObject::window.draw(rodShape);
}

void rodObject::update(sf::Vector2f p1, sf::Vector2f p2)
{
    rodPoint1 = p1;
    rodPoint2 = p2;
    drawObject();
}

bobObject::bobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color): window(windowRef)
{
    bobColor = color;
    bobRadius = radius;
    sf::Vector2f objectCenter = center - sf::Vector2f(bobRadius, bobRadius);
    bobShape.setPosition(objectCenter);
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
            sf::Vector2f bobCenter = bobShape.getPosition() + sf::Vector2f(bobRadius, bobRadius);
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

sf::Vector2f bobObject::moveObject(sf::Event& event)
{
    sf::Vector2f bobCenter = bobShape.getPosition() + sf::Vector2f(bobRadius, bobRadius);
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);
            // printf("object selected! (%.2f, %.2f)\n", mousePos.x, mousePos.y);
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
        bobShape.setPosition(bobCenter - sf::Vector2f(bobRadius, bobRadius));
        // printf("object selected! (%.2f, %.2f) (%.2f, %.2f)\n", mousePos.x, mousePos.y, bobCenter.x, bobCenter.y);
    }
    else
    {
        moving = false;
    }
    return bobCenter;
}

sf::Vector2f bobObject::update(sf::Event& event)
{
    selectObject(event);
    sf::Vector2f circleCenter = moveObject(event);
    drawObject();
    return circleCenter;
}

void bobObject::drawObject()
{
    window.draw(bobShape);
}

doublePendulum::doublePendulum(sf::RenderWindow& windowRef): window(windowRef)
{
    sf::Vector2f hingePointCenter = sf::Vector2f(0.0, 0.0);
    sf::Vector2f bob1Center = sf::Vector2f(5.0, -2.5);
    sf::Vector2f bob2Center = sf::Vector2f(-3.5, 2.0);

    hingePoint = new bobObject(window, hingePointCenter, 0.3, sf::Color::Blue);
    bob1 = new bobObject(window, bob1Center, 0.5, sf::Color::Yellow);
    bob2 = new bobObject(window, bob2Center, 0.8, sf::Color::Green);
    rod1 = new rodObject(window, hingePointCenter, bob1Center, 0.1, sf::Color::Black);
    rod2 = new rodObject(window, bob1Center, bob2Center, 0.1, sf::Color::Black);
}

void doublePendulum::update(sf::Event& event)
{
    sf::Vector2f hingePointCenter = hingePoint->update(event);
    sf::Vector2f bob1Center = bob1->update(event);
    sf::Vector2f bob2Center = bob2->update(event);
    rod1->update(hingePointCenter, bob1Center);
    rod2->update(bob1Center, bob2Center); 
}

void doublePendulum::draw()
{
    rod1->drawObject();
    rod2->drawObject();
    hingePoint->drawObject();
    bob1->drawObject();
    bob2->drawObject();
}

