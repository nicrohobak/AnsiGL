/*
 * image.cpp
 *
 * A content-based surface
 */


#include "AnsiGL/image.h"


namespace AnsiGL
{
	Image::Image():
		TransparentSpaces(false)
	{
	}

	Image::Image( const Area2D &size ):
		TransparentSpaces(false)
	{
		Image::Resize( size );
	}

	Image::~Image()
	{
	}

	const Area2D &Image::Size() const
	{
		return Content::_Size;
	}

	const tSizeType Image::Width() const
	{
		return Content::_Size.Width();
	}

	const tSizeType Image::Height() const
	{
		return Content::_Size.Height();
	}

	void Image::Width( tSizeType width )
	{
		Content::Width( width );
		Surface::Width( width );
	}

	void Image::Height( tSizeType height )
	{
		Content::Height( height );
		Surface::Height( height );
	}

	void Image::Resize( const Area2D &size )
	{
		Content::Resize( size );
		Surface::Resize( size );
	}

	void Image::FlipX()
	{
		// TODO: Make sure glyphs get flipped as needed
	}

	void Image::FlipY()
	{
		// TODO: Make sure glyphs get flipped as needed
	}

	void Image::Rotate( int rotation )
	{
		// TODO: Make sure glyphs get flipped as needed
	}

	std::string Image::str()
	{
		return Surface::str();
	}

	std::string Image::Render() const
	{
		return Surface::Render();
	}

	void Image::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		Surface::RenderToSurface( dest, pos + Offset, TransparentSpaces );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


