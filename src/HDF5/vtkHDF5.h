#ifndef VTKHDF5_H_
#define VTKHDF5_H_

#include <Headers/DLLExport.h>

#include <hdf5.h>

#include "vtkImageData.h"

#include <string>

class  vtkHDF5
{
public:
  
  /**
   * @brief Given one of the types defined in VTKTypes.h this will return the 
   * appropriate HDF5 type
   * @param value The vtkType such at VTK_DOUBLE
   * @return The hdf5 type.
   */
  static MXA_EXPORT hid_t HDFTypeForVTKType(int value );
  
  /**
   * @brief Returns the VTKType of the data stored at the given location in the
   * HDF5 data file.
   * @param loc_id The HDF5 file or group id
   * @param dsetName The path/name to the data set
   * @return The VTK Type of the data.
   */
  static MXA_EXPORT int getScalarType(hid_t loc_id, const std::string &dsetName);
  
  static MXA_EXPORT herr_t getDataAsVTKImage(hid_t loc_id, const std::string &dsetName, vtkImageData* imgData);
  

protected:
  vtkHDF5();
  virtual ~vtkHDF5();
  
  static MXA_EXPORT herr_t _readVtkImageDataSet(hid_t loc_id, const std::string &dsetName, vtkImageData* imgData);
  
private:
  vtkHDF5(const vtkHDF5&);   //Copy Constructor Not Implemented
  void operator=(const vtkHDF5&); //Copy Assignment Not Implemented
};

#endif /*VTKHDF5_H_*/
