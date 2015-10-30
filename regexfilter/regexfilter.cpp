#include <boost/regex.hpp>
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
boost::smatch what;

int after_context = 0;
int before_context = 0;
bool print_byte_offset = true;
bool count_only = false;
std::wstring pattern;
std::wstring current_file;
int file_count = 0;
int debug = 0;
time_t saved_mtime = 0;

char     fname[] = "c:\\regexrules.txt";



bool process_stream(std::map<std::wstring, boost::wregex *, ltstr> *regexStore, std::wistream& is, std::wostringstream& os)
{
   bool processed = false;
   std::wstring line;
   int match_found = 0;
   int linenum = 1;

   while(std::getline(is, line))
   {     
     for(std::map<std::wstring, boost::wregex *, ltstr>::iterator it = regexStore->begin(); it != regexStore->end(); ++it)
     {
       int startpos = 0;
       bool result;
       string sub_str ;
       std::wstring  str = it->first;
       boost::wregex *re = it->second;
       std::wostringstream   linestrm;

       line = regex_replace(line , *re, "");
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


bool readRules(std::map<std::wstring, boost::wregex *, ltstr> *regexStore)
{
  struct stat b;  
  wifstream inFile (fname);
  wstring   line;
  int       linenum = 0;
  bool      rulefound = false;

  if (!stat(fname, &b)) {
    if (saved_mtime == b.st_mtime)
    {
      return true;
    }
    cleanRules(regexStore);
    saved_mtime = b.st_mtime;
  }

  while (getline (inFile, line))
  {
    linenum++;    
    wistringstream linestream(line);
    wstring item;

    if (getline (linestream, item))
    {

      boost::wregex *re = new boost::wregex;
      if (!re)
      {
        inFile.close();
        return false;
      }

      // assign the rule
      re->assign(item, boost::regex_constants::extended);
      
      // insert to store
      (*regexStore)[item] = re;
      
      rulefound = true;
    }
  }
  inFile.close();

  return rulefound;
}

void cleanRules(std::map<std::wstring, boost::wregex *, ltstr> *regexStore)
{
 
  for(std::map<std::wstring, boost::wregex *, ltstr>::iterator it = regexStore->begin(); it != regexStore->end(); ++it)
  {
    boost::wregex *re = it->second;
    delete re;
  }

  regexStore->clear();

}


bool filterInput(std::map<std::wstring, boost::wregex *, ltstr> *regexStore, std::wistream  &instrm, std::wstring &output)
{   
   bool                 ruleFound;
   bool                 processed;
   std::wostringstream  outstrm;
   
   // read the rules from file
   ruleFound = readRules(regexStore);

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
