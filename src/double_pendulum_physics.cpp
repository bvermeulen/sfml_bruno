#include <double_pendulum_constants.h>
#include <double_pendulum_physics.h>
#include <cmath>
#include <vector>

doublependulumPhysics::doublependulumPhysics(
    double b1Weight, double r1Length,
    double b2Weight, double r2Length,
    double initialTheta1, double initialTheta2,
    double df1, double df2, double dt
)
{
    bob1Weight = b1Weight;
    rod1Length = r1Length;
    bob2Weight = b2Weight;
    rod2Length = r2Length;
    theta1 = initialTheta1;
    theta2 = initialTheta2;
    dampingFactor1 = df1;
    dampingFactor2 = df2;
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


void doublependulumPhysics::calcThetasDoubleDotMethod1(double t1, double t2)
{
/*    
    see: https://www.engineered-mind.com/engineering/double-pendulum-matlab-code/#conclusion
    https://www.myphysicslab.com/pendulum/double-pendulum
*/  
    
    double _n1, _n2, _n3, _n4, _den;
    _n1 = -g * (2 * bob1Weight + bob2Weight) * sin(t1);
    _n2 = -bob2Weight * g * sin(t1-2.0 * t2);
    _n3 = -2.0*sin(t1-t2) * bob2Weight;
    _n4 = theta2Dot * theta2Dot * rod2Length + \
            theta1Dot * theta1Dot * rod1Length * cos(t1-t2);
    _den = rod1Length * (2.0 * bob1Weight + bob2Weight - \
           bob2Weight * cos(2.0*t1-2.0*t2));
    theta1DoubleDot = (_n1 + _n2 + _n3 * _n4) / _den;

    _n1 = 2.0 * sin(t1-t2);
    _n2 = (theta1Dot * theta1Dot * rod1Length * (bob1Weight + bob2Weight));
    _n3 = g * (bob1Weight + bob2Weight) * cos(t1);
    _n4 = (theta2Dot * theta2Dot * rod2Length * \
            bob2Weight * cos(t1-t2));
    _den = rod2Length * (2.0 * bob1Weight + bob2Weight - \
           bob2Weight * cos(2.0*t1-2.0*t2));
    theta2DoubleDot = (_n1 * (_n2 + _n3 + _n4)) / _den;    
}


void doublependulumPhysics::calcThetasDoubleDotMethod2(double t1, double t2)
{
        double _n1, _n2, _n3, _n4;
        double dt = t1 - t2;
        double _sin_dt = sin(dt);
        double _den = (bob1Weight + bob2Weight * _sin_dt * _sin_dt);

        _n1 = bob2Weight * rod1Length * theta1Dot * theta1Dot * sin(2*dt);
        _n2 = 2 * bob2Weight * rod2Length * theta2Dot * theta2Dot * _sin_dt;
        _n3 = 2 * g * (bob2Weight * cos(t2) * _sin_dt + bob1Weight * sin(t1));
        _n4 = 2 * (dampingFactor1 * theta1Dot - dampingFactor2 * theta2Dot * cos(dt));
        theta1DoubleDot = (_n1 + _n2 + _n3 + _n4)/ (-2 * rod1Length * _den);

        _n1 = bob2Weight * rod2Length * theta2Dot * theta2Dot * sin(2*dt);
        _n2 = 2 * (bob1Weight + bob2Weight) * rod1Length * theta1Dot * theta1Dot * _sin_dt;
        _n3 = 2 * g * (bob1Weight + bob2Weight) * cos(t1) * _sin_dt;
        _n4 = 2 * (dampingFactor1 * theta1Dot * cos(dt) - dampingFactor2 * theta2Dot * (bob1Weight + bob2Weight)/ bob2Weight);
        theta2DoubleDot = (_n1 + _n2 + _n3 + _n4)/ (2 * rod2Length *_den);
}

void doublependulumPhysics::updateThetasEuler()
{
    calcThetasDoubleDotMethod2(theta1, theta2);
    theta1Dot += theta1DoubleDot * deltaT;
    theta2Dot += theta2DoubleDot * deltaT;
    // theta1Dot = theta1Dot * (1 - dampingFactor1);
    // theta2Dot = theta2Dot * (1 - dampingFactor2);
    theta1 += theta1Dot * deltaT;
    theta2 += theta2Dot * deltaT;
    theta1 = fmod(theta1, 2 * PI);
    theta2 = fmod(theta2, 2 * PI);    
    time += deltaT;
}

void doublependulumPhysics::updateThetasRK4()
{
    calcThetasDoubleDotMethod2(theta1, theta2);
    double V10 = deltaT * theta1Dot;
    double A10 = deltaT * theta1DoubleDot;
    double V20 = deltaT * theta2Dot;
    double A20 = deltaT * theta2DoubleDot;

    calcThetasDoubleDotMethod2(theta1 + 0.5*V10, theta2 + 0.5*V20);
    double V11 = deltaT * (theta1Dot + 0.5*A10);
    double A11 = deltaT * theta1DoubleDot;
    double V21 = deltaT * (theta2Dot + 0.5*A20);
    double A21 = deltaT * theta2DoubleDot;

    calcThetasDoubleDotMethod2(theta1 + 0.5*V11, theta2 + 0.5*V21);
    double V12 = deltaT * (theta1Dot + 0.5*A11);
    double A12 = deltaT * theta1DoubleDot;
    double V22 = deltaT * (theta2Dot + 0.5*A21);
    double A22 = deltaT * theta2DoubleDot;

    calcThetasDoubleDotMethod2(theta1 + 0.5*V12, theta2 + 0.5*V22);
    double V13 = deltaT * (theta1Dot + 0.5*A12);
    double A13 = deltaT * theta1DoubleDot;
    double V23 = deltaT * (theta2Dot + 0.5*A22);
    double A23 = deltaT * theta2DoubleDot;

    const double den = 1.0 / 6.0;
    theta1 += den * (V10+2.0*V11+2.0*V12+V13);
    theta1Dot += den * (A10+2.0*A11+2.0*A12+A13);
    theta2 += den * (V20+2.0*V21+2.0*V22+V23);
    theta2Dot += den * (A20+2.0*A21+2.0*A22+A23);

}

void doublependulumPhysics::setThetas(double t1, double t2)
{
    theta1 = t1;
    theta2 = t2;
    theta1Dot = 0;
    theta2Dot = 0;
    theta1DoubleDot = 0.0;
    theta2DoubleDot = 0.0;
    time = 0.0;
}