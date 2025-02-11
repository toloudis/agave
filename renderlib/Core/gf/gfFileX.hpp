/****************************************************************************\
**  gfFileX.hpp
**
**      gfFileX.hpp defines the exception classes used by the gf package.
**
**
**
\****************************************************************************/

#ifdef GF_FILEX_HPP
#error gfFileX.hpp multiply included
#endif
#define GF_FILEX_HPP

#ifndef FS_FILEX_HPP
#include "Core/Fs/fsFileX.hpp"
#endif

//============================================================================
//============================================================================
class gfInvalidFileBinX : public fsFileExceptionX
{
public:
  //------------------------------------------------------------------------
  //	gfInvalidFileBinX is thrown when the header of a FileBin is invalid
  //------------------------------------------------------------------------
  gfInvalidFileBinX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//============================================================================
class gfInvalidTokenFileTxtX : public fsFileExceptionX
{
public:
  //------------------------------------------------------------------------
  //	gfInvalidTokenFileTxtX is thrown when errors are found reading
  //	tokens from an ascii file.
  //------------------------------------------------------------------------
  gfInvalidTokenFileTxtX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};
