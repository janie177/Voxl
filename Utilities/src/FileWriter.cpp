#include "file/FileWriter.h"

namespace utilities
{
	FileWriter::FileWriter(const std::string& path) : path(path), isOpen(false)
	{
		
	}

	bool FileWriter::open()
	{
		file.open(path, std::ios_base::app);
		if(file.good())
		{
			isOpen = true;
			return true;
		}
		return false;
	}

	bool FileWriter::close()
	{
		if (isOpen)
		{
			file.close();
			return file.good();
		}
		return false;
	}

	bool FileWriter::write(const std::string& string)
	{
		if(isOpen)
		{
			file << string;
			return true;
		}
		return false;
	}

	bool FileWriter::flush()
	{
		if(isOpen)
		{
			file.flush();
			return file.good();
		}
		return false;
	}
}
