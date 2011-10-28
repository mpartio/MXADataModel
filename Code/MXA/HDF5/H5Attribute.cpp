/*
 * H5Attribute.cpp
 *
 *  Created on: May 28, 2009
 *      Author: mjackson
 */

#include "H5Attribute.h"




#define H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(type, Suffix)\
int32_t Suffix##H5Attribute::writeHDF5Attribute(hid_t fileId, const std::string &datasetPath)\
{\
  herr_t err = 0;\
  err = H5Lite::writeMXAAttribute(fileId, datasetPath, getKey(), getAttributeValue().get() );\
  if (err < 0) {\
    std::cout << logTime() << "Error writing attribute " << getKey() \
    <<   " to data set " << datasetPath << std::endl;\
  }\
  return err;\
}\
\
int32_t Suffix##H5Attribute::readHDF5Attribute(hid_t fileId, const std::string &datasetPath)\
{\
  herr_t err = -1;\
  IMXAArray* attrValue = H5Lite::readMXAAttribute(fileId, datasetPath, getKey() );\
  if (NULL != attrValue) {\
    IMXAArray::Pointer ptr (attrValue);\
    setAttributeValue(ptr);\
    err = 0;\
  }\
  else {\
    std::cout << logTime() << "Error Reading attribute " << getKey() << " from dataset " << datasetPath\
    << std::endl;\
  }\
  return err;\
}\
  \
void Suffix##H5Attribute::printValue(std::ostream &out)\
{\
  getAttributeValue()->printSelf(out, 1);\
}\




H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(int8_t, Int8);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(uint8_t, UInt8);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(int16_t, Int16);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(uint16_t, UInt16);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(int32_t, Int32);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(uint32_t, UInt32);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(int64_t, Int64);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(uint64_t, UInt64);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(float, Float);
H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(double, Double);

H5ATTRIBUTE_CONCRETE_IMPLEMENTATION(std::string, String);


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractH5Attribute::Pointer H5Attribute::ReadH5Attribute(hid_t loc_id,
                                                    const std::string &dsetName,
                                                    const std::string &attributeKey)
{
  /* identifiers */
   hid_t      obj_id;
   H5O_info_t statbuf;
   herr_t err = 0;
   herr_t retErr = 0;
   hid_t attr_id;
   std::string sData;
   AbstractH5Attribute::Pointer attrPtr = AbstractH5Attribute::NullPointer();
   /* Get the type of object */
   err = H5Oget_info_by_name(loc_id, dsetName.c_str(),  &statbuf, H5P_DEFAULT);
   if (err<0)
     return attrPtr;
   /* Open the object */
   obj_id = H5Lite::openId( loc_id, dsetName, statbuf.type);

   if ( obj_id >= 0)
   {
     attr_id = H5Aopen_by_name( obj_id, dsetName.c_str(), attributeKey.c_str(), H5P_DEFAULT, H5P_DEFAULT );

     if ( attr_id >= 0 )
     {
       //Need to allocate the array size
       hid_t typeId = -1;
       H5T_class_t type_class;
       size_t attr_size;
       std::vector<hsize_t> dims;
       err = H5Lite::getAttributeInfo(loc_id, dsetName, attributeKey, dims, type_class, attr_size, typeId);
       if (err < 0){  }
       std::vector<uint64_t> _dims(dims.size(), 0);
       for (unsigned int i = 0; i<dims.size(); ++i)
       {
         _dims[i] = static_cast<uint64_t>(dims[i]);
       }
       switch(type_class)
       {
         case H5T_STRING:
           attrPtr = StringH5Attribute::New(attributeKey);
           attrPtr->readHDF5Attribute(loc_id, dsetName);
           break;
         case H5T_INTEGER:
            if IF_ELSE_BODY(typeId,H5T_STD_U8, UInt8)
            else if IF_ELSE_BODY(typeId, H5T_STD_U16, UInt16)
            else if IF_ELSE_BODY(typeId, H5T_STD_U32, UInt32)
            else if IF_ELSE_BODY(typeId, H5T_STD_U64, UInt64)
            else if IF_ELSE_BODY(typeId, H5T_STD_I8, Int8)
            else if IF_ELSE_BODY(typeId, H5T_STD_I16, Int16)
            else if IF_ELSE_BODY(typeId, H5T_STD_I32, Int32)
            else if IF_ELSE_BODY(typeId, H5T_STD_I64, Int64)
            else {
             std::cout << "Unknown Type: " << typeId << " at " <<  dsetName << __FILE__ << "(" << __LINE__ << ")"  << std::endl;
             err = -1;
             retErr = -1;
           }
           break;
         case H5T_FLOAT:
           if (attr_size == 4) {
             attrPtr = FloatH5Attribute::New(attributeKey);\
             attrPtr->readHDF5Attribute(loc_id, dsetName);
           } else if (attr_size == 8 ) {
             attrPtr = DoubleH5Attribute::New(attributeKey);\
             attrPtr->readHDF5Attribute(loc_id, dsetName);
           } else {
             std::cout << "Unknown Floating point type" << __FILE__ << "(" << __LINE__ << ")"  << std::endl;
             err = -1;
             retErr = -1;
           }
           break;
         default:
           std::cout << "Error:" << __FILE__ << "(" << __LINE__ << ")"  << std::endl;

       }
       CloseH5T(typeId, err, retErr);
       err = H5Aclose( attr_id );
       if ( err < 0 ) {
         std::cout << "Error Closing Attribute" << std::endl;
         retErr = err;
       }
     }
     else
     {
       retErr = attr_id;
     }
     err = H5Lite::closeId( obj_id, statbuf.type );
     if ( err < 0 ) {
      std::cout << "Error Closing Object" << std::endl;
      retErr = err;
     }
   }
   return attrPtr;
}
