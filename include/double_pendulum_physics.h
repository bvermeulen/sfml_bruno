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

class HarmOscillator
{
    public:
        HarmOscillator(
            double b1Weight, double r1Length,
            double b2Weight, double r2Length,
            double df1, double df2            
        );
        ~HarmOscillator();
        void operator() (const state_type &thetaState, state_type &thetaDotState, const double t);

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
            HarmOscillator &hoRef,
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
        HarmOscillator &ho;
        
        void calcThetasDoubleDotMethod1(double t1, double t2);
        void calcThetasDoubleDotMethod2(double t1, double t2);
};

#endif // DOUBLE_PENDULUM_PHYICS

