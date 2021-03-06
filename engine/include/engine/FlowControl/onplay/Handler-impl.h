#pragma once

#include "engine/FlowControl/onplay/Handler.h"

#include "engine/FlowControl/onplay/Contexts.h"
#include "engine/FlowControl/onplay/Contexts-impl.h"

#include "state/State.h"
#include "engine/FlowControl/FlowContext.h"

namespace engine {
	namespace FlowControl
	{
		namespace onplay
		{
			inline bool Handler::CheckPlayable(state::State const & state, state::PlayerIdentifier player, state::CardRef card_ref) const
			{
				if (check_playable) {
					context::CheckPlayable context(state, player);
					if (!(*check_playable)(context)) return false;
				}

				// check target exists (if required)
				if (specified_target_getter) {
					context::GetSpecifiedTarget context(state, player, card_ref);
					// Don't call context.SetChooseOneChoice, so the choice will be kInvalidCardId
					// Client card returns the targets rule to decide playable or not
					(*specified_target_getter)(context);

					if (context.NeedToPrepareTarget() && !context.IsAllowedNoTarget()) {
						state::targetor::Targets targets_rule = context.GetTargets();
						std::vector<state::CardRef> targets;
						bool targets_empty = true;
						targets_rule.ForEach(state, [&](...) {
							targets_empty = false;
							return false;
						});
						if (targets_empty) return false;
					}
				}

				return true;
			}

			inline void Handler::PrepareChooseOne(state::State & state, FlowContext & flow_context) const
			{
				Cards::CardId choice = Cards::kInvalidCardId;
				if (choose_one) {
					context::GetChooseOneOptions context(state, flow_context);
					choice = choose_one(context);
				}
				Manipulate(state, flow_context).SaveUserChoice(choice);
			}

			inline void Handler::PrepareTarget(state::State & state, FlowContext & flow_context, state::PlayerIdentifier player, state::CardRef card_ref) const
			{
				PrepareChooseOne(state, flow_context);

				if (specified_target_getter) {
					context::GetSpecifiedTarget context(state, player, card_ref);
					context.SetChooseOneChoice(flow_context.GetSavedUserChoice());
					(*specified_target_getter)(context);

					if (context.NeedToPrepareTarget()) {
						// Note: do not check context.IsAllowedNoTarget() here
						// That should already be checked when checking playable or not
						// And, it's possible that, there's a valid target when checking playable,
						// but the target vanished when actually play that
						state::targetor::Targets targets = context.GetTargets();
						flow_context.PrepareSpecifiedTarget(state, card_ref, targets);
					}
				}
			}

			inline void Handler::OnPlay(state::State & state, FlowContext & flow_context, state::PlayerIdentifier player, state::CardRef card_ref, state::CardRef * new_card_ref) const
			{
				if (!onplay) return;
				(*onplay)({ Manipulate(state, flow_context), player, card_ref, new_card_ref });
			}
		}
	}
}