#pragma once

#include <functional>
#include <utility>
#include <memory>
#include "Utils/CloneableContainers/RemovableVector.h"
#include "state/Cards/EnchantableStates.h"

namespace state
{
	class State;
	namespace Cards { class Card; }
}

namespace FlowControl
{
	namespace enchantment
	{
		class Enchantments
		{
		public:
			typedef void(*ApplyFunctor)(state::Cards::EnchantableStates &);
			struct ManagedEnchantment {
				ApplyFunctor apply_functor;
				int valid_until_turn; // -1 if always valid
			};
			typedef Utils::CloneableContainers::RemovableVector<ManagedEnchantment> ContainerType;

			typename ContainerType::Identifier PushBack(ApplyFunctor apply_functor)
			{
				int valid_until_turn = -1; // TODO
				return enchantments_.PushBack(ManagedEnchantment{apply_functor, valid_until_turn});
			}

			void Remove(ContainerType::Identifier id)
			{
				return enchantments_.Remove(id);
			}

			void Clear()
			{
				enchantments_.Clear();
			}

			bool Exists(ContainerType::Identifier id) const
			{
				return enchantments_.Get(id) != nullptr;
			}

			void ApplyAll(state::Cards::EnchantableStates & card)
			{
				enchantments_.IterateAll([&card](ManagedEnchantment& item) -> bool {
					item.apply_functor(card);
					return true;
				});
			}

		private:
			ContainerType enchantments_;
		};
	}
}