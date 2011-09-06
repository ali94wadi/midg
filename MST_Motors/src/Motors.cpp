/*******************************************************************************
 * @file: Motors.cpp
 * @author: Chris Bessent <cmbq76>
 *
 * @version 1.1
 * @brief: Motor controller for ELMOs using serial communication.  Subscribes
 *       to 'motion' topic and translates the messages into the appropiate
 *       commands for the ELMO controllers.  Includes a watchdog functionality
 *       so it will kill the motors after a certain amount of time has passed
 *       without a message being posted. Takes in a twist message
 ******************************************************************************/

/***********************************************************
* ROS specific includes
***********************************************************/
#include <ros/ros.h>
#include <ros/callback_queue.h>

/***********************************************************
* Message includes
***********************************************************/
//#include "mst_common/Velocity.h"
#include <geometry_msgs/Twist.h>


/***********************************************************
* Other includes
***********************************************************/
#include "Motors.h"
#include "motorController.h"
#include <pthread.h>
#include <string.h>

/***********************************************************
* Global variables
***********************************************************/
bool   g_watchdog_tripped;      // TRUE if message receieved in last N seconds
bool   g_motors_enabled;        // TRUE if all motors are turned on
double g_linear_velocity;
double g_angular_velocity;

static struct
{
    char    left_motor_file[20];
    char    right_motor_file[20];
    float   topspeed;
    float   watchdog_timeout;
} params;

#define GEARRATIO 7
#define CLICKS2METERS (.2459 / 2000 )
#define WHEELBASE 0.5588
#define WHEEL_DIAMETER  WHEEL_RADIUS*2
#define WHEEL_RADIUS    0.2032
#define ROBOT_RADIUS    0.2794
#define ROBOT_DIAMETER  ( ROBOT_RADIUS * 2 )
#define ENCODER_RESOLUTION  12000
#define TWO_WHEEL_DRIVE
#define NUMBER_OF_MOTORS    2
    #define MOTOR_CHANNELS      2
    #define RIGHT_MOTOR_CHANNEL 0
    #define RIGHT_MOTOR_WARP    -1
    #define LEFT_MOTOR_CHANNEL  1
    #define LEFT_MOTOR_WARP     1

/***********************************************************
* Function prototypes
***********************************************************/
static bool        setVelocity( motorController*, double, double );
static bool        initMotors( motorController* );
static bool        killMotors( motorController* );
static void        softStart( double &, double &);

/***********************************************************
* Message Callbacks
***********************************************************/
void motionCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    g_watchdog_tripped = true;
    g_linear_velocity  = msg->linear.y;
    g_angular_velocity = msg->angular.z;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Motors");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/cmd_vel", 1, motionCallback);
    
    strcpy( params.left_motor_file, (char*) "/dev/ttyUSB1" );
    strcpy( params.right_motor_file,(char*) "/dev/ttyUSB2" );
    params.topspeed = 4.4704;
    params.watchdog_timeout = 1.0;

    /***********************************************************
    * Global variable initialization
    ***********************************************************/
    g_watchdog_tripped = false;
    g_motors_enabled = false;
    g_linear_velocity = 0.0;
    g_angular_velocity = 0.0;

    /***********************************************************
    * Motor initialization
    ***********************************************************/
    motorController     m[] =
        { motorController( params.right_motor_file ),
          motorController( params.left_motor_file ) };

    while (ros::ok())
    {
        g_watchdog_tripped = false;

        /***********************************************************
        * The following call will block the process until a msg is
        * available or until WATCHDOG_TIMEOUT seconds has passed.
        ***********************************************************/
        ros::getGlobalCallbackQueue()->
            callAvailable(ros::WallDuration(params.watchdog_timeout));

        if( g_watchdog_tripped == false )
        {
            ROS_WARN("Watchdog timed out!");
            killMotors(m);
            g_motors_enabled = false;
        }
        else
      {
            if( g_motors_enabled == false )
            {
                g_motors_enabled = initMotors( m );
            }

            /* TODO PUT THINGY HERE */
            softStart( g_linear_velocity, g_angular_velocity );
            setVelocity( m, g_linear_velocity, g_angular_velocity );
        }
    }

    return 0;
}

