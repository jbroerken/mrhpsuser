mrhpsuser
=========
.. image:: banner.png
   :align: center
   
The mrhpsuser platform service handles events of the user category. The service 
reacts to user file access and current location requests.

User file access is handled by linking directories into a service created linked 
directory inside the currently running package. The user location is recieved 
with the help of an external solution which gives the location data to mrhpsuser 
over a local stream.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Service_Info/Service_Info
   Building/Building
   User_Content/User_Content
   Configuration_File/Configuration_File
   Callbacks/Callbacks
