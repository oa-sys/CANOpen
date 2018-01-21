# CANOpen
Corto mount for connecting to Master &amp; Slave devices over the CANOpen protocol.

# Implementation Resources
CANOpen implementation implements support for CANOpen (Layers 3-7). Library is
currently dependent on SocketCAN (available on most Linux distros). Other
CANOpen libraries such as
[CANopenNode](https://github.com/CANopenNode/CANopenNode)
have integrated support for native CAN data link implementations for embedded
and RTOS implementations. We look forward to assisting anyone interested in
extending the library to support additional CAN stacks.
<br>
Protocol Implementation based on the following sources:
+ CAN in Automation [CIA](https://www.can-cia.org/) Specification [301 v4.2.0](https://github.com/oa-sys/CANOpen/doc/CANOPEN_301v04020006_cor3.pdf)
+ [Wikipedia](https://en.wikipedia.org/wiki/CANopen)
