/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: PixelMapImageTest01.cxx,v $
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <sstream>

#include "itkImage.h"
#include "itkPixelMapImage.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkShiftScaleImageFilter.h"

int main(int argc, char * argv [])
{
  // Typedefs
  const unsigned int Dimension = 2;
  typedef unsigned short PixelType;
  typedef itk::PixelMapImage< PixelType, Dimension > PixelMapImageType;
  typedef itk::Image< PixelType, Dimension > ContiguousImageType;
  typedef itk::ImageRegionIterator< PixelMapImageType > SparseIteratorType;
  typedef itk::ImageRegionIterator< ContiguousImageType > ContiguousIteratorType;
  typedef itk::NeighborhoodIterator< PixelMapImageType > SparseNeighborhoodIteratorType;

  // Create image
  PixelType value = 123;
  PixelMapImageType::Pointer image = PixelMapImageType::New();
  PixelMapImageType::IndexType start;
  start.Fill( 0 );
  PixelMapImageType::SizeType size;
  size.Fill( 1000000 ); // try this with a "normal" image!
  PixelMapImageType::RegionType region( start, size );
  image->SetRegions( region );
  image->Allocate( );
  image->FillBuffer( value );

  // Create a sub region for operations
  PixelMapImageType::IndexType substart;
  substart.Fill( 100 );
  PixelMapImageType::SizeType subsize;
  subsize.Fill( 100 );
  PixelMapImageType::RegionType subregion;
  subregion.SetSize( subsize );
  subregion.SetIndex( substart );

  // Test Get/SetPixel
  PixelMapImageType::IndexType index0;
  index0.Fill( 100 );
  image->SetPixel( index0, 5 );
  PixelType pixel0 = image->GetPixel( index0 );
  if( pixel0 != 5 )
    {
    std::cerr << __LINE__ << std::endl;
    std::cerr << pixel0 << " != 5" << std::endl;
    return EXIT_FAILURE;
    }
  image->SetPixel( index0, value );
  pixel0 = image->GetPixel( index0 );
  if( pixel0 != value )
    {
    std::cerr << __LINE__ << std::endl;
    std::cerr << pixel0 << " != " << value << std::endl;
    return EXIT_FAILURE;
    }

  PixelMapImageType::IndexType index1;
  index1.Fill( 15 );
  PixelType pixel1 = image->GetPixel( index1 );
  if( pixel1 != value )
    {
    std::cerr << __LINE__ << std::endl;
    std::cerr << pixel1 << " != " << value << std::endl;
    return EXIT_FAILURE;
    }

  // Test using filter
  typedef itk::ShiftScaleImageFilter<PixelMapImageType, PixelMapImageType>
    ShiftScaleFilterType;
  ShiftScaleFilterType::Pointer filter = ShiftScaleFilterType::New();
  filter->SetInput( image );
  filter->SetShift( 10 );
  PixelMapImageType::Pointer output = filter->GetOutput();
  output->SetRequestedRegion( subregion );
  output->FillBuffer( 0 );
  filter->SetNumberOfThreads( 1 ); // IMPORTANT!!!
  filter->Update();

  // Iterate subregion of filter output
  SparseIteratorType it1( output, subregion );
  it1.GoToBegin();

  while( !it1.IsAtEnd() )
    {
    if( it1.Get() != (value + 10) )
      {
      std::cerr << __LINE__ << std::endl;
      std::cerr << it1.Get() << " != " << value + 10 << std::endl;
      return EXIT_FAILURE;
      }
    ++it1;
    }

  // Test const iterator
  PixelMapImageType::IndexType substart1;
  substart1.Fill( 50 );
  subregion.SetIndex( substart1 );
  SparseIteratorType it( output, subregion );
  it.GoToBegin();

  while( !it.IsAtEnd() )
    {
    PixelMapImageType::IndexType index = it.GetIndex();
    if ( index[0] >= 100 && index[1] >= 100 )
      {
      if( it.Get() != (value + 10) )
        {
        std::cerr << __LINE__ << std::endl;
        std::cerr << it1.Get() << " != " << value + 10 << std::endl;
        return EXIT_FAILURE;
        }
      }
    else
      {
      if( it.Get() != 0 )
        {
        std::cerr << __LINE__ << std::endl;
        std::cerr << it1.Get() << " != " << 0 << std::endl;
        return EXIT_FAILURE;
        }
      }
    ++it;
    }

  PixelMapImageType::SizeType radius;
  radius.Fill( 1 );
  SparseNeighborhoodIteratorType nit( radius, output, subregion );
  nit.GoToBegin();
  SparseNeighborhoodIteratorType::OffsetType offset1 = {{1,1}};

  while( !nit.IsAtEnd() )
    {
    PixelType p = nit.GetCenterPixel();
    PixelType p1 = nit.GetPixel( offset1 );
    if (p == p1)
      {
      // Do nothing, here to prevent warning about not using p and p1
      }
    ++nit;
    }

  // Test iterator
  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    it.Set( (it.Get()+1) * 10 );
    ++it;
    }

  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    PixelMapImageType::IndexType index = it.GetIndex();
    if ( index[0] >= 100 && index[1] >= 100 )
      {
      if( it.Get() != ((value+11) * 10) )
        {
        std::cerr << __LINE__ << std::endl;
        std::cerr << it1.Get() << " != " << (value+11) * 10 << std::endl;
        return EXIT_FAILURE;
        }
      }
    else
      {
      if( it.Get() != 10 )
        {
        std::cerr << __LINE__ << std::endl;
        std::cerr << it1.Get() << " != " << 10 << std::endl;
        return EXIT_FAILURE;
        }
      }
    ++it;
    }

  // Test fill buffer
  image->FillBuffer( 100 );
  PixelMapImageType::IndexType indexA;
  indexA.Fill( 100 );
  PixelType pixelA = image->GetPixel( indexA );
  if( pixelA != 100 )
    {
    std::cerr << __LINE__ << std::endl;
    std::cerr << pixelA << " != " << 100 << std::endl;
    return EXIT_FAILURE;
    }
  PixelMapImageType::IndexType indexB;
  indexB.Fill( 10000 );
  image->SetPixel( indexB, 5 );
  PixelType pixelB = image->GetPixel( indexB );
  if( pixelB != 5 )
    {
    std::cerr << __LINE__ << std::endl;
    std::cerr << pixelA << " != " << 100 << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

