#include "stdafx.h"
#include "precompiled_objects.h"
#include "camera_ctrl.h"
#include "objects/nodes_management.h"

namespace camera_object
{

	object_info_ptr ctrl::create(object_create_t const& oc, dict_copt dict)
	{
		Verify(dict);
		return object_info_ptr(new ctrl(oc, dict));
	}

	ctrl::ctrl(object_create_t const& oc, dict_copt dict)
		: view          (oc, dict)
		, magnification_(1.)
	{
        if(mng())
            root()->set_position(geo_position(cam_pos, cam_orien));
	}


	geo_point_3 ctrl::pos() const 
	{
		return view::pos(); 
	}

	cpr ctrl::orien() const 
	{ 
		return view::orien(); 
	}

	void ctrl::set_trajectory(fms::trajectory_ptr  traj)
	{
		traj_ = traj;
		fms::traj_data data(*traj);
		set(msg::traj_assign_msg(data));
	}
    
    void ctrl::set_trajectory(const fms::traj_data&  traj_data)
    {
        // traj_ = traj;
        set(msg::traj_assign_msg(traj_data));
    }

	fms::trajectory_ptr  ctrl::get_trajectory()
	{
		return traj_;
	} 
}

AUTO_REG_NAME(camera_ext_ctrl, camera_object::ctrl::create);