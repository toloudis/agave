/*****************************************************************************
**  gfFileTxt.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/Gf/gfFileTxt.hpp"

#include "Core/Gf/gfFileX.hpp"
#include "Core/Gf/gfPaths.hpp"
#include "Core/Gf/gfFileTranslationMgr.hpp"

#include <ctype.h> // for isspace, etc.

//============================================================================
//============================================================================
namespace {

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline bool
is_whitespace(char i_Ch)
{
  // make colon whitespace to make "name:value" pairs easier
  return (isspace(i_Ch) || (i_Ch == ':'));
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool
should_break(char i_LastChar, char i_ThisChar, char i_NextChar)
{
  // alpha breaks unless last char was alpha or period or underscore
  if (isalpha(i_ThisChar) || (i_ThisChar == '_')) {
    if (!(isalnum(i_LastChar) || (i_LastChar == '.') || (i_LastChar == '_')))
      return true;
    else
      return false;
  }

  // number breaks unless last char was alnum, period or minus
  if (isdigit(i_ThisChar)) {
    if (!(isalnum(i_LastChar) || (i_LastChar == '.') || (i_LastChar == '-') || (i_LastChar == '_')))
      return true;
    else
      return false;
  }

  // handle eof case
  if (i_ThisChar == 0)
    return true;

  // space or newline always breaks
  // if( (isspace(i_ThisChar)) || (i_ThisChar == '\n') ) return true;
  if ((is_whitespace(i_ThisChar)) || (i_ThisChar == '\n'))
    return true;

  // punctuation besides certain values always breaks
  if (ispunct(i_ThisChar) && !((i_ThisChar == '.') || (i_ThisChar == '-') || (i_ThisChar == '=') ||
                               //			(i_ThisChar == ':') ||
                               (i_ThisChar == '_')))
    return true;

  // colon breaks unless last char was alphanum
  //	if( (i_ThisChar == ':') && !isalnum(i_LastChar) )
  //		return true;

  // equals breaks unless last char was punct
  if ((i_ThisChar == '=') && !ispunct(i_ThisChar))
    return true;

  //	period breaks unless in the middle of a word or number
  if (i_ThisChar == '.') {
    if (!(isdigit(i_LastChar) || isdigit(i_NextChar) || (isalpha(i_LastChar) && isalpha(i_NextChar))))
      return true;
    else
      return false;
  }

  // underscore breaks unless in the middle of a word
  if ((i_ThisChar == '_') && !(isalpha(i_LastChar) && isalpha(i_NextChar)))
    return true;

  // minus breaks unless last char was a space and next is a digit or period
  if ((i_ThisChar == '-') && !(isspace(i_LastChar) && (isdigit(i_NextChar) || i_NextChar == '.')))
    return true;

  return false;
}

}

//------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------
gfFileTxt::gfFileTxt(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess)
  : gfFile(i_Locator, i_DesiredAccess)
  , m_Indentation(0)
{
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
gfFileTxt::~gfFileTxt() {}

//------------------------------------------------------------------------
//	ReadToken
//
//		ReadToken reads characters until whitespace or delimiter
//  is found.  Comments are lines that begin with "#"
//  Quotes (") can allow strings which contain spaces,
//  quotes within quoted strings should be escaped '\'.
//		Returns true if characters were read.
//------------------------------------------------------------------------
bool
gfFileTxt::ReadToken(std::string& o_Token, int i_MaxChars, char i_bDelimiter, bool i_bComments)
{
  char c = 0;
  o_Token.resize(0);

  while (true) {
    //	Skip the white space...
    //
    do {
      if (!Read(sizeof(c), &c)) {
        return false;
      }

      if (c == i_bDelimiter)
        return false;
    } while (is_whitespace(c));

    //	If the first non-white space character is a # skip the rest of this
    //	line.
    //
    if (c == '#' && i_bComments) {
      //	Skip line, looking for a carriage return.
      //
      do {
        if (!Read(sizeof(c), &c)) {
          return false;
        }
      } while (c != '\n');

      if (c == i_bDelimiter)
        return false;

      continue;
    }

    break;
  }

  //	Read the token until we get to a break character...
  //
  char last, cur, next;

  if (c == '\"') {
    // special case for quotes
    // return everything inside the quotes (not including the quotes)
    // o_Token += c; // don't include '"'
    last = c;

    while (true) {
      if (!Read(sizeof(cur), &cur)) {
        // error, end of file before end of quote
        if (m_FileStream)
          throw gfInvalidTokenFileTxtX(m_FileStream->GetLocator());
        // else
        //	throw gfInvalidTokenFileTxtX(m_PakStream->GetName());
      }

      if (last == '\\') {
        // Escaped characters
        switch (cur) {
          case 'a':
            o_Token += '\a';
            break;
          case 'b':
            o_Token += '\b';
            break;
          case 'f':
            o_Token += '\f';
            break;
          case 'n':
            o_Token += '\n';
            break;
          case 'r':
            o_Token += '\r';
            break;
          case 't':
            o_Token += '\t';
            break;
          case 'v':
            o_Token += '\v';
            break;
          case '\\':
            o_Token += '\\';
            // we need to change the value of cur to something other than '\\', or else the parser
            // will think that the '\\' just read is acting as an escape character, when in fact
            // it was actually the second half of an escape character, and therefore shouldn't be used
            // to escape the next character read in, right now it doesn't matter what we replace cur
            // with, as long as its not '\\'
            cur = '\0';
            break;
          case '\'':
            o_Token += '\'';
            break;
          case '\"':
            o_Token += '\"';
            break;
          case '\?':
            o_Token += '\?';
            break;
          default:
            o_Token += cur;
            break;
        }
      } else {
        if (cur == '\n') {
          // error, new line before end of quote
          if (m_FileStream)
            throw gfInvalidTokenFileTxtX(m_FileStream->GetLocator());
          // else
          //	throw gfInvalidTokenFileTxtX(m_PakStream->GetName());
        } else if (cur == '\\') {
          last = cur;
          continue; // delay writing of backslash
        } else if (cur == '\"') {
          // got ending quote, don't add it to string
          return true;
        }
        o_Token += cur;

        // double check size against max characters.
        // can't interrupt token reading between quotes
        //
        if ((i_MaxChars > 0) && (o_Token.length() > i_MaxChars)) {
          // error, max characters exceeded before end of quote
          if (m_FileStream)
            throw gfInvalidTokenFileTxtX(m_FileStream->GetLocator());
          // else
          //	throw gfInvalidTokenFileTxtX(m_PakStream->GetName());
        }
      }
      last = cur;
    }
  }

  last = c;
  o_Token += c;

  if (!Read(sizeof(cur), &cur)) {
    // eof,
    return (!o_Token.empty());
  }

  while ((i_MaxChars <= 0) || (o_Token.length() <= i_MaxChars)) {
    if (!Read(sizeof(next), &next))
      next = 0;

    if (cur == i_bDelimiter) {
      SetFilePos(-2, fsFileStream::e_Current);
      return (!o_Token.empty());
    }

    if (should_break(last, cur, next)) {
      SetFilePos(-2, fsFileStream::e_Current);
      return (!o_Token.empty());
    }

    o_Token += cur;
    last = cur;
    cur = next;
  }

  return (!o_Token.empty());
}

//------------------------------------------------------------------------
//	WriteToken
//
//		WriteToken writes string so that it can be read with
//	later call to ReadToken.  If string contains spaces or
//	quotes, the whole string will be quoted.  Internal quotes
//  will be escaped.
//------------------------------------------------------------------------
void
gfFileTxt::WriteToken(std::string& i_Token)
{
  bool bNeedsQuotes = false;
  int len = i_Token.length();

  for (int i = 0; i < len; i++) {
    if (isspace(i_Token[i]) || (i_Token[i] == '\"') || (i_Token[i] == '\\')) {
      bNeedsQuotes = true;
      break;
    }
    DBG_ASSERT((i_Token[i] != '\n'), "Cannot have newlines in token.");
  }

  if (!bNeedsQuotes) {
    // Just write string directly
    Write(i_Token.length(), (void*)i_Token.c_str());
  } else {
    char q = '\"';
    char bs = '\\';
    char n = 'n';

    // Start quoted string
    Write(1, &q);

    // Write character at a time
    for (int i = 0; i < len; i++) {
      if ((i_Token[i] == '\"') || (i_Token[i] == '\\')) {
        Write(1, &bs);
      }

      if (i_Token[i] == '\n') {
        Write(1, &bs);
        Write(1, &n);
        continue;
      }

      Write(1, &(i_Token[i]));
    }

    // End quoted string
    Write(1, &q);
  }
}

//--------------------------------------------------------------------
//	IncrementIndentation
//--------------------------------------------------------------------
void
gfFileTxt::IncrementIndentation()
{
  m_Indentation++;
}

//--------------------------------------------------------------------
//	DecrementIndentation
//--------------------------------------------------------------------
void
gfFileTxt::DecrementIndentation()
{
  m_Indentation--;
  DBG_ASSERT(m_Indentation >= 0, "Indentation Level less than zero.");
}

//--------------------------------------------------------------------
//	GetIndentation
//--------------------------------------------------------------------
int
gfFileTxt::GetIndentation()
{
  return m_Indentation;
}

//--------------------------------------------------------------------
//	WriteIndentation
//--------------------------------------------------------------------
void
gfFileTxt::WriteIndentation()
{
  char c('\t');

  for (int i = 0; i < m_Indentation; i++) {
    this->Write(sizeof(c), &c);
  }
}

//------------------------------------------------------------------------
//	ReadLine
//
//		ReadLine reads characters until an end of line is reached
//	(or end of file).
//
//		Returns true if characters were read.
//------------------------------------------------------------------------
bool
gfFileTxt::ReadLine(std::string& o_Line, int i_MaxChars)
{
  o_Line.resize(0);

  //	Read the token until we get to a break character...
  //
  char cur;

  while ((i_MaxChars <= 0) || (o_Line.length() <= i_MaxChars)) {
    if (!Read(sizeof(cur), &cur)) {
      // eof
      return (!o_Line.empty());
    }

    // handle carriage return.
    //
    if ((cur == 13)) {
      //	handle the newline
      if (!Read(sizeof(cur), &cur)) {
        // eof
        return (!o_Line.empty());
      }

      if (cur != 10) {
        SetFilePos(-1, fsFileStream::e_Current);
      }
      return (!o_Line.empty());
    } else if (cur == 10) {
      // Other types of text files use a single character 10 to end a line
      return (!o_Line.empty());
    }

    o_Line += cur;
  }

  return (!o_Line.empty());
}

//------------------------------------------------------------------------
//	WriteLine
//
//		WriteLine writes a string.
//------------------------------------------------------------------------
void
gfFileTxt::WriteLine(const std::string& i_Line)
{
  Write(i_Line.length(), (void*)i_Line.c_str());
}

//------------------------------------------------------------------------
//	Read reads i_NumBytes into the buffer.  If the file is too short
//	to read	i_NumBytes, it will read as many as it can.  The return
//	value is the number actually read into the o_Buffer.  This function
//	also advances the file pointer.
//------------------------------------------------------------------------
int
gfFileTxt::Read(envType::Int64 i_NumBytes, void* o_Buffer)
{
  return m_FileStream->Read(i_NumBytes, o_Buffer);
}

//------------------------------------------------------------------------
//	Write writes i_NumBytes from o_Buffer into the file.  The file
//	pointer will also be advanced.
//------------------------------------------------------------------------
void
gfFileTxt::Write(envType::Int64 i_NumBytes, const void* i_Buffer)
{
  DBG_ASSERT(m_FileStream != NULL, "Must have a fsFileStream to write");
  m_FileStream->Write(i_NumBytes, i_Buffer);
}
