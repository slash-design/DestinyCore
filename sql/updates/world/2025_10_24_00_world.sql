-- creature_template
UPDATE `creature_template` SET `ScriptName`='boss_lei_shi' WHERE `entry`=62983;
UPDATE `creature_template` SET `ScriptName`='mob_animated_protector' WHERE `entry`=62995;
UPDATE `creature_template` SET `ScriptName`='mob_lei_shi_hidden' WHERE `entry`=63099;
UPDATE `creature_template` SET `ScriptName`='npc_lei_shi_reflection' WHERE `entry`=71095;
UPDATE `creature_template` SET `ScriptName`='npc_lei_shi_reflection' WHERE `entry`=71095;
UPDATE `creature_template` SET `ScriptName`='boss_ancient_regail' WHERE `entry` = 60585;
UPDATE `creature_template` SET `ScriptName`='boss_ancient_asani' WHERE `entry` = 60586;
UPDATE `creature_template` SET `ScriptName`='boss_protector_kaolan' WHERE `entry` = 60583;
UPDATE `creature_template` SET `ScriptName`='mob_defiled_ground' WHERE `entry` = 60906;
UPDATE `creature_template` SET `ScriptName`='mob_coalesced_corruption' WHERE `entry` = 60886;
UPDATE `creature_template` SET `ScriptName`='mob_cleansing_water' WHERE `entry` = 60646;
UPDATE `creature_template` SET `ScriptName`='mob_corrupting_waters' WHERE `entry` = 60621;
UPDATE `creature_template` SET `ScriptName`='mob_minion_of_fear' WHERE entry = 60885;
UPDATE `creature_template` SET `ScriptName`='mob_minion_of_fear_controller' WHERE entry = 60957;
UPDATE `creature_template` SET `ScriptName`='boss_sha_of_fear' WHERE entry = 60999;
UPDATE `creature_template` SET `ScriptName`='mob_pure_light_terrace' WHERE `entry` = 60788;
UPDATE `creature_template` SET `ScriptName`='mob_terror_spawn' WHERE `entry` = 61034;
UPDATE `creature_template` SET `ScriptName`='npc_sha_of_fear_bowman' WHERE `entry`=61042;
UPDATE `creature_template` SET `ScriptName`='npc_sha_of_fear_bowman' WHERE `entry`=61038;
UPDATE `creature_template` SET `ScriptName`='npc_sha_of_fear_bowman' WHERE `entry`=61046;
UPDATE `creature_template` SET `ScriptName`='npc_sha_globe' WHERE `entry`=65691;
UPDATE `creature_template` SET `ScriptName`='npc_waterspout' WHERE `entry`=48571;
UPDATE `creature_template` SET `ScriptName`='npc_dread_spawn' WHERE `entry`=61003;
UPDATE `creature_template` SET `ScriptName`='npc_return_to_terrace' WHERE `entry`=61741;
UPDATE `creature_template` SET `ScriptName`='boss_tsulong' WHERE entry = 62442;
UPDATE `creature_template` SET `ScriptName`='npc_sunbeam' WHERE entry = 62849;
UPDATE `creature_template` SET `ScriptName`='npc_embodied_terror' WHERE `entry` = 62969;
UPDATE `creature_template` SET `ScriptName`='npc_fright_spawn' WHERE `entry`=62977;
UPDATE `creature_template` SET `ScriptName` = 'npc_unstable_sha' WHERE `entry` = 62919;
UPDATE `creature_template` SET `modelid1`=42973, `modelid2`= 2973, `ScriptName`='npc_dark_of_night' WHERE `entry`=63346;
UPDATE `creature_template` SET `ScriptName`='npc_the_light_of_day' WHERE `entry`=63337;
UPDATE `creature_template` SET `ScriptName`='npc_terrace_springtender_ashani' WHERE `entry`=64846;
UPDATE `creature_template` SET `ScriptName`='npc_apparition_of_fear' WHERE `entry`=64368;
UPDATE `creature_template` SET `ScriptName`='npc_apparition_of_terror' WHERE `entry`=66100;
UPDATE `creature_template` SET `ScriptName`='npc_night_terrors' WHERE `entry`=64390;
UPDATE `creature_template` SET `ScriptName`='npc_night_terror_summon' WHERE `entry`=64393;
UPDATE `creature_template` SET `ScriptName`='npc_night_terror_summon' WHERE `entry`=64393;

-- spell_script_names
DELETE FROM `spell_script_names` WHERE `spell_id` IN (123461,123467,123233,123244,123705,117988,117955,118040,118053,118054,118055,118064,118077,118004,118005,118007,118008,111850,118191,117905,117283,117866,119414,119108,119693,119692,119593,120047,125786,119887,120458,120285,129147,120519,118977,136584,125843,122768,122789,122438,122453,123018,123011,122775,123816,122881,125736,125745);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(123461,'spell_toes_get_away'),
(123467,'spell_toes_get_away_dmg'),
(123233,'spell_toes_hide_stacks'),
(123244,'spell_toes_hide'),
(123705,'spell_toes_scary_fog_dot'),
(117988,'spell_toes_defiled_ground_damage'),
(117955,'spell_toes_expelled_corruption'),
(118040,'spell_toes_lightning_storm_aura'),
(118053,'spell_toes_lightning_storm_aura'),
(118054,'spell_toes_lightning_storm_aura'),
(118055,'spell_toes_lightning_storm_aura'),
(118064,'spell_toes_lightning_storm_aura'),
(118077,'spell_toes_lightning_storm_aura'),
(118004,'spell_toes_lightning_storm_damage'),
(118005,'spell_toes_lightning_storm_damage'),
(118007,'spell_toes_lightning_storm_damage'),
(118008,'spell_toes_lightning_storm_damage'),
(111850,'spell_toes_lightning_prison'),
(118191,'spell_toes_corrupted_essence'),
(117905,'spell_toes_superior_corrupted_essence'),
(117283,'spell_toes_cleansing_waters_regen'),
(117866,'spell_toes_champion_of_light'),
(119414,'spell_toes_breath_of_fear'),
(119108,'spell_toes_conjure_terror_spawn'),
(119693,'spell_toes_ominous_caclke_target'),
(119692,'spell_toes_ominous_caclke_target'),
(119593,'spell_toes_ominous_caclke_target'),
(120047,'spell_toes_dread_spray'),
(125786,'spell_toes_breath_of_fear_fear'),
(119887,'spell_toes_death_blossom'),
(120458,'spell_emerge'),
(120285,'spell_transfer_light'),
(129147,'spell_toes_ominous_cackle_immunities'),
(120519,'spell_toes_waterspout_periodic'),
(118977,'spell_toes_fearless'),
(136584,'spell_toes_fearless_pet'),
(125843,'spell_toes_dread_shadows_damage'),
(122768,'spell_toes_dread_shadows_malus'),
(122789,'spell_toes_sunbeam'),
(122438,'spell_toes_tsulong_sha_regen'),
(122453,'spell_toes_tsulong_sha_regen'),
(123018,'spell_toes_terrorize_periodic_player'),
(123011,'spell_toes_tsulong_terrorize'),
(122775,'spell_toes_tsulong_nightmares'),
(123816,'spell_the_light_of_day'),
(122881,'spell_unstable_bolt_selfdamage'),
(125736,'spell_night_terrors_missile'),
(125745,'spell_night_terrors_periodic');

-- instance_template
UPDATE `instance_template` SET `script`='instance_terrace_of_endless_spring' WHERE `map`=996;