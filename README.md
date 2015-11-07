GTee
====

Introduction
------------
A graphical tee Win32 application to inspect data piped through command line

Building
--------
 1. Clone the project and cd its directory.
 2. Run:  
    ```
    runhaskell Shakefile.hs --variant=<VARIANT> --arch=<ARCH> --toolchain=<TOOLCHAIN>
    ```
    where <VARIANT> can be either Release|Debug, <ARCH> can be either x86|x64
    and <TOOLCHAIN> can be either MSVC|GCC|LLVM.
 3. Built binaries will reside in the `bin\<ARCH>\<VARIANT>` directory.

ChangeLog
---------
 * TODO: Track major changes

Contributing
------------
 * For bug fixes, any well checked pull requests are welcome

Credits
-------
Written and maintained by: 
* "Agorgianitis Loukas" <agorglouk@gmail.com>

Licensing
---------
Read [LICENSE](LICENSE.md)  

Copyright (C) 2015 Agorgianitis Loukas <agorglouk@gmail.com>  
All rights reserved.

