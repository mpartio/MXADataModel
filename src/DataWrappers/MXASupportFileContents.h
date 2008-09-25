///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MXASUPPORTFILECONTENTS_H_
#define MXASUPPORTFILECONTENTS_H_

#include <Common/MXATypeDefs.h>
#include <DataWrappers/MXAArrayTemplate.hpp>


/**
* @class MXASupportFileContents MXASupportFileContents.h src\DataWrappers\MXASupportFileContents.h
* @brief
* @author  mjackson
* @date Sep 23, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT MXASupportFileContents : public MXAArrayTemplate<uint8>
{
public:
  static IMXAArrayPtr CreateMXASupportFileContents(int64 fileLength);

  static MXASupportFileContents* New( int64 fileLength );

  virtual ~MXASupportFileContents();

  uint8* getFilePointer(uint64);


protected:
	explicit MXASupportFileContents(int64 fileLength);


private:
  MXASupportFileContents(const MXASupportFileContents&);    // Copy Constructor Not Implemented
  void operator=(const MXASupportFileContents&);  // Operator '=' Not Implemented

};

#endif /* MXASUPPORTFILECONTENTS_H_ */
