#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "AVL.h"
#include "Node.h"
#include "BSTtoString.h"

using std::cout;
using std::endl;

const int NUM_FILES = 3; // the total number of files to be read from

// the string array containing the file names
std::string fileArray[NUM_FILES];

// This will take a string temp and an AVL object and will execute an instruction from the string
// no return, but writes the results of the instruction into the ofs
void parse_instruction(std::string temp, std::ofstream &ofs, AVL<int> *aptr);

// This function is a platform independent way of reading files of various line ending types.
// It's definiton is at the bottom of the file, don't worry if you don't understand it.
namespace ta
{
	std::istream &getline(std::istream &is, std::string &line);
}

int main()
{

	std::ifstream ifs;		 // create the stream to read in from the files
	std::ofstream ofs;		 // create the output stream to write to an output file
	std::string temp;		 // used to store the current instruction
	AVL<int> *avlptr = NULL; // the AVL

	for (int i = 1; i < NUM_FILES; i++)
	{
		std::ostringstream fileNameStream;
		fileNameStream << "file" << i << ".txt";
		fileArray[i] = fileNameStream.str();
	}

	for (int i = 1; i < NUM_FILES; i++)
	{
		ifs.open("in_" + fileArray[i]);	 // open the file to read from
		ofs.open("out_" + fileArray[i]); // open the file to write to
		avlptr = new AVL<int>();

		if (!ifs.is_open())
		{ // if the file did not open, there was no such file
			std::cout << "File " << i << " could not open, please check your lab setup" << std::endl;
		}
		else
		{
			std::cout << "Reading in_file" << i << ".txt..." << std::endl;
		}

		std::cout << "Beginning out_file" << i << ".txt write" << std::endl;
		while (ta::getline(ifs, temp))
		{										  // while there are more instructions to get,
			parse_instruction(temp, ofs, avlptr); // parse the instructions using the AVL
		}
		std::cout << "File write complete" << std::endl
				  << std::endl;
		if (avlptr != NULL)
		{
			delete avlptr;
			avlptr = NULL;
		}
		ifs.close();
		ofs.close();
	}
	std::cout << "end" << std::endl; // indicate that the program has successfuly executed all instructions
	return 0;
}

// a function that takes an AVL and returns a level-order string representation of the AVL
// returns a string representation of the nodes in level order
// std::string BSTtoString(AVL<int> *bst);

void parse_instruction(std::string temp, std::ofstream &ofs, AVL<int> *aptr)
{
	std::string command, expression;
	std::stringstream ss(temp);

	if (!(ss >> command))
	{
		return;
	} // get command, but if string was empty, return
	if (command == "Insert")
	{ // insert the given value to the AVL if possible
		int valToInsert;
		ss >> valToInsert;
		if (aptr->insert(valToInsert))
		{
			ofs << temp << " True" << std::endl;
		}
		else
		{
			ofs << temp << " False" << std::endl;
		}
	}
	else if (command == "Remove")
	{ // remove the given value from the AVL
		int valToRemove;
		ss >> valToRemove;
		if (aptr->remove(valToRemove))
		{
			ofs << temp << " True" << std::endl;
		}
		else
		{
			ofs << temp << " False" << std::endl;
		}
	}
	else if (command == "Clear")
	{ // clear the AVL
		aptr->clear();
		ofs << temp << std::endl;
	}
	else if (command == "PrintBST")
	{ // you don't need to implement any function for this command
		ofs << temp << "\n"
			<< BSTtoString(aptr->getRootNode()) << std::endl;
	}
	else
	{ // invalid command, wrong input file format
		std::cout << "Command: \"" << command << "\"" << std::endl;
		std::cout << "Invalid command.  Do you have the correct input file?" << std::endl;
	}
}

// Version of getline which does not preserve end of line characters
namespace ta
{
	std::istream &getline(std::istream &in, std::string &line)
	{
		line.clear();

		std::istream::sentry guard(in, true); // Use a sentry to maintain the state of the stream
		std::streambuf *buffer = in.rdbuf();  // Use the stream's internal buffer directly to read characters
		int c = 0;

		while (true)
		{						  // Continue to loop until a line break if found (or end of file)
			c = buffer->sbumpc(); // Read one character
			switch (c)
			{
			case '\n': // Unix style, return stream for further parsing
				return in;
			case '\r': // Dos style, check for the following '\n' and advance buffer if needed
				if (buffer->sgetc() == '\n')
				{
					buffer->sbumpc();
				}
				return in;
			case EOF: // End of File, make sure that the stream gets flagged as empty
				in.setstate(std::ios::eofbit);
				return in;
			default: // Non-linebreak character, go ahead and append the character to the line
				line.append(1, (char)c);
			}
		}
	}
}
