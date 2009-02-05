#error This file needs updating. It should NOT be included in any compiles

#include <ComplexImportExample.h>
#include <ExampleImportDelegate.h>
#include <MXADataModel/MXANode.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ComplexImportExample::ComplexImportExample() :
_model(),
_hdfFileOutput("ComplexImportExample.h5"),
_simRoot("OurExperiment/MyData"),
_timestepStart(1),
_timestepEnd(2),
_timestepIncrement(1),
_sliceStart(0),
_sliceEnd(2),
_sliceIncrement(1)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ComplexImportExample::~ComplexImportExample() {

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComplexImportExample::run() {
  this->setupModel();

  std::map<int, int> dimVals;
  ExampleImportDelegate delegate;
  std::string dataFile("");

  for (int t = this->_timestepStart; t <= this->_timestepEnd; t+=this->_timestepIncrement) {

    for (int s = this->_sliceStart; s <= this->_sliceEnd; s+=this->_sliceIncrement) {

      dimVals[0] = t;
      dimVals[1] = s;

      //Composition
      _model.addDataSource(dimVals, "", 0, dataFile, &delegate); //Composition
      _model.addDataSource(dimVals, "1", 0, dataFile, &delegate); //Eta 1
      _model.addDataSource(dimVals, "1", 1, dataFile, &delegate); //Eta 2
      _model.addDataSource(dimVals, "1", 2, dataFile, &delegate); //Eta 3

    }

  }

  _model.writeMHDFile(this->_hdfFileOutput);
}



// -----------------------------------------------------------------------------
//  Convenience Method to setup the Phasefield specific Data Model Section
// -----------------------------------------------------------------------------
void ComplexImportExample::setupModel()
{
  // herr_t err = 0;

  _model.setDataRoot(this->_simRoot); //Set the "DataRoot" property
  _model.setMHDVersion(0.4);
  _model.setMHDFileType("MHD");

  //Create a Map of all the attributes
  RequiredMetaData reqMetaData;
  makeRequiredMetaData(reqMetaData);
  this->_model.setRequiredMetaData(reqMetaData);
  makeDimensions();
  makeRecords();

  //return err;
}

// -----------------------------------------------------------------------------
//  Sets up the Required Meta Data for this Data set
// -----------------------------------------------------------------------------
void ComplexImportExample::makeRequiredMetaData(ComplexImportExample::RequiredMetaData &md) {
  md[MXA_CREATOR_TAG] = "Mike Jackson";
  md[MXA_DATE_TAG] = "2006:12:24 15:34.51";
  md[MXA_DSET_NAME_TAG] = "Phasefield VF54 simulation";
  md[MXA_DESCRIPTION_TAG] = "Computer Simulation of NiAl at 54% Vol Fraction";
  md[MXA_PEDIGREE_TAG] = "Original";
  md[MXA_DERIVED_SRC_TAG] = "Original Data Files";
  md[MXA_RIGHTS_TAG] = "Unlimited";
  md[MXA_RELEASE_TAG] = "Ws-xxx-xxx";
}


// -----------------------------------------------------------------------------
// Create the DataDimensions
// -----------------------------------------------------------------------------
void ComplexImportExample::makeDimensions() {

  MXANode* tsDim = this->_model.addDataDimension( ExampleConstants::TimestepName,
                                                  ExampleConstants::TimestepAltName);
  _model.setDimensionStartValue(tsDim, this->_timestepStart);
  _model.setDimensionCount(tsDim, this->_timestepEnd - this->_timestepStart + 1);
  _model.setDimensionUniform(tsDim, (this->_timestepIncrement == 1));  // this is the default
  _model.setDimensionIncrement(tsDim, this->_timestepIncrement);   // this is the default


  MXANode* slDim = this->_model.addDataDimension(ExampleConstants::SliceName,
                                                 ExampleConstants::SliceAltName);
  _model.setDimensionStartValue(slDim, this->_sliceStart);
  _model.setDimensionCount(slDim, this->_sliceEnd - this->_sliceStart + 1);
  _model.setDimensionUniform(slDim, (this->_sliceIncrement == 1));  // this is the default
  _model.setDimensionIncrement(slDim, this->_sliceIncrement);   // this is the default
}

// -----------------------------------------------------------------------------
// Create the DataRecords
// -----------------------------------------------------------------------------
void ComplexImportExample::makeRecords() {

  MXANode* composition = _model.addDataRecord(ExampleConstants::CompositionName, ExampleConstants::CompositionName);
  MXANode* orderParam = _model.addDataRecord(ExampleConstants::OrderParameters, ExampleConstants::OrderParameters);
  MXANode* eta1 = _model.addDataRecord(ExampleConstants::Eta1, ExampleConstants::Eta1, orderParam);
  MXANode* eta2 = _model.addDataRecord(ExampleConstants::Eta2, ExampleConstants::Eta2, orderParam);
  MXANode* eta3 = _model.addDataRecord(ExampleConstants::Eta3, ExampleConstants::Eta3, orderParam);

}

