/*
 * area.cpp
 *
 * A simple definition of a square area.
 */


#include "AnsiGL/area.h"


namespace AnsiGL
{
	Area2D::Area2D():
		m_Width(0),
		m_Height(0)
	{
	}

	Area2D::Area2D( tSizeType width, tSizeType height ):
		m_Width(width),
		m_Height(height)
	{
	}

	Area2D::~Area2D()
	{
	}

	void Area2D::Width( tSizeType width )
	{
		if ( width < 0 )
			m_Width = 0;
		else
			m_Width = width;
	}

	void Area2D::Height( tSizeType height )
	{
		if ( height < 0 )
			m_Height = 0;
		else
			m_Height = height;
	}

	bool Area2D::operator==( const Area2D &right ) const
	{
		return (m_Width == right.m_Width && m_Height == right.m_Height);
	}

	bool Area2D::operator!=( const Area2D &right ) const
	{
		return !((*this) == right);
	}

	bool Area2D::operator>( const Area2D &right ) const
	{
		return ((m_Width * m_Height) > (right.m_Width * right.m_Height));
	}

	bool Area2D::operator<( const Area2D &right ) const
	{
		return !((*this) > right);
	}

	bool Area2D::operator>=( const Area2D &right ) const
	{
		return ((m_Width * m_Height) >= (right.m_Width * right.m_Height));
	}

	bool Area2D::operator<=( const Area2D &right ) const
	{
		return !((*this) >= right);
	}

	void Area2D::ResizeTo( const Area2D &size )
	{
		Width( size.m_Width );
		Height( size.m_Height );
	}

	void Area2D::ResizeTo( tSizeType width, tSizeType height )
	{
		Width( width );
		Height( height );
	}


	Area3D::Area3D():
		m_Depth(0)
	{
	}

	Area3D::Area3D( tSizeType width, tSizeType height, tSizeType depth ):
		m_Depth(depth)
	{
		ResizeTo( width, height, depth );
	}

	void Area3D::Depth( tSizeType depth )
	{
		if ( depth < 0 )
			m_Depth = 0;
		else
			m_Depth = depth;
	}

	bool Area3D::operator==( const Area3D &right ) const
	{
		return (m_Width == right.m_Width && m_Height == right.m_Height && m_Depth == right.m_Depth);
	}

	bool Area3D::operator!=( const Area3D &right ) const
	{
		return !((*this) == right);
	}

	bool Area3D::operator>( const Area3D &right ) const
	{
		return ((m_Width * m_Height * m_Depth) > (right.m_Width * right.m_Height * right.m_Depth));
	}

	bool Area3D::operator<( const Area3D &right ) const
	{
		return !((*this) > right);
	}

	bool Area3D::operator>=( const Area3D &right ) const
	{
		return ((m_Width * m_Height * m_Depth) >= (right.m_Width * right.m_Height * right.m_Depth));
	}

	bool Area3D::operator<=( const Area3D &right ) const
	{
		return !((*this) >= right);
	}

	void Area3D::ResizeTo( const Area3D &size )
	{
		Width( size.m_Width );
		Height( size.m_Height );
		Depth( size.m_Depth );
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
		m_Point(pos)
	{
		m_Width = area.Width();
		m_Height = area.Height();
	}

	void FixedArea2D::Point( const Point2D &point )
	{
		m_Point = point;
	}

	void FixedArea2D::X( tPointType x )
	{
		m_Point.X( x );
	}

	void FixedArea2D::Y( tPointType y )
	{
		m_Point.Y( y );
	}


	FixedArea3D::FixedArea3D()
	{
	}

	FixedArea3D::FixedArea3D( const Area3D &area, const Point3D &point ):
		m_Point(point)
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area3D &area, const Point2D &point ):
		m_Point(point)
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area2D &area, const Point3D &point ):
		m_Point(point)
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = 0;
	}

	FixedArea3D::FixedArea3D( const Area2D &area, const Point2D &point ):
		m_Point(point)
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = 0;
	}

	FixedArea3D::FixedArea3D( const Area3D &area )
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = area.Depth();
	}

	FixedArea3D::FixedArea3D( const Area2D &area )
	{
		m_Width = area.Width();
		m_Height = area.Height();
		m_Depth = 0;
	}

	void FixedArea3D::Point( const Point3D &point )
	{
		m_Point = point;
	}

	void FixedArea3D::X( tPointType x )
	{
		m_Point.X( x );
	}

	void FixedArea3D::Y( tPointType y )
	{
		m_Point.Y( y );
	}

	void FixedArea3D::Z( tPointType z )
	{
		m_Point.Z( z );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


