#################################
Defining and Creating More Chares
#################################

In this section we will learn about a second type of chare, the singleton
chare.  We will learn how to define our own singleton chares, instantiate them,
and send messages between them via *entry methods*. Code for this part of the
tutorial is found in ``examples/tutorial_2_singlton``.

***********************************************
The CI file: Singleton Chares and Entry Methods
***********************************************

The most basic type of chare in Charm++ is the singleton chare. It represents a
single object in the application which has a global identifier and can be
communicated with via entry method invocations. As before, we start with
declaring the chare in a .ci file. Below is the .ci file used for this exercise.
It can be found at ``charm/examples/tutorial/2_chare_creation/singleton.ci``.

.. literalinclude:: examples/tutorial_2_singleton/singleton.ci
  :language: charmci
  :linenos:
  :emphasize-lines: 7-10
  :caption: examples/tutorial_2_singleton/singleton.ci

The new part here is the chare declaration on lines 7-10. This declaration is
largely the same as our **mainchare** with the primary difference being that the
declared type is **chare** instead of **mainchare**. We also have more freedom
with the constructors signature, and in this case we have the constructor take
a single ``int`` as a parameter on line 8.

We also have decided to declare an additional entry method on line 9. Unlike
the constructor, this entry method has a ``void`` return type. In general, the
syntax for declaring an entry method is as follows:

.. code:: charmci

  entry void <method-name> (<parameter-list>);

.. note:: Here, the <parameter-list> follows the same syntax as declaring
  parameter lists in C++.

.. note:: For the purposes of this tutorial, all entry methods will have the
  ``void`` return type. It is possible to have a non-void return type as
  discussed here: <link to manual>.

******************************************
Chare Creation and Communication via Proxy
******************************************

In the .C file, we see that defining the behavior of our new chare is largely
the same as before. This is shown in
``charm/examples/tutorial/2_chare_creation/singleton.C``.

.. literalinclude:: examples/tutorial_2_singleton/singleton.C
  :language: c++
  :linenos:
  :lines: 17-32
  :lineno-match:
  :caption: Snippet from examples/tutorial_2_singleton/singleton.C

In this snippet, we define a class with the same name (``MyChare``) as given in
the .ci file, and publicly inherit from ``CBase_MyChare``. We must also define
publicly the behavior of each entry method declared in the .ci file. In this
case, that means a constructor which takes a single ``int`` (lines 5-7), and a
``void`` method named ``hello`` which takes a ``bool`` (lines 9-15). On top of
that, we can add whatever members to the class that we like. In this case, we
just need a private ``int`` field to store the chares number (line 3). 

Now that we've defined the chare, how do we use it? In normal C++, we would
create a ``MyChare`` object by using ``new``. In Charm++, however, we need this
new object to be created as a chare managed by the runtime system. To do so, we
invoke the generated ``ckNew`` method associated with the chare as follows:

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

.. literalinclude:: examples/tutorial_2_singleton/singleton.C
  :language: c++
  :linenos:
  :lines: 3-16
  :emphasize-lines: 5-6
  :lineno-match:
  :caption: Snippet from examples/tutorial_2_singleton/singleton.C

It is important to stress the asynchronous nature of communication. Here, on
line 8, the entry method invocation is packed up as a message and is sent to
whichever processing element (PE) the new chare currently resides on. Once the
message is sent, execution resumes immediately, and does not block waiting for
the message to be received, or for ``hello`` to actually be invoked.
Because of this, the message printed out on line 12 may occur before or after
the message arrives on its destination PE. Furthermore, once the message
arrives, it is placed in that PEs message queue, and the actual invocation of
``hello`` will happen sometime later.

.. note:: For more details on the Charm++ scheduler and execution model, see: <something>

******************************************
Extending the Example with Multiple Chares
******************************************

Now that we have created our own singleton chare and sent it a message, it
follows that we will want to be able to create multiple chares that can
communicate amongst themselves. In
``charm/examples/tutorial/2_chare_creation/multiple.ci``, we declare two
different types of singleton chares: a ``Coordinator`` chare, and a ``Worker``
chare.

.. literalinclude:: examples/tutorial_2_singleton/multiple.ci
  :language: charmci
  :linenos:
  :emphasize-lines: 11,17
  :caption: examples/tutorial_2_singleton/multiple.ci

The main new thing to notice in this file is the use of proxies as parameters
to entry methods on lines 11 and 17. This allows proxies to be communicated
between chares so that they can communicate with one another. We can see how
this works in ``charm/examples/tutorial/2_chare_creation/multiple.C``.

.. literalinclude:: examples/tutorial_2_singleton/multiple.C
  :language: c++
  :linenos:
  :lines: 3-23
  :emphasize-lines: 14, 18
  :lineno-match:
  :caption: Definition of Main in examples/tutorial_2_singleton/multiple.C

The ``Main`` constructor creates a single ``Coordinator`` chare one line 16,
and a user specified number of ``Worker`` chares on line 20. Instead of saving
the proxy to each worker, the proxies are immediately sent to the
``Coordinator`` chare via the ``introduce`` entry method we declared in the .ci
file.

.. literalinclude:: examples/tutorial_2_singleton/multiple.C
  :language: c++
  :linenos:
  :lines: 25-44
  :emphasize-lines: 10
  :lineno-match:
  :caption: Definition of Coordinator in examples/tutorial_2_singleton/multiple.C

Looking at the defintion of the ``Coordinator`` chare, we see a very basic
constructor, and then the definitions of the ``introduce`` and ``done`` entry
methods.

The ``introduce`` method is how the ``Coordinator``
learns about new ``Worker`` chares. Upon receiving a proxy to a ``Worker``
chare, it invokes the ``work`` entry method on that ``Worker`` and passes it
``thisProxy`` on line 34. Much like ``this`` in C++ objects, ``thisProxy`` is a
member every chare has which is the proxy that refers to itself. By passing it
to the ``Worker`` chares, the ``Workers`` will be able to communicate back with
the ``Coordinator``.

The ``done`` method simply increments a counter tracking how many times the
entry method is called, and exits when it is called once per ``Worker`` chare.

.. literalinclude:: examples/tutorial_2_singleton/multiple.C
  :language: c++
  :linenos:
  :lines: 46-58
  :emphasize-lines: 11
  :lineno-match:
  :caption: Definition of Worker in examples/tutorial_2_singleton/multiple.C

Lastly is the definition of the ``Worker`` chares. Upon creation, they print out
a message stating their number, and the value returned by ``CkMyPe()``, which is
a Charm++ function that returns the PE of the caller.

The ``work`` method simply prints out a message and calls the ``done`` method
on the ``Coordinator`` chare on line 56.

.. Now running on multiple PEs makes sense, try running on different PE numbers.
.. Observe that execution order differs based on order of arrival, number of
.. PEs, etc. Re-iterate that all these message are sent asynchronously.

.. Segway into the next section by saying that this method of creating many
.. chares is not the normal way to do things, and that normally chare
.. collections are used, both to improve performance, get better perf, and
.. enable features like LB.

.. Also point to Fibonacci as another example of creating multiple chares and
.. passing around proxies.

