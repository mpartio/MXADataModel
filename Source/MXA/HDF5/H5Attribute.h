/*
 * H5Attribute.h
 *
 *  Created on: May 28, 2009
 *      Author: mjackson
 */

#ifndef H5ATTRIBUTE_H_
#define H5ATTRIBUTE_H_

#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/HDF5/H5Lite.h>



#define STATIC_NEW_METHOD(SuperClass, thisClass)\
static SuperClass::Pointer New( const std::string &key) {\
  SuperClass::Pointer ptr (new thisClass());\
  ptr->setKey(key);\
  return ptr;\
}\


/**
 * @def
 */
#define H5ATTRIBUTE_CONCRETE_DECLARATION(type, Suffix, SuperClass)\
  class Suffix##H5Attribute : public SuperClass \
  {\
    public:\
    MXA_SHARED_POINTERS(Suffix##H5Attribute)\
    MXA_TYPE_MACRO(Suffix##H5Attribute)\
    STATIC_NEW_METHOD(SuperClass, Suffix##H5Attribute)\
    MXA_STATIC_NEW_MACRO(Suffix##H5Attribute)\
    virtual ~Suffix##H5Attribute() { }\
    int writeHDF5Attribute(hid_t fileId, const std::string &datasetPath);\
    int readHDF5Attribute(hid_t fileId, const std::string &datasetPath);\
    void printValue(std::ostream &out);\
   protected:\
     Suffix##H5Attribute() {}\
   private:\
     Suffix##H5Attribute(const Suffix##H5Attribute&);\
     void operator=(const Suffix##H5Attribute&);\
  };

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class AbstractH5Attribute
{
  public:
    MXA_SHARED_POINTERS(AbstractH5Attribute)
    MXA_TYPE_MACRO(AbstractH5Attribute)

    virtual ~AbstractH5Attribute() {}

    MXA_INSTANCE_STRING_PROPERTY(Key);
    MXA_INSTANCE_PROPERTY(IMXAArray::Pointer, AttributeValue);

    virtual int writeHDF5Attribute(hid_t fileId, const std::string &datasetPath) = 0;
    virtual int readHDF5Attribute(hid_t fileId, const std::string &datasetPath) = 0;
    virtual void printValue(std::ostream &out) = 0;

  protected:
    AbstractH5Attribute() {}

  private:
    AbstractH5Attribute(const AbstractH5Attribute&);    // Copy Constructor Not Implemented
    void operator=(const AbstractH5Attribute&);  // Operator '=' Not Implemented
};


H5ATTRIBUTE_CONCRETE_DECLARATION(int8_t, Int8, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(uint8_t, UInt8, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(int16_t, Int16, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(uint16_t, UInt16, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(int32_t, Int32, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(uint32_t, UInt32, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(int64_t, Int64, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(uint64_t, UInt64, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(float, Float, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(double, Double, AbstractH5Attribute);
H5ATTRIBUTE_CONCRETE_DECLARATION(std::string, String, AbstractH5Attribute);


#define IF_ELSE_BODY(typeId, H5_TYPE, Prefix)\
  ( H5Tequal(typeId, H5_TYPE##BE) || H5Tequal(typeId,H5_TYPE##LE) ){\
   attrPtr = Prefix##H5Attribute::New(attributeKey);\
   attrPtr->readHDF5Attribute(loc_id, dsetName);\
  }\


/**
* @class H5Attribute H5Attribute.h MXA/HDF5/H5Attribute.h
* @brief Class that holds some static convenience methods
* @author Michael A. Jackson for BlueQuartz Software
* @date May 28, 2009
* @version 1.0
*/
class MXA_EXPORT H5Attribute
{
public:
/**
 * @brief Reads an attribute from a dataset or group from an HDF5 data file
 * @param loc_id The id of the root location which can be a File, Group or Dataset Id
 * @param dsetName The name of the data set
 * @param attributeKey The name of the attribute to read.
 * @return An AbstractH5Attribute shared pointer.
 */
static AbstractH5Attribute::Pointer ReadH5Attribute(hid_t loc_id,
                                                    const std::string &dsetName,
                                                    const std::string &attributeKey);

protected:
 H5Attribute() {}
 ~H5Attribute() {}

private:
  H5Attribute(const H5Attribute&);    // Copy Constructor Not Implemented
  void operator=(const H5Attribute&);  // Operator '=' Not Implemented

};



#endif /* H5ATTRIBUTE_H_ */
