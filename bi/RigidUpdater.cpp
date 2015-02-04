#include "stdafx.h"

#include "../find_node_visitor.h"
#include "../high_res_timer.h"
#include "BulletInterface.h"
#include "../aircraft.h"                // FIXME TODO don't need here 
#include "../aircraft_common.h"
#include "../aircraft_shassis_impl.h"   // And maybe this too
#include "nodes_manager.h"
#include "../ada.h"
#include "../bada_import.h"
#include "../phys_aircraft.h"
#include "../vehicle.h"

#include "RigidUpdater.h"

namespace bi
{

    void RigidUpdater::addGround( const osg::Vec3& gravity )
    {
        _sys->createWorld( osg::Plane(0.0f, 0.0f, 1.0f, 0.0f), gravity,
            [&](int id){
                if(_on_collision)
                    _on_collision(_physicsNodes[id].get());   
        } );

        //const bool debug( arguments.read( "--debug" ) );
        if( _debug )
        {
            //osg::notify( osg::INFO ) << "osgbpp: Debug" << std::endl;
            _dbgDraw = new avCollision::GLDebugDrawer();
            _dbgDraw->setDebugMode( ~btIDebugDraw::DBG_DrawText );
            if(_root->getNumParents()>0)
                _root->getParent(0)->addChild( _dbgDraw->getSceneGraph() );
            else
                _root->addChild( _dbgDraw->getSceneGraph() );
        }

        if(_dbgDraw)
            _sys->setDebugDrawer(_dbgDraw);
    }



    void RigidUpdater::addPhysicsAirplane( osg::Node* node, const osg::Vec3& pos, const osg::Vec3& vel, double mass )
    {
        int id = _physicsNodes.size();
        
        osg::ComputeBoundsVisitor cbv;
        node->accept( cbv );
        const osg::BoundingBox& bb = cbv.getBoundingBox();

        float xm = bb.xMax() - bb.xMin();
        float ym = bb.yMax() - bb.yMin();
        float zm = bb.zMax() - bb.zMin();

        float rot_angle = -90.f;
        auto tr = osg::Matrix::translate(osg::Vec3(0.0,0.0,-(ym)/2.0f));
        if(dynamic_cast<osg::LOD*>(node))
        {
            rot_angle = 0;
            tr = osg::Matrix::translate(osg::Vec3(0,0,-(zm)/2.0f));
        }

        osg::MatrixTransform* positioned = new osg::MatrixTransform(tr);

        const osg::Quat quat(osg::inDegrees(rot_angle), osg::X_AXIS,                      
            osg::inDegrees(0.f) , osg::Y_AXIS,
            osg::inDegrees(0.f)   , osg::Z_AXIS ); 

        osg::MatrixTransform* rotated = new osg::MatrixTransform(osg::Matrix::rotate(quat));

        rotated->addChild(node);
        positioned->addChild(rotated);
        osg::Vec3 half_length ( (bb.xMax() - bb.xMin())/2.0f,(bb.zMax() - bb.zMin())/2.0f,(bb.yMax() - bb.yMin()) /2.0f );
        if(dynamic_cast<osg::LOD*>(node))
        {
            half_length = osg::Vec3 ( (bb.xMax() - bb.xMin())/2.0f,(bb.yMax() - bb.yMin())/2.0f,(bb.zMax() - bb.zMin()) /2.0f );
        }

        osg::Node*  lod3 =  findFirstNode(node,"Lod3",findNodeVisitor::not_exact);

#if 0
        _sys->createBox( id, half_length, mass );
        addPhysicsData( id, positioned, pos, vel, mass );
#else  
        _sys->createShape(lod3, id, mass);
        addPhysicsData( id, rotated, pos, vel, mass );
#endif
    }

    void RigidUpdater::addUFO(osg::Node* node,const osg::Vec3& pos, const osg::Vec3& vel, double mass)
    {
        osg::Node*  lod3 =  findFirstNode(node,"Lod3",findNodeVisitor::not_exact);
        int id = _physicsNodes.size();
        _sys->createUFO( lod3,id, mass );

        _sys->setMatrix( id, osg::Matrix::translate(pos) );
        _sys->setVelocity( id, vel );

    }

