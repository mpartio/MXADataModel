///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef COMPLEXIMPORTEXAMPLE_H_
#define COMPLEXIMPORTEXAMPLE_H_

#include "MXADataModel/MXADataModel.h"


#include <string>
#include <map>


/**
 * @brief Stores some constants just for use in the example import code
 * @namespace ExampleConstants
 */
namespace ExampleConstants {
  const std::string TimestepAltName("Timestep");
  const std::string TimestepName("Timestep");
  const std::string SliceName("Slice");
  const std::string SliceAltName("XY Volume Slice");
  const std::string CompositionName("Composition");
  const std::string CompositionAltName("Volume Fraction");
  const std::string OrderParameters("Order Parameters");
  const std::string Eta1("Eta 1");
  const std::string Eta2("Eta 2");
  const std::string Eta3("Eta 3");
  
};

/**
* @class ComplexImportExample ComplexImportExample.h UnitTests/ModelConstructionEx/ComplexImportExample.h
* @brief Shows a more involved example of importing data into the MXA file format
* @author Mike Jackson
* @date May 2007
* @version 1.0
*/
class ComplexImportExample
{
public:
	ComplexImportExample();
	virtual ~ComplexImportExample();
  
  typedef std::map<std::string, std::string> RequiredMetaData;
  /**
   * @brief
   */
  void run();
  
  
  protected:
    MXADataModel _model;
    std::string _hdfFileOutput;
    std::string _simRoot;
    
    int32 _timestepStart;
    int32 _timestepEnd;
    int32 _timestepIncrement;

    int32 _sliceStart;
    int32 _sliceEnd;
    int32 _sliceIncrement;
    
    virtual   void setupModel();
    virtual   void makeDimensions();
    virtual   void makeRecords();
    virtual   void makeRequiredMetaData(ComplexImportExample::RequiredMetaData &md);
    
  private:
      ComplexImportExample(const ComplexImportExample&);  //Copy constructor NOT implemented
      void operator=(const ComplexImportExample); //Copy assignment NOT implemented
};

#endif /*COMPLEXIMPORTEXAMPLE_H_*/
