#ifndef DOUBLE_PENDULUM_SIM
#define DOUBLE_PENDULUM_SIM

#include <SFML/Graphics.hpp>
#include <double_pendulum_view.h>
#include <double_pendulum_physics.h>

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
        doublependulumPhysics *dpp;

        float rod1Length;
        float rod2Length;
        float angledeg1;
        float angledeg2;
        float bob1Weight;
        float bob2Weight;
        float dampingFactor;
        float deltaT;
        sf::Clock clock;

        bool started;
        bool paused;
        result result;
        float timeSim;

        void initWindow();
        void initDoublePendulum();
        void pollEvents();
        void rendering();
        void pauseSim();
        void startSim();
        void stopSim();
};

#endif // DOUBLE_PENDULUM_SIM
