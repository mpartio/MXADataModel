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


#include "Common/DLLExport.h"
#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Core/MXAAttribute.h"
#include "HDF5/H5Lite.h"

// C++ Includes
#include <map>
#include <list>
#include <string>

//-- HDF Headers
#include "hdf5.h"

/**
 * @brief General Utilities for working with the HDF5 data files and API
 * @author Mike Jackson/Shawn Nicholson
 * @date March 2007
 * @version 1.0
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
  /**
  * @brief 
  * @param objId
  * @param trim=false
  * @return 
  */
  static MXA_EXPORT std::string getObjectPath(hid_t objId, bool trim=false);
 
  /**
  * @brief 
  * @param objId
  * @param datasetPath
  * @param type
  * @return 
  */
  static MXA_EXPORT herr_t getObjectType(hid_t objId, std::string datasetPath, int *type);
 
  /**
  * @brief 
  * @param fileId
  * @param idx
  * @param name
  * @return 
  */
  static MXA_EXPORT herr_t objectNameAtIndex(hid_t fileId, int idx, std::string &name);
 
  /**
  * @brief 
  * @param nodeId
  * @param objName
  * @return 
  */
  static MXA_EXPORT bool isGroup(hid_t nodeId, std::string objName);
  

  /**
  * @brief 
  * @param locId
  * @param objectPath
  * @return 
  */
  static MXA_EXPORT hid_t openHDF5Object(hid_t locId, std::string objectPath);

  /**
  * @brief 
  * @param locId
  * @return 
  */
  static MXA_EXPORT herr_t closeHDF5Object(hid_t locId);


  /**
  * @brief 
  * @param classT
  * @return 
  */
  static MXA_EXPORT void printHDFClassType(H5T_class_t classT);

  // -------------- HDF Group Methods ----------------------------
  /**
  * @brief 
  * @param groupId
  * @param typeFilter
  * @param list
  * @return 
  */
  static MXA_EXPORT herr_t getGroupObjects(hid_t groupId, int typeFilter, std::list<std::string> &list);
  
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
  static MXA_EXPORT herr_t  createGroupsFromPath(std::string path, hid_t parent);
  


  // -------------- HDF Attribute Methods ----------------------------
  /**
  * @brief 
  * @param loc_id
  * @param obj_name
  * @param attr_name
  * @return 
  */
  static MXA_EXPORT bool probeForAttribute(hid_t loc_id, 
                                            std::string obj_name, 
                                            std::string attr_name);


  /**
  * @brief 
  * @param objId
  * @return 
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, std::list<std::string> &names);

  /**
  * @brief 
  * @param objId
  * @param std::stringobj_name
  * @return 
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, std::string obj_name, std::list<std::string> &names);

  /**
  * @brief 
  * @param objId
  * @param obj_name
  * @return 
  */
  static MXA_EXPORT std::map<std::string, std::string> getAttributesMap(hid_t objId, std::string obj_name);
  
  /**
   * @brief
   * @param fileId
   * @param datasetPath
   * @param attributes
   * @return
   */
  static MXA_EXPORT herr_t readAllAttributes(hid_t fileId, const std::string &datasetPath, MXAAttributes &attributes);
  
/**
  * @brief 
  * @param locId
  * @param datasetPath
  * @param key
  * @param dims
  * @return 
  */
  template<typename T>
  static MXAAttributePtr readPrimitiveAttribute( hid_t locId, const std::string &datasetPath, 
                                 const std::string &key, 
                                 const std::vector<uint64> &dims)
  {
    herr_t err = -1;
    MXAAttributePtr ptr;
    if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
    {
      T data;
      err = H5Lite::readScalarAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data);
        return attr;
      }
    } 
    else // Multi-Dimensional Data 
    {
      std::vector<T> data;
      err = H5Lite::readVectorAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data, dims);
        return attr;
      }
    }
    return ptr;
  }

  
protected:
  H5Utilities() {}; //This is just a bunch of Static methods
  
private:
    H5Utilities(const H5Utilities&);   //Copy Constructor Not Implemented
    void operator=(const H5Utilities&); //Copy Assignment Not Implemented
};



#endif /* _HDF5UTILITIES_H_ */

