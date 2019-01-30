// PIC 01
#include <p30Fxxxx.h> //picks the correct device automatically
#include "../velocity_control.h"
#include "../timer1_setup.h"
#include "../pwm_setup.h"
#include "../uart1_setup.h"
#include "../uart2_setup.h"
#include "adc.h"
#include "io.h"
#include "sensor.h"
#include "position_control.h"
#include "timer1.h"


static char wall_state = WALL_STATE_DOUBLE_WALL;
static char wall_state_right = TRUE;
static char wall_state_left = TRUE;

// START OF DELETING
static unsigned int const_SHOW_VELOCITY = 0;
static unsigned int const_SHOW_SENSOR = 0;
static int const_SENSOR_DIFFERENCE_CALIBRATION = 90;
static unsigned int const_NUMBER_COUNT = 10;
static unsigned int dead_zone = 100;


void setSHOW_VELOCITY(unsigned int value) {
    const_SHOW_VELOCITY = value;
}

void setSENSOR_DIFFERENCE_CALIBRATION(int value) {
    const_SENSOR_DIFFERENCE_CALIBRATION = value;
}

void setSHOW_SENSOR(unsigned int value) {
    const_SHOW_SENSOR = value;
}

void setNUMBER_COUNT(unsigned int value) {
    const_NUMBER_COUNT = value;
}

unsigned int getSHOW_SENSOR() {
    return const_SHOW_SENSOR;
}

void setDeadZone(unsigned int value) {
    dead_zone = value;
}
// END OF DELETING

void updateSideWall(void) {
    wall_state_right = IsThereWallBySensor(RIGHT_SENSOR);
    wall_state_left = IsThereWallBySensor(LEFT_SENSOR);
    LED1Latch = wall_state_right;
    LED2Latch = wall_state_left;
}

void updateWallState(void) {
    if (wall_state_left && wall_state_right) {
        wall_state = WALL_STATE_DOUBLE_WALL;
    } else if (wall_state_right) {
        wall_state = WALL_STATE_RIGHT_WALL;
    } else if (wall_state_left) {
        wall_state = WALL_STATE_LEFT_WALL;
    } else {
        wall_state = WALL_STATE_NO_WALL;
    }
}

