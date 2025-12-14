#include <SFML/Graphics.hpp>
#include <double_pendulum_sim.h>

using namespace std;

int main()
{
    sf::RenderWindow window = createWindow();
    doublependulumSim *dpSim = new doublependulumSim(window);
    dpSim->running();
}