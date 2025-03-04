#pragma once
#include "unreal_engine_class.hpp"
#include "uobject.hpp"

namespace big
{
#pragma pack(push, 1)

	struct SkillSlot
	{
		int8_t m_slot_1;
		int8_t m_slot_2;
		int8_t m_slot_3;
		int8_t m_slot_4;
		int8_t m_slot_5;
		int8_t m_slot_6;
		int8_t m_slot_7;
		int8_t m_slot_8;
	};

	class BP_SAS_PlayerState
	{
	public:
		char pad_0000[0xE0];//0x0000
		bool m_disable_sas_duration; //0x00E0
	};

	class PlayerSkill
	{
	public:
		SkillSlot m_skill;
	};

	class SkillCooldown
	{
	public:
		char pad_0000[8]; //0x0000
		int8_t m_selected_skill;
		char pad_0009[3];//0x0009
		//size 0x000C
	};

	class SASRecastBase
	{
	public:
		class SkillCooldown m_skill_cooldown[0xFF]; //0x000C
	};

	class SASRecastGauge
	{
	public:
		char pad_0000[0xD8]; //0x0000
		class PlayerSkill* m_player_skill; //0x00D8
		char pad_00E0[0x8]; //0x00E0
		class SASRecastBase* m_sas_recast; //0x00E8
		int m_num_skill; //0x00F0
	};

	class PlayerGauge
	{
	public:
		char pad_0000[0xC8]; //0x0000
		float m_telekinetic_gauge; //0x00C8
		float m_max_telekinetic_gauge; //0x00CC
		char pad_00D0[0x1C]; //0x00D0
		float m_battle_bonus_rate; //0x00EC
		char pad_00F0[152]; //0x00F0
		float m_telekinetic_regen; //0x0188
	};

	class CItemList
	{
	public:
		int64_t pad_0000; //0x0000
		int8_t m_selected_items; //0x0008
		char pad_0009[3]; //0x0009
	};

	class ItemRecast
	{
	public:
		char pad_0000[0xC]; //0x0000
		CItemList m_item_list[0xFF]; //0x000C
	};

	class ItemUseComponent
	{
	public:
		char pad_0000[0xF8]; //0x0000
		class ItemRecast* m_item_recast; //0x00F8
		int m_num_item; // 0x0100
	};

	class PlayerDriveComponent
	{
	public:
		char pad_0000[0x180]; //0x0000
		float m_current_drive_progress; //0x0180
		float m_drive_accumulative_rate; //0x0184
		char pad_0188[0x1A]; //0x0188
		bool m_enter_brain_drive; //0x01A2
		char pad_01A3[0x45]; //0x01A3
		float m_drive_timer_tick_rate; //0x01E8
		char pad_01EC[0x30]; //0x01EC
		float m_instant_brain_field; //0x021C
		char pad_0220[0x2C]; //0x0220
		float m_infinite_brain_drive; //0x024C
		float drive_duration_timer_max; //0x0250
	};

	class CharactersParameter
	{
	public:
		char pad_0000[0x118]; //0x0000
		int m_max_health; //0x0118
		char pad_011C[500]; //0x011C
		int m_health; //0x0310
	};

	class CharacterMovement
	{
	public:
		char pad_0000[0x168]; //0x0000
		float m_jump_velocity; //0x0168
	};

	class UserParamManager
	{
	public:
		char pad_0000[0x50]; //0x0000
		int m_credit; //0x0050
		char pad_0054[0x27C]; //0x0054
		int m_battle_point; //0x02D0
		int m_skill_point; //0x02D4
	};

	class DamageCalc
	{
	public:
		char pad_0000[0xC0]; //0x0000
		float m_dmg_factor;
		float m_obj_dmg_factor;
		float m_fire_dmg_factor;
		float m_electric_dmg_factor;
		float m_random_factor_min;
		float m_random_factor_max;
		float m_critical_factor;
		float m_critical_crash_factor;
		float m_bad_up_factor;
		float m_bad_down_factor;
		float m_crash_factor;
		float m_armor_dmg_factor;
		float m_copy_psy_armor_factor;
		float m_psy_dmg_factor_boss;
	};

	class Character
	{
	public:
		char pad_0000[0x298]; //0x0000
		class CharacterMovement* m_ability; //0x0298
		char pad_02A0[0x6A8]; //0x02A0
		class CharactersParameter* m_character_params; //0x0948
		char pad_0950[0x48]; //0x0950
		uint64_t m_player_damage; //0x998
		char pad_09A0[8]; //0x09A0
		class DamageCalc* m_character_stats; //0x09A8
		char pad_9B0[0x13C]; //0x09B0
		int m_attack_power; //0x0AEC
		int m_brain_attack_power; //0x0AF0
		char pad_00AF4[0x574]; //0x0AF4
		class PlayerDriveComponent* m_brain_drive; //0x1068
		char pad_1070[0x100]; //0x1070
		class ItemUseComponent* m_item_use_recast; //0x1170
		char pad_1178[0x20]; //0x1178
		class PlayerGauge* m_gauge_base; //0x1198
		char pad_11A0[0x20]; //0x11A0
		class SASRecastGauge* m_sas_base; //0x11C0
		char pad_11C8[0x60]; //0x11C8
		class SprintBP* m_sprint; //0x1228
		char pad_1230[0x908]; //0x1230
		class BP_SAS_PlayerState* m_sas_state; //0x1B38
	};

	class PlayerController
	{
	public:
		char pad_0000[0x270];
		class Character* m_character;
	};

	class LocalPlayer
	{
	public:
		char pad_0000[0x30];
		class PlayerController* m_get_character;
	};

	class GameInstance
	{
	public:
		char pad_0000[0x38]; //0x0000
		TArray<LocalPlayer*> m_character_base; //0x0038
		char pad_0048[656]; //0x0048
		TArray<UserParamManager*> m_manager; //0x2D8
	};

	class EngineBase : public UObject
	{
	public:
		char pad_0028[0xDE0]; //0x0028
		class GameInstance* m_game; //0x0E08
	};

	static_assert(sizeof(GameInstance) == 0x2E8, "GameInstance not Properly sized");
	static_assert(sizeof(Character) == 0x1B40);
	static_assert(sizeof(UserParamManager) == 0x2D8, "UserParamManager not proper sized");

	static_assert(sizeof(CharacterMovement) == 0x16C);
	static_assert(sizeof(CharactersParameter) == 0x0314);
	static_assert(sizeof(PlayerDriveComponent) == 0x254);
	static_assert(sizeof(EngineBase) == 0xE10);
#pragma pack(pop)
}