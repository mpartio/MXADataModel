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
#ifndef HDF5HIGHLEVEL_H_
#define HDF5HIGHLEVEL_H_

#include "Headers/DLLExport.h"

//-- C++ Headers
#include <string>
#include <iostream>
#include <vector>

//-- HDF Headers
#include "hdf5.h"



class  H5Lite
{
public:

/**
 * @brief Opens and object for HDF5 operations
 * @param loc_id The parent object that holds the true object we want to open
 * @param obj_name The string name of the object
 * @param The HDF5_TYPE of object
 * @return Standard HDF5 Error Conditions
 */
static herr_t openId( hid_t loc_id, std::string obj_name, int obj_type);
                     
/**
 * @brief Opens an HDF5 Object
 * @param obj_id The Object id
 * @param obj_type Basic Object Type
 * @return Standard HDF5 Error Conditions
 */
static herr_t closeId( hid_t obj_id, int obj_type );

template<typename T>
static hid_t HDFTypeForPrimitive(T value)
{
  if (typeid(value) == typeid(char)) return H5T_NATIVE_INT8;
  if (typeid(value) == typeid(signed char)) return H5T_NATIVE_INT8;  
  if (typeid(value) == typeid(unsigned char)) return H5T_NATIVE_UINT8;
  
  if (typeid(value) == typeid(short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(signed short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(unsigned short int)) return H5T_NATIVE_UINT16;
  
  if (typeid(value) == typeid(int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(long int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed long int)) return H5T_NATIVE_INT32;
  
  if (typeid(value) == typeid(unsigned int)) return H5T_NATIVE_UINT32;
  if (typeid(value) == typeid(unsigned long int)) return H5T_NATIVE_UINT32;
  
  if (typeid(value) == typeid(long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(signed long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(unsigned long long int)) return H5T_NATIVE_UINT64;
  
  
  if (typeid(value) == typeid(float)) return H5T_NATIVE_FLOAT;
  if (typeid(value) == typeid(double)) return H5T_NATIVE_DOUBLE;
  
  std::cout << "Error: HDFTypeForPrimitive - Unknown Type: " << typeid(value).name() << std::endl;
  return -1;
}



/**
 * @brief Inquires if an attribute named attr_name exists attached to the object loc_id.
 * @param loc_id The location to search
 * @param attrName The attribute to search for
 * @return Standard HDF5 Error condition
 */
static herr_t findAttribute( hid_t loc_id, std::string attrName );

/**
 * @brief Creates a Dataset with the given name at the location defined by loc_id
 * 
 * The permissible types that can be used for dataType are:
 * <ul>
 * <li>H5T_NATIVE_INT8</li>
 * <li>H5T_NATIVE_UINT8</li>
 * <li>H5T_NATIVE_INT16</li>
 * <li>H5T_NATIVE_UINT16</li>
 * <li>H5T_NATIVE_INT32</li>
 * <li>H5T_NATIVE_UINT32</li>
 * <li>H5T_NATIVE_INT64</li>
 * <li>H5T_NATIVE_UINT64</li>
 * <li>H5T_NATIVE_FLOAT</li>
 * <li>H5T_NATIVE_DOUBLE</li>
 * </ul>
 * 
 * @param loc_id The Parent location to store the data
 * @param datasetName The name of the dataset
 * @param dims The dimensions of the dataset
 * @param data The data to write to the file
 * @param dataType The HDF_Type of data
 * @return Standard HDF5 error conditions
 * 
 * The dimensions of the data sets are usually passed as both a "rank" and 
 * dimensions array. By using a std::vector<hsize_t> that stores the values of
 * each of the dimensions we can reduce the number of arguments to this method as
 * the value of the "rank" simply becomes dims.length(). So to create a Dims variable
 * for a 3D data space of size(x,y,z) = {10,20,30} I would use the following code:
 * <code>
 * std::vector<hsize_t> dims;
 * dims.push_back(10);
 * dims.push_back(20);
 * dims.push_back(30);
 * </code>
 * 
 * Also when passing data BE SURE that the type of data and the data type match. 
 * For example if I create some data in a std::vector<UInt8Type> I would need to
 * pass H5T_NATIVE_UINT8 as the dataType.
 */
template <typename T>
static herr_t writeDataset (hid_t loc_id, std::string &datasetName, 
                           std::vector<hsize_t> &dims, std::vector<T> &data,
                           hid_t dataType)
{
  herr_t err = -1;
  hid_t did = -1;
  hid_t sid = -1;
  herr_t retErr = 0;
  //Create the DataSpace
  sid = H5Screate_simple( dims.size(), &(dims.front()), NULL );
  if (sid < 0) 
  {
    return sid;
  }
  // Create the Dataset
  did = H5Dcreate (loc_id, datasetName.c_str(), dataType, sid, H5P_DEFAULT);
  if ( did >= 0 ) 
  {
    err = H5Dwrite( did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(data.front()) );
    if (err < 0 ) {
      std::cout << "Error Writing Data" << std::endl;
      retErr = err;
    }
    err = H5Dclose( did );
    if (err < 0) {
      std::cout << "Error Closing Dataset." << std::endl;
      retErr = err;
    }
  }
  /* Terminate access to the data space. */
  err= H5Sclose( sid );
  if (err< 0) {
    std::cout << "Error Closing Dataspace" << std::endl;
    retErr = err; 
  }
  return retErr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
static herr_t writeDataset (hid_t loc_id,
                            std::string &dsetName,
                            T &value,
                            hid_t dataType) 
{
  herr_t err = -1;
  hid_t did = -1;
  hid_t sid = -1;
  herr_t retErr = 0;
  hsize_t dims = 1;
  hid_t rank = 1;
  //Create the DataSpace
  sid = H5Screate_simple( rank, &(dims), NULL );
  if (sid < 0) 
  {
    return sid;
  }
  // Create the Dataset
  did = H5Dcreate (loc_id, dsetName.c_str(), dataType, sid, H5P_DEFAULT);
  if ( did >= 0 ) 
  {
    err = H5Dwrite( did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value );
    if (err < 0 ) {
      std::cout << "Error Writing Data" << std::endl;
      retErr = err;
    }
    err = H5Dclose( did );
    if (err < 0) {
      std::cout << "Error Closing Dataset." << std::endl;
      retErr = err;
    }
  }
  /* Terminate access to the data space. */
  err= H5Sclose( sid );
  if (err< 0) {
    std::cout << "Error Closing Dataspace" << std::endl;
    retErr = err; 
  }
  return retErr;
}

/**
 * @brief Writes a std::string as a HDF Dataset.
 * @param loc_id The Parent location to write the dataset
 * @param dset_name The Name to use for the dataset
 * @param data The actual data to write as a null terminated string
 * @return Standard HDF5 error conditions
 */
static herr_t writeDataset (hid_t loc_id, std::string &dset_name, std::string &data);

/**
 * @brief Writes an Attribute to an HDF5 Object
 * @param loc_id The Parent Location of the HDFobject that is getting the attribute
 * @param objName The Name of Object to write the attribute into.
 * @param attrName The Name of the Attribute
 * @param tid The HDF5 Type of the Attribute
 * @param data The Attribute Data to write
 * @return Standard HDF Error Condition
 *  
 * The permissible types that can be used for dataType are:
 * <ul>
 * <li>H5T_NATIVE_INT8</li>
 * <li>H5T_NATIVE_UINT8</li>
 * <li>H5T_NATIVE_INT16</li>
 * <li>H5T_NATIVE_UINT16</li>
 * <li>H5T_NATIVE_INT32</li>
 * <li>H5T_NATIVE_UINT32</li>
 * <li>H5T_NATIVE_INT64</li>
 * <li>H5T_NATIVE_UINT64</li>
 * <li>H5T_NATIVE_FLOAT</li>
 * <li>H5T_NATIVE_DOUBLE</li>
 * </ul>
 */
template <typename T>
static herr_t writeAttribute(hid_t loc_id, 
                             std::string &objName, 
                             std::string &attrName, 
                             std::vector<hsize_t> &dims, 
                             std::vector<T> &data, 
                             hid_t dataType )
{
  hid_t      obj_id, sid, attr_id;
  //hsize_t    dim_size = data.size();
  int        has_attr;
  H5G_stat_t statbuf;
  herr_t err = 0;
  herr_t retErr = 0;
  /* Get the type of object */
  if (H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf) < 0) {
    std::cout << "Error getting object info." << std::endl;
    return -1;
  }
  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
  if ( obj_id < 0) {
    std::cout << "Error opening Object for Attribute operations." << std::endl;
    return -1;
  }
  
  /* Create the data space for the attribute. */
  hsize_t* dimsPtr = &(dims.front());
  sid = H5Screate_simple( dims.size(), dimsPtr, NULL );
  if ( sid >= 0 ) {
    /* Verify if the attribute already exists */
    has_attr = H5Lite::findAttribute( obj_id, attrName );
    
    /* The attribute already exists, delete it */
    if ( has_attr == 1 ) {
      err = H5Adelete( obj_id, attrName.c_str() );
      if (err < 0) {
        std::cout << "Error Deleting Existing Attribute" << std::endl;
        retErr = err; 
      }
    }
    
    if ( err >= 0 ) {
      /* Create the attribute. */
      attr_id = H5Acreate( obj_id, attrName.c_str() , dataType, sid, H5P_DEFAULT );
      if ( attr_id >= 0 ) {
        /* Write the attribute data. */
        err = H5Awrite( attr_id, dataType, &(data.front()) );
        if ( err < 0 ) {
          std::cout << "Error Writing Attribute" << std::endl;
          retErr = err;
        }
      }
      /* Close the attribute. */
      err = H5Aclose( attr_id );
      if (err < 0) {
        std::cout << "Error Closing Attribute" << std::endl;
        retErr = err; 
      }
    }
    /* Close the dataspace. */
    err = H5Sclose( sid );
    if ( err < 0 ) {
      std::cout << "Error Closing Dataspace" << std::endl;
      retErr = err;
    }
  }
  
  /* Close the object */
  err = H5Lite::closeId( obj_id, statbuf.type );
  if ( err < 0 ) {
    std::cout << "Error Closing HDF5 Object ID" << std::endl;
    retErr = err;
  }
  return retErr;  
}

/**
 * @brief
 */
template <typename T>
static herr_t writeAttribute(hid_t loc_id, 
                             std::string &objName, 
                             std::string &attrName, 
                             T &data, 
                             hid_t dataType )
{
  hid_t      obj_id, sid, attr_id;
  int        has_attr;
  H5G_stat_t statbuf;
  herr_t err = 0;
  herr_t retErr = 0;
  hsize_t dims = 1;
  hid_t rank = 1;
  /* Get the type of object */
  if (H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf) < 0) {
    std::cout << "Error getting object info." << std::endl;
    return -1;
  }
  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
  if ( obj_id < 0) {
    std::cout << "Error opening Object for Attribute operations." << std::endl;
    return -1;
  }
  
  /* Create the data space for the attribute. */
  sid = H5Screate_simple( rank, &dims, NULL );
  if ( sid >= 0 ) {
    /* Verify if the attribute already exists */
    has_attr = H5Lite::findAttribute( obj_id, attrName );
    
    /* The attribute already exists, delete it */
    if ( has_attr == 1 ) {
      err = H5Adelete( obj_id, attrName.c_str() );
      if (err < 0) {
        std::cout << "Error Deleting Existing Attribute" << std::endl;
        retErr = err; 
      }
    }
    
    if ( err >= 0 ) {
      /* Create the attribute. */
      attr_id = H5Acreate( obj_id, attrName.c_str() , dataType, sid, H5P_DEFAULT );
      if ( attr_id >= 0 ) {
        /* Write the attribute data. */
        err = H5Awrite( attr_id, dataType, &data );
        if ( err < 0 ) {
          std::cout << "Error Writing Attribute" << std::endl;
          retErr = err;
        }
      }
      /* Close the attribute. */
      err = H5Aclose( attr_id );
      if (err < 0) {
        std::cout << "Error Closing Attribute" << std::endl;
        retErr = err; 
      }
    }
    /* Close the dataspace. */
    err = H5Sclose( sid );
    if ( err < 0 ) {
      std::cout << "Error Closing Dataspace" << std::endl;
      retErr = err;
    }
  }
  
  /* Close the object */
  err = H5Lite::closeId( obj_id, statbuf.type );
  if ( err < 0 ) {
    std::cout << "Error Closing HDF5 Object ID" << std::endl;
    retErr = err;
  }
  return retErr;  
}

/**
 * @brief Writes a string as a null terminated attribute.
 * @param loc_id The location to look for objName
 * @param objName The Object to write the attribute to
 * @param attrName The name of the Attribute
 * @param data The string to write as the attribute
 * @return Standard HDF error conditions
 */
static herr_t writeAttribute(hid_t loc_id, 
                              std::string &objName, 
                              std::string &attrName, 
                              std::string &data );

/**
 * @brief Reads data from the HDF5 File
 * @param loc_id The parent location that contains the dataset to read
 * @param dsetname The name of the dataset to read
 * @param data A std::vector<T>. Note the vector WILL be resized to fit the data.
 * The best idea is to just allocate the vector but not to size it. The method 
 * will size it for you.
 * @param dataType An H5T_Type constant. See the notes below.
 * @return Standard HDF error condition
 * 
 * <b>The permissible types that can be used for dataType are:</b>
 * <ul>
 * <li>H5T_NATIVE_INT8</li>
 * <li>H5T_NATIVE_UINT8</li>
 * <li>H5T_NATIVE_INT16</li>
 * <li>H5T_NATIVE_UINT16</li>
 * <li>H5T_NATIVE_INT32</li>
 * <li>H5T_NATIVE_UINT32</li>
 * <li>H5T_NATIVE_INT64</li>
 * <li>H5T_NATIVE_UINT64</li>
 * <li>H5T_NATIVE_FLOAT</li>
 * <li>H5T_NATIVE_DOUBLE</li>
 * </ul>
 */
template <typename T>
static herr_t readDataset(hid_t loc_id, std::string dsetName, std::vector<T> &data,
                          hid_t dataType) {
  hid_t   did;
  herr_t  err = 0;
  herr_t retErr = 0;
  hid_t spaceId;
  
  
 /* Open the dataset. */
// std::cout << "  Opening " << dsetName << " for data Retrieval.  " << std::endl;
  did = H5Dopen( loc_id, dsetName.c_str() );
  if ( did < 0 ) {
    std::cout << " Error opening Dataset: " << did << std::endl;
    return -1;
  }
  if ( did >= 0 ) {
    spaceId = H5Dget_space(did);
    if ( spaceId > 0 ) {
      int rank = H5Sget_simple_extent_ndims(spaceId);
      if (rank > 0) {
        std::vector<hsize_t> dims;
        dims.resize(rank);// Allocate enough room for the dims
        rank = H5Sget_simple_extent_dims(spaceId, &(dims.front()), NULL);
        hsize_t numElements = 1;
        for (std::vector<hsize_t>::iterator iter = dims.begin(); iter < dims.end(); ++iter ) {
          numElements = numElements * (*iter); 
        }
        //Resize the vector
        data.resize( static_cast<int>(numElements) );
        err = H5Dread(did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, &( data.front() ) );
        if (err < 0) {
          std::cout << "Error Reading Data." << std::endl; 
          retErr = err;
        }
      }
      err = H5Sclose(spaceId);
      if (err < 0 ) {
        std::cout << "Error Closing Data Space" << std::endl;
        retErr = err;
      }
    }
    err = H5Dclose( did );
    if (err < 0 ) {
      std::cout << "Error Closing Dataset" << std::endl;
      retErr = err;
    }
  }
  return retErr;
}

/**
 * @brief
 */
template <typename T>
static herr_t readDataset(hid_t loc_id, std::string dsetName, T &data, hid_t dataType)
{
  hid_t   did;
  herr_t  err = 0;
  herr_t retErr = 0;
  hid_t spaceId;
 // hsize_t dims = 1;
  
 /* Open the dataset. */
  did = H5Dopen( loc_id, dsetName.c_str() );
  if ( did < 0 ) {
    std::cout << "Error opening Dataset." << std::endl;
    return -1;
  }
  if ( did >= 0 ) {
    spaceId = H5Dget_space(did);
    if ( spaceId > 0 ) {
      int rank = H5Sget_simple_extent_ndims(spaceId);
      if (rank > 0) {
        std::vector<hsize_t> dims;
        dims.resize(rank);// Allocate enough room for the dims
        rank = H5Sget_simple_extent_dims(spaceId, &(dims.front()), NULL);
        hsize_t numElements = 1;
        for (std::vector<hsize_t>::iterator iter = dims.begin(); iter < dims.end(); ++iter ) {
          numElements = numElements * (*iter); 
        }
        err = H5Dread(did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data );
        if (err < 0) {
          std::cout << "Error Reading Data." << std::endl; 
          retErr = err;
        }
      }
      err = H5Sclose(spaceId);
      if (err < 0 ) {
        std::cout << "Error Closing Data Space" << std::endl;
        retErr = err;
      }
    }
    err = H5Dclose( did );
    if (err < 0 ) {
      std::cout << "Error Closing Dataset" << std::endl;
      retErr = err;
    }
  }
  return retErr;
}


/** 
 * @brief Reads a string dataset into the supplied string.
 * @param loc_id The parent group that holds the data object to read
 * @param dsetName The name of the dataset.
 * @param data The std::string to hold the data
 * @return Standard HDF error condition
 */
static herr_t readDataset(hid_t loc_id, std::string dsetName, std::string &data);

/**
 * @brief Reads an Attribute from an HDF5 Object.
 * 
 * Use this method if you already know the datatype of the attribute. If you do 
 * not know this already then use another form of this method.
 * 
 * @param loc_id The Parent object that holds the object to which you want to read an attribute
 * @param objName The name of the object to which the attribute is to be read
 * @param attrName The name of the Attribute to read
 * @param data The memory to store the data
 * @param dataType The H5T_Type of data
 * @return Standard HDF Error condition
 */
template <typename T>
static herr_t readAttribute(hid_t loc_id, std::string objName, std::string attrName,
                    std::vector<T> &data, hid_t dataType)
{
  /* identifiers */
  hid_t      obj_id;
  H5G_stat_t statbuf;
  herr_t err = 0;
  herr_t retErr = 0;
  hid_t attr_id;
  hid_t tid;
  //std::cout << "   Reading Vector Attribute at Path '" << objName << "' with Key: '" << attrName << "'" << std::endl;
  /* Get the type of object */
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf);
  if (err<0)
    return err;
  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type); 
  if ( obj_id >= 0)
  {
    attr_id = H5Aopen_name( obj_id, attrName.c_str() );
    if ( attr_id >= 0 ) 
    {
      //Need to allocate the array size
      H5T_class_t type_class;
      size_t type_size;
      std::vector<hsize_t> dims;
      H5Lite::getAttributeInfo(loc_id, objName, attrName, dims, type_class, type_size, tid);
      hsize_t numElements = 1;
      for (std::vector<hsize_t>::iterator iter = dims.begin(); iter < dims.end(); ++iter )
      {
        numElements *= *(iter);
      }
      //std::cout << "    Vector Attribute has " << numElements << " elements." << std::endl;
      data.resize( static_cast<signed int>(numElements) );
      err = H5Aread( attr_id, dataType, &(data.front()) );
      if ( err < 0 ) {
        std::cout << "Error Reading Attribute." << err << std::endl;
        retErr = err;
      }
      err = H5Aclose( attr_id );
      if ( err < 0 ) {
        std::cout << "Error Closing Attribute" << std::endl;
        retErr = err;
      }
    }
    err = H5Lite::closeId( obj_id, statbuf.type );
    if ( err < 0 ) {
     std::cout << "Error Closing Object" << std::endl;
     retErr = err; 
    }
  }
  return retErr;
}                    

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
static herr_t readAttribute(hid_t loc_id, 
                            std::string objName, 
                            std::string attrName,
                            T &data, 
                            hid_t dataType)
{

  /* identifiers */
  hid_t      obj_id;
  H5G_stat_t statbuf;
  herr_t err = 0;
  herr_t retErr = 0;
  hid_t attr_id;
 // std::cout << "Reading Scalar style Attribute at Path '" << objName << "' with Key: '" << attrName << "'" << std::endl;
  /* Get the type of object */
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf);
  if (err<0)
    return err;
  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type); 
  if ( obj_id >= 0)
  {
    attr_id = H5Aopen_name( obj_id, attrName.c_str() );
    if ( attr_id >= 0 ) 
    {
      err = H5Aread( attr_id, dataType, &data );
      if ( err < 0 ) {
        std::cout << "Error Reading Attribute." << std::endl;
        retErr = err;
      }
      err = H5Aclose( attr_id );
      if ( err < 0 ) {
        std::cout << "Error Closing Attribute" << std::endl;
        retErr = err;
      }
    }
    err = H5Lite::closeId( obj_id, statbuf.type );
    if ( err < 0 ) {
     std::cout << "Error Closing Object" << std::endl;
     retErr = err; 
    }
  }
  return retErr;
} 

/**
 * @brief Reads a string attribute from an HDF object
 * @param loc_id The Parent object that holds the object to which you want to read an attribute
 * @param objName The name of the object to which the attribute is to be read
 * @param attrName The name of the Attribute to read
 * @param data The memory to store the data
 * @return Standard HDF Error condition
 */
static herr_t readAttribute(hid_t loc_id, std::string objName, std::string attrName,
                              std::string &data);

/**
 * @brief
 */
static hid_t getAttributeNDims(hid_t loc_id, std::string objName, std::string attrName, hid_t &rank);

/**
 * @brief
 */
static hid_t getDatasetNDims(hid_t loc_id, std::string objName, hid_t &rank);

/**
 * @brief Get the information about a dataset.
 *
 * The dims array should already be allocated to hold all the dimensions which means prior to this
 * call you should have called a method to get the rank of the dataset.
 * @param loc_id The parent location of the Dataset 
 * @param dsetName The name of the dataset
 * @param dims A std::vector that will hold the sizes of the dimensions
 * @param type_class The HDF5 class type
 * @param type_size THe HDF5 size of the data
 * @return Negative value is Failure. Zero or Positive is success;
 */
static herr_t   getDatasetInfo( hid_t loc_id,
                              std::string dsetName,
                              std::vector<hsize_t> &dims,
                              H5T_class_t &type_class,
                              size_t &type_size );

/**
 * @brief
 */
static herr_t getAttributeInfo(hid_t loc_id, 
                               std::string obj_name,
                               std::string attr_name,
                               std::vector<hsize_t> &dims,
                               H5T_class_t &type_class,
                               size_t &type_size,
                               hid_t &attr_type);

// -----------------------------------------------------------------------------
protected:
  H5Lite();
  ~H5Lite();
};




#endif /* HDF5HIGHLEVEL_H_ */
