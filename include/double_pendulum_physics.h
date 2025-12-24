#ifndef DOUBLE_PENDULUM_PHYSICS
#define DOUBLE_PENDULUM_PHYSICS
#include <boost/numeric/odeint.hpp>
#include <double_pendulum_constants.h>


typedef std::vector<double> state_type;

struct result {
    double theta1;
    double theta2;
    double time;
};

class HarmOsc
{
    public:
        HarmOsc(
            double b1Weight, double r1Length,
            double b2Weight, double r2Length,
            double df1, double df2            
        );
        ~HarmOsc();
        
        void operator() (const state_type &thetaState, state_type &thetaDotState, const double t)
        {
            double _n1, _n2, _n3, _n4;
            double dt = thetaState[0] - thetaState[1];
            double _sin_dt = sin(dt);
            double _den = (bob1Weight + bob2Weight * _sin_dt * _sin_dt);

            _n1 = bob2Weight * rod1Length * thetaState[2] * thetaState[2] * sin(2*dt);
            _n2 = 2 * bob2Weight * rod2Length * thetaState[3] * thetaState[3] * _sin_dt;
            _n3 = 2 * g * (bob2Weight * cos(thetaState[1]) * _sin_dt + bob1Weight * sin(thetaState[0]));
            _n4 = 2 * (dampingFactor1 * thetaState[2] - dampingFactor2 * thetaState[3] * cos(dt));
            thetaDotState[2] = (_n1 + _n2 + _n3 + _n4)/ (-2 * rod1Length * _den);
            
            _n1 = bob2Weight * rod2Length * thetaState[3] * thetaState[3] * sin(2*dt);
            _n2 = 2 * (bob1Weight + bob2Weight) * rod1Length * thetaState[2] * thetaState[2] * _sin_dt;
            _n3 = 2 * g * (bob1Weight + bob2Weight) * cos(thetaState[0]) * _sin_dt;
            _n4 = 2 * (dampingFactor1 * thetaState[2] * cos(dt) - dampingFactor2 * thetaState[3] * (bob1Weight + bob2Weight)/ bob2Weight);
            thetaDotState[3] = (_n1 + _n2 + _n3 + _n4)/ (2 * rod2Length *_den);
            
            thetaDotState[0] = thetaState[2]; 
            thetaDotState[1] = thetaState[3];
        }

    private:
        double bob1Weight;        
        double rod1Length;
        double bob2Weight;
        double rod2Length;    
        double dampingFactor1;
        double dampingFactor2;
};

class DoublependulumPhysics
{
    public:
        DoublependulumPhysics(
            double b1Weight, double r1Length,
            double b2Weight, double r2Length,
            double initialTheta1, double initialTheta2,
            double df1, double df2, double dt
        );
        ~DoublependulumPhysics();
        
        void setThetas(double t1, double t2);
        void updateThetasEuler();
        void updateThetasRK4();
        void updateThetasBoost();
        result getResult();
        
    private:
        double bob1Weight;
        double rod1Length;
        double bob2Weight;
        double rod2Length;
        double theta1;
        double theta2;
        double theta1Dot;
        double theta2Dot;
        double theta1DoubleDot;
        double theta2DoubleDot;
        double deltaT;
        double dampingFactor1;
        double dampingFactor2;
        double time;
        state_type thetaState;
        boost::numeric::odeint::runge_kutta4<state_type> rk4;        
        HarmOsc *ho;
        
        void calcThetasDoubleDotMethod1(double t1, double t2);
        void calcThetasDoubleDotMethod2(double t1, double t2);
};

#endif // DOUBLE_PENDULUM_PHYICS

