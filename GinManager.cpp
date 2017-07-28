/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GinManager.cpp
 * Author: ConnorFlynn
 * 
 * Created on July 16, 2017, 10:09 PM
 */

#include "GinManager.h"

/*
 * Name: GinManager(string) 
 * Desc: Constructor
 * Prec: Takes in a filepath containg .gnf and/or .ginf files
 * Post: Creates the GinManager object with loaded subobjects
 */
GinManager::GinManager(const std::string path)
{
    LoadSettings();
    LoadDirectory(path);
}

GinManager::GinManager(const GinManager& orig) {
}

GinManager::~GinManager() {
}

/* [NOTE - REMEMBER TO UPDATE WITH ALL NEW CHANGES ADDED TO FUNCTION]
 * Name: LoadSettings
 * Desc: Defines some settings, such as object type vector
 * Prec: None -- runs only when new instance created
 * Post: Defines settings:
 *          - m_object_types (vector of object types)
 */
void GinManager::LoadSettings()
{
    /*
     * Object Types
     */
    m_object_types.push_back("char"); // char - character
    m_object_types.push_back("location"); // lctn - location
    m_object_types.push_back("event"); // evnt - event
    m_object_types.push_back("scene"); // scne - scene
    m_object_types.push_back("menu"); // menu - menu
    m_object_types.push_back("driver"); // drvr - driver
}

/*
 * Name: LoadFile
 * Desc: Takes in a file path, reads its contents, and initializes the object
 * Prec: A valid .gnf file path
 * Post: Initializes the object defined in the .gnf file
 */
std::string GinManager::LoadFile(const std::string path) throw()
{
    // Load the file's contents into a vector
    std::vector<std::string> lines;
    std::ifstream file;
    file.open(path.c_str());
    char chr_input[1024];
    while(file.peek() != EOF)
    {
        file.getline(chr_input,80);
        lines.push_back(chr_input);
    }
    
    std::string type = "NULL";
    std::string id;
    // Read every line in the vector
    for(int i = 0; i < lines.size(); i++)
    {
        std::string line = lines.at(i);
        std::string prefix = line.substr(0,line.find_first_of(" "));
        
        // For every object type
        for(int j = 0; j < m_object_types.size(); j++)
        {
            // If the prefix is an object type
            if(prefix == m_object_types.at(j))
            {
                // If it has two arguments
                if(HasArgs(line,2))
                {
                    // If the type is not defined
                    if(type == "NULL")
                    {
                        // Define it
                        type = prefix;
                    }
                    else
                    {
                        throw TwoType_Error(path, std::to_string(i));
                    }
                }
                else
                {
                    throw TooManyArgs_Error(path, std::to_string(i));
                }
            }
        }
    }
    
    std::cout << type << " " << id << std::endl;
}

/* 
 * Name: LoadDirectory
 * Desc: Takes in the path to a directory file and completes all steps therein
 * Prec: A .gnd file path
 * Post: Loads all the objects from the files in the directory.
 */
void GinManager::LoadDirectory(const std::string path)
{
    std::ifstream directory_file;
    directory_file.open(path.c_str());
    
    // Directory vector and char array for input
    std::vector<std::string> directory;
    char chr_input[1024];
    
    // Read all the lines into the vector
    while(directory_file.peek() != EOF)
    {
        directory_file.getline(chr_input,80);
        directory.push_back(chr_input);
    }
    
    // Current directory string, current file string, and current type char
    std::string curr_directory;
    char type;
    std::string file;
    for(int i = 0; i < directory.size(); i++)
    {
        // Type is the first character, file is from 2 to the end
        type = directory.at(i).at(0);
        file = directory.at(i).substr(2);
        
        // If type is 'd', set the current directory
        if(type == 'd')
        {
            curr_directory = file;
            // If the directory doesn't end with a slash, add it
            if(curr_directory.find_last_of("\\") != curr_directory.length()-1)
            {
                curr_directory.append("\\");
            }
        }
        
        // Else if type is 'f', load the file
        else if(type == 'f')
        {
            file = curr_directory + file;
            LoadFile(file);
        }
        
        // Else if type is 'i', load a different directory
        else if(type = 'i')
        {
            LoadDirectory(file);
        }
        
        // Else if type is 's', load this file and set m_driver as its id
        else if(type == 's')
        {
            file = curr_directory + file;
            m_driver = LoadFile(file);
        }
    }
}

/*
 * Name: HasArgs
 * Desc: Determines if a given line has a certain number of arguments
 * Prec: A string and an integer
 * Post: Returns true if the line has arg_num arguments, false otherwise
 */
bool GinManager::HasArgs(std::string line, int arg_num)
{
    std::string regex_test_str = "^((\\S+)";
    // From 1 to arg_num (ie. arg_num - 1 times
    for(int i = 1; i < arg_num; i++)
    {
        regex_test_str.append(" (\\S+)");
    }
    regex_test_str.append(")$");
    std::regex regex_test_rgx(regex_test_str);
    
    return std::regex_match(line, regex_test_rgx);
}