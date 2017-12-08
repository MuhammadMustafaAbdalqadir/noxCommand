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
        if (isalpha(str[i])){ start = i; break }
        else continue;
    }

    for (int i = (n - 1); i >= 0 ; i --){
        if (str[i] == '.') { end = i; break; }
        else continue;
    }

    return res;
}

// -----------------//}

// read_from_user
vector < vector < string > > rfu(){
    vector < vector < string > > res;
    getline(cin, user);

    user = clean(user);
    user = prune(user);


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

        vector < string > cmds = rfu();

        int ln = user.length();
        for (int i = 0 ; i < n ; i ++){

        }
    }

    return 0;
}
