#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace ENG 
{

	class Score
	{
	public:
		Score();
		bool insertScore(std::string name, int score);
		const int maxScoreList = 5;
		void readScore();
		std::vector<int> timeScore;
		std::vector<std::string> playerName;

	};

}