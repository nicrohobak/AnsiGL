/*
 * surface.cpp
 *
 * A rendering context for ANSI-enabled ASCII "graphics".
 */


#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>

#include "AnsiGL/surface.h"


namespace AnsiGL
{
	Surface::Surface():
		AutoPalette(true),
		DesiredDepth(ColorDepth_8Bit),
		TabSize(8),
		RenderANSI(true),
		RenderBell(true),
		UseCLS(false),
		UseENDL(true),
		UseLF(false),
		UseCR(false)
	{
	}

	Surface::Surface( const Area2D &size ):
		AutoPalette(true),
		DesiredDepth(ColorDepth_8Bit),
		TabSize(8),
		RenderANSI(true),
		RenderBell(true),
		UseCLS(false),
		UseENDL(true),
		UseLF(false),
		UseCR(false)
	{
		Resize( size );
	}

	Surface::~Surface()
	{
	}

	Scanline Surface::GetScanline( int lineNum ) const
	{
		if ( lineNum < 0 || lineNum >= _Size.Height() )
			return Scanline();

		return _Pixels[lineNum];
	}

	const achar &Surface::GetPixel( const Point &pixel ) const
	{
		static achar ach;
		ach.Clear();

		if ( !HasPoint( pixel ) )
			return ach;

		const Pixel *CurPixel = &_Pixels[pixel.Y()][pixel.X()];

		ach.Glyph( CurPixel->Glyph() );
		ach.CopyAttributes( CurPixel->Attributes() );
		ach.Bell = CurPixel->Bell;

		if ( Palette && CurPixel->PaletteIndex >= 0 && CurPixel->PaletteIndex < (int)Palette->size() )
			ach.Color = (*((*Palette)[CurPixel->PaletteIndex]));

		return ach;
	}

	const Area &Surface::Size() const
	{
		return _Size;
	}

	void Surface::Resize( Area size )
	{
		Width( size.Width() );
		Height( size.Height() );
	}

	const tSizeType &Surface::Width() const
	{
		return _Size.Width();
	}

	void Surface::Width( tSizeType width )
	{
		if ( width < 0 )
			_Size.Width( 0 );
		else
			_Size.Width( width );

		if ( _Pixels.empty() )
			return;

		for ( int y = 0; y < _Size.Height(); ++y )
			_Pixels[y].resize( _Size.Width() );
	}

	const tSizeType &Surface::Height() const
	{
		return _Size.Height();
	}

	void Surface::Height( tSizeType height )
	{
		if ( height < 0 )
			_Size.Height( 0 );
		else
			_Size.Height( height );

		if ( _Size.Height() == 0 )
		{
			_Pixels.clear();
			return;
		}

		Scanline BlankLine( _Size.Width() );
		_Pixels.resize( _Size.Height(), BlankLine );
	}

	void Surface::Clear()
	{
		for ( tSizeType CurY = 0; CurY < _Size.Height(); ++CurY )
		{
			for ( tSizeType CurX = 0; CurX < _Size.Width(); ++CurX )
				_Pixels[ CurY ][ CurX ].Clear();
		}
	}

	int Surface::FindColorIndex( const ColorDef &color )
	{
		// Attempt to find the color in the palette first...it may contain required color already
		int Index = (Palette) ? (Palette->FindIndex( color )) : (-1);

		// If we didn't find it, and we're supposed to manage our own palette...then lets do that here
		if ( Index == -1 && AutoPalette )
		{
			if ( !Palette )
				Palette = ColorPalette::Ptr( new ColorPalette );

			Index = Palette->Add( ColorDef::Ptr( new ColorDef(color) ) );
		}

		return Index;
	}

	ColorDef::Ptr Surface::FindColorFromIndex( int index ) const
	{
		ColorDef::Ptr FoundColor;

		if ( !Palette || index < 0 || index > (int)Palette->size() )
			return FoundColor;

		return (*Palette)[index];
	}

	//
	// Render functions
	//
	std::string Surface::str()
	{
		bool RenderANSI_Save = RenderANSI;
		RenderANSI = false;
		std::string RetStr( Render() );
		RenderANSI = RenderANSI_Save;
		return RetStr;
	}

