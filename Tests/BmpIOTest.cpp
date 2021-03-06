///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
//-- MXA Headers
#include "MXA/MXA.h"
#include <MXA/BMPIO/MXABmpIO.h>
#include "UnitTestSupport.hpp"

//-- MXA Unit Test Headers
#include "MXAUnitTestDataFileLocations.h"




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES

#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestBMPIO()
{
  // Insert Test Code here.
  /**************Test stub I was using to develop class,
  ***************needs modified for use with test framework
	MXABmpIO* reader = new MXABmpIO();
	cout<<"New MXABmpIO class created\n";
	LOAD_TEXTUREBMP_RESULT res = reader->loadBMPData("/Users/hmullens/Desktop/RoboMetData/mosaic_test3_0816_10.bmp");

	switch(res)
  	{
  	case LOAD_TEXTUREBMP_SUCCESS:
  		{
			cout<<"Bitmap loaded successfully\n";
			cout<<"Dimensions: "<<reader->getWidth()<<" x "<<reader->getHeight()<<endl;
			cout<<"Number of channels: "<<reader->getNumberOfChannels()<<endl;
			break;
  		}
  	case LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE:
    	{
    		cout<<"File not found or could not be opened\n";
    		break;
    	}
  	case LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT:
    	{
    		cout<<"File format not a valid BMP file\n";
    		break;
    	}
    case LOAD_TEXTUREBMP_OPENGL_ERROR:
    	{
    		cout<<"OPENGL ERROR\n";
    		break;
    	}
    case LOAD_TEXTUREBMP_OUT_OF_MEMORY:
    	{
    		cout<<"System ran out of memory when allocating array for bitmap data\n";
    		break;
    	}
  	default:
    	{
    		cout<<"Unhandled error occurred"<<endl<<flush;
    		break;
    	}
  }
  */


}



// -----------------------------------------------------------------------------
//  Use test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  int err = EXIT_SUCCESS;

  MXA_REGISTER_TEST( TestBMPIO() );
  MXA_REGISTER_TEST( RemoveTestFiles() );
  PRINT_TEST_SUMMARY();
  return err;
}
