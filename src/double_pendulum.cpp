#include "double_pendulum_sim.h" 

int main()
{
    sf::RenderWindow window = createWindow();
    doublependulumSim dpSim = doublependulumSim(window);
    dpSim.running();
}