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

void DoublependulumSim::initWindow()
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

void DoublependulumSim::initDoublePendulum()
{
    rod1Length = 5.0;
    rod2Length = 2.5;
    bob1Weight = 10.0;
    bob2Weight = 5.0;
    angledeg1 = -157.08; // +120; // -157.08
    angledeg2 = +157.35; // +157.35
    dampingFactor1 = 0.0;
    dampingFactor2 = 0.0;
    deltaT = 0.01; //0.00005;
    sf::Color bob1Color = sf::Color::Yellow;
    sf::Color bob2Color = sf::Color::Green;

    ho = new HarmOscillator(
        bob1Weight, rod1Length,
        bob2Weight, rod2Length,
        dampingFactor1, dampingFactor2
    );

    dpv = new DoublependulumViewObject(
        window, 
        rod1Length, rod2Length,
        angledeg1 * deg_rad, angledeg2 * deg_rad, 
        bob1Weight, bob2Weight,
        bob1Color, bob2Color
    );
    dpp = new DoublependulumPhysics(
        *ho,
        bob1Weight, rod1Length,
        bob2Weight, rod2Length,
        angledeg1 * deg_rad, angledeg2 * deg_rad,
        dampingFactor1, dampingFactor2, deltaT
    );

    started = false;
    paused = false;
    timeSim = 0.0;
    timeFrame = 0.0;
}

void DoublependulumSim::pollEvents() 
{
    while (optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            exitSim();

        if (event->is<sf::Event::KeyPressed>())
        {   
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                pauseSim();

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G))
                startSim();
    
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                stopSim();
            
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                exitSim();
        }

        dpv->update(*event);
        rendering();
    }
}

void DoublependulumSim::rendering()
{
    window.clear(backgroundColor);
    dpv->draw();
    window.display();
}

void DoublependulumSim::running()
{
    long int elapsedTime;
    int count = 0;
    printf("Running app: %s\n", title.c_str());
    int c = 0;
    while (window.isOpen())
    {
        if (started && !paused) 
        {
            elapsedTime = clock.getElapsedTime().asMicroseconds();
            if (elapsedTime > timeSim * million)
            {
                c++;
                dpp->updateThetasBoost();
                timeSim += deltaT;
                if (c == 10000) 
                {
                    printf("et: %li, ts: %.2f\n", elapsedTime, timeSim);
                    c = 0;
                }
            }
            
            if (elapsedTime > timeFrame * million)
            {
                count++;
                result = dpp->getResult();
                angledeg1 = result.theta1 * rad_deg;
                angledeg2 = result.theta2 * rad_deg;
                dpv->updateThetas(result.theta1, result.theta2);
                rendering();
                timeFrame += frameRate;
                if (count == 30)
                {
                    printf("time: %.4f\n", timeFrame);
                    count = 0;
                }
            }
        }

        pollEvents();
    }
}

void DoublependulumSim::pauseSim()
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

void DoublependulumSim::startSim()
{
    sf::Vector2f thetas = dpv->getThetas();
    dpp->setThetas(thetas.x, thetas.y);
    started = true;
    timeSim = 0.0;
    timeFrame = 0.0;
    clock.restart();
}

void DoublependulumSim::stopSim()
{
    started = false;
    clock.stop();
}

void DoublependulumSim::exitSim()
{
    window.close();
    delete dpv;
    delete dpp;
    delete ho;
    std::exit(0);
}

DoublependulumSim::DoublependulumSim(sf::RenderWindow& windowRef) : window(windowRef)
{
    initWindow();
    initDoublePendulum();
}
