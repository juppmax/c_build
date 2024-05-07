#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void clear(){
    #ifdef _WIN32
        std::system("cls");
    #elif __unix__
        std::system("clear");
    #else 
        std::system("clear");
    #endif
}

void display_help(){
    clear();
    std::cout << R"(
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
            )" << std::endl;
}

void read_config(std::vector<std::string>& command_list, bool& start_after_compile) {
    std::ifstream config("commands.sh");
    std::string command;
    if(!config){
        std::cout << "Error no config file found!" << std::endl;
        std::cout << "Fallback to alternative config! " << std::endl;
        command_list.push_back("nothing");
    }
    while (std::getline(config, command)) {
        if(command != ""){
            std::cout << command << std::endl;
            command_list.push_back(command);
        }
    }
    config.close(); 
}

//voids for the compilation
void make_compile_command(std::string& name, std::string& output, std::string& extras, std::string& compiler, std::string& compile_command){
    //bundle the local infos into one string for execution
    compile_command = compiler + "  " + name + " -o " + output + " " + extras;
    std::cout << compile_command << std::endl;
}

//safety check + running the command 
void run_compile_command(std::string& compile_command){
    if(compile_command[0] == 'g' || compile_command[0] == 'G') {
        std::system(compile_command.c_str());
    }else {
        std::cout << "Invalid compiler specified!" << std::endl;
        std::cout << "Please use 'g++' or 'gcc'." << std::endl;
    }
}

void run_command(const std::string& command){
    if(command[0] == 'g' || command[0] == 'G'){
        std::system(command.c_str());
    }else{
        std::cout << "Invalid compiler specified!" << std::endl;
        std::cout << "Please use 'g++' or 'gcc'." << std::endl;
    }
}

int main() {
    std::vector<std::string> command_list;
    std::string input;
    std::string __version__ = "0.00.05";

    std::string name;
    std::string output;
    std::string extras;
    std::string compiler = "g++ ";
    std::string compile_command;
    std::string start_command;
    bool start_after_compile = false;

    //clean display to start
    clear();
    std::cout << "app build: " << __version__ << std::endl;
    //read build commands in vector
    read_config(command_list, start_after_compile);
    //check if config uses g++ or gcc
    for (const std::string& command : command_list) {
        if (command != "nothing"){
            if(command[3] == 'c'){
                compiler = "gcc";
            }else if(command[3] == '+'){
                compiler = "g++";
            }
        }
    }
    
    //start shell
    std::cout << "type --help for help" << std::endl;
    while(true){
        //print tty environment
        std::cout << "|>: ";
        std::getline(std::cin, input);
        
        if(input == "exit"){
            return 0;
        }else if(input == "--help"){
            display_help();
        }else if(input == "clear"){
            clear();
        }else if(input == "shpkg"){
            for (const std::string& command : command_list) {
                if(command == "nothing"){
                    std::cout << "Error!" << std::endl;
                    std::cout << "No compile command or config found!" << std::endl;

                    if(name != "" || output != ""){
                        std::cout << "-----------------------------------" << std::endl;
                        std::cout << "Local configuration;" << std::endl;
                        std::cout << name << std::endl;
                        std::cout << output << std::endl;
                        std::cout << extras << std::endl;
                    }
                }else{
                    std::cout << command << std::endl;
                }
            }
            std::cin.ignore();   
        }else if(input == "makepkg"){
            for (const std::string& command : command_list) {
                if(command == "nothing"){
                    std::cout << "Error!" << std::endl;
                    std::cout << "No compile command or config found!" << std::endl;
                    std::cout << "Fallback to local configuration?: |[Y|N]|";
                    std::cin >> input;
                    if(input == "Y" || input == "y"){
                        if(name == "" || output == ""){
                            std::cout << "Error missing or invalid arguments!" << std::endl;
                            std::cout << "Pleas enter compile infos!" << std::endl;
                        }else{
                            make_compile_command(name, extras, output, compile_command, compiler);
                            run_compile_command(compile_command);
                        }
                    }
                }else {
                    if(name != "" || output != ""){
                        std::cout << "Found local config!" << std::endl;
                        std::cout << "juse it?: |[Y|N]| ";
                        std::cin >> input;
                        if(input == "y" || input == "Y"){
                            make_compile_command(name, extras, output, compile_command, compiler);
                            run_compile_command(compile_command);
                        }else{
                            run_command(command);
                        }

                    }
                    run_command(command);
                }
            }
            std::cin.ignore();
        }else if(input == "name"){
            std::cout << "Enter name of your file: ";
            std::cin >> name;
            std::cout << name << std::endl;
            std::cin.ignore();
        }else if(input == "out"){
            std::cout << "Enter output name: ";
            std::cin >> output;
            std::cout << output << std::endl;
            std::cin.ignore();
        }else if(input == "extra"){
            std::cout << "Add compile extras: ";
            std::cin >> extras;
            std::cin.ignore();
        }else if(input == "compile"){
            std::cout << "g++ or gcc: ";
            std::cin >> compiler;
	    if(compiler == "g++" || compiler == "gcc"){
		    std::cout << "set compiler successfull!" << std::endl;
	    }else {
		    std::cout << "Error! " << std::endl;
		    std::cout << "Pleas enter a valid compiler!" << std::endl;
		    std::cout << "Fallback to g++!" << std::endl;
		    compiler = "g++";
	    }
            std::cin.ignore();
        }else if(input == "save"){
            if(name != "" && output != ""){
                make_compile_command(name, extras, output, compile_command, compiler);
                if(compile_command[0] == 'g' || compile_command[0] == 'G') {
                    std::ofstream write_new_config("commands.sh", std::ios::out);
                    write_new_config << compile_command << std::endl;
                    write_new_config.close();            
                } else {
                    std::cout << "Invalid compiler specified!" << std::endl;
                    std::cout << "Please use 'g++' or 'gcc'." << std::endl;
                }
                
            }else{
                std::cout << "Error!" << std::endl;
                std::cout << "Before save enter compile infos like name, out" << std::endl;
                std::cin.ignore();
            }
        }else if(input == "d"){
            for (const std::string& command : command_list) {
                std::cout << command << std::endl;
            }
        }else if(input == "start"){
                    std::cout << "Pleas enter program name: ";
                    std::cin >> name;
                    #ifdef _WIN32
                    start_command = ".\\" + name;
                    std::cout << start_command << std::endl;
                    std::system(start_command.c_str());
                    #else
                    start_command = "./" + name;
                    std::system(start_command.c_str());
                    #endif
        }
    }
}