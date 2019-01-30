#include "velocity_control.h"
#include "pwm_setup.h"

static int Kp = 0;
static int Ki = 0;
static int Kd = 0;
static int desired_velocity = 0;

void setKp(int value){
    Kp = value;
}
void setKi(int value){
    Ki = value;
}
void setKd(int value){
    Kd = value;
}
void setDesiredVelocity(int value){
    desired_velocity = value;
}

int  PID_controller ( int  velocity  ) {
    float Proportional_Component;  // }
    float Derivative_Component;  // }   Separate components of controller
    float Integral_Component;  // }
    unsigned int drive ;       // Output of the controller
    static int error_1 = 0;   // Previous difference between desired and actual velocities
    static float integrator_sum = 0;  // Sum of all differences between desired and actual velocities
    int error_deriv;    // change in difference between desired and actual velocities
    int error;

    error = desired_velocity - velocity;

    // Calculate Proportional component
    Proportional_Component =  (float) (error * Kp);

     // Calculate Integral component
    if(integrator_sum > 32000)
        integrator_sum = 32000;   // To prevent integral overflow
    if(integrator_sum < -32000)
        integrator_sum = -32000;   // To prevent integral underflow
    integrator_sum = (float) (integrator_sum + error);  // update the integral sum with current error
    Integral_Component = (float) (integrator_sum * Ki);

 // Calculate Derivative component
    error_deriv =  error - error_1;    // current error ? previous error
    Derivative_Component = (float) (error_deriv * Kd);
    error_1 = error;      // update previous error for next control iteration

    drive = (int) (Proportional_Component  + Integral_Component + Derivative_Component);   // sum the components

    return drive;
}