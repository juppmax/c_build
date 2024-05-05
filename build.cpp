#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void clear(){
    #ifdef _WIN32
        system("cls");
    #elif __unix__
        system("clear");
    #else 
        system("clear");
    #endif
}

void display_help(){
    clear();
    cout << R"(
            ###############################
            # exit <=> exit               #
            # help <=> help               #
            # shpkg <=> show config       #
            # makepkg <=> build app       #
            # clear <=> clear display     #
            # name <=> enter file         #
            # out <=> enter output        #
            # extra <=> add extras        #
            # compile <=> use gcc/g++     #
            # save <=> save local config  #
            # start <=> start program     #
            #                             #
            ###############################
            )" << endl;
}
void read_config(vector<string>& command_list, bool& start_after_compile) {
    ifstream config("commands.sh");
    string command;
    if(!config){
        cout << "Error no config file found!" << endl;
        cout << "Fallback to alternative config! " << endl;
        command_list.push_back("nothing");
    }
    while (getline(config, command)) {
        if(command != ""){
            cout << command << endl;
            command_list.push_back(command);
        }
    }
    config.close(); 
}

int main() {
    vector<string> command_list;
    bool __runtime__ = false;
    int __runtime__error; 
    string input;
    string __version__ = "0.00.01";

    string name;
    string output;
    string extras;
    string compiler = "g++ ";
    string compile_command;
    string start_command;
    bool start_after_compile = false;
    //start __runtime__
    __runtime__ = true;
    clear();
    cout << "app build: " << __version__ << endl;
    //read build commands in vector
    read_config(command_list, start_after_compile);
    //check if config uses g++ or gcc
    for (const auto& command : command_list) {
        if (command != "nothing"){
            if(command[3] == 'c'){
                compiler = "gcc";
            }else if(command[3] == '+'){
                compiler = "g++";
            }
        }
    }
    

    //start shell
    cout << "type --help for help" << endl;
    while(__runtime__){
        //check if __runtime__ still exists
        if(__runtime__ == false){
            if(__runtime__error == 0){
                exit(0);
            }
        }
        //print tty environment
        cout << "|>: ";
        getline(cin, input);
        
        if(input == "exit"){
            __runtime__ = false;
            __runtime__error = 0;
        }else if(input == "--help"){
            display_help();
        }else if(input == "clear"){
            clear();
        }else if(input == "shpkg"){
            for (const auto& command : command_list) {
                if(command == "nothing"){
                    cout << "Error!" << endl;
                    cout << "No compile command or config found!" << endl;

                    if(name != "" || output != ""){
                        cout << "-----------------------------------" << endl;
                        cout << "Local configuration;" << endl;
                        cout << name << endl;
                        cout << output << endl;
                        cout << extras << endl;
                    }
                }else{
                    cout << command << endl;
                }
            }
            cin.ignore();   
        }else if(input == "makepkg"){
            for (const auto& command : command_list) {
                if(command == "nothing"){
                    cout << "Error!" << endl;
                    cout << "No compile command or config found!" << endl;
                    cout << "Fallback to local configuration?: |[Y|N]|";
                    cin >> input;
                    if(input == "Y" || input == "y"){
                        if(name == "" || output == ""){
                            cout << "Error missing or invalid arguments!" << endl;
                            cout << "Pleas enter compile infos!" << endl;
                        }else{
                            compile_command = compiler + "  " + name + " -o " + output + " " + extras;
                            cout << compile_command << endl;
                            if(compile_command[0] == 'g' || compile_command[0] == 'G') {
                                system(compile_command.c_str());
                            } else {
                                cout << "Invalid compiler specified!" << endl;
                                cout << "Please use 'g++' or 'gcc'." << endl;
                            }
                        }
                    }
                }else {
                    if(name != "" || output != ""){
                        cout << "Found local config!" << endl;
                        cout << "juse it?: |[Y|N]| ";
                        cin >> input;
                        if(input == "y" || input == "Y"){
                            compile_command = compiler + "  " + name + " -o " + output + " " + extras;
                            cout << compile_command << endl;
                            if(compile_command[0] == 'g' || compile_command[0] == 'G') {
                                system(compile_command.c_str());
                            } else {
                                cout << "Invalid compiler specified!" << endl;
                                cout << "Please use 'g++' or 'gcc'." << endl;
                            }
                        }else{
                            if(command[0] == 'g' || command[0] == 'G'){
                            	system(command.c_str());
                            }else{
                                cout << "Invalid compiler specified!" << endl;
                                cout << "Please use 'g++' or 'gcc'." << endl;
                            }
                        }

                    }
                    if(command[0] == 'g' || command[0] == 'G'){
                        system(command.c_str());
                    }else{
                        cout << "Invalid compiler specified!" << endl;
                        cout << "Please use 'g++' or 'gcc'." << endl;
                    }
                }
            }
            cin.ignore();
        }else if(input == "name"){
            cout << "Enter name of your file: ";
            cin >> name;
            cout << name << endl;
            cin.ignore();
        }else if(input == "out"){
            cout << "Enter output name: ";
            cin >> output;
            cout << output << endl;
            cin.ignore();
        }else if(input == "extra"){
            cout << "Add compile extras: ";
            cin >> extras;
            cin.ignore();
        }else if(input == "compile"){
            cout << "g++ or gcc: ";
            cin >> compiler;
            cin.ignore();
        }else if(input == "save"){
            if(name != "" && output != ""){
                compile_command = compiler + "  " + name + " -o " + output + " " + extras;
                if(compile_command[0] == 'g' || compile_command[0] == 'G') {
                    ofstream write_new_config("commands.sh", ios::out);
                    write_new_config << compile_command << endl;
                    write_new_config.close();            
                } else {
                    cout << "Invalid compiler specified!" << endl;
                    cout << "Please use 'g++' or 'gcc'." << endl;
                }
                
            }else{
                cout << "Error!" << endl;
                cout << "Before save enter compile infos like name, out" << endl;
                cin.ignore();
            }
        }else if(input == "d"){
            for (const auto& command : command_list) {
                cout << command << endl;
            }
        }else if(input == "start"){
                    cout << "Pleas enter program name: ";
                    cin >> name;
                    #ifdef _WIN32
                    start_command = ".\\" + name;
                    cout << start_command << endl;
                    system(start_command.c_str());
                    #else
                    start_command = "./" + name;
                    system(start_command.c_str());
                    #endif
        }
    }
    return 0;
}


//enable later
/*cout << "Current build commands: " << endl;
    for (const auto& command : command_list) {
        cout << command << endl;
    }
*/

/*else if(input == "out"){
            cout << "Enter output name: ";
            cin >> output;
            cin.ignore();
        }
            the cin.ignore(); is to clear the input buffer to prevent a bug where it displays |>: |>: 
*/
