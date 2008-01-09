
#include <Base/IDataModel.h>
#include <Base/IDataRecord.h>
#include <HDF5/H5Utilities.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5DataArrayTemplate.hpp>
#include <HDF5/H5AttributeArrayTemplate.hpp>
#include <HDF5/H5AsciiStringAttribute.h>
#include <HDF5/H5AsciiStringData.h>
#include <Utilities/StringUtils.h>
#include <Common/LogTime.h>

// C++ Includes
#include <iostream>


#define CheckValidLocId(locId)\
  if (locId < 0 ) {std::cout << "Invalid HDF Location ID: " << locId << std::endl;return -1;}



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
herr_t H5Utilities::getObjectType(hid_t objId, const std::string &objName, int32 *objType)
{
  herr_t err=0;
  H5G_stat_t obj_info;
  
  err = H5Gget_objinfo(objId, objName.c_str(), 1, &obj_info);
  if (err < 0) {
    return err;
  }

  (* objType) = obj_info.type;

  return err;
}



// Opens and returns the HDF object (since the HDF api requires
//  different open and close methods for different types of objects
hid_t H5Utilities::openHDF5Object(hid_t loc_id, const std::string &objName)
{
  int32 obj_type;
  hid_t obj_id;
  herr_t err=0;
  HDF_ERROR_HANDLER_OFF;
  err = getObjectType(loc_id, objName, &obj_type);
  if (err < 0) {
    std::cout << "Error: Unable to get object type for object: " << objName << std::endl;
    HDF_ERROR_HANDLER_ON;
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
  HDF_ERROR_HANDLER_ON;
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
herr_t H5Utilities::getGroupObjects(hid_t loc_id, int32 typeFilter, std::list<std::string>& names)
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
  int32 type=-1;
  
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
hid_t H5Utilities::createGroup(hid_t loc_id, const std::string &group) 
{
  hid_t grp_id;
  herr_t err = -1;

  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);

  err = H5Gget_objinfo(loc_id, group.c_str(), 0, NULL);
//  std::cout << "H5Gget_objinfo = " << err << " for " << group << std::endl;  
  if (err == 0)
  {
    grp_id = H5Gopen(loc_id, group.c_str());
  } else 
  {
    grp_id = H5Gcreate(loc_id, group.c_str(), 0);
  }
  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);
  return grp_id;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5Utilities::createGroupsForDataset(const std::string &datasetPath, hid_t parent)
{
  // Generate the internal HDF dataset path and create all the groups necessary to write the dataset
  uint32 pos = 0;
  pos = datasetPath.find_last_of("/");
  std::string parentPath ( datasetPath.substr(0, pos)  );
  
  //Make sure all the intermediary groups are in place in the HDF5 File
  return H5Utilities::createGroupsFromPath(parentPath, parent);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5Utilities::createGroupsFromPath(const std::string &pathToCheck, hid_t parent)
{
  
  hid_t gid = 1;
  herr_t err = -1;
  std::string first;
  std::string second;
  std::string path (pathToCheck); //make a copy of the input
  
  
  if (parent <= 0) {
    std::cout << "Bad parent Id. Returning from createGroupsFromPath" << std::endl;
    return -1;
  }
  // remove any front slash
  std::string::size_type pos = path.find_first_of("/", 0);
  if ( 0 == pos ) 
  {
    path = path.substr(1, path.size());
  } 
  else if (pos == std::string::npos) // Path contains only one element
  {
    gid = H5Utilities::createGroup(parent, path);
    if (gid < 0)
    {
      std::cout << "Error creating group: " << path << " err:" << gid << std::endl;
      return gid;
    }
    err = H5Gclose(gid);
    if (err < 0) { std::cout << logTime() << "Error closing group during group creation." << std::endl; return err; }
    return err; //Now return here as this was a special case.
  }
  
  //Remove any trailing slash
  pos = path.find_last_of("/");
  if ( pos == (path.size() - 1) ) // slash was in the last position
  {
    path = path.substr(0, pos);
  }
  
  if (path.size() == 0) 
  {
    return -1; // The path that was passed in was only a slash.. 
  }

  
  pos = path.find_first_of("/", 0);
  if (pos == std::string::npos) // Only one element in the path
  {
    gid = H5Utilities::createGroup(parent, path);
    if (gid < 0)
    {
      std::cout << "Error creating group '" << path << "' for group id " << gid << std::endl;
      return gid;
    }
    err = H5Gclose(gid);
    return err;
  }
  
  while (pos != std::string::npos )
  {
    first = path.substr(0, pos);
    second = path.substr(pos+1, path.length() );
    gid = H5Utilities::createGroup(parent,first);
    if (gid < 0)
    {
      std::cout << "Error creating group:" << gid << std::endl;
      return gid;
    }
    err = H5Gclose(gid);
    pos = path.find_first_of("/", pos+1);
    if (pos == std::string::npos) 
    {
      first += "/" + second;
      gid = createGroup(parent,first);
      if (gid < 0)
      {
        std::cout << "Error creating group:" << gid << std::endl;
        return gid;
      }
      err = H5Gclose(gid);
    }
  }
  return err;
}


//--------------------------------------------------------------------//
// HDF Attribute Methods
//--------------------------------------------------------------------//

/*! @brief Returns true if there is an attribute named attr_name
 *   associated with obj_name at loc_id
 */
bool H5Utilities::probeForAttribute(hid_t loc_id, const std::string &obj_name, 
    const std::string &attr_name)
{
  herr_t err=0;
  int32 rank;
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
herr_t H5Utilities::getAllAttributeNames(hid_t obj_id, std::list<std::string> &results)
{
  if (obj_id < 0) { return -1; }
  herr_t err = -1;
  int32 num_attrs;
  hid_t attr_id;
  size_t name_size;

  num_attrs = H5Aget_num_attrs(obj_id);

  for (int i=0; i<num_attrs; i++) {
    attr_id = H5Aopen_idx(obj_id, i);
    name_size = 1 + H5Aget_name(attr_id, 0, NULL);
    std::vector<char> attr_name(name_size * sizeof(char), 0);
    H5Aget_name(attr_id, name_size, &(attr_name.front() ) );
    results.push_back(std::string( &(attr_name.front() ) ) );
    err = H5Aclose(attr_id);
  }

  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
 herr_t H5Utilities::getAllAttributeNames(hid_t loc_id, 
                                           const std::string &obj_name, 
                                           std::list<std::string> &names)
{
  hid_t obj_id = -1;
  herr_t err = -1;
  names.clear();

  obj_id = openHDF5Object(loc_id, obj_name);
  if (obj_id < 0) {
    return obj_id;
  }
  err = getAllAttributeNames(obj_id, names); 
  err = closeHDF5Object(obj_id);

  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5Utilities::readAllAttributes(hid_t fileId, const std::string &datasetPath, MXAAttributes &attributes)
{
  CheckValidLocId(fileId);
  herr_t err = -1;
  herr_t retErr = 1;
  hid_t typeId = -1;
  H5T_class_t attr_type;
  size_t attr_size;
  std::string res;
 
  std::vector<uint64> dims;  //Reusable for the loop
  std::list<std::string> names;
  err = H5Utilities::getAllAttributeNames(fileId, datasetPath, names );

  for (std::list<std::string>::iterator iter=names.begin(); iter != names.end(); iter++) 
  {
    err = H5Lite::getAttributeInfo(fileId, datasetPath, (*iter), dims, attr_type, attr_size, typeId);
    if (err < 0) {
      std::cout << "Error in getAttributeInfo method in readUserMetaData." << std::endl;
    } else {
      switch(attr_type) 
      {
      case H5T_STRING:
        res.clear(); //Clear the string out first
        err = H5Lite::readStringAttribute(fileId, datasetPath, (*iter), res );
        if (err >= 0) {
          MXAAttributePtr attr = MXAAttribute::createAttribute((*iter), res);
          attributes.push_back(attr);
        }
        break;
      case H5T_INTEGER:
        //std::cout << "User Meta Data Type is Integer" << std::endl;
        if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<uint8>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<uint16>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<uint32>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<uint64>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<int8>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<int16>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<int32>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<int64>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else {
          std::cout << "Unknown Type: " << typeId << " at " <<  datasetPath << std::endl;
          err = -1;
          retErr = -1;
        }
        break;
      case H5T_FLOAT:
        if (attr_size == 4) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<float32>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else if (attr_size == 8 ) {
          MXAAttributePtr ptr = H5Utilities::readPrimitiveAttribute<float64>(fileId, const_cast<std::string&>(datasetPath), (*iter), dims);
          attributes.push_back(ptr);
        } else {
          std::cout << "Unknown Floating point type" << std::endl;
          err = -1;
          retErr = -1;
        }
        break;
      default:
        std::cout << "Error: readUserMetaData() Unknown attribute type: " << attr_type << std::endl;
        H5Utilities::printHDFClassType(attr_type);
      }
      CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
    }
   if (retErr < 0)
   {
     break;
   }
  }

  return retErr;
}


// Returns a std::map with all of the attributes for obj_name 
//  and their attribute values in a std::string std::map
std::map<std::string, std::string> H5Utilities::getAttributesMap(hid_t loc_id, const std::string &obj_name)
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
  std::vector<uint64> dims;  //Reusable for the loop
  std::list<std::string> names;
  err = getAllAttributeNames(loc_id, obj_name, names);
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
      	err = H5Lite::readStringAttribute(loc_id, obj_name, (*iter), res );
      	if (err >= 0) {
      	  attributes[(*iter)] = res;
      	}
      	break;
      case H5T_INTEGER:
      	err = H5Lite::readVectorAttribute(loc_id, obj_name, (*iter), ires);
      	if (err >= 0) {
      	  attributes[(*iter)] = StringUtils::numToString(ires[0]);
      	}
      	break;
      case H5T_FLOAT:
      	err = H5Lite::readVectorAttribute(loc_id, obj_name, (*iter), fres);
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
herr_t H5Utilities::objectNameAtIndex(hid_t fileId, int32 idx, std::string &name)
{
  herr_t err = -1;
  // call H5Gget_objname_by_idx with name as NULL to get its length
  ssize_t name_len = H5Gget_objname_by_idx(fileId, idx, NULL, 0);
  if(name_len < 0) {
  	name.clear();
    return -1;
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
bool H5Utilities::isGroup(hid_t nodeId, const std::string &objName)   {
  bool isGroup = true;
  herr_t err = -1;
  H5G_stat_t statbuf;
  err = H5Gget_objinfo(nodeId, objName.c_str(), false, &statbuf);
  if (err < 0)
  {
    std::cout << DEBUG_OUT(logTime) << "Error in methd H5Gget_objinfo" << std::endl;
    return false;
  }
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

// -----------------------------------------------------------------------------
 //  Use a new set of indices to generate a path
 // -----------------------------------------------------------------------------
 std::string H5Utilities::generateH5PathToDataset ( IDataModelPtr model, 
                                                   std::vector<int32> &indices,  
                                                   IDataRecordPtr record) 
 {
   std::string path;
   //Put the data root on first
   path += model->getDataRoot();
   //Put the Data Dimensions on Next
   for (std::vector<int32>::iterator iter = indices.begin(); iter != indices.end(); ++iter ) {
     path += StringUtils::numToString(*iter);
     path += "/";
   }
    // Now build up the DataRecord path
   path = path + record->generatePath();
   return path;
 }


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractDataPtr H5Utilities::readDataArray(IDataFilePtr dataFile, const std::string &datasetPath)
{
  MXAAbstractDataPtr ptr;
  hid_t fileId = dataFile->getFileId();
  if (fileId < 0)
  {
    return ptr;
  }
  herr_t err = -1;
  herr_t retErr = 1;
  hid_t typeId = -1;
  H5T_class_t attr_type;
  size_t attr_size;
  std::string res;
 
  std::vector<hsize_t> dims;  //Reusable for the loop
  err = H5Lite::getDatasetInfo(fileId, datasetPath, dims, attr_type, attr_size);
  if (err < 0 )
  {
    return ptr;
  }
  if(dims.size() == 0)
  {
    std::cout << "dims.size() == 0. BAD" << std::endl;
    return ptr;
  }
  mxaIdType numElements = 1;
  for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }
  typeId = H5Lite::getDatasetType(fileId, datasetPath);
  if (typeId < 0)
  {
    return ptr;
  }
  
  switch(attr_type) 
  {
  case H5T_STRING:
    res.clear(); //Clear the string out first
    ptr = H5AsciiStringData::New(datasetPath, res);
    break;
  case H5T_INTEGER:
    //std::cout << "User Meta Data Type is Integer" << std::endl;
    if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
      ptr = H5DataArrayTemplate<uint8>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
       ptr = H5DataArrayTemplate<uint16>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
       ptr = H5DataArrayTemplate<uint32>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
       ptr = H5DataArrayTemplate<uint64>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
       ptr = H5DataArrayTemplate<int8>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
       ptr = H5DataArrayTemplate<int16>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
       ptr = H5DataArrayTemplate<int32>::CreateAbstractDataArray(datasetPath, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
       ptr = H5DataArrayTemplate<int64>::CreateAbstractDataArray(datasetPath, numElements);
    } else {
      std::cout << "Unknown Type: " << typeId << " at " <<  datasetPath << std::endl;
      err = -1;
      retErr = -1;
    }
    break;
  case H5T_FLOAT:
    if (attr_size == 4) {
      ptr = H5DataArrayTemplate<float32>::CreateAbstractDataArray(datasetPath, numElements);
    } else if (attr_size == 8 ) {
      ptr = H5DataArrayTemplate<float64>::CreateAbstractDataArray(datasetPath, numElements);
    } else {
      std::cout << "Unknown Floating point type" << std::endl;
      err = -1;
      retErr = -1;
    }
    break;
  default:
    std::cout << "Error: H5Utilities::readDatasetArray() Unknown attribute type: " << attr_type << std::endl;
    H5Utilities::printHDFClassType(attr_type);
  }
  CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  if (ptr.get() != NULL)
  {
    err = ptr->readFromFile(dataFile);
    if (err < 0)
    {
      MXAAbstractData* nullData = 0x0;
      ptr.reset(nullData); // Swap in a null pointer
    }
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractAttributePtr H5Utilities::readAttributeArray(IDataFilePtr dataFile, const std::string &datasetPath, const std::string &attributeKey)
{
  MXAAbstractAttributePtr ptr;
  hid_t fileId = dataFile->getFileId();
  if (fileId < 0)
  {
    return ptr;
  }
  herr_t err = -1;
  herr_t retErr = 1;
  hid_t typeId = -1;
  H5T_class_t attr_type;
  size_t attr_size;
  std::string res;
 
  std::vector<uint64> dims;  //Reusable for the loop
  err = H5Lite::getAttributeInfo(fileId, datasetPath, attributeKey, dims, attr_type, attr_size, typeId);
  if (err < 0 )
  {
    return ptr;
  }
  mxaIdType numElements = 1;
  for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }  
  switch(attr_type) 
  {
  case H5T_STRING:
    res.clear(); //Clear the string out first
    ptr = H5AsciiStringAttribute::New(datasetPath, attributeKey, res);
    break;
  case H5T_INTEGER:
    //std::cout << "User Meta Data Type is Integer" << std::endl;
    if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
      ptr = H5AttributeArrayTemplate<uint8>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
       ptr = H5AttributeArrayTemplate<uint16>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
       ptr = H5AttributeArrayTemplate<uint32>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
       ptr = H5AttributeArrayTemplate<uint64>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
       ptr = H5AttributeArrayTemplate<int8>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
       ptr = H5AttributeArrayTemplate<int16>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
       ptr = H5AttributeArrayTemplate<int32>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
     } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
       ptr = H5AttributeArrayTemplate<int64>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
    } else {
      std::cout << DEBUG_OUT(logTime) << "Unknown Integer Type: " << typeId << " for attribute:'" << attributeKey << "' for dataset:'" <<  datasetPath <<"'" << std::endl;
      err = -1;
      retErr = -1;
    }
    break;
  case H5T_FLOAT:
    if (attr_size == 4) {
      ptr = H5AttributeArrayTemplate<float32>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
    } else if (attr_size == 8 ) {
      ptr = H5AttributeArrayTemplate<float64>::CreateAbstractAttributeArray(datasetPath, attributeKey, numElements);
    } else {
      std::cout << DEBUG_OUT(logTime) << "Unknown Floating point type: " << typeId << " for attribute:'" << attributeKey << "' for dataset:'" <<  datasetPath <<"'" << std::endl;
      err = -1;
      retErr = -1;
    }
    break;
  default:
    std::cout << DEBUG_OUT(logTime) << "Error: H5Utilities::readAttributeArray()\n\t\t\t\t Unknown attribute_type:'" << attr_type << "' for typeId:'" << typeId << "' for attribute:'" << attributeKey << "' for dataset:'" <<  datasetPath <<"'" << std::endl;
    H5Utilities::printHDFClassType(attr_type);
  }
  CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  if (ptr.get() != NULL)
  {
    ptr->readFromFile(dataFile);
  }
  return ptr;
}

