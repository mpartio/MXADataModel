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
#ifndef _HDF5UTILITIES_H_
#define _HDF5UTILITIES_H_


// C++ Includes
#include <map>
#include <list>
#include <string>

//-- HDF Headers
#include "hdf5.h"

/**
 * @brief
 * @author
 * @date
 * @version
 * @header
 */
class H5Utilities
{

public:
  
  virtual ~H5Utilities();
#if 0
  static const int MXA_GROUP = 1;
  static const int MXA_DATATYPE = 2;
  static const int MXA_TYPE = 4;
  static const int MXA_LINK = 8;
  static const int MXA_ANY = 15;
  #endif
  
  #if 1
  enum CustomHDFDataTypes {
   MXA_GROUP = 1,
   MXA_DATASET = 2,
   MXA_TYPE = 4,
   MXA_LINK = 8,
   MXA_ANY = 15
  };
  #endif
  
  // -------------- HDF Indentifier Methods ----------------------------
  static std::string getObjectPath(hid_t, bool trim=false);
  static herr_t getObjectType(hid_t, std::string, int *);
  static herr_t objectNameAtIndex(hid_t fileId, int idx, std::string &name);
  static bool isGroup(hid_t nodeId, std::string objName);
  
  static hid_t openHDF5Object(hid_t locId, std::string);
  static herr_t closeHDF5Object(hid_t locId);

  static void printHDFClassType(H5T_class_t);

  // -------------- HDF Group Methods ----------------------------
  static herr_t getGroupObjects(hid_t, int, std::list<std::string> &);
  static hid_t createGroup(hid_t, std::string);
  static void createGroups(hid_t, std::list<std::string>);
  static void createPathGroups(hid_t, std::string, bool trimlast=false);

  // -------------- HDF Attribute Methods ----------------------------
  static bool probeForAttribute(hid_t, std::string, std::string);


  static std::list<std::string> getAllAttributeNames(hid_t);
  static std::list<std::string> getAllAttributeNames(hid_t, std::string);
  static std::map<std::string, std::string> getAttributesMap(hid_t, std::string);
  
  
protected:
  H5Utilities(); //This is just a bunch of Static methods
  
private:
    H5Utilities(const H5Utilities&);   //Copy Constructor Not Implemented
    void operator=(const H5Utilities&); //Copy Assignment Not Implemented
};



#endif /* _HDF5UTILITIES_H_ */

