## Introduction

IX of IXWSS stands for Interface Extension, and WSS stands for WebSocket Server. In other words, IXWSS can be understood as a component that can easily extend the interface between client and server based on WebSocket/HTTP communication protocol. In a client / server architecture, a communication protocol is basically a standard method, but various data formats may be required for data to be transmitted. IXWSS provides an environment that can be developed using individual plugins so that users can directly process data in various formats.

At this point, the following requirements are required to use IXWSS.

- WebSocket Server in the casting system as a data relay agent between sender and receiver
- WebSocket Server to support JSON-PRC


## Overview
### Software Architecture
IXWSS is a component for data processing and transmission/reception between client and server based on communication protocol as described in chapter 1. The overall structure is shown in the figure below.

![Figure 1](/docs/diagram.PNG)

IXWSS is a component based on WebSocket Server. It creates each session dynamically each time a client is connected, and each session is connected to the client in a 1:1 connection. 

![Figure 2](/docs/diagram2.PNG)

All data from the client is received by each session, and the receive data is passed to plugin loaded at the session start. The advantage of IXWSS is that users can process the data through the plug-in in the form they want. In addition, each plug-in can send any type of data to the client side. In other words, the data received from each session is transferred to the individual plug-in, and the transferred data can be processed in any form in the plug-in. If a response is required to the client side, data can be created and transmitted in a desired form.

![Figure 3](/docs/diagram3.PNG)

The last module to introduce is IXPlugin. The requirement for various WebSocket-based services are increasing. In order to support theses requirement as a single software component, we have structured the plug-in that can be loaded dynamically by users. We tried to include all of the following things while considering the plug-in development environment structure.
- Easy of development
  - Easy interface for developers to easily develop
  - Provides various macros
- Independent build environment
  - Provide 100% independent build environment with IXWSS
  - Shared Object type output
- Various Examples
  - Simple Plugin Example
  - JSON-RPC Plugin Example
- Provides Development Guide
  - Provides the development guide of Plugin

