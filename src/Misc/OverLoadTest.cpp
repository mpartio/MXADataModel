

#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class SuperClass
{
  public:
    SuperClass() {}
    virtual ~SuperClass() {}
    
    virtual int doSomething() = 0;
    virtual int writeData() = 0;
    virtual void* getVoidPointer() = 0;
  
  private:

    SuperClass(const SuperClass&);    // Copy Constructor Not Implemented
    void operator=(const SuperClass&);  // Operator '=' Not Implemented
  
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
class PointerDataTemplate : public SuperClass
{
  public:
    PointerDataTemplate(boost::shared_ptr<T> data) : _data(data) {};
    virtual ~PointerDataTemplate() {};
    
    virtual int doSomething()
    {
      std::cout << "doSomething" << std::endl;
      return -2;
    }
    
    virtual int writeData()
    {
      std::cout << "WriteData" << std::endl;
      return 1;
    }
    
    virtual void* getVoidPointer()
    {
      return static_cast<void*>(_data.get());
    }
    
    virtual boost::shared_ptr<T>   getValue()
    {
      return _data;
    }
    
  private:
    boost::shared_ptr<T> _data;
    PointerDataTemplate(const PointerDataTemplate&);    // Copy Constructor Not Implemented
      void operator=(const PointerDataTemplate&);  // Operator '=' Not Implemented
  
  
};

#if 0
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class DoubleArray : public PointerDataTemplate<double>
{
  
  public:
    DoubleArray() : _value(99.99) {};
    virtual ~DoubleArray() {};
    
    int doSomething() {std::cout << "doSomething<double>" << std::endl; return 1; }
    int doSomethingElse(double value) { this->_value = value; return -1; }
    double   getValue() { return _value; }
    int   writeData()
    {
      std::cout << "Writing Double..." << std::endl;
      return 1;
    }
    
  private:
    double _value;
    DoubleArray(const DoubleArray&);    // Copy Constructor Not Implemented
    void operator=(const DoubleArray&);  // Operator '=' Not Implemented
};
#endif

typedef PointerDataTemplate<double> DoubleArray;



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) 
{
//  DoubleArray da;
//  da.doSomething();
//  double val = da.getValue();
//  std::cout << "val: " << val << std::endl;
  double md[20];
  double* data = static_cast<double*>(malloc(60 * sizeof(double)));
  std::cout << "ptr: " << data << std::endl;
  boost::shared_ptr<double> dblPtr(data);
  SuperClass* sc = new DoubleArray(dblPtr);
  sc->writeData();
  double* dd = static_cast<double*>(sc->getVoidPointer());
  std::cout << "dd: " << dd << std::endl;
  
  
  return EXIT_SUCCESS;
}
