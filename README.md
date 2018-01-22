# CANOpen
Corto mount for connecting to Master &amp; Slave devices over the CANOpen protocol.

# Configuration

## Object Dictionary
### Object Code
| Object Name | Comments | Supported |
|:-----------:|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:---------:|
| NULL | An object with no data fields | X |
| DOMAIN | Large variable amount of data e.g.executable program code. Application controls data and length. |  |
| DEFTYPE | Denotes a basic type definition such as a BOOLEAN, UNSIGNED16, FLOAT and so on | X |
| DEFSTRUCT | Defines a new record type e.g. the PDO mapping structure at 21h. For use with RECORD |  |
| VAR | A single value such as an UNSIGNED8, BOOLEAN, FLOAT, INTEGER16, VISIBLE STRING etc. Subindex = 0 | X |
| ARRAY | A multiple data field object where each data field is a simple variable of the SAME basic data type e.g. array of UNSIGNED16 etc. Sub-index 0 is of UNSIGNED8 and therefore not part of the ARRAY data. Subindex 1 - <array length> specifies sub-member. |  |
| RECORD | A multiple data field object where the data fields may be any combination of simple variables. Sub-index 0 is of UNSIGNED8 and sub-index 255 is of UNSIGNED32 and therefore not part of the RECORD data. Subindex 1 - <array length> specifies sub-member. |  |
### Access
The Attribute defines the access rights for a particular object. The viewpoint is from the network into the CANOpen device.
It shall be one of the following:
| Attribute |                                                                                Description                                                                                |
|:---------:|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|     rw    | read and write access                                                                                                                                                     |
|     ro    | write only access                                                                                                                                                         |
|     ro    | read only access                                                                                                                                                          |
|   const   | read-only access, value is constant The value may change in NMT state Initialisation.The value shall not change in the NMT states pre-operation, operational and stopped. |

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
+ CAN in Automation [CIA](https://www.can-cia.org/) Specification [301 v4.2.0](https://github.com/oa-sys/CANOpen/blob/master/doc/CANOPEN_301v04020006_cor3.pdf)
+ [Wikipedia](https://en.wikipedia.org/wiki/CANopen)
