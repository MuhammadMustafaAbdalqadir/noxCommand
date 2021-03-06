#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <windows.h>
#include <dirent.h>
#include <tchar.h>
#include <vector>
#include <string.h>
#include <sstream>
#include <stack>
#include <direct.h>
#include <unordered_map>
#include <set>
#include <map>

#include "Parser.h"
#include "Message.h"
#include "Volume.h"

#define pp push_back
#define nl printf("\n")
#define pvec(a) { for (int TTT = 0; TTT < a.size(); TTT++) cout << a[TTT] << " "; }

using namespace std;


int bad[100000] = { 0 };

string current_path, user;
stack < string > st;

/*

|__ Data
| |__ bate5a.exe
| |__ borto2al.bat
|__ Images
  |__ acm
    |__ acpc15
      |__ img213.png



      Tree? -------------------//
*/

bool is_dir_wrong(string str){
    const char *path = str.c_str();
    struct dirent *entry;
    DIR *dir = opendir(path);
    int is = (dir == NULL); return is;
}

vector < string > list_dir(string str){
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


void make_dir(string path, string name){ // make sure path is valid dir path***
    path += ("\\" + name);
    if(!CreateDirectory(path.c_str() ,NULL)) return;
}

// can use for copying, renaming, moving, ...files
bool copy_file(string from, string to){ // make sure that everything is ok ***
    return CopyFile( from.c_str(), to.c_str(), FALSE );
}

typedef pair < bool, vector < string > > value; // is_dir, sons

unordered_map < string, value > tree;
map < string, vector < string > > names;

bool is_in_tree(string path){
    auto it = tree.find(path);
    if (it == tree.end()) return false;
    else return true;
}

void store(string name, string path){
    auto it = names.find(name);
    if (it == names.end()) {
        vector < string > temp;
        names[name] = temp;
    }
    names[name].push_back(path);
}

void pre_clone(){ tree.clear(); names.clear(); }

void clone(string path){
    if (is_in_tree(path)) return;

    value val = make_pair(!is_dir_wrong(path), list_dir(path));
    tree[path] = val;

    if (!val.first) return;

    vector < string > v = val.second;
    int n = v.size();

    for (int i = 0 ; i < n ; i ++)
        store(v[i], path + "\\" + v[i]), clone(path + "\\" + v[i]);
}

bool del_file(string path){ //***
    if(remove(path.c_str()) != 0)
        return false;
    else return true;
}

bool rename(string path, string oldname, string newname){
    int result;
    result = rename( (path + "\\" + oldname).c_str() , (path + "\\" + newname).c_str() );
    if ( result == 0 ) return true;
    else return false;
}

bool del_dir_if_empty(string path){ // make sure that the dir exists ***
    if(_rmdir(path.c_str()) != -1) return true;
    else return false;
}

void delete_the_tree(string path){ //The tree must be updated before using this function!
    clone(path);

    value val = tree[path];
    int isdir = val.first;

    if (isdir){
        vector < string > v = val.second;
        if (v.empty()){
            del_dir_if_empty(path); return;
        }
        else {
            int n = v.size();
            for (int i = 0 ; i < n ; i ++)
                delete_the_tree(path + "\\" + v[i]);
            del_dir_if_empty(path); return;
        }
    }
    else {
        del_file(path); return;
    }
}

void copy_the_tree(string from, string to){    //The tree must be updated before using this function!
    clone(from);

    value val = tree[from];
    int isdir = val.first;
    vector < string > v = val.second; int n = v.size();

    for (int i = 0 ; i < n ; i ++){
        if (tree[from + "\\" + v[i]].first){ // isdir
            make_dir(to, v[i]);
            copy_the_tree(from + "\\" + v[i], to + "\\" + v[i]);
        }
        else {
            copy_file(from + "\\" + v[i], to + "\\" + v[i]);
        }
    }
}

string get_name_of_file(string from){
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

// assuming that every thing with paths is okay
//
bool copy_dir(string from, string to){

    if (is_dir_wrong(from) || is_dir_wrong(to)) return false;

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

    string temp; start--;
    for (int i = 0 ; i < start; i ++)
        temp.pp(from[i]);

    from = temp;

    if (!is_dir_wrong(to + "\\" + name)) return false;

    make_dir(to, name);

    pre_clone(); clone(from); clone(to);

    copy_the_tree(from + "\\" + name, to + "\\" + name);

    return true;
}

//-----------------------------//


// prepossessing ---//{

string clean(string str){
    string res = ""; int n = str.length();
    for (int i = 0 ; i < n ; i ++){
        if (bad[str[i]]) continue;
        else res.pp(str[i]);
    } return res;
}

string prune(string str){
    string res = ""; int n = str.length();
    int start = 0, end = n - 1;

    for (int i = 0 ; i < n; i ++){
        if (str[i] == '-'){ start = i+1; break; }
        else continue;
    }

    for (int i = (n - 1); i >= 0 ; i --){
        if (str[i] == '.') { end = i; str[i] = '&'; break; }
        else continue;
    }

    for (int i = start; i <= end; i ++)
        res.pp(str[i]);

    return res;
}

string get_name_or_path(string str){
    string res; int n = str.length();
    for (int i = 1 ; i < (n-1); i ++)
        res.pp(str[i]); return res;
}

// -----------------//}

bool is_name_or_path(string str){
    int n = str.length();
    return (str[0] == '<') & (str[n-1] == '>') ;
}

// read_from_user
vector < vector < string > > rfu(){
    vector < vector < string > > res;
    getline(cin, user);

    user = clean(user);
    user = prune(user);

    // - [command] & [command] & [command] .

    int n = user.length(), is_group = 0;
    vector < string > v; string s;

    for (int i = 0 ; i < n; i ++){
        if (user[i] == ' '){
            if (is_group){
                s.pp(user[i]);
            }
            else {
                if(!s.empty()) v.pp(s), s = "";
            }
        }
        else if (user[i] == '<'){
            if(!s.empty()) v.pp(s), s = "";
            s.pp(user[i]);
            is_group = 1;
        }
        else if (user[i] == '>'){
            is_group = 0;
            s.pp(user[i]);

            if(!s.empty()) v.pp(s), s = "";
        }
        else if (user[i] == '&'){
            if(!s.empty()) v.pp(s), s = "";
            if (!v.empty()) res.pp(v), v.clear();
        }
        else {
            s.pp(user[i]);
        }
    }

    return res;
}

// Fast Fancy printing
void ffp(string str){
    int n = str.length();
    for (int i = 0 ; i < n; i ++){
        cout << str[i]; Sleep(0);
    }
}
// just Fancy printing
void fp(string str){
    int n = str.length();
    for (int i = 0 ; i < n; i ++){
        cout << str[i]; Sleep(20);
    }
}


void get_home_dir(){
    current_path = getenv("USERPROFILE");
}

string ret_home_dir(){
    return getenv("USERPROFILE");
}

void print_in_the_dir(vector < string > v){
    for (int i = 0 ; i < v.size(); i ++)
        ffp("-->" + v[i] + "\n");
}

void add_in_bad(string str){
    for (int i = 0 ; i < str.size(); i ++)
        bad[str[i]] = 1;
}

string toString(int n){
    string res; stringstream ss;
    ss << n; ss >> res; return res;
}

void make_file(string path, string name){ // make sure path is valid dir path***
    ifstream in; in.open(path + "\\" + name);
    if(in.fail()){
        ofstream test(path + "\\" + name); test.close();
    }
    in.close();
}

void print_in_the_list(vector < string > v){
    for (int i = 0 ; i < v.size(); i ++)
        ffp("-->" + v[i] + "\n");
}

inline void wrong(int n){
    ffp("-->Something gone wrong with command number " + toString(n) + "!\n");
}

inline void wrong_path(int n){
    ffp("-->Path is wrong in command number " + toString(n) + "!\n");
}

inline void file_deleted_msg(int n){
    ffp("-->File deleted successfully in command number " + toString(n) + "!\n");
}

inline void file_not_deleted_msg(int n){
    ffp("-->Something gone wrong with deleting in command number " + toString(n) + "!\n");
}

inline void dir_deleted_msg(int n){
    ffp("-->Directory deleted successfully in command number " + toString(n) + "!\n");
}

int is_path(string str){
    int n = str.length(), is = 0;
    for (int i = 0 ; i < n ; i ++)
        is |= (str[i] == '\\');
    return is;
}


int main(){

    //-------------------------------------------Testing Area





    //-----------------------------------------------------//

    system("color A");
    get_home_dir();

    ffp("noxCommand [Version 1.0]\n");
    ffp("(c) 2017 noxhollow (Muhammad Mustafa). All rights reserved.\n\n");
    ffp("Available commands <list, search, go, make, rename, copy, cut, del, clear, help, exit>\n");
    ffp("-.\n");
    ffp("Remember that you can type help then the command name!\n\n\n\n\n\n");

    add_in_bad("\n\t"); int CON = 1;




    while (CON){


        if (is_dir_wrong(current_path)) get_home_dir();


        ffp("->noxCommand in <" + current_path + ">\n$ ");

        vector < vector < string > > cmds = rfu();

        for (int x = 0 ; x < cmds.size(); x ++){

            vector < string > now = cmds[x]; int n = now.size();

            //pvec(now); cout << n << " "; nl;






            if (now[0] == "clear"){
                if (n == 1) system("CLS");
                else { wrong(x+1); nl; }
            }





            else if (now[0] == "list"){
                    /*
                        -list.
                        -list <path>.
                    */
                if (n == 1){
                    print_in_the_dir(  list_dir( current_path )  );   nl;
                }
                else if (n == 2){
                    if (is_name_or_path(now[1])){
                        string p = get_name_or_path(now[1]);
                        if (is_dir_wrong(p)){
                            wrong_path(x+1); nl;
                        }
                        else {
                            print_in_the_dir(  list_dir( p )  );   nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }





            else if (now[0] == "go"){
                    /*
                        -go prev.
                        -go home.
                        -go <path>.
                        -go <dir_name>.
                    */
                if (n == 2){
                    if (now[1] == "prev"){
                        if (st.empty()){
                            wrong(x+1); nl;
                        }
                        else {
                            current_path = st.top();
                            st.pop();
                        }
                    }
                    else if (now[1] == "home"){
                        if (ret_home_dir() != current_path){
                            st.push(current_path);
                            current_path = ret_home_dir();
                        }
                    }
                    else if (is_name_or_path(now[1])){
                        string p = get_name_or_path(now[1]);
                        if (is_dir_wrong(p)){
                                string pp = current_path + "\\" + p;
                            if (is_dir_wrong(pp)){
                                wrong(x+1); nl;
                            }
                            else {
                                if (pp != current_path){
                                    st.push(current_path);
                                    current_path = pp;
                                }
                            }
                        }
                        else {
                            if (p != current_path){
                                st.push(current_path);
                                current_path = p;
                            }
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }





            else if (now[0] == "make"){
                /*
                   -make dir <path> <name>.
                   -make dir <name>.
                   -make file <path> <name>.
                   -make file <name>.
                */
                if (n == 3){
                    if (now[1] == "dir"){
                        if (is_name_or_path(now[2])){
                            make_dir(current_path, get_name_or_path(now[2]));
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else if (now[1] == "file"){
                        if (is_name_or_path(now[2])){
                            make_file(current_path, get_name_or_path(now[2]));
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                /*
                   -make dir <path> <name>.
                   -make dir <name>.
                   -make file <path> <name>.
                   -make file <name>.
                */
                else if (n == 4){
                    if (now[1] == "dir"){
                        if (is_name_or_path(now[2])){
                            if (is_name_or_path(now[3])){
                                make_dir(get_name_or_path(now[2]), get_name_or_path(now[3]));
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else if (now[1] == "file"){
                        if (is_name_or_path(now[2])){
                            if (is_name_or_path(now[3])){
                                make_file(get_name_or_path(now[2]), get_name_or_path(now[3]));
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }





            else if (now[0] == "del"){
                    /*
                        -del file <name>.
                        -del file <path> <name>.
                        -del dir <name>.
                        -del dir <path> <name>.
                    */
                    if (n == 3){
                        if (now[1] == "file"){
                            if (is_name_or_path(now[2])){
                                int is = del_file(current_path + "\\" + get_name_or_path(now[2]));
                                if (is == 1) { file_deleted_msg(x+1); nl; }
                                else { file_not_deleted_msg(x+1); nl; }
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else if (now[1] == "dir"){
                            if (is_name_or_path(now[2])){

                                pre_clone(); clone(current_path + "\\" + get_name_or_path(now[2]));
                                   delete_the_tree(current_path + "\\" + get_name_or_path(now[2]));

                                /*int is = del_dir_if_empty(current_path + "\\" + get_name_or_path(now[2]));
                                if (is == 1) { dir_deleted_msg(x+1); nl; }
                                else { file_not_deleted_msg(x+1); nl; }*/
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else if (n == 4){
                        if (now[1] == "file"){
                            if (is_name_or_path(now[2])){
                                if (is_name_or_path(now[3])){
                                    int is = del_file(get_name_or_path(now[2]) + "\\" + get_name_or_path(now[3]));
                                    if (is == 1) { file_deleted_msg(x+1); nl; }
                                    else { file_not_deleted_msg(x+1); nl; }
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else if (now[1] == "dir"){
                            if (is_name_or_path(now[2])){
                                if (is_name_or_path(now[3])){

                                    pre_clone(); clone(get_name_or_path(now[2]) + "\\" + get_name_or_path(now[3]));
                                       delete_the_tree(get_name_or_path(now[2]) + "\\" + get_name_or_path(now[3]));

                                    /*int is = del_dir_if_empty(get_name_or_path(now[2]) + "\\" + get_name_or_path(now[3]));
                                    if (is == 1) { dir_deleted_msg(x+1); nl; }
                                    else { file_not_deleted_msg(x+1); nl; }*/
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else {
                                wrong(x+1); nl;
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
            }







            else if (now[0] == "copy"){
                /*              from    to
                    -copy file <path> <path>.
                    -copy dir <path> <path>.
                    -copy file <name> <path>.
                    -copy dir <name> <path>.
                */
                if (n == 4){
                    if (now[1] == "file"){
                        if (is_name_or_path(now[2]) && is_name_or_path(now[3])){
                            if (is_path(now[2])){
                                if (is_path(now[3])){
                                    copy_file(get_name_or_path(now[2]), get_name_or_path(now[3]) + "\\" + get_name_of_file(get_name_or_path(now[2])));
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else { // name
                                if (is_path(now[3])){
                                    copy_file(current_path + "\\" + get_name_or_path(now[2]), get_name_or_path(now[3]) + "\\" + get_name_or_path(now[2]));
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else if (now[1] == "dir"){
                        if (is_name_or_path(now[2]) && is_name_or_path(now[3])){
                            if (is_path(now[2])){
                                if (is_path(now[3])){
                                    int is = copy_dir(get_name_or_path(now[2]), get_name_or_path(now[3]));
                                    if (!is) { wrong(x+1); nl; }
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else { // name
                                if (is_path(now[3])){
                                    int is = copy_dir(current_path + "\\" + get_name_or_path(now[2]), get_name_or_path(now[3]));
                                    if (!is) { wrong(x+1); nl; }
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }





            else if (now[0] == "cut"){
                /*              from    to
                    -cut file <path> <path>.
                    -cut dir <path> <path>.
                    -cut file <name> <path>.
                    -cut dir <name> <path>.
                */

                if (n == 4){
                    if (now[1] == "file"){
                        if (is_name_or_path(now[2]) && is_name_or_path(now[3])){
                            if (is_path(now[2])){
                                if (is_path(now[3])){
                                    copy_file(get_name_or_path(now[2]), get_name_or_path(now[3]) + "\\" + get_name_of_file(get_name_or_path(now[2])));
                                    del_file(get_name_or_path(now[2]));
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else { // name
                                if (is_path(now[3])){
                                    copy_file(current_path + "\\" + get_name_or_path(now[2]), get_name_or_path(now[3]) + "\\" + get_name_or_path(now[2]));
                                    del_file(current_path + "\\" + get_name_or_path(now[2]));
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else if (now[1] == "dir"){
                        if (is_name_or_path(now[2]) && is_name_or_path(now[3])){
                            if (is_path(now[2])){
                                if (is_path(now[3])){
                                    int is = copy_dir(get_name_or_path(now[2]), get_name_or_path(now[3]));
                                    if (!is) { wrong(x+1); nl; }
                                    else {
                                        pre_clone(); clone(get_name_or_path(now[2]));
                                           delete_the_tree(get_name_or_path(now[2]));
                                    }
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                            else { // name
                                if (is_path(now[3])){
                                    int is = copy_dir(current_path + "\\" + get_name_or_path(now[2]), get_name_or_path(now[3]));
                                    if (!is) { wrong(x+1); nl; }
                                    else {
                                        pre_clone(); clone(current_path + "\\" + get_name_or_path(now[2]));
                                           delete_the_tree(current_path + "\\" + get_name_or_path(now[2]));
                                    }
                                }
                                else {
                                    wrong(x+1); nl;
                                }
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }




            else if (now[0] == "rename"){
                /*
                            from    to
                    -rename <name> <name>.
                */
                if (n == 3){
                    if (is_name_or_path(now[1]) && is_name_or_path(now[2])){
                        int is = rename(current_path, get_name_or_path(now[1]), get_name_or_path(now[2]));
                        if (!is) { wrong(x+1); nl; }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }





            else if (now[0] == "search"){
                /*
                    -search <name> <path>.
                */
                if (n == 3){
                    if (is_name_or_path(now[1]) && is_name_or_path(now[2])){
                        string name = get_name_or_path(now[1]);
                        string path = get_name_or_path(now[2]);
                        if (!is_path(name) && is_path(path)){
                            if (is_dir_wrong(path)){
                                wrong(x+1); nl;
                            }
                            else { // here
                                pre_clone(); clone(path); store(get_name_of_file(path), path);

                                //for (auto itt = names.begin(); itt != names.end(); itt++)
                                    //cout << (*itt).first << endl;

                                auto it = names.find(name);
                                if (it == names.end()){
                                    ffp(name + " not found.\n");
                                }
                                else {
                                    ffp(name + " found in ...\n");
                                    print_in_the_list(names[name]); nl;
                                }
                            }
                        }
                        else {
                            wrong(x+1); nl;
                        }
                    }
                    else {
                        wrong(x+1); nl;
                    }
                }
                else {
                    wrong(x+1); nl;
                }
            }




            else if (now[0] == "help"){
                    // -help <command_name>.
                ffp("-->Coming Soon!\n"); nl;
            }




            else if (now[0] == "exit"){
                //-exit.
                CON = 0; break;
            }




            else {
                wrong(x+1); nl;
            }
        }
    }

    return 0;
}
