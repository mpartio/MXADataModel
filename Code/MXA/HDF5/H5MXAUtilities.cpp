/*
 * H5MXAUtilities.cpp
 *
 *  Created on: Jul 21, 2008
 *      Author: mjackson
 */

#include "H5MXAUtilities.h"
//-- MXA Includes
#include <MXA/Common/LogTime.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IDataRecord.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/Utilities/StringUtils.h>

//-- hdf5 includes
#include <hdf5.h>

#define CheckValidLocId(locId)\
  if (locId < 0 ) {std::cout << "Invalid HDF Location ID: " << locId << std::endl;return -1;}


// -----------------------------------------------------------------------------
 //  Use a new set of indices to generate a path
 // -----------------------------------------------------------------------------
 std::string H5MXAUtilities::generateH5PathToDataset ( IDataModel::Pointer model,
                                                   std::vector<int32_t> &indices,
                                                   IDataRecord::Pointer record)
 {
   std::string path;
   //Put the data root on first
   path += model->getDataRoot();
   //Put the Data Dimensions on Next
   for (std::vector<int32_t>::iterator iter = indices.begin(); iter != indices.end(); ++iter ) {
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
 IMXAArray::Pointer H5MXAUtilities::readData(IDataFile::Pointer dataFile, const std::string &datasetPath)
 {
   IMXAArray::Pointer ptr;
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

   typeId = H5Lite::getDatasetType(fileId, datasetPath);
   if (typeId < 0)
   {
     return ptr;
   }

   switch(attr_type)
   {
   case H5T_STRING:
     res.clear(); //Clear the string out first
     ptr = MXAAsciiStringData::Create( res);
     break;
   case H5T_INTEGER:
     //std::cout << "User Meta Data Type is Integer" << std::endl;
     if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
        ptr = H5Utilities::readH5Data<uint8_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
        ptr = H5Utilities::readH5Data<uint16_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
        ptr = H5Utilities::readH5Data<uint32_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
        ptr = H5Utilities::readH5Data<uint64_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
        ptr = H5Utilities::readH5Data<int8_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
        ptr = H5Utilities::readH5Data<int16_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
        ptr = H5Utilities::readH5Data<int32_t>(dataFile->getFileId(), datasetPath,  dims);
      } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
        ptr = H5Utilities::readH5Data<int64_t>(dataFile->getFileId(), datasetPath,  dims);
     } else {
       std::cout << "Unknown Type: " << typeId << " at " <<  datasetPath << std::endl;
       err = -1;
       retErr = -1;
     }
     break;
   case H5T_FLOAT:
     if (attr_size == 4) {
       ptr = H5Utilities::readH5Data<float32>(dataFile->getFileId(), datasetPath,  dims);
     } else if (attr_size == 8 ) {
       ptr = H5Utilities::readH5Data<float64>(dataFile->getFileId(), datasetPath,  dims);
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

   return ptr;
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 IMXAArray::Pointer H5MXAUtilities::readAttribute(IDataFile::Pointer dataFile,
                                                         const std::string &datasetPath,
                                                         const std::string &attributeKey)
 {
   IMXAArray::Pointer ptr;
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
   err = H5Lite::getAttributeInfo(fileId, datasetPath, attributeKey, dims, attr_type, attr_size, typeId);
   if (err < 0 )
   {
     return ptr;
   }
   size_t numElements = 1;
   for (std::vector<uint64_t>::size_type i = 0; i < dims.size(); ++i)
   {
     numElements = numElements * dims[i];
   }
   switch(attr_type)
   {
   case H5T_STRING:
     res.clear(); //Clear the string out first
     ptr = MXAAsciiStringData::Create( res);
     break;
   case H5T_INTEGER:
     //std::cout << "User Meta Data Type is Integer" << std::endl;
     if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
        ptr = H5Utilities::readH5Attribute<uint8_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
        ptr = H5Utilities::readH5Attribute<uint16_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
        ptr = H5Utilities::readH5Attribute<uint32_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
        ptr = H5Utilities::readH5Attribute<uint64_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
        ptr = H5Utilities::readH5Attribute<int8_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
        ptr = H5Utilities::readH5Attribute<int16_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
        ptr = H5Utilities::readH5Attribute<int32_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
      } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
        ptr = H5Utilities::readH5Attribute<int64_t>(dataFile->getFileId(), datasetPath, attributeKey, dims);
     } else {
       std::cout << DEBUG_OUT(logTime) << "Unknown Integer Type: " << typeId << " for attribute:'" << attributeKey << "' for dataset:'" <<  datasetPath <<"'" << std::endl;
       err = -1;
       retErr = -1;
     }
     break;
   case H5T_FLOAT:
     if (attr_size == 4) {
       ptr = H5Utilities::readH5Attribute<float32>(dataFile->getFileId(), datasetPath, attributeKey, dims);
     } else if (attr_size == 8 ) {
       ptr = H5Utilities::readH5Attribute<float64>(dataFile->getFileId(), datasetPath, attributeKey, dims);
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

   return ptr;
 }
