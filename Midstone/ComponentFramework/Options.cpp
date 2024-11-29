#include "Options.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Debug.h"

void Options::SaveOptions(const std::string& filename, float mVol, float sVol, Vec4 sCol)
{
	std::ofstream file(filename);

	if (file.is_open())
	{
		file << mVol << " " << sVol << " " << sCol.x << " " << sCol.y << " " << sCol.z << " " << sCol.w << std::endl;

		Debug::Info("Options have been saved", __FILE__, __LINE__);
		file.close();
	}
	else
	{
		Debug::Info("Options have not been saved. Something went wrong.", __FILE__, __LINE__);
	}
}

void Options::readOptions(const std::string& filename)
{
	std::ifstream file (filename);

	if (file.is_open())
	{
		file >> musicVol >> sfxVol >> shipColour.x >> shipColour.y >> shipColour.z >> shipColour.w;

		Debug::Info("Options have been loaded", __FILE__, __LINE__);
	}
	else
	{
		Debug::Info("Options have not been loaded. Something went wrong.", __FILE__, __LINE__);
	}
}
