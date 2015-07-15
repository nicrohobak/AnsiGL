#ifndef __ANSIGL_SPRITE_H__
#define __ANSIGL_SPRITE_H__

/*
 * sprite.h
 *
 * A surface lineage with functions and rendering rules better suited for being meaningful content instead of being a simple container for content (like the Window lineage).
 */

#include "surface.h"


namespace AnsiGL
{
    class Sprite : public Surface
    {
    public:
	Point		Offset;		// Rendering offset from the origin

    public:
	Sprite();
	Sprite( const Area2D &size );

	void FlipX();
	void FlipY();

	void Rotate( int rotation );	// Rotates in 90 degree increments...negative is counter-clockwise, positive is clockwise

	virtual void RenderToSurface( Surface::Ptr dest, const Point &pos = Point(), bool transparentSpaces = false );
    };
}



#endif // __ANSIGL_SPRITE_H__




