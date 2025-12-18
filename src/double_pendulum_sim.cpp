#include <SFML/Graphics.hpp>
#include <double_pendulum_sim.h>

using namespace std;

sf::RenderWindow createWindow()
{
    sf::RenderWindow window;
    return window;
}

void doublependulumSim::initWindow()
{
    title = "Double Pendulum!!";
    windowSize.x = 1250;
    windowSize.y = 750;
    sf::VideoMode vm = sf::VideoMode(sf::Vector2u(windowSize.x, windowSize.y));
    window.create(vm, title);
    float xRange = 30;
    float viewScale = xRange / windowSize.x;
    backgroundColor = sf::Color(100,100,100);
    sf::View dpView;
    dpView.setSize(viewScale * sf::Vector2f(windowSize.x, -windowSize.y));
    dpView.setCenter({0, 0});
    window.setView(dpView);
    // window.setFramerateLimit(60);
    printf("width = %.2f, height = %.2f, viewScale = %.2f\n", windowSize.x, windowSize.y, viewScale);
}

void doublependulumSim::initDoublePendulum()
{
    float rod1Length = 3.0;
    float rod2Length = 4.0;
    float angledeg1 = +135;
    float angledeg2 = +210;
    float bob1Radius = 0.4;
    float bob2Radius = 0.5;
    sf::Color bob1Color = sf::Color::Yellow;
    sf::Color bob2Color = sf::Color::Green;
    dpv = new dpViewObject(
        window, 
        rod1Length, rod2Length,
        angledeg1, angledeg2, 
        bob1Radius, bob2Radius,
        bob1Color, bob2Color
    );
}

void doublependulumSim::pollEvents() 
{
    while (optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        updating(*event);
    }
}

void doublependulumSim::updating(sf::Event& event)
{
    window.clear(backgroundColor);
    dpv->update(event);
    rendering();
}

void doublependulumSim::rendering()
{
    window.clear(backgroundColor);
    dpv->draw();
    window.display();
}

void doublependulumSim::running()
{
    printf("Running app: %s\n", title.c_str());
    while (window.isOpen())
    {
        window.clear(backgroundColor);
        dpv->draw();
        window.display();
        rendering();
        pollEvents();
    }
}

doublependulumSim::doublependulumSim(sf::RenderWindow& windowRef) : window(windowRef)
{
    initWindow();
    initDoublePendulum();
}

doublependulumSim::~doublependulumSim()
{
    delete dpv;
}