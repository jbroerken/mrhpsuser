************
User Content
************
All content which is accessed by the mrhpsuser service is defined by the events supported by 
the service. The type of content is provided by the service, supporting both folders and files.

User Directory
--------------
The user directory, managed by the service, contains all specific content accessed by the service. 
THe content stored here is static in the naming used. This allows the service to create links to 
the content.

Found inside the user directory is also the directory used to store links to the content. This 
directory is also statically named. The service will link this directory inside the currently 
running user application package to allow access.

.. note::

    The user directory can differ from the actual OS user directory. Using a custom or the 
    default user directory does not change the type of content accessible.

User Content Directories
------------------------
User content directories store data based on a type of content, like pictures or music. The 
content can be read and written by the applications having access to the directories.

.. list-table::
    :header-rows: 1

    * - Type
      - Description
    * - Content Link Directory
      - The directory used to store links to other 
        user content. Linked into the package directory.
    * - Documents Directory
      - The user documents directory.
    * - Pictures Directory
      - The user pictures directory.
    * - Music Directory
      - The user music directory.
    * - Videos Directory
      - The user videos directory.
    * - Downloads Directory
      - The user downloads directory.

User Content Files
------------------
The user also has specific files containing information. These files are accessed directly.

.. list-table::
    :header-rows: 1

    * - Type
      - Description
    * - Clipboard File
      - The clipboard text file can be used to store
        information to copy and paste.
    * - User Person Info File
      - The user person info file stores user info 
        like the first and last name, birthday and so 
        on.
    * - User Residence Info File
      - The residence info file stores residence info 
        for the user like country, city, street and more.
