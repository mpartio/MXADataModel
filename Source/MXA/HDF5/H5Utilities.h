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
#ifndef _HDF5_UTILITIES_H_
#define _HDF5_UTILITIES_H_


//#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypes.h>
//#include <MXA/Common/MXATypeDefs.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>

// C++ Includes
#include <map>
#include <list>
#include <string>

//-- HDF Headers
#include <hdf5.h>

/**
 * @brief General Utilities for working with the HDF5 data files and API
 * @author Mike Jackson/Shawn Nicholson
 * @date March 2007
 * @version $Revision: 1.2 $
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

  // -----------HDF5 File Operations
  static MXA_EXPORT hid_t openFile(const std::string &filename, bool readOnly=false);

  static MXA_EXPORT hid_t createFile(const std::string &filename);

  static MXA_EXPORT herr_t closeFile(hid_t &fileId);

  // -------------- HDF Indentifier Methods ----------------------------
  /**
  * @brief Retuirns the path to an object
  * @param objId The HDF5 id of the object
  * @param trim set to False to trim the path
  * @return  The path to the object relative to the objId
  */
  static MXA_EXPORT std::string getObjectPath(hid_t objId, bool trim=false);

  /**
  * @brief Returns the hdf object type
  * @param objId The hdf5 object id
  * @param objName The path to the data set
  * @param objType The type of the object
  * @return  Negative value on error
  */
  static MXA_EXPORT herr_t getObjectType(hid_t objId, const std::string &objName, int32 *objType);

  /**
  * @brief Retrieves the object name for a given index
  * @param fileId The hdf5 object id
  * @param idx The index to retrieve the name for
  * @param name The variable to store the name
  * @return Negative value is error
  */
  static MXA_EXPORT herr_t objectNameAtIndex(hid_t fileId, int32 idx, std::string &name);

  /**
  * @brief Returns if a given hdf5 object is a group
  * @param objId The hdf5 object that contains an object with name objName
  * @param objName The name of the object to check
  * @return True if the given hdf5 object id is a group
  */
  static MXA_EXPORT bool isGroup(hid_t objId, const std::string &objName);


  /**
  * @brief Opens an HDF5 object for hdf5 operations
  * @param locId the Object id of the parent
  * @param objectPath The path of the object to open
  * @return The hdf5 id of the opened object. Negative value is error.
  */
  static MXA_EXPORT hid_t openHDF5Object(hid_t locId, const std::string &objectPath);

  /**
  * @brief Closes the object id
  * @param locId The object id to close
  * @return Negative value is error.
  */
  static MXA_EXPORT herr_t closeHDF5Object(hid_t locId);


  static MXA_EXPORT std::string HDFClassTypeAsStr(hid_t class_type);

  /**
  * @brief prints the class type of the given class
  * @param classT The Class Type to print
  */
  static MXA_EXPORT void printHDFClassType(H5T_class_t classT);

  // -------------- HDF Group Methods ----------------------------
  /**
  * @brief Returns a list of child hdf5 objects for a given object id
  * @param loc_id The parent hdf5 id
  * @param typeFilter A filter to apply to the list
  * @param names Variable to store the list
  * @return
  */
  static MXA_EXPORT herr_t getGroupObjects(hid_t loc_id, int32 typeFilter, std::list<std::string>& names);

  /**
   * @brief Creates a HDF Group by checking if the group already exists. If the
   * group already exists then that group is returned otherwise a new group is
   * created.
   * @param loc_id The HDF unique id given to files or groups
   * @param group The name of the group to create. Note that this group name should
   * not be any sort of 'path'. It should be a single group.
   */
  static MXA_EXPORT hid_t createGroup(hid_t loc_id, const std::string &group);

  /**
   * @brief Given a path relative to the Parent ID, this method will create all
   * the intermediate groups if necessary.
   * @param pathToCheck The path to either create or ensure exists.
   * @param parent The HDF unique id for the parent
   * @return Error Condition: Negative is error. Positive is success.
   */
  static MXA_EXPORT herr_t  createGroupsFromPath(const std::string &pathToCheck, hid_t parent);

  /**
   * @brief Given a path relative to the Parent ID, this method will create all
   * the intermediate groups if necessary.
   * @param datasetPath The path to the dataset that you want to make all the intermediate groups for
   * @param parent The HDF unique id for the parent
   * @return Error Condition: Negative is error. Positive is success.
   */
  static MXA_EXPORT herr_t createGroupsForDataset(const std::string &datasetPath, hid_t parent);

  /**
  * @brief Extracts the object name from a given path
  * @param path The path which to extract the object name
  * @return The name of the object
  */
  static MXA_EXPORT std::string extractObjectName(const std::string &path);

  // -------------- HDF Attribute Methods ----------------------------
  /**
  * @brief Looks for an attribute with a given name
  * @param loc_id The objects Parent id
  * @param obj_name The name of the object
  * @param attr_name The attribute to look for (by name)
  * @return True if the attribute exists.
  */
  static MXA_EXPORT bool probeForAttribute(hid_t loc_id,
                                            const std::string &obj_name,
                                            const std::string &attr_name);


  /**
  * @brief Returns a list of all the attribute names
  * @param objId The parent object
  * @param names Variable to hold the list of attribute names
  * @return Negate value is error
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, std::list<std::string> &names);

  /**
  * @brief Returns a list of all the attribute names
  * @param objId The parent object
  * @param obj_name The name of the object whose attribute names you want a list
  * @param names Variable to hold the list of attribute names
  * @return Negative value is error
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, const std::string &obj_name,
                                                  std::list<std::string> &names);


  /**
   * @brief Returns a vector of IAttributes, one for each attribute of a given hdf5 object
   * @param fileId The parent hdf5 id
   * @param datasetPath The path to the hdf5 object whose attributes you want
   * @param attributes Variable to store the attributes
   * @return Negative value on error
   */
  static MXA_EXPORT herr_t readAllAttributes(hid_t fileId,
                                             const std::string &datasetPath,
                                             MXAAbstractAttributes &attributes);


