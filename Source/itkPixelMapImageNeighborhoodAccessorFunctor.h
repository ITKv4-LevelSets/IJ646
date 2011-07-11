/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPixelMapImageNeighborhoodAccessorFunctor.h,v $
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPixelMapImageNeighborhoodAccessorFunctor_h
#define __itkPixelMapImageNeighborhoodAccessorFunctor_h

#include "itkImageBoundaryCondition.h"
#include "itkNeighborhood.h"
#include "itkImageBase.h"
#include "itkNumericTraits.h"

namespace itk
{

/** \class PixelMapImageNeighborhoodAccessorFunctor
 * \brief Provides accessor interfaces to Access pixels and is meant to be
 * used on pointers to pixels held by the Neighborhood class.
 *
 * A typical user should not need to use this class. The class is internally
 * used by the neighborhood iterators.
 *
 * \author Dan Mueller, Philips Healthcare, PII Development
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/10380/3068
 *
 */
template< class TImage >
class PixelMapImageNeighborhoodAccessorFunctor
{
public:
  typedef TImage                                           ImageType;
  typedef typename ImageType::PixelType                    PixelType;
  typedef typename ImageType::InternalPixelType            InternalPixelType;
  typedef typename ImageType::OffsetType                   OffsetType;
  typedef typename ImageType::PixelContainer::PixelMapType PixelMapType;
  typedef unsigned int                                     VectorLengthType;

  typedef Neighborhood< InternalPixelType *,
          ::itk::GetImageDimension< TImage >::ImageDimension> NeighborhoodType;

  typedef ImageBoundaryCondition< ImageType > const
                          *ImageBoundaryConditionConstPointerType;

  PixelMapImageNeighborhoodAccessorFunctor( PixelMapType* map,
                                            PixelType fillBufferValue )
    : m_PixelMap( map ),
      m_FillBufferValue( fillBufferValue ),
      m_Begin( 0 )
  {}

  PixelMapImageNeighborhoodAccessorFunctor()
    : m_PixelMap( NULL ),
      m_FillBufferValue( NumericTraits<PixelType>::Zero ),
      m_Begin( NULL )
  {}

  /** Set the pointer index to the start of the buffer.
   * This must be set by the iterators to the starting location of the buffer.
   * Typically a neighborhood iterator iterating on a neighborhood of an Image,
   * say \c image will set this in its constructor. For instance:
   *
   * \code
   * ConstNeighborhoodIterator( radius, image, )
   *   {
   *   ...
   *   m_NeighborhoodAccessorFunctor.SetBegin( image->GetBufferPointer() );
   *   }
   * \endcode
   */
  inline void SetBegin( const InternalPixelType * begin )  // NOTE: begin is always 0
    { this->m_Begin = const_cast< InternalPixelType * >( begin ); }

  /** Method to dereference a pixel pointer. This is used from the
   * ConstNeighborhoodIterator as the equivalent operation to (*it).
   * This method should be preferred over the former (*it) notation.
   * The reason is that dereferencing a pointer to a location of
   * VectorImage pixel involves a different operation that simply
   * dereferencing the pointer. Here a PixelType (array of InternalPixelType s)
   * is created on the stack and returned.  */
  inline PixelType Get( const InternalPixelType *pixelPointer ) const
    {
    OffsetValueType offset = pixelPointer  - m_Begin; // NOTE: begin is always 0
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

  /** Method to set the pixel value at a certain pixel pointer */
  inline void Set( InternalPixelType* &pixelPointer, const PixelType &p ) const
    {
    OffsetValueType offset = pixelPointer - m_Begin; // NOTE: begin is always 0
    m_PixelMap->operator[](offset) = p;
    }

  inline PixelType BoundaryCondition(
      const OffsetType& point_index,
      const OffsetType &boundary_offset,
      const NeighborhoodType *data,
      const ImageBoundaryConditionConstPointerType boundaryCondition) const
    {
    return boundaryCondition->operator()(point_index, boundary_offset, data, *this);
    }

  /** Required for some filters to compile. */
  void SetVectorLength( VectorLengthType length )
    {
    }

  /** Required for some filters to compile. */
  VectorLengthType SetVectorLength()
    {
    return 0;
    }

private:
  PixelMapType*       m_PixelMap;
  PixelType           m_FillBufferValue;
  InternalPixelType*  m_Begin;  // Begin of the buffer, always 0
};

} // end namespace itk
#endif
