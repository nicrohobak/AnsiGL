/*
 * sprite.cpp
 *
 * A "surface" with functions and rendering rules better suited for being meaningful content instead of being a simple container for content (like the Window lineage).
 */


#include "AnsiGL/sprite.h"


namespace AnsiGL
{
    Sprite::Sprite():
	Offset(Point(0,0))
    {
    }

    Sprite::Sprite( const Area2D &size ):
	Offset(Point(0,0))
    {
	Resize( size );
    }

    void Sprite::FlipX()
    {
	// TODO: Make sure glyphs get flipped as needed
    }

    void Sprite::FlipY()
    {
	// TODO: Make sure glyphs get flipped as needed
    }

    void Sprite::Rotate( int rotation )
    {
	// TODO: Make sure glyphs get flipped as needed
    }

    void Sprite::RenderToSurface( Surface::Ptr dest, const Point &pos, bool transparentSpaces )
    {
	Point NewPos;

	NewPos.X( pos.X() + Offset.X() );
	NewPos.Y( pos.Y() + Offset.Y() );

	Surface::RenderToSurface( dest, NewPos, transparentSpaces );
    }
}




