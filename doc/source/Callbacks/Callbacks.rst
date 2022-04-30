*********
Callbacks
*********
The service uses callbacks to react to recieved events. All callbacks are 
given to libmrhpsb and executed on a callback thread. Service and location 
callbacks handle a specific event. The access content callback handles all 
types of content access events, while the clear access callback removes all 
content access at once.

Service Callbacks
-----------------
.. toctree::
  :maxdepth: 1

  Service Availability <Service/CBAvail>
  Service Reset <Service/CBReset>
  Custom Command <Service/CBCustomCommand>
  
  
Content Callbacks
-----------------
.. toctree::
  :maxdepth: 1

  Access User Content <Content/CBAccessContent>
  Clear User Content Access <Content/CBAccessClear>
  
  
Location Callbacks
------------------
.. toctree::
  :maxdepth: 1

  Get User Location <Location/CBGetLocation>
