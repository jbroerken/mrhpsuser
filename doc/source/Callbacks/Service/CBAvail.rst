CBAvail
=======
The CBAvail callback is used to react to recieved service available 
request events.

Action
------
The callback will create a service available response event 
for the requested event. The event is added directly to the 
events to be sent to the currently used package.

The created availability response event is set up to contain the 
service identifiers mentioned in :doc:`../../Service_Info/Service_Info`. 
The result of the availability response event is positive if the 
service was able to reset all content access information. A failed 
reset indicates issues with file handling and will cause the service 
to return a unavailable result.

Recieved Events
---------------
* MRH_EVENT_USER_AVAIL_U

Returned Events
---------------
* MRH_EVENT_USER_AVAIL_S

Files
-----
The callback is implemented in the following files:

.. code-block:: c

    Callback/Service/CBAvail.cpp
    Callback/Service/CBAvail.h