#pragma once
#include <fstream>
#include <string>

namespace utilities
{
	/*
	 * FileReader can open a file from a string path.
	 * It can then read the file line by line, or copy it into a char array.
	 */
	class FileReader {
	private:
		std::ifstream file;
		std::string path;
		std::string currentLine;
		std::size_t size;
		bool eof;
		bool isOpen;

	public:
		FileReader(const std::string& path);

		bool open();
		bool close();
		bool nextLine();
		std::string getCurrentLine() const;
		std::ifstream& getFile();

		/*
		 * Store this entire file in an array.
		 * Size is checked to make sure it fits.
		 */
		std::unique_ptr<char[]> toArray();

		//Set location in the file stream
		void rewind();
		std::int32_t getPos();

		/*
		 * Set the current position in the file.
		 * If the position is invalid, false is returned.
		 */
		bool setPos(std::int32_t pos);

		std::size_t getSize() const;
	};
}