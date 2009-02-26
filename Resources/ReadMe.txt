ReadMe.txt

To get started with MXADataModel there are a few things that you can do to make
things easier during development. A few environment variables can be set so that
when CMake is run the external libraries that are needed can be found easier. Those
environment variables are:

HDF5_INSTALL
EXPAT_INSTALL
TIFF_INSTALL
BOOST_ROOT
MXADataModel_INSTALL

An example of setting those variables would be the following for the bash shell
on unix systems (including OS X)

export HDF5_INSTALL="/opt/local/hdf5-166"
export EXPAT_INSTALL="/opt/local/expat"
export TIFF_INSTALL="/opt/local/tiff"
export BOOST_ROOT="/opt/local/Boost-1.36.0"
export MXADataModel_INSTALL="/opt/local/MXADataModel"

Here we have installed the libraries to /opt/local and have then used subdirectories 
within that install root to store each library and its files.

On windows you would use the "Advanced" tab from the "My Computer->Properties" to set
the environment variables.

After those are set then either developing with MXADataModel or actually adding to the
code base of MXADataModel will be much easier.

MXADataModel is dependent on the following libraries:
  Expat, Tiff, HDF5 V1.6.6, Boost 1.35 (or greater: filesystem, system, program_options, 
     thread, test)
  
All of these libraries should be compiled first before attempting to compile MXADataModel.
In addition if you are going to compile the GUI applications then you will need Qt version
 4.4.x installed also.
 
If you would like to write code to import your data there is a utility called 'ImportGenerator'
that can generate all the needed skeleton code and cmake project file to get you started.
The program is located in the 'bin' directory of the MXADataModel folder.

The general documentation is located in the doc/MXADataModel directory. A more detailed
set of API documentations is located in the doc/MXADataMOdel/API-Docs directory.

