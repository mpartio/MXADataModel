#include "Utilities/StringUtils.h"
#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"
#include "Headers/MXATypes.h"


// C++ Includes
#include <iostream>

// *****************************************************************************************
// * H5Utilities Implementation Methods
// *****************************************************************************************


// -----------------------------------------------------------------------------
//  Returns the full path to the object referred to by the
// -----------------------------------------------------------------------------
std::string H5Utilities::getObjectPath(hid_t loc_id, bool trim)
{
  //char *obj_name;
  size_t name_size;
  name_size = 1 + H5Iget_name(loc_id, NULL, 0);
  std::vector<char> obj_name(name_size, 0);
  H5Iget_name(loc_id, &(obj_name.front()), name_size);
  std::string objPath(&(obj_name.front()));

  if ((objPath != "/") &&
      (objPath.at(0) == '/')) {
    objPath.erase(0, 1); 
  }
  
  return objPath;
}


// -----------------------------------------------------------------------------
// @brief Retrieves the HDF object type for obj_name at loc_id and stores
//    it in the parameter obj_type passed in.
// -----------------------------------------------------------------------------
herr_t H5Utilities::getObjectType(hid_t loc_id, std::string objName, 
				    int *obj_type)
{
  herr_t err=0;
  H5G_stat_t obj_info;
  
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &obj_info);
  if (err < 0) {
    return err;
  }

  (* obj_type) = obj_info.type;

  return err;
}



// Opens and returns the HDF object (since the HDF api requires
//  different open and close methods for different types of objects
hid_t H5Utilities::openHDF5Object(hid_t loc_id, std::string objName)
{
  int obj_type;
  hid_t obj_id;
  herr_t err=0;

  err = getObjectType(loc_id, objName, &obj_type);
  if (err < 0) {
    std::cout << "Error: Unable to get object type for object: " 
	      << objName << std::endl;
    return -1;
  }

  switch (obj_type) {
  case H5G_GROUP:
    obj_id = H5Gopen(loc_id, objName.c_str());
    break;
  case H5G_DATASET:
    obj_id = H5Dopen(loc_id, objName.c_str());
    break;
  default:
    std::cout << "Unknonwn HDF Type: " << obj_type << std::endl;
    obj_id = -1;
  }

  return obj_id;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5Utilities::closeHDF5Object(hid_t obj_id)
{
  H5I_type_t obj_type;
  herr_t err=-1;  // default to an error 

  obj_type = H5Iget_type(obj_id);
  
  switch(obj_type) {
  case H5I_FILE:
    err = H5Fclose(obj_id);
    break;
  case H5I_GROUP:
    err = H5Gclose(obj_id);
    break;
  case H5I_DATASET:
    err = H5Dclose(obj_id);
    break;
  case H5I_ATTR:
    err = H5Aclose(obj_id);
    break;
  case H5I_DATATYPE:
    err = H5Tclose(obj_id);
    break;
  case H5I_DATASPACE:
    err = H5Sclose(obj_id);
    break;
  default:
    std::cout << "Error unknown HDF object for closing: " 
	      << obj_type << std::endl;
    err = -1;
  }

  return err;
}
  
  


//--------------------------------------------------------------------//
// HDF Group Methods
//--------------------------------------------------------------------//

/*! @brief Returns a std::list of std::strings containing the names
 *   of all objects attached to the group referred to by loc_id
 * 
 * @parameter typeFilter is one of HDF5_GROUP, HDF5_DATASET, HDF5_TYPE,
 *  or HDF5_LINK or any combination of these using the bitwise or | 
 *  command.  Or you can pass in HDF5_ANY (equivalent to: HDF5_GROUP |
 *  HDF5_DATASET | HDF5_TYPE | HDF5_LINK) to not filter at all
 */
herr_t H5Utilities::getGroupObjects(hid_t loc_id, int typeFilter, std::list<std::string>& names)
{
  herr_t err=0;
  hsize_t numObjs = 0;
  
  err = H5Gget_num_objs(loc_id, &numObjs);
  if (err < 0) {
    std::cout << "Error getting number of objects for group: " 
	      << loc_id << std::endl;
    return err;
  }
  
  if (numObjs <= 0) {
    return 0; // no objects in group
  }
   
  size_t size=0;
  int type=-1;
  
  for (hsize_t i=0; i<numObjs; i++) {
    size = 1 + H5Gget_objname_by_idx(loc_id, i, NULL, 0);
    std::vector<char> name(size * sizeof(char), 0);
      
    H5Gget_objname_by_idx(loc_id, i, &(name.front()), size);
    if (typeFilter == MXA_ANY) {
      std::string objName( &(name.front()) );
      names.push_back(objName);
    } else {
      type = H5Gget_objtype_by_idx(loc_id, i);
      if ((type == H5G_GROUP) && (MXA_GROUP & typeFilter) ||
          (type == H5G_DATASET) && (MXA_DATASET & typeFilter) ||
          (type == H5G_TYPE) && (MXA_TYPE & typeFilter) ||
          (type == H5G_LINK) && (MXA_LINK & typeFilter)) 
      {
        std::string objName( &(name.front()) );
        names.push_back(objName);
      }
    }
  }
  
  return err;
}


// HDF Creation/Modification Methods
hid_t H5Utilities::createGroup(hid_t loc_id, std::string group) 
{
  hid_t test_id, grp_id;

  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);


  // check if it already exists
  test_id = H5Gopen(loc_id, group.c_str());
  if (test_id < 0) { 
    #if DEBUG1
    std::cout << "Group doesn't exist - creating: " << group << std::endl;
    #endif
    grp_id = H5Gcreate(loc_id, group.c_str(), 0);
  } else {
    #if DEBUG1
      std::cout << "Group already exists: " << group << std::endl;
    #endif
    grp_id = test_id;
  }

  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);

  return grp_id;
}

