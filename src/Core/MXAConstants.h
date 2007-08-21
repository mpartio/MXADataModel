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
  
  const std::string MXACurrentFileType("MXA");
  const float  MXACurrentFileVersion = 0.4f;

  // Defines the paths within the MXA Model Heirarchy
  const std::string ModelVersion("Model Version");
  const std::string ModelType("Model Type");
  const std::string DataModel("Data Model");
  const std::string DataRoot("Data Root");
  const std::string DataDimensions("Data Dimensions");
  const std::string DataRecords("Data Records");
  const std::string MetaData("Meta Data");
  const std::string RequiredMetaData("Required");
  const std::string UserMetaData("User Defined");
  
  // Defines the paths within the MXA Model Heirarchy
  const std::string ModelVersionPath("Model Version");
  const std::string ModelTypePath("Model Type");
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

// -----------------------------------------------------------------------------
//  Pedigree is either Original OR Derived
// -----------------------------------------------------------------------------
  const std::string MXA_PEDIGREE_TAG("Pedigree");
  //Constants for the Pedigree Meta Data Value - Should be one of these
  const std::string MXA_PEDIGREE_ORIGINAL_VALUE("Original");
  const std::string MXA_PEDIGREE_DERIVED_VALUE("Derived");

  
// -----------------------------------------------------------------------------
//  If the pedigree is derived then the value of Original_Source_File tag is
//   the file path and name to the original data other wise use Not Applicable
//   [MXA_NOT_APPLICABLE_VALUE] as the value
// -----------------------------------------------------------------------------
  const std::string MXA_DERIVED_SRC_TAG("Original_Source_File");
  
// -----------------------------------------------------------------------------
//  Distribution rights are either Unlimited or Limited. If Limited then the 
//  Release Limitation should contain a description of the nature of the limitation, 
//  ie, who imposed the limitation, what groups CAN have possession of the data or what groups
//  can NOT be in possession of the data.
// -----------------------------------------------------------------------------
  const std::string MXA_RIGHTS_TAG("Distribution_Rights");
  //Constants for predefined values related to the Distribution Rights tag
  const std::string MXA_RIGHTS_UNLIMITED_VALUE("Unlimited");
  const std::string MXA_RIGHTS_LIMITED_VALUE("Limited");  
  const std::string MXA_RELEASE_LIMITATION_TAG("Release_Limitation");
  
// -----------------------------------------------------------------------------
//  Some organizations assign a tracking value to data that is publicly released.
//   That value should be placed in this tag otherwise use the [MXA_NOT_APPLICABLE_VALUE]
//   default value.
// -----------------------------------------------------------------------------
  const std::string MXA_RELEASE_NUMBER_TAG("Release_Number");
  const std::string MXA_NOT_APPLICABLE_VALUE("Not Applicable");  




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
