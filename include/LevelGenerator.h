#pragma once
#include <fstream>
class LevelGenerator {
public:
	virtual int getScore()=0;
	virtual bool Update()=0;
	virtual void Draw()=0;
	virtual void ToFile(std::ostream& out)=0;
	virtual ~LevelGenerator();
};
