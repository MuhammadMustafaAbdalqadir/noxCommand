#include "Volume.h"



Volume::Volume(string path_){
    path = path_;

    if (is_valid_path(path)){
        name = name_of_path(path);

        if (is_dir(path)){
            type = "dir";
            sons = sons_of_dir(path);
        }
        else {
            type = "file"; sons.clear();
        }
    }

    else {
        name = ""; path = ""; type = "nth"; sons.clear();
    }
}

void Volume::update(){

    if (is_valid_path(path)){

        if (type == "dir"){
            sons = sons_of_dir(path);
        }
    }

    else {
        name = ""; path = ""; type = "nth"; sons.clear();
    }
}


string Volume::home_path(){
    return getenv("USERPROFILE");
}


bool Volume::is_dir(string str){
    const char *path = str.c_str();
    struct dirent *entry;
    DIR *dir = opendir(path);
    int is = (dir == NULL); return (!is);
}


bool Volume::is_file(string str){
    int is = 1;
    ifstream in; in.open(str);
    if(in.fail()) is = 0;
    in.close(); return is;
}


bool Volume::is_valid_path(string str){
    int n = str.length(), is = 0;
    for (int i = 0 ; i < n ; i ++)
        is |= (str[i] == '\\');                         //Issue***
    if (is) return (is_file(str) | is_dir(str));
    else return false;
}


string Volume::name_of_path(string str){
    string from = str;

    int n = from.length();
    int start = n - 1;
    for (int i = (n-1); i >= 0; i --){
        if (from[i] == '\\') break;
        start = i;
    }

    string name;
    for (int i = start; i < n; i ++){
        name.pp(from[i]);
    }

    return name;
}


vector < string > Volume::sons_of_dir(string str){
    vector < string > res;

    const char *path = str.c_str();
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) return res;

    while ((entry = readdir(dir)) != NULL)
        res.pp(entry->d_name);

    closedir(dir);

    vector < string > res2;

    for (int i = 0 ; i < res.size(); i ++){
        if (res[i] == "." || res[i] == "..") continue;
        else {
            res2.pp(res[i]);
        }
    }

    return res2;
}


void Volume::set_path(string path_){
    path = path_;

    if (is_valid_path(path)){
        name = name_of_path(path);

        if (is_dir(path)){
            type = "dir";
            sons = sons_of_dir(path);
        }
        else {
            type = "file"; sons.clear();
        }
    }

    else {
        name = ""; path = ""; type = "nth"; sons.clear();
    }
}


string Volume::get_name(){ return name; }
string Volume::get_path(){ return path; }
string Volume::get_type(){ return type; }
vector < string > Volume::get_sons(){ return sons; }
