#ifndef _XMLStreamReaderDelegate_hpp_
#define _XMLStreamReaderDelegate_hpp_

#include "MXA/Common/MXASetGetMacros.h"

#include "MXA/Base/IMXADataModelReaderDelegate.h"
#include <MXA/Base/IDataModel.h>
#include "MXA/XML/DataModelXMLEvtHandler.h"

#define BUFFER_SIZE 1024

template<typename T>
class XMLStreamReaderDelegate: public IMXADataModelReaderDelegate,
                               public ExpatEvtHandler
{
  public:
    MXA_SHARED_POINTERS(XMLStreamReaderDelegate<T> )
    MXA_TYPE_MACRO(XMLStreamReaderDelegate<T> )


    typedef typename boost::shared_ptr<T>  StreamPointer;

    static Pointer New()
    {
     StreamPointer out = StreamPointer(new T);
     Pointer sharedPtr(new XMLStreamReaderDelegate<T>(out));
     return sharedPtr;
    }

    virtual ~XMLStreamReaderDelegate() {}

    virtual IDataModel::Pointer readModel();

    MXA_INSTANCE_PROPERTY_m(StreamPointer, StreamPointer);


  protected:
    XMLStreamReaderDelegate(StreamPointer stream)
    {
      m_StreamPointer = stream;
    }

  private:
    MXA_INSTANCE_PROPERTY_m(IDataModel::Pointer, DataModel)
    MXA_INSTANCE_PROPERTY_m(DataModelXMLEvtHandler::Pointer, XMLEvtHandler)

    ExpatParser::Pointer      m_Parser;

    XMLStreamReaderDelegate(const XMLStreamReaderDelegate&); // Copy Constructor Not Implemented
    void operator=(const XMLStreamReaderDelegate&); // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
IDataModel::Pointer XMLStreamReaderDelegate<T>::readModel()
{
//  std::cout << "XMLDataModelReader::readDataModel -> Data Records Size: " << this->_dataModel->getDataRecords().size() << std::endl;
//  if ( this->m_DataModel->getDataRecords().size() != 0 )
//  {
//    std::cout << "XMLStreamReaderDelegate::readDataModel - The data model has Data Records. This will turn out to be bad. You should be"
//    << " supplying a data model that is clean." << std::endl;
//    return IDataModel::NullPointer();
//  }

  m_DataModel = MXADataModel::New();

  // Create and initialise an instance of the parser.
  ExpatEvtHandler::Pointer evtHandler = DataModelXMLEvtHandler::NewExpatEvtHandler(m_DataModel);
  m_Parser = ExpatParser::New(evtHandler.get() );
  DataModelXMLEvtHandler* dmEvtHandler = dynamic_cast<DataModelXMLEvtHandler*>(evtHandler.get());
  dmEvtHandler->setParser(m_Parser.get() ); // We use the raw pointer other wise a circular dependency will occur

  m_Parser->Create(NULL, NULL);
  m_Parser->EnableElementHandler();
  m_Parser->EnableCharacterDataHandler();

  // Load the XML file.
  char buf[BUFFER_SIZE];
  bool   atEnd = false;
  size_t nRead;
  int xmlParseError = 0;
  if ( !m_StreamPointer->good())
  {
    return IDataModel::NullPointer();
  }

  while ( false == atEnd && xmlParseError >= 0)
  {
    // Read a block from the XML file and pass it to the parser
    m_StreamPointer->read (buf, BUFFER_SIZE);
    nRead = m_StreamPointer->gcount();
    if (m_StreamPointer->eof() || m_StreamPointer->fail() )
    {
      atEnd = true;
    }
    m_Parser->Parse(buf, nRead, atEnd);
    xmlParseError = evtHandler->getParseError();
  }

  if (xmlParseError < 0) {
    return IDataModel::NullPointer();
  }
  return m_DataModel;
}


#endif /* _XMLStreamReaderDelegate_hpp_  */
