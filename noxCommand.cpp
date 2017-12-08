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

using namespace std;




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

// read_from_user
void rf(){
    cin >> user;
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

int main(){

    system("color A");
    get_home_dir();

    ffp("noxCommand [Version 1.0]\n");
    ffp("(c) 2017 noxhollow (MuhammadMustafa). All rights reserved.\n\n");
    ffp("Available commands <list, search, change, create, rename, move, copy, delete, clear, help>\n");
    ffp("Remember that you can type help then the command name!\n\n\n\n\n\n");

    while (true){
        ffp("->noxCommand in <" + current_path + ">\n$ "); rf();
        if (user == "clear"){
            system("CLS");
        }
        else if (user == "list"){
            vector < string > v = list_dir(current_path);

            for (int i = 0 ; i < v.size(); i ++){
                ffp("-->"+v[i]+"\n");
            }

            printf("\n");
        }
        else if (user == "help"){

        }
        else {

        }
    }

    return 0;
}