bool setVelocity( motorController* m, double linear, double angular )
{
    ROS_INFO("\nlinear %f\nangular %f",linear,angular);
	//Motor velocity command -> RPM*200=JV --M.C. 4/11/2011
    bool    success         = true;
    double  left_velocity   = 0.0;
    double  right_velocity  = 0.0;

    if( abs(linear) > params.topspeed )
    {
        linear = 0.0;
        angular = 0.0;
        ROS_ERROR("GO SLOWER!");
    }

    const double MPS2TPS = ENCODER_RESOLUTION * GEARRATIO/(2*WHEEL_RADIUS*M_PI);
    const double TURNOFFSET = MPS2TPS * ROBOT_RADIUS;

    left_velocity = ((linear * MPS2TPS) - (TURNOFFSET * angular)) * LEFT_MOTOR_WARP;
    right_velocity = ((linear * MPS2TPS) + (TURNOFFSET * angular)) * RIGHT_MOTOR_WARP;

    ROS_INFO("\nleft-velocity: %f\nrght_velocity: %f\n",left_velocity,right_velocity);

    success &= m[LEFT_MOTOR_CHANNEL].setVelocityTick( left_velocity );
    success &= m[RIGHT_MOTOR_CHANNEL].setVelocityTick( right_velocity );
    
    success &= m[LEFT_MOTOR_CHANNEL].startMotion();
    success &= m[RIGHT_MOTOR_CHANNEL].startMotion();

    if( success == false )
    {
        ROS_ERROR("Motors failed to setVelocity!");
    }

    return success;
}

bool initMotors( motorController* m )
{
    ROS_INFO("Initializing motors");
    bool success = true;
    for( int i = 0; i < NUMBER_OF_MOTORS; i++ )
    {
        if( m[i].getState() == MOTOR_OFF )
        {
            success &= m[i].turnOn();
            // success &= m[i].setVelocityTick( 0 );
            // success &= m[i].stopMotion();
        }
    }
    if(!success)
        ROS_ERROR("Failed to initialize motors");
    return success;
}

bool killMotors( motorController* m )
{
    ROS_INFO("Killing motors");
    bool success = true;
    for( int i = 0; i < NUMBER_OF_MOTORS; i++ )
    {
        if( (m[i].getState() == MOTOR_ON)
         || (m[i].getState() == MOTOR_MOVING) )
         {
             // success &= m[i].stopMotion();
             // success &= m[i].setVelocityTick(0);
             success &= m[i].turnOff();
         }
    }
    if(!success)
        ROS_ERROR("Failed to kill motors");
    return success;
}

// Programmer: ANDREW REIDMEYER  Date: 6-4-11
// Descr: softStart() limits acceleration and decceleration 
//        of the robot to provide smoother driving.
void softStart( double & V, double & Omega )
{
  static double Vcur = 0;
  static double Ocur = 0;

  const double linearAccMax = 1/120.0;
  const double linearDecMax = linearAccMax*4.0;
  const double angularAccMax = 1/2.0;
  const double angularDecMax = 1/2.0;

  bool flipV = false;
  bool flipW = false;
  if( V < 0 ) {V = -V; Vcur = -Vcur; flipV = true;}
  if( Omega < 0) {Omega = -Omega; Ocur = -Ocur; flipW = true;}

  double t_linear = V - Vcur;
  double ErrLinear = t_linear;
  if( t_linear >= 0 ) t_linear/=linearAccMax;
  else t_linear/=-linearDecMax;

  double t_angular = Omega - Ocur;
  double ErrAngular = t_angular;
  if( t_angular >= 0) t_angular/=angularAccMax;
  else t_angular/=-angularDecMax;

  if( t_linear > t_angular )
  {
    if( Vcur >= 0 )
    {
      if(ErrLinear >= 0 )
      {
         if( ErrLinear < linearAccMax ) Vcur = V;
         else Vcur += linearAccMax;
      }
      else 
      {
        if( -ErrLinear < linearDecMax) Vcur = V;
        else Vcur -= linearDecMax;
      }
    }
    else Vcur += linearDecMax;
    if( Ocur >= 0 )
    {
      if(ErrAngular >= 0 )
      {
        if( ErrAngular < angularAccMax ) Ocur = Omega;
        else Ocur += angularAccMax*t_angular/t_linear;
      }
      else 
      {
        if( -ErrAngular < angularDecMax) Ocur = Omega;
        else Ocur -= angularDecMax*t_angular/t_linear;
      }
    }
    else Ocur += angularDecMax*t_angular/t_linear;
  }
  else
  {
    if( Vcur >= 0 )
    {
      if(ErrLinear >= 0 )
      {
        if( ErrLinear < linearAccMax ) Vcur = V;
        else Vcur += linearAccMax*t_linear/t_angular;
      }
      else 
      {
        if( -ErrLinear < linearDecMax) Vcur = V;
        else Vcur -= linearDecMax*t_linear/t_angular;
      }
    }
    else Vcur += linearDecMax*t_linear/t_angular;
    if( Ocur >= 0 )
    {
      if(ErrAngular >= 0 )
      {
        if( ErrAngular < angularAccMax ) Ocur = Omega;
        else Ocur += angularAccMax;
      }
      else 
      {
        if( -ErrAngular < angularDecMax) Ocur = Omega;
        else Ocur -= angularDecMax;
      }
    }
    else Ocur += angularDecMax;
  }

  if( t_linear == 0 ) Vcur = V;
  if( t_angular == 0) Ocur = Omega;

  if(flipV) { V = -V; Vcur = -Vcur;}
  if(flipW) { Omega = -Omega; Ocur = -Ocur;}
  
  V = Vcur;
  Omega = -Ocur;

}
