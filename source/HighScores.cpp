#include "..\include\HighScores.h"

/*
sorts descending
returns true if the first int is greater than the second, else returns false;
*/
bool SortFunction(const int& first, const int& second)
{
	return first > second;
}

HighScores::HighScores()
{
}


HighScores::~HighScores()
{
}

void HighScores::LoadScores()
{
	std::fstream fs;

	//open file if exists, create if not. read mode
	fs.open("leaderboard.txt", std::fstream::in);

	if (fs.is_open())
	{
		//read until end of file
		while (!fs.eof())
		{
			int score = 0;
			fs >> score;
			//verify int was read
			if (fs.good())
			{
				scores.push_back(score);
			}
		}
		fs.close();
	}
}

void HighScores::SaveScores()
{
	std::fstream fs;

	//open file for writing and to overwrite existing
	fs.open("leaderboard.txt", std::fstream::out | std::fstream::trunc);

	//LoadScores should always run before so this should never fail
	assert(fs.is_open());

	if (fs.is_open())
	{
		//only save top 5 
		int counter = 0;
		for (std::list<int>::iterator it = scores.begin(); it != scores.end(); it++)
		{
			//output to fstream
			fs << *it << std::endl;
			counter++;
			if (counter >= 5)
			{
				break;
			}
		}
		fs.close();
	}
}

void HighScores::AddScore(int a_score)
{
	scores.push_back(a_score);
}

void HighScores::SortScores()
{
	scores.sort(SortFunction);
}

bool HighScores::IsEmpty()
{
	return scores.size() == 0;
}

const std::list<int> HighScores::GetScores()
{
	return scores;
}

const int HighScores::GetHighScore()
{
	if (scores.size() == 0)
	{
		return 0;
	}
	else
	{
		return scores.front();
	}
}

