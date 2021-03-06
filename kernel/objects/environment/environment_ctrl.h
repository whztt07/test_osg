#pragma once

#include "environment_view.h"
#include "kernel/kernel_fwd.h"
#include "objects/environment.h"

namespace environment
{
    struct ctrl
        : view
		, control
    {
        static object_info_ptr create(kernel::object_create_t const& oc, dict_copt dict);

    private:
        ctrl(kernel::object_create_t const& oc, dict_copt dict);

    private:
        ctrl(kernel::object_create_t const& oc, istream_opt  stream);

    private:
        void set_weather(const weather_t&) override;

    private:
        void settings_edited    ();
        void ex_settings_changed();

    private:
        scoped_connection ex_settings_changed_connection_ ;
};

} // environment
