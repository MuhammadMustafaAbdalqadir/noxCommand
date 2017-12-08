#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <windows.h>
#include <dirent.h>
#include <tchar.h>
#include <vector>

#define pp push_back
#define nl printf("\n")
#define pvec(a) { for (int TTT = 0; TTT < a.size(); TTT++) cout << a[TTT] << " "; }

using namespace std;


int bad[100000] = { 0 };

string current_path, user;

vector < string > list_dir(string str){
    vector < string > res;

    const char *path = str.c_str();
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) return res;

    while ((entry = readdir(dir)) != NULL)
        res.pp(entry->d_name);

    closedir(dir); return res;
}


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

// -----------------//}

// read_from_user
vector < vector < string > > rfu(){
    vector < vector < string > > res;
    getline(cin, user);

    user = clean(user);
    user = prune(user);

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

void print_in_dir(vector < string > v){
    for (int i = 0 ; i < v.size(); i ++)
        ffp("-->" + v[i] + "\n");
}

/*
        if (user == "clear"){
            system("CLS");
        }
        else if (user == "list"){
            print_in_dir(  list_dir( current_path )  );   nl;
        }
        else if (user == "change"){

        }
        else if (user == "help"){
            ffp("-->Coming Soon!\n"); nl;
        }
        else {
            ffp("-->Something gone wrong dude!\n"); nl;
        }
*/

void add_in_bad(string str){
    for (int i = 0 ; i < str.size(); i ++)
        bad[str[i]] = 1;
}

int main(){

    system("color A");
    get_home_dir();

    ffp("noxCommand [Version 1.0]\n");
    ffp("(c) 2017 noxhollow (MuhammadMustafa). All rights reserved.\n\n");
    ffp("Available commands <list, search, change, create, rename, move, copy, delete, clear, help>\n");
    ffp("Remember that you can type help then the command name!\n\n\n\n\n\n");

    add_in_bad("\n");


    while (true){
        ffp("->noxCommand in <" + current_path + ">\n$ ");

        vector < vector < string > > cmds = rfu();

        for (int x = 0 ; x < cmds.size(); x ++){
            vector < string > now = cmds[x]; int n = now.size();

            pvec(now); nl;
        }
    }

    return 0;
}