    void RigidUpdater::addUFO2(osg::Node* node,const osg::Vec3& pos, const osg::Vec3& vel, double mass)
    {
        osg::Node*  lod3 =  findFirstNode(node,"Lod3",findNodeVisitor::not_exact);
        int id = _physicsNodes.size();
        
        //nm::manager_ptr man = nm::create_manager(lod3);
        //
        //size_t pa_size = _phys_aircrafts.size();
        ////if(_phys_aircrafts.size()==0)
        //{
        //    aircraft::phys_aircraft_ptr ac_ = aircraft::phys_aircraft_impl::create(
        //                      cg::geo_base_3(cg::geo_point_3(0.000* (pa_size+1),0.0*(pa_size+1),0)),
        //                                                                           _sys,
        //                                                                            man,
        //        geo_position(cg::geo_point_3(0.000* (pa_size+1),0.005*(pa_size+1),0),cg::quaternion(cg::cpr(0, 0, 0))),
        //                                                  ada::fill_data("BADA","A319"),                                                   
        //                        boost::make_shared<aircraft::shassis_support_impl>(man),
        //                                                                             0);

        //    _phys_aircrafts.push_back(ac_);
        //}

        _aircrafts.push_back(_sys->createUFO2( lod3,id, mass ));

        //osg::ComputeBoundsVisitor cbv;
        //node->accept( cbv );
        //const osg::BoundingBox& bb = cbv.getBoundingBox();

        //float xm = bb.xMax() - bb.xMin();
        //float ym = bb.yMax() - bb.yMin();
        //float zm = bb.zMax() - bb.zMin();

        //float rot_angle = -90.f;
        //auto tr = osg::Matrix::translate(osg::Vec3(0.0,-(ym)/2.0f,0.0));
        //if(dynamic_cast<osg::LOD*>(node))
        //{
        //    rot_angle = 0;
        //    tr = osg::Matrix::translate(osg::Vec3(0,0,-(zm)/2.0f));
        //}        

        //osg::MatrixTransform* positioned = new osg::MatrixTransform(tr);

        //const osg::Quat quat(osg::inDegrees(rot_angle), osg::X_AXIS,                      
        //    osg::inDegrees(0.f) , osg::Y_AXIS,
        //    osg::inDegrees(0.f)   , osg::Z_AXIS ); 

        //osg::MatrixTransform* rotated = new osg::MatrixTransform(osg::Matrix::rotate(quat));
        //positioned->addChild(rotated);
        //rotated->addChild(node);

        addPhysicsData( id, addGUIObject(node), pos, /*vel*/osg::Vec3(0.0,0.0,0.0), mass );
        phys::aircraft::control_ptr(_aircrafts.back())->apply_force(vel);
    }


    void RigidUpdater::addUFO3(osg::Node* node,const osg::Vec3& pos, const osg::Vec3& vel, double mass)
    {
        // TODO FIXME � ��� ����� �������� ������� ���������� ����� ������ �����

        //osg::Node*  lod0 =  findFirstNode(node,"Lod0",findNodeVisitor::not_exact);
        osg::Node*  lod3 =  findFirstNode(node,"Lod3",findNodeVisitor::not_exact);

        int id = _physicsNodes.size();

        nm::manager_ptr man = nm::create_manager(lod3);

        aircraft::shassis_support_ptr s = boost::make_shared<aircraft::shassis_support_impl>(nm::create_manager(node));

        size_t pa_size = _phys_aircrafts.size();
        aircraft::phys_aircraft_ptr ac_ = aircraft::phys_aircraft_impl::create(
            cg::geo_base_3(cg::geo_point_3(0.0,0.0,0)),
            _sys,
            man,
            geo_position(cg::geo_point_3(0.000,0.005*(pa_size+1),0),cg::quaternion(cg::cpr(90, 0, 0))),
            ada::fill_data("BADA","A319"),                                                   
            s,
            0);


        _phys_aircrafts.emplace_back(aircraft_model(ac_,s));
        _sys->registerBody(id,ac_->get_rigid_body());

        //addPhysicsData( id, positioned, pos, /*vel*/osg::Vec3(0.0,0.0,0.0), mass );
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        mt->addChild( addGUIObject(node) );
        _root->addChild( mt.get() );

        _physicsNodes[id] = mt;
    }

    void RigidUpdater::addVehicle(osg::Node* node,const osg::Vec3& pos, const osg::Vec3& vel, double mass)
    {           
        // TODO FIXME � ��� ����� �������� ������� ���������� ����� ������ �����

        //osg::Node*  lod0 =  findFirstNode(node,"Lod0",findNodeVisitor::not_exact);
        osg::Node*  lod3 =  findFirstNode(node,"Lod3",findNodeVisitor::not_exact);

        int id = _physicsNodes.size();
        phys::ray_cast_vehicle::info_ptr veh = _sys->createVehicle(lod3?lod3:node,id,mass);
        
        // FIXME
        lod3->setNodeMask(0);

        //_sys->registerBody(id,phys::rigid_body_impl_ptr(veh)->get_body());
        
        _phys_vehicles.emplace_back(veh);

        //addPhysicsData( id, positioned, pos, /*vel*/osg::Vec3(0.0,0.0,0.0), mass );
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        mt->addChild( addGUIObject(node) );
        _root->addChild( mt.get() );

        _physicsNodes[id] = mt;

        veh->set_steer(10);
    }


