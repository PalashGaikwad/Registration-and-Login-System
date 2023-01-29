#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  std::string username {};
  std::string password {};
  std::string selection {};
  std::ifstream in_file {}; 
  std::ofstream out_file {};
  std::string delete_file{};
  std::string current_working_directory = get_current_dir_name();
  bool create_file {true};
    do {
      std::cout << "1: Registration\n";
      std::cout << "2: Login\n";
      std::cout << "Q: Quit\n";
      std::cin>> selection;

       if (selection == "1") {
         bool duplicate_folder {};
         char overwrite_file {};
         current_working_directory = get_current_dir_name(); 
         if([] (bool &duplicate_folder) {
           const std::string directory = get_current_dir_name();
           std::string final_directory;
           for (int i {}; i < 5; i++) {final_directory += directory.at((directory.size() -1 )- i);}
           int i {};
           for (int j {int(final_directory.size())-1}; j >=(final_directory.size()+1)/2; j--) {
              char temp {final_directory.at(j)};
              final_directory.at(j) = final_directory.at(i);
              final_directory.at(i) = temp;
              ++i;
          }
       duplicate_folder = true;
       return final_directory;
      } (duplicate_folder) != "Users") {
        for(const auto& item: std::filesystem::directory_iterator(get_current_dir_name())) {
          std::string file_name {};
          std::string item_in_string {item.path().string()};
          for(long unsigned int i {current_working_directory.size()+static_cast<long unsigned int> (1)}; i < item_in_string.size(); i++) {
            file_name += item_in_string.at(i);
          }
          if (file_name == "Users") { 
            duplicate_folder= true;
            break;
          } 
          else if (file_name != "Users"){
            duplicate_folder = false;
          }
        }
      }
        if (duplicate_folder == false) {
          std::filesystem::create_directory("Users");
          std::cout << "Users folder created.\n";
        }

        std::string clone = current_working_directory;
        clone += "/Users";
        const char* new_working_directory = clone.c_str();
        chdir(new_working_directory);
        std::cout << "Enter you're Username: ";
        std::cin.ignore();
        getline(std::cin,username);

        if (*username.end() == ' ') {
          std::string username_copy = username;
          username = "";
          for (int i {0}; i < username_copy.size()-1; i++) {
            username = username_copy.at(i);
          }
        }

        std::cout << "Enter you're Password: ";
        getline(std::cin,password);
        for(const auto& item: std::filesystem::directory_iterator(get_current_dir_name())) {
          std::string file_name {};
          std::string item_in_string {item.path().string()};
          for(long unsigned int i {current_working_directory.size()+static_cast<long unsigned int> (1)}; i < item_in_string.size(); i++) {
            file_name += item_in_string.at(i);
          }
          if (file_name == username+".txt" || file_name == "Users/"  +  username + ".txt") {
            std::cout << "User already exists. Do you wish to delete this user? Y/N\n";
            std::cin>> delete_file;
            while (delete_file != "y" || delete_file != "n" || delete_file != "Y" || delete_file != "N") {
              if (delete_file == "Y" || delete_file == "y") {
                std::cout << "Deleting file...\n";
                if (remove(username.c_str()))  {
                  std::cout << "File deleted.\n";
                  break;
                }
                else {
                  std::cerr<< "File deletion gone wrong.\n";
                }
              }
              else if (delete_file == "N" || delete_file == "n") {
                create_file = false;
                std::cout << "OK.\n";
                break;
              }
              else {
                std::cout << "Incorrect. Answer only with Y or N\n";
                std::cin >> delete_file; 
              }
            }
            if (delete_file == "Y" || delete_file == "y"){ 
            std::cout<<"Do you want to overwrite the old file with the new one? Y/N\n";
            std::cin>> overwrite_file;
            while (overwrite_file != 'y' || overwrite_file != 'Y' || overwrite_file != 'N' || overwrite_file != 'n'){
              if (overwrite_file == 'y' || overwrite_file == 'Y') {
                create_file = true;
                break;
              }
              else if (overwrite_file == 'n' || overwrite_file == 'N') {
                create_file = false;
                break;
              }
              else {
                std::cout << "Incorrect Choice. Answer only with Y/N\n";
                std::cin>> overwrite_file;
              }
            }
          }
        } 
      }
       if (create_file == true) {  
         out_file = std::ofstream{username +".txt"};
         if (out_file.is_open()) {
           out_file << username << "\n" << password;
           std::cout << "User Added\n";
         }
         else if (!out_file.is_open()){
           std::cerr << "Something went wrong. \n";
         }
       }
       out_file.close();
       duplicate_folder = false;
       create_file = true;
      }

      else if(selection == "2") {
         int attempt_counter {};
         std::string temp {};
         create_file = true;
         current_working_directory = get_current_dir_name(); 
         const std::string directory = get_current_dir_name();
         std::string final_directory;
         int i {};

           for (int i {}; i < 5; i++) {
             final_directory += directory.at((directory.size() -1 )- i);
           }
           for (int j {int(final_directory.size())-1}; j >=(final_directory.size()+1)/2; j--) {
              char temp {final_directory.at(j)};
              final_directory.at(j) = final_directory.at(i);
              final_directory.at(i) = temp;
              ++i;
            }
          if (final_directory != "Users" && final_directory != "/Users") {
            std::string arguement = current_working_directory + "/Users";
            if (chdir(arguement.c_str()) == 0) {
              current_working_directory = get_current_dir_name();
              std::cout << "Enter username: ";
              std::cin.ignore();
              getline(std::cin,username);
              std::cout << "Enter password: ";
              in_file.close();
              in_file.open(current_working_directory + "/" + username + ".txt");
              getline(std::cin,password);
              if(in_file.is_open()) {
                int ignore_first_line {1};
                while (!in_file.eof()) {
                if (ignore_first_line == 1) {
                  getline(in_file,temp);
                  ignore_first_line++;
                  continue;
                }
                getline(in_file,temp);
                if (temp == password) {
                  std::cout << "\n\nLogged in. \n\n";
                }
                else {
                 while (temp != password) {
                  if (attempt_counter == 3) {
                    std::cout << "\nIncorrect Password. You have exhausted all of you're tries. You have to wait 30 seconds.\n";
                    for (int i {30}; i > -1; i--) {
                      std::cout << i << "\n";
                      sleep(1);
                    }
                    attempt_counter = 0;
                    continue;
                  } 
                    std::cout << "\nIncorrect password. You have " << 3-attempt_counter << " more tries. \n";
                    std::cout << "Enter password: ";
                    std::cin >> password;
                    if (password == temp) {
                      std::cout << "\n\nLogged in.\n\n\n";
                      break;
                    }
                    attempt_counter++;
                  }
                }
              }
              continue;   
              }
              else {
                std::cerr << "Error: File not opened. Possible wrong username.\n";
              }
            }
            else if (chdir(arguement.c_str()) == -1){
              std::cout << "You haven't registered! You have to register first to login into a user! \n";
            }
          }
          else {
              std::cout << "Enter username: ";
              std::cin.ignore();
              getline(std::cin,username);
              std::cout << "Enter password: ";
              in_file.close();
              in_file.open(current_working_directory + "/" + username + ".txt");
              getline(std::cin,password);
              if(in_file.is_open()) {
                int ignore_first_line {1};
                while (!in_file.eof()) {
                if (ignore_first_line == 1) {
                  getline(in_file,temp);
                  ignore_first_line++;
                  continue;
                }
                getline(in_file,temp);
                if (temp == password) {
                  std::cout << "\n\nLogged in.\n\n";
                }
                else {
                 while (temp != password) {
                  if (attempt_counter == 3) {
                    std::cout << "\nIncorrect Password. You have exhausted all of you're tries. You have to wait 30 seconds.\n";
                    for (int i {30}; i > -1; i--) {
                      std::cout << i << "\n";
                      sleep(1);
                    }
                    attempt_counter = 0;
                    continue;
                  } 
                    std::cout << "\nIncorrect password. You have " << 3-attempt_counter << " more tries. \n";
                    std::cout << "Enter password: ";
                    std::cin >> password;
                    if (password == temp) {
                      std::cout << "\n\nLogged in.\n\n";
                      break;
                    }
                    attempt_counter++;
                  }
                }
              }
            } else {
                std::cerr << "Error: File not opened. Possible wrong username.\n";
              }
          }
        }  
      } while (selection != "Q" && selection != "q");
     in_file.close();
     out_file.close();
    return 0;
}