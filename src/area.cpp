/*
 * area.cpp
 *
 * A simple definition of a square area.
 */


#include "AnsiGL/area.h"


namespace AnsiGL
{
	Area2D::Area2D():
		_Width(0),
		_Height(0)
	{
	}

	Area2D::Area2D( tSizeType width, tSizeType height ):
		_Width(width),
		_Height(height)
	{
	}

	Area2D::~Area2D()
	{
	}

	void Area2D::Width( tSizeType width )
	{
		if ( width < 0 )
			_Width = 0;
		else
			_Width = width;
	}

	void Area2D::Height( tSizeType height )
	{
		if ( height < 0 )
			_Height = 0;
		else
			_Height = height;
	}

	bool Area2D::operator==( const Area2D &right ) const
	{
		return (_Width == right._Width && _Height == right._Height);
	}

	bool Area2D::operator!=( const Area2D &right ) const
	{
		return !((*this) == right);
	}

	bool Area2D::operator>( const Area2D &right ) const
	{
		return ((_Width * _Height) > (right._Width * right._Height));
	}

	bool Area2D::operator<( const Area2D &right ) const
	{
		return !((*this) > right);
	}

	bool Area2D::operator>=( const Area2D &right ) const
	{
		return ((_Width * _Height) >= (right._Width * right._Height));
	}

	bool Area2D::operator<=( const Area2D &right ) const
	{
		return !((*this) >= right);
	}

	void Area2D::ResizeTo( const Area2D &size )
	{
		Width( size._Width );
		Height( size._Height );
	}

	void Area2D::ResizeTo( tSizeType width, tSizeType height )
	{
		Width( width );
		Height( height );
	}


	Area3D::Area3D():
		_Depth(0)
	{
	}

	Area3D::Area3D( tSizeType width, tSizeType height, tSizeType depth ):
		_Depth(depth)
	{
		ResizeTo( width, height, depth );
	}

	void Area3D::Depth( tSizeType depth )
	{
		if ( depth < 0 )
			_Depth = 0;
		else
			_Depth = depth;
	}

	bool Area3D::operator==( const Area3D &right ) const
	{
		return (_Width == right._Width && _Height == right._Height && _Depth == right._Depth);
	}

	bool Area3D::operator!=( const Area3D &right ) const
	{
		return !((*this) == right);
	}

	bool Area3D::operator>( const Area3D &right ) const
	{
		return ((_Width * _Height * _Depth) > (right._Width * right._Height * right._Depth));
	}

	bool Area3D::operator<( const Area3D &right ) const
	{
		return !((*this) > right);
	}

	bool Area3D::operator>=( const Area3D &right ) const
	{
		return ((_Width * _Height * _Depth) >= (right._Width * right._Height * right._Depth));
	}

	bool Area3D::operator<=( const Area3D &right ) const
	{
		return !((*this) >= right);
	}

	void Area3D::ResizeTo( const Area3D &size )
	{
		Width( size._Width );
		Height( size._Height );
		Depth( size._Depth );
	}

	void Area3D::ResizeTo( tSizeType width, tSizeType height, tSizeType depth )
	{
		Width( width );
		Height( height );
		Depth( depth );
	}


	FixedArea2D::FixedArea2D()
	{
	}

	FixedArea2D::FixedArea2D( const Area2D &area, const Point2D &pos ):
		_Point(pos)
	{
		_Width = area.Width();
		_Height = area.Height();
	}

	void FixedArea2D::Point( const Point2D &point )
	{
		_Point = point;
	}

	void FixedArea2D::X( tPointType x )
	{
		_Point.X( x );
	}

	void FixedArea2D::Y( tPointType y )
	{
		_Point.Y( y );
	}


	FixedArea3D::FixedArea3D()
	{
	}

	FixedArea3D::FixedArea3D( const Area3D &area, const Point3D &point ):
		_Point(point)
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area3D &area, const Point2D &point ):
		_Point(point)
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area2D &area, const Point3D &point ):
		_Point(point)
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = 0;
	}

	FixedArea3D::FixedArea3D( const Area2D &area, const Point2D &point ):
		_Point(point)
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = 0;
	}

	FixedArea3D::FixedArea3D( const Area3D &area )
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area2D &area )
	{
		_Width = area.Width();
		_Height = area.Height();
		_Depth = 0;
	}

	void FixedArea3D::Point( const Point3D &point )
	{
		_Point = point;
	}

	void FixedArea3D::X( tPointType x )
	{
		_Point.X( x );
	}

	void FixedArea3D::Y( tPointType y )
	{
		_Point.Y( y );
	}

	void FixedArea3D::Z( tPointType z )
	{
		_Point.Z( z );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


