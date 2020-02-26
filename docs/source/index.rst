.. Charm++ Tutorial documentation master file, created by
   sphinx-quickstart on Tue Feb 25 15:59:25 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Charm++ Tutorial's documentation!
============================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

=======================================
The Charm++ Parallel Programming System
=======================================

What is Charm++?
================
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

Getting Started
===============

*Info on checking out a copy of charm, also discuss commercial vs
research and evaluation purposes.*

Building Charm++
----------------

*The ./build script (or maybe CMake?)*
  - *Explain targets (just need charm++)*
  - *Explain comm layers (for this tutorial likely just need darwin, linux, or win)*

Running Tests
-------------

*Where are the tests, how to run, also maybe include examples dir?*

Your First Charm++ Program
==========================
In simple "Hello World" example, we will learn how Charm++ programs start, and
how to define our very first chare.  Code for this part of the tutorial is
found in ``charm/examples/tutorial/1_mainchare``.

Main chare: the entry point of the application
----------------------------------------------

As described in `What is Charm++?`_, a Charm++ application is composed of
chares. Therefore, in order to write a Charm++ application we must learn how
to define and create chares. The first type of chare we will learn about is
called a **mainchare**. The **mainchare** for a Charm++ application is
automatically created by the runtime system at application startup. Just like
any other C++ object, this invokes the constructor of the **mainchare**, which
is the main entry point of the application. As such, the **mainchare**
constructor serves the same purpose as ``int main()`` in a regular C++
application.

Defining a new chare type in Charm++ works much like defining a new class in a
C++ application, but with one extra step. In order inform the runtime system
which of your C++ objects are actually chares, the first step is to write a
Charm++ Interface file (.ci file).

CI Files
--------

A .ci file is an interface file which is compiled with the rest of the program
in order to promote certain C++ objects to chares. A .ci file declares one or
more *modules*, and each module contains one or more declarations for various
Charm++ constructs. For now, our simple example programs will all contain just
a single **mainmodule**, but in general programs can be broken into as many
modules as desired to organize the code. More specifics on modules can be found
in the manual.

An example .ci file is shown below, and can be found in the tutorial code in the
Charm++ repository: ``charm/examples/charm++/tutorial/1_mainchare/main.ci``.

.. Can we add a caption?
.. include:: examples/tutorial_1_main/main.ci
  :code: charmci
  :number-lines:

The first line declares a **mainmodule** named ``main``, followed by a pair of
curly braces. Between these braces, we can declare any number of chares that
we like. For now, we just declare a **mainchare** named ``Main`` on lines 2-4.
A chare declaration follows the form:

.. code:: c++

  chare-type chare-name {
    <body>
  }

The body is always a list of what are called *entry methods*. A more detailed
discussion of what entry methods are and how they are work will come in the
later parts of this tutorial. For now, it is enough to just think of them as
methods which the Charm++ runtime system is aware of. In this case ``Main`` has
a single entry method for its constructor, which takes a single argument of type
``CkArgMsg*``.

Now that we have written a very basic .ci file we can compile it using the
Charm++ compiler script that was built when you first built Charm++ in the
previous section. The compiler script can be found in ``charm/bin/charmc``.

To compile the .ci file, execute the following command:

.. code:: bash

  charmc main.ci

There should be no errors, and the compiler will have generated two new files:
``main.decl.h`` and ``main.def.h``. When compiling a .ci file, ``charmc`` will
always generate a ``*.decl.h`` and a ``*.def.h`` for each module defined in the
.ci file.  These files contain generated definitions for the chares declared in
these modules and will be used in the next step of this tutorial.

Chare definitions
-----------------

In the .ci file, we declared the chares that will be a part of our application.
Other than this thin interface, the rest of a Charm++ application (including the
actual definition of chares and their entry methods) is just regular C++ code.