	std::string Surface::Render() const
	{
		std::stringstream RenderedStr("");

		if ( UseCLS )
		{
			if ( RenderANSI )
			{
				// TODO: Add a better way to print VT100 codes
				//RenderedStr << ANSI_CODE_START << "2J";	// 2J is the VT100 CLS code -- Though, CLS may not be what we want...
				RenderedStr << ANSI_CODE_START << "H";  // H is the VT100 code to return the cursor to the home position (top-left, 0,0)
			}
			else
			{
				for ( int i = 0; i < _Size.Height(); ++i )
					RenderedStr << std::endl;
			}
		}

		Point Cur;
		int CurColor = -1;

		// Render the screen line by line...
		for ( ; Cur.Y() < _Size.Height(); Cur.IncY() )
		{
			Cur.X( 0 );		// Make sure to reset our X position as we work through our scanlines

			for ( ; Cur.X() < _Size.Width(); Cur.IncX() )
			{
				const Pixel *CurPixel = &_Pixels[Cur.Y()][Cur.X()];

				if ( RenderBell && CurPixel->Bell )
					RenderedStr << '\007';

				if ( RenderANSI && Palette && (CurColor != CurPixel->PaletteIndex || Cur.X() == 0) )
				{
					CurColor = CurPixel->PaletteIndex;
					RenderedStr << pixelRenderAnsi( Cur );
				}

				if ( CurPixel->Glyph().empty() || CurPixel->IsSpace()
				        || (!CurPixel->Glyph().empty() && CurPixel->Glyph().c_str()[0] == '\0') )
					RenderedStr << ' ';
				else
					RenderedStr << CurPixel->Glyph();
			}

			// Make sure that our colors are closed
			if ( RenderANSI )
				RenderedStr << ANSI_CODE_START << ANSI_Default << ANSI_CODE_END;

			if ( UseENDL )
				RenderedStr << "\r\n";

			if ( UseLF )
				RenderedStr << '\n';

			if ( UseCR )
				RenderedStr << '\r';
		}

		return RenderedStr.str();
	}

	void Surface::RenderToSurface( Surface::Ptr dest, const Point2D &pos, bool transparentSpaces ) const
	{
		RenderAreaToSurface( FixedArea2D(Area2D(_Size.Width(), _Size.Height()), Point2D(0, 0)), dest, pos, transparentSpaces );
	}

	void Surface::RenderAreaToSurface( FixedArea2D visibleArea, Surface::Ptr dest, const Point2D &pos, bool transparentSpaces ) const
	{
		if ( !dest )
			return;

		// Check and make sure we have something visible to render
		if ( visibleArea.X() + visibleArea.Width() < 0 || visibleArea.X() > _Size.Width() )
			return;

		if ( visibleArea.Y() + visibleArea.Height() < 0 || visibleArea.Y() > _Size.Height() )
			return;

		// And check and make sure there's a place to put it
		if ( pos.X() + visibleArea.Width() < 0 || pos.X() > dest->Width() )
			return;

		if ( pos.Y() + visibleArea.Height() < 0 || pos.Y() > dest->Height() )
			return;

		// Determine our destination area
		FixedArea2D Destination( Area2D(visibleArea.Width(), visibleArea.Height()), pos );

		// Now that we've got the desired target on our destination surface, make sure it's visible and that we have something to render
		if ( Destination.Point().X() + Destination.Width() < 0 || Destination.Point().X() > dest->_Size.Width() )
			return;

		const int PaletteSize = Palette ? (int)Palette->size() : 0;
		int *Map_PrevIndexNewIndex = new int[ PaletteSize ];

		if ( Palette )
		{
			ColorPalette::const_iterator CurColor;
			int PrevIndex = 0;

			// For each color in our palette, we're adding to the destination palette and storing the appropriate index of that palette in our map
			for ( CurColor = Palette->begin(); CurColor != Palette->end(); ++CurColor, ++PrevIndex )
				Map_PrevIndexNewIndex[PrevIndex] = dest->FindColorIndex( *(*CurColor) );	// This conveniently works with either syntax
		}

		// Current points we're working with in our loop
		Point2D Dest;
		Point2D Src;

		for ( Src.Y( visibleArea.Point().Y() ), Dest.Y( Destination.Point().Y() ); Src.Y() < visibleArea.Point().Y() + visibleArea.Height(); Src.IncY(), Dest.IncY() )
		{
			if ( Src.Y() < 0 || Src.Y() >= _Size.Height() || Dest.Y() < 0 || Dest.Y() >= dest->_Size.Height() )
				continue;

			for ( Src.X( visibleArea.Point().X() ), Dest.X( Destination.Point().X() ); Src.X() < visibleArea.Point().X() + visibleArea.Width(); Src.IncX(), Dest.IncX() )
			{
				if ( Src.X() < 0 || Src.X() >= _Size.Width() || Dest.X() < 0 || Dest.X() >= dest->_Size.Width() )
					continue;

				const Pixel *CurSrcPixel = &_Pixels[Src.Y()][Src.X()];
				Pixel *CurDestPixel = &(dest->_Pixels[Dest.Y()][Dest.X()]);

				if ( !transparentSpaces || (transparentSpaces && !_Pixels[Src.Y()][Src.X()].IsSpace()) )
				{
					*CurDestPixel = *CurSrcPixel;

					int SrcIndex = CurSrcPixel->PaletteIndex;

					if ( SrcIndex >= 0 && SrcIndex < PaletteSize )
						CurDestPixel->PaletteIndex = Map_PrevIndexNewIndex[ CurSrcPixel->PaletteIndex ];
					else
						CurDestPixel->PaletteIndex = -1;
				}
			}
		}

		// Cleanup
		delete [] Map_PrevIndexNewIndex;
	}

