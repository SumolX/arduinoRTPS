# arduinoRTPS

This repository was forked from embeddedRTPS, a portable and open-source C++ implementation of the Real-Time Publish-Subscribe Protocol (RTPS) for embedded system.  RTPS is based on the publish-subscribe mechanism and is at the core of the Data Distribution Service (DDS). DDS is used, among many other applications, in Robot Operating System 2 (ROS2) and is also part of the AUTOSAR Adaptive platform. embeddedRTPS allows to integrate Ethernet-capable microcontrollers into DDS-based systems as first-class participants.

arduinoRTPS goal is to easily integrate with the Arduino Framework to allow for embedded devices to communicate with each other via RTPS.  Please note that arduinoRTPS rudimentary Quality-of-Service (QoS) policies and is far from a complete RTPS implementation.
