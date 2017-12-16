#pragma once

#include <shared_mutex>
#include "MCTS/Types.h"
#include "FlowControl/PlayerStateView.h"
#include "Utils/SpinLocks.h"

namespace mcts
{
	namespace board
	{
		class IRawActionParameterGetter;
		class IRandomGenerator;

		class BoardActionAnalyzer
		{
		public:
			BoardActionAnalyzer() : mutex_(), op_map_(), op_map_size_(0), attackers_(), playable_cards_() {}

			BoardActionAnalyzer(BoardActionAnalyzer const& rhs) :
				mutex_(),
				op_map_(rhs.op_map_),
				op_map_size_(rhs.op_map_size_),
				attackers_(rhs.attackers_),
				playable_cards_(rhs.playable_cards_)
			{}

			BoardActionAnalyzer & operator=(BoardActionAnalyzer const& rhs) {
				op_map_ = rhs.op_map_;
				op_map_size_ = rhs.op_map_size_;
				attackers_ = rhs.attackers_;
				playable_cards_ = rhs.playable_cards_;
				return *this;
			}

			void Reset() { op_map_size_ = 0; }

			void Prepare(FlowControl::CurrentPlayerStateView const& board);
			int GetActionsCount() const { return (int)op_map_size_; }
			auto const& GetAttackers() const { return attackers_; }
			auto const& GetPlayableCards() const { return playable_cards_; }


			template <class Functor>
			void ForEachMainOp(Functor && functor) const {
				std::shared_lock<Utils::SharedSpinLock> lock(mutex_);
				for (size_t i = 0; i < op_map_size_; ++i) {
					if (!functor(i, GetMainOpType(op_map_[i]))) return;
				}
			}
			FlowControl::MainOpType GetMainOpType(size_t choice) const {
				std::shared_lock<Utils::SharedSpinLock> lock(mutex_);
				return op_map_[choice];
			}
			template <class Functor>
			void ForEachPlayableCard(Functor && functor) const {
				std::shared_lock<Utils::SharedSpinLock> lock(mutex_);
				for (auto hand_idx : playable_cards_) {
					if (!functor(hand_idx)) break;
				}
			}
			size_t GetPlaybleCard(size_t idx) const {
				std::shared_lock<Utils::SharedSpinLock> lock(mutex_);
				return playable_cards_[idx];
			}

			int GetEncodedAttackerIndex(size_t idx) const {
				std::shared_lock<Utils::SharedSpinLock> lock(mutex_);
				return attackers_[idx];
			}

		private:
			mutable Utils::SharedSpinLock mutex_;
			std::array<FlowControl::MainOpType, FlowControl::MainOpType::kMainOpMax> op_map_;
			size_t op_map_size_;
			std::vector<int> attackers_;
			std::vector<size_t> playable_cards_;
		};
	}
}