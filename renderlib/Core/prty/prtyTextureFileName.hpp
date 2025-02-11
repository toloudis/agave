/****************************************************************************\
**	prtyTextureFileName.hpp
**
**		TextureFileName property
**
**
**
\****************************************************************************/
#ifdef PRTY_TEXTUREFILENAME_HPP
#error prtyTextureFileName.hpp multiply included
#endif
#define PRTY_TEXTUREFILENAME_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef PRTY_FILEPATH_HPP
#include "Core/prty/prtyFilePath.hpp"
#endif

#ifndef CH_DEFS_HPP
#include "Core/ch/chDefs.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

#include <string>

class prtyObject;

//============================================================================
// class that will allow callbacks to recognize both texture location
// changes and button presses
//============================================================================
class prtyTextureFileData
{
public:
  //--------------------------------------------------------------------
  //	constructor
  //--------------------------------------------------------------------
  prtyTextureFileData()
    : m_TextureLocator(fsLocator())
    , m_bButtonPressed(false)
    , m_CurrentCallback("Texture")
  {
  }
  prtyTextureFileData(const fsLocator& i_InitialValue)
    : m_TextureLocator(i_InitialValue)
    , m_bButtonPressed(false)
    , m_CurrentCallback("Texture")
  {
  }

  //--------------------------------------------------------------------
  //	operator overloads
  //--------------------------------------------------------------------
  bool operator==(const prtyTextureFileData& i_Value) const
  {
    return ((m_TextureLocator == i_Value.m_TextureLocator) && (m_bButtonPressed == i_Value.m_bButtonPressed)) &&
           (m_CurrentCallback == i_Value.m_CurrentCallback);
  }
  bool operator!=(const prtyTextureFileData& i_Value) const
  {
    return ((m_TextureLocator != i_Value.m_TextureLocator) || (m_bButtonPressed != i_Value.m_bButtonPressed)) ||
           (m_CurrentCallback != i_Value.m_CurrentCallback);
  }
  bool operator==(const fsLocator& i_Value) const { return m_TextureLocator == i_Value; }
  bool operator!=(const fsLocator& i_Value) const { return m_TextureLocator != i_Value; }

public:
  fsLocator m_TextureLocator;
  bool m_bButtonPressed;
  std::string m_CurrentCallback;
  shared_ptr<prtyObject> m_RampObject;
};

//============================================================================
//============================================================================
class prtyTextureFileName : public prtyPropertyTemplate<prtyTextureFileData, const prtyTextureFileData&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextureFileName();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextureFileName(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextureFileName(const std::string& i_Name, const fsLocator& i_InitialValue);

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
  //	override the GetValue command to return only the fsLocator
  //--------------------------------------------------------------------
  const fsLocator& GetValue() const;

  //--------------------------------------------------------------------
  //	get the actual value property, not just the fsLocator
  //--------------------------------------------------------------------
  const prtyTextureFileData& GetFullValue() const;
  const prtyTextureFileData& GetRevertValue() const;

  //--------------------------------------------------------------------
  // Set value of property
  //--------------------------------------------------------------------
  void SetValue(prtyTextureFileData i_Value, bool i_bDirty = false);
  void SetValueWithoutNotify(prtyTextureFileData i_Value);

  //--------------------------------------------------------------------
  // Set the revert value for the control
  //--------------------------------------------------------------------
  void SetRevertValue(prtyTextureFileData i_RevertValue);

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyTextureFileName& operator=(const prtyTextureFileName& i_Property);
  prtyTextureFileName& operator=(const prtyTextureFileData& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyTextureFileName& i_Property) const;
  bool operator!=(const prtyTextureFileName& i_Property) const;
  bool operator==(const prtyTextureFileData& i_Value) const;
  bool operator!=(const prtyTextureFileData& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void ReadTexture(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void WriteTexture(chWriter& io_Writer) const;

public:
  prtyTextureFileData m_RevertValue;
};
