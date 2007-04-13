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
#ifndef _XMLWRITER_H_
#define _XMLWRITER_H_

// MXA Includes
#include "MXADataModel/MXADataModel.h"
#include "Interfaces/IDataModelWriter.h"
#include "Interfaces/IDataDimensionWriter.h"
#include "Interfaces/IDataDimension.h"
#include "Interfaces/IDataRecordWriter.h"
#include "Interfaces/IDataRecord.h"

//-- STL Headers
#include <iostream>
#include <string>
#include <fstream>

class MXANode;


/**
 * @brief
 * @auther
 * @date
 * @version
 * @header
 */
class XMLDataModelWriter : public IDataModelWriter, 
                           public IDataDimensionWriter,
                           public IDataRecordWriter
{

public:
  XMLDataModelWriter( MXADataModel* dataModel, const std::string &fileName );
  virtual ~XMLDataModelWriter();

  bool writeModelToFile(int32 fileId);

  int32 writeDataModelTemplate(int32 uniqueId);
  int32 writeDataDimensions(int32 uniqueId);
  int32 writeDataRecords(int32 uniqueId);
  int32 writeRequiredMetaData(int32 uniqueId);
  int32 writeUserMetaData(int32 uniqueId);

  void _saveDataRoot(int);

  // --------IDataDimensionWriter Interface ----------------------
  int32 writeDataDimension(IDataDimension* dim);
  // --------IDataRecordWriter Interface -------------------------
  int32 writeDataRecord(IDataRecord* record);

protected:
  static std::string indent(int indent);
  static std::string _toUpper(const std::string value);
  static void _replaceAll(std::string&, std::string, std::string);
  static std::string escapedText(const std::string &value);
  static std::string escapedAttribute(const std::string &value);
  void _openTag(std::string, int, bool, std::map<std::string, std::string>, bool allupper=true);
  void _openTag(std::string, int, bool group=true);
  void _closeGroupTag(std::string, int);

  
private:
  //H5IODelegate* _ioDelegate;
  MXADataModel* _dataModel;
  boost::shared_ptr<std::ofstream> _ofstreamPtr;
  std::string _fileName;
  
  XMLDataModelWriter(const XMLDataModelWriter&);   //Copy Constructor Not Implemented
  void operator=(const XMLDataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _XMLWRITER_H_ */