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

        double rod1Length;
        double rod2Length;
        double angledeg1;
        double angledeg2;
        double bob1Weight;
        double bob2Weight;
        double dampingFactor1;
        double dampingFactor2;
        double deltaT;
        sf::Clock clock;

        bool started;
        bool paused;
        result result;
        double timeSim;
        double timeFrame;

        void initWindow();
        void initDoublePendulum();
        void pollEvents();
        void rendering();
        void pauseSim();
        void startSim();
        void stopSim();
};

#endif // DOUBLE_PENDULUM_SIM
