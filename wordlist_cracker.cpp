#include <iostream>
#include <cstdlib>
#include <fstream>
#include "md5.h"

using namespace std;


void crack(const string *hash, const string *wordlist_path, string *cleartext);
void usage(char *argv[]);

int main(int argc, char *argv[])
{
    // Make sure all required arguments were passed
    if(argc != 5)
    {
        usage(argv);
        return 1;
    }

    string wordlist_path;
    string hash;
    string cleartext;

    // Parse the command line arguments
    for(int i = 0; i < 5; i++)
    {
        if(strcmp(argv[i], "-w") == 0)
        {
            wordlist_path = argv[i+1];
        }
        else if(strcmp(argv[i], "-h") == 0)
        {
            hash = argv[i+1];
        }
        else if(strcmp(argv[i], "--help") == 0)
        {
            usage(argv);
            return 1;
        }
    }

    if(hash.empty() || wordlist_path.empty())
    {
        usage(argv);
        return 1;
    }

    crack(&hash, &wordlist_path, &cleartext);
    if(cleartext.empty())
    {
        cerr << "Not found: " << hash << endl;
    }
    else
    {
        cout << "Cleartext: " << hash << ":" << cleartext << endl;
    }

    return 0;
}


/* Compare each character inside the passed hash string with the allowed_chars
   array, when a char was found which is not inside the array, the hash is invalid. */
bool isValidHash(const string *hash)
{
    const char allowed_chars[] = {'a', 'b', 'c', 'd', 'e', 'f', '1', '2', '3',
                                  '4', '5', '6', '7', '8', '9', '0'};

    for(char c : *hash)
    {
        bool char_found = false;
        for(char allowed_char : allowed_chars)
        {
            if(c == allowed_char)
            {
                char_found = true;
                break;
            }
        }

        if(!char_found)
        {
            return false;
        }
    }

    return true;
}

/* Tries to find the passed hash inside the wordlist, stores the found cleartext
   inside the "cleartext" pointer. */
void crack(const string *hash, const string *wordlist_path, string *cleartext)
{
    // Make sure the passed hash is a valid md5 hash
    if(!isValidHash(hash))
    {
        cerr << "Passed hash isn't a valid MD5 hash!" << endl;
        exit(1);
    }

    // Open the wordlist for reading
    ifstream wordlist("wordlist.txt");
    if(!wordlist)
    {
        cerr << "Failed to open wordlist!" << endl;
        exit(1);
    }

    // Read the passed wordlist line by line
    char word[256];
    while(wordlist.getline(word, 256))
    {
        // Get the MD5 hexdigest of the current word
        string current_hash = md5(word);
        // Compare the calculated hexdigest with the passed hash
        if(current_hash == *hash)
        {
            *cleartext = word;
            break;
        }
    }
}

void usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " -h HASH  -w WORDLIST " << endl;
}

