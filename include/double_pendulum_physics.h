#ifndef DOUBLE_PENDULUM_PHYSICS
#define DOUBLE_PENDULUM_PHYSICS

struct result {
    float theta1;
    float theta2;
    float time;
};

class doublependulumPhysics
{
    public:
        doublependulumPhysics(
            float b1Weight, float r1Length,
            float b2Weight, float r2Length,
            float initalTheta1, float initialTheta2,
            float df, float dt
        );
  
        void setThetas(float t1, float t2);
        void calcThetas();
        result getResult();

    private:
        float bob1Weight;
        float rod1Length;
        float bob2Weight;
        float rod2Length;
        double theta1;
        double theta2;
        double theta1Dot;
        double theta2Dot;
        double theta1DoubleDot;
        double theta2DoubleDot;
        float deltaT;
        float dampingFactor;
        float time;

        void calcThetasDoubleDot();
};

#endif // DOUBLE_PENDULUM_PHYICS