/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPixelMapImagePixelAccessor.h,v $
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPixelMapImagePixelAccessor_h
#define __itkPixelMapImagePixelAccessor_h

#include "itkMacro.h"
#include "itkNumericTraits.h"

namespace itk
{

/** \class PixelMapImagePixelAccessor
 * \brief Give access to partial aspects of a type
 *
 * PixelMapImagePixelAccessor is specifically meant to provide PixelMapImage
 * with the same \c DefaultPixelAccessor interface that
 * DefaultPixelAccessor provides to Image.
 *
 * PixelMapImagePixelAccessor is templated over an internal type and an
 * external type representation. This class encapsulates a
 * customized convertion between the internal and external
 * type representations.
 *
 * \author Dan Mueller, Philips Healthcare, PII Development
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/10380/3068
 */
template <class TType, class TPixelMapType>
class ITK_EXPORT PixelMapImagePixelAccessor
{
public:

 /** External typedef. It defines the external aspect
   * that this class will exhibit. */
  typedef TType ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data. */
  typedef TType InternalType;

  /** Typedef for pixel map. */
  typedef TPixelMapType PixelMapType;

  typedef typename PixelMapType::key_type PixelMapIdentifierType;

  /** Set output using the value in input */
  inline void Set(InternalType & output,
                  const ExternalType & input,
                  const PixelMapIdentifierType offset ) const
    {
    m_PixelMap->operator[](offset) = input;
    }

  /** Get the value from input */
  inline ExternalType Get( const InternalType & begin,
                           const PixelMapIdentifierType offset ) const
    {
    typename PixelMapType::const_iterator it = m_PixelMap->find( offset );
    if ( it == m_PixelMap->end() )
      {
      return m_FillBufferValue;
      }
    else
      {
      return it->second;
      }
    }

  PixelMapImagePixelAccessor() :
    m_PixelMap( NULL ),
    m_FillBufferValue( NumericTraits< InternalType >::Zero )
    {}

   /** Constructor to initialize slices and image size at construction time */
   PixelMapImagePixelAccessor( PixelMapType* pixelMap,
                               TType fillBufferValue ) :
     m_PixelMap( pixelMap ), m_FillBufferValue( fillBufferValue )
     {}

  virtual ~PixelMapImagePixelAccessor() {}

private:
  PixelMapType* m_PixelMap;
  InternalType  m_FillBufferValue;
};

} // end namespace itk


#endif
