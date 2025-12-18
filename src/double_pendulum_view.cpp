#include <cmath>
#include <double_pendulum_view.h>

using namespace std;

sf::Vector2f calcXY(float length, float theta)
{
    float x = length * sin(theta);
    float y = - length * cos(theta);
    return sf::Vector2f(x, y);
}

float calcTheta(sf::Vector2f point)
{
    return atan2(point.x, -point.y);
}

bobObject::bobObject(sf::RenderWindow& windowRef, sf::Vector2f center, float radius, sf::Color color) : window(windowRef)
{
    bobColor = color;
    bobRadius = radius;
    bobShape.setOrigin(sf::Vector2f(bobRadius, bobRadius));
    bobShape.setPosition(center);
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
            sf::Vector2f bobCenter = bobShape.getPosition();
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
            sf::Vector2f distVector = mousePos - bobShape.getPosition();
            float distanceSquare = distVector.x * distVector.x + distVector.y * distVector.y;
            if (distanceSquare <= bobRadius * bobRadius)
            {
                moving = true;
            }
        }
    }

    if (moving && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        moving = true;
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
    return bobShape.getPosition();
}
const bool bobObject::isMoving()
{
    return moving;
}
void bobObject::setBobCenter(sf::Vector2f point)
{
    bobShape.setPosition(point);
}

void bobObject::drawObject()
{
    window.draw(bobShape);
}

rodObject::rodObject(
    sf::RenderWindow& windowRef,
    sf::Vector2f p1, float r1,
    sf::Vector2f p2, float r2,
    float width, sf::Color color) : window(windowRef)
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
/*
Draw of rod up to the edge of the bobs, so that rod is not visible inside.
*/
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

    window.draw(rodShape);
}

void rodObject::update(sf::Vector2f p1, float r1, sf::Vector2f p2, float r2)
{
    rodPoint1 = p1;
    rodPoint2 = p2;
    radius1 = r1;
    radius2 = r2;
}

traceLine::traceLine(sf::RenderWindow& windowRef, float width, sf::Color color) : window(windowRef)
{
    traceWidth = width;
    traceColor = color;
    traceOn = false;
    traceShape.setFillColor(traceColor);
}

void traceLine::setTraceOn()
{
    traceOn = true;
    tracePoints.clear();
}

void traceLine::resetTraceOn()
{
    traceOn = false;
}

void traceLine::addPoint(sf::Vector2f point)
{
    if (traceOn) {
        tracePoints.emplace_back(point);
        printf("size of tracePoint: %lu\n", tracePoints.size());
    }
}

void traceLine::draw()
{
    if ((tracePoints.size() < 2) | (!traceOn)) return;

    sf::Vector2f p1 = tracePoints[0];
    for (unsigned long long i=1; i < tracePoints.size(); i++)
    {
        sf::Vector2f p2 = tracePoints[i];
        sf::Vector2f direction = p2 - p1;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        float angle = atan2(direction.y, direction.x);
        traceShape.setSize(sf::Vector2f(length, traceWidth));
        traceShape.setOrigin(sf::Vector2f(length * 0.5, traceWidth * 0.5));
        sf::Vector2f center = (p1 + p2) * 0.5f;
        traceShape.setPosition(center);
        traceShape.setRotation(sf::radians(angle));

        window.draw(traceShape);
        p1 = p2;
    }
}

dpViewObject::dpViewObject(
    sf::RenderWindow& windowRef,
    float r1Length, float r2Length,
    float theta1Initial, float theta2Initial,
    float bob1Radius, float bob2Radius,
    sf::Color bob1Color, sf::Color bob2Color
): window(windowRef)
{
    sf::Vector2f hingePointCenter = sf::Vector2f(0.0, 0.0);
    float hingePointRadius = 0.3;
    hingePoint = new bobObject(window, hingePointCenter, hingePointRadius, sf::Color::Blue);
    bob1 = new bobObject(window, hingePointCenter, bob1Radius, bob1Color);
    bob2 = new bobObject(window, hingePointCenter, bob2Radius, bob2Color);

    rod1Length = r1Length;
    rod2Length = r2Length;
    theta1 = theta1Initial * deg_rad;
    theta2 = theta2Initial * deg_rad;
    setBobPositions();
    rod1 = new rodObject(
        window,
        hingePointCenter, hingePointRadius,
        bob1->getBobCenter(), bob1Radius,
        0.1, sf::Color::Black
    );
    rod2 = new rodObject(
        window,
        bob1->getBobCenter(), bob1Radius,
        bob2->getBobCenter(), bob2Radius,
        0.1, sf::Color::Black
    );
    trace = new traceLine(window, 0.05, sf::Color(150,150,150));
    draw();
}

void dpViewObject::update(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
        trace->setTraceOn();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        trace->resetTraceOn();

    // hingePoint->update(event);
    bob1->update(event);
    bob2->update(event);
    if (bob1->isMoving())
    {
        theta1 = calcTheta(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        setBobPositions();
        setRodPositions();

    }
    if (bob2->isMoving())
    {
        theta2 = calcTheta(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - bob1->getBobCenter());
        setBobPositions();
        setRodPositions();
    }
}

void dpViewObject::setBobPositions()
{
    sf::Vector2f point1 = calcXY(rod1Length, theta1);
    bob1->setBobCenter(point1);
    sf::Vector2f point2 = calcXY(rod2Length, theta2);
    point2 += point1;
    bob2->setBobCenter(point2);
}

void dpViewObject::setRodPositions()
{
    sf::Vector2f hingePointCenter = hingePoint->getBobCenter();
    float hingePointRadius = hingePoint->getBobRadius();
    sf::Vector2f bob1Center = bob1->getBobCenter();
    float bob1Radius = bob1->getBobRadius();
    sf::Vector2f bob2Center = bob2->getBobCenter();
    float bob2Radius = bob2->getBobRadius();
    rod1->update(hingePointCenter, hingePointRadius, bob1Center, bob1Radius);
    rod2->update(bob1Center, bob1Radius, bob2Center, bob2Radius);
    trace->addPoint(bob2Center);
}

void dpViewObject::draw()
{
    hingePoint->drawObject();
    bob1->drawObject();
    bob2->drawObject();
    rod1->drawObject();
    rod2->drawObject();
    trace->draw();
}

