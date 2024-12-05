// #pragma once

// #include "actuator.h"
// #include "sensor.h"

// /*
//  * TActuatorInputs and TSensorStates are expected to be tuples that contain the
//  * type of input for each actuator and the type of output of each state respectively.
//  * These tuples will be looped over to set motors and collect sensor data.
//  * TRobotState is the local representation of state on the robot.
//  * A function must be provided that given the current TRobotState generates the
//  * corresponding TActuatorInputs.
//  * A function must be provided that given the previous TSensorStates, 
//  * the previous TRobotState, and the current TSensorStates, generates the current TRobotState.
//  * 
//  * Some examples of how to implement this functionality are using a scheduling library to map
//  * the current robot state into actuator inputs or using a particle filter to update the current
//  * state based on the previous robot/sensor states and the current sensor state.
//  */
// template <typename TActuatorInputs, typename TSensorStates, typename TRobotState>
// class Robot {
// private:
//   TSensorStates previous_sensor_states_;
//   TRobotState previous_robot_state_;

//   std::vector<Actuator> actuators_;
//   std::vector<Sensor> sensors_;

// public:
//   Robot(std::vector<Actuator> actuators, std::vector<Sensor> sensors)
//     : actuators_{actuators}, sensors_{sensors} { }


//   virtual TActuatorInputs GenerateActuatorInputs(TRobotState current_state) = 0;
//   virtual TRobotState GenerateCurrentState(TRobotState previous_state, TSensorStates previous_sensor_states, TSensorStates current_sensor_states) = 0;

//   TSensorStates ReadSensors() {
//     //Loop over sensors, read their values, then add then to a return tuple
//   }

//   void SetActuators(TActuatorInputs inputs) {
//     //Loop over actuators and set their input to the corresponding input
//   }

//   void RunRobot() {
//     while (true) {
//       TSensorStates current_sensor_state = ReadSensors();
//       TRobotState current_robot_state = GenerateCurrentState(previous_robot_state_, previous_sensor_states_, current_sensor_state);
//       TActuatorInputs actuator_inputs = GenerateActuatorInputs(current_robot_state);
//       SetActuators(actuator_inputs);
//       previous_sensor_states_ = current_sensor_state;
//       previous_robot_state_ = current_robot_state;
//     }
//   }
// };