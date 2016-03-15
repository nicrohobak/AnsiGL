#ifndef __ANSIGL_IMAGE_H__
#define __ANSIGL_IMAGE_H__

/*
 * image.h
 *
 * A content-based surface.
 */


#include "content.h"
#include "surface.h"


namespace AnsiGL
{
    class Image : public Content, public Surface
    {
    public:
	ANSIGL_POINTERS( Image )

    public:
	bool		TransparentSpaces;

    public:
	Image();
	Image( const Area2D &size );
	virtual ~Image();

	virtual const Area2D &Size() const;
	virtual const tSizeType &Width() const;
	virtual const tSizeType &Height() const;
	virtual void Width( tSizeType width );
	virtual void Height( tSizeType height );
	void Resize( const Area2D &size );

	virtual std::string str();
	virtual std::string Render() const;
	virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;
    };
}


#endif // __ANSIGL_IMAGE_H__




