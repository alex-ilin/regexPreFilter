// WinMergeScript.cpp : Implementation of CWinMergeScript
#include "stdafx.h"
#include "TestRegex.h"
#include "WinMergeScript.h"
#include "boost/regex.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "regexfilter.h"
using namespace std;



std::vector<REGEXVARIANT *> s_regexStore;

int main(int argc)
{

   std::wstring          input = L"\
   __in     if(param!=false)\n\
        {";

   std::wistringstream   instrm(input);
   std::wstring          output;
   std::wostringstream   outstrm;
   bool                 ruleFound;
   
   bool res = filterInput(&s_regexStore, instrm, output );
   std::wcout << output << std::endl;
   
   getchar();

   return 0;
}


