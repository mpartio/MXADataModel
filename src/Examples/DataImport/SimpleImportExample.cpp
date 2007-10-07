#include <SimpleImportExample.h>


//-- MHD Headers
#include <MXADataModel/MXADataModel.h>



//-- STL Headers
#include <iostream>


SimpleImportExample::SimpleImportExample()
{
}

SimpleImportExample::~SimpleImportExample()
{
}

// Will create a data model for the Ni-Al-Cr sample
//  This has 1 dimension (slice) and one record (image)
//  For this sample - there will be 4 values the dimension slice
//  can take on but I will only fill in 2 of them (sparse sample)
//
// After the model is built - this app will write out three files:
//   1) ni-al-cr-sample-datamodel.xml
//   2) ni-al-cr-sample-datafile.hdf
//   3) ni-al-cr-sample-datafile-w-sources.hdf
// containting an XML copy of the datamodel and a written HDF file
//  without the sample image sources and with the sample image sources
//  respectively
//

void SimpleImportExample::run()
{
  MXADataModelPtr modelPtr = MXADataModel::New();
  MXADataModel* model = modelPtr.get();

  // Set the data root (Defaults to "Data/")
  //  Note the MHDataModel code automatically assures the
  //  data root will end with a /
  //  If you pass in an empty string it will revert to the default
  //    ie  setDataRoot("") == a data root of "Data/"
  //  Note it currently does not ensure against strings
  //   that contain only whitespace (on to do list)
  //const std::string dataRoot("NI-AL-CR-ImageData");
  model->setDataRoot( "NI-AL-CR-ImageData");

  // Set the file version 
  //  Note - this should NOT be called in most cases - the default
  //   value is the currently supported version (currently 0.4)
  model->setModelVersion( MXA::MXACurrentFileVersion );

  // Set the file type
  //  Note - this should NOT be called in most cases - the default
  //   value is the currently supported file version (currently MHD or XMDA)
  //  Note - case does matter  (on TO DO list)
  model->setModelType( MXA::MXACurrentFileType );

  
  // For setting the required meta data you have a couple options
  //  1) set each indidually by calling:
  //      model->setRequiredMetaData(MHD_CREATOR_TAG, "Troublemaker");
  //      model->setRequiredMetaData(MHD_DATE_TAG, "Dec 20, 2006");
  //      ... 
  //   
  //  2) set them all at once by calling
  //      model->setRequiredMetaData(creator, date, name, description,
  //                 distribution_rights, release_number, pedigree, 
  //                 original_source_file)
  //     where each argument is a std::string
  //       Pedigree must be one of:  Original  Derived
  //       distribution_rights must be one of:  Unlimited  Proprietary
  //         Case Matters
  //         This is not enforced but there is currently no support
  //         for any other types
  // 
  //  3) create a std::map<std::string, std::string> mymap of all the 
  //         attributes
  //       where the key is one of the MHD_<META DATA>_TAG constants
  //          (see MHDConstants.h in the Meta data section for them)
  //       and the value is the value and call:
  //      model->setRequiredMetaData(mymap);
  // 
  //  side note:  calling  model->clearRequiredMetaData()
  //    sets the required meta data to default values of all empty strings
  //    except distribution rights (set to Unlimited) and pedigree (set
  //    to Original)
  model->setRequiredMetaData("Shawn", "12/20/2006 17:34:06", 
            "ni-al-cr dataset sample", 
            "A sample dataset showing images from an experiment using ni-al-cr",
            "Unlimited", "Release v1.0", "Original", "");

  
  // For setting user defined meta data you have two choices
  //  1) set each indidually by calling:
  //      model->setUserMetaData("sample label", "sample value")
  //      model->setUserMetaData("experimenter", "john doe");
  //      ... 
  //   
  //  2) create a std::map<std::string, std::string> mymap of all the 
  //         attributes
  //       where the key is the label for the attribute
  //       and the value is the value for the attribute
  //     and then calling:
  //      model->setUserMetaData(mymap);
  //
  model->addUserMetaData(std::string("Experimenter"), std::string("John Doe") );

  
  // Adding Dimensions
  // See MHDataModel.h for the different methods for adding dimensions
  //  The easiest way to add a dimension is to call
  //     model->addDataDimension(name, alt_name) 
  //   and then set any properties you want for it.  This call
  //  creates a dimension with a uniform increment of 1 starting at 0
  //  and having only 1 value it can take on.  
  //    Using this method the order of the dimensions is set by
  //  the order they are created (the first is dim 0, second dim 1, etc)
  //  
  //  non-uniform dimensions are not fully supported yet.

  MXADataDimensionPtr dim = model->addDataDimension("Slice", "Image Slice", 0, 4, 100, 103, 1, 1);


  // Adding Records
  // See MHDataModel.h for the different methods for adding records
  //   Like dimensions the easiest way is to call:
  //     model->addDataRecord(name, alt_name, MHDNode *parent)
  //   where parent is the node pointer from a previously added record
  //   ex)
  //    MHDNode *rec1 = model->addDataRecord("Image", "image data");
  //    MHDNode *comp = model->addDataRecord("Composition", "composition");
  //    MHDNode *eta1 = model->addDataRecord("eta1", "eta1", comp);
  //
  MXADataRecordPtr rec0 = MXADataRecord::New(0,std::string("Image"), std::string("Ni-AL-CR Slice Image"));
  model->addDataRecord(rec0);
  //---------
  // And that would be a complete Data Model description
  //---------

  // Writing this out to an HDF Data File (*.hdf *.h5  *.hdf5):
  model->writeModel( MHD_FILE);

//TODO: Export out to XML file
  
  
  //---------
  // Assigning data sources
  //  Assigning data sources can also be done in a couple ways
  //  This is currently still a work in progress
  // Adding a data source uses the call:
  //    model->addDataSource(dimensionValues, recordPath, recordLuid, 
  //                           sourceFilePath);
  //  where dimensionValues is a std::map<int, int>
  //     where the key is the dimension order(index) and the value
  //     is the value of that dimension 
  //  recordPath is the path to the record from any parent records (luids)
  //     image would have a record path of ""
  //     composition for composition would have a record path of ""
  //     composition/eta1 for eta1 would have a record path of "1"
  //        assuming composition had the luid: 1
  //  recordLuid is the luid for the data record being assigned a source
  //  sourceFilePath is the path to the data source file
  //
  //  In this sample - we have 1 dimension (so the dimvals map will 
  //    have only 1 key/val pair)
  //   this dimension has values ranging from 100-103 (4 values)
  //   I am only setting two of them.
  
#if 0
  TiffToHDFDelegate delegate; //Allocate on the stack
  
  std::map<int, int> sample_dim_vals;
  sample_dim_vals[0] = 100;
  model->addDataSource(sample_dim_vals, "", 0, "/Users/mjackson/Task_7/Workspace/MHDataModel/UnitTests/100.tif", &delegate);
  sample_dim_vals[0] = 101;
  model->addDataSource(sample_dim_vals, "", 0, "/Users/mjackson/Task_7/Workspace/MHDataModel/UnitTests/101.tif", &delegate);
  
  //Clean up the memory
  model->writeMHDFile(MHD_FILE_W_SOURCES);
#endif

}




