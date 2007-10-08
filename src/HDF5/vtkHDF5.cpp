#include <vtkHDF5.h>
#include <HDF5/H5Utilities.h>
#include <Common/LogTime.h>


#define CHECK_ERR_WITH_RETURN(err, message)\
  if(err < 0) { std::cout << logTime() << message << std::endl; return err; }


vtkHDF5::vtkHDF5()
{
}

vtkHDF5::~vtkHDF5()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t vtkHDF5::getDataAsVTKImage(hid_t loc_id, const std::string &datasetPath, vtkImageData* imgData)
{
  herr_t err = 1;
  H5T_class_t classType;
  size_t typeSize;
  //hid_t typeId = -1;
  std::vector<uint64> dims; // The dimensions of the data set
  err = H5Lite::getDatasetInfo(loc_id, datasetPath, dims, classType, typeSize);
  if (err < 0)
  {
    std::cout << logTime() << "Error getting the dastaset info for " << datasetPath << std::endl;
    return -1;
  }
  //Holds dimensions for our vtkImage Object
  int32 xDim=1, yDim=1, zDim=1;

  //..Check for each dimension and assign its size to the variable
  if (dims.size() >= 1)
  {
    xDim = dims[0];
  }
  if (dims.size() >= 2)
  {
    yDim = dims[1];
  }
  if (dims.size() >= 3)
  {
    zDim = dims[2];
  }

  int32 scalarType = vtkHDF5::getScalarType(loc_id, datasetPath);
  if (scalarType < 0)
  {
    return -1;
  }

  imgData->SetScalarType(scalarType);
//TODO: Look for an Attribute that would tell us the spacing
  imgData->SetSpacing( 1.0, 1.0, 1.0);
//TODO: Look for an attribute that would tell us the origin
  imgData->SetOrigin( 0.0, 0.0, 0.0);
  imgData->SetDimensions(xDim, yDim, zDim );
  imgData->SetNumberOfScalarComponents(1);
 // imgData->PrepareForNewData();
  imgData->AllocateScalars();
  imgData->Update();

  err = vtkHDF5::_readVtkImageDataSet(loc_id, datasetPath, imgData );
  if (err < 0)
  {
    std::cout << logTime() << "Error: Could not get vtkDataset from HDF5 file."
        << std::endl;
    return -1;
  }
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int vtkHDF5::getScalarType(hid_t loc_id, const std::string &datasetPath)
{
  herr_t err = 1;
  int32 scalarType = -1;
  H5T_class_t classType;
  size_t typeSize;
  std::vector<uint64> dims; // The dimensions of the data set
  err = H5Lite::getDatasetInfo(loc_id, datasetPath, dims, classType, typeSize);
  if (err < 0)
  {
    std::cout << DEBUG_OUT(logTime) << "Error Retrieving Dataset Info at line " << __LINE__ << std::endl;
    return err;
  }
  hid_t typeId = H5Lite::getDatasetType(loc_id, datasetPath);
  if (typeId < 0) { 
    std::cout << DEBUG_OUT(logTime) << "Error getting type id for data set" << std::endl; 
    return -1; 
   }
  
  
  switch(classType)
   {
     case H5T_STRING:
       std::cout << "Error: Attempting to read string data into an image" << std::endl;
       err = -1;
       break;
     case H5T_INTEGER:
 
       if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) )
       {
         scalarType = (VTK_TYPE_UINT8);
       }
       else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) )
       {
         scalarType = (VTK_TYPE_UINT16);
       }
       else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) )
       {
         scalarType = (VTK_TYPE_UINT32);
       }
       else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) )
       {
         scalarType = (VTK_TYPE_UINT64);
       }
       else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) )
       {
         scalarType = VTK_TYPE_INT8;
       }
       else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) )
       {
         scalarType = (VTK_TYPE_INT16);
       }
       else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) )
       {
         scalarType = (VTK_TYPE_INT32);
       }
       else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) )
       {
         scalarType = (VTK_TYPE_INT64);
       }
       else
       {
         std::cout << "Unknown Type: " << typeId << std::endl;
         scalarType = -1;
       }
       break;
     case H5T_FLOAT:
       if (typeSize == 4)
       {
         scalarType = (VTK_TYPE_FLOAT32);
       }
       else if (typeSize == 8 )
       {
         scalarType = (VTK_TYPE_FLOAT64);
       }
       else
       {
         std::cout << "Unknown Floating point type" << std::endl;
         scalarType = -1;
       }
       break;
     default:
       std::cout << "Error: Unknown data type: " << classType << std::endl;
       H5Utilities::printHDFClassType(classType);
   }
  
  err = H5Tclose(typeId);
  if (err < 0) 
  {
    std::cout << logTime() << "Error Closing H5Type." << std::endl;
    scalarType = -1;
  }
  return scalarType;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
