///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Shawn Nicholson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MHDXMLGENERATOR_H_
#define _MHDXMLGENERATOR_H_

#include "MXADataModel/MXADataModel.h"

#include <list>
#include <fstream>

class MHDXMLGenerator
{
 public:
  MHDXMLGenerator(MXADataModel *);

  bool write(string);

 private:
  static string indent(int);
  static string _toUpper(const string);
  static void _replaceAll(string&, string, string);
  static string escapedText(const string &);
  static string escapedAttribute(const string &);
  void _openTag(string, int, bool, map<string, string>, bool allupper=true);
  void _openTag(string, int, bool group=true);
  void _closeGroupTag(string, int);

  MXADataModel *_model;
  ofstream _out;

  void _saveDataRoot(int);
  void _saveDataDimensions(int);
  void _saveSignal(MXANode *, int);
  void _saveDataRecords(int);
  void _saveRequiredMetaData(int);
  void _saveUserDefinedMetaData(int);



};


#endif /* _MHDXMLGENERATOR_H_ */
