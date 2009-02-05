#ifndef ITIFFTAGEXTRACTOR_H_
#define ITIFFTAGEXTRACTOR_H_

#include <tiffio.h>

/**
* @class ITiffTagExtractor ITiffTagExtractor.h Base/ITiffTagExtractor.h
* @brief Abstract Base class for classes wishing to extract the contents from 
* specific Tiff Tags from the image file
* @author Mike Jackson
* @date Aug 2007
* @version $Revision: 1.1 $
*/
class MXA_EXPORT ITiffTagExtractor
{
  public:
    ITiffTagExtractor(){};
    virtual ~ITiffTagExtractor(){};
    
  /**
   * @brief Extracts the contents of a tag or tags, what ever the subclass needs to do.
   * @param tiff Pointer to the TIFF file
   */
  virtual void extractTagContents(TIFF* tiff) = 0;
};



#endif /*ITIFFTAGEXTRACTOR_H_*/
