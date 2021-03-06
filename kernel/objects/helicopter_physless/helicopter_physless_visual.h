#pragma once

#include "helicopter_physless_view.h"
#include "common/aircraft_support_fwd.h"
#include "av/avFx/Fx.h"

namespace helicopter_physless
{
	struct visual
			: view
	{
		static object_info_ptr create(kernel::object_create_t const& oc, dict_copt dict);

    private:
        visual(object_create_t const& oc, dict_copt dict);

    protected:
        void update(double time);

    private:
        void on_malfunction_changed  ( aircraft::malfunction_kind_t kind ) override;
        void on_rotor_state           (double target, double speed, rotor_state_t visible) override;
    
    private:
        void fill_nodes();

    private:
        nm::node_info_ptr engine_node_;
        visual_object_ptr smoke_object_;
        visual_object_ptr label_object_;
        optional<double>  last_update_;
   

    private:
        boost::shared_ptr<visual_objects::label_support>   ls_;
        boost::shared_ptr<visual_objects::morphs_support>  ms_;
        boost::shared_ptr<visual_objects::smoke_support>   smoke_sup_;

        visual_system*                                     vsys_;

	private: 
		boost::function<void()>                            start_ ;
	    bool                                               deffered_init_;
	private:
		static const double sparks_end_duration_;
	};
}            