Now we will define the **mainchare** we declared in the .ci file, and run our
first Charm++ program. Below is the code for defining the **mainchare**,
``Main``, declared in the previous section. It can be found in
``charm/examples/tutorial/1_mainchare/main.C``. As we can see, it is just a
standard C++ class definition with inheritance from a base class. Due to the
small size of the example, we've defined the whole class inline in a .C file,
but as with any C++ code, the class declaration and defintion can be split into
.h and .C files if desired.  Lines which contain Charm++ specific changes to
normal C++ usage are discussed below.

.. include:: examples/tutorial_1_main/main.C
  :code: c++
  :number-lines:

.. Find out EXACTLY where .decl.h should be included. I think its actually just
.. in any place where it is visible to class declarations?

The first difference we see is the inclusion of the generated ``.decl.h`` and
``.def.h`` files on lines 1 and 21 respectively. The ``.decl.h`` contains
generated code for classes related to the chares declared within that module,
and should be included before those classes are declared. Here we include it at
the start of the .C file. If we had split the file into a .h and a .C, the
``decl.h`` would be included in the .h file.

The ``.def.h`` includes definitions for the declarations generated in the
``.decl.h``, and therefore should be included in a single compilation unit.
Usually this means including it at the end of the .C file where the modules
classes are defined. If the class definitions are split across multiple .C
files, then the ``.def.h`` should only be included in one of them.

Beyond this, we have a normal C++ class definition for the class ``Main`` on
lines 3-19. The class name matches the name given to the chare type in the .ci
file, and the class publicly inherits from the generated class ``CBase_Main``.
Whenever defining a chare, ``Foo``, the generated class name will always be of
the form ``CBase_Foo``.  It also defines a method for each entry method
declared in the .ci file. In this case, it is only the constructor which needs
a definition. Because Main is just like any other C++ class, we could also
declare additional member variables and methods at this point. However, for this
example we only need the constructor for now.

Because this is a **mainchare**, the constructor is automatically invoked by
the runtime system during application startup, and is passed a pointer to a
``CkArgMsg``. A ``CkArgMsg`` is simply a wrapper around the command line
arguments used by the Charm++ runtime system. Lines 6 and 7 show how the
message is used to get the command line arguments, which can then be used just
as in a normal C++ program. After dealing with command line arguments, the
``CkArgMsg`` should be deleted.

In a normal application this is likely where we would begin creating the rest
of the chares for our application and start sending messages. In this simple
example, we simply print out the command line arguments (using a Charm++
specific output method, ``CkPrintf``, whose signature is the same as
``printf``), print out a Hello World message, and exit. To exit a
Charm++ program, we must call ``CkExit()``. ``CkExit()`` coordinates the exit
procedure across all processes no matter where the exit call originated from.
It only needs to be called once from anywhere in the program to
correctly exit.

To compile our code, we will again use the Charm++ compile script, ``charmc``:

.. code:: bash

  charmc main.C -o main

As an aside, as your programs become larger, you will want to use a Makefile to
coordinate compilation. A typical Makefile for a Charm++ program is shown below:

.. include:: examples/tutorial_1_main/Makefile
  :code: make

Running a Charm++ program
-------------------------

Now that we've succesfully built our first Charm++ program we need to run it.
How to run a Charm++ program can vary depending on the environment you are
where you are running, but in most cases, you will use the ``charmrun`` script
to run your executable. This script is placed in your build directory along
with the executable when you compile a charm program.

To run, use the following command:

.. code:: bash

  ./charmrun +p1 ./main

This tells ``charmrun`` to run the executable, ``main``, on 1 processor without
any extra arguments. If you get errors about SSH access denied, you can either
set up your machine to allow `something`, or add the ``++local`` option after
``+p1``. Your application should now run, print out some messages, and exit.
You can add any command line arguments intended for the application after the
executable name, and for our example this should print them out. The +p
argument controls the number of processes run on, however for this example, we
have only defined a **mainchare** which is always created exactly once on
processor 0. So running on higher +p will not change execution.

In the next tutorial we will learn how to define and create more chares, and
send messages between them.

Defining and Creating More Chares
=================================
In this section, we will learn how to define and create additional chares, and
how to send messages between them.  Code for this part of the tutorial is found
in ``charm/examples/tutorial/2_chare_creation``.

