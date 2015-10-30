/////////////////////////////////////////////////////////////////////////////
//
//    regexUnpacker:  a plugin for WinMerge to display XML files
//    Copyright (C) 2005  Christian List
//    Author: Christian List
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
/////////////////////////////////////////////////////////////////////////////

// RCS ID line follows -- this is updated by CVS
// $Id: WinMergeScript.cpp 5855 2008-08-24 15:49:22Z kimmov $

#include "stdafx.h"
#include <stdio.h>
#include "regexUnpacker.h"
#include "WinMergeScript.h"
#include "boost/regex.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


#include "boost/regex.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "regexfilter.h"

std::map<std::wstring, boost::wregex *, ltstr> regexStore;
/////////////////////////////////////////////////////////////////////////////
// CWinMergeScript


STDMETHODIMP CWinMergeScript::get_PluginEvent(BSTR *pVal)
{
	*pVal = SysAllocString(L"FILE_PACK_UNPACK");
	return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginDescription(BSTR *pVal)
{
	*pVal = SysAllocString(L"Transform an xml file so WinMerge can display it - save will not transform back");
	return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginFileFilters(BSTR *pVal)
{
	// xml - XML files
	// xsd - XML schema files
	// xsl - XML style sheet, xslt-fo, whatever
	// xslt - XML style sheet
	// svg - Scalable Vector graphics
	// wsf - Windows Scripting Host files
	*pVal = SysAllocString(L"\\.c;\\.cpp;\\.h;\\.h;\\.cc;\\.inl");
	return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginIsAutomatic(VARIANT_BOOL *pVal)
{
	*pVal = VARIANT_TRUE;
	return S_OK;
}


STDMETHODIMP CWinMergeScript::UnpackBufferA(SAFEARRAY **pBuffer, INT *pSize, VARIANT_BOOL *pbChanged, INT *pSubcode, VARIANT_BOOL *pbSuccess)
{
	// We don't need it
	return S_OK;
}

STDMETHODIMP CWinMergeScript::PackBufferA(SAFEARRAY **pBuffer, INT *pSize, VARIANT_BOOL *pbChanged, INT subcode, VARIANT_BOOL *pbSuccess)
{
	// We don't need it
	return S_OK;
}



STDMETHODIMP CWinMergeScript::UnpackFile(BSTR fileSrc, BSTR fileDst, VARIANT_BOOL *pbChanged, INT *pSubcode, VARIANT_BOOL *pbSuccess)
{
	USES_CONVERSION;


  std::wifstream inFile (fileSrc);
  std::wofstream onFile (fileDst);

  // get length of file:
  inFile.seekg (0, std::ios::end);
  unsigned int length = inFile.tellg();
  inFile.seekg (0, std::ios::beg);

  // read the rules from file
  bool ruleFound = readRules(&regexStore);

  if (!ruleFound )
  {
    return S_OK;
  }


  std::wstring filteredline;

  bool res = filterInput(&regexStore, inFile, filteredline );    

  if (res)
  {
    onFile << filteredline ;
  }
  else
  {
    return S_FALSE;
  }

	*pbChanged = VARIANT_TRUE;
	*pbSuccess = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CWinMergeScript::PackFile(BSTR fileSrc, BSTR fileDst, VARIANT_BOOL *pbChanged, INT pSubcode, VARIANT_BOOL *pbSuccess)
{
	// always return error so the users knows we can not repack
	*pbChanged = VARIANT_FALSE;
	*pbSuccess = VARIANT_FALSE;
	return S_OK;
}
