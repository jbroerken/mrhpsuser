CBGetLocation
=============
The CBGetLocation callback is used to recieve the current user 
location.

Action
------
The callback will read the current location retrieved from a external service 
and return that location info together with the information if the location is 
valid or not.

.. note::

    The service will return a invalid location as long as no location was 
    recieved, even if the external source exists and can supply location data.

Recieved Events
---------------
* MRH_EVENT_USER_GET_LOCATION_U

Returned Events
---------------
* MRH_EVENT_USER_GET_LOCATION_S

Files
-----
The callback is implemented in the following files:

.. code-block:: c

    Callback/Location/CBGetLocation.cpp
    Callback/Location/CBGetLocation.h