The CI file: Singleton Chares and Entry Methods
-----------------------------------------------

The most basic type of chare in Charm++ is the singleton chare. It represents a
single object in the application which has a global identifier and can be
communicated with via entry method invocations. As shown below, declaring a
singelton chare in a .ci file works similarly to declaring a **mainchare**. It
can be found at ``charm/examples/tutorial/2_chare_creation/singleton.ci``.

.. include:: examples/tutorial_2_singleton/singleton.ci
  :code: charmci
  :number-lines:

The only difference we see in the declaration on line 7 other than the name is
that the type is now just **chare**. We have also declared two entry methods on
lines 8 and 9. On line 8 we have the constructor, which follows the form:

.. code:: charmci

 entry <chare-name> (<parameter-list>);

This is the same as the **mainchare** constructor, however since this is not a
**mainchare** we can declare whatever parameters we like. The parameter list
declaration uses the same syntax as parameter lists in C++.

The second entry method, on line 9, is a regular entry method, whose
declaration follows the form:

.. code:: charmci

  entry void <method-name> (<parameter-list>);

For the purposes of this tutorial, all entry methods will have the ``void``
return type. It is possible to have a non-void return type, but this is an
advanced feature discuss in <somewhere>.

Chare Creation and Communication via Proxy
------------------------------------------

In the .C file, we see that defining the behavior of our new chare is largely
the same as before. This is shown in
``charm/examples/tutorial/2_chare_creation/singleton.C``.

.. include:: examples/tutorial_2_singleton/singleton.C
  :code: c++
  :number-lines:
  :start-line: 14
  :end-line: 35

In this snippet, we define a class with the same name (``MyChare``) as given in
the .ci file, and publicly inherit from ``CBase_MyChare``. We must also define
publicly the behavior of each entry method declared in the .ci file. In this
case, that means a constructor which takes a single ``int`` (lines 5-7), and a
``void`` method named ``hello`` which takes a ``bool`` (lines 9-15). On top of
that, we can add whatever members to the class that we like. In this case, we
just need a private ``int`` field to store the chares number (line 3). 

Now that we've defined the chare, how do we use it? In normal C++, we would
create a ``MyChare`` object by using ``new``. In Charm++, however, we need this
new object to be created as a parallel object managed by the runtime system. To
do so, we invoke the generated ``ckNew`` method associated with the chare as
follows:

.. code:: c++

  CProxy_MyChare::ckNew(n);

This call creates the ``MyChare`` object, and invokes its constructor, passing
an ``int``, ``n``, as the first and only argument. The method signature of
``ckNew`` matches the signature of the constructor declared in the .ci file,
which is why we pass a single ``int``. If we declared multiple constructors,
there would be a corresponding ``ckNew`` function generated for each one.
Unlike ``new``, this call does not return a pointer to the object. Instead, it
returns what is called a proxy to the object. A proxy is a global identifier
that can be used to communicate with the object, regardless of where the object
is located. To store a proxy to the object would look as follows:

.. code:: c++

  CProxy_MyChare proxy = CProxy_MyChare::ckNew(n);

``CProxy_MyChare`` is a class generated when compiling the .ci file which
contains static ``ckNew`` methods for creating the object. It also contains
methods for communicating with the chare based on the entry methods we declared
in the .ci file. Now that we have a proxy to the newly created chare,
communication with it is simply a matter of calling the entry methods on the
chare. For example:

.. code:: c++

  proxy.hello(true);

This will invoke the ``hello`` method on the chare, passing in ``true`` as the
first argument. This communication happens asynchronously, and the runtime
system manages packing up the entry method invocation and sending it to the
chare, wherever it may live.

Below we see the modified **mainchare** for this example program, which now
creates a single chare on line 7, and invokes ``hello`` on it on line 8. Upon
receiving the message, the chare will print out a message and exit the program.

.. include:: examples/tutorial_2_singleton/singleton.C
  :code: c++
  :number-lines:
  :end-line: 14

