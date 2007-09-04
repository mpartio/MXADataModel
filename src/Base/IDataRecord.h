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
#ifndef IDATARECORD_H_
#define IDATARECORD_H_

#include "Common/DLLExport.h"

class IDataRecordWriter;

/**
 * @brief Abstract class defining the DataRecord public interface
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT IDataRecord
{
  public:
    IDataRecord() {};
    virtual ~IDataRecord() {};
    
    /**
     * @brief Setter for property name
     * @param aValue The new value to set for property name
     */
     virtual void setRecordName(std::string aValue) = 0;

     /**
     * @brief Getter for property name
     * @return The value of name
     */
     virtual std::string getRecordName() = 0;

     /**
     * @brief Setter for property altName
     * @param aValue The new value to set for property altName
     */
     virtual void setAltName(std::string aValue) = 0;

     /**
     * @brief Getter for property altName
     * @return The value of altName
     */
     virtual std::string getAltName() = 0;
     
     
     /**
     * @brief Setter for property luid
     * @param aValue The new value to set for property luid
     */
     virtual void setLuid(int32 aValue) = 0;

     /**
     * @brief Getter for property luid
     * @return The value of luid
     */
     virtual int32 getLuid() = 0;
     
     /**
     * @brief Setter for property guid
     * @param aValue The new value to set for property guid
     */
     virtual void setGuid(int32 aValue) = 0;

     /**
     * @brief Getter for property guid
     * @return The value of guid
     */
     virtual int32 getGuid() = 0;
     
    
    // Over ride from Superclass 
     virtual void setNodeName(std::string nodeName) = 0;
     virtual void printNode(std::ostream& os, int32 indentSize=0) = 0;
          
     virtual std::string generatePath()  = 0;
     virtual std::string generateParentPath()  = 0;
     
     virtual int32 writeRecord(IDataRecordWriter* writer) = 0;
    
     /**
      * @brief Checks some basic properties of the model to make sure they are
      * set correctly.
      * @param message String to hold messages concerning errors/omissions from
      * the model
      * @return True if the model passes the basic checks
      */
     virtual bool isValid(std::string &message) = 0;
     
  protected:
    
    
  private:
    IDataRecord(const IDataRecord&);    //Not Implemented
    void operator=(const IDataRecord&); //Not Implemented
};

#endif /*IDATARECORD_H_*/
