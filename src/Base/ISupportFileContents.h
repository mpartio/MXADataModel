///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ISUPPORTFILECONTENTS_H_
#define ISUPPORTFILECONTENTS_H_


/**
* @class ISupportFileContents ISupportFileContents.h src\Base\ISupportFileContents.h
* @brief
* @author  mjackson
* @date Sep 23, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT ISupportFileContents
{

  public:
    ISupportFileContents() {};
    virtual ~ISupportFileContents() {};

    virtual uint8* getFilePointer(uint64) = 0;

  private:
    ISupportFIleContents(const ISupportFIleContents&);    // Copy Constructor Not Implemented
      void operator=(const ISupportFIleContents&);  // Operator '=' Not Implemented


};



#endif /* ISUPPORTFILECONTENTS_H_ */
