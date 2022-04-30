************
Service Info
************
mrhpsuser is the default user service used by the MRH platform. 
This service only implements default user events.

Service Identifiers
-------------------
The service uses the following identifiers:

.. list-table::
    :header-rows: 1

    * - Identifier
      - Value
    * - Supplier ID
      - MRH (0x4d524800)
    * - Binary ID
      - USER (0x55534552)
    * - Version
      - 1


Features
--------
The following features are provided by the service:
  
.. list-table::
    :header-rows: 1

    * - Feature
      - Description
    * - Handle user file access
      - The service is able to link user files into the package directory
        of the currently running service. Access will be removed once a 
        application finishes.
    * - Provide user location
      - The service stores the current user location if available.

  
Events
------
The following events are handled by the service:

* MRH_EVENT_PS_RESET_REQUEST_U
* MRH_EVENT_USER_AVAIL_U
* MRH_EVENT_USER_CUSTOM_COMMAND_U
* MRH_EVENT_USER_ACCESS_DOCUMENTS_U
* MRH_EVENT_USER_ACCESS_PICTURES_U
* MRH_EVENT_USER_ACCESS_MUSIC_U
* MRH_EVENT_USER_ACCESS_VIDEOS_U
* MRH_EVENT_USER_ACCESS_DOWNLOADS_U
* MRH_EVENT_USER_ACCESS_CLIPBOARD_U
* MRH_EVENT_USER_ACCESS_INFO_PERSON_U
* MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U
* MRH_EVENT_USER_ACCESS_CLEAR_U
* MRH_EVENT_USER_GET_LOCATION_U

.. note::
    
    Custom command events will return the event MRH_EVENT_NOT_IMPLEMENTED_S!