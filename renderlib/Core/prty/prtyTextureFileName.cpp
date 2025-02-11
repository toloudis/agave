/****************************************************************************\
**	prtyTextureFileName.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyTextureFileName.hpp"

#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/prty/prtyTextureDataParser.hpp"
#include "Core/fs/fsFileUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextureFileName::prtyTextureFileName()
  : prtyPropertyTemplate("TextureFileName", prtyTextureFileData())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextureFileName::prtyTextureFileName(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, prtyTextureFileData())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextureFileName::prtyTextureFileName(const std::string& i_Name, const fsLocator& i_InitialValue)
  : prtyPropertyTemplate(i_Name, prtyTextureFileData(i_InitialValue))
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyTextureFileName::GetType()
{
  return "TextureFileName";
}

//--------------------------------------------------------------------
// Set path value by passing single string
//--------------------------------------------------------------------
void
prtyTextureFileName::SetString(const std::string& i_FilePath, bool i_bDirty) // UndoFlags i_Undoable )
{
  prtyTextureFileData path;
  fsFileUtil::ANSIFilenameToLocator(i_FilePath, path.m_TextureLocator);
  SetValue(path, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
// Get path as a single string
//--------------------------------------------------------------------
const std::string
prtyTextureFileName::GetString() const
{
  std::string path;
  fsFileUtil::LocatorToANSIFilename(m_Value.m_TextureLocator, path);
  return path;
}

//--------------------------------------------------------------------
//	override the GetValue command to return only the fsLocator
//--------------------------------------------------------------------
const fsLocator&
prtyTextureFileName::GetValue() const
{
  return m_Value.m_TextureLocator;
}

//--------------------------------------------------------------------
//	get the actual pair value
//--------------------------------------------------------------------
const prtyTextureFileData&
prtyTextureFileName::GetFullValue() const
{
  return m_Value;
}

//--------------------------------------------------------------------
//	get the actual pair value
//--------------------------------------------------------------------
const prtyTextureFileData&
prtyTextureFileName::GetRevertValue() const
{
  return m_RevertValue;
}

//----------------------------------------------------------------------------
// Set value of property. The boolean flag is true
//	if the change is coming from the user interface and therefore
//	should mark the document containing the property as dirty.
//----------------------------------------------------------------------------
void
prtyTextureFileName::SetValue(prtyTextureFileData i_Value, bool i_bDirty)
{
  if (m_Value != i_Value) {
    m_Value = i_Value;
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyTextureFileName::SetRevertValue(prtyTextureFileData i_RevertValue)
{
  if (m_RevertValue != i_RevertValue)
    m_RevertValue = i_RevertValue;
}

//--------------------------------------------------------------------
// Set value of property without notifying the callbacks
//--------------------------------------------------------------------
void
prtyTextureFileName::SetValueWithoutNotify(prtyTextureFileData i_Value)
{
  m_Value = i_Value;
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyTextureFileName&
prtyTextureFileName::operator=(const prtyTextureFileName& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetFullValue());
  m_RevertValue = i_Property.GetRevertValue();
  return *this;
}
prtyTextureFileName&
prtyTextureFileName::operator=(const prtyTextureFileData& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyTextureFileName::operator==(const prtyTextureFileName& i_Property) const
{
  return (m_Value == i_Property.GetFullValue());
}
bool
prtyTextureFileName::operator!=(const prtyTextureFileName& i_Property) const
{
  return (m_Value != i_Property.GetFullValue());
}
bool
prtyTextureFileName::operator==(const prtyTextureFileData& i_Value) const
{

  return ((m_Value.m_TextureLocator == i_Value.m_TextureLocator) &&
          (m_Value.m_bButtonPressed == i_Value.m_bButtonPressed));
}
bool
prtyTextureFileName::operator!=(const prtyTextureFileData& i_Value) const
{
  return ((m_Value.m_TextureLocator != i_Value.m_TextureLocator) ||
          (m_Value.m_bButtonPressed != i_Value.m_bButtonPressed));
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTextureFileName::Read(chReader& io_Reader)
{
  // parse the chunk related to this control and set the value
  // accordingly
  prtyTextureFileName temp;
  prtyTextureDataParser::ReadData(io_Reader, temp);
  SetValue(temp.m_Value);
  m_RevertValue = temp.m_Value;
}

//--------------------------------------------------------------------
// Read Texture will read in simple file path properties not within
// the main chunk of the control
//--------------------------------------------------------------------
// virtual
void
prtyTextureFileName::ReadTexture(chReader& io_Reader)
{
  prtyTextureFileData val;
  itString tex_filename;
  chChunkParserUtil::Read(io_Reader, tex_filename);
  fsFileUtil::UnicodeStringToLocator(tex_filename, val.m_TextureLocator);
  SetValue(val);
  m_RevertValue = val;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTextureFileName::Write(chWriter& io_Writer) const
{
  prtyTextureFileName temp;
  temp.SetValue(m_Value);

  // write the values of this control into the chunk
  prtyTextureDataParser::WriteData(io_Writer, temp);
}

//--------------------------------------------------------------------
// Write Texture will be used to continue to write out any texture
// files for the control.  This is a simple implementation
//--------------------------------------------------------------------
// virtual
void
prtyTextureFileName::WriteTexture(chWriter& io_Writer) const
{
  // write out the current texture file
  itString tex_filename;
  fsFileUtil::LocatorToUnicodeString(m_Value.m_TextureLocator, tex_filename);
  chChunkParserUtil::Write(io_Writer, tex_filename);
}