/**
  * @brief Reads data  into an IMXAArrayPtr
  * @param locId The hdf5 object id of the parent
  * @param datasetPath The path to the data set containing the attributes you want
  * @param dims The dimensions of the attribute
  * @return Boost shared pointer to the data
  */
  template<typename T>
  static IMXAArrayPtr readH5Data( hid_t locId,
                                         const std::string &datasetPath,
                                         const std::vector<uint64> &dims)
  {
    herr_t err = -1;
    IMXAArrayPtr ptr;

    ptr = MXAArrayTemplate<T>::CreateMultiDimensionalArray( dims.size(), &(dims.front()));
    if (ptr.get() == NULL)
    {
      return ptr; // empty attribute
    }
    T* data = (T*)(ptr->getVoidPointer(0));
    err = H5Lite::readPointerDataset(locId, datasetPath, data);
    if ( err < 0)
    {
      std::cout << "readH5Data read error: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
      IMXAArray* nullData = 0x0;
      ptr.reset(nullData); // Swap in a null pointer
    }
    return ptr;
  }



/**
  * @brief Reads data from an Attribute into an IAttributePtr
  * @param locId The hdf5 object id of the parent
  * @param datasetPath The path to the data set containing the attributes you want
  * @param key The name of the attribute to read
  * @param dims The dimensions of the attribute
  * @return Boost shared pointer to the attribute
  */
  template<typename T>
  static IMXAArrayPtr readH5Attribute(  hid_t locId,
                                               const std::string &datasetPath,
                                               const std::string &key,
                                               const std::vector<uint64> &dims)
  {
    herr_t err = -1;
    IMXAArrayPtr ptr;
    if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
    {
      T data;
      err = H5Lite::readScalarAttribute(locId, datasetPath, key, data);
      if (err >= 0) {
        IMXAArrayPtr attr = MXAArrayTemplate<T>::CreateSingleValueArray( data);
        if (attr.get() != NULL)
        {
          ptr = attr;
        }
      }
    }
    else // Multi-Dimensional Data
    {
      IMXAArrayPtr attr = MXAArrayTemplate<T>::CreateMultiDimensionalArray( dims.size(), &(dims.front()));
      if (attr.get() == NULL)
      {
        return ptr; // empty attribute
      }
      // All the needed space is now preallocated in the attribute
      T* data = static_cast<T*>(attr->getVoidPointer(0) );
      err = H5Lite::readPointerAttribute(locId, datasetPath, key, data);
      if (err >= 0)
      {
         ptr = attr;
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



#endif /* _HDF5_UTILITIES_H_ */
