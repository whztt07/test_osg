#pragma  once

#include "common/debug_render.h"

namespace avCollision
{

class  DebugRenderer : public osg::Referenced 
                     , public debug_render
{
public:
    DebugRenderer();
    virtual ~DebugRenderer();

    osg::Node* getSceneGraph();

    virtual         void setEnabled    ( bool enable );
    virtual         bool getEnabled    () const;

    virtual void	drawLine           ( const cg::point_3& from,const cg::point_3& to,const cg::point_3& color );
    virtual void	drawSphere         ( const cg::point_3& p, float radius, const cg::point_3& color );

    virtual void	drawTriangle       ( const cg::point_3& a,const cg::point_3& b,const cg::point_3& c,const cg::point_3& color,float alpha );
    virtual void	drawContactPoint   ( const cg::point_3& PointOnB,const cg::point_3& normalOnB,float distance,int lifeTime,const cg::point_3& color );
    virtual void	reportErrorWarning ( const char* warningString );
    virtual void	draw3dText         ( const cg::point_3& location,const char* textString );

    virtual void	setDebugMode       (int debugMode);
    virtual int		getDebugMode       () const;

    virtual void    EndDraw            ();
    virtual void    BeginDraw          ();

    virtual void    setTextSize        ( const float size );
    virtual float   getTextSize        () const;

private:
    osgText::Text* initText            ();

	int _debugMode;
    bool _enabled;
    bool _active;
    float _textSize;

    osg::ref_ptr< osg::Group >    _group;

    // Points, lines, triangles, and text
    osg::ref_ptr< osg::Geode >    _geode;
    osg::ref_ptr< osg::Geometry > _ptGeom;
    osg::ref_ptr< osg::Geometry > _lnGeom;
    osg::ref_ptr< osg::Geometry > _triGeom;

    typedef std::vector< osg::ref_ptr< osgText::Text > > TextVec;
    TextVec                       _textVec;
    unsigned int                  _textStrings;

    osg::Vec3Array*               _ptVerts;
    osg::Vec4Array*               _ptColors;

    osg::Vec3Array*               _lnVerts;
    osg::Vec4Array*               _lnColors;

    osg::Vec3Array*               _triVerts;
    osg::Vec4Array*               _triColors;


    int                          _frame;
    int                          _contacts;
};


// osgbCollision
}


