# -Waste-Management-System-In-Smart-Campus

The project was built in 2023 for a scientific research and graduation project of VNU University of Engineering and Technology, Vietnam.

## Contributor:
1. Dinh Quang Lam, VNU University of Engineering and Technology, Vietnam.
2. Vu Hoang Long, VNU University of Engineering and Technology, Vietnam.

## Abstract:
Our project is an Wastebin network equipped with sensors and use NB-IoT technology to pubish real-time information to a MQTT broker and Platform to manage and support decision making for a garbage collection, beside we use the algorithm to optimalize the time that wastebin is in use, lastly we use the collected data to distribute trash traffic for each area of a campus. 

## Introduction:
Waste management is a thorny issue and a challenge for any city. The waste management systems in use today are either absent or inadequate to meet the needs of the modern city.According to statistics from the Ministry of Natural Resources and Environment [1]: “Currently, Hanoi generates more than 6,000 tons of domestic waste every day on average. In addition, there is a large amount of industrial waste from factories, enterprises and industrial zones. According to experts, with the current momentum, each year, the amount of waste in Hanoi increases by about 5%. It is estimated that by 2030, every day, Hanoi will have to process nearly 1.5 times the current amount of waste. But currently, the city is only applying temporary collection measures, lacking treatment scenarios, no clear division of responsibilities for stakeholders and no synchronous system for collecting and distributing waste to treatment zones in a methodical manner, causing a large backlog of waste that affects the urban environment and citizen's daily life.
Recent advances in the manufacture of mobile computers and smartphones, smart sensors, and sensor networks connected to next-generation mobile networks have opened vast opportunities for research and development in new technologies. various systems and applications in the field of Smart City. Although areas such as intelligent traffic monitoring have been thoroughly studied, there are still many other areas that have not been given attention commensurate with their importance. A typical example in the above fields is “Waste Management in the City”. In a smart city, the process of "collecting - transporting - treating waste" is a paramount issue that directly affects the environment and human health, it needs to be seriously considered. To better understand the concepts of “Smart City” and IoT we provide the definition that best fits our topic of these two new concepts. In the paper [2]: “It is an innovative city that uses information and communication technology (ICT) and other means to improve the quality of life, operational efficiency and urban services as well as meet current and future needs in areas such as : Smart Economy, Smart Mobility, Smart Environment, Smart Residents, Smart Life and Government smart ". Next on IoT [3]: “The Internet of Things allows anyone and everything to be connected anytime, anywhere, ideally using Any Path/Network and Any Service ". From the two definitions above, our proposal can be summarized with two keywords: "Smart environment" and "connected anytime, anywhere". We build a closed-loop system that connects smart trash bins equipped with sensors and NB IoT connection technology to push data to the Cloud using HTTPS protocol. Data is processed with algorithm statistics, scheduling, and then optimization of the final route is pushed to the web and application so that people, garbage collection teams, waste treatment parties, and local authorities can all use it to monitor and manage the clear division of responsibility.

## System model
//image

## Pros?
1.The fleet of collection trucks is scheduled to start collecting proactively to save time and promptly treat and dispose of waste to meet the daily usage volume of people.
2.The collection route is routed according to distance and priority, making the route cost-optimized.
3.After a period of collecting data on the traffic of using trash bins, we will redistribute the number of trash bins to each location so that they are optimal.
4.Restore the landscape, keep the surrounding environment green, clean and beautiful, and protect people's health in a sustainable way.


## Operation
....


## Module: 
1. Arduino UNO
2. SIM7020X HAT
3. HC-SR05 
4. Servo motor SG90
.....


