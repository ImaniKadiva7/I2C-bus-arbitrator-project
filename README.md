# I2C-bus-arbitrator-project

Brief Summary of this project.

In this project, the bus arbitrator is implemented as a finite state machine.

According to [https://www.geeksforgeeks.org/bus-arbitration-in-computer-organization/], 
a Bus Arbitration refers to the process by which the current bus master accesses and then 
leaves the control of the bus and passes it to another bus requesting processor unit.

According to [https://brilliant.org/wiki/finite-state-machines/] a finite state machine 
is a computation model that can be implemented with hardware or software and can be used 
to simulate sequential logic and some computer programs.

The bus arbitrator in this project uses finite state machine because it will be running
concurrently to all other subsystems within the prototype.

![image](https://user-images.githubusercontent.com/105231479/226638137-0c925d85-c507-4a29-8f6e-7914f26e3968.png)

The image above shows how the bus arbitrator is connected. It is independednt of the I2C wires.

As seen in the image above, for each micro-controller the otherMaster line is an INPUT_PULLUP and the OUT line is 
an OUTPUT.

In this project, both microcontrollers have the OUT line High initially, but if any micro-controller wants to become 
a master or gain control, then it has to change the OUT line to Low. 

The system in this prototype has 4 states as seen on the table below,

![image](https://user-images.githubusercontent.com/105231479/226643284-3c5a1631-9bd0-4782-966c-440aff4e4fec.png)

The bus arbitrator in this project has got 2 input variables which are OtherMaster and trigger and an output variable
OUT. It also has a state variable.

![image](https://user-images.githubusercontent.com/105231479/226647374-655131a4-9d57-499e-86de-d6d3c33aec27.png)

The schematic above shows how the prototype in this project is connected. 

Consider the system flow diagram below,

![image](https://user-images.githubusercontent.com/105231479/226649097-a1712c04-b485-419b-909b-736624bdc70b.png)

As seen from the system flow diagram above, 
 1. The system starts by putting the OUT line high while reading the state of the otherMaster line.
 2. Then the bus arbitrator/ finite state machine is implemented.
 3. Trigger module is finally implemented.
 
BUS ARBITRATOR/ FINITE STATE MACHINE DIAGRAM LOGIC IMPLEMENTED

![IMG_7426](https://user-images.githubusercontent.com/105231479/226668372-c870bf24-fd60-442a-a0f4-4f5287f7e562.jpg)

Looking at the finite state machine image above, there are 7 states involved.

Initially the finite state machine begins at NoMasterNoTrigger state where the OUT line is high and none of the modules 
are a master. In this case, if a trigger isn't detected, then the state remains in the same case. If there is a trigger,
the state moves to TriggerNoMaster case. If there is no trigger detected but the other module is a master then the state 
moves to NoTriggerOtherMaster case.

In the TriggerNoMaster case, the OUT line is High. In this case, if trigger is detected and the other module isn't a master,
the state moves to AttemptToBeAMaster case. If there is no trigger detected, the state moves to the NoMasterNoTrigger case.
If there is a trigger and the other is a master, then the state moves to the TriggerOtherMaster case.

In the AttemptToBeAMaster case, the OUT line is changed to LOW and a random delay is waited in this case. If there is no trigger 
detected in this case, the state moves to NoMasterNoTrigger case. If there is a trigger and the other module isn't a master, the 
state moves to TriggerIAmMaster case. If there is a trigger and other line is a master, the state moves to the trigger collision
case.

In the TriggerCollision case, the OUT line is pulled high and a random delay time is waited. If there is a trigger and the other 
module is a master then remain in the same case. If there is a trigger and the other module is a master, then state moves to Attempt 
to be a master case.

In the TriggerOtherMaster case, the OUT line is high. if there is a trigger and the other is a master then remain in the same case. 
If there is no trigger, then the state moves to NoMasterNoTrigger case. If there is a trigger and the other module isn't a master 
then the state moves to AttemptToBeAMaster case.

In the IAmMaster case, the OUT line is pulled Low. If there is a trigger and the other is not a master, then remain inthe same case.
If there is no trigger in this case, the state moves to NoMasterNoTrigger case. If there is a trigger and the other is a master, the 
state moves to trigger collision case.

In the NoTriggerOtherMaster case, the OUT line is pulled High. If there is no trigger and the other isn't a master, the state moves to 
NoMasterNoTrigger case. If there is a trigger in this case and the other is a master, the state moves to TriggerOtherMaster. If there is 
trigger and the other module is not a master then the state moves to AttemptToBeAMaster case.

TRIGGER MODULE FOR THE ATMEGA MICRO-CONTROLLER.

In the Atmega micro-controller, the system is triggered after every 100milliseconds,
 1. While the trigger is on,
      - If the Atmega is the master,
      - It initializes the MPU-6050 only once
      - resets the time.
      - gets the orientation of the MPU 6050
      - displays the orientation
 2. trigger is changed back to false.
 
 The trigger is always sampled after every 100ms in the Atmega.
 
 TRIGGER MODULE FOR THE NODE MCU 1.0
 
 After every 333ms,
 The nodeMCU activates the trigger variable to true.
 1. While the trigger is true,
      - If the nodeMCU is a master,
      - the module resets the time.
      - The nodeMCU begins transmission from the Atmega address.
      - It then requests 1 byte of the orientation data from the Atmega
2. It then checks, while the data is available,
      - It reads the available data.
      - Changes the trigger state to false.
      
The trigger variable is sampled after every 333ms in the nodeMCU.

SYSTEM IMAGE BELOW

![image](https://user-images.githubusercontent.com/105231479/226689609-c273f7b4-f495-4b53-a379-451b75a6f06e.png)
