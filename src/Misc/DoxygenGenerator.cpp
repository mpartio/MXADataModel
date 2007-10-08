#include <Headers/LogTime.h>
#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <sstream>


  /**
  * @brief Parses the input string
  * @param input The string to parse.
  * @return error condition
  */
  int parse(std::string input);

 /**
 * @brief General tool to strip spaces from both ends:
 * @param s String to trim
 * @return New trimmed string
 */
 std::string trim(const std::string& s) 
{
  if(s.length() == 0)
    return s;
  int b = s.find_first_not_of(" \t");
  int e = s.find_last_not_of(" \t");
  if(b == -1) // No non-spaces
    return "";
  return std::string(s, b, e - b + 1);
}


/**
* @brief prints the output
* @param indent The number of spaces to use as a 'soft tab'
* @param params The params
* @param retToken token for return
*/
void doOutput(std::string indent, std::list<std::string> params, std::string retToken)
{
  params.push_front(indent + "* @brief ");
  params.push_front(indent + "/**");
  if (retToken.empty() == false) {
    params.push_back(indent + retToken);
  }
  params.push_back(indent + "*/");
  for (std::list<std::string>::iterator iter = params.begin(); iter != params.end(); ++iter ) {
    std::cout << *iter << std::endl;
  }

}
 
 

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int parse(std::string input) {
  std::list<std::string> tokens;
  std::string retToken;
  
  //Trim spaces from the beginning and end of the string
  int numSpace = input.find_first_not_of(' ');
  std::string indent = input.substr(0, numSpace);
  input = trim(input);
  std::string::size_type pos = 0;
  std::string::size_type NOTFOUND = std::string::npos;
  //find the semicolon and peel it off if necessary
  pos = input.find_last_of(';');
  if (pos != NOTFOUND)
  {
    input = input.substr(0, pos);
  }
  //check for static keyword
  if ( input.find("static") == 0 ) {
    pos = input.find_first_of(' ');
    input = input.substr(pos +1);
  }
  //Check for virtual keyword
  if ( input.find("virtual") == 0 ) {
    pos = input.find_first_of(' ');
    input = input.substr(pos +1);
  }
    
  //----------- Print the string out
#if 0
  std::stringstream ss;
  ss.setf(std::ios::fixed);
  ss.fill(' ');
  
  int i = 0;
  for (std::string::iterator iter = input.begin(); iter != input.end(); ++iter ) {
    std::cout << "  " << *iter ;
    ss  << std::setw(3) << i++;
  }
  std::cout << std::endl;
  std::cout << ss.str() << std::endl;
#endif
  //------------- Continue processing the input string
  
  // Now get the return type
  pos = input.find_first_of(' ');
  if (input.substr(0, pos) != "void")
  {
    retToken = "* @return ";
  }
  
  std::string::size_type parenStart = 0;
  std::string::size_type parenEnd = 0;
  // Now find the beginning of the argument list
  parenStart = input.find_first_of('(', pos + 1);
  parenEnd = input.find_last_of(')');
  pos = input.find_first_not_of(' ', parenStart + 1);
  if (pos == std::string::npos || pos == parenEnd) // No Arguments
  {
    doOutput(indent, tokens, retToken);
    return 0;
  }
  // We have arguments, so start parsing them
  pos = input.find_first_of(',', parenStart);
  if ( pos == std::string::npos) // Only one argument
  {
    //Find where the argument stops
    std::string::size_type argEnd = input.find_last_not_of(' ', parenEnd);
    std::string::size_type argBegin = input.find_last_of(' ', argEnd-1) + 1;
    if (input[argBegin] == '&' || input[argBegin] == '*')
    {
      argBegin++;
    }
    std::string arg = input.substr(argBegin, argEnd - argBegin);
    tokens.push_front(indent + "* @param " + arg);
    doOutput(indent, tokens, retToken);
    return 0;
  }
  bool done = false;
  // More than one argument
  //Find where the argument stops
  while (pos != NOTFOUND )
  {
    std::string::size_type argEnd = input.find_last_not_of(' ', pos-1);
    std::string::size_type argBegin = input.find_last_of(' ', argEnd-1) + 1;
    if (input[argBegin] == '&' || input[argBegin] == '*')
    {
      argBegin++;
    }
    std::string arg = input.substr(argBegin, argEnd - argBegin + 1);
    tokens.push_back(indent + "* @param " + arg);
    pos = input.find_first_of(',', pos +1);
    if (done) break;
    if (pos == NOTFOUND )
    {
      pos = input.find_first_of(')', argEnd );
      done = true;
    }
  }

  doOutput(indent, tokens, retToken);
  return 0;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {

  std::string input (argv[1]);
  parse(input);
  
 #if 0
 input = ("static herr_t writeDataset(hid_t loc_id ,    \
                     const std::string&  dsetName,  \
                     signed int  &rank ,\
                     hsize_t* *dims , \
                     T* data ) ");
 // parse(input);
 #endif 
                   std::cout << input;
  return 0;
}
