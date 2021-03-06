#include "stdafx.h"
#include "precompiled_objects.h"

#include "flock_manager_ctrl.h"
#include "common/randgen.h"

//       ��� �������� �������� �������� (��� ����� �����-��)
FIXME(�������� ��������� �������� �������� � ������������)
#include "kernel/systems/fake_system.h"
#include "flock_child/flock_child_view.h"
#include "nodes_manager/nodes_manager_view.h"



namespace flock
{

using namespace kernel;

namespace child 
{
    object_info_ptr create(fake_objects_factory* sys, uint32_t   parent_id, const settings_t& sett,const geo_position& init_pos)
    {
        const std::string class_name = "flock_child";
        const std::string unique_name = sys->generate_unique_name(class_name);

        obj_create_data ocd(class_name, unique_name, dict::wrap(child_data(sett, state_t(init_pos.pos, init_pos.orien), parent_id)));
        ocd
            .add_child(obj_create_data("nodes_manager", "nodes_manager", dict::wrap(nodes_management::nodes_data          ())));

        return sys->create_object(ocd);	
    }
}

namespace manager
{


object_info_ptr ctrl::create(kernel::object_create_t const& oc, dict_copt dict)
{
    return object_info_ptr(new ctrl(oc, dict));
}


AUTO_REG_NAME(flock_manager_ext_ctrl, ctrl::create);

ctrl::ctrl( kernel::object_create_t const& oc, dict_copt dict)
    : view    (oc,dict)
    , _obj_col(dynamic_cast<kernel::object_collection*>(oc.sys))
{

	// settings_._childAmount = 7 * 7;
}

void ctrl::update( double time )
{   
	view::update(time);

	int count = settings_._childAmount - roamers_.size();
	if (count>0)
	{
		auto * of = dynamic_cast<kernel::fake_objects_factory*>(sys_);

		simplerandgen  rnd(static_cast<unsigned>(::time(nullptr)));

		child::settings_t vs;
		vs.model        = "crow"; 

		decart_position child_pos;

		for (int i=0; i < 4; ++i )
		{
			// child_pos.pos   = point_3(rnd.random_8bit(),rnd.random_8bit(),300 + rnd.random_8bit()/2.0);
			FIXME("�������� ���, �������������� ���� ����")
	        point_3 spawner_pos = cg::geo_base_3(get_base())(state_.pos);
			child_pos.pos   = point_3(rnd.random_range(-1.0,1.0), rnd.random_range(-1.0,1.0), rnd.random_range(-1.0,1.0) ) * settings_._spawnSphere + spawner_pos;
			child_pos.orien = state_.orien;
			geo_position vgp(child_pos, get_base());

			/*roamers_.insert(*/child::create(of, this->object_id(),vs,vgp)/*)*/;

		}
	}

}

void ctrl::on_object_destroying(object_info_ptr object)
{
    base_view_presentation::on_object_destroying(object) ;

    if (object->object_id() == this->object_id())
    {

		while (  roamers_.size()>0  )
		{
			auto it = roamers_.begin();
			uint32_t oid = object_info_ptr(*it)->object_id();
			roamers_.erase(it);
			_obj_col->destroy_object(oid);
		}
     
        
    }
}

} // manager

} // flock