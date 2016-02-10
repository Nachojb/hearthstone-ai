#pragma once

#include <vector>
#include "mcts.h"
#include "game-engine/move.h"

class Decider
{
public:
	struct MoveInfo {
		GameEngine::Move move;
		int wins;
		int count;
	};

	class MovesInfo
	{
	public:
		void DebugPrint();
		void Clear() { this->moves.clear(); }

		std::vector<MoveInfo> moves;
	};

public:
	void Add(const MCTS& mcts);

	MovesInfo GetBestMoves();

	void DebugPrint();

private:
	struct ProgressData {
		MCTS const* mcts;
		TreeNode const* node;
	};

private:
	int GetRandom();

	std::unordered_map<GameEngine::Move, TreeNode> AggregateProgressChildren(std::vector<ProgressData> const& progresses);
	void GoToNextProgress(std::vector<ProgressData> &progresses, ProgressData const* stepping_progress, TreeNode const* stepping_node, const GameEngine::Board &next_board);
	void GoToNextProgress(std::vector<ProgressData> &progresses, GameEngine::Move const& move);
	bool GetNextStep(std::vector<ProgressData> &progress, GameEngine::Board &board, MoveInfo &move);
	GameEngine::Board GetCurrentBoard();

	void PrintTree(TreeNode const *node, int level, const int max_level);
	void PrintBestRoute(int levels);

private:
	std::vector<MCTS const*> data;
	MovesInfo best_moves;
};