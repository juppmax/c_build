#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

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
            ########################################
            # exit <=> exit                        #
            # --help <=> help                      #
            # shpkg <=> show config                #
            # makepkg <=> build app                #
            # makepkg -loc <=> make with local conf#
            # clear <=> clear display              #
            # name <=> enter file                  #
            # out <=> enter output                 #
            # extra <=> add extras                 #
            # compile <=> use gcc/g++/java         #
            # save <=> save local config           #
            # start <=> start program              #
            # update <=> refresh the app           #
            #                                      #
            # !     Wont work with mingw         ! #
            # !     because of java              ! #
            # !     only c/c++ will work         ! #
            # !     with mingw                   ! #
            #                                      #
            ########################################
            )" << std::endl;
}

void read_config(std::vector<std::string>& command_list) {
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
void read_config_silent(std::vector<std::string>& command_list) {
    std::ifstream config("commands.sh");
    std::string command;
    if(!config){
        command_list.push_back("nothing");
    }
    while (std::getline(config, command)) {
        if(command != ""){
            command_list.push_back(command);
        }
    }
    config.close(); 
}
void draw_update(std::vector<std::string>& command_list){
    command_list.clear();
    read_config(command_list);
}
void draw_silent_update (std::vector<std::string>& command_list){
    command_list.clear();
    read_config_silent(command_list);
}
void draw_compiler(std::string& compiler){
    std::cout << "g++/gcc or java: ";
    std::cin >> compiler;
	if(compiler == "g++" || compiler == "gcc" || compiler == "java"){
		std::cout << "set compiler successfull!" << std::endl;
	}else {
		std::cout << "Error! " << std::endl;
		std::cout << "Pleas enter a valid compiler!" << std::endl;
	    std::cout << "Fallback to g++!" << std::endl;
	    compiler = "g++";
	}
    std::cin.ignore();
}
//voids for the compilation
void make_compile_command(std::string& name, std::string& output, std::string& extras, std::string& compiler, std::string& compile_command){
    //bundle the local infos into one string for execution
    compile_command = "";
    if(compiler == "java"){
        compile_command = "javac " + name;
        std::cout << compile_command << std::endl;
    }else{
        compile_command = compiler + "  " + name + " -o " + output + " " + extras;
        std::cout << compile_command << std::endl;
    }
}

//safety check + running the command 
void run_compile_command(std::string& compile_command){
    if(compile_command[0] == 'g' || compile_command[0] == 'G' || compile_command[0] == 'j') {
        std::system(compile_command.c_str());
    }else {
        std::cout << "Invalid compiler specified!" << std::endl;
        std::cout << "Please use 'g++' or 'gcc'." << std::endl;
    }
}

void run_command(const std::string& command){
    if(command[0] == 'g' || command[0] == 'G' || command[0] == 'j'){
        std::system(command.c_str());
    }else{
        std::cout << "Invalid compiler specified!" << std::endl;
        std::cout << "Please use 'g++' or 'gcc'." << std::endl;
    }
}
//exit function
void exit(){
    exit(0);
}
void show_local_config(std::string& name, std::string& output, std::string& extras){
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Local configuration;" << std::endl;
    std::cout << name << std::endl;
    std::cout << output << std::endl;
    std::cout << extras << std::endl;
}

//the draw functions are the commands
void draw_shpkg(std::string& name, std::string& output, std::string& extras, std::vector<std::string>& command_list){
    //read the vector;
    for (const std::string& command : command_list) {
        if(command == "nothing"){
            std::cout << "Error!" << std::endl;
            std::cout << "No compile command or config found!" << std::endl;

            if(name != "" || output != ""){
                show_local_config(name, output, extras);
            }
            }else{
                std::cout << command << std::endl;
            }
            }
}
void draw_makepkg(std::string& input, std::string& name, std::string& output,std::string& extras,std::string& compile_command, std::string& compiler, std::vector<std::string>& command_list){
    draw_update(command_list);
    for (const std::string& command : command_list) {
        run_command(command);
    }
}

void draw_name(std::string& name){
    std::cout << "Enter name of your file: ";
    std::cin >> name;
    std::cout << name << std::endl;
    std::cin.ignore();
}
void draw_out(std::string& output){
    std::cout << "Enter output name: ";
    std::cin >> output;
    std::cout << output << std::endl;
    std::cin.ignore();
}
void draw_extras(std::string& extras){
    std::cout << "Add compile extras: ";
    std::cin >> extras;
    std::cin.ignore();
}
void draw_save(std::string& name, std::string& output,std::string& extras ,std::string& compiler,std::string& compile_command){
    if(name != "" && output != ""){
        make_compile_command(name, extras, output, compile_command, compiler);
        if(compile_command[0] == 'g' || compile_command[0] == 'G') {
            std::ofstream write_new_config("commands.sh", std::ios::out);
            write_new_config << compile_command << std::endl;
            write_new_config.close();            
            }else {
                std::cout << "Invalid compiler specified!" << std::endl;
                std::cout << "Please use 'g++' or 'gcc'." << std::endl;
            }
            }else{
                std::cout << "Error!" << std::endl;
                std::cout << "Before save enter compile infos like name, out" << std::endl;
                std::cin.ignore();
            }
}
void draw_start(){
    std::string tmp_name;
    std::string start_command;

    std::cout << "if it is a java program please type 'java' " << std::endl;
    std::cout << "Pleas enter program name: ";
    std::cin >> tmp_name;

    if(tmp_name == "java"){
        tmp_name = "";
        std::cout << "Pleas enter java program name" << std::endl;
        std::cin >> tmp_name;
        start_command = "java " + tmp_name;
        std::system(start_command.c_str());
    }else{
        #ifdef _WIN32
        start_command = ".\\" + tmp_name;
        std::cout << start_command << std::endl;
        std::system(start_command.c_str());
        #else
        start_command = "./" + tmp_name;
        std::system(start_command.c_str());
        #endif
    }
}

void commands(std::string& input, std::string& name, std::string& output,std::string& extras,std::string& compile_command, std::string& compiler, std::vector<std::string>& command_list){
    if(input == "exit"){
        exit();
    }else if(input == "--help"){
        display_help();
    }else if(input == "clear"){
        clear();
    }else if(input == "shpkg"){
        draw_shpkg(name, output, extras, command_list);
    }else if(input == "makepkg"){
        draw_makepkg(input, name, output, extras, compile_command, compiler, command_list);
    }else if(input == "makepkg -loc"){
        //had to put that here otherwise it would not work
        make_compile_command(name, output, extras, compiler, compile_command);
        run_compile_command(compile_command);
    }else if(input == "name"){
        draw_name(name);
    }else if(input == "out"){
        draw_out(output);
    }else if(input == "extra"){
        draw_extras(extras);
    }else if(input == "compiler"){
        draw_compiler(compiler);
    }else if(input == "save"){
        draw_save(name, output, extras, compiler, compile_command);
    }else if(input == "start"){
        draw_start();
    }else if(input == "update"){
        draw_update(command_list);
    }else if(input == "d"){
        make_compile_command(name, output, extras, compiler, compile_command);
    }else if(input == "e"){
        run_compile_command(compile_command);
    }else if(input == ""){
    }else{
        std::cout << "Error!" << std::endl;
        std::cout << "Command not found!" << std::endl;
        std::cout << "Type --help for help" << std::endl;
    }
    input = "";
}
void start_app(std::string& __version__, std::string compiler, std::vector<std::string> command_list){
    clear();
    clear();
    clear();
    std::cout << "app build: " << __version__ << std::endl;
    for (const std::string& command : command_list) {
        if (command != "nothing"){
            if(command[3] == 'c'){
                compiler = "gcc";
            }else if(command[3] == '+'){
                compiler = "g++";
            }
        }
    }
    read_config(command_list);
}
//managing the tty
void start_tyy(std::string& input, std::string& name, std::string output,std::string& extras,std::string compile_command, std::string& compiler, std::vector<std::string> command_list){
    draw_silent_update(command_list);
    while(true){
        std::cout << "|>: ";
        std::getline(std::cin, input);
        commands(input, name, output, extras, compile_command, compiler, command_list);
    }
}

int main() {
    std::vector<std::string> command_list;
    std::string input;
    std::string __version__ = "0.00.05";
    

    std::string name;
    std::string output;
    std::string extras;
    std::string compiler = "g++";
    std::string compile_command;

    start_app(__version__, compiler, command_list);
    
    //start shell
    std::cout << "type --help for help" << std::endl;
    start_tyy(input, name, output, extras, compile_command, compiler, command_list);
}
//nothing means in this app that there is no file to be read from