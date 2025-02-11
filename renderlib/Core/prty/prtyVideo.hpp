/****************************************************************************\
**	prtyVideo.hpp
**
**		Video property
**
**
**
\****************************************************************************/
#ifdef PRTY_VIDEO_HPP
#error prtyVideo.hpp multiply included
#endif
#define PRTY_VIDEO_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//============================================================================
class CVideoData
{
public:
  CVideoData();
  virtual ~CVideoData() {}

  //------------------------------------------------------------------------
  //	Assignment
  //------------------------------------------------------------------------
  const CVideoData& operator=(const CVideoData& i_CopyFrom);

  //------------------------------------------------------------------------
  //	Equality
  //------------------------------------------------------------------------
  bool operator==(const CVideoData& i_Data) const;
  bool operator!=(const CVideoData& i_Data) const;

  fsLocator m_Filename;
  int m_Frame;
  int m_Width, m_Height;
  void* m_pVideo; // can't store avi ptr here as it exists outside of libxlt
};

//============================================================================
//============================================================================
class prtyVideo : public prtyPropertyTemplate<CVideoData, const CVideoData&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVideo();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVideo(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVideo(const std::string& i_Name, const CVideoData& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  // Set path value by passing single string
  //--------------------------------------------------------------------
  void SetString(const std::string& i_FilePath, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  // Get path as a single string
  //--------------------------------------------------------------------
  const std::string GetString() const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyVideo& operator=(const prtyVideo& i_Property);
  prtyVideo& operator=(const CVideoData& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyVideo& i_Property) const;
  bool operator!=(const prtyVideo& i_Property) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
