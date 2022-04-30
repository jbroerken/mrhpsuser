CBReset
=======
The CBReset callback is used to react to a new user package 
being used.

Action
------
The callback will remove all active content links and try to 
create a user directory link to access user content.

Recieved Events
---------------
* MRH_EVENT_PS_RESET_REQUEST_U

Returned Events
---------------
None.

Files
-----
The callback is implemented in the following files:

.. code-block:: c

    Callback/Service/CBReset.cpp
    Callback/Service/CBReset.h