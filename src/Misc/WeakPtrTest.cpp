#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/iostreams/device/file.hpp>
#include <boost/array.hpp>

class A
{
public:
  A(){ std::cout << "A Constructing" << std::endl; };

     ~A()
     {
      std::cout << "Destructing A" << std::endl;
     }


  static boost::shared_ptr<A> New()
  {
    boost::shared_ptr<A> a(new A());
    a->setWeakPointer(boost::weak_ptr<A>(a));
    std::cout << "Use Count During New: " << a.use_count() << std::endl;
    return a;
  }


  void setWeakPointer(boost::weak_ptr<A> ptr)
  {
    _weakPtr = ptr;
    std::cout << "setWeakPointer _weakPtr use count: " << _weakPtr.use_count() << std::endl;
  }

  void addChild(boost::shared_ptr<A> ptr)
  {
    std::cout << "AddChild:" << std::endl;
    ptr->setParent(this->_weakPtr);
    _children.push_back(ptr);
    std::cout << "Child Use Count: " << ptr.use_count() << std::endl;
    std::cout << "Parent Use Count: " << this->_weakPtr.use_count() << std::endl;
  }

  void setParent(boost::weak_ptr<A> ptr)
  {
    this->_parent = ptr;
  }

  boost::weak_ptr<A> getParent()
  {
    return _parent;
  }

protected:
 
  boost::weak_ptr<A> _weakPtr;
  boost::weak_ptr<A> _parent;
  std::vector<boost::shared_ptr<A> > _children;
private:


};


int main2 (int argc, char const* argv[])
{

  boost::shared_ptr<A> child = A::New();
  {
    std::cout << ".... A going INTO Scope...." << std::endl;
    boost::shared_ptr<A> a = A::New();
    a->addChild(child);
    std::cout << "Child Use Count:" << child.use_count() << std::endl;
    boost::weak_ptr<A> parent = child->getParent();
    if ( parent.expired() )
    {
      std::cout << "Parent is invalid" << std::endl;
    } else {
      std::cout << "Parent is still valid" << std::endl;
    }
    std::cout << ".... A going out of Scope...." << std::endl;
  }
  std::cout << "Child Use Count:" << child.use_count() << std::endl;
  boost::weak_ptr<A> parent = child->getParent();
  if ( parent.expired() )
  {
    std::cout << "Parent is invalid" << std::endl;
  }  else {
      std::cout << "Parent is still valid" << std::endl;
    }
  return 0;
}



class B
{

public:
  B() { 
    this->_s = "Default";
     std::cout << _s << " Constructing " << std::endl; 
  }
  B(const std::string &s) :
   _s(s)
  { std::cout << _s << " Constructing " << std::endl; }
  virtual ~B() 
    { std::cout << _s << " Destructing " << std::endl; }
  
  std::string name() { return _s; }
private:
  std::string _s;

};




int main(int argc, char **argv) {

  //boost::shared_ptr<A> aPtr = A::New();
  {
    std::cout << "operator = with valid and default shared_ptr" << std::endl;
    B* b = new B("B");
    boost::shared_ptr<B> bPtr(b);
    boost::shared_ptr<B> test;

    std::cout << "  B is NOT Null: " << bPtr->name() << std::endl;
    if (test.get() == NULL) { std::cout << "  test is NULL:" << std::endl; }
    std::cout << "  test = bPtr;" << std::endl;
    test = bPtr;
    std::cout << "  test: " << test->name() << std::endl;

  }
    std::cout << "---------------------------------------------" << std::endl;
  {
    std::cout << "operator = with 2 non-default shared_ptr<B>" << std::endl;
    boost::shared_ptr<B> bPtr(new B("B") );
    boost::shared_ptr<B> test(new B("TEST") );

    std::cout << "  b is NOT Null: " << bPtr->name() << std::endl;
    std::cout << "  test is NOT Null: " << test->name() << std::endl;
    std::cout << "  test = bPtr;" << std::endl;
    test = bPtr;
    std::cout << "  test: " << test->name() << std::endl;
   
  }
 std::cout << "---------------------------------------------" << std::endl;
 {
    B* a = new B("A");
    B* b = new B("B");
    B* nullA = NULL;
    boost::shared_ptr<B> aPtr(a);
    boost::shared_ptr<B> bPtr(b);
   // boost::shared_ptr<B> cPtr = bPtr;
    boost::shared_ptr<B> dPtr(nullA);
    boost::shared_ptr<B> test;
    if (NULL == test.get() )
    {
      std::cout << "empty shared_ptr constructor is to have a NULL Pointer" << std::endl;
    }


    test = bPtr;
    if (NULL != test.get() ) 
    {
      std::cout << "test is NO Longer NULL: " << test->name() << std::endl; 
    }


    std::cout << "Swapping Pointers with a NULL wrapped Pointer " << std::endl;
    aPtr.swap(dPtr); // Just swaps the raw pointers between the 2 wrappers
    std::cout << "Resetting pointer with a NULL Pointer " << std::endl;
    dPtr.reset(nullA); // will now destruct A because the ref count will go to Zero
    
 // std::cout << "aPtr.reset(NULL) being called" << std::endl;
 // aPtr.reset(nullAPtr);

    std::cout << "bPtr.reset(NULL) being called" << std::endl;
    bPtr.reset(nullA);
  }

  

  std::cout << "DONE" << std::endl;
  return 0;
}

#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main4( int argc, char* argv[] )
{

  if ( argc != 2 )
  {
    std::cout << "Usage: file_size path\n";
    return 1;
  }

  std::cout << "sizeof(intmax_t) is " << sizeof(boost::intmax_t) << '\n';

  fs::path p( argv[1], fs::native );

  if ( !fs::exists( p ) )
  {
    std::cout << "not found: " << argv[1] << std::endl;
    return 1;
  }

  if ( !fs::is_regular( p ) )
  {
    std::cout << "not a regular file: " << argv[1] << std::endl;
    return 1;
  }
 
  std::cout << "size of " << argv[1] << " is " << fs::file_size( p )  << std::endl;
  return 0;
}

#if 0
//--- Convenience code -----------------
typedef boost::filesystem::path FilePath;
namespace FileSystem = boost::filesystem;
int main5( int argc, char* argv[] )
{

  std::string filesystempath("C:\\Workspace\\MXADataModel\\CMakeLists.txt");
  FileSystem::path p( filesystempath, FileSystem::native );

  // Make sure the file exists
  if ( !FileSystem::exists( p ) )
    {
      std::cout << filesystempath << " not found." << std::endl;
      return -1;
    }
  
  // Check to make sure the file is regular
  if ( !FileSystem::is_regular( p ) )
    {
      std::cout << filesystempath << "not a regular file." << std::endl;
      return -1;
    }
   
   // std::cout << "size of " << argv[1] << " is " << fs::file_size( p )  << std::endl;
    boost::intmax_t fileSize = FileSystem::file_size(p);
    std::cout << "File Size: " << fileSize << std::endl;

    boost::iostreams::file_source sourceFile (filesystempath, BOOST_IOS::in);
    if (sourceFile.is_open() == true )
    {
      boost::array<char, fileSize> contents;
      char* front = contents.front();
      
      sourceFile.close();
    }
 return 0;
}

#endif
