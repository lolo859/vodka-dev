    for (int i=0;i<cell.content.size();++i) {
        vodka::analyser::line localline;
        localline.line=cell.start.line+i+1;
        localline.file=file;
        localline.content=cell.content[i];
        auto direct_declared_data=split(cell.content[i]," ");
        if (direct_declared_data.size()>2) {
            if (direct_declared_data[0]!="vodka") {
                direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+1);
                for (auto arg:direct_declared_data) {
                    if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::vodint intele;
                            intele.varinfo.algo_dependant=false;
                            intele.varinfo.is_vodka_constant=true;
                            intele.varinfo.in_data_section=true;
                            intele.varinfo.define=true;
                            intele.varinfo.uuid=to_string(vodka::utilities::genuid());
                            intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::element contele;
                            contele.intele=intele;
                            contele.thing="vodint";
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return -1;
                        }
                    } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::vodec decele;
                            decele.varinfo.algo_dependant=false;
                            decele.varinfo.is_vodka_constant=true;
                            decele.varinfo.in_data_section=true;
                            decele.varinfo.define=true;
                            decele.varinfo.uuid=to_string(vodka::utilities::genuid());
                            decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::element contele;
                            contele.decele=decele;
                            contele.thing="vodec";
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return -1;
                        }
                    }
                }
            }
        }
    }
