#include "reader_csv.h"
#include <algorithm>

reader_csv::reader_csv(std::string location, bool header)
{	
	_ifstream.open(location);
	_ifstream.seekg(0, std::ios::end);
	_file_length = _ifstream.tellg();
	_ifstream.seekg(0, std::ios::beg);
	find_start();
}

reader_csv::reader_csv(std::string location, std::vector<std::string> expectedHeaders) : reader_csv::reader_csv(location)
{
	
}

std::vector<std::string> reader_csv::read_line()
{
	std::vector<std::string> entry;
	std::string value = "";
	bool row_end = false;

	while (row_end == 0 && (_file_length > 0 || _buffer_length > 0))
	{
		//Read from file stream if buffer has reached its end.
		if (_buffer_position == _buffer_length)
		{
			std::streamsize buffer_max = CSV_READER_BUFFER_LENGTH;
			_buffer_length = std::min(_file_length, buffer_max);
			_ifstream.read(_buffer, _buffer_length);
			_buffer_position = 0;
			_file_length -= _buffer_length;
		}

		while(row_end == 0 && _buffer_position < _buffer_length)
		{
			char currentChar = _buffer[_buffer_position++];
			switch (currentChar)
			{
			case ',':
			{
				entry.push_back(value);
				value.clear();
			}
			break;

			case '\n':
			{
				entry.push_back(value);
				value.clear();
				row_end = true;
			}
			break;

			default:
			{
				value += currentChar;
				break;
			}
			}
		}
	}
	return entry;
}

void reader_csv::find_start()
{
	//Detect Byte Order Mark
	if (_file_length >= 3)
	{
		char begChars[3];
		_ifstream.read(begChars, 3);
		if (begChars[0] == '\xEF' && begChars[1] == '\xBB' && begChars[2] == '\xBF')
		{
			_file_length -= 3;
		}
		else
		{
			_ifstream.seekg(0, std::ios::beg);
		}
	}
}

void reader_csv::iterator::next()
{
	entry_row.clear();
	if (_reader->_buffer_length > 0 || _reader->_file_length > 0)
	{
		auto entry = _reader->read_line();
		entry_row = entry;
		_current_row++;
	}

	//No further items processed; end of file.
	if(entry_row.empty())
	{
		_current_row = -1;
	}
}