UPDATE `creature_template` SET `ScriptName` = "" WHERE `ScriptName` IN ("npc_helarjar_champion", "npc_bane_skeleton", "npc_bane_sphere", "npc_meteor_gob", "npc_eagle_spirit", "
npc_gls_gob", "npc_altruis_sufferer_artifact", "npc_mardum_dh_learn_spec", "npc_helya_maw_destructor_tentacle", "npc_fusee_gob", "npc_galaw", "npc_tele_q42740", "
npc_korvas_bloodthorn_summon", "npc_chipie_quest_giver_end_event", "npc_girocoptere", "npc_gyrocoptere_quest_giver", "npc_seacursed_swiftblade", "npc_mardum_inquisitor_pernissius", "
npc_lianne_gobelin", "npc_mardum_tyranna", "npc_night_watch_mariner", "npc_waterlogged_soul_guard", "npc_phaseswift", "npc_seacursed_soulkeeper", "npc_shackled_servitor", "
npc_shroud_hound", "npc_skjal_maw", "npc_sumkaja_gob", "boss_ymiron_maw", "boss_harbaron_maw", "npc_seacursed_slaver");
UPDATE `creature` SET `ScriptName` = "" WHERE `ScriptName` = "0";
UPDATE `spell_script_names` SET `ScriptName` = "" WHERE `ScriptName` IN ("spell_kezan_despawn_sharks", "spell_hunt_artefact_wind", "spell_bank_67498", "
spell_dh_metamorphosis_aura", "spell_bank_67495", "spell_dru_rejuvenation", "spell_sha_hailstorm", "spell_68281", "spell_warr_ravager_damage", "spell_bank_67497", "
spell_dru_fury_of_elune", "spell_item_prydaz_abs", "spell_monk_mastery_combo_strikes_periodic_auras", "spell_monk_mastery_combo_strikes_periodic_triggers", "
spell_monk_mastery_combo_strikes", "spell_bank_67499", "spell_bank_67496", "spell_pri_sphere_of_insanity");
DELETE FROM `spell_script_names` WHERE `ScriptName` = "";
UPDATE `gameobject_template` SET `ScriptName` = "" WHERE `ScriptName` IN ("gob_fronde_gobelin", "go_q38727", "gob_bank_gobelin", "gob_dyn_gobelin");
UPDATE `instance_template` SET `script` = "" WHERE `script` IN ("instance_karazhan_legion", "instance_alliance_garrison_tier_2", "instance_arcway", "instance_isle_of_the_thunder_king_horde_boat", "
instance_alliance_garrison_tier_3", "instance_terrace_of_the_endless_spring", "instance_scarlet_halls", "instance_vault_of_the_wardens", "instance_nexus_tarecgosas_rest", "
instance_horde_garrison_tier_2", "instance_violet_hold_legion", "instance_upper_blackrock_spire_wod", "instance_horde_garrison_tier_3", "instance_trial_of_valor", "
instance_alliance_garrison_tier_1", "instance_antorus_the_burning_throne", "instance_anquiraj_event", "instance_auchindoun", "instance_azshara_highmountains_flashback", "
instance_blackrock_foundry", "instance_bloodmaul_slag_mines", "instance_broken_islands", "instance_grim_depot", "instance_hellfire_citadel", "instance_horde_garrison_tier_1", "
instance_iron_docks", "instance_isle_of_the_thunder_king_alliance_boat", "instance_scholomance_mop", "instance_skyreach", "instance_small_battleground", "instance_tanaan_jungle_6.2", "
instance_throne_of_thunder");
UPDATE `areatrigger_scripts` SET `ScriptName` = "" WHERE `ScriptName` IN ("areatrigger_dh_shattered_souls", "areatrigger_dh_shear", "areatrigger_dh_soul_fragment");
UPDATE `areatrigger_template` SET `ScriptName` = "" WHERE `ScriptName` IN ("areatrigger_hunter_windburst", "areatrigger_hunter_windburst_spawn");
