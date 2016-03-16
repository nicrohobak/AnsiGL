#ifndef __ANSIGL_CAMERA_H__
#define __ANSIGL_CAMERA_H__

/*
 * camera.h
 *
 * A camera object that can render a snapshot from a context.
 * Consider it a special 'clone' for a context that has its own independent
 * viewport.
 */


#include "content.h"
#include "context.h"


namespace AnsiGL
{
	class Camera : public Content
	{
	public:
		ANSIGL_POINTERS( Camera )

	protected:
		Context::wPtr	m_Context;
		Point3D		m_Position;

	public:
		Camera();
		Camera( Context::Ptr context, const Area2D &viewportSize = Area2D(), const Point3D &pos = Point3D() );
		virtual ~Camera();

		virtual Point3D CurCameraPos() const;
		virtual tPointType CameraX() const;
		virtual tPointType CameraY() const;
		virtual tPointType CameraZ() const;
		virtual void MoveCameraTo( const Point3D &pos );
		virtual void MoveCameraTo( const Point2D &pos );

		virtual Context::Ptr GetContext() const;
		virtual void SetContext( Context::Ptr context );

		std::string str();
		std::string Render() const;
		void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;
	};
}


#endif // __ANSIGL_CAMERA_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