    void RigidUpdater::addPhysicsBox( osg::Box* shape, const osg::Vec3& pos, const osg::Vec3& vel, double mass )
    {
        int id = _physicsNodes.size();
        _sys->createBox( id, shape->getHalfLengths(), mass );
        addPhysicsData( id, shape, pos, vel, mass );
    }

    void RigidUpdater::addPhysicsSphere( osg::Sphere* shape, const osg::Vec3& pos, const osg::Vec3& vel, double mass )
    {
        int id = _physicsNodes.size();
        _sys->createSphere( id, shape->getRadius(), mass );
        addPhysicsData( id, shape, pos, vel, mass );
    }

    struct frame_timer
    {
        inline frame_timer(osgViewer::View* view,double& last_frame_time)
            : last_frame_time(last_frame_time)
            , current_time(0)
        {
           current_time = view->getFrameStamp()->getSimulationTime();
        }

        inline double diff()
        {
          return current_time - last_frame_time ; 
        }

        inline ~frame_timer()
        {
           last_frame_time  = current_time;
        }

    private:
        double  current_time;
        double& last_frame_time;
    };


    bool RigidUpdater::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        osgViewer::View* view = static_cast<osgViewer::View*>( &aa );

        frame_timer ftm (view,_last_frame_time);

        if ( !view || !_root ) return false;
        
