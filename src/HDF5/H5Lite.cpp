#include "H5Lite.h"

#define CloseH5A(aid, err, retError)\
   err = H5Aclose( attr_id );\
   if (err<0) {std::cout << "Error Closing Attribute." << std::endl;retErr = err;}

#define CloseH5D(did, err, retError)\
  err = H5Dclose(did);\
  if (err < 0) { std::cout << "Error Closing Dataset." << std::endl; retError = err;}

#define CloseH5S(sid, err, retError)\
  err = H5Sclose(sid); \
  if ( err < 0) {std::cout << "Error closing Dataspace." << std::endl;retErr = err;}

#define CloseH5T(tid, err, retError)\
  err = H5Tclose(tid);\
  if (err < 0 ) {std::cout << "Error closing DataType" << std::endl; retErr = err;}

#define HDF_ERROR_HANDLER_OFF\
  herr_t (*_oldHDF_error_func)(void *);\
  void *_oldHDF_error_client_data;\
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);\
  H5Eset_auto(NULL, NULL);

#define HDF_ERROR_HANDLER_ON  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);


/*-------------------------------------------------------------------------
 * Function: find_dataset
 *
 * Purpose: operator function used by H5LTfind_dataset
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: June 21, 2001
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t find_dataset( hid_t loc_id, const char *name, void *op_data)
{

 /* Define a default zero value for return. This will cause the iterator to continue if
  * the dataset is not found yet.
  */

 int32 ret = 0;

 char *dset_name = (char*)op_data;

 /* Shut the compiler up */
 loc_id=loc_id;

 /* Define a positive value for return value if the dataset was found. This will
  * cause the iterator to immediately return that positive value,
  * indicating short-circuit success
  */

 if( strcmp( name, dset_name ) == 0 )
  ret = 1;


 return ret;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static herr_t find_attr( hid_t loc_id, const char *name, void *op_data)
{

 /* Define a default zero value for return. This will cause the iterator to continue if
  * the palette attribute is not found yet.
  */

 int32 ret = 0;

 char *attr_name = (char*)op_data;

 /* Shut the compiler up */
 loc_id=loc_id;

 /* Define a positive value for return value if the attribute was found. This will
  * cause the iterator to immediately return that positive value,
  * indicating short-circuit success
  */

 if( strcmp( name, attr_name ) == 0 )
  ret = 1;


 return ret;
}


// -----------------------------------------------------------------------------
//  Protected Constructor
// -----------------------------------------------------------------------------
H5Lite::H5Lite()
{
}

// -----------------------------------------------------------------------------
//  Protected Destructor
// -----------------------------------------------------------------------------
H5Lite::~H5Lite()
{
}

// -----------------------------------------------------------------------------
//  Opens an ID for HDF5 operations
// -----------------------------------------------------------------------------
herr_t H5Lite::openId( hid_t loc_id, const std::string& obj_name, int32 obj_type)
{

 hid_t   obj_id = -1;

 switch ( obj_type )
 {
  case H5G_DATASET:

   /* Open the dataset. */
   if ( (obj_id = H5Dopen( loc_id, obj_name.c_str() )) < 0 )
    return -1;
   break;

  case H5G_GROUP:

   /* Open the group. */
   if ( (obj_id = H5Gopen( loc_id, obj_name.c_str() )) < 0 )
    return -1;
   break;

  default:
   return -1;
 }

 return obj_id;

}

// -----------------------------------------------------------------------------
//  Closes the given ID
// -----------------------------------------------------------------------------
herr_t H5Lite::closeId( hid_t obj_id, int32 obj_type )
{
 switch ( obj_type )
 {
  case H5G_DATASET:
   /* Close the dataset. */
   if ( H5Dclose( obj_id ) < 0 )
    return -1;
   break;

  case H5G_GROUP:
  /* Close the group. */
   if ( H5Gclose( obj_id ) < 0 )
    return -1;
   break;

  default:
   return -1;
 }

 return 1;
}

