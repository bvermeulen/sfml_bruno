#ifndef DOUBLE_PENDULUM_SIM
#define DOUBLE_PENDULUM_SIM

#include <SFML/Graphics.hpp>
#include <double_pendulum_view.h>

using namespace std;

class doublependulumSim
{
    public:
        doublependulumSim();
        ~doublependulumSim();
        void running();

    private:
        sf::RenderWindow* window;
        sf::Vector2f windowSize;
        string title;
        sf::VideoMode vm;
        sf::Color backgroundColor;
        dpViewObject *dpv;

        void initWindow();
        void initDoublePendulum();
        void pollEvents();
        void updating(sf::Event& event);
        void rendering();
};

#endif // DOUBLE_PENDULUM_SIM
