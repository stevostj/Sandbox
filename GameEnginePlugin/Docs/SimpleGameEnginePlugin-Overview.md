# Simple Game Engine Plugin

The basic premise behind the Simple Game Engine Plugin to allow a developer to compartmentalize logic dealing with 'game objects' so that it may be easily ingested by a wide variety of game engines, image generators, or simulation hosts that provide 3D visuals. Given that these hosts have a wide variety of capabilities through their SDKs that are all different, the intent of this framework to allow *simple* things to be ported between different hosts and leave the specifics to any sort of interoperation layer that can be written for the host in question. 

![](./GameEnginePluginConcept.png)

A host application ingests instances of a game engine plugin and interacts with it via the defined IGameEnginePlugin interface functions. 

![](./GameEnginePluginInterface.png)

![](./GameEnginePluginLifecycle.png)

![](./Game%20Engine%20Plugin%20-%20X-Plane%20Integration.png)

![](./XPlaneWrapper.png)