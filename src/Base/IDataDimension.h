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
#ifndef IDATADIMENSION_H_
#define IDATADIMENSION_H_

#include "Common/DLLExport.h"
#include "Base/IDataDimensionWriter.h"


/**
 * @brief The formal interface description for a DataDimension
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT IDataDimension 
{
  public:
    IDataDimension() {};
    virtual ~IDataDimension() {};
    
    /**
     * @brief Setter for property Index
     * @param aValue The new value to set for property Index
     */
     virtual void setIndex(int32 aValue) = 0;

     /**
     * @brief Getter for property Index
     * @return The value of Index
     */
     virtual int32 getIndex() = 0;
     
     /**
     * @brief Setter for property Count
     * @param aValue The new value to set for property Count
     */
     virtual void setCount(int32 aValue) = 0;

     /**
     * @brief Getter for property Count
     * @return The value of Count
     */
     virtual int32 getCount() = 0;

     /**
     * @brief Setter for property startValue
     * @param aValue The new value to set for property startValue
     */
     virtual void setStartValue(int32 aValue) = 0;

     /**
     * @brief Getter for property startValue
     * @return The value of startValue
     */
     virtual int32 getStartValue() = 0;

     /**
     * @brief Setter for property increment
     * @param aValue The new value to set for property increment
     */
     virtual void setIncrement(int32 aValue) = 0;

     /**
     * @brief Getter for property increment
     * @return The value of increment
     */
     virtual int32 getIncrement() = 0;

     /**
     * @brief Setter for property endValue
     * @param aValue The new value to set for property endValue
     */
     virtual void setEndValue(int32 aValue) = 0;

     /**
     * @brief Getter for property endValue
     * @return The value of endValue
     */
     virtual int32 getEndValue() = 0;

     /**
     * @brief Setter for property name
     * @param aValue The new value to set for property name
     */
     virtual void setDimensionName(std::string aValue) = 0;

     /**
     * @brief Getter for property name
     * @return The value of name
     */
     virtual std::string getDimensionName() = 0;

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
     * @brief Setter for property uniform
     * @param aValue The new value to set for property uniform
     */
     virtual void setUniform(int32 aValue) = 0;

     /**
     * @brief Getter for property uniform
     * @return The value of uniform
     */
     virtual int32 getUniform() = 0;
     
     /**
      * @brief This method uses the given IDataDimensionWriter to serialize itself
      * to the underlying data storage system.
      * @param writer Valid (Non Null) IDataDimensioWriter object
      * @return Error condition.
      */
     virtual int32 writeDimension(IDataDimensionWriter* writer) = 0;

 
  private:
    IDataDimension(const IDataDimension&);    //Not Implemented
    void operator=(const IDataDimension&); //Not Implemented
};

#endif /*IDATADIMENSION_H_*/
