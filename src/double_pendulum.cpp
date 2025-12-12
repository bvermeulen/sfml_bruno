#include <SFML/Graphics.hpp>
#include <iostream>
#include <dp_view.h>

using namespace std;

int main()
{
    float width = 1250;
    float height = 750;
    float xRange = 30;
    float viewScale = xRange / width;
    printf("width = %.2f, height = %.2f, viewScale = %.2f\n", width, height, viewScale);
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(width, height)), "Double Pendulum!!");
    sf::Color background = sf::Color(100,100,100);
    sf::View dpView;
    dpView.setSize(viewScale * sf::Vector2f(width, -height));
    dpView.setCenter({0, 0});
    window.setView(dpView);
    float rod1Length = 3.0;
    float rod2Length = 4.0;
    float angledeg1 = +135;
    float angledeg2 = +210;
    float bob1Radius = 0.4;
    float bob2Radius = 0.5;
    sf::Color bob1Color = sf::Color::Yellow;
    sf::Color bob2Color = sf::Color::Green;  
    dpViewObject dp = dpViewObject(
        window, 
        rod1Length, rod2Length,
        angledeg1, angledeg2, 
        bob1Radius, bob2Radius,
        bob1Color, bob2Color
    );

    printf("Hello Double Pendulum!!\n");
    while (window.isOpen())
    {
        window.clear(background);
        dp.draw();
        window.display();

        while (optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            window.clear(background);
            dp.update(*event);
            dp.draw();
            window.display();
        }
    }
}