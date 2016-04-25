#pragma once 


#include "application/application_fwd.h"
#include "application/panels_fwd.h"

#include "av/avLights/LightMaps.h"
#include "av/avCore/DecalRender.h"
#include "av/ISky.h"
#include "av/avFx/Fx.h"


#ifdef NON_DLL
# define VISUAL_API
#else
#ifdef VISUAL_EXPORTS
# define VISUAL_API __declspec(dllexport)
#else
# define VISUAL_API __declspec(dllimport)
#endif
#endif

//
//  Forwards
//

namespace app
{
    struct settings_t;
    struct cloud_params_t;
}

namespace avSky
{
    class Ephemeris;
    class Sky;
}

namespace avWeather
{
	class Weather;
};

namespace avTerrain
{
    class Terrain;
}

namespace avShadow
{
    class ShadowTechnique;
}

namespace avScene
{
    class Lights;
}

namespace bi
{
    class RigidUpdater;
}

namespace Utils
{
    struct  LoadNodeThread;
    class TrajectoryDrawer;
}


class PickHandler;


namespace avScene {
    
    // forward declaration
    class ScreenTextureManager;

    //
    // Scene class itself
    //
   

    class VISUAL_API Scene : public osg::Group, 
                             public av::IScene
    {

    public:
        static bool                                 Create( osgViewer::Viewer* vw );
        static void                                 Release();
        static Scene*                               GetInstance();
        
        virtual bool                                PreUpdate();

        inline osgViewer::Viewer*                   GetViewer();
        inline osg::Group*                          getEphemerisNode();
        inline osg::Group*                          getCommonNode();
        inline osg::Camera*                         getCamera();
        inline avScene::Lights *                    getLights();
        std::vector<osg::ref_ptr<osg::Node>>&       getLamps();
		avSky::ISky*                                getSky();   
		inline avWeather::Weather*                  getWeather();
        inline ScreenTextureManager*                getScreenTextureManager() const;
        inline osg::Group*                          getTerrainRoot() const { return _terrainRoot.get(); } 
        
        void                                        setHomePosition(const osg::Vec3d& eye, const osg::Vec3d& center);

        osg::Node*                                  load(std::string path, osg::Node* parent=0, uint32_t seed=0, bool async=true);

        static std::string                          zoneToReload(){return zone_to_reload_;}
    
    private: // IScene interface declaration
        av::environment_weather*                    getEnvWeather() const override;
        av::ITrajectoryDrawer*                      GetTrajectoryDrawer() const override; 
    
    private:

        Scene();
        virtual									    ~Scene();
        bool									    Initialize( osgViewer::Viewer* vw );
        osg::Group*                                 createTerrainRoot();
        void                                        createObjects();
        void                                        setupDecals(const std::string& scene);

    private:
        void                                        onZoneChanged   ( int zone );
		void										onSetLights     ( bool on  );
        void                                        onSetShadows(const optional<bool>& on, const optional<bool>& on_part );
		void										onSetMap        ( float val );
        void										onSetGlobalIntensity ( float val );
        void                                        onSetCloudParams( const app::cloud_params_t& s );
        bool                                        onEvent         ( const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa, osg::Object * obj, osg::NodeVisitor * nv );
        void                                        onExit();

    private:
        void                                        update( osg::NodeVisitor * nv );

	private:
        static osg::ref_ptr<Scene>                  _scenePtr;

        osg::ref_ptr<Utils::TrajectoryDrawer>       _trajectory_drawer;

        osg::ref_ptr<osgViewer::Viewer>             _viewerPtr;
        osg::ref_ptr<osg::Group>                    _commonNode;
        osg::ref_ptr<osg::Group>                    _environmentNode;
        osg::ref_ptr<avSky::Sky>                    _Sky;
        osg::ref_ptr<avWeather::Weather>			_Weather;
        osg::ref_ptr<avSky::Ephemeris>              _ephemerisNode;
        osg::ref_ptr<osg::Group>                    _weatherNode;
        osg::ref_ptr<avTerrain::Terrain>            _terrainNode; 
        osg::ref_ptr<osg::LightSource>              _ls;
        osg::ref_ptr<osg::Group>                    _terrainRoot;
        osg::ref_ptr<avShadow::ShadowTechnique>     _st;  
        osg::ref_ptr<ScreenTextureManager>          _screenTextureManager;

         osg::ref_ptr<osg::Node>                    _loadManager;

        osg::ref_ptr<osg::Group>                    _groupMainReflection;
#if !defined(VISUAL_EXPORTS)
        osg::ref_ptr<bi::RigidUpdater>              _rigidUpdater;
#endif
        osg::ref_ptr<PickHandler>                   _pickHandler; 
        std::vector<osg::ref_ptr<osg::Node>>        _lamps;
        osg::ref_ptr<Lights>                        _lights;                                            
        ILightMapRendererPtr                        _light_map;

        avCore::IDecalRendererPtr                   _decal_map;

        app::vis_settings_panel_ptr                 _vis_settings_panel;
        app::time_panel_ptr                         _time_panel;
		app::main_window_ptr						_mw;
        connection_holder                           conn_holder_;

        static std::string                          zone_to_reload_;
        
		// uniforms	
	private:
		osg::ref_ptr<osg::Uniform>                  _windTime;


    private:
        osg::ref_ptr<osg::Node>                     _logo;

		SmokeSfxNode *                 smoke_sfx_weak_ptr_;
		SparksSfxNode *                sparks_sfx_weak_ptr;
		FrictionDustSfxNode*           fd_sfx_weak_ptr_;

		LandingDustSfxNode *           ld_sfx_weak_ptr_;
		FoamStreamSfxNode *            fs_sfx_weak_ptr_;
    private:
        Utils::LoadNodeThread*                          _lnt;
        std::vector<osg::ref_ptr<osg::MatrixTransform>> mt_;

    private:
       app::settings_t*                               settings_;

    public:
        DECLARE_EVENT(object_loaded, (uint32_t)) ;
    };

} // end namespace


#include "Scene.inl"