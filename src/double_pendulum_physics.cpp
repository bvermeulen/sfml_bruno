#include <double_pendulum_constants.h>
#include <double_pendulum_physics.h>
#include <cmath>
#include <vector>

doublependulumPhysics::doublependulumPhysics(
    float b1Weight, float r1Length,
    float b2Weight, float r2Length,
    float initialTheta1, float initialTheta2,
    float df, float dt
)
{
    bob1Weight = b1Weight;
    rod1Length = r1Length;
    bob2Weight = b2Weight;
    rod2Length = r2Length;
    theta1 = initialTheta1;
    theta2 = initialTheta2;
    dampingFactor = df;
    deltaT = dt;
    theta1DoubleDot = 0.0;
    theta2DoubleDot = 0.0;
    time = 0.0;
}

result doublependulumPhysics::getResult()
{
    result result;
    result.theta1 = theta1;
    result.theta2 = theta2;
    result.time = time;
    return result;
}

void doublependulumPhysics::calcThetasDoubleDot()
{
    double _n1, _n2, _n3, _n4, _den;
    _n1 = -g * (2 * bob1Weight + bob2Weight) * sin(theta1);
    _n2 = -bob2Weight * g * sin(theta1-2.0 * theta2);
    _n3 = -2.0*sin(theta1-theta2) * bob2Weight;
    _n4 = theta2Dot * theta2Dot * rod2Length + \
            theta1Dot * theta1Dot * rod1Length * cos(theta1-theta2);
    _den = rod1Length * (2.0 * bob1Weight + bob2Weight - \
           bob2Weight * cos(2.0*theta1-2.0*theta2));
    theta1DoubleDot = (_n1 + _n2 + _n3 * _n4) / _den;

    _n1 = 2.0 * sin(theta1-theta2);
    _n2 = (theta1Dot * theta1Dot * rod1Length * (bob1Weight + bob2Weight));
    _n3 = g * (bob1Weight + bob2Weight) * cos(theta1);
    _n4 = (theta2Dot * theta2Dot * rod2Length * \
            bob2Weight * cos(theta1-theta2));
    _den = rod2Length * (2.0 * bob1Weight + bob2Weight - \
           bob2Weight * cos(2.0*theta1-2.0*theta2));
    theta2DoubleDot = (_n1 * (_n2 + _n3 + _n4)) / _den;    
}

void doublependulumPhysics::calcThetas()
{
    calcThetasDoubleDot();
    theta1Dot += theta1DoubleDot * deltaT;
    theta2Dot += theta2DoubleDot * deltaT;
    theta1Dot = theta1Dot * (1 - dampingFactor);
    theta2Dot = theta2Dot * (1 - dampingFactor);
    theta1 += theta1Dot * deltaT;
    theta2 += theta2Dot * deltaT;
    theta1 = fmod(theta1, 2 * PI);
    theta2 = fmod(theta2, 2 * PI);    
    time += deltaT;
}

void doublependulumPhysics::setThetas(float t1, float t2)
{
    theta1 = t1;
    theta2 = t2;
    theta1Dot = 0;
    theta2Dot = 0;
    theta1DoubleDot = 0.0;
    theta2DoubleDot = 0.0;
    time = 0.0;
}