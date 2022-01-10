#include <iostream>
#include <string>
#include "../hxf/log.h"
#include "../hxf/util.h"
#include "../hxf/singleton.h"
#include "../hxf/config.h"

#include <yaml-cpp/yaml.h>

const char* path = "/home/busy/workspace/hxf/bin/conf/test.yml";

 hxf::ConfigVar<int>::ptr g_int_value_config
     = hxf::Config::Lookup("system.port", (int)8080, "system port");
 hxf::ConfigVar<float>::ptr g_float1_value_config
     = hxf::Config::Lookup("system.port1", (float)8080, "system port1");

 hxf::ConfigVar<float>::ptr g_float_value_config
     = hxf::Config::Lookup("system.value", (float)10.2f, "system value");

 hxf::ConfigVar<std::vector<int>>::ptr g_vec_int_value_config
     = hxf::Config::Lookup("system.vec_int", std::vector<int>{1, 2}, "system vec_int");

 hxf::ConfigVar<std::list<int>>::ptr g_list_int_value_config
     = hxf::Config::Lookup("system.list_int", std::list<int>{1, 2}, "system list_int");

 hxf::ConfigVar<std::set<int>>::ptr g_set_int_value_config
     = hxf::Config::Lookup("system.set_int", std::set<int>{1, 2}, "system set_int");

 hxf::ConfigVar<std::unordered_set<int>>::ptr g_uset_int_value_config
     = hxf::Config::Lookup("system.uset_int", std::unordered_set<int>{1, 2}, "system uset_int");

 hxf::ConfigVar<std::map<std::string, int>>::ptr g_map_int_value_config
     = hxf::Config::Lookup("system.map_int", std::map<std::string, int>{{"k", 1},{"l",2}}, "system map_int");

 hxf::ConfigVar<std::unordered_map<std::string, int>>::ptr g_umap_int_value_config
     = hxf::Config::Lookup("system.umap_int", std::unordered_map<std::string, int>{{"k", 1},{"l",2}}, "system map_int");

void print_yaml(const YAML::Node& node, int level) {
    if(node.IsScalar()) {
        HXF_LOG_DEBUG(HXF_LOG_ROOT()) << std::string(level * 4, ' ')
            << node.Scalar() << " - " << node.Type() << " - " << level;
    } else if(node.IsNull()) {
        HXF_LOG_DEBUG(HXF_LOG_ROOT()) << std::string(level * 4, ' ')
            << "NULL - " << node.Type() << " - " << level;
    } else if(node.IsMap()) {
        for (auto it = node.begin();
                it != node.end(); ++it) {
            HXF_LOG_DEBUG(HXF_LOG_ROOT()) << std::string(level * 4, ' ')
                << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    } else if(node.IsSequence()) {
        for (size_t i = 0; i < node.size(); ++i) {
            HXF_LOG_INFO(HXF_LOG_ROOT()) << std::string(level * 4, ' ')
                << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml() {
    YAML::Node root = YAML::LoadFile(path); 
    //print_yaml(root, 0);
    hxf::Config::LoadFromYaml(root);
}

#define XX(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) { \
            HXF_LOG_INFO(HXF_LOG_ROOT()) << #prefix " " #name ": " << i; \
        } \
    }

#define XX_M(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) { \
            HXF_LOG_INFO(HXF_LOG_ROOT()) << #prefix " " #name ": {" << i.first \
            << " - " << i.second << "}"; \
        } \
    }

void test_config() {
    XX(g_vec_int_value_config, vec_int, before);
    XX(g_list_int_value_config, list_int, before);
    XX(g_set_int_value_config, set_int, before);
    XX(g_uset_int_value_config, uset_int, before);
    XX_M(g_map_int_value_config, map_int, before);
    XX_M(g_umap_int_value_config, umap_int, before);

    YAML::Node root = YAML::LoadFile(path); 
    hxf::Config::LoadFromYaml(root);

    XX(g_vec_int_value_config, vec_int, after);
    XX(g_list_int_value_config, list_int, after);
    XX(g_set_int_value_config, set_int, after);
    XX(g_uset_int_value_config, uset_int, after);
    XX_M(g_map_int_value_config, map_int, after);
    XX_M(g_umap_int_value_config, umap_int, after);
    

}

class Person {
public:
    Person() {};
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           << "]";
        return ss.str();
    }

    bool operator==(const Person& oth) const {
        return m_name == oth.m_name
            && m_age == oth.m_age
            && m_sex == oth.m_sex;
    }
};

namespace hxf {

template<>
class LexicalCast<std::string, Person> {
public:
    Person operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template<>
class LexicalCast<Person, std::string> {
public:
    std::string operator()(const Person& p) {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

}


hxf::ConfigVar<Person>::ptr g_person =
    hxf::Config::Lookup("class.person", Person(), "system person");

hxf::ConfigVar<std::map<std::string, Person> >::ptr g_person_map =
    hxf::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

hxf::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map =
    hxf::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person> >(), "system person");




void test_class() {
    HXF_LOG_INFO(HXF_LOG_ROOT()) << "before: " << g_person->getValue().toString() << " - " << g_person->toString();

#define XX_PM(g_var, prefix) \
    { \
        auto m = g_person_map->getValue(); \
        for(auto& i : m) { \
            HXF_LOG_INFO(HXF_LOG_ROOT()) <<  prefix << ": " << i.first << " - " << i.second.toString(); \
        } \
        HXF_LOG_INFO(HXF_LOG_ROOT()) <<  prefix << ": size=" << m.size(); \
    }

    g_person->addListener([](const Person& old_value, const Person& new_value){
        HXF_LOG_INFO(HXF_LOG_ROOT()) << "old_value=" << old_value.toString()
                << " new_value=" << new_value.toString();
    });

    XX_PM(g_person_map, "class.map before");
    HXF_LOG_INFO(HXF_LOG_ROOT()) << "before: " << g_person_vec_map->toString();

    YAML::Node root = YAML::LoadFile(path);
    hxf::Config::LoadFromYaml(root);

    HXF_LOG_INFO(HXF_LOG_ROOT()) << "after: " << g_person->getValue().toString() << " - " << g_person->toString();
    XX_PM(g_person_map, "class.map after");
    HXF_LOG_INFO(HXF_LOG_ROOT()) << "after: " << g_person_vec_map->toString();
}

void test_log() {
    std::cout << hxf::LoggerMgr::GetInstance()->toYamlString() << std::endl;    

    const char* logfile = "/home/busy/workspace/hxf/bin/conf/log.yml";
    YAML::Node root = YAML::LoadFile(logfile);
    hxf::Config::LoadFromYaml(root);    
    std::cout << "==============" << std::endl;
    std::cout << hxf::LoggerMgr::GetInstance()->toYamlString() << std::endl;    
    HXF_LOG_INFO(HXF_LOG_NAME("root")) << "this is a test, test log config.";
}

int main() {

//  HXF_LOG_DEBUG(HXF_LOG_ROOT()) << "before "<< g_int_value_config.get();
//  HXF_LOG_DEBUG(HXF_LOG_ROOT()) << g_float1_value_config.get();
//  if (g_float_value_config) {
//  HXF_LOG_INFO(HXF_LOG_ROOT()) << g_float_value_config->toString();
//  HXF_LOG_INFO(HXF_LOG_ROOT()) << g_float_value_config->getValue();
//  }
  
    //test_config();
    //test_class();
    test_log();
    return 0;
}
