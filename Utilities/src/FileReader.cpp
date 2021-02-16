#include "file/FileReader.h"

namespace utilities
{
	FileReader::FileReader(const std::string& path) 
	{
		this->path = path;
		eof = false;
		size = -1;
		isOpen = false;
	}

	bool FileReader::open() 
	{
		file.open(path, std::ios::in);
		if (file.good())
		{
			//Find size.
			size = static_cast<std::size_t>(file.seekg(0, file.end).tellg());
			file.seekg(0, file.beg);
			isOpen = true;
			return true;
		}

		return false;
	}

	bool FileReader::close() 
	{
		if(isOpen)
		{
			file.close();
			isOpen = false;
			eof = false;
			size = -1;
			return file.good();
		}
		return false;
	}

	//Read the next line as long as there's lines available.
	//Ignore any lines that start with //
	bool FileReader::nextLine() 
	{
		while (!eof) 
		{
			eof = std::getline(file, currentLine).eof();
			if (!currentLine.empty() && currentLine.substr(0, 2) != "//") 
			{
				return true;
			}
		}
		return false;
	}

	std::string FileReader::getCurrentLine() const
	{
		return currentLine;
	}

	std::ifstream& FileReader::getFile() 
	{
		return file;
	}

	std::unique_ptr<char[]> FileReader::toArray()
	{
		if (size <= 0)
		{
			return nullptr;
		}

		std::unique_ptr<char[]> array = std::unique_ptr<char[]>(new char[size + 1]);
		char c;
		std::uint32_t index = 0;
		while (file.get(c) && index < size)
		{
			array[index] = c;
			index++;
		}

		//null terminate at the end of the source.
		array[index] = '\0';

		return array;
	}

	//Start stream from the start again.
	void FileReader::rewind() 
	{
		file.clear();
		file.seekg(0);
		eof = false;
	}

	std::int32_t FileReader::getPos()
	{
		return static_cast<std::int32_t>(file.tellg());
	}

	bool FileReader::setPos(std::int32_t pos) 
	{
		const auto end = file.seekg(0, file.end).tellg();

		if (pos <= end && pos >= 0)
		{
			file.clear();
			file.seekg(pos);
			eof = pos >= end;
			return true;
		}

		//Out of range
		return false;
	}

	std::size_t FileReader::getSize() const
	{
		return size;
	}
}