#include <SFML/Graphics.hpp>
#include <iostream>
#include <bobs_view.h>

using namespace std;

int main()
{
    float width = 1250;
    float height = 750;
    float xRange = 20;
    float viewScale = xRange / width;
    printf("width = %.2f, height = %.2f, viewScale = %.2f\n", width, height, viewScale);
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(width, height)), "Elastic Bobs!!");
    sf::Color background = sf::Color(100,100,100);
    sf::View bobsView;
    bobsView.setSize(viewScale * sf::Vector2f(width, -height));
    bobsView.setCenter({0, 0});
    window.setView(bobsView);
    BobsViewObject bv = BobsViewObject(window);

    printf("Hello weights!!\n");
    while (window.isOpen())
    {
        window.clear(background);
        bv.draw();
        window.display();

        while (optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            window.clear(background);
            bv.update(*event);
            bv.draw();
            window.display();
        }
    }
}