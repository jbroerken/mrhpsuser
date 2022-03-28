# mrhpsuser

<p align="center">
<img width="100%" height="100%" src="/doc/source/banner.png">
</p>


## About

The mrhpsuser project provides the default user platform service for the MRH platform.
It manages access events for user data and information as well as the user location.


## Requirements

#### Compilation

This project is built using CMake. You can find CMake here:

https://cmake.org/

#### Library Dependencies

This project requires other libraries and headers to function:

Dependency | Source
---------- | ------
libmrhbf |  https://github.com/jbroerken/libmrhbf/
libmrhevdata | https://github.com/jbroerken/libmrhevdata/
libmrhls |  https://github.com/jbroerken/libmrhls/
libmrhpsb | https://github.com/jbroerken/libmrhpsb/
mrhshared | https://github.com/jbroerken/mrhshared/

For more information about the requirements, check the "Building" section found in the documentation.


## Documentation

All documentation is build with sphinx-doc using the Read The Docs theme.
To build the documentation, grab the requirements for it:

#### sphinx-doc
https://www.sphinx-doc.org/en/master/

#### Read The Docs Theme
https://sphinx-rtd-theme.readthedocs.io/en/stable/

## Licence

This project is licenced under the GNU General Public 2 licence. 
Please read the included LICENCE.txt for the exact terms.


## Directories

This project supplies multiple directories for the development of said project. 
Their names and descriptions are as follows:

Directory | Description
--------- | -----------
bin | Contains the built project executables.
build | CMake build directory.
doc | Documentation files.
src | Project source code.