void T1interrupt_10ms() {

    int diff_velocity;
    int velocity;
    int control_value;

    char old_wall_state = 0;
    char trigger;

    char new_wall_state_right = FALSE;
    char new_wall_state_left = FALSE;

    velocity = POSCNT - 0x7fff;
    POSCNT = 0x7fff;
    velocity = -velocity;

    FRONT_LEFT_SENSOR_Latch = 1;
    Sample(FRONT_LEFT_SENSOR);
    FRONT_LEFT_SENSOR_Latch = 0;

    FRONT_RIGHT_SENSOR_Latch = 1;
    Sample(FRONT_RIGHT_SENSOR);
    FRONT_RIGHT_SENSOR_Latch = 0;

    DIAGONAL_LEFT_SENSOR_Latch = 1;
    Sample(DIAGONAL_LEFT_SENSOR);
    DIAGONAL_LEFT_SENSOR_Latch = 0;

    DIAGONAL_RIGHT_SENSOR_Latch = 1;
    Sample(DIAGONAL_RIGHT_SENSOR);
    DIAGONAL_RIGHT_SENSOR_Latch = 0;

    LEFT_SENSOR_Latch = 1;
    Sample(LEFT_SENSOR);
    LEFT_SENSOR_Latch = 0;

    setSideSensorValue(LEFT_SENSOR, getSensorValue(LEFT_SENSOR));

    RIGHT_SENSOR_Latch = 1;
    Sample(RIGHT_SENSOR);
    RIGHT_SENSOR_Latch = 0;

    setSideSensorValue(RIGHT_SENSOR, getSensorValue(RIGHT_SENSOR));

    if (const_SHOW_SENSOR) {

        uart1_sendParameter('A', getSensorValue(FRONT_LEFT_SENSOR));
        uart1_sendParameter('B', getSensorValue(FRONT_RIGHT_SENSOR));
        uart1_sendParameter('C', getSensorValue(DIAGONAL_LEFT_SENSOR));
        uart1_sendParameter('D', getSensorValue(DIAGONAL_RIGHT_SENSOR));
        uart1_sendParameter('E', getSensorValue(LEFT_SENSOR));
        uart1_sendParameter('F', getSensorValue(RIGHT_SENSOR));

        uart1_sendEnter();
    }

    diff_velocity = 0;

    if (getFlagStop() == 0) {

        //        if (!const_SHOW_SENSOR) {
        //            FRONT_LEFT_SENSOR_Latch = 1;
        //            Sample(FRONT_LEFT_SENSOR);
        //            FRONT_LEFT_SENSOR_Latch = 0;
        //
        //            FRONT_RIGHT_SENSOR_Latch = 1;
        //            Sample(FRONT_RIGHT_SENSOR);
        //            FRONT_RIGHT_SENSOR_Latch = 0;
        //        }

        addPosition(velocity);

        if (getReferenceVelocityFoward() > 0) {

            if (wall_state_right) {
                if (getSideSensorValue(RIGHT_SENSOR) > getRELIABLE_DISTANCE_RIGHT()) {
                    new_wall_state_right = TRUE;
                }
            } else {
                if (getSideSensorValue(RIGHT_SENSOR) > getRELIABLE_DISTANCE_RIGHT_IN()) {
                    new_wall_state_right = TRUE;
                }
            }

            if (wall_state_left) {
                if (getSideSensorValue(LEFT_SENSOR) > getRELIABLE_DISTANCE_LEFT()) {
                    new_wall_state_left = TRUE;
                }
            } else {
                if (getSideSensorValue(LEFT_SENSOR) > getRELIABLE_DISTANCE_LEFT_IN()) {
                    new_wall_state_left = TRUE;
                }
            }

            if (wall_state_right != new_wall_state_right) {
                if (IsThereWallBySensor(DIAGONAL_RIGHT_SENSOR) == new_wall_state_right) {
                    if ((wall_state_right == 0) && (new_wall_state_right == 1))
                        trigger = TRIGGER_IN;
                    else
                        trigger = TRIGGER_OUT;
                    wall_state_right = new_wall_state_right;
                    LED1Latch = new_wall_state_right;
                }
            }

            if (wall_state_left != new_wall_state_left) {
                if (IsThereWallBySensor(DIAGONAL_LEFT_SENSOR) == new_wall_state_left) {
                    if ((wall_state_left == 0) && (new_wall_state_left == 1))
                        trigger = TRIGGER_IN;
                    else
                        trigger = TRIGGER_OUT;
                    wall_state_left = new_wall_state_left;
                    LED2Latch = new_wall_state_left;
                }
            }

            old_wall_state = wall_state;

            updateWallState();

            if (old_wall_state != wall_state)
                UpdatePosition(trigger);

            switch (wall_state) {
                case WALL_STATE_DOUBLE_WALL:
                    diff_velocity = getSideSensorValue(LEFT_SENSOR) - getSideSensorValue(RIGHT_SENSOR) + const_SENSOR_DIFFERENCE_CALIBRATION;
                    diff_velocity = (int) (diff_velocity * getConstCorrectionVelocity());
                    break;
                case WALL_STATE_RIGHT_WALL:
                    diff_velocity = getIDEAL_DISTANCE_RIGHT() - getSideSensorValue(RIGHT_SENSOR);
                    diff_velocity = (int) (diff_velocity * getConstCorrectionVelocity());
                    break;
                case WALL_STATE_LEFT_WALL:
                    diff_velocity = -(getIDEAL_DISTANCE_LEFT() - getSideSensorValue(LEFT_SENSOR));
                    diff_velocity = (int) (diff_velocity * getConstCorrectionVelocity());
                    break;
                case WALL_STATE_NO_WALL:
                    diff_velocity = 0;
                    break;
            }

        }
    }

    setDesiredVelocity(getReferenceVelocityFoward() + diff_velocity + getReferenceVelocityTurn());

    uart2_sendParameter('V', (getReferenceVelocityFoward() - diff_velocity - getReferenceVelocityTurn()));

    control_value = PID_controller(velocity);

    if (const_SHOW_VELOCITY) {

        uart1_sendParameter('V', velocity);

        uart1_sendParameter('C', control_value);

        // uart1_sendParameter('L', (getReferenceVelocityFoward() + diff_velocity + getReferenceVelocityTurn()));

        // uart1_sendParameter('R', (getReferenceVelocityFoward() - diff_velocity - getReferenceVelocityTurn()));

        uart1_sendEnter();
    }

    if (control_value > 0) {
        DIRALatch = 1;
        DIRBLatch = 0;
    } else {
        DIRALatch = 0;
        DIRBLatch = 1;
        control_value = -control_value;
    }

    if (control_value > 1000) PWM_DC = 1000;
    if (control_value < dead_zone) PWM_DC = 0;
    else PWM_DC = control_value;

}