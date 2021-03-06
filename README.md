# README

## Overview

This repository currently includes a single concept for a simple 'game engine plugin'. The basic premise is to allow a developer to compartmentalize logic dealing with 'game objects' so that it may be easily ingested by a wide variety of game engines, image generators (IG), or simulation hosts that provide 3D visuals. The need to support multiple 3D visualization applications over the lifetime of a product is fairly common occurance, particular in the military space. 

Given that commercial simulation and image generator applications can have a wide variety of capabilities through their SDKs, the intent of this framework to allow *simple* things to be ported between different hosts and leave the specifics to interoperation layers that can be written for the host in question. 

This concept explores the ideas of test-driven development, mocking via gMock, dependency injection, maximization of compatibility via choice of programming language (c-style interfaces) and open standard  interfaces (SISO CIGI 4.0) as software engineering practices that can reduce the burden of integration in this context. 

## Running the Concept

This project was developed on Windows 10 using Visual Studio 2019 Community Edition. Currently, X-Plane 11 trial version is used as a host application. It will need to be installed in order to run the example game engine plugin with X-Plane.

After downloading the repository as a zip file, extract the zip file, navigate to the *Docs* folder and view the *SimpleGameEnginePlugin-Overview.md* in the Markdown viewer of your choice. When the solution is built in Debug configuration, it will copy the plugin dlls to the default X-Plane 11 installation folder. From that point, when X-Plane is run it will load and execute the example game engine plugin.

The example plugin will eventually display a 'RADAR' widget in a corner of the screen that displays a blip for each other entity within a certain range of the the eyepoint. Running the plugin in XPlane will display some measure of progress towards that idea. 

## Where things are, Where they are headed

Current progress and future ideas are being tracked [here](https://github.com/stevostj/Sandbox/projects/1).
