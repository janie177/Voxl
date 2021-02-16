#pragma once
#include <string>
#include <fstream>

namespace utilities
{
	class FileWriter
	{
	public:
		FileWriter(const std::string& path);

	public:
		/*
		 * Try to open the file with the path of this FileWriter.
		 * If the file couldn't be opened, returns false.
		 */
		bool open();

		/*
		 * Try to close the opened file.
		 * If the file was not open or invalid, false is returned.
		 */
		bool close();

		/*
		 * Write to the file.
		 */
		bool write(const std::string& string);

		/*
		 * Flush all data in the buffer to the file without actually having to close the file.
		 */
		bool flush();

	private:
		std::ofstream file;
		std::string path;
		bool isOpen;
	};
}
