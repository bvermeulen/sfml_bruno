#include <double_pendulum_constants.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <double_pendulum_sim.h>
#include <double_pendulum_physics.h>

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
    printf("width = %.2f, height = %.2f, viewScale = %.2f\n", windowSize.x, windowSize.y, viewScale);
}

void doublependulumSim::initDoublePendulum()
{
    rod1Length = 5.0;
    rod2Length = 2.5;
    angledeg1 = -157.08; // +120; // -157.08
    angledeg2 = +157.35; // +157.35
    bob1Weight = 5.0;
    bob2Weight = 2.5;
    dampingFactor = 0.0;
    deltaT = 0.001;
    sf::Color bob1Color = sf::Color::Yellow;
    sf::Color bob2Color = sf::Color::Green;

    dpv = new dpViewObject(
        window, 
        rod1Length, rod2Length,
        angledeg1 * deg_rad, angledeg2 * deg_rad, 
        bob1Weight, bob2Weight,
        bob1Color, bob2Color
    );
    dpp = new doublependulumPhysics(
        bob1Weight, rod1Length,
        bob2Weight, rod2Length,
        angledeg1 * deg_rad, angledeg2 * deg_rad,
        dampingFactor, deltaT
    );

    started = false;
    paused = false;
}

void doublependulumSim::pollEvents() 
{
    while (optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (event->is<sf::Event::KeyPressed>())
        {   
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                pauseSim();

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G))
                startSim();
    
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                stopSim();
            
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        dpv->update(*event);
        rendering();
    }
}

void doublependulumSim::rendering()
{
    window.clear(backgroundColor);
    dpv->draw();
    window.display();
}

void doublependulumSim::running()
{
    sf::Time elapsedTime;
    printf("Running app: %s\n", title.c_str());
    while (window.isOpen())
    {
        if (started && !paused) 
        {
            elapsedTime = clock.getElapsedTime();
            if (elapsedTime.asMilliseconds() > timeSim * 1000)
            {
                dpp->calcThetas();
                result = dpp->getResult();
                angledeg1 = result.theta1 * rad_deg;
                angledeg2 = result.theta2 * rad_deg;
                timeSim = result.time;
                dpv->updateThetas(result.theta1, result.theta2);
                rendering();
            }
        }

        pollEvents();
    }
}

void doublependulumSim::pauseSim()
{
    if (paused) 
    {
        paused = false;
        clock.start();
    }
    else
    {
        paused = true;
        clock.stop();
    }
}

void doublependulumSim::startSim()
{
    sf::Vector2f thetas = dpv->getThetas();
    dpp->setThetas(thetas.x, thetas.y);
    started = true;
    timeSim = 0.0;
    clock.restart();
}

void doublependulumSim::stopSim()
{
    started = false;
    clock.stop();
}

doublependulumSim::doublependulumSim(sf::RenderWindow& windowRef) : window(windowRef)
{
    initWindow();
    initDoublePendulum();
}

doublependulumSim::~doublependulumSim()
{
    delete dpv;
    delete dpp;
}