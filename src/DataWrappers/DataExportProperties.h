///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _DataExportProperties_h_
#define _DataExportProperties_h_

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Base/IImportProperty.h>

//-- HDF includes
#include <hdf5.h>


/**
 * @namespace This is for the Exporting functionality
 */
namespace MXA
{
namespace Export
{
  static int Ascii = 0;
  static int Binary = 1;

  static int BigEndian = 1;
  static int LittleEndian = 0;

  const std::string Comma(",");
  const std::string Tab("\t");
  const std::string Space(" ");


  MXA_PROPERTY_CONSTANT(HDFFileId);
  MXA_PROPERTY_CONSTANT(HDFDatasetPath);
  MXA_PROPERTY_CONSTANT(ExportFile);
  MXA_PROPERTY_CONSTANT(FileType);
  MXA_PROPERTY_CONSTANT(AsciiDelimiter);
  MXA_PROPERTY_CONSTANT(Endian);
}

}


/**
* @class DataExportProperties DataExportProperties.h Base/DataExportProperties.h
* @brief Base class for Data Exporters
* @author Michael Jackson for BlueQuartz Software
* @date Oct 20, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT DataExportProperties
{
  public:

    /**
     * @brief
     */
    static DataExportPropertiesPtr New()
    {
      DataExportPropertiesPtr ptr(new DataExportProperties);
      return ptr;
    }

    /**
     * @brief
     */
    static DataExportPropertiesPtr New(hid_t fileId, const std::string &exportFile, const std::string &datasetPath,
                         int fileType, const std::string &asciiDelimiter, int endian)
    {
      DataExportPropertiesPtr ptr(new DataExportProperties);
      ptr->setHDFFileId(fileId);
      ptr->setExportFile(exportFile);
      ptr->setHDFDatasetPath(datasetPath);
      ptr->setFileType(fileType);
      ptr->setAsciiDelimiter(asciiDelimiter);
      ptr->setEndian(endian);
      return ptr;
    }

    virtual ~DataExportProperties(){}

    MXA_PROPERTY(hid_t, HDFFileId, _fileId);

    MXA_STRING_PROPERTY(HDFDatasetPath, _datasetPath);

    MXA_STRING_PROPERTY(ExportFile, _exportFile);

    MXA_PROPERTY (int, FileType, _fileType);
    virtual void setFileTypeToBinary() { this->_fileType = MXA::Export::Binary; }
    virtual void setFileTypeToAscii() { this->_fileType = MXA::Export::Ascii; }

    MXA_STRING_PROPERTY( AsciiDelimiter, _asciiDelimiter);

    MXA_PROPERTY (int, Endian, _endian);
    virtual void setEndianToBig() { this->_endian = MXA::Export::BigEndian; }
    virtual void setEndianToLittle() { this->_endian = MXA::Export::LittleEndian;  }

    virtual int32 setProperty (const std::string &key, const std::string &value)
    {
      SET_PROPERTY_BODY(MXA::Export, hid_t, HDFFileId, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, ExportFile, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, HDFDatasetPath, key, value);
      SET_PROPERTY_BODY(MXA::Export, int, FileType, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, AsciiDelimiter, key, value);
      SET_PROPERTY_BODY(MXA::Export, int, Endian, key, value);
      return -1;
    }

    virtual int32 getProperty(const std::string &key, std::string &value)
    {
      GET_PROPERTY_BODY(MXA::Export, hid_t, HDFFileId, _fileId, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, ExportFile, _exportFile, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, HDFDatasetPath,_datasetPath, key, value);
      GET_PROPERTY_BODY(MXA::Export, int, FileType, _fileType, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, AsciiDelimiter, _asciiDelimiter, key, value);
      GET_PROPERTY_BODY(MXA::Export, int, Endian, _endian, key, value);
      return -1;
    }

  protected:
    DataExportProperties() :
     _fileId (-1),
     _exportFile(),
     _datasetPath(),
     _fileType(MXA::Export::Ascii),
     _asciiDelimiter(MXA::Export::Comma),
     _endian(MXA::Export::LittleEndian)
    {}

  private:
    hid_t _fileId;
    std::string _exportFile;
    std::string _datasetPath;
    int _fileType;
    std::string _asciiDelimiter;
    int _endian;

    DataExportProperties(const DataExportProperties&);    //Not Implemented
    void operator=(const DataExportProperties&); //Not Implemented

};




#endif /*_DataExportProperties_H_ */
