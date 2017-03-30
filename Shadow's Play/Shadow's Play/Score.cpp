#include "Score.h"
#include <fstream>
#include <iostream>
#define pathOfScore "../assets/texts/highscores.txt"

namespace ENG
{

	bool Score::insertScore(std::string name, int score)
	{
		readScore();
		if (score < timeScore[maxScoreList - 1])
			return false;
		for (int i = 0; i < maxScoreList; i++)
		{
			if (score > timeScore[i])
			{
				timeScore.insert(timeScore.begin() + i, score);
				timeScore.pop_back();
				playerName.insert(playerName.begin() + i, name);
				playerName.pop_back();
				i = maxScoreList;
			}
		}
		std::ifstream inFile(pathOfScore); //Exception handling here would be good
		if (inFile)
		{
			std::string currentLine;
			std::string tempScore;
			std::string tempName;
			std::vector<std::string> scoreList;
			while (!inFile.eof())
			{
				std::getline(inFile, currentLine, '\n');
				if (!inFile.eof())
				{
					scoreList.push_back(currentLine);
				}
			}
			inFile.close();
			std::ofstream outFile(pathOfScore, std::ios::out);
			if (!outFile)
			{
				std::cout << "Something is very very wrong" << std::endl;
			}
			else
			{
				for (int i = 0; i < scoreList.size(); i++)
				{
					outFile << timeScore[i] << " " << playerName[i] << std::endl;
				}
			}
			outFile.close();
		}
		return true;
	}

	void Score::readScore()
	{
		std::ifstream inFile(pathOfScore, std::ios::binary);
		if (inFile)
		{
			std::string currentLine;
			int score;
			char line[512];
			char scoreName[512];
			for (int i = 0; i < maxScoreList; i++)
			{
				inFile.getline(line, 512);
				std::sscanf(line, "%i %s", &score, &scoreName);
				playerName[i] = scoreName;
				timeScore[i] = score;
			}
			inFile.close();
			return;
		}
		std::cout << "It no work good dude" << std::endl;
	}



	Score::Score()
	{
		timeScore.resize(maxScoreList);
		playerName.resize(maxScoreList);
	}

}