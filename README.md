This is a board I designed and put together during my time as a mentor on the FRC team 4467 Titanium Titans. The original purpose of this device was to easily test Falcon 500 Motors since using the CAN interface made it sometimes difficult to troubleshoot. This board ties between a standard FRC battery, and a Falcon 500. A FRC valid circuit breaker is needed to complete the circuit. 

Version 1 of this board failed because I designed the board with one dc-to-dc converter, but ended up having to go with anther due to shortages. The footprint was mirrored on the new chip and it released the magic smoke after recieving 14V on the output side. This version's files are likely lost to time, but was used to make V2.

Version 2 (Currently uploaded) has a slightly smaller circuit board, but is essentially completely the same design. The programming header J2 was changed to the size it was supposed to be (V1 was too small), and U1 was flipped to match the parts I had on-hand. 

In the image below left to right: Version 2 PCB only. Version 1 PCB only. Version 2 with components except a ciruit breaker.
![Left to right. Version 2 PCB only. Version 1 PCB only. Version 2 with components except a ciruit breaker.](https://github.com/user-attachments/assets/d094b7ca-3a2c-45f7-a801-8ec9d9d05715)

Unfortunately, I don't believe I ever recorded this working (Although it definitely has). I would have issues handing this to a student to use since everything is so exposed. Software and hardware wise, I think it is fine. (There should maybe be a protection circuit for the 5V side since U1 blew up in version 1 due to the previously mentioned design mistake.)

## Potential improvments. 
* Label the programming pins. (Needs to be reverse engineered again!)
* Use the virtical anderson connector for the battery connection. This allows for an off-the-shelf plastic sleeve to protect the exposed contacts. 
* Design a case. (This could be 3d printed, but with the forces needed to connect the battery, a 3d printed case will need to be designed to withstand large gripping forces.)
* Better protection on the 5V side. 
