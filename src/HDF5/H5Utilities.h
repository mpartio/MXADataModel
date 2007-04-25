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


#include "Headers/DLLExport.h"
#include "Headers/MXATypes.h"

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
 *  
 */
class H5Utilities
{

public:
  
  virtual ~H5Utilities();

  
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
  static MXA_EXPORT std::string getObjectPath(hid_t, bool trim=false);
  static MXA_EXPORT herr_t getObjectType(hid_t, std::string, int *);
  static MXA_EXPORT herr_t objectNameAtIndex(hid_t fileId, int idx, std::string &name);
  static MXA_EXPORT bool isGroup(hid_t nodeId, std::string objName);
  
  static MXA_EXPORT hid_t openHDF5Object(hid_t locId, std::string);
  static MXA_EXPORT herr_t closeHDF5Object(hid_t locId);

  static MXA_EXPORT void printHDFClassType(H5T_class_t);

  // -------------- HDF Group Methods ----------------------------
  static MXA_EXPORT herr_t getGroupObjects(hid_t, int, std::list<std::string> &);
  
  /**
   * @brief Creates a HDF Group by checking if the group already exists. If the 
   * group already exists then that group is returned otherwise a new group is 
   * created.
   * @param loc_id The HDF unique id given to files or groups
   * @param group The name of the group to create. Note that this group name should
   * not be any sort of 'path'. It should be a single group.
   */
  static MXA_EXPORT hid_t createGroup(hid_t loc_id, std::string group);
  
  /**
   * @brief Given a path relative to the Parent ID, this method will create all
   * the intermediate groups if necessary.
   * @param path The path to either create or ensure exists
   * @param parent The HDF unique id for the parent
   * @return Error Condition: Negative is error. Positive is success.
   */
  static MXA_EXPORT herr_t  createGroupsFromPath(std::string path, int32 parent);

  // -------------- HDF Attribute Methods ----------------------------
  static MXA_EXPORT bool probeForAttribute(hid_t, std::string, std::string);


  static MXA_EXPORT std::list<std::string> getAllAttributeNames(hid_t);
  static MXA_EXPORT std::list<std::string> getAllAttributeNames(hid_t, std::string);
  static MXA_EXPORT std::map<std::string, std::string> getAttributesMap(hid_t, std::string);
  
  
protected:
  H5Utilities() {}; //This is just a bunch of Static methods
  
private:
    H5Utilities(const H5Utilities&);   //Copy Constructor Not Implemented
    void operator=(const H5Utilities&); //Copy Assignment Not Implemented
};



#endif /* _HDF5UTILITIES_H_ */