#if 0
There is a bug in here somewhere that is causing HDF5 to not create a group 
but create a dataset instead. Use the H5IODelegate->createGroupPaths() function instead.
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5Utilities::createGroups(hid_t loc_id, std::list<std::string> groups)
{
  hid_t grpId = loc_id;
  std::list<hid_t> groupIds;

  std::list<std::string>::const_iterator iter;
  for (iter=groups.begin(); iter!=groups.end(); iter++) {
    grpId = createGroup(grpId, *iter);
    groupIds.push_back(grpId);
  }
  std::list<hid_t>::const_iterator citer;
  for (citer=groupIds.begin(); citer!=groupIds.end(); citer++) {
    H5Gclose((*citer));
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5Utilities::createPathGroups(hid_t loc_id, std::string path, 
				     bool trimlast)
{
  std::list<std::string> groups = StringUtils::splitString("/", path);
  if (trimlast) {
    groups.pop_back();
  }
  createGroups(loc_id, groups);
}


#endif

//--------------------------------------------------------------------//
// HDF Attribute Methods
//--------------------------------------------------------------------//

/*! @brief Returns true if there is an attribute named attr_name
 *   associated with obj_name at loc_id
 */
bool H5Utilities::probeForAttribute(hid_t loc_id, std::string obj_name, 
				    std::string attr_name)
{
  herr_t err=0;
  int rank;
  err = H5Lite::getAttributeNDims(loc_id, obj_name, attr_name, rank);
  //err = H5LTget_attribute_ndims(loc_id, obj_name.c_str(), attr_name.c_str(), &rank);
  if (err < 0) {
    return false;
  } else {
    return true;
  } 
}


// Returns a std::list of all attribute names attached to the object
//  referred to by obj_id
std::list<std::string> H5Utilities::getAllAttributeNames(hid_t obj_id)
{
  std::list<std::string> results;
  int num_attrs;
  hid_t attr_id;
  size_t name_size;

  num_attrs = H5Aget_num_attrs(obj_id);

  for (int i=0; i<num_attrs; i++) {
    attr_id = H5Aopen_idx(obj_id, i);
    name_size = 1 + H5Aget_name(attr_id, 0, NULL);
    std::vector<char> attr_name(name_size * sizeof(char), 0);
    H5Aget_name(attr_id, name_size, &(attr_name.front() ) );
    results.push_back(std::string( &(attr_name.front() ) ) );
    H5Aclose(attr_id);
  }

  return results;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::list<std::string> H5Utilities::getAllAttributeNames(hid_t loc_id, 
					       std::string obj_name)
{
  hid_t obj_id;
  std::list<std::string> names;

  obj_id = openHDF5Object(loc_id, obj_name);
  if (obj_id < 0) {
    return names;
  }
  names = getAllAttributeNames(obj_id); 
  closeHDF5Object(obj_id);

  return names;
}




// Returns a std::map with all of the attributes for obj_name 
//  and their attribute values in a std::string std::map
std::map<std::string, std::string>
H5Utilities::getAttributesMap(hid_t loc_id, std::string obj_name)
{	
  //AbstractKeyValuePtr attr(new StringAttribute(key, value));
  std::map<std::string, std::string> attributes;
  herr_t err=0;
  H5T_class_t attr_type;
  size_t attr_size;
  std::string res;
  hid_t tid;
  MXATypes::Int32Vector ires;
  MXATypes::Float32Vector fres;
  std::vector<hsize_t> dims;  //Reusable for the loop
  std::list<std::string> names = getAllAttributeNames(loc_id, obj_name);
  std::list<std::string>::iterator iter;
  for (iter=names.begin(); iter != names.end(); iter++) 
  {
    err = H5Lite::getAttributeInfo(loc_id, obj_name, (*iter), dims, attr_type, attr_size, tid);
    if (err < 0) {
      std::cout << "Error getting all attributes" << std::endl;
    } else {
      switch(attr_type) 
      {
      case H5T_STRING:
      	err = H5Lite::readAttribute(loc_id, obj_name, (*iter), res );
      	if (err >= 0) {
      	  attributes[(*iter)] = res;
      	}
      	break;
      case H5T_INTEGER:
      	err = H5Lite::readAttribute(loc_id, obj_name, (*iter), ires, MXATypes::Int32Type);
      	if (err >= 0) {
      	  attributes[(*iter)] = StringUtils::numToString(ires[0]);
      	}
      	break;
      case H5T_FLOAT:
      	err = H5Lite::readAttribute(loc_id, obj_name, (*iter), fres, MXATypes::Float32Type);
      	if (err >= 0) {
      	  attributes[(*iter)] = StringUtils::numToString(fres[0]);
      	}
      	break;
      default:
      	std::cout << "Unknown attribute type: " << attr_type << ": ";
      	printHDFClassType(attr_type);
      }
    }
  }

  return attributes;
}



void H5Utilities::printHDFClassType(H5T_class_t class_type)
{
  switch(class_type) {
  case H5T_INTEGER:
    std::cout << "H5T_INTEGER" << std::endl;
    break;
  case H5T_FLOAT:
    std::cout << "H5T_FLOAT" << std::endl;
    break;
  case H5T_STRING:
    std::cout << "H5T_STRING" << std::endl;
    break;
  case H5T_TIME:
    std::cout << "H5T_TIME" << std::endl;
    break;
  case H5T_BITFIELD:
    std::cout << "H5T_BITFIELD" << std::endl;
    break;
  case H5T_OPAQUE:
    std::cout << "H5T_OPAQUE" << std::endl;
    break;
  case H5T_COMPOUND:
    std::cout << "H5T_COMPOUND" << std::endl;
    break;
  case H5T_REFERENCE:
    std::cout << "H5T_REFERENCE" << std::endl;
    break;
  case H5T_ENUM:
    std::cout << "H5T_ENUM" << std::endl;
    break;
  case H5T_VLEN:
    std::cout << "H5T_VLEN" << std::endl;
    break;
  case H5T_ARRAY:
    std::cout << "H5T_ARRAY" << std::endl;
    break;
  default:
    std::cout << "OTHER" << std::endl;
  }  

}


// -----------------------------------------------------------------------------
//  Returns a std::string that is the name of the object at the given index
// -----------------------------------------------------------------------------
herr_t H5Utilities::objectNameAtIndex(hid_t fileId, int idx, std::string &name)
{
  herr_t err = -1;
  // call H5Gget_objname_by_idx with name as NULL to get its length
  ssize_t name_len = H5Gget_objname_by_idx(fileId, idx, NULL, 0);
  if(name_len < 0) {
    return NULL;
  }
  
  std::vector<char> buf(name_len + 1, 0);
  err = H5Gget_objname_by_idx ( fileId, idx, &(buf.front()), name_len + 1 );
  if (err < 0) {
    std::cout << "Error Trying to get the dataset name for index " << idx << std::endl;
    name.clear(); //Make an empty string if this fails
  } else {
    name.append( &(buf.front()) ); //Append the string to the given string
  }
  return err;
}

// -----------------------------------------------------------------------------
// Checks the given name object to see what type of HDF5 object it is.
// -----------------------------------------------------------------------------
bool H5Utilities::isGroup(hid_t nodeId, std::string objName)   {
  bool isGroup = true;
  herr_t err = -1;
  H5G_stat_t statbuf;
  err = H5Gget_objinfo(nodeId, objName.c_str(), false, &statbuf);
  switch (statbuf.type) {
    case H5G_GROUP:
      isGroup = true;
      break;
    case H5G_DATASET:
      isGroup = false;
      break;
    case H5G_TYPE:
      isGroup = false;
      break;
    default:
      isGroup = false;
  }
  return isGroup;
}

