# -Waste-Management-System-In-Smart-Campus

The project was built in 2023 for a scientific research and graduation project of VNU University of Engineering and Technology, Vietnam.

## Contributor:
1. Dinh Quang Lam, VNU University of Engineering and Technology, Vietnam.
2. Vu Hoang Long, VNU University of Engineering and Technology, Vietnam.

## Abstract:
  Waste management is a painful and challenging issue for any city in Vietnam. But now cities are only applying small collection measures temporarily lacking handling scenarios, there is no clear division of responsibility to the stakeholders and there is no synchronous system in terms of Collecting and dividing the flow of waste to the treatment areas in a methodical manner, causing many waste to affect the urban environment and daily life of the people. To deploy a complete waste management solution, we need to build an IoT system with full components such as: Devices, connections, Data processing and user interface. In this paper, we propose a complete IoT system that provides a solution to manage and monitor the network of trash bins in a smart campus using NB-IoT technology with wide coverage and stable connection as well as energy saving. We apply more algorithms in data processing to support scheduling and routing to optimize waste collection time and costs. Besides, with the obtained real-time data, we will allocate trash traffic to each area to optimize the user experience. All of that is integrated into a single platform to communicate, divide responsibilities for stakeholders involved in the management, use and collection of waste in a smart campus.

## Introduction:
  In this part, we shrink the researching object, for example, the campus of VNU in Hoa Lac. This campus is as large as a province with 11,372km2, so the number of deployment bins will also be very large. According to “World Bank Open Data” '”with a population of 98.173, Vietnamese people emit 65,000 tons of waste a day.  So about 9000 students according to the data taken on VNU website, the amount of waste to this campus is about 5.95 tons per day. “A good rule of thumb is having one can for every 100 square yards (one can for every football field-sized area)” - Trash cans unlimited, LLC. So 11,372km2 /7140km2 = 1592 waste bins. So how can VNU management know which garbage bins need to be collected so that the overload time of the pull bin does not last for too long. The time and the collection route will be optimal for the cost for the garbage collection party. And finally, how to know which area often overloads when the number of trash can not meet the needs or areas of the garbage bin capacity is not fully utilized, to redistribute the flow of trash bins. For each area to optimize student experience.
Recent advances in the production of mobile computers and smartphones, smart sensors and sensors connected to new generation mobile networks have opened a vast opportunity for research and development Different systems and applications in the field of smart city. Although areas such as smart traffic supervision have been thoroughly researched, there are still many other areas that have not been paid attention to commensurate with its importance. A typical of the above areas is "urban waste management". To understand the concept of "smart city" and IoT we provide the definition that best suits our theme of these two new concepts. The first is "Smart City" [2]: "This is a good city in the future based on the following basic components  smart economy, smart mobile, smart environment , Intelligent, intelligent and intelligent people), built based on the 'smart' combination of self-decided, independent and cognitive citizens. Next to IoT [3]: “The technology of connecting all things allows people and everything is connected to each other anytime, anywhere, ideally using any path / network and any service. "From our two definitions, we can summarize with the two keywords" Smart environment "and" connect anytime, anywhere ". We build a "waste management system in a smart campus" to help connect between trash bins equipped with sensors and NB-IOT connection technology to push data that includes the information of position and capacity to a database by The MQTT protocol, the data is then connected and processed in the server with statistical algorithms, scheduling, optimizing the last path is pushing to the web and and mobile app interface for people, the collection team, and local authorities can use to monitor, manage and divide the responsibilities clearly.
  The rest of the article is structured as follows. Part II presents related tasks in the field of waste collection to support IoT in smart cities. Part III Description of the system includes Hardware and Software. Part IV The improvements in our testing. Part V The future development direction micro reference.


## System model
![system-model](./images/System-model.jpg)
![system-diaragram](./images/system-block-diagram.png)
  System Model includes : Device, Connection Technologies, IoT Platform, Web. Firstly, Device: Smart garbage can model includes: NB-IoT module used to connect NB-IoT network, an ultrasonic sensor module to detect the entire garbage, and module servo to close/open the garbage. The Arduino Uno is responsible for collecting data, acting as a GATEWAY, connecting to the platform, and sending data across the platform. Secondly, The Connection includes MQTT connection protocol and NB-IOT Connecting Technology. The next is The IoT Platform includes an MQTT broker and Database Mongodb MQTT broker acts as a relay station for messages received from GATEWAY and MongoDB: stores data transferred from MQTT. Last but not least, a Self-built Web user interface to display the current garbage status using data on Mongodb and the optimal path for the garbage truck, close garbage. After the data is processed and displayed on the web along with the map interface, it will be decentralized to the citizen, the garbage collection team and the government. For the citizens, they will see the traffic of the nearest trash cans to choose the suitable one. For the garbage collection team, they will receive a notification to start collecting trash when the number of full or nearly full trash bins exceeds the allowed threshold, then the fleet's travel route will be automatically optimized according to the trash bin’s fullness, distance from the collection vehicle, route and location of the trash bins.


## Pros?
1.The fleet of collection trucks is scheduled to start collecting proactively to save time and promptly treat and dispose of waste to meet the daily usage volume of people.

2.The collection route is routed according to distance and priority, making the route cost-optimized.

3.After a period of collecting data on the traffic of using trash bins, we will redistribute the number of trash bins to each location so that they are optimal.

4.Restore the landscape, keep the surrounding environment green, clean and beautiful, and protect people's health in a sustainable way.


## Module: 
1. Arduino UNO
2. SIM7020X HAT
3. HC-SR05 
4. Servo motor SG90
.....


