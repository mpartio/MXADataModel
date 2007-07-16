#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include <iostream>
#include <vector>


class A
{
public:
     ~A()
     {
      std::cout << "Destructing" << std::endl; 
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
  //A(){};
  boost::weak_ptr<A> _weakPtr;
  boost::weak_ptr<A> _parent;
  std::vector<boost::shared_ptr<A> > _children;
private:

  
};


int main (int argc, char const* argv[])
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
