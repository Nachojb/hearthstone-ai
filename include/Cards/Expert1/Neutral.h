#pragma once

// http://www.hearthpwn.com/cards?filter-set=3&filter-premium=1&filter-class=1&sort=-cost&display=1
// finished: Cairne Bloodhoof

namespace Cards
{
	struct Card_CS2_188_Enchant : public EnchantmentForThisTurn<Card_CS2_188_Enchant, Attack<2>> {};
	struct Card_CS2_188 : public MinionCardBase<Card_CS2_188> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			Manipulate(context).Card(context.GetTarget()).Enchant().Add<Card_CS2_188_Enchant>();
		}
	};

	struct Card_EX1_009_Enchant : public Enchantment<Card_EX1_009_Enchant, Attack<5>> {};
	struct Card_EX1_009 : public MinionCardBase<Card_EX1_009, Taunt> {
		static auto GetEnrageTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			context.new_target = context.card_ref_;
		}
		Card_EX1_009() {
			Enrage<Card_EX1_009_Enchant>();
		}
	};

	struct Card_EX1_008 : public MinionCardBase<Card_EX1_008, Shield> {};

	struct Card_NEW1_025 : public MinionCardBase<Card_NEW1_025> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier opponent = 
				context.manipulate_.Board().GetCard(context.card_ref_).GetPlayerIdentifier().Opposite();
			state::CardRef weapon = context.manipulate_.Board().Player(opponent).GetWeaponRef();
			if (!weapon.IsValid()) return;
			Manipulate(context).Weapon(weapon).Damage(context.card_ref_, 1);
		}
	};

	struct Card_NEW1_017e : public Enchantment<Card_NEW1_017e, Attack<2>, MaxHP<2>> {};
	struct Card_NEW1_017 : public MinionCardBase<Card_NEW1_017> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Murlocs();
		}
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;

			context.manipulate_.OnBoardMinion(target).Destroy();
			context.manipulate_.OnBoardMinion(context.card_ref_).Enchant().Add<Card_NEW1_017e>();
		}
	};

	struct Card_EX1_029 : public MinionCardBase<Card_EX1_029> {
		Card_EX1_029() {
			this->deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				context.manipulate_.Hero(context.player_.Opposite())
					.Damage(context.card_ref_, 2);
			});
		}
	};

	struct Card_EX1_001e : public Enchantment<Card_EX1_001e, Attack<2>> {};
	struct Card_EX1_001 : public MinionCardBase<Card_EX1_001> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnHeal::Context context) {
			if (context.amount_ < 0) return true;
			if (context.manipulate_.GetCard(context.card_ref_).GetDamage() <= 0) return true;
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_EX1_001e>();
			return true;
		};
		Card_EX1_001() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnHeal>();
		}
	};

	struct Card_EX1_509e : public Enchantment<Card_EX1_509e, Attack<1>> {};
	struct Card_EX1_509 : public MinionCardBase<Card_EX1_509> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterMinionSummoned::Context context) {
			if (context.manipulate_.GetCard(context.card_ref_).GetRace() != state::kCardRaceMurloc) return true;
			if (context.card_ref_ == self) return true;
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_EX1_509e>();
			return true;
		}
		Card_EX1_509() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterMinionSummoned>();
		}
	};

	struct Card_EX1_080o : public Enchantment<Card_EX1_080o, Attack<1>,MaxHP<1>> {};
	struct Card_EX1_080 : public MinionCardBase<Card_EX1_080> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSecretPlayed::Context context) {
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_EX1_080o>();
			return true;
		}
		Card_EX1_080() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSecretPlayed>();
		}
	};

	struct Card_EX1_405 : public MinionCardBase<Card_EX1_405, Taunt> {};

	struct Card_CS2_146o : public Enchantment<Card_CS2_146o, Charge> {};
	struct Card_CS2_146 : public MinionCardBase<Card_CS2_146> {
		static auto GetAuraTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			state::PlayerIdentifier owner_player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			if (!context.manipulate_.Board().Player(owner_player).GetWeaponRef().IsValid()) return;
			context.new_target = context.card_ref_;
		}
		Card_CS2_146() {
			SingleEnchantmentAura<Card_CS2_146o, EmitWhenAlive, FlowControl::aura::kUpdateAlways>();
		}
	};

	struct Card_EX1_010 : public MinionCardBase<Card_EX1_010, Stealth> {};
	struct Card_CS2_169 : public MinionCardBase<Card_CS2_169, Windfury> {};
	
	struct Card_EX1_004e : public Enchantment<Card_EX1_004e, MaxHP<1>> {};
	struct Card_EX1_004 : public MinionCardBase<Card_EX1_004> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnEnd::Context context) {
			auto player = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.Board().GetCurrentPlayerId() != player) return true;

			auto targets = TargetsGenerator(player).Ally(player).Minion().Exclude(self);
			auto target = context.manipulate_.GetRandomTarget(targets.GetInfo());

			if (target.IsValid()) Manipulate(context).Card(self).Enchant().Add<Card_EX1_004e>();
			return true;
		};
		Card_EX1_004() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

	struct Card_EX1_393_Enchant : public Enchantment<Card_EX1_393_Enchant, Attack<3>> {};
	struct Card_EX1_393 : public MinionCardBase<Card_EX1_393> {
		static auto GetEnrageTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			context.new_target = context.card_ref_;
		}
		Card_EX1_393() {
			Enrage<Card_EX1_393_Enchant>();
		}
	};

	struct Card_EX1_045 : public MinionCardBase<Card_EX1_045, CantAttack> {};
	struct Card_EX1_012 : public MinionCardBase<Card_EX1_012, SpellDamage<1>> {
		Card_EX1_012() {
			deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				context.manipulate_.Hero(context.player_).DrawCard();
			});
		}
	};
	
	template <int v>
	struct Card_NEW1_018e : public Enchantment<Card_NEW1_018e<v>, Attack<v>> {};
	struct Card_NEW1_018 : public MinionCardBase<Card_NEW1_018> {
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef weapon_ref = context.manipulate_.Board().Player(context.player_).GetWeaponRef();
			if (!weapon_ref.IsValid()) return;
			int weapon_atk = context.manipulate_.GetCard(weapon_ref).GetAttack();
			if (weapon_atk <= 0) return;
			context.manipulate_.OnBoardMinion(context.card_ref_).Enchant().Add<Card_NEW1_018e>(weapon_atk);
		}
	};

	template <int v>
	struct Card_EX1_059_atk : public Enchantment<Card_EX1_059_atk<v>, SetAttack<v>> {};
	template <int v>
	struct Card_EX1_059_hp : public Enchantment<Card_EX1_059_hp<v>, SetMaxHP<v>> {};
	struct Card_EX1_059 : public MinionCardBase<Card_EX1_059> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;
			state::Cards::Card const& card = context.manipulate_.GetCard(target);
			int hp = card.GetHP();
			int atk = card.GetAttack();
			context.manipulate_.OnBoardMinion(target).Enchant().Add<Card_EX1_059_atk>(hp);
			context.manipulate_.OnBoardMinion(target).Enchant().Add<Card_EX1_059_hp>(atk);
			context.manipulate_.OnBoardMinion(target).Enchant().SetHealthToMaxHP();
		}
	};

	struct Card_EX1_162o : public Enchantment<Card_EX1_162o, Attack<1>> {};
	struct Card_EX1_162 : public MinionCardBase<Card_EX1_162> {
		Card_EX1_162() {
			AdjacentBuffAura<Card_EX1_162o>();
		}
	};

	struct Card_NEW1_021 : public MinionCardBase<Card_NEW1_021> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnStart::Context context) {
			if (context.manipulate_.GetCard(self).GetPlayerIdentifier() != context.manipulate_.Board().GetCurrentPlayerId()) return true;
			auto op = [&](state::CardRef ref) {
				context.manipulate_.OnBoardMinion(ref).Destroy();
			};
			context.manipulate_.Board().FirstPlayer().minions_.ForEach(op);
			context.manipulate_.Board().SecondPlayer().minions_.ForEach(op);
			return true;
		};
		Card_NEW1_021() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnStart>();
		}
	};

	struct Card_NEW1_023 : public MinionCardBase<Card_NEW1_023, ImmuneToSpellAndHeroPower> {};

	struct Card_NEW1_019 : public MinionCardBase<Card_NEW1_019> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterMinionSummoned::Context context) {
			if (self == context.card_ref_) return true;
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			state::PlayerIdentifier summoning = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			if (owner != summoning) return true;
			
			state::CardRef target = context.manipulate_.GetRandomTarget(TargetsGenerator(owner).Enemy(owner).Alive().GetInfo());
			if (!target.IsValid()) return true;
			context.manipulate_.OnBoardCharacter(target).Damage(context.card_ref_, 1);
			return true;
		};
		Card_NEW1_019() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterMinionSummoned>();
		}
	};

	struct Card_EX1_096 : public MinionCardBase<Card_EX1_096> {
		Card_EX1_096() {
			deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				context.manipulate_.Hero(context.player_).DrawCard();
			});
		}
	};

	struct Card_EX1_100 : public MinionCardBase<Card_EX1_100> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSpellPlayed::Context context) {
			int card_id = context.manipulate_.GetCard(context.card_ref_).GetCardId();
			state::PlayerIdentifier player = context.manipulate_.Board().GetCurrentPlayerId().Opposite();
			context.manipulate_.Hero(player).AddHandCard(card_id);
			return true;
		};
		Card_EX1_100() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSpellPlayed>();
		}
	};

	struct Card_EX1_082 : public MinionCardBase<Card_EX1_082> {
		static void Battlecry(Contexts::OnPlay context) {
			for (int i = 0; i < 3; ++i) {
				state::CardRef target = context.manipulate_.GetRandomTarget(
					TargetsGenerator(context.player_).Alive().Exclude(context.card_ref_).GetInfo()
				);
				context.manipulate_.OnBoardCharacter(target).Damage(context.card_ref_, 1);
			}
		}
	};

	struct Card_EX1_055o : public EnchantmentForThisTurn<Card_EX1_055o, Attack<2>> {};
	struct Card_EX1_055 : public MinionCardBase<Card_EX1_055> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSpellPlayed::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.player_) return true;
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_EX1_055o>();
			return true;
		};
		Card_EX1_055() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSpellPlayed>();
		}
	};

	struct Card_EX1_616 : public MinionCardBase<Card_EX1_616> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::GetPlayCardCost::Context context) {
			if (context.manipulate_.GetCard(context.card_ref_).GetCardType() != state::kCardTypeMinion) return true;
			++(*context.cost_);
			return true;
		}
		Card_EX1_616() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::GetPlayCardCost>();
		}
	};

	struct Card_NEW1_037e : public Enchantment<Card_NEW1_037e, Attack<1>> {};
	struct Card_NEW1_037 : public MinionCardBase<Card_NEW1_037> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnEnd::Context context) {
			auto player = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.Board().GetCurrentPlayerId() != player) return true;

			auto targets = TargetsGenerator(player).Ally(player).Minion().Exclude(self);
			auto target = context.manipulate_.GetRandomTarget(targets.GetInfo());

			if (target.IsValid()) Manipulate(context).Card(self).Enchant().Add<Card_NEW1_037e>();
			return true;
		};
		Card_NEW1_037() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

	struct Card_NEW1_029 : public MinionCardBase<Card_NEW1_029> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier().Opposite();
			int turn = context.manipulate_.Board().GetTurn() + 1;
			context.manipulate_.AddEvent<state::Events::EventTypes::GetPlayCardCost>(
				[turn, player](state::Events::EventTypes::GetPlayCardCost::Context context)
			{
				if (context.manipulate_.Board().GetCurrentPlayerId() != player) return true;

				int turn_now = context.manipulate_.Board().GetTurn();
				if (turn_now > turn) return false;
				if (turn_now < turn) return true;

				if (context.manipulate_.GetCard(context.card_ref_).GetCardType() != state::kCardTypeSpell) return true;
				*context.cost_ = 0;
				return true;
			});
		}
	};

	struct Card_EX1_557 : public MinionCardBase<Card_EX1_557> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnStart::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.manipulate_.Board().GetCurrentPlayerId()) return true;

			if (context.manipulate_.GetRandom().Get(2) == 0) return true;
			context.manipulate_.Hero(owner).DrawCard();
			return true;
		}
		Card_EX1_557() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnStart>();
		}
	};

	struct Card_EX1_076 : public MinionCardBase<Card_EX1_076> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::GetPlayCardCost::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.Board().GetCurrentPlayerId() != owner) return true;
			if (context.manipulate_.GetCard(context.card_ref_).GetCardType() != state::kCardTypeMinion) return true;
			if (context.manipulate_.Board().Player(context.manipulate_.Board().GetCurrentPlayerId()).played_minions_this_turn_ == 0) {
				--(*context.cost_);
			}
			return true;
		}
		Card_EX1_076() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::GetPlayCardCost>();
		}
	};

	struct Card_EX1_058 : public MinionCardBase<Card_EX1_058> {
		static void Battlecry(Contexts::OnPlay context) {
			state::Cards::Card const& card = context.manipulate_.GetCard(context.card_ref_);
			assert(card.GetCardType() == state::kCardTypeMinion);
			assert(card.GetZone() == state::kCardZonePlay);

			state::PlayerIdentifier player = card.GetPlayerIdentifier();
			auto & minions = context.manipulate_.Board().Player(player).minions_;
			int zone_pos = card.GetZonePosition();

			if (zone_pos > 0) {
				context.manipulate_.OnBoardMinion(minions.Get(zone_pos - 1)).Taunt(true);
			}
			if (zone_pos < minions.Size() - 1) {
				context.manipulate_.OnBoardMinion(minions.Get(zone_pos + 1)).Taunt(true);
			}
		}
	};

	struct Card_NEW1_020 : public MinionCardBase<Card_NEW1_020> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSpellPlayed::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.manipulate_.Board().GetCurrentPlayerId()) return true;

			auto op = [&](state::CardRef ref) {
				context.manipulate_.OnBoardMinion(ref).Damage(self, 1);
			};
			context.manipulate_.Board().FirstPlayer().minions_.ForEach(op);
			context.manipulate_.Board().SecondPlayer().minions_.ForEach(op);
			return true;
		}
		Card_NEW1_020() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSpellPlayed>();
		}
	};

	struct Card_EX1_049 : public MinionCardBase<Card_EX1_049> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return TargetsGenerator(context.player_).Ally(context).Minion();
		}
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;
			context.manipulate_.OnBoardMinion(target).MoveTo<state::kCardZoneHand>(context.player_);
		}
	};

	struct Card_EX1_007 : public MinionCardBase<Card_EX1_007> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTakeDamage::Context context) {
			context.manipulate_.Hero(
				context.manipulate_.GetCard(self).GetPlayerIdentifier()
			).DrawCard();
			return true;
		};

		Card_EX1_007() {
			RegisterEvent<OnSelfTakeDamage>();
		}
	};

	struct Card_EX1_006 : public MinionCardBase<Card_EX1_006> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnStart::Context context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.Board().GetCurrentPlayerId() != player) return true;

			std::vector<state::CardRef> candidates;
			context.manipulate_.Board().Player(player).hand_.ForEach([&](state::CardRef ref) {
				if (context.manipulate_.GetCard(ref).GetCardType() != state::kCardTypeMinion) return true;
				candidates.push_back(ref);
				return true;
			});

			if (candidates.empty()) return true;

			size_t rand = context.manipulate_.GetRandom().Get(candidates.size());
			state::CardRef target = candidates[rand];

			// swap minion
			context.manipulate_.OnBoardMinion(self).SwapWith<state::kCardZoneHand>(target);
			return true;
		}
		Card_EX1_006() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnStart>();
		}
	};

	struct Card_EX1_089 : public MinionCardBase<Card_EX1_089> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.Board().Player(context.player_.Opposite()).GetResource().GainCrystal();
		}
	};

	template <int v>
	struct Card_EX1_590e : public Enchantment<Card_EX1_590e<0>, Attack<3*v>, MaxHP<3*v>> {};
	struct Card_EX1_590 : public MinionCardBase<Card_EX1_590> {
		static void Battlecry(Contexts::OnPlay context) {
			int count = 0;
			auto op = [&](state::CardRef ref) {
				if (!context.manipulate_.GetCard(ref).HasShield()) return;

				++count;
				context.manipulate_.OnBoardMinion(ref).Shield(false);
			};

			if (count <= 0) return;
			context.manipulate_.Board().FirstPlayer().minions_.ForEach(op);
			context.manipulate_.Board().SecondPlayer().minions_.ForEach(op);
			context.manipulate_.OnBoardMinion(context.card_ref_).Enchant().Add<Card_EX1_590e>(count);
		}
	};

	struct Card_EX1_050 : public MinionCardBase<Card_EX1_050> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.Hero(state::PlayerIdentifier::First()).DrawCard();
			context.manipulate_.Hero(state::PlayerIdentifier::First()).DrawCard();
			context.manipulate_.Hero(state::PlayerIdentifier::Second()).DrawCard();
			context.manipulate_.Hero(state::PlayerIdentifier::Second()).DrawCard();
		}
	};

	struct Card_EX1_103e : public Enchantment<MaxHP<2>> {};
	struct Card_EX1_103 : public MinionCardBase<Card_EX1_103> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.Board().Player(context.player_).minions_.ForEach([&](state::CardRef ref) {
				if (ref == context.card_ref_) return;
				if (context.manipulate_.GetCard(ref).GetCardType() != state::kCardRaceMurloc) return;
				context.manipulate_.OnBoardMinion(ref).Enchant().Add<Card_EX1_103e>();
			});
		}
	};

	struct Card_EX1_102 : public MinionCardBase<Card_EX1_102> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnStart::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.manipulate_.Board().GetCurrentPlayerId()) return true;

			state::CardRef target = context.manipulate_.GetRandomTarget(TargetsGenerator(owner).Enemy(owner).GetInfo());
			assert(target.IsValid());
			context.manipulate_.OnBoardCharacter(target).Damage(self, 2);
			return true;
		}
		Card_EX1_102() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnStart>();
		}
	};

	struct Card_CS2_117 : public MinionCardBase<Card_CS2_117> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return TargetsGenerator(context.player_).Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.OnBoardCharacter(context.GetTarget()).Heal(
				context.card_ref_, 3);
		}
	};

	struct Card_EX1_170 : public MinionCardBase<Card_EX1_170, Poisonous> {};

	struct Card_tt_004o : public Enchantment<Card_tt_004o, Attack<1>> {};
	struct Card_tt_004 : public MinionCardBase<Card_tt_004> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterMinionDied::Context context) {
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_tt_004o>();
			return true;
		}
		Card_tt_004() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterMinionDied>();
		}
	};

	struct Card_EX1_556 : public MinionCardBase<Card_EX1_556> {
		Card_EX1_556() {
			this->deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				SummonAt(context, context.player_, context.zone_pos_, Cards::ID_skele21);
			});
		}
	};

	struct Card_EX1_597 : public MinionCardBase<Card_EX1_597> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnEnd::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.manipulate_.Board().GetCurrentPlayerId()) return true;

			SummonToRight(context.manipulate_, self, Cards::ID_EX1_598);
			context.manipulate_.OnBoardMinion(self).Damage(self, 1);
			return true;
		}
		Card_EX1_597() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

	struct Card_CS2_181 : public MinionCardBase<Card_CS2_181> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.OnBoardMinion(context.card_ref_).Damage(context.card_ref_, 3);
		}
	};

	struct Card_CS2_203 : public MinionCardBase<Card_CS2_203> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			return Manipulate(context).OnBoardMinion(context.GetTarget()).Silence();
		}
	};

	struct Card_EX1_017 : public MinionCardBase<Card_EX1_017, Stealth> {};

	struct Card_EX1_014te : public Enchantment<Card_EX1_014te, Attack<1>, MaxHP<1>> {};
	struct Card_EX1_014t : public SpellCardBase<Card_EX1_014t> {
		Card_EX1_014t() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter const& context) {
				*context.allow_no_target = false;
				return TargetsGenerator(context.player_).SpellTargetable().GetInfo();
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				context.manipulate_.OnBoardMinion(context.GetTarget()).Enchant().Add<Card_EX1_014te>();
			});
		}
	};
	struct Card_EX1_014 : public MinionCardBase<Card_EX1_014> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.Hero(context.player_.Opposite()).AddHandCard(Cards::ID_EX1_014t);
			context.manipulate_.Hero(context.player_.Opposite()).AddHandCard(Cards::ID_EX1_014t);
		}
	};

	struct Card_EX1_085 : public MinionCardBase<Card_EX1_085> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier().Opposite();
			auto & board_player = context.manipulate_.Board().Player(player);
			size_t count = board_player.minions_.Size();
			if (count < 4) return;
			size_t pos = context.manipulate_.GetRandom().Get(count);
			state::CardRef target = board_player.minions_.Get(pos);
			context.manipulate_.OnBoardMinion(target).ChangeOwner(player.Opposite());
		}
	};

	struct Card_EX1_507e : public Enchantment<Card_EX1_507e, Attack<2>, MaxHP<1>> {};
	struct Card_EX1_507 : public MinionCardBase<Card_EX1_507> {
		static auto GetAuraTargets(FlowControl::aura::contexts::AuraGetTargets context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			TargetsGenerator(player)
				.Ally(context).Minion().Murlocs() // friendly murlocs
				.Exclude(context.card_ref_) // only apply on other
				.GetInfo().Fill(context.manipulate_, context.new_targets);
		}
		Card_EX1_507() {
			Aura<Card_EX1_507e, EmitWhenAlive, FlowControl::aura::kUpdateWhenMinionChanges>();
		}
	};

	struct Card_EX1_044e : public Enchantment<Card_EX1_044e, Attack<1>, MaxHP<1>> {};
	struct Card_EX1_044 : public MinionCardBase<Card_EX1_044> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnPlay::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.manipulate_.Board().GetCurrentPlayerId()) return true;
			context.manipulate_.OnBoardMinion(self).Enchant().Add<Card_EX1_044e>();
			return true;
		}
		Card_EX1_044() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnPlay>();
		}
	};

	struct Card_EX1_412e : public Enchantment<Card_EX1_412e, Windfury> {};
	struct Card_EX1_412 : public MinionCardBase<Card_EX1_412> {
		static auto GetEnrageTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			context.new_target = context.card_ref_;
		}
		Card_EX1_412() {
			Enrage<Card_EX1_412e>();
		}
	};

	struct Card_EX1_020 : public MinionCardBase<Card_EX1_020, Shield> {};

	struct Card_NEW1_027e : public Enchantment<Card_NEW1_027e, Attack<1>, MaxHP<1>> {};
	struct Card_NEW1_027 : public MinionCardBase<Card_NEW1_027> {
		static auto GetAuraTargets(FlowControl::aura::contexts::AuraGetTargets context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			TargetsGenerator(player)
				.Ally(context).Minion().Pirates() // friendly murlocs
				.Exclude(context.card_ref_) // only apply on other
				.GetInfo().Fill(context.manipulate_, context.new_targets);
		}
		Card_NEW1_027() {
			Aura<Card_NEW1_027e, EmitWhenAlive, FlowControl::aura::kUpdateWhenMinionChanges>();
		}
	};

	struct Card_EX1_390_Enchant : public Enchantment<Card_EX1_390_Enchant, Attack<3>> {};
	struct Card_EX1_390 : public MinionCardBase<Card_EX1_390, Taunt> {
		static auto GetEnrageTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			context.new_target = context.card_ref_;
		}
		Card_EX1_390() {
			Enrage<Card_EX1_390_Enchant>();
		}
	};

	struct Card_EX1_021 : public MinionCardBase<Card_EX1_021, Windfury> {};

	struct Card_EX1_083 : public MinionCardBase<Card_EX1_083> {
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.manipulate_.GetRandomTarget(
				Targets(context.player_).Minion().Exclude(context.card_ref_).GetInfo()
			);
			if (!target.IsValid()) return;

			Cards::CardId transform_id;
			if (context.manipulate_.GetRandom().Get(2) == 0) transform_id = Cards::ID_EX1_tk28;
			else transform_id = Cards::ID_EX1_tk29;

			context.manipulate_.OnBoardMinion(target).Transform(transform_id);
		}
	};

	struct Card_EX1_057 : public MinionCardBase<Card_EX1_057> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return TargetsGenerator(context.player_).Ally(context).Minion();
		}
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;
			context.manipulate_.OnBoardMinion(target).MoveTo<state::kCardZoneHand>(context.player_);
		}
	};

	struct Card_EX1_584e : public Enchantment<Card_EX1_584e, SpellDamage<1>> {};
	struct Card_EX1_584 : public MinionCardBase<Card_EX1_584> {
		static void Battlecry(Contexts::OnPlay context) {
			ApplyToAdjacent(context.manipulate_, context.card_ref_, [&](state::CardRef target) {
				context.manipulate_.OnBoardMinion(target).Enchant().Add<Card_EX1_584e>();
			});
		}
	};

	struct Card_EX1_595 : public MinionCardBase<Card_EX1_595> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterMinionDied::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier() != owner) return true;
			context.manipulate_.Hero(owner).DrawCard();
			return true;
		}
		Card_EX1_595() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterMinionDied>();
		}
	};

	struct Card_EX1_046e : public EnchantmentForThisTurn<Card_EX1_046e, Attack<2>> {};
	struct Card_EX1_046 : public MinionCardBase<Card_EX1_046> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			Manipulate(context).Card(context.GetTarget()).Enchant().Add<Card_EX1_046e>();
		}
	};

	struct Card_EX1_093e : public Enchantment<Card_EX1_093e, Attack<1>, MaxHP<1>> {};
	struct Card_EX1_093 : public MinionCardBase<Card_EX1_093> {
		static void Battlecry(Contexts::OnPlay context) {
			ApplyToAdjacent(context.manipulate_, context.card_ref_, [&](state::CardRef target) {
				context.manipulate_.OnBoardMinion(target).Taunt(true);
				context.manipulate_.OnBoardMinion(target).Enchant().Add<Card_EX1_093e>();
			});
		}
	};

	struct Card_NEW1_022 : public MinionCardBase<Card_NEW1_022> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::GetPlayCardCost::Context context) {
			if (context.card_ref_ != self) return true;
			state::PlayerIdentifier player = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			state::CardRef weapon_ref = context.manipulate_.Board().Player(player).GetWeaponRef();
			if (!weapon_ref.IsValid()) return true;
			int weapon_attack = context.manipulate_.GetCard(weapon_ref).GetAttack();
			*context.cost_ -= weapon_attack;
			return true;
		}
		Card_NEW1_022() {
			RegisterEvent<InHandZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::GetPlayCardCost>();
		}
	};

	struct Card_EX1_396 : public MinionCardBase<Card_EX1_396, Taunt> {};
	struct Card_EX1_023 : public MinionCardBase<Card_EX1_023, Shield> {};

	struct Card_EX1_048 : public MinionCardBase<Card_EX1_048> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			return Manipulate(context).OnBoardMinion(context.GetTarget()).Silence();
		}
	};

	template <int v>
	struct Card_EX1_043e : public Enchantment<Card_EX1_043e<v>, MaxHP<v>> {};
	struct Card_EX1_043 : public MinionCardBase<Card_EX1_043> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier player = context.manipulate_.Board().GetCurrentPlayerId();
			int hand_cards = (int)context.manipulate_.Board().Player(player).hand_.Size();
			context.manipulate_.OnBoardMinion(context.card_ref_).Enchant()
				.Add<Card_EX1_043e>(hand_cards);
		}
	};

	struct Card_NEW1_026 : public MinionCardBase<Card_NEW1_026> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSpellPlayed::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.player_ != owner) return true;
			SummonToRight(context.manipulate_, self, Cards::ID_NEW1_026t);
			return true;
		}
		Card_NEW1_026() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSpellPlayed>();
		}
	};

	struct Card_EX1_097 : public MinionCardBase<Card_EX1_097, Taunt> {
		Card_EX1_097() {
			this->deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				auto op = [&](state::CardRef card_ref) {
					context.manipulate_.OnBoardMinion(card_ref).Damage(context.card_ref_, 2);
				};
				context.manipulate_.Board().FirstPlayer().minions_.ForEach(op);
				context.manipulate_.Board().SecondPlayer().minions_.ForEach(op);
				context.manipulate_.Hero(state::PlayerIdentifier::First()).Damage(context.card_ref_, 2);
				context.manipulate_.Hero(state::PlayerIdentifier::Second()).Damage(context.card_ref_, 2);
			});
		}
	};

	struct Card_EX1_005 : public MinionCardBase<Card_EX1_005> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().MinionAttackerGreaterOrEqualTo(7);
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			return Manipulate(context).OnBoardMinion(context.GetTarget()).Destroy();
		}
	};
	
	struct Card_NEW1_024o : public Enchantment<Card_NEW1_024o, Attack<1>, MaxHP<1>> {};
	struct Card_NEW1_024 : public MinionCardBase<Card_NEW1_024> {
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;
			state::PlayerIdentifier owner = context.player_;
			context.manipulate_.Weapon(owner).Enchant().Add<Card_NEW1_024o>();
		}
	};

	struct Card_EX1_564 : public MinionCardBase<Card_EX1_564> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets(context.player_).Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;

			state::CardRef new_ref = Manipulate(context).OnBoardMinion(context.card_ref_)
				.BecomeCopyof(context.GetTarget());

			*context.new_card_ref = new_ref;
		}
	};

	struct Card_CS1_069 : public MinionCardBase<Card_CS1_069, Taunt> {};

	struct Card_EX1_558 : public MinionCardBase<Card_EX1_558> {
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef weapon_ref = context.manipulate_.Board().Player(context.player_.Opposite()).GetWeaponRef();
			int draw = context.manipulate_.GetCard(weapon_ref).GetHP();
			context.manipulate_.Hero(context.player_.Opposite()).DestroyWeapon();
			for (int i = 0; i < draw; ++i) {
				context.manipulate_.Hero(context.player_).DrawCard();
			}
		}
	};

	struct Card_EX1_116 : public MinionCardBase<Card_EX1_116, Charge> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier player = context.player_.Opposite();
			SummonToRightmost(context.manipulate_, player, Cards::ID_EX1_116t);
			SummonToRightmost(context.manipulate_, player, Cards::ID_EX1_116t);
		}
	};

	struct Card_CS2_151 : public MinionCardBase<Card_CS2_151> {
		static void Battlecry(Contexts::OnPlay context) {
			SummonToRight(context.manipulate_, context.card_ref_, Cards::ID_CS2_152);
		}
	};

	struct Card_CS2_221e : public Enchantment<Card_CS2_221e, Attack<2>> {};
	struct Card_CS2_221 : public MinionCardBase<Card_CS2_221> {
		static auto GetEnrageTarget(FlowControl::aura::contexts::AuraGetTarget context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(context.card_ref_).GetPlayerIdentifier();
			state::CardRef weapon_ref = context.manipulate_.Board().Player(owner).GetWeaponRef();
			context.new_target = weapon_ref;
		}
		Card_CS2_221() {
			Enrage<Card_CS2_221e>();
		}
	};

	struct Card_NEW1_041 : public MinionCardBase<Card_NEW1_041> {
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.manipulate_.GetRandomTarget(TargetsGenerator(context.player_)
				.Enemy(context.player_).Minion().MinionAttackerLessOrEqualTo(2).GetInfo());
			if (!target.IsValid()) return;
			context.manipulate_.OnBoardMinion(target).Destroy();
		}
	};

	struct Card_EX1_028 : public MinionCardBase<Card_EX1_028, Stealth> {};

	struct Card_CS2_227 : public MinionCardBase<Card_CS2_227> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::GetPlayCardCost::Context context) {
			state::PlayerIdentifier player = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (context.manipulate_.Board().GetCurrentPlayerId() != player) return true;
			*context.cost_ += 3;
			return true;
		}
		Card_CS2_227() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::GetPlayCardCost>();
		}
	};

	struct Card_EX1_067 : public MinionCardBase<Card_EX1_067, Charge, Shield> {};

	struct Card_EX1_110 : public MinionCardBase<Card_EX1_110> {
		Card_EX1_110() {
			deathrattle_handler.Add([](FlowControl::deathrattle::context::Deathrattle context) {
				SummonAt(context, context.player_, context.zone_pos_, Cards::ID_EX1_110t);
			});
		}
	};

	struct Card_EX1_283 : public MinionCardBase<Card_EX1_283> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return TargetsGenerator(context.player_).Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;
			context.manipulate_.OnBoardCharacter(target).Freeze(true);
		}
	};

	struct Card_EX1_095 : public MinionCardBase<Card_EX1_095> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::AfterSpellPlayed::Context context) {
			state::PlayerIdentifier owner = context.manipulate_.GetCard(self).GetPlayerIdentifier();
			if (owner != context.player_) return true;
			context.manipulate_.Hero(owner).DrawCard();
			return true;
		}
		Card_EX1_095() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::AfterSpellPlayed>();
		}
	};

	struct Card_NEW1_040t : public MinionCardBase<Card_NEW1_040t, Taunt> {};
	struct Card_NEW1_040 : public MinionCardBase<Card_NEW1_040> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnEnd::Context context) {
			SummonToRight(context.manipulate_, self, Cards::ID_NEW1_040t);
			return true;
		};
		Card_NEW1_040() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

	struct Card_EX1_614 : public MinionCardBase<Card_EX1_614> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnPlay::Context context) {
			SummonToRight(context.manipulate_, self, Cards::ID_EX1_614t);
			return true;
		}
		Card_EX1_614() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnPlay>();
		}
	};


	struct Card_NEW1_038_Enchant : public Enchantment<Card_NEW1_038_Enchant, Attack<1>, MaxHP<1>> {};
	struct Card_NEW1_038 : public MinionCardBase<Card_NEW1_038> {
		static bool HandleEvent(state::CardRef self, state::Events::EventTypes::OnTurnEnd::Context context) {
			Manipulate(context).Card(self).Enchant().Add<Card_NEW1_038_Enchant>();
			return true;
		};
		Card_NEW1_038() {
			RegisterEvent<MinionInPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

}

REGISTER_CARD(EX1_614)
REGISTER_CARD(NEW1_040t)
REGISTER_CARD(NEW1_040)
REGISTER_CARD(EX1_095)
REGISTER_CARD(EX1_283)
REGISTER_CARD(EX1_110)
REGISTER_CARD(EX1_067)
REGISTER_CARD(CS2_227)
REGISTER_CARD(EX1_028)
REGISTER_CARD(NEW1_041)
REGISTER_CARD(CS2_221)
REGISTER_CARD(CS2_151)
REGISTER_CARD(EX1_116)
REGISTER_CARD(EX1_558)
REGISTER_CARD(CS1_069)
REGISTER_CARD(EX1_564)
REGISTER_CARD(NEW1_024)
REGISTER_CARD(EX1_005)
REGISTER_CARD(EX1_097)
REGISTER_CARD(NEW1_026)
REGISTER_CARD(EX1_043)
REGISTER_CARD(EX1_048)
REGISTER_CARD(EX1_023)
REGISTER_CARD(EX1_396)
REGISTER_CARD(NEW1_022)
REGISTER_CARD(EX1_093)
REGISTER_CARD(EX1_046)
REGISTER_CARD(EX1_595)
REGISTER_CARD(EX1_584)
REGISTER_CARD(EX1_057)
REGISTER_CARD(EX1_083)
REGISTER_CARD(EX1_021)
REGISTER_CARD(EX1_390)
REGISTER_CARD(NEW1_027)
REGISTER_CARD(EX1_020)
REGISTER_CARD(EX1_412)
REGISTER_CARD(EX1_044)
REGISTER_CARD(EX1_507)
REGISTER_CARD(EX1_085)
REGISTER_CARD(EX1_014t)
REGISTER_CARD(EX1_014)
REGISTER_CARD(EX1_017)
REGISTER_CARD(CS2_203)
REGISTER_CARD(CS2_181)
REGISTER_CARD(EX1_597)
REGISTER_CARD(EX1_556)
REGISTER_CARD(tt_004)
REGISTER_CARD(EX1_170)
REGISTER_CARD(CS2_117)
REGISTER_CARD(EX1_102)
REGISTER_CARD(EX1_103)
REGISTER_CARD(EX1_050)
REGISTER_CARD(EX1_590)
REGISTER_CARD(EX1_089)
REGISTER_CARD(EX1_006)
REGISTER_CARD(EX1_007)
REGISTER_CARD(EX1_049)
REGISTER_CARD(NEW1_020)
REGISTER_CARD(EX1_058)
REGISTER_CARD(EX1_076)
REGISTER_CARD(EX1_557)
REGISTER_CARD(NEW1_029)
REGISTER_CARD(NEW1_037)
REGISTER_CARD(EX1_616)
REGISTER_CARD(EX1_055)
REGISTER_CARD(EX1_082)
REGISTER_CARD(EX1_100)
REGISTER_CARD(EX1_096)
REGISTER_CARD(NEW1_019)
REGISTER_CARD(NEW1_023)
REGISTER_CARD(NEW1_021)
REGISTER_CARD(EX1_162)
REGISTER_CARD(EX1_059)
REGISTER_CARD(NEW1_018)
REGISTER_CARD(EX1_012)
REGISTER_CARD(EX1_045)
REGISTER_CARD(EX1_393)
REGISTER_CARD(EX1_004)
REGISTER_CARD(CS2_169)
REGISTER_CARD(EX1_010)
REGISTER_CARD(CS2_146)
REGISTER_CARD(EX1_405)
REGISTER_CARD(EX1_080)
REGISTER_CARD(EX1_509)
REGISTER_CARD(EX1_001)
REGISTER_CARD(EX1_029)
REGISTER_CARD(NEW1_017)
REGISTER_CARD(NEW1_025)
REGISTER_CARD(EX1_008)
REGISTER_CARD(EX1_009)
REGISTER_CARD(CS2_188)

REGISTER_CARD(NEW1_038)
