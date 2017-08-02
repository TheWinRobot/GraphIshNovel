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
 *              - m_object_types_regex (regex OR of all object types)
 *          - vectors of valid object prefixes
 *              - associated regex ORs
 *          - m_collections (vector of "collection" types)
 *              - m_collections_regex (regex OR of "collection" types)
 */
void GinManager::LoadSettings()
{
    std::vector<std::string> all_prefixes;
    
    /*
     * Object Types
     */
    m_object_types.push_back("char");
    m_object_types.push_back("character");
    m_object_types.push_back("location");
    m_object_types.push_back("event");
    m_object_types.push_back("scene");
    m_object_types.push_back("menu");
    m_object_types.push_back("driver");
    
    m_object_types_regex = MakeRegex(m_object_types);
    
    all_prefixes.insert(all_prefixes.end(), m_object_types.begin(),
            m_object_types.end());
    
    /*
     * Valid Character prefixes
     */
    m_char_prefixes.push_back("name");
    m_char_prefixes.push_back("prefer");
    m_char_prefixes.push_back("short");
    m_char_prefixes.push_back("color");
    m_char_prefixes.push_back("image");
    m_char_prefixes.push_back("say");
    
    m_char_prefixes_regex = MakeRegex(m_char_prefixes);
    
    all_prefixes.insert(all_prefixes.end(), m_char_prefixes.begin(),
            m_char_prefixes.end());
    
    /*
     * Valid Location Prefixes
     * (WARNING: Highly subject to change)
     */
    m_loc_prefixes.push_back("background");
    m_loc_prefixes.push_back("events");
    m_loc_prefixes.push_back("map_name");
    m_loc_prefixes.push_back("referred");
    m_loc_prefixes.push_back("adjacent");
    
    m_loc_prefixes_regex = MakeRegex(m_loc_prefixes);
    
    all_prefixes.insert(all_prefixes.end(), m_loc_prefixes.begin(),
            m_loc_prefixes.end());
    
    /*
     * Valid Scene Prefixes
     */
    m_scne_prefixes.push_back("setting");
    m_scne_prefixes.push_back("start_flow");
    m_scne_prefixes.push_back("flow");
    m_scne_prefixes.push_back("say");
    m_scne_prefixes.push_back("if");
    m_scne_prefixes.push_back("elif");
    m_scne_prefixes.push_back("else");
    m_scne_prefixes.push_back("while");
    m_scne_prefixes.push_back("play");
    m_scne_prefixes.push_back("branch");
    m_scne_prefixes.push_back("display");
    m_scne_prefixes.push_back("choice");
    m_scne_prefixes.push_back("response");
    
    m_scne_prefixes_regex = MakeRegex(m_scne_prefixes);
    
    all_prefixes.insert(all_prefixes.end(), m_scne_prefixes.begin(),
            m_scne_prefixes.end());
    
    /*
     * Valid Menu Prefixes
     * (WARNING: Highly subject to change)
     */
    m_menu_prefixes.push_back("section");
    m_menu_prefixes.push_back("button");
    m_menu_prefixes.push_back("image");
    m_menu_prefixes.push_back("position");
    m_menu_prefixes.push_back("dropdown");
    m_menu_prefixes.push_back("play");
    m_menu_prefixes.push_back("start");
    m_menu_prefixes.push_back("set");
    
    m_menu_prefixes_regex = MakeRegex(m_menu_prefixes);
    
    all_prefixes.insert(all_prefixes.end(), m_menu_prefixes.begin(),
            m_menu_prefixes.end());
    
    /*
     * Valid Driver Prefixes
     * (WARNING: Highly subject to change)
     */
    m_drvr_prefixes.push_back("MAIN_DRIVER");
    m_drvr_prefixes.push_back("vars");
    m_drvr_prefixes.push_back("start");
    m_drvr_prefixes.push_back("include");
    
    m_drvr_prefixes_regex = MakeRegex(m_drvr_prefixes);
    
    all_prefixes.insert(all_prefixes.end(), m_drvr_prefixes.begin(),
            m_drvr_prefixes.end());
    
    /*
     * TODO: See issue #1
    /*
     * Variable Types
     *\/
    m_var_types.push_back("int"double"););
    m_var_types.push_back("string");
    m_var_types.push_back("
    m_var_types.push_back("bool");
     */
    
    m_all_prefixes_regex = MakeRegex(all_prefixes);
    
    /*
     * "Collection" Types
     * "Collections" are structures that have nested statements that do not
     *      have a valid prefix. For now, we ignore them.
     */
    m_collections.push_back("events");
    m_collections.push_back("adjacent");
    m_collections.push_back("include");
    // [TODO] When variable types are implemented, remove the following line
    m_collections.push_back("vars");
    
    // Collection types will all end with a colon so just roll them in
    m_colon_check.insert(m_colon_check.end(), m_collections.begin(),
            m_collections.end());
    
    m_collections_regex = MakeRegex(m_collections);
    
    /*
     * Colon Check List
     * Any statement that needs to end with a colon
     */
    m_colon_check.push_back("flow");
    m_colon_check.push_back("if");
    m_colon_check.push_back("elif");
    m_colon_check.push_back("else");
    m_colon_check.push_back("while");
    m_colon_check.push_back("choice");
    m_colon_check.push_back("response");
    
    m_colon_check_regex = MakeRegex(m_colon_check);
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
    
    
    bool in_collection;
    int coll_indent;
    
    std::string type = "NULL";
    std::string id;
    std::regex valid_prefixes;
    int i = 0;
    // Read every line in the vector
    for(std::vector<std::string>::iterator it = lines.begin();
            it != lines.end(); ++it)
    {
        std::string line = *it;
        i++;
        
        int curr_indent = 0;
        /*
         * Indentation does not matter now, so get rid of it before problems
         *      Keep track of indentation, though
         */
        while(line.front() == ' ' || line.front() == '\t')
        {
            line.erase(0,1);
            curr_indent++;
        }
        std::string prefix = line.substr(0,line.find_first_of(" "));
        
        // If prefix ends with a colon, drop it
        if(prefix.back() == ':')
        {
            prefix.pop_back();
        }
        
        /*
         * If curr_indent is not greater than coll_indent and isn't a comment
         *      or whitespace, then we're not in a collection anymore
         */
        if(curr_indent <= coll_indent &&
                !(!std::regex_match(prefix,std::regex("\\S+")) ||
                prefix.front() == '#'))
        {
            coll_indent = 0;
            in_collection = false;
        }
        
        /*
         * If in collection, just skip it (for now)
         */
        if(in_collection){}
        
        /*
         * CHECK IF OBJECT TYPE DECLARATION
         */
        // Else if the prefix is an object type
        else if(std::regex_match(prefix, m_object_types_regex))
        {
            // If it has two arguments
            if(HasArgs(line,2))
            {
                // If the type is not defined
                if(type == "NULL")
                {
                    // Define it
                    type = prefix;
                    
                    /*
                     * Conditions over type, including:
                     *  - regex setting
                     *  - shortenings
                     */
                    if(type == "character")
                    {
                        type = "char";
                        valid_prefixes = m_char_prefixes_regex;
                    }
                    else if(type == "char")
                    {
                        valid_prefixes = m_char_prefixes_regex;
                    }
                    else if(type == "location")
                    {
                        valid_prefixes = m_loc_prefixes_regex;
                    }
                    else if(type == "event" || type == "scene")
                    {
                        valid_prefixes = m_scne_prefixes_regex;
                    }
                    else if(type == "menu")
                    {
                        valid_prefixes = m_menu_prefixes_regex;
                    }
                    else if(type == "driver")
                    {
                        valid_prefixes = m_drvr_prefixes_regex;
                    }
                    
                    
                    // Get the identifier
                    id = line.substr(prefix.size() + 1);
                    
                    // Remove the type definition; it's not needed anymore
                    lines.erase(lines.cbegin()+i);
                    i--;
                }
                // If it is, throw TwoType_Error
                else
                {
                    throw TwoType_Error(path, i+1);
                }
            }
            // If it does not, throw ArgsMismatch_Error
            else
            {
                throw ArgsMismatch_Error(path, i+1);
            }
        }
        
        /*
         * FUTURE PREFIX IMPLEMENTATIONS GO HERE
         */
        
        /*
         * Else if a comment / all-whitespace line, remove it
         */
        else if(!std::regex_match(prefix,std::regex("\\S+")) ||
                prefix.front() == '#')
        {
            it = lines.erase(it);
            --it;
        }
        
        /*
         * Else if a valid prefix, but not for this object's type,
         *      throw WrongPrefix_Error
         */
        else if(std::regex_match(prefix,m_all_prefixes_regex) &&
                !std::regex_match(prefix,valid_prefixes))
        {
            throw WrongPrefix_Error(path, i+1);
        }
        
        /*
         * Else if a "collection" type, go into collection mode
         */
        else if(std::regex_match(prefix,m_collections_regex))
        {
            coll_indent = curr_indent;
            in_collection = true;
        }
        
        /*
         * Else if prefix does not match any valid prefix, throw UnknownPrefix_Error
         */
        else if(!std::regex_match(prefix,m_all_prefixes_regex))
        {
            throw UnknownPrefix_Error(path, i+1);
        }
        
        
        /*
         * If prefix should be checked for colon and last char isn't a colon,
         *      throw a NoColon_Error
         */
        if(std::regex_match(prefix,m_colon_check_regex) && line.back() != ':')
        {
            throw NoColon_Error(path, i+1);
        }
    }
    
    /*
     * If type was never defined, throw NoType_Error
     */
    if(type == "NULL")
    {
        throw NoType_Error(path);
    }
    
    std::cout << type << " " << id << std::endl;
    
    /*
     * Pass control to function associated with object type
     */
    // TODO: ^ that
    //      See also issue #3
    
    return id;
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
        
        std::cout << file << std::endl;
        
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
        else if(type == 'i')
        {
            LoadDirectory(file);
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
    /*
     * Note to future users: the regex bit for an arg, with or without quotes,
     *  is (\w+|"[^"\n]+")
     */
    // Start with start-of-line flag, open-paren, and a regex bit for an arg
    std::string regex_test_str = "^((\\w+|\"[^\"\n]+\")";
    // From 1 to arg_num (ie. arg_num - 1 times)
    for(int i = 1; i < arg_num; i++)
    {
        // Add the regex bit for an arg
        regex_test_str.append(" (\\w+|\"[^\"\n]+\")");
    }
    // End with a close-paren and end-of-line flag
    regex_test_str.append(")$");
    std::regex regex_test_rgx(regex_test_str);
    
    return std::regex_match(line, regex_test_rgx);
}


/*
 * Name: MakeRegex
 * Desc: Takes in a vector and returns the matching regex OR statement
 * Prec: A non-empty string vector
 * Post: A regex containing the regex OR of the contents of vec
 */
std::regex GinManager::MakeRegex(std::vector<std::string> vec)
{
    // Begin with a start-of-string flag and open-paren
    std::string objects_regex = "^(";
    // For every string in vec
    for(int i = 0; i < vec.size(); i++)
    {
        // Add that string and the OR flag '|'
        objects_regex.append(vec.at(i) + "|");
    }
    // Replace the trailing OR flag with a close-paren
    objects_regex.back() = ')';
    // Add an end-of-string flag
    objects_regex.append("$");
    
    // Make a regex object with the string and return it
    std::regex output(objects_regex);
    return output;
}