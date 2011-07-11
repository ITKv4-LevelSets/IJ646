/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPixelMapImageContainer,v $
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPixelMapImageContainer_h
#define __itkPixelMapImageContainer_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkIntTypes.h"
#include <utility>

namespace itk
{

/** \class PixelMapImageContainer
 *  \breif An image container for itk::PixelMapImage.
 *
 * \author Dan Mueller, Philips Healthcare, PII Development
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/10380/3068
 *
 */
template <typename TElementIdentifier, typename TElement>
class PixelMapImageContainer:  public Object
{
public:
  /** Standard class typedefs. */
  typedef PixelMapImageContainer Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Save the template parameters. */
  typedef TElementIdentifier  ElementIdentifier;
  typedef TElement            Element;
  typedef std::map< ElementIdentifier, TElement > PixelMapType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Standard part of every itk Object. */
  itkTypeMacro(PixelMapImageContainer, Object);

  /** Get the pixel map containing the pixels. */
  PixelMapType* GetPixelMap()
    { return &m_PixelMap; }

  /** Get the number of elements currently stored in the container. */
  IdentifierType Size(void) const
    { return static_cast< IdentifierType >( m_PixelMap.size() ); }

  /** Sparse image containers can not reserve memory in advance.
   *  This method does nothing.
   */
  void Reserve(void);

  /** Sparse image containers can not squeeze memory.
   *  This method does nothing.
   */
  void Squeeze(void);

  /** Tell the container to release any of its allocated memory. */
  void Initialize(void);

  /** These methods allow to define whether upon destruction of this class
   *  the memory buffer should be released or not.  Setting it to true
   *  (or ON) makes that this class will take care of memory release.
   *  Setting it to false (or OFF) will prevent the destructor from
   *  deleting the memory buffer. This is desirable only when the data
   *  is intended to be used by external applications.
   *  Note that the normal logic of this class set the value of the boolean
   *  flag. This may override your setting if you call this methods prematurely.
   *  \warning Improper use of these methods will result in memory leaks */
  itkSetMacro(ContainerManageMemory,bool);
  itkGetMacro(ContainerManageMemory,bool);
  itkBooleanMacro(ContainerManageMemory);

protected:
  PixelMapImageContainer();
  virtual ~PixelMapImageContainer();

  /** PrintSelf routine. Normally this is a protected internal method. It is
   * made public here so that Image can call this method.  Users should not
   * call this method but should call Print() instead. */
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  PixelMapImageContainer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PixelMapType         m_PixelMap;
  bool                 m_ContainerManageMemory;

};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_PixelMapImageContainer(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT PixelMapImageContainer< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef PixelMapImageContainer< ITK_TEMPLATE_2 x > PixelMapImageContainer##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/PixelMapImageContainer+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkPixelMapImageContainer.txx"
#endif

#endif
