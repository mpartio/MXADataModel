///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TIFFMAKER_H_
#define TIFFMAKER_H_

#include <string>


class TiffMaker
{
  public:
    TiffMaker();
    virtual ~TiffMaker();


    void createTiffFile(const std::string &filename);

  private:
    TiffMaker(const TiffMaker&);    // Copy Constructor Not Implemented
      void operator=(const TiffMaker&);  // Operator '=' Not Implemented
};

#endif /* TIFFMAKER_H_ */
