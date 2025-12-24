#include "double_pendulum_sim.h" 

int main()
{
    sf::RenderWindow window = createWindow();
    DoublependulumSim dpSim = DoublependulumSim(window);
    dpSim.running();
}