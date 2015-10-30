#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include "regexfilter.h"



using namespace std;
using namespace boost;
using namespace std;
using namespace boost;
REGEXMATCH what;

int after_context = 0;
int before_context = 0;
bool print_byte_offset = true;
bool count_only = false;
std::wstring pattern;
std::string current_file;
int file_count = 0;
int debug = 0;
time_t saved_mtime = 0;





bool process_stream(std::vector<REGEXVARIANT *> *regexStore, std::wistream& is, std::wostringstream& os)
{
   bool processed = false;
   std::wstring line;
   int match_found = 0;
   int linenum = 1;

   while(std::getline(is, line))
   {     
     for (unsigned int i = 0; i < regexStore->size(); i++)
     {
       int startpos = 0;
       line = REGEX_REPLACE(line , *(*regexStore)[i], L"");
       processed = true;

     }
     os << line;
     if (!is.eof())
     {
       os << endl;
     }
   }


   return processed;
}


bool readRules(std::string *fileName, std::vector<REGEXVARIANT *> *regexStore)
{
  //MessageBoxA(NULL, fileName->c_str(), NULL, 0);
  struct stat b;  
  wifstream inFile(fileName->c_str());
  wstring   line;
  int       linenum = 0;
  bool      rulefound = false;

  if (!stat(fileName->c_str(), &b)) {
    if ((saved_mtime == b.st_mtime) && (current_file.compare(*fileName) == 0))
    {
      return true;
    }
    cleanRules(regexStore);
    saved_mtime = b.st_mtime;
    current_file.assign(*fileName);
  }

  while (getline (inFile, line))
  {
    linenum++;    
    wistringstream linestream(line);
    wstring item;

    if (getline (linestream, item))
    {

      REGEXVARIANT *re = new REGEXVARIANT;
      if (!re)
      {
        inFile.close();
        return false;
      }

#ifdef USE_XPRESSIVE
      *re = boost::xpressive::wsregex::compile(item);
#else
      // assign the rule
      re->assign(item, boost::regex_constants::extended);
#endif
      
      // insert to store
      (*regexStore).push_back(re);
      
      rulefound = true;
    }
  }
  inFile.close();

  return rulefound;
}


void cleanRules(std::vector<REGEXVARIANT *> *regexStore)
{
  while (!regexStore->empty())
  {
    REGEXVARIANT *Item = regexStore->back();
    regexStore->pop_back();
    delete Item;
  }
  regexStore->clear();

}


bool filterInput(std::string *rulesFileName, std::vector<REGEXVARIANT *> *regexStore, std::wistream  &instrm, std::wstring &output)
{   
   bool                 ruleFound;
   bool                 processed;
   std::wostringstream  outstrm;
   
   // read the rules from file
   ruleFound = readRules(rulesFileName, regexStore);

   if (!ruleFound)
     return false;

   // process the input
   processed = process_stream(regexStore, instrm, outstrm);

   if (processed)
   {
     output = outstrm.str();
   }

   return processed;
}
