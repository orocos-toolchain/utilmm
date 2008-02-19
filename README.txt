/*! \mainpage Util--: A C++ toolkit library
  
Util-- is a C++ toolkit library written by Sylvain Joyeux <sylvain.joyeux@m4x.org> and Frederic Py <fpy@laas.fr>

Copyright 2003-2008 LAAS/CNRS <openrobots@laas.fr>

This software is provided under the CeCILL B License. This license provides is
OSI-approved and provides terms of use comparable to the BSD license. See
LICENSE.txt and LICENSE.fr.txt, provided with the sources, for the complete terms.

\section Installation

\subsection Getting the sources
Releases are available on SourceForge: <a
href="http://sourceforge.net/projects/utilmm/">http://sourceforge.net/projects/utilmm/</a>.
You can access this project page from anywhere in the documentation by clicking
on the SourceForge.net logo at the bottom of documentation pages.

The development repository is managed by git and is (for now) publicly available in GitHub:

<pre>
  git clone git://github.com/doudou/utilmm.git
</pre>

(see <a href="http://github.com/doudou/">this page</a> for more information)

\subsection Dependencies
  - cmake and pkg-config
  - Boost libraries: boost, boost/thread, boost/regex, boost/filesystem plus the boost/graph headers
  - Boost/test to build the test suite
  - Doxygen for the documentation

\subsection Installation instructions
 
Go into the directory where the sources are installed and do

<pre>
   mkdir build
   cd build
   cmake ..
   make
   make doc
</pre>
 
Then, as root,
<pre>
   make install
</pre>

Alternatively, you can add the \c -DCMAKE_INSTALL_PREFIX=path/to/the/installation/target option to cmake
to install the files in a custom directory

*/
