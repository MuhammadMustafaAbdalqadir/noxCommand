#ifndef VOLUME_H
#define VOLUME_H

#include <bits/stdc++.h>
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


class Volume{

    public:
        Volume(string path_);

        void update();

        static string home_path();
        static bool is_dir(string str);
        static bool is_file(string str);
        static bool is_valid_path(string str);
        static string name_of_path(string str);
        static vector < string > sons_of_dir(string str);

        void set_path(string path_);

        string get_name();
        string get_path();
        string get_type();
        vector < string > get_sons();


    private:
        string name, path, type; // type = "file", "dir" or "nth".
        vector < string > sons;
};

#endif // VOLUME_H
