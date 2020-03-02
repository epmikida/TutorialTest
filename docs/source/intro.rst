################
What is Charm++?
################
.. name

*How much do we want to get into this vs just referring to the manual?*

Charm++ is a programming system for writing parallel applications. It is
primarily written in C++ and allows for both shared-memory and distributed
paralellism. There are also interfaces for writing applications in Fortran or
Python, but in this tutorial we will be focused on C++.

In Charm++, applications are defined in terms of parallel objects called
chares. Chares look and behave much like regular C++ objects with a few key
differences. First, the Charm++ runtime system is aware of chares and manages
their location on the physical hardware. Secondly, chares can communicate with
each other via asynchronous messages which are handled and delivered by the
runtime system. Finally, chares have a unique handle (called a proxy), which
can be used to refer to a specific chare regardless of where it is located on
physical hardware. As such, a Charm++ application will contain some number of
chares distributed across the hardware, and the behavior of the application will
largely be defined by the interactions between chares via asynchronous message
passing.

This basic knowledge of the Charm++ programming model will be sufficient to
work through exercises in this tutorial, but for more details on the programming
model and how it works, please refer to the Charm++ manual (link to manual, and
or maybe some other reference material).

###############
Getting Started
###############

*Info on checking out a copy of charm, also discuss commercial vs
research and evaluation purposes.*

****************
Building Charm++
****************

*The ./build script (or maybe CMake?)*
  - *Explain targets (just need charm++)*
  - *Explain comm layers (for this tutorial likely just need darwin, linux, or win)*

*************
Running Tests
*************

*Where are the tests, how to run, also maybe include examples dir?*

