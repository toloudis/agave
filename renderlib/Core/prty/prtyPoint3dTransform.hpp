/****************************************************************************\
**	prtyPoint3dTransform.hpp
**
**		Point3d property
**
**
**
\****************************************************************************/
#ifdef PRTY_POINT3DTRANSFORM_HPP
#error prtyPoint3dTransform.hpp multiply included
#endif
#define PRTY_POINT3DTRANSFORM_HPP

#ifndef PRTY_POINT3D_HPP
#include "Core/prty/prtyPoint3d.hpp"
#endif
#ifndef MA_MATRIX4X4_HPP
#include "Core/Ma/maMatrix4x4.hpp"
#endif

//============================================================================
//============================================================================
class prtyPoint3dTransform : public prtyPoint3d
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3dTransform();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3dTransform(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3dTransform(const std::string& i_Name, const maPoint3d& i_InitialValue);

  //----------------------------------------------------------------------------
  // Set value of property. The boolean flag is true
  //	if the change is coming from the user interface and therefore
  //	should mark the document containing the property as dirty.
  //----------------------------------------------------------------------------
  virtual void SetValue(const maPoint3d& i_Value, bool i_bDirty = false);

  //--------------------------------------------------------------------
  // Some prtyPoint3dTransform properties contain a transformation so that
  // you can set and get the position in world space or object space.
  // The traditional Get/SetValue functions would return the object
  // space properties and these function return the world space values.
  // Default implementation of these functions is to treat the
  // object and world space positions as the same value.
  //--------------------------------------------------------------------
  // virtual bool HasWorldSpace() const;
  virtual maPoint3d GetWorldSpaceValue() const;
  virtual void SetWorldSpaceValue(const maPoint3d& i_Value, bool i_bDirty = false);

  //--------------------------------------------------------------------
  // Set transformation matrix from object space to world space
  //--------------------------------------------------------------------
  void SetTransformation(const maMatrix4x4& i_Matrix);

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyPoint3d& operator=(const maPoint3d& i_Value);

private:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void set_object_space_value(const maPoint3d& i_Value, bool i_bDirty);

  // m_bIsWorldSpace keeps track of whether the last call that set the property's
  // value was a object space or world space position.
  bool m_bIsWorldSpace;
  maMatrix4x4 m_Matrix;      // matrix for transforming between spaces
  maPoint3d m_WorldPosition; // current world space position
};
