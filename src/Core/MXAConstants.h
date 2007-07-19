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
#ifndef _MXA_CONSTANTS_H_
#define _MXA_CONSTANTS_H_

#include <string>

#define FIELD_VARIABLE      "Signal"
#define DATA_TYPE_IMAGE     "Image"
//------------------------------------------------------------
// Data Model constant identifiers
//------------------------------------------------------------
namespace MXA {
  
  const std::string MXACurrentFileType("MHD");
  const float  MXACurrentFileVersion = 0.4f;

  // Defines the paths within the MXA Model Heirarchy
  const std::string FileVersion("File Version");
  const std::string FileType("File Type");
  const std::string DataModel("Data Model");
  const std::string DataRoot("Data Root");
  const std::string DataDimensions("Data Dimensions");
  const std::string DataRecords("Data Records");
  const std::string MetaData("Meta Data");
  const std::string RequiredMetaData("Required");
  const std::string UserMetaData("User Defined");
  
  // Defines the paths within the MXA Model Heirarchy
  const std::string FileVersionPath("File Version");
  const std::string FileTypePath("File Type");
  const std::string DataModelPath("Data Model");
  const std::string DataRootPath("Data Model/Data Root");
  const std::string DataDimensionsPath("Data Model/Data Dimensions");
  const std::string DataRecordsPath("Data Model/Data Records");
  const std::string MetaDataPath("Meta Data");
  const std::string RequiredMetaDataPath("Meta Data/Required");
  const std::string UserMetaDataPath("Meta Data/User Defined");
  
  
  // Data Model Tags
  const std::string MXA_NAME_TAG("Name");
  const std::string MXA_ALT_NAME_TAG("Alt_Name");

  // Data Dimension Tags
 // const std::string MXA_ORDER_TAG("Order");
  const std::string MXA_COUNT_TAG("Count");
  const std::string MXA_START_VALUE_TAG("Start_Value");
  const std::string MXA_END_VALUE_TAG("End_Value");
  const std::string MXA_UNIFORM_TAG("Uniform");
  const std::string MXA_INCREMENT_TAG("Increment");
  const std::string MXA_INDICES_TAG("Indices");
  const std::string MXA_INDEX_TAG("Index");

  // Data Record Tags
  const std::string MXA_LUID_TAG("LUID");
  const std::string MXA_GUID_TAG("GUID");

  // Required Meta Data Tags
  const std::string MXA_CREATOR_TAG("Creator");
  const std::string MXA_DATE_TAG("Date");
  const std::string MXA_DSET_NAME_TAG("Name");
  const std::string MXA_DESCRIPTION_TAG("Description");
  const std::string MXA_PEDIGREE_TAG("Pedigree");
  const std::string MXA_DERIVED_SRC_TAG("Original_Source_File");
  const std::string MXA_RIGHTS_TAG("Distribution_Rights");
  const std::string MXA_RELEASE_TAG("Release_Number");
#if 0
  // XML File Tags
  const std::string MXA_XML_FILE_ROOT("File_Root");
  const std::string MXA_XML_DATA_MODEL("Data_Model");
  const std::string MXA_XML_DATA_ROOT("Data_Root");
  const std::string MXA_XML_NAME_ATTR("NAME");
  const std::string MXA_XML_ALT_NAME_ATTR("ALT_NAME");
  const std::string MXA_XML_DATA_DIMENSIONS_GROUP("Data_Dimensions");
  const std::string MXA_XML_DATA_DIMENSION("Dimension");
  const std::string MXA_XML_DATA_RECORDS_GROUP("Data_Records");
  const std::string MXA_XML_SIGNAL_GROUP("Signal_Group");
  const std::string MXA_XML_SIGNAL("Signal");
  const std::string MXA_XML_META_DATA("Meta_Data");
  const std::string MXA_XML_REQUIRED_META_DATA("Required_MD");
  const std::string MXA_XML_USER_META_DATA("UserDefined_MD");
#endif
  
  /* These are tags that can be used in configuration files for data import
   * or other purposes. Use these constants to get the correct tag. */

  const std::string MD_Creator("REQ_MD_CREATOR");
  const std::string MD_DateCreated("REQ_MD_DATE_CREATED");
  const std::string MD_DatasetName("REQ_MD_DATASET_NAME");  
  const std::string MD_Description("REQ_MD_DESCRIPTION");
  const std::string MD_Pedigree("REQ_MD_PEDIGREE");
  const std::string MD_DistributionRights("REQ_MD_DISTRIBUTION_RIGHTS");
  const std::string MD_DerivedSourceFiles("REQ_MD_DERIVED_SOURCE_FILES");
  const std::string MD_ReleaseNumber("REQ_MD_RELEASE_NUMBER");
  
  /* Tags for Actual Data entries. Can help with parsers. Optional */
  const std::string FieldVariable(FIELD_VARIABLE);
  const std::string DataTypeImage(DATA_TYPE_IMAGE);
}

// -----------------------------------------------------------------------------
//  Rendering Hint Attributes that can be added to datasets to help out data viewers
// -----------------------------------------------------------------------------
namespace RenderHint
{
  const std::string RenderHint ("RenderHint");
  const std::string ImageRGB ("ImageRGB");
  const std::string ImageRGBA ("ImageRGBA");
  const std::string ImageGrayScale("ImageGrayScale");
  const std::string ImageFloatingPoint("ImageFloatingPoint");
  const std::string ImageIndexed ("ImageIndexed");
}
#endif /* _MXA_CONSTANTS_H_ */
