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
#ifndef _MXADATAMODELNODE_H_
#define _MXADATAMODELNODE_H_

#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Base/IDataDimension.h"
#include "Core/MXANode.h"
#include "Core/MXAConstants.h"
#include "Utilities/StringUtils.h"

#include "boost/shared_ptr.hpp"

#include <limits>


/**
 * @brief A concrete implementation of the IDataDimension Abstract Class.
 * 
 * @author Mike Jackson @ IMTS.us 
 * @date March 2007
 * @version 1.0
 *  
 */
class  MXA_EXPORT MXADataDimension : public IDataDimension, public MXANode
{

public:
  
  
  
  /**
  * @brief Used to create a new MXANode
  * @param type The Type of Node this is going to be
  * @param nodeName The Name of this node
  * @return A Boost SharedPointer to the MXANode Object
  */
  static MXADataDimensionPtr New(std::string name, std::string altName, 
        int32 index = std::numeric_limits<int32>::min(), 
        int32 count = std::numeric_limits<int32>::min(),
        int32 startValue  = std::numeric_limits<int32>::max(),
        int32 endValue = std::numeric_limits<int32>::max(),
        int32 increment = std::numeric_limits<int32>::max(),
        int32 uniform = std::numeric_limits<int32>::min()
  );

//  #warning Add another static constructor that takes no args.
  
  virtual ~MXADataDimension();

  /**
  * @brief Setter for property Index
  * @param aValue The new value to set for property Index
  */
  void setIndex(int32 aValue);

  /**
  * @brief Getter for property Index
  * @return The value of Index
  */
  int32 getIndex();
  
  /**
  * @brief Setter for property Count
  * @param aValue The new value to set for property Count
  */
  void setCount(int32 aValue) { _count = aValue; }

  /**
  * @brief Getter for property Count
  * @return The value of Count
  */
  int32 getCount() { return _count; }

  /**
  * @brief Setter for property startValue
  * @param aValue The new value to set for property startValue
  */
  void setStartValue(int32 aValue) { _startValue = aValue; }

  /**
  * @brief Getter for property startValue
  * @return The value of startValue
  */
  int32 getStartValue() { return _startValue; }

  /**
  * @brief Setter for property increment
  * @param aValue The new value to set for property increment
  */
  void setIncrement(int32 aValue) { _increment = aValue; }

  /**
  * @brief Getter for property increment
  * @return The value of increment
  */
  int32 getIncrement() { return _increment; }

  /**
  * @brief Setter for property endValue
  * @param aValue The new value to set for property endValue
  */
  void setEndValue(int32 aValue) { _endValue = aValue; }

  /**
  * @brief Getter for property endValue
  * @return The value of endValue
  */
  int32 getEndValue() { return _endValue; }

  /**
  * @brief Setter for property name
  * @param aValue The new value to set for property name
  */
  void setDimensionName(std::string aValue) { _dimensionName = aValue; }

  /**
  * @brief Getter for property name
  * @return The value of name
  */
  std::string getDimensionName() { return _dimensionName; }

  /**
  * @brief Setter for property altName
  * @param aValue The new value to set for property altName
  */
  void setAltName(std::string aValue) { _altName = aValue; }

  /**
  * @brief Getter for property altName
  * @return The value of altName
  */
  std::string getAltName() { return _altName; }

  /**
  * @brief Setter for property uniform
  * @param aValue The new value to set for property uniform
  */
  void setUniform(int32 aValue) { _uniform = aValue; }

  /**
  * @brief Getter for property uniform
  * @return The value of uniform
  */
  int32 getUniform() { return _uniform; }

  int32 writeDimension(IDataDimensionWriter* writer);
 
 // Over ride from Superclass 
  void setNodeName(std::string nodeName);
  // Print out the node information to the console
  void printNode(std::ostream& os, int32 indent);
  
  /**
   * @brief Returns the maximum value that the Start Value can have */
  IDataDimension::size_type maxStartValue() { return std::numeric_limits<int32>::max(); }
  
  /** @brief Returns the maximum value that the End Value can have  */
  IDataDimension::size_type  maxEndValue() { return std::numeric_limits<int32>::max(); }
  
  /** @brief Returns the Maximun Increment value */
  IDataDimension::size_type  maxIncrement() { return std::numeric_limits<int32>::max(); }
  
  /** @brief returns the maximum value that the Count can be */
  IDataDimension::size_type  maxCount() { return std::numeric_limits<int32>::max(); }
  
  /** @brief   */
  bool isPropertyInitialized(int32 value)
  {
    if (value == std::numeric_limits<int32>::max() ||
        value == std::numeric_limits<int32>::min() )
      { return false; }
    
    return true;
  }
  
  bool isValid(std::string &message);

protected:
  MXADataDimension();

  int32 _index;
  int32 _count;
  int32 _startValue;
  int32 _endValue;
  int32 _increment;
  int32 _uniform;
  std::string _dimensionName;
  std::string _altName;

private:
    MXADataDimension(const MXADataDimension&);   //Copy Constructor Not Implemented
    void operator=(const MXADataDimension&); //Copy Assignment Not Implemented
};



#endif /* _MXADATAMODELNODE_H_ */

