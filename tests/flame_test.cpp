#include "stdafx.h"
#include "visitors/info_visitor.h"
#include "visitors/find_tex_visitor.h"
#include "creators.h"
#include "av/avTerrain/Terrain.h"

namespace
{
	const size_t size_x = 200;//1980;
	const size_t size_z = 200;//1200;

    void createDAIGeometry( osg::Geometry& geom, int nInstances=1 )
    {
        const float halfDimX( size_x / 2.0 );
        const float halfDimZ( size_z / 2.0);

        osg::Vec3Array* v = new osg::Vec3Array;
        v->resize( 4 );
        geom.setVertexArray( v );

        // Geometry for a single quad.
#if 0
        (*v)[ 0 ] = osg::Vec3( -halfDimX, 0., 0. );
        (*v)[ 1 ] = osg::Vec3( halfDimX, 0., 0. );
        (*v)[ 2 ] = osg::Vec3( halfDimX, 0., halfDimZ*2.0f );
        (*v)[ 3 ] = osg::Vec3( -halfDimX, 0., halfDimZ*2.0f );
#endif
		(*v)[ 0 ] = osg::Vec3(  0., 0., 0. );
		(*v)[ 1 ] = osg::Vec3( halfDimX*2.0f, 0., 0. );
		(*v)[ 2 ] = osg::Vec3( halfDimX*2.0f, 0., halfDimZ*2.0f );
		(*v)[ 3 ] = osg::Vec3( 0., 0., halfDimZ*2.0f );

        // create color array data (each corner of our triangle will have one color component)
        osg::Vec4Array* pColors = new osg::Vec4Array;
        pColors->push_back( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
        pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
        pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
        pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
        geom.setColorArray( pColors );

        // make sure that our geometry is using one color per vertex
        geom.setColorBinding( osg::Geometry::BIND_PER_VERTEX );

        osg::Vec2Array* pTexCoords = new osg::Vec2Array( 4 );
        (*pTexCoords)[0].set( 0.0f, 0.0f );
        (*pTexCoords)[1].set( 1.0f, 0.0f );
        (*pTexCoords)[2].set( 1.0f, 1.0f );
        (*pTexCoords)[3].set( 0.0f, 1.0f );
        geom.setTexCoordArray( 0, pTexCoords );

        // Use the DrawArraysInstanced PrimitiveSet and tell it to draw 1024 instances.
        geom.addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4, nInstances ) );
    }


    osg::Geometry* createGeometry()
    {
        osg::Geometry* geom = new osg::Geometry;
        geom->setUseDisplayList( false );
        geom->setUseVertexBufferObjects( true );
        createDAIGeometry( *geom, 1*1 );

        return geom;
    }
}

int main_flame_test( int argc, char** argv )
{
    osg::ArgumentParser arguments(&argc,argv);

    osgViewer::Viewer viewer(arguments);

    osgDB::getDataFilePathList().push_back(osgDB::getCurrentWorkingDirectory() + "\\data\\models");
    osgDB::getDataFilePathList().push_back(osgDB::getCurrentWorkingDirectory() + "\\data\\areas");
    osgDB::getDataFilePathList().push_back(osgDB::getCurrentWorkingDirectory() + "\\data\\materials");
    osgDB::getDataFilePathList().push_back(osgDB::getCurrentWorkingDirectory() + "\\data\\materials\\sky");
    osgDB::getDataFilePathList().push_back(osgDB::getCurrentWorkingDirectory() + "\\data\\materials\\lib");   

    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;

    osg::StateSet * pSceneSS = root->getOrCreateStateSet();

    osg::Geode*		geodeGrass = new osg::Geode();	
    osg::ref_ptr<osg::Geometry> geom = createGeometry();

    geodeGrass->setCullingActive( false );   
    geodeGrass->addDrawable( geom.get() );

    root->addChild(geodeGrass);

	pSceneSS->addUniform(new osg::Uniform("iResolution"  , osg::Vec2(size_x,size_z)));

    osg::ref_ptr<osg::Program> cFlameProg = creators::createProgram("flame").program; 
    cFlameProg->setName("FlameShader");
    pSceneSS->setAttributeAndModes(cFlameProg.get());
	pSceneSS->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	// setup blending
	osg::BlendFunc * pBlendFunc = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	pSceneSS->setAttributeAndModes(pBlendFunc, osg::StateAttribute::ON);

	osg::BlendEquation* pBlendEquation = new osg::BlendEquation(osg::BlendEquation::FUNC_ADD);
	pSceneSS->setAttributeAndModes(pBlendEquation,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

    viewer.apply(new osgViewer::SingleScreen(1));
    // Add some useful handlers to see stats, wireframe and onscreen help
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgGA::StateSetManipulator(root->getOrCreateStateSet()));
    viewer.addEventHandler(new osgViewer::HelpHandler);
    viewer.setSceneData( root.get() );
    return viewer.run();
}

AUTO_REG(main_flame_test)