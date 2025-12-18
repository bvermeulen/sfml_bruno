#ifndef DOUBLE_PENDULUM_SIM
#define DOUBLE_PENDULUM_SIM

#include <SFML/Graphics.hpp>
#include <double_pendulum_view.h>

using namespace std;

sf::RenderWindow createWindow();

class doublependulumSim
{
    public:
        doublependulumSim(sf::RenderWindow& windowRef);
        ~doublependulumSim();
        void running();

    private:
        sf::RenderWindow& window;
        string title;
        sf::Vector2f windowSize;
        sf::Color backgroundColor;
        dpViewObject *dpv;

        void initWindow();
        void initDoublePendulum();
        void pollEvents();
        void updating(sf::Event& event);
        void rendering();
};

#endif // DOUBLE_PENDULUM_SIM
