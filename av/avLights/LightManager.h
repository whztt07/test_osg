#pragma once

namespace avScene
{

class LightManager 
    : public osg::Node
{
private:

    LightManager();
    ~LightManager();


public:
    struct Light
    {
        osg::Transform * transform;

        cg::range_2f     spotFalloff;
        cg::range_2f     distanceFalloff;
        cg::colorf       color;

        cg::point_3f     position;
        cg::vector_3f    direction;

        bool             active;
        bool             high_priority;
        bool             lm_only;
        float            normal_coeff;

        inline Light() : transform(nullptr), active(false), high_priority(false), lm_only(false), normal_coeff(0.f) { }
    };

public:
    static void Create();
    static void Release();

    static LightManager * GetInstance();



public:

    void      update  ( osg::NodeVisitor * nv );
    uint32_t  addLight( const Light& light);


    FIXME(Check before return);
    
    Light&   getLight(uint32_t id) {return _LightsMap[id];};

private:
	uint32_t genUID();

private:

    static osg::ref_ptr<LightManager> g_Instance;

    typedef std::map<unsigned,Light>  LightsMap;
    LightsMap                         _LightsMap;
    mutable OpenThreads::Mutex        _lightManagerMapMutex;
};


} // namespace avScene