	//
	// Draw functions
	//
	void Surface::DrawChar( const achar &ach, const Point &pos, bool transparentSpaces )
	{
		if ( !HasPoint( pos ) )
			return;

		if ( transparentSpaces && ach.IsSpace() )
			return;

		_Pixels[pos.Y()][pos.X()].Glyph( ach.Glyph() );
		_Pixels[pos.Y()][pos.X()].Bell = ach.Bell;
		_Pixels[pos.Y()][pos.X()].Attributes().clear();

		if ( !ach.Attributes().empty() )
			_Pixels[pos.Y()][pos.X()].AddAttributes( ach.Attributes() );

		// Finds the color of this character in our palette...creating it if necessary (if we're auto-managing the palette)
		_Pixels[pos.Y()][pos.X()].PaletteIndex = FindColorIndex( ach.Color );
	}

	void Surface::DrawChar( const std::string &ch, const Point &pos, bool transparentSpaces )
	{
		DrawChar( achar(ch), pos, transparentSpaces );
	}

	void Surface::DrawString( const astring &str, const Point &pos, bool transparentSpaces, bool invertString, bool rightToLeft, bool invertNewlineDir )
	{
		if ( str.empty() )
			return;

		astring::const_iterator CurChar, StrEnd;
		Point CurPos(pos.X(), pos.Y());
		ColorDef CurColor;
		achar Space(' ');

		for ( CurChar = str.begin(), StrEnd = str.end(); CurChar != StrEnd; ++CurChar )
		{
			if ( CurColor != CurChar->Color )
				CurColor = CurChar->Color;

			if ( CurChar->Glyph().length() == 1 ) // ASCII character
			{
				switch ( CurChar->Glyph()[0] )
				{
				case '\n':
					CurPos.IncY();
					break;

				case '\r':
					CurPos.X( pos.X() );
					break;

				case '\t':
					{
						int CurColumn = CurPos.X() - pos.X();
						int NumSpaces = 0;
						Space.Color = CurColor;

						if ( CurColumn < TabSize )
							NumSpaces = TabSize - CurColumn;
						else
							NumSpaces = TabSize - (CurColumn % TabSize);

						for ( int i = 0; i < NumSpaces; ++i )
						{
							DrawChar( Space, CurPos, transparentSpaces );
							CurPos.IncX();
						}

						break;
					}

				case ' ':
					{
						Space.Color = CurColor;
						DrawChar( Space, CurPos, transparentSpaces );
						CurPos.IncX();
						break;
					}

				default:
					{
						DrawChar( *CurChar, CurPos );
						CurPos.IncX();
						break;
					}
				}
			}
			else // Unicode character
			{
				DrawChar( *CurChar, CurPos );
				CurPos.IncX();
			}
		}

		return;
	}

	void Surface::DrawString( const std::string &str, const Point2D &pos, bool transparentSpaces, bool invertString, bool rightToLeft, bool invertNewlineDir )
	{
		DrawString( astring(str), pos, transparentSpaces, invertString, rightToLeft, invertNewlineDir );
	}

	// TODO: Make sure I'm finished!  This wasn't thouroughly tested!
	void Surface::DrawVString( const astring &str, const Point2D &pos, bool transparentSpaces, bool invertString, bool bottomToTop, bool invertNewlineDir )
	{
		if ( str.empty() )
			return;

		astring::const_iterator CurChar;
		Point CurPos(pos.X(), pos.Y());
		achar Space(' ');

		for ( CurChar = str.begin(); CurChar != str.end(); ++CurChar )
		{
			if ( CurChar->Glyph().length() == 1 )
			{
				switch ( CurChar->Glyph()[0] )
				{
				case '\n':
					CurPos.IncX();
					break;

				case '\r':
					CurPos.Y( pos.Y() );
					break;

				case '\t':
					{
						int CurRow = CurPos.Y() - pos.Y();
						int NumSpaces = 0;
						int VTabSize = TabSize / 2;

						if ( CurRow < VTabSize )
							NumSpaces = VTabSize - CurRow;
						else
							NumSpaces = VTabSize - (CurRow % VTabSize);

						for ( int i = 0; i < NumSpaces; ++i )
						{
							DrawChar( Space, CurPos, transparentSpaces );
							CurPos.IncY();
						}

						break;
					}

				default:
					DrawChar( *CurChar, CurPos, transparentSpaces );
					CurPos.IncY();
					break;
				}
			}
			else // Unicode character
			{
				DrawChar( *CurChar, CurPos, transparentSpaces );
				CurPos.IncY();
			}
		}
	}

