#pragma once
#ifndef _HIGH_SCORES_H_
#define _HIGH_SCORES_H_

#include <list>
#include <fstream>
#include <assert.h>

class HighScores
{
public:
	HighScores();
	~HighScores();

	void LoadScores();

	void SaveScores();

	void AddScore(int a_score);

	void SortScores();

	bool IsEmpty();

	const std::list<int> GetScores();

	const int GetHighScore();

private:
	std::list<int> scores;

	
};

#endif
