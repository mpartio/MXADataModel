#include <string>
#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>




// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class A
{
  public:
      explicit A(int i):
        _i(i)
        {}
      virtual ~A() {}
     
      
      int getI() { return _i;}
      void print(std::ostream &out) { out << " i = " << _i << std::endl; }
  private:
    int _i;
};


typedef boost::shared_ptr<A> APtr;
typedef std::vector<APtr>    AVec;

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
signed int move(int from, int to, AVec &avec)
{
  if (from < 0 || static_cast<AVec::size_type>(from) >= avec.size() ||
      to < 0 || static_cast<AVec::size_type>(to) >= avec.size() )
   {
     std::cout << "Tried to move data dimension from index " << from <<
     " to index " << to << ". The valid range is 0 to " << avec.size() - 1 << std::endl;
     return -1;
   }
  if (from == to)
  {
    return 1;
  }
    APtr a100 = avec[from];
  //  std::cout << "Use Count a100: " << a100.use_count() << std::endl;
    avec.erase(avec.begin() + from);
  //  std::cout << "Use Count a100: " << a100.use_count() << std::endl;
    avec.insert(avec.begin() + to, a100);
 //   std::cout << "Use Count a100: " << a100.use_count() << std::endl;
    std::cout << " --------- MOVE --------- " << std::endl;
    int index = 0;
    for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
      std::cout << index << " ";
      (*(iter))->print(std::cout);
      ++index;
    }
    std::cout << " ------------------------- " << std::endl;
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
signed int swap(int from, int to, AVec &avec)
{
  if (from < 0 || static_cast<AVec::size_type>(from) >= avec.size() ||
      to < 0 || static_cast<AVec::size_type>(to) >= avec.size() )
   {
     std::cout << "Tried to swap data dimensions " << from <<
     " and " << to << ". The valid range is 0 to " << avec.size() - 1 << std::endl;
     return -1;
   }
  
  if (from == to)
  {
    return 1;
  }
  
  APtr src = avec[from];
  APtr dest = avec[to];
  
  avec[from] = dest;
  avec[to] = src;
  
  std::cout << " -------- SWAP -------- " << std::endl;
  A* a1 = NULL;
  int index = 0;
  for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
    std::cout << index << " ";
  a1 = (*(iter)).get();
  if (NULL == a1)
  {
   std::cout << " NULL " << std::endl; 
  }
  else {
    (*(iter))->print(std::cout);
  }
    ++index;
  }
  std::cout << " ------------------------- " << std::endl;
  
  
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
signed int insert(APtr a, AVec::size_type index, AVec &avec)
{
  AVec::size_type size = avec.size();
  if ( index >= avec.size()) 
  {
    avec.resize(index);
  }
  
  AVec::iterator iter = avec.begin() + index;
  avec.insert(iter, a);
  size = avec.size();
  std::cout << " -------- INSERT -------- " << std::endl;
  index = 0;
  A* a1 = NULL;
  for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
    std::cout << index << " ";
    a1 = (*(iter)).get();
    if (NULL == a1)
    {
     std::cout << " NULL " << std::endl; 
    }
    else {
      (*(iter))->print(std::cout);
    }
    ++index;
  }
  std::cout << " ------------------------- " << std::endl;

  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
signed int squeeze(AVec &avec)
{
  for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {
    
    if ( (*iter).get() == NULL)
    {
      avec.erase(iter);
      iter = avec.begin();
    }
  }
  std::cout << " -------- SQUEEZE -------- " << std::endl;
  signed int index = 0;
  A* a1 = NULL;
  for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
    std::cout << index << " ";
    a1 = (*(iter)).get();
    if (NULL == a1)
    {
     std::cout << " NULL " << std::endl; 
    }
    else {
      (*(iter))->print(std::cout);
    }
    ++index;
  }
  std::cout << " ------------------------- " << std::endl;
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
 
  AVec avec;
#if 0
  for (int i = 0; i < 8; ++i) {
    A* a1 = new A(i);
    APtr aPtr(a1);
    avec.push_back(aPtr);
  }
  int index = 0;
//  for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {
//    std::cout << index << " ";
//    (*(iter))->print(std::cout);
//    ++index;
//  }
  
  //Now insert another A into position 5
  AVec::iterator iter = avec.begin();
  iter +=4;
  {
    A* a1 = new A(100);
    APtr aPtr(a1);
    avec.insert(iter, aPtr);
    std::cout << " ------------------------- " << std::endl;
    index = 0;
    for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
      std::cout << index << " ";
      (*(iter))->print(std::cout);
      ++index;
    }
  }
  

  move (4, 1, avec);
  move (4, 4, avec);
  move (1, 4, avec);
  move (4, 10, avec);
  move (4, -1, avec);
  move (-1, 4, avec);
  move (10, 4, avec);
  move (-1, 10, avec);
  move (10, 100, avec);
  move (-1, -10, avec);
  
std::cout << "-- TESTING SWAP FUNCTION --" << std::endl;
  swap (4, 1, avec);
  swap (1, 4, avec);
  swap (1, 1, avec);
  swap (4, 10, avec);
  swap (4, -1, avec);
  swap (-1, 4, avec);
  swap (10, 4, avec);
  swap (-1, 10, avec);
  swap (10, 100, avec);
  swap (-1, -10, avec);
  
#endif
  {
    A* a1 = new A(999);
    APtr aPtr(a1);
    insert(aPtr, 5, avec);
    swap (5, 1, avec);
    swap (5, 4, avec);
    insert(aPtr, 10, avec);
    insert (aPtr, avec.size(), avec);
    avec.push_back(aPtr);
    std::cout << " -------- ADD -------- " << std::endl;
    signed int index = 0;
    a1 = NULL;
    for (AVec::iterator iter = avec.begin(); iter != avec.end(); ++iter ) {\
      std::cout << index << " ";
      a1 = (*(iter)).get();
      if (NULL == a1)
      {
       std::cout << " NULL " << std::endl; 
      }
      else {
        (*(iter))->print(std::cout);
      }
      ++index;
    }
    squeeze(avec);
  }
  std::cout << "DONE" << std::endl;
	return 1;
}