	void Surface::DrawVString( const std::string &str, const Point &pos, bool transparentSpaces, bool invertString, bool bottomToTop, bool invertNewlineDir )
	{
		DrawVString( astring(str), pos, transparentSpaces, invertString, bottomToTop, invertNewlineDir );
	}

	void Surface::FillGlyph( const Area2D &size, const uchar &glyph, const Point2D &pos )
	{
		// If the pos isn't anywhere on the surface, then none of it will be
		if ( !HasPoint(pos) )
			return;

		tSizeType MinX = pos.X();
		tSizeType MaxX = pos.X() + size.Width();
		tSizeType MinY = pos.Y();
		tSizeType MaxY = pos.Y() + size.Height();

		// Keep us in bounds
		if ( MaxX > Width() )
			MaxX = Width();

		if ( MaxY > Height() )
			MaxY = Height();

		for ( tSizeType y = MinY; y < MaxY; ++y )
		{
			for ( tSizeType x = MinX; x < MaxX; ++x )
				_Pixels[y][x].Glyph( glyph );
		}
	}

	void Surface::FillColor( const Area2D &size, const ColorDef &color, const Point2D &pos )
	{
		// If the pos isn't anywhere on the surface, then none of it will be
		if ( !HasPoint(pos) )
			return;

		// Since this is a surface, we need to find our color with the palette
		int NewPaletteIndex = FindColorIndex( color );

		tSizeType MinX = pos.X();
		tSizeType MaxX = pos.X() + size.Width();
		tSizeType MinY = pos.Y();
		tSizeType MaxY = pos.Y() + size.Height();

		// Keep us in bounds
		if ( MaxX > Width() )
			MaxX = Width();

		if ( MaxY > Height() )
			MaxY = Height();

		for ( tSizeType y = MinY; y < MaxY; ++y )
		{
			for ( tSizeType x = MinX; x < MaxX; ++x )
				_Pixels[y][x].PaletteIndex = NewPaletteIndex;
		}
	}

	void Surface::FillChar( const Area2D &size, const achar &ach, const Point2D &pos )
	{
		// If the pos isn't anywhere on the surface, then none of it will be
		if ( !HasPoint(pos) )
			return;

		tSizeType MinX = pos.X();
		tSizeType MaxX = pos.X() + size.Width();
		tSizeType MinY = pos.Y();
		tSizeType MaxY = pos.Y() + size.Height();

		// Keep us in bounds
		if ( MaxX > Width() )
			MaxX = Width();

		if ( MaxY > Height() )
			MaxY = Height();

		Point2D CurPoint;

		for ( CurPoint.Y(MinY); CurPoint.Y() < MaxY; CurPoint.IncY() )
		{
			for ( CurPoint.X(MinX); CurPoint.X() < MaxX; CurPoint.IncX() )
				DrawChar( ach, CurPoint, false );
		}
	}

	void Surface::DrawBox( const Area2D &box, const Point2D &pos, const achar &hBorder, const achar &vBorder, const achar &fillCh, bool transparentSpaces )
	{
		if ( !HasPoint(pos) )
			return;

		// Fill the background first
		FillChar( Area2D(box.Width(), box.Height()), fillCh, pos );

		// Draw the vertical lines
		FillChar( Area2D(1, box.Height()), vBorder, pos );
		FillChar( Area2D(1, box.Height()), vBorder, Point2D(pos.X() + (box.Width() - 1), pos.Y()) );

		// Draw the horizontal lines
		FillChar( Area2D(box.Width(), 1), hBorder, pos );
		FillChar( Area2D(box.Width(), 1), hBorder, Point2D(pos.X(), pos.Y() + (box.Height() - 1)) );
	}

	bool Surface::pixelHasAnsi( const Point2D &pixel ) const
	{
		if ( !HasPoint( pixel ) )
			return false;

		return (_Pixels[pixel.Y()][pixel.X()].PaletteIndex != -1 || !_Pixels[pixel.Y()][pixel.X()].Attributes().empty());
	}

	std::string Surface::pixelRenderAnsi( const Point2D &pixel ) const
	{
		std::stringstream RenderedStr("");
		achar CurPixel = GetPixel( pixel );

		RenderedStr << CurPixel.RenderAnsi( DesiredDepth );

		return RenderedStr.str();
	}


	std::ostream &operator<<( std::ostream &left, const Surface &right )
	{
		left << right.Render();
		return left;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


