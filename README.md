## Overview

The goal of the command scheduler is to improve the ease with which programmers can control multiple subsystems of a robot at once.

All advanced autonomous robotics teams eventually face the challenge of scheduling a movement route that coordinates actions between multiple parts of a robot
(e.g. moves the robot and some actuator simultaneously at specified times).

With a round robin scheduling algorithm and an easy user interface, the Robot Command Scheduler improves the speed and accuracy of competition programming.

## User Interface

What does this "easy" user interface involve? It's simple. Users must tag any function with a requirement, and no two functions with the same requirements will be run at once.
To support this, functions return a bool with true if they have finished running or false if they need to be called again. This means a simple movement function might look like:

```
bool Move(int degrees, int power) {
  SetPower(power);
  degrees_moved = MovementSinceStart();
  If (degrees_moved >= degrees) {
    return true;
  }
  else {
    return false;
  }
}
```

  Some people might notice that "MovementSinceStart" is a little magical. Don't worry, you can also provide an initialization function to take care of things like storing the starting position of the robot(`SetPower(power)` could also be moved to initialize).

  The second part of the user interface is how behaviors between functions are scheduled. This behavior is encoded in "groups". 
  For example, to schedule two functions to run in parallel, you could wrap them in a parallel group
  (even the parallel group will only run functions in parallel if they do not use the same system, functions that use the same system are defaulted to run sequentially in this case). \
  Groups can be nested, so if you want to move, run two commands in parallel after the movement is done, then move again, that can be accomplished. \
  The required system of any group is all the systems used by components within the group. \
  The currently supported groups are sequential group, parallel group, loop group, and conditional group. \
  There are also convenience features such as command schedulers to help automate building a movement schedule for autonomous robotics.
  
