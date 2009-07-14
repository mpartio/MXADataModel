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
#ifndef _MXA_DLL_EXPORT_H_
#define _MXA_DLL_EXPORT_H_

#include <MXA/MXAConfiguration.h>

/* Cmake will define MXADataModel_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MXADataModel_EXPORTS when
building the MXADatModel DLL on windows.
*/

#if defined (WIN32) && defined (MXA_BUILT_AS_DYNAMIC_LIB)
#if defined (_MSC_VER)
#pragma warning (push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 4668)
#pragma warning(disable: 4265)
#pragma warning(disable: 4189)
#pragma warning(disable: 4640)
#pragma warning(disable: 4996)
#pragma warning(disable: 4548)
#endif
  #if defined(MXADataModel_EXPORTS)
    #define  MXA_EXPORT __declspec(dllexport)
  #else
    #define  MXA_EXPORT __declspec(dllimport)
  #endif /* MXADataModel_EXPORTS */
#else /* defined (_WIN32) && defined (MXA_BUILD_SHARED_LIBS)  */
 #define MXA_EXPORT
#endif


#endif /* _MXA_DLL_EXPORT_H_ */
