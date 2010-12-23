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

#include <MXA/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportProperty.h>

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
* @version $Revision: 1.2 $
*/
class MXA_EXPORT DataExportProperties
{
  public:
    MXA_SHARED_POINTERS(DataExportProperties);

    /**
     * @brief
     */
    static DataExportProperties::Pointer New()
    {
      DataExportProperties::Pointer ptr(new DataExportProperties);
      return ptr;
    }

    /**
     * @brief
     */
    static DataExportProperties::Pointer New(hid_t fileId, const std::string &exportFile,
                                             const std::string &datasetPath,
                                             int fileType,
                                             const std::string &asciiDelimiter,
                                             int endian)
    {
      DataExportProperties::Pointer ptr(new DataExportProperties);
      ptr->setHDFFileId(fileId);
      ptr->setExportFile(exportFile);
      ptr->setHDFDatasetPath(datasetPath);
      ptr->setFileType(fileType);
      ptr->setAsciiDelimiter(asciiDelimiter);
      ptr->setEndian(endian);
      return ptr;
    }

    virtual ~DataExportProperties(){}

    MXA_INSTANCE_PROPERTY(hid_t, HDFFileId);

    MXA_INSTANCE_STRING_PROPERTY(HDFDatasetPath);

    MXA_INSTANCE_STRING_PROPERTY(ExportFile);

    MXA_INSTANCE_PROPERTY (int, FileType);
    virtual void setFileTypeToBinary() { this->m_FileType = MXA::Export::Binary; }
    virtual void setFileTypeToAscii() { this->m_FileType = MXA::Export::Ascii; }

    MXA_INSTANCE_STRING_PROPERTY( AsciiDelimiter);

    MXA_INSTANCE_PROPERTY (int, Endian);
    virtual void setEndianToBig() { this->m_Endian = MXA::Export::BigEndian; }
    virtual void setEndianToLittle() { this->m_Endian = MXA::Export::LittleEndian;  }

    virtual int32_t setProperty (const std::string &key, const std::string &value)
    {
      /*
      SET_PROPERTY_BODY(MXA::Export, hid_t, HDFFileId, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, ExportFile, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, HDFDatasetPath, key, value);
      SET_PROPERTY_BODY(MXA::Export, int, FileType, key, value);
      SET_PROPERTY_BODY(MXA::Export, std::string, AsciiDelimiter, key, value);
      SET_PROPERTY_BODY(MXA::Export, int, Endian, key, value);
      */
      return -1;
    }

    virtual int32_t getProperty(const std::string &key, std::string &value)
    {
#if 0
      GET_PROPERTY_BODY(MXA::Export, hid_t, HDFFileId, _fileId, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, ExportFile, _exportFile, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, HDFDatasetPath,_datasetPath, key, value);
      GET_PROPERTY_BODY(MXA::Export, int, FileType, _fileType, key, value);
      GET_PROPERTY_BODY(MXA::Export, std::string, AsciiDelimiter, _asciiDelimiter, key, value);
      GET_PROPERTY_BODY(MXA::Export, int, Endian, _endian, key, value);
#endif
      return -1;
    }

  protected:
    DataExportProperties() :
     m_HDFFileId (-1),
     m_HDFDatasetPath(),
     m_ExportFile(),
     m_FileType(MXA::Export::Ascii),
     m_AsciiDelimiter(MXA::Export::Comma),
     m_Endian(MXA::Export::LittleEndian)
    {}

  private:
    DataExportProperties(const DataExportProperties&);    //Not Implemented
    void operator=(const DataExportProperties&); //Not Implemented

};




#endif /*_DataExportProperties_H_ */
