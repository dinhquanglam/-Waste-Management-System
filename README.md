# -Waste-Management-System-In-Smart-Campus

The project was built in 2023 for a scientific research and graduation project of VNU University of Engineering and Technology, Vietnam.

## Contributor:
1. Dinh Quang Lam, VNU University of Engineering and Technology, Vietnam.
2. Vu Hoang Long, VNU University of Engineering and Technology, Vietnam.

## Abstract:
  Waste management is an urgent and challenging issue that most cities in Vietnam are
facing every day. However, until now, cities have only applied temporary waste collection
measures. Clearly, these solutions do not cover all practical waste treatment scenarios. This
results in a lot of waste in urban areas, directly affecting the environment and daily life of
people. This is because there is no synchronized system for collecting and distributing waste to
processing areas. To comprehensively address the issue of waste management, in this report,
my colleague, Vu Hoang Long and I have proposed a Waste Management System based on
NB-IoT with the ability to manage and monitor a network of trash cans in a smart area (e.g.,
VNU campus Hoa Lac). The system consists of four components: Devices, Connectivity,
Processing Platform, and User Interface. By leveraging NB-IoT technology in hardware design,
my Wireless Ultrasonic Level Sensor for trash bins ensures stable connectivity, wide coverage,
energy efficiency, and operational cost savings. Additionally, in the Data processing platform,
Long proposed scheduling and routing algorithms to optimize waste collection time and cost.
With data on the usage status of each trash can, we can allocate some trash cans to each area to
enhance user experience. All of the above functions are integrated into a single platform for
communication, dividing responsibilities for relevant parties in managing, using, and collecting
waste in a smart area. In this report, I will present the hardware design, NB-IoT network
communication, and hardware performance with the conformance testing according to the
regulations of the Ministry of Information and Communications - QCVN 131:2022 / BTTTT
and energy consumption measurement. Finally, I will present the user interface with a network
of tracked trash cans integrated into the map.
Keywords: Waste management system , NB-IoT, Wireless Ultrasonic Level Sensor, platform.

## System model
![system-model](./images/System-model.jpg)
![system-diaragram](./images/system-block-diagram.png)
   The working flow of the proposed waste management system is illustrated in Fig.
2.1 Firstly, the status of garbage level is collected and pre-processed by the Arduino
module. Then the Arduino module controls the SIM7020E (NB-IoT module) to perform
registration to a NB-IoT network via AT commands. After successfully connecting to
the NB-IoT network, the Arduino module sends the information of the full level of
garbage, position and state of the bin’s lid to the platform via MQTT protocol. The IoT

platform is built up with a MQTT broker, a noSQL database (mongoDb), and a back-
end server. The MQTT broker acts as a relay station for messages received from the

Arduino module, while the mongoDB database stores sensing data from the trash bins
and the control data from the server. Last but not least, a web app is developed to display
the current garbage status using data on Mongodb and the optimal path for the garbage
truck, close garbage. After the data is processed and displayed on the web along with
the map interface, it will be decentralized to the garbage collection team and the
manager. For the waste collection team, they can receive a notification to start collecting
garbage when the number of full- or nearly full-trash bins exceeds an allowed threshold.
The optimal travel routes of garbage trucks are then automatically calculated based on the full level of trash bins, and locations of the trash bins. For the manager, there are
graphs showing data from the collecting process.


## Pros?
1.The fleet of collection trucks is scheduled to start collecting proactively to save time and promptly treat and dispose of waste to meet the daily usage volume of people.

2.The collection route is routed according to distance and priority, making the route cost-optimized.

3.After a period of collecting data on the traffic of using trash bins, we will redistribute the number of trash bins to each location so that they are optimal.

4.Restore the landscape, keep the surrounding environment green, clean and beautiful, and protect people's health in a sustainable way.


## Module: 
1. Arduino UNO
2. SIM7020X HAT
3. HC-SR05 
4. J5019 HW-357 Boost Module
5. PIN 18600 
.....


