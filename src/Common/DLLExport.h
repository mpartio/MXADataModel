///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////


/* Cmake will define MXADataModel_EXPORTS on Windows when it 
configures to build a shared library. If you are going to use 
another build system on windows or create the visual studio
projects by hand you need to define MXADataModel_EXPORTS when 
building a DLL on windows.
*/

#if defined (_WIN32) && defined (MXA_BUILD_SHARED_LIBS)
#pragma warning(disable: 4251)
  #if defined(MXADataModel_EXPORTS) 
    #define  MXA_EXPORT __declspec(dllexport)
  #else
    #define  MXA_EXPORT __declspec(dllimport)
  #endif /* MXADataModel_EXPORTS */
#else /* defined (_WIN32) && defined (MXA_BUILD_SHARED_LIBS)  */
 #define MXA_EXPORT
#endif 
