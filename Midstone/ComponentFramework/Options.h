#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
#include <string>
#include <Vector.h>

using namespace MATH;

class Options
{
public:
	float musicVol;
	float sfxVol;
	Vec4 shipColour;

	void SaveOptions(const std::string& filename, float mVol, float sVol, Vec4 sCol);
	void readOptions(const std::string& filename);
};

#endif // OPTIONS_H