// -----------------------------------------------------------------------------
//  Finds an Attribute given an object to look in
// -----------------------------------------------------------------------------
herr_t H5Lite::findAttribute( hid_t loc_id, const std::string& attrName )
{

 uint32 attr_num;
 herr_t       ret;

 attr_num = 0;
 ret = H5Aiterate( loc_id, &attr_num, find_attr, (void *)(attrName.c_str()) );

 return ret;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5Lite::findDataset( hid_t loc_id, const std::string& dsetName )
{

 herr_t  ret;

 ret = H5Giterate( loc_id, ".", 0, find_dataset, (void*)(dsetName.c_str() ) );

 return ret;
}



// -----------------------------------------------------------------------------
//  Writes a string to a HDF5 dataset
// -----------------------------------------------------------------------------
herr_t H5Lite::writeStringDataset (hid_t loc_id, const std::string& dsetName, const std::string &data)
{
  hid_t   did=-1;
  hid_t   sid=-1;
  hid_t   tid;
  size_t  size;
  herr_t err = -1;
  herr_t retErr = 0;

  /* create a string data type */
  if ( (tid = H5Tcopy( H5T_C_S1 )) >= 0 )
  {
    size = data.size() + 1;
    if ( H5Tset_size(tid, size) >= 0 )
    {
      if ( H5Tset_strpad(tid, H5T_STR_NULLTERM ) >= 0 )
      {
      /* Create the data space for the dataset. */
        if ( (sid = H5Screate( H5S_SCALAR )) >= 0 )
        {
          /* Create the dataset. */
          if ( (did = H5Dcreate(loc_id, dsetName.c_str(), tid, sid, H5P_DEFAULT)) >= 0 )
          {
             if ( !data.empty() )
              {
                  err = H5Dwrite(did, tid, H5S_ALL, H5S_ALL,H5P_DEFAULT, data.c_str() );
                  if (err<0 ) {
                    std::cout << "Error Writing String Data" << std::endl;
                    retErr = err;
                  }
              }
          } else {
            retErr = did;
          }
          CloseH5D(did, err, retErr);
//          err = H5Dclose(did);
//          if (err < 0) {
//            std::cout << "Error Closing Dataset." << std::endl;
//            retErr = err;
//          }
        }
        CloseH5S(sid, err, retErr);
//        err = H5Sclose(sid);
//        if ( err < 0) {
//          std::cout << "Error closing Dataspace." << std::endl;
//          retErr = err;
//        }
      }
    }
    CloseH5T(tid, err, retErr);
//    err = H5Tclose(tid);
//    if (err < 0 ) {
//     std::cout << "Error closing DataType" << std::endl;
//     retErr = err;
//    }
  }
  return retErr;
}



// -----------------------------------------------------------------------------
//  Writes a string to an HDF5 Attribute
// -----------------------------------------------------------------------------
herr_t H5Lite::writeStringAttribute(hid_t loc_id, 
                                 const std::string& objName, 
                                 const std::string& attrName, 
                                 const std::string& data )
{
  hid_t      attr_type;
  hid_t      attr_space_id;
  hid_t      attr_id;
  hid_t      obj_id;
  int32        has_attr;
  H5G_stat_t statbuf;
  size_t     attr_size;
  herr_t err = 0;
  herr_t retErr = 0;

  /* Get the type of object */
  retErr = H5Gget_objinfo( loc_id, objName.c_str(), 1, &statbuf );
  if (retErr >= 0) {
    /* Open the object */
    obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
    if ( obj_id >= 0) {
      /* Create the attribute */
      attr_type = H5Tcopy( H5T_C_S1 );
      if ( attr_type >= 0 ) {
        attr_size = data.length() + 1; /* extra null term */
        err = H5Tset_size( attr_type, (size_t)attr_size);
        if (err<0) {
          std::cout << "Error Setting H5T Size" << std::endl;
          retErr = err; 
        }
        if ( err >= 0 ) {
          err = H5Tset_strpad( attr_type, H5T_STR_NULLTERM );
          if (err<0) {
            std::cout << "Error adding a null terminator." << std::endl;
            retErr = err; 
          }
          if ( err >= 0 )  {
            attr_space_id = H5Screate( H5S_SCALAR );
            if ( attr_space_id >= 0 ) {
              /* Verify if the attribute already exists */
              has_attr = H5Lite::findAttribute( obj_id, attrName );
              /* The attribute already exists, delete it */
              if ( has_attr == 1 )
              {
                err = H5Adelete( obj_id, attrName.c_str() );
                if (err<0) {
                  std::cout << "Error Deleting Attribute." << std::endl;
                  retErr = err;
                }
              }
              if (err >= 0) {
                /* Create and write the attribute */
                attr_id = H5Acreate( obj_id, attrName.c_str(), attr_type, attr_space_id, H5P_DEFAULT );
                if ( attr_id >= 0 ) {
                  err = H5Awrite( attr_id, attr_type, data.data() );
                  if ( err < 0 ) {
                    std::cout << "Error Writing String attribute." << std::endl;\
                    retErr = err;
                  }
                }
                CloseH5A(attr_id, err, retErr);
              }
              CloseH5S(attr_space_id, err, retErr);
            }
          }
        }
        CloseH5T(attr_type, err, retErr);
      }
      /* Close the object */
      err = H5Lite::closeId( obj_id, statbuf.type );
      if (err < 0) {
        std::cout << "Error Closing Object Id" << std::endl;
        retErr = err;
      }
    }
  }
  return retErr;
}

// -----------------------------------------------------------------------------
//  Reads a String dataset
// -----------------------------------------------------------------------------
herr_t H5Lite::readStringDataset(hid_t loc_id, const std::string& dsetName, std::string &data) {
  hid_t did; // dataset id
  hid_t tid; //type id
  herr_t err;
  herr_t retErr = 0;
  hsize_t size;
  
  did = H5Dopen(loc_id, dsetName.c_str() );
  if (did < 0) {
    std::cout << "Error Opening Dataset" << std::endl;
    return -1; 
  }
  tid = H5Dget_type(did);
  if ( tid >= 0 ) {
    
    size = H5Dget_storage_size(did); 
    std::vector<char> buf(static_cast<int>(size+1), 0x00); //Allocate and Zero and array
    err = H5Dread(did, tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(buf.front()) );
    if (err<0) {
      std::cout << "Error Reading string dataset." << std::endl;
      retErr = err;
    } else {
      data.append( &(buf.front()) ); //Append the string to the given string
    }
  }
  CloseH5D(did, err, retErr);
  CloseH5T(tid, err, retErr);
  return retErr;
}

// -----------------------------------------------------------------------------
//  Reads a string Attribute from the HDF file
// -----------------------------------------------------------------------------
herr_t H5Lite::readStringAttribute(hid_t loc_id, const std::string& objName, const std::string& attrName,
                              std::string &data)
{
  
 /* identifiers */
 hid_t      obj_id;
 H5G_stat_t statbuf;
 hid_t      attr_id;
 hid_t      attr_type;
 std::vector<char> attr_out;
 hsize_t    size;
 herr_t err = 0;
 herr_t retErr = 0;
 
 HDF_ERROR_HANDLER_OFF;
 
  /* Get the type of object */
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf);
  if (err<0) {
    return err;
  }

  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
  if ( obj_id >= 0)
  {
    attr_id = H5Aopen_name( obj_id, attrName.c_str() );
    if ( attr_id >= 0 )
    {
      size = H5Aget_storage_size(attr_id);
      attr_out.resize( static_cast<int>(size) );  //Resize the vector to the proper length
      attr_type = H5Aget_type( attr_id );
      if ( attr_type >= 0 )
      {
        err = H5Aread( attr_id, attr_type, &(attr_out.front()) );
        if (err < 0) {
          std::cout << "Error Reading Attribute." << std::endl;
          retErr = err;
        } else {
          data.append( &(attr_out.front()) ); //Append the data to the passed in string
        }
        CloseH5T(attr_type, err, retErr);
      }  
      CloseH5A(attr_id, err, retErr);      
    }
    err = H5Lite::closeId( obj_id, statbuf.type );
    if (err<0) {
      std::cout << "Error Closing Object ID" << std::endl;
      retErr = err;
    }
   
 }
  HDF_ERROR_HANDLER_ON;
 return retErr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5Lite::getDatasetNDims( hid_t loc_id, const std::string& dsetName, hid_t &rank)
{
 hid_t       did;
 hid_t       sid;
 herr_t err = 0;
 herr_t retErr = 0;
 rank = 0;
 
 /* Open the dataset. */
 if ( (did = H5Dopen( loc_id, dsetName.c_str() )) < 0 )
  return -1;

 /* Get the dataspace handle */
 sid = H5Dget_space( did );
 if ( sid >= 0 )
 {

   /* Get rank */
  rank = H5Sget_simple_extent_ndims( sid );
  if ( rank < 0 ) {
    retErr = rank;
    rank = 0;
    std::cout << "Error Getting the rank of the dataset:" << std::endl;
  }
  
   /* Terminate access to the dataspace */
   CloseH5S(sid, err, retErr);
 }

 /* End access to the dataset */
 err = H5Dclose( did );
 if ( err < 0 )
  {
    retErr = err;
    rank = 0; 
  }
  return retErr;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5Lite::getAttributeNDims(hid_t loc_id, const std::string& objName, 
                                const std::string& attrName, hid_t &rank)
{
   /* identifiers */
   hid_t      obj_id;
   H5G_stat_t statbuf;
   hid_t      attr_id;
   herr_t err = 0;
   herr_t retErr = 0;
   hid_t sid;
   rank = -1;
    /* Get the type of object */
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf);
  if (err<0) {
    return err;
  }
  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
  if ( obj_id >= 0)
  {
    attr_id = H5Aopen_name( obj_id, attrName.c_str() );
    if ( attr_id >= 0 )
    {
      sid = H5Aget_space( attr_id );
      if ( sid >= 0 )
      {
        rank = H5Sget_simple_extent_ndims( sid );
        CloseH5T(sid, err, retErr);
      }  
      CloseH5A(attr_id, err, retErr);      
    }
    err = H5Lite::closeId( obj_id, statbuf.type );
    if (err<0) {
      std::cout << "Error Closing Object ID" << std::endl;
      retErr = err;
    }
 }
 
 return retErr;
}

// -----------------------------------------------------------------------------
//  Get the dataset information
// -----------------------------------------------------------------------------
herr_t H5Lite::getDatasetInfo( hid_t loc_id,
                             const std::string& dsetName,
                             std::vector<uint64> &dims,
                             H5T_class_t &classType,
                             size_t &sizeType )
{
  hid_t     did;
  hid_t     tid;
  hid_t     sid;
  herr_t    err = 0;
  herr_t    retErr = 0;
  hid_t     rank = 0;


  /* Open the dataset. */
  if ( (did = H5Dopen( loc_id, dsetName.c_str() )) < 0 )
  return -1;

  /* Get an identifier for the datatype. */
  tid = H5Dget_type( did );
  if ( tid >= 0) 
  {
    /* Get the class. */
    classType = H5Tget_class( tid );
    /* Get the size. */
    sizeType = H5Tget_size( tid );
    /* Release the datatype. */
    err = H5Tclose( tid );
    if ( err < 0) {
      std::cout << "Error Closing H5Type" << std::endl;
      retErr = err;
    }
  }
  /* Get the dataspace handle */
  sid = H5Dget_space( did );
  if ( sid >= 0 )
  {
    /* Get the Number of Dimensions */
    rank = H5Sget_simple_extent_ndims(sid);
    if ( rank > 0) {
      hsize_t _dims[rank]; // allocate space for the dimensions
      /* Get dimensions */
      err = H5Sget_simple_extent_dims( sid, _dims, NULL);
      if ( err < 0 ) {
        std::cout << "Error Getting Simple Extents for dataset" << std::endl;
        retErr = err;
      }
      //Copy the dimensions into the dims vector
        dims.clear(); //Erase everything in the Vector
        dims.resize(rank);
        for (hid_t i = 0; i < rank; ++i) 
        {
        	dims[i] = static_cast<uint64>(_dims[i]);
        }
      
    }
    /* Terminate access to the dataspace */
    CloseH5S(sid, err, retErr);
  }

  /* End access to the dataset */
  CloseH5D(did, err, retErr);
  return retErr;  
}

// -----------------------------------------------------------------------------
//  You must close the attributeType argument or resource leaks will occur. Use
//  H5Tclose(tid); after your call to this method if you do not need the id for
//   anything.
// -----------------------------------------------------------------------------
herr_t H5Lite::getAttributeInfo(hid_t loc_id, 
                                const std::string& objName,
                                const std::string& attrName,
                                std::vector<uint64> &dims,
                                H5T_class_t &type_class,
                                size_t &type_size,
                                hid_t &tid)
{
   /* identifiers */
   hid_t      obj_id;
   H5G_stat_t statbuf;
   hid_t      attr_id;
 //  hid_t      tid;
   herr_t err = 0;
   herr_t retErr = 0;
   hid_t sid;
   hid_t rank = -1;
   
  err = H5Gget_objinfo(loc_id, objName.c_str(), 1, &statbuf);
  if (err<0) {
    return err;
  }

  /* Open the object */
  obj_id = H5Lite::openId( loc_id, objName, statbuf.type );
  if ( obj_id >= 0)
  {
    attr_id = H5Aopen_name( obj_id, attrName.c_str() );
    if ( attr_id >= 0 )
    {
      /* Get an identifier for the datatype. */
      tid = H5Aget_type( attr_id );
      if (tid>0)
      { 
        /* Get the class. */
        type_class = H5Tget_class( tid );
        /* Get the size. */
        type_size = H5Tget_size( tid );
        sid = H5Aget_space(attr_id);
        if (sid >= 0 ) 
        {
          rank = H5Sget_simple_extent_ndims( sid );
		      hsize_t _dims[rank];
          err = H5Sget_simple_extent_dims(sid, _dims, NULL );
          if (err<0) {
            std::cout << "Error Getting Attribute dims" << std::endl;
            retErr = err; 
          }
          //Copy the dimensions into the dims vector
    		  dims.clear(); //Erase everything in the Vector
    		  dims.resize(rank);
    		  for (hid_t i = 0; i < rank; ++i) 
    		  {
    			dims[i] = static_cast<uint64>(_dims[i]);
    		  }
          CloseH5S(sid, err, retErr);
        }
        //CloseH5T(tid, err, retErr); We are passing back the tid so don't close it
      }
      CloseH5A(attr_id, err, retErr);
    }
    err = H5Lite::closeId( obj_id, statbuf.type );
    if (err<0) {
      std::cout << "Error Closing Object ID" << std::endl;
      retErr = err;
    }
   
 }
 return retErr;
}

