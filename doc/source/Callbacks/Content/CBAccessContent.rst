CBAccessContent
===============
The CBAccessContent callback is used give acces to a specified 
user content type.

Action
------
The callback will create a content link inside the linked user 
content directory inside the currently running application package. 
The type of content linked depends on the event recieved by the 
callback.

.. note::

    Linked content can be both a directory or a file.

Recieved Events
---------------
* MRH_EVENT_USER_ACCESS_DOCUMENTS_U
* MRH_EVENT_USER_ACCESS_PICTURES_U
* MRH_EVENT_USER_ACCESS_MUSIC_U
* MRH_EVENT_USER_ACCESS_VIDEOS_U
* MRH_EVENT_USER_ACCESS_DOWNLOADS_U
* MRH_EVENT_USER_ACCESS_CLIPBOARD_U
* MRH_EVENT_USER_ACCESS_INFO_PERSON_U
* MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U

Returned Events
---------------
* MRH_EVENT_USER_ACCESS_DOCUMENTS_S
* MRH_EVENT_USER_ACCESS_PICTURES_S
* MRH_EVENT_USER_ACCESS_MUSIC_S
* MRH_EVENT_USER_ACCESS_VIDEOS_S
* MRH_EVENT_USER_ACCESS_DOWNLOADS_S
* MRH_EVENT_USER_ACCESS_CLIPBOARD_S
* MRH_EVENT_USER_ACCESS_INFO_PERSON_S
* MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S

Files
-----
The callback is implemented in the following files:

.. code-block:: c

    Callback/Content/CBAccessContent.cpp
    Callback/Content/CBAccessContent.h