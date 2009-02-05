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
#ifndef _IDATADIMENSIONWRITER_H_
#define _IDATADIMENSIONWRITER_H_

#include <MXA/Common/DLLExport.h>

class IDataDimension;

/**
 * @brief Formal Interface description for classes wanting to implement DataDimension
 * serialization capabilities.
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *  
 */
class MXA_EXPORT IDataDimensionWriter
{

public:
  IDataDimensionWriter(){};
  virtual ~IDataDimensionWriter(){};
  
  /**
   * @brief Writes an IDataDimension to the underlying data storage
   * @param dim Non-NULL IDataDimension object
   * @return Error condition
   */
  virtual int32 writeDataDimension(IDataDimension* dim) = 0;

private:
    IDataDimensionWriter(const IDataDimensionWriter&);   //Copy Constructor Not Implemented
    void operator=(const IDataDimensionWriter&); //Copy Assignment Not Implemented
};



#endif /* _IDATADIMENSIONWRITER_H_ */