It is important to stress the asynchronous nature of communication. Here, on
line 8, the entry method invocation is sent to the newly created chare by
calling ``hello`` on ``proxy``. This does not wait for the message to actually
be delivered to the chare, nor does it wait for any kind of response. The
message is simply packed up and sent, and execution immediately resumes.
Because of this, the message printed out on line 12 may occur before or after
the invocation of ``hello`` on the chare actually occurs.

Extending the Example with Multiple Chares
------------------------------------------

Now that we have created our own singleton chare and sent it a message, it
follows that we will want to be able to create multiple chares that can
communicate amongst themselves. In
``charm/examples/tutorial/2_chare_creation/multiple.ci``, we declare two
different types of singleton chares: a ``Coordinator`` chare, and a ``Worker``
chare.

.. include:: examples/tutorial_2_singleton/multiple.ci
  :code: charmci
  :number-lines:

The main new thing to notice in this file is the use of proxies as parameters to
entry methods. This allows proxies to be communicated between chares so that
can communicate with one another. We can see how this works in ``charm/examples/tutorial/2_chare_creation/multiple.C``.

.. Only show definition of Main
.. include:: examples/tutorial_2_singleton/multiple.C
  :code: c++
  :number-lines:
  :start-line: 3
  :end-line: 23

The ``Main`` constructor creates a single ``Coordinator`` chare, and a user
specified number of ``Worker`` chares. Instead of saving the proxy to each
worker, the proxies are immediately sent to the ``Coordinator`` chare via the
``introduce`` entry method we declared in the .ci file.

.. Only show definition of Coordinator
.. include:: examples/tutorial_2_singleton/multiple.C
  :code: c++
  :number-lines:
  :start-line: 25
  :end-line: 44

Looking at the defintion of the ``Coordinator`` chare, we see a very basic
constructor, and then the definitions of the ``introduce`` and ``done`` entry
methods. The ``introduce`` method is how the ``Coordinator`` learns about new
``Worker`` chares. Upon receiving a proxy to a ``Worker`` chare, it invokes the
``work`` entry method on that ``Worker`` and passes it ``thisProxy``. Much like
``this`` in C++ objects, ``thisProxy`` is a member every chare has which is the
proxy that refers to itself. By passing it to the ``Worker`` chares, the
``Workers`` will be able to communicate back with the ``Coordinator``.

The ``done`` method simply increments a counter tracking how many times the
entry method is called, and exits when it is called once per ``Worker`` chare.

.. Only show definition of Coordinator
.. include:: examples/tutorial_2_singleton/multiple.C
  :code: c++
  :number-lines:
  :start-line: 46
  :end-line: 58

Lastly is the definition of the ``Worker`` chares. Upon creation, they print out
a message stating their number, and the value returned by ``CkMyPe()``, which is
a Charm++ function that returns the processor number of the caller.

The ``work`` method simply prints out a message and calls the ``done`` method
on the ``Coordinator`` chare.

.. Now running on multiple PEs makes sense, try running on different PE numbers.
.. Observe that execution order differs based on order of arrival, number of
.. PEs, etc. Re-iterate that all these message are sent asynchronously.

.. Segway into the next section by saying that this method of creating many
.. chares is not the normal way to do things, and that normally chare
.. collections are used, both to improve performance, get better perf, and
.. enable features like LB.

.. Also point to Fibonacci as another example of creating multiple chares and
.. passing around proxies.

Collections of Chares: Arrays
=============================

.. First, a ring example. Pass a message in a ring some number of times.
.. Then transition into broadcasts, use a 2D array this time maybe.
.. Still end by all sending p2p message to main.
.. Then add reductions, so they all contribute to main instead.

.. include:: examples/tutorial_3_arrays/ring.ci
  :code: charmci

.. include:: examples/tutorial_3_arrays/ring.C
  :code: c++

.. include:: examples/tutorial_3_arrays/collective.ci
  :code: charmci

.. include:: examples/tutorial_3_arrays/collective.C
  :code: c++

Collections of Chares: Groups
=============================

Collections of Chares: Node Groups
==================================
