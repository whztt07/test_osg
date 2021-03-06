#pragma once

namespace aircraft
{
    using namespace kernel;
    
    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys,const settings_t& sett,const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys,const settings_t& sett,const geo_position& init_pos);
}

namespace vehicle
{
    using namespace kernel;
    
    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys, const settings_t& sett, const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys, const settings_t& sett, const geo_position& init_pos);
}

namespace flock
{
    namespace manager 
    {
        using namespace kernel;
            
        struct settings_t;

        obj_create_data pack(fake_objects_factory* sys,const settings_t& sett,const geo_position& init_pos);
        object_info_ptr create(fake_objects_factory* sys,const settings_t& sett,const geo_position& init_pos);
    }
}

namespace aerostat
{
	using namespace kernel;
        
    struct settings_t;
	
    obj_create_data pack(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
}

namespace human
{
    using namespace kernel;

    struct settings_t;
    
    obj_create_data pack(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
}

namespace simple_route
{
    using namespace kernel;

    struct settings_t;
    
    obj_create_data pack(fake_objects_factory* sys,const settings_t& sett,const cg::geo_point_3& init_pos);
    object_info_ptr create(fake_objects_factory* sys,const settings_t& sett,const cg::geo_point_3& init_pos);
}

namespace aircraft_physless
{
    using namespace kernel;
    
    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys,const aircraft::settings_t& sett,const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys,const aircraft::settings_t& sett,const geo_position& init_pos);
}

namespace helicopter_physless
{
    using namespace kernel;
    
    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys,const aircraft::settings_t& sett,const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys,const aircraft::settings_t& sett,const geo_position& init_pos);
}

namespace airport
{
    using namespace kernel;
    
    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys,const settings_t& sett);
    object_info_ptr create(fake_objects_factory* sys,const settings_t& sett);
}

namespace camera
{
	using namespace kernel;

    obj_create_data pack(fake_objects_factory* sys, const geo_position& init_pos, boost::optional<std::string> name=boost::none);
	object_info_ptr create(fake_objects_factory* sys, const geo_position& init_pos, boost::optional<std::string> name=boost::none);
}

namespace arresting_gear
{
    using namespace kernel;

    struct settings_t;

    obj_create_data pack(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
    object_info_ptr create(fake_objects_factory* sys, const settings_t& sett,  const geo_position& init_pos);
}


namespace auto_object
{

    using namespace kernel;

    obj_create_data pack(fake_objects_factory* sys, const std::string& class_name);
    object_info_ptr create(fake_objects_factory* sys, const std::string& class_name);
}