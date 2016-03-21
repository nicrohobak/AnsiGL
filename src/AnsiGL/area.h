#ifndef __ANSIGL_AREA_H__
#define __ANSIGL_AREA_H__

/*
 * area.h
 *
 * A simple definition of a square area.
 */


#include "point.h"


namespace AnsiGL
{
	// A single data type for all sizes based on AnsiGL::Area
	typedef int			tSizeType;

	//
	// A square area
	//
	class Area2D
	{
	public:
		ANSIGL_POINTERS( Area2D )

	public:
		Area2D():
			_Width(0),
			_Height(0)
		{
		}

		Area2D( tSizeType width, tSizeType height ):
			_Width(width),
			_Height(height)
		{
		}

		~Area2D()
		{
		}

		tSizeType Width() const
		{
			return _Width;
		}

		tSizeType Height() const
		{
			return _Height;
		}

		void Width( tSizeType width )
		{
			if ( width < 0 )
				_Width = 0;
			else
				_Width = width;
		}

		void Height( tSizeType height )
		{
			if ( height < 0 )
				_Height = 0;
			else
				_Height = height;
		}

		bool operator==( const Area2D &right ) const
		{
			return (_Width == right._Width && _Height == right._Height);
		}

		bool operator!=( const Area2D &right ) const
		{
			return !((*this) == right);
		}

		bool operator>( const Area2D &right ) const
		{
			return ((_Width * _Height) > (right._Width * right._Height));
		}

		bool operator<( const Area2D &right ) const
		{
			return !((*this) > right);
		}

		bool operator>=( const Area2D &right ) const
		{
			return ((_Width * _Height) >= (right._Width * right._Height));
		}

		bool operator<=( const Area2D &right ) const
		{
			return !((*this) >= right);
		}

		void ResizeTo( const Area2D &size )
		{
			Width( size._Width );
			Height( size._Height );
		}

		void ResizeTo( tSizeType width, tSizeType height )
		{
			Width( width );
			Height( height );
		}

	protected:
		tSizeType	_Width;
		tSizeType	_Height;
	};


	// For most things, an area is simply 2D anyway...
	typedef Area2D	Area;


	//
	// A cubic area
	//
	class Area3D : public Area2D
	{
	public:
		ANSIGL_POINTERS( Area3D )

	public:
		Area3D():
			_Depth(0)
		{
		}

		Area3D( tSizeType width, tSizeType height, tSizeType depth ):
			_Depth(depth)
		{
			ResizeTo( width, height, depth );
		}

		tSizeType Depth() const
		{
			return _Depth;
		}

		void Depth( tSizeType depth )
		{
			if ( depth < 0 )
				_Depth = 0;
			else
				_Depth = depth;
		}

		bool operator==( const Area3D &right ) const
		{
			return (_Width == right._Width && _Height == right._Height && _Depth == right._Depth);
		}

		bool operator!=( const Area3D &right ) const
		{
			return !((*this) == right);
		}

		bool operator>( const Area3D &right ) const
		{
			return ((_Width * _Height * _Depth) > (right._Width * right._Height * right._Depth));
		}

		bool operator<( const Area3D &right ) const
		{
			return !((*this) > right);
		}

		bool operator>=( const Area3D &right ) const
		{
			return ((_Width * _Height * _Depth) >= (right._Width * right._Height * right._Depth));
		}

		bool operator<=( const Area3D &right ) const
		{
			return !((*this) >= right);
		}

		void ResizeTo( const Area3D &size )
		{
			Width( size._Width );
			Height( size._Height );
			Depth( size._Depth );
		}

		void ResizeTo( tSizeType width, tSizeType height, tSizeType depth )
		{
			Width( width );
			Height( height );
			Depth( depth );
		}

	protected:
		tSizeType	_Depth;
	};


	//
	// An area, fixed to a specific point
	//
	class FixedArea2D : public Area2D
	{
	public:
		ANSIGL_POINTERS( FixedArea2D )

	public:
		FixedArea2D()
		{
		}

		FixedArea2D( const Area2D &area, const Point2D &pos ):
			_Point(pos)
		{
			_Width = area.Width();
			_Height = area.Height();
		}

		const Point2D &Point() const
		{
			return _Point;
		}

		void Point( const Point2D &point )
		{
			_Point = point;
		}

		const tPointType &X() const
		{
			return _Point.X();
		}

		void X( tPointType x )
		{
			_Point.X( x );
		}

		const tPointType &Y() const
		{
			return _Point.Y();
		}
		
		void Y( tPointType y )
		{
			_Point.Y( y );
		}

		// NOTE: Since the ==/!= operators have not been defined here, any use of those operators will not compare the position itself, only the size of the fixed areas!

	protected:
		Point2D		_Point;	// The point to which this area is fixed
	};


	// For most things, an area is simply 2D anyway...
	typedef FixedArea2D		FixedArea;


	//
	// A 3D area, fixed to a specific point
	//
	class FixedArea3D : public Area3D
	{
	public:
		ANSIGL_POINTERS( FixedArea3D )

	public:
		FixedArea3D()
		{
		}

		FixedArea3D( const Area3D &area, const Point3D &point ):
			_Point(point)
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = area.Depth();
		}

		FixedArea3D( const Area3D &area, const Point2D &point ):
			_Point(point)
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = area.Depth();
		}

		FixedArea3D( const Area2D &area, const Point3D &point ):
			_Point(point)
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = 0;
		}

		FixedArea3D( const Area2D &area, const Point2D &point ):
			_Point(point)
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = 0;
		}

		FixedArea3D( const Area3D &area )
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = area.Depth();
		}

		FixedArea3D( const Area2D &area )
		{
			_Width = area.Width();
			_Height = area.Height();
			_Depth = 0;
		}

		const Point3D &Point() const
		{
			return _Point;
		}

		void Point( const Point3D &point )
		{
			_Point = point;
		}

		const tPointType &X() const
		{
			return _Point.X();
		}
		
		void X( tPointType x )
		{
			_Point.X( x );
		}

		const tPointType &Y() const
		{
			return _Point.Y();
		}
		
		void Y( tPointType y )
		{
			_Point.Y( y );
		}

		const tPointType &Z() const
		{
			return _Point.Z();
		}

		void Z( tPointType z )
		{
			_Point.Z( z );
		}

		// NOTE: Since the ==/!= operators have not been defined here, any use of those operators will not compare the position itself, only the size of the fixed areas!

	protected:
		Point3D		_Point;			// The position to which this area is fixed
	};
}


#endif // __ANSIGL_AREA_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