hid_t vtkHDF5::HDFTypeForVTKType(int value)
{
  if ( value == VTK_FLOAT) return H5T_NATIVE_FLOAT;
  if ( value == VTK_DOUBLE) return H5T_NATIVE_DOUBLE;
  
  if ( value == VTK_TYPE_INT8) return H5T_NATIVE_INT8;
  if ( value == VTK_TYPE_UINT8) return H5T_NATIVE_UINT8; 
  
  if ( value == VTK_TYPE_INT16) return H5T_NATIVE_INT16;
  if ( value == VTK_TYPE_UINT16) return H5T_NATIVE_UINT16; 
  
  if ( value == VTK_TYPE_INT32) return H5T_NATIVE_INT32;
  if ( value == VTK_TYPE_UINT32) return H5T_NATIVE_UINT32; 
  
  if ( value == VTK_TYPE_INT8) return H5T_NATIVE_INT64;
  if ( value == VTK_TYPE_UINT64) return H5T_NATIVE_UINT64; 
  
  std::cout << DEBUG_OUT(logTime) <<  "Error: HDFTypeForPrimitive - Unknown Type: " << value << " Consult vtkType.h for more information."<< std::endl;

  return -1;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t vtkHDF5::_readVtkImageDataSet(hid_t loc_id, const std::string &dsetName, vtkImageData* imgData)
{
  if ( NULL == imgData )
  {
    std::cout << DEBUG_OUT(logTime) << "Error: imgData pointer was NULL." << std::endl;
  }
  void* dataPtr = imgData->GetScalarPointer();  
  //unsigned char *data = static_cast < unsigned char* > ( imgData->GetScalarPointer( ) );
  hid_t   did;
  herr_t  err = 0;
  herr_t retErr = 0;
  hid_t spaceId;
  hid_t dataType;

  dataType = HDFTypeForVTKType( imgData->GetScalarType() );
  if (dataType == -1)
  {
    return -1;
  }

  did = H5Dopen( loc_id, dsetName.c_str() );
  if ( did < 0 ) {
    std::cout << " Error opening Dataset: " << did << std::endl;
    return -1;
  }
  if ( did >= 0 ) {
    spaceId = H5Dget_space(did);
    if ( spaceId > 0 ) {
      int32 rank = H5Sget_simple_extent_ndims(spaceId);
      if (rank > 0) {
        std::vector<hsize_t> dims;
        dims.resize(rank);// Allocate enough room for the dims
        rank = H5Sget_simple_extent_dims(spaceId, &(dims.front()), NULL);
        hsize_t numElements = 1;
        for (std::vector<hsize_t>::iterator iter = dims.begin(); iter < dims.end(); ++iter ) {
          numElements = numElements * (*iter); 
        }
        err = H5Dread(did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataPtr );
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
    else
    {
      retErr = spaceId;
    }
    err = H5Dclose( did );
    if (err < 0 ) {
      std::cout << "Error Closing Dataset" << std::endl;
      retErr = err;
    }
  }
  return retErr;
}

