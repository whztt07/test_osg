#pragma once    

class heilVisitor : public osg::NodeVisitor
    {  

    public:
        heilVisitor(std::ofstream& filelogic)  
            :_level(0)
            , osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
            , root_visited(false)
            , lod_visited (false)
			, got_lod(false)
            , ostream_nodes(filelogic)
        {
             using namespace binary;
            // fill default
            //write(ostream_nodes, 1);
            auto d = wrap(binary::size_type(1));
            ostream_nodes.write(raw_ptr(d), size(d));//1 transform before root
        }

        std::string spaces()
        { return std::string(_level*2, ' '); }
        
        size_t num_children(osg::Group& node)
        {
            size_t num = 0;

            for (unsigned i = 0; i < node.getNumChildren(); i++) //lod level
            {
                if (node.getChild(i)->asTransform() || node.getChild(i)->asGroup())
                    ++num;
            }
            return num;
        }

        void apply( osg::Node& node )
        {   
            using namespace binary;
            std::cout << spaces() << node.libraryName() << "::" << node.className() << " : " << node.getName() << std::endl;
            std::string node_name = boost::to_lower_copy(node.getName());
            std::string name_cut = node_name.substr(0, node_name.find("_lod"));
            std::string name_first_part = name_cut.substr(0, name_cut.find("_")); 

            if(node_name == "root")
               root_visited = true;
            
            FIXME("�������")
            
            //if(boost::starts_with(node_name,"lod") && name_cut == name_first_part && name_first_part != "lod0")
            //   return;
            
            if(boost::starts_with(node_name,"lod") && lod_visited)
                return;
            else
                if(boost::starts_with(node_name,"lod0"))
                {
                    lod_visited = true;
                }
				else if(boost::starts_with(node_name,"lod"))
					got_lod = true;

            if (root_visited && (node.asTransform() || node.asGroup() ))
            {

                model_structure::node_data new_node;
            
                const binary::size_type  children_count = got_lod?1:(node.asGroup()?num_children(*node.asGroup()):0);
                
				if(got_lod) got_lod=false;
                
				new_node.pos = from_osg_vector3(node.asTransform()->asMatrixTransform()->getMatrix().getTrans());
                new_node.orien = from_osg_quat(node.asTransform()->asMatrixTransform()->getMatrix().getRotate());
            
           

                new_node.name = name_cut; // ���������� ���. ����� ��� ��� �����

                {
                    auto d = wrap(new_node);
                    ostream_nodes.write(raw_ptr(d), size(d));
                }

                {
                    auto d = wrap(children_count);// print root
                    ostream_nodes.write(raw_ptr(d), size(d));
                }

                std::stringstream cstr;

                cstr << std::setprecision(8) 
                    << "--------------------------------------- \n"
                    << spaces() << "   Node /Name=   "          << new_node.name << "\n"
                    << spaces() << "   Translate =   ("         << new_node.pos.x  << " ," << new_node.pos.y << " ,"  << new_node.pos.z << " )" << "\n"
                    << spaces() << "   Rotate    =   ("         << new_node.orien.get_course()  << " ," << new_node.orien.get_pitch() << " ,"  << new_node.orien.get_roll() << " )" << "\n"
                    << spaces() << "   Logic children number: " << children_count << "\n";

                OutputDebugStringA(cstr.str().c_str());

            }

            _level++;
            traverse( node );
            _level--;
        }


        void apply( osg::Geode& geode )
        {
            std::cout << spaces() << geode.libraryName() << "::" << geode.className() << " : " << geode.getName() << std::endl;
            
            std::stringstream cstr;

            //cstr << std::setprecision(8) 
            //    << "physics x:  "         << cur_pos.pos.x
            //    << "\n" ;

            //OutputDebugString(cstr.str().c_str());

            _level++;
            for ( unsigned int i=0; i<geode.getNumDrawables(); ++i )
            {
                osg::Drawable* drawable = geode.getDrawable(i);
                std::cout << spaces() << drawable->libraryName() << "::" << drawable->className() << " : " << geode.getName() << std::endl;
            }

            traverse( geode );
            _level--;
        }

        //vector<char> get_model_data()
        //{
        //     return ostream_nodes.detach();
        //}

    protected:
        unsigned int _level;
        /*binary::output_stream*/std::ofstream& ostream_nodes;
        bool root_visited;
        bool lod_visited;
		bool got_lod;
    };