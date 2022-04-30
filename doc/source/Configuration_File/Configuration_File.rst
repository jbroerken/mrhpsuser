******************
Configuration File
******************
The mrhpsuser service loads a configuration file which contains information about the 
user content locations as well as the connection info for the location data socket used 
to retrieve the current user location from a external service.

The configuration file uses the MRH Block File format.

File Structure
--------------
The block file stores the source user data directory, the link directories, the linkeable 
content and the connection info in individual blocks. The source user data is found in the 
**UserSource** block, the link target directories in the **UserDestination** block, the user 
content to link in the **UserContent** block and the connection info in the **Server** block.

User Source Block
-----------------
The UserSource block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - SourceDirPath
      - The full path to the directory containing all user content 
        to link.
        
User Destination Block
----------------------
The UserDestination block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - ContentLinkDirPath
      - The full path to the directory to link into the 
        current package directory. Content links will be 
        stored inside this directory.
    * - PackageLinkDirPath
      - The relative path from the package directory root 
        where the content link path will be added in.
        
User Content Block
------------------
The UserContent block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - DocumentsDir
      - The name of the user documents directory to link.
    * - PicturesDir
      - The name of the user pictures directory to link.
    * - MusicDir
      - The name of the user music directory to link.
    * - VideosDir
      - The name of the user videos directory to link.
    * - DownloadsDir
      - The name of the user downloads directory to link.
    * - ClipboardFile
      - The name of the user clipboard file to link.
    * - InfoPersonFile
      - The name of the user person info file to link.
    * - InfoResidenceFile
      - The name of the user residence info file to link.
        
Server Block
------------
The Server block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - SocketPath
      - The full path to the socket file used for connecting 
        with the extern location service.

Example
-------
The following example shows a user service configuration file with 
default locations and names:

.. code-block:: c

    <MRHBF_1>
    
    <UserSource>{
        <SourceDirPath></var/mrh/mrhpsuser/>
    }

    <UserDestination>{
        <ContentLinkDirPath></var/mrh/mrhpsuser/_User/>
        <PackageLinkDirPath><FSRoot/_User>
    }

    <UserContent>{
        <DocumentsDir><Documents>
        <PicturesDir><Pictures>
        <MusicDir><Music>
        <VideosDir><Videos>
        <DownloadsDir><Downloads>
        <ClipboardFile><Clipboard.txt>
        <InfoPersonFile><UserPerson.conf>
        <InfoResidenceFile><UserResidence.conf>
    }

    <Server>{
        <SocketPath></tmp/mrh/mrhpsuser_location.sock>
    }
    
