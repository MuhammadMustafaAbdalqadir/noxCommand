#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <windows.h>

using namespace std;




string current_path, user;

//Fancy printing
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



    return 0;
}
