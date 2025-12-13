#include <SFML/Graphics.hpp>
#include <double_pendulum_sim.h>

using namespace std;

int main()
{
    doublependulumSim *dpSim = new doublependulumSim();
    dpSim->running();
}