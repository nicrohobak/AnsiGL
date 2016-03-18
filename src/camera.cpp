/*
 * camera.cpp
 *
 * A camera object that can render a snapshot from a context.
 * Consider it a special 'clone' for a context that has its own independent
 * viewport.
 */

#include "AnsiGL/camera.h"


namespace AnsiGL
{
	Point3D Camera::CurCameraPos() const
	{
		return _Position;
	}

	tPointType Camera::CameraX() const
	{
		return _Position.X();
	}

	tPointType Camera::CameraY() const
	{
		return _Position.Y();
	}

	tPointType Camera::CameraZ() const
	{
		return _Position.Z();
	}

	void Camera::MoveCameraTo( const Point3D &pos )
	{
		_Position = pos;
	}

	void Camera::MoveCameraTo( const Point2D &pos )
	{
		_Position = pos;
	}

	Context::Ptr Camera::GetContext() const
	{
		Context::Ptr MyContext = _Context.lock();
		return MyContext;
	}

	void Camera::SetContext( Context::Ptr context )
	{
		_Context = context;
	}

	std::string Camera::str()
	{
		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(Area2D(Content::_Size.Width(), Content::_Size.Height())) );

		// Render to the temp surface
		RenderToSurface( Temp );

		// Then render out to a string
		return Temp->str();
	}

	std::string Camera::Render() const
	{
		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(Area2D(Content::_Size.Width(), Content::_Size.Height())) );

		// Render to the temp surface
		RenderToSurface( Temp );

		// Then render out to a string
		return Temp->Render();
	}

	void Camera::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		Context::Ptr MyContext = _Context.lock();

		if ( !MyContext )
			return;

		Point3D SavedViewportPos = MyContext->CurViewportPos();

		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(Area2D(Content::_Size.Width(), Content::_Size.Height())) );

		MyContext->MoveViewportTo( _Position );
		MyContext->RenderToSurface( Temp );
		MyContext->MoveViewportTo( SavedViewportPos );

		Temp->RenderToSurface( dest, pos );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


