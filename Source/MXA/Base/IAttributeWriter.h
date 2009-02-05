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
#ifndef _IATTRIBUTEWRITER_H_
#define _IATTRIBUTEWRITER_H_

#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypes.h>

#include <string>

/**
 * @brief Writes an IAttribute to the underlying data storage.
 * @author Mike Jackson
 * @date march 2007
 * @version $Revision: 1.2 $
 *  
 */
class MXA_EXPORT IAttributeWriter
{
public:
  
  virtual ~IAttributeWriter(){};

  /**
   * @brief Writes an attribute with a string value to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::string &value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int8 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint8 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */  
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int16 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint16 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int32 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint32 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int64 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint64 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */  
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, float32 value) = 0;
  /**
   * @brief Writes an attribute to the underlying data storage mechanism
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, float64 value) = 0;
  
  /**
   * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param dims The size of each dimension for this array based attribute
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int8> &value) = 0;
  
  /**
   * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param dims The size of each dimension for this array based attribute
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint8> & value) = 0;
  
   /**
    * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
    * @param locationId Unique location or file identifier
    * @param datasetPath The Path to the dataset
    * @param key The Attribute key
    * @param dims The size of each dimension for this array based attribute
    * @param value The value of the Attribute
    * @return Error condition. Negative=Error: Zero or Positive=Success
    */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int16> &value) = 0;
   
   /**
    * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
    * @param locationId Unique location or file identifier
    * @param datasetPath The Path to the dataset
    * @param key The Attribute key
    * @param dims The size of each dimension for this array based attribute
    * @param value The value of the Attribute
    * @return Error condition. Negative=Error: Zero or Positive=Success
    */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint16> & value) = 0;
  
   /**
    * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
    * @param locationId Unique location or file identifier
    * @param datasetPath The Path to the dataset
    * @param key The Attribute key
    * @param dims The size of each dimension for this array based attribute
    * @param value The value of the Attribute
    * @return Error condition. Negative=Error: Zero or Positive=Success
    */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int32> &value) = 0;
   
   /**
    * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
    * @param locationId Unique location or file identifier
    * @param datasetPath The Path to the dataset
    * @param key The Attribute key
    * @param dims The size of each dimension for this array based attribute
    * @param value The value of the Attribute
    * @return Error condition. Negative=Error: Zero or Positive=Success
    */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint32> & value) = 0;
  
   /**
    * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
    * @param locationId Unique location or file identifier
    * @param datasetPath The Path to the dataset
    * @param key The Attribute key
    * @param dims The size of each dimension for this array based attribute
    * @param value The value of the Attribute
    * @return Error condition. Negative=Error: Zero or Positive=Success
    */ 
   virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int64> &value) = 0;
  
  /**
   * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param dims The size of each dimension for this array based attribute
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint64> & value) = 0;
  
  /**
   * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param dims The size of each dimension for this array based attribute
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<float32> &value) = 0;
  
  /**
   * @brief Writes an attribute to the underlying data storage mechanism where the attribute value is really an array of values.
   * @param locationId Unique location or file identifier
   * @param datasetPath The Path to the dataset
   * @param key The Attribute key
   * @param dims The size of each dimension for this array based attribute
   * @param value The value of the Attribute
   * @return Error condition. Negative=Error: Zero or Positive=Success
   */ 
  virtual int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<float64> & value) = 0;
  
  
protected:
  IAttributeWriter(){};
  
};



#endif /* _IATTRIBUTEWRITER_H_ */

