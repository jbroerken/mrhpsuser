CBAccessClear
=============
The CBAccessClear callback is used to remove all current content 
access.

Action
------
The callback will check for any active content links and remove them 
from the linked user directory.

.. note::

    The linked directory inside the package directory will not 
    be removed.

Recieved Events
---------------
* MRH_EVENT_USER_ACCESS_CLEAR_U

Returned Events
---------------
* MRH_EVENT_USER_ACCESS_CLEAR_S

Files
-----
The callback is implemented in the following files:

.. code-block:: c

    Callback/Content/CBAccessClear.cpp
    Callback/Content/CBAccessClear.h