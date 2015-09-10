University of Pennsylvania

CIS 660: Advanced Topics in Computer Graphics 

Final Project 

Name: Move-ready Tree 

Authors: Haoyun Qiu, Siqi Huang 

An input plant mesh may usually have these problems: the mesh has been grouped; it consists of loops and separated sub-mesh. These problems make it not appropriate to do the deformation simulation. 

Our goal is to build a system, more accurately, a middle-ware for artist and programmer to modify and fix a plant mesh and transfer it into a simulation-ready mesh.

Here are some features of our tool:
o	First, it can decompose the input mesh into domains such as branches, twigs, and leaves.
o	We use Instancing technique which can highly decrease users’ authoring time.
o	By this tool, you can easily find separated domain components and connect them into one.
o	And you can also easily find and remove the loops of domains
o	And then it can output a loop-free, connected domain graph hierarchy.
o	For demonstration and test, it also contains an “imperfect” built-in simulator, which can output volumetric mesh by tetrahedralizing. And then you can timestep a “rough” deformation of the mesh with series of parameters tune.

Our tool is based on the paper: 
Interactive Authoring of Simulation-Ready Plants proposed on SIGGRAPH 2013.

