#ifndef _IDataset_h_
#define _IDataset_h_

#include <Base/IDataFileIO.h>


/**
* @class IDataset IDataset.h PathToHeader/IDataset.h
* @brief Pure virtual class that represents a dataset with appropriate attributes
* so that the data could be written to an underlying data file. Currently HDF5.
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT IDataset : public IDataFileIO
{
  public:
    IDataset(){}
    virtual ~IDataset(){}

    /**
     * @brief Sets the dataset path and name
     * @param path + name of the dataset
     */
    virtual void setDatasetPath(const std::string &path) = 0;

    /**
     * @brief Returns the path+name of the dataset
     */
    virtual std::string getDatasetPath() = 0;

    /**
     * @brief Sets the data for the dataset
     * @param data Instance of IMXAArray
     */
    virtual void setData(IMXAArrayPtr data) = 0;

    /**
     * @brief Returns the data for this class
     */
    virtual IMXAArrayPtr getData() = 0;

  /**
   * @brief Adds an attribute to this dataset
   * @param attributeKey The name of the attribute
   * @param data The data for the attribute
   */
    virtual void addAttribute (const std::string &attributeKey, IMXAArrayPtr data) = 0;

    /**
     * @brief Removes an attribute from the data set
     * @param attributeKey The name of the attribute to remove
     */
    virtual void removeAttribute (const std::string &attributeKey) = 0;

    /**
     * @brief Returns an attribute or NULL if not found
     * @param attributeKey The name of the attribute to retrieve
     * @return
     */
    virtual IMXAArrayPtr getAttribute(const std::string &attributeKey) = 0;

  protected:

  private:
      IDataset(const IDataset&);    //Not Implemented
      void operator=(const IDataset&); //Not Implemented

};

#endif //_IDataset_h_

