///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IDATARECORDWRITER_H_
#define _IDATARECORDWRITER_H_

#include <MXA/Common/DLLExport.h>

class IDataRecord;

/**
 * @brief The interface that defines the method to implement in order to write a
 * data record to a file
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *  
 */
class MXA_EXPORT IDataRecordWriter
{

public:
  IDataRecordWriter(){};
  virtual ~IDataRecordWriter(){};
  
  /**
  * @brief Writes a DataRecord to the underlying storage mechanism
  * @param record The Data Record to Write
  * @return Error Code: Negate is error condition
  */
  virtual int32_t writeDataRecord(IDataRecord* record) = 0;
  
private:
    IDataRecordWriter(const IDataRecordWriter&);   //Copy Constructor Not Implemented
    void operator=(const IDataRecordWriter&); //Copy Assignment Not Implemented
};



#endif /* _IDATARECORDWRITER_H_ */

