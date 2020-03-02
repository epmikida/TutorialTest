##########################
Your First Charm++ Program
##########################

In simple "Hello World" example, we will learn how Charm++ programs start, and
how to define our very first chare.  Code for this part of the tutorial is
found in ``charm/examples/tutorial/1_mainchare``.

**********************************************
The Main Chare: Entry Point of the Application
**********************************************

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

CI Files: Declaring a Chare
===========================

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

C Files: Defining a Chare
=========================

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

.. Should we compile as we go, or move this to the last section and make it
.. compiling and running your first charm program. Probably it will stay here
.. to stay consistent. We compile after writing the .ci file which I think is
.. useful.

To compile our code, we will again use the Charm++ compile script, ``charmc``:

.. code:: bash

  charmc main.C -o main

As an aside, as your programs become larger, you will want to use a Makefile to
coordinate compilation. A typical Makefile for a Charm++ program is shown below:

.. include:: examples/tutorial_1_main/Makefile
  :code: make

**********************************
Running Your First Charm++ Program
**********************************

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

