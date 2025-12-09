#include <SFML/Graphics.hpp>
#include <iostream>
#include <double_pendulum.h>

using namespace std;

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
    doublePendulum dpObject = doublePendulum(window);

    printf("Hello weights!!\n");
    while (window.isOpen())
    {
        window.clear(background);
        dpObject.draw();
        window.display();

        while (optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            window.clear(background);
            dpObject.update(*event);
            window.display();
        }
    }
}