        switch ( ea.getEventType() )
        {
        case osgGA::GUIEventAdapter::KEYUP:
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Return )
            {
                osg::Vec3 eye, center, up, dir;
                view->getCamera()->getViewMatrixAsLookAt( eye, center, up );
                dir = center - eye; dir.normalize();
                addPhysicsSphere( new osg::Sphere(osg::Vec3(), 0.5f), eye, dir * 200.0f, 2.0 );
            } 
            else if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )
            {
                double steer  = phys::aircraft::control_ptr(_aircrafts[0])->get_steer();
                steer = cg::bound(cg::norm180(/*desired_course - cur_course*/++steer),-65., 65.);
                phys::aircraft::control_ptr(_aircrafts[1])->set_steer(steer);  

            }
            else if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )
            {
                double steer  = phys::aircraft::control_ptr(_aircrafts[0])->get_steer();
                steer = cg::bound(cg::norm180(/*desired_course - cur_course*/--steer),-65., 65.);
                phys::aircraft::control_ptr(_aircrafts[1])->set_steer(steer);
            }

            break;
        case osgGA::GUIEventAdapter::FRAME:
            {
                double dt = _hr_timer.get_delta();
                double dt1 = ftm.diff();

                if (abs(dt-dt1)>0.1) 
                    OutputDebugString("Simulation time differ from real time more the 0.1 sec\n");

                if( _dbgDraw)
                    _dbgDraw->BeginDraw();

                _sys->update( dt );

                for(auto it = _phys_aircrafts.begin();it!=_phys_aircrafts.end();++it)
                {   
#if 1
                    if((*it).traj.get())
                    {
                         if ((*it).traj->cur_len() < (*it).traj->get().length())
                         {
                             
                             (*it).aircraft->set_prediction(15.); 
                             (*it).aircraft->freeze(false);
                             const double  cur_len = (*it).traj->cur_len();
                             (*it).traj->des_len((*it).traj->cur_len() + dt*(*it).desired_velocity);
                             const double  tar_len = (*it).traj->cur_len();
                             decart_position target_pos;
                                                                          
                             target_pos.pos = cg::point_3((*it).traj->get().value(tar_len),0);
                             geo_position gtp(target_pos, cg::geo_base_3(cg::geo_point_3(0.0,0.0,0)));
                             (*it).aircraft->go_to_pos(gtp.pos ,gtp.orien);
                            
                             
                             const double curs_change = (*it).traj->get_curses().value(tar_len) - (*it).traj->get_curses().value(cur_len);

                             if(cg::eq(curs_change,0.0))
                                 (*it).desired_velocity = aircraft_model::max_desired_velocity;
                             else
                                 (*it).desired_velocity = aircraft_model::min_desired_velocity;

                             const decart_position cur_pos = _phys_aircrafts[0].aircraft->get_local_position();
                            
                             //std::stringstream cstr;

                             //cstr << std::setprecision(8) 
                             //     << "curr_pods_len:  "             << (*it).traj->cur_len() 
                             //     << "    desired_velocity :  "     << (*it).desired_velocity   
                             //     << "    delta curs :  "  << curs_change
                             //     << ";   cur_pos x= "     << cur_pos.pos.x << " y= "  << cur_pos.pos.y  
                             //     << "    target_pos x= "  << target_pos.pos.x << " y= "  << target_pos.pos.y <<"\n" ;

                             //OutputDebugString(cstr.str().c_str());
                        }
                        else
                        {
                                                     
                           cg::point_3 cur_pos = _phys_aircrafts[0].aircraft->get_local_position().pos;
                           cg::point_3 d_pos = _phys_aircrafts[0].aircraft->get_local_position().dpos;
                           cg::point_3 trg_p((*it).traj->get().value((*it).traj->get().length()),0);
                           d_pos.z = 0;
                           if(cg::distance(trg_p,cur_pos) > 1.0 && cg::norm(d_pos) > 0.05)
                           {   
                               decart_position target_pos;
                               target_pos.pos = trg_p;
                               geo_position gp(target_pos, cg::geo_base_3(cg::geo_point_3(0.0,0.0,0)));
                               (*it).aircraft->go_to_pos(gp.pos ,gp.orien);
                           }
                           else
                           {
                               // (*it).traj.reset();
                               (*it).aircraft->freeze(true);
                           }
                        }

                    }
#endif

                    (*it).aircraft->update();

                }                


                for ( NodeMap::iterator itr=_physicsNodes.begin();
                    itr!=_physicsNodes.end(); ++itr )
                {
                    osg::Matrix matrix = _sys->getMatrix(itr->first);
                    itr->second->setMatrix( matrix );
                }

                if( _dbgDraw)
                {
                    _sys->getScene()->debugDrawWorld();
                    _dbgDraw->EndDraw();
                }
            } 

            break;
        default: break;
        }
        return false;
    }


    void RigidUpdater::addPhysicsData( int id, osg::Shape* shape, const osg::Vec3& pos,
        const osg::Vec3& vel, double /*mass*/ )
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable( new osg::ShapeDrawable(shape) );

        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        mt->addChild( geode.get() );
        _root->addChild( mt.get() );

        _sys->setMatrix( id, osg::Matrix::translate(pos) );
        _sys->setVelocity( id, vel );
        _physicsNodes[id] = mt;
    }

    void RigidUpdater::addPhysicsData( int id, osg::Node* node, const osg::Vec3& pos,
        const osg::Vec3& vel, double /*mass*/ )
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        mt->addChild( node );
        _root->addChild( mt.get() );

        _sys->setMatrix( id, osg::Matrix::translate(pos) );
        _sys->setVelocity( id, vel );
        _physicsNodes[id] = mt;
    }


    void RigidUpdater::handlePointEvent(std::vector<cg::point_3> const &simple_route)
    {   
        decart_position target_pos;
        target_pos.pos = simple_route.back();
        //geo_position gp(target_pos, cg::geo_base_3(cg::geo_point_3(0.0,0.0,0)));
        decart_position cur_pos = _phys_aircrafts[0].aircraft->get_local_position();
        target_pos.orien = cg::cpr(cg::polar_point_2(target_pos.pos - cur_pos.pos).course,0,0);
        
        

        if (!_phys_aircrafts[0].traj)
        {
             _phys_aircrafts[0].traj = boost::make_shared<fms::trajectory>(cur_pos,target_pos,aircraft_model::min_radius(),aircraft_model::step());
        }
        else
        {  
            fms::trajectory_ptr main_ = _phys_aircrafts[0].traj;

            decart_position begin_pos(cg::point_3(main_->get().value(main_->get().length()),0)
                                     ,cg::cpr(main_->get_curses().value(main_->get().length()),0,0) );
            
            std::stringstream cstr;

            cstr << std::setprecision(8) 
                << "x:  "         << main_->get().value(main_->get().length()).x
                << "    y: "      << main_->get().value(main_->get().length()).y
                << "    curs :  " << main_->get_curses().value(main_->get().length()) 
                << "    angle:  " << cg::angle(target_pos.pos,begin_pos.pos) 
                << "    angle:  " << cg::polar_point_2(target_pos.pos - begin_pos.pos).course 
                << "\n" ;

            OutputDebugString(cstr.str().c_str());

            target_pos.orien = cg::cpr(cg::polar_point_2(target_pos.pos - begin_pos.pos).course,0,0);
            
            fms::trajectory_ptr traj = boost::make_shared<fms::trajectory>( begin_pos,
                                                                            target_pos,
                                                                            aircraft_model::min_radius(),
                                                                            aircraft_model::step());

            main_->append(*traj.get());
        }
       
        _trajectory_drawer->set(_phys_aircrafts[0].traj->get());
        
        //_phys_aircrafts[0].aircraft->go_to_pos(gp.pos ,gp.orien);
        
    }

}