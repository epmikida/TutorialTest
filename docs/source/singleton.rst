#################################
Defining and Creating More Chares
#################################

In this section, we will learn how to define and create additional chares, and
how to send messages between them.  Code for this part of the tutorial is found
in ``charm/examples/tutorial/2_chare_creation``.

***********************************************
The CI file: Singleton Chares and Entry Methods
***********************************************

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

******************************************
Chare Creation and Communication via Proxy
******************************************

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

******************************************
Extending the Example with Multiple Chares
******************************************

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

