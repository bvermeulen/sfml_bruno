#ifndef DOUBLE_PENDULUM_PHYSICS
#define DOUBLE_PENDULUM_PHYSICS

struct result {
    double theta1;
    double theta2;
    double time;
};

class doublependulumPhysics
{
    public:
        doublependulumPhysics(
            double b1Weight, double r1Length,
            double b2Weight, double r2Length,
            double initalTheta1, double initialTheta2,
            double df1, double df2, double dt
        );
  
        void setThetas(double t1, double t2);
        void updateThetasEuler();
        void updateThetasRK4();
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

        void calcThetasDoubleDotMethod1(double t1, double t2);
        void calcThetasDoubleDotMethod2(double t1, double t2);

};

#endif // DOUBLE_PENDULUM_PHYICS