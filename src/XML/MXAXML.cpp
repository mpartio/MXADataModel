
#include "MXADataModel/MXADataModel.h"
#include "MHDXMLGenerator.h"

#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataModel::exportXMLDataModel(string filename)
{
  std::cout << "Exporting XML DAta Model" << std::endl;
  if (filename.empty()) {
    return false;
  }

  MHDXMLGenerator generator(this);
  std::cout << "Created XML Generator object" << std::endl;

  bool ok = generator.write(filename);
  if (ok) {
    std::cout << "Data Model Saved" << std::endl;
  }

  return ok;
}
