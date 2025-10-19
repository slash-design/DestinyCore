DELETE FROM game_event WHERE eventEntry in (113,114,115,116,117,118,119,120,121,122,123,124,125,126,127);
INSERT INTO `game_event` (`eventEntry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`, `world_event`) VALUES 
('113', '2017-03-13 00:00:00', '2035-01-01 02:00:00', '30240', '10080', '0', 'Invasion World Boss #1', '0'),
('114', '2017-03-20 00:00:37', '2035-01-01 02:00:00', '30240', '10080', '0', 'Invasion World Boss #2', '0'),
('115', '2017-03-27 00:00:00', '2035-01-01 02:00:00', '30240', '10080', '0', 'Invasion World Boss #3', '0'),
('116', '2018-08-06 00:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Cen\'gar (Krokun) 68-81', '0'),
('117', '2018-08-06 02:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Val (Antorian) 72-75', '0'),
('118', '2018-08-06 04:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Bonish (Mac´Aree) 60-19', '0'),
('119', '2018-08-06 06:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Aurinor (Antorian) 64-69', '0'),
('120', '2018-08-06 08:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion (Mac´Aree) 70-38', '0'),
('121', '2018-08-06 10:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Val (Krokun) 46-77', '0'),
('122', '2018-08-06 12:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Cen´gar (Antorian) 60-42', '0'),
('123', '2018-08-06 14:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Sangua (Krokun) 73-33', '0'),
('124', '2018-08-06 16:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Aurinor (Mac´Aree) 38-12', '0'),
('125', '2018-08-06 18:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Bonish (Antor/ian) 55-19', '0'),
('126', '2018-08-06 20:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Naigtal (Antorian) 55-29', '0'),
('127', '2018-08-06 22:59:20', '2035-01-01 02:00:00', '1440', '360', '0', 'Invasion Sangua (Antorian) 67-33', '0');

DELETE FROM creature_text WHERE `CreatureID` in (124719,124625,124492,124592,124555,124514,125785,125865,125939);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES
(124625, 0, 0, 'New playthings? How irresistible!', 14, 0, 100, 0, 0, 88349, 135976, 'Mistress Alluradel'),
(124625, 1, 0, '|TINTERFACE\ICONS\SPELL_FIRE_LAVASPAWN.BLP:20|t Mistress Alluradel begins to cast |cFFFF0404|Hspell:247551|h[Beguiling Charm]|h|r. Look away!', 41, 0, 100, 0, 0, 0, 133347, 'Mistress Alluradel'),
(124625, 2, 0, 'You left me... wanting...', 14, 0, 100, 0, 0, 88350, 135977, 'Mistress Alluradel'),
(124719, 0, 0, 'All worlds will burn in felfire!', 14, 0, 100, 0, 0, 88341, 135971, 'Pit Lord Vilemus'),
(124719, 1, 0, 'The flames... will... claim you...', 14, 0, 100, 0, 0, 88342, 135972, 'Pit Lord Vilemus'),
(125785, 0, 0, 'Я порублю тебя на куски!', 12, 0, 100, 0, 0, 55188, 94935, 'Felfrenzy Berserker'),
(125865, 0, 0, 'By the might of Sargeras!', 12, 0, 100, 0, 0, 56997, 109464, 'Shadowsworn Heretic'),
(125865, 1, 0, 'Your world will be purged!', 12, 0, 100, 0, 0, 56994, 96722, 'Shadowsworn Heretic'),
(125939, 0, 0, 'Entrap them, my children.', 12, 0, 100, 0, 0, 55024, 96303, 'Noxious Shadowstalker'),
(124492, 0, 0, 'I see the weakness in your soul!', 14, 0, 100, 0, 0, 88339, 135974, 'Occularus'),
(124492, 1, 0, 'My sight fails me...', 14, 0, 100, 0, 0, 88340, 135975, 'Occularus'),
(124592, 0, 0, 'Your fate is only death!', 14, 0, 100, 0, 0, 52400, 98125, 'Inquisitor Meto'),
(124592, 1, 0, 'You cannot escape our eyes...', 14, 0, 100, 0, 0, 52397, 98129, 'Inquisitor Meto'),
(124555, 0, 0, 'Come, small ones. Die by my hand!', 14, 0, 100, 0, 0, 88343, 135969, 'Sotanathor'),
(124555, 1, 0, '|TInterface\Icons\Spell_Shadow_SeedOfDestruction.blp:20|tYour |cFFFF0000|Hspell:181508|h[Seed of Destruction]|h|r will emit |cFFFF0000|Hspell:181498|h[Wakes of Destruction]|h|r.', 42, 0, 100, 0, 0, 0, 94037, 'Sotanathor'),
(124555, 2, 0, '|TInterface\Icons\warrior_talent_icon_mastercleaver.blp:20|tSotanathor\'s axe sends out |cFFFF0000|Hspell:181498|h[Wakes of Destruction]|h|r!', 41, 0, 100, 0, 0, 0, 137617, 'Sotanathor'),
(124555, 3, 0, 'Im... impossible...', 14, 0, 100, 0, 0, 88344, 135970, 'Sotanathor'),
(124514, 0, 0, 'Yes... come closer, little ones!', 14, 0, 100, 0, 0, 88347, 135978, 'Matron Folnuna'),
(124514, 1, 0, 'The master... will be... your end.', 14, 0, 100, 0, 0, 88348, 135979, 'Matron Folnuna');

 -- - Scripting Bosses
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_mistress_alluradel' WHERE (`entry`='124625');
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_pit_lor_vilemus' WHERE (`entry`='124719');
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_occularus' WHERE (`entry`='124492');
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_inquisitor_meto' WHERE (`entry`='124592');
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_sotanathor' WHERE (`entry`='124555');
UPDATE `creature_template` SET `mechanic_immune_mask`='617299967', `ScriptName`='boss_matron_folnuna' WHERE (`entry`='124514');
UPDATE `creature_template` SET `spell8`='254443', flags_extra=1536 WHERE (`entry`='124492');
UPDATE `creature_template` SET `spell8`='254441', flags_extra=1538 WHERE (`entry`='124514');
UPDATE `creature_template` SET `spell8`='254439', flags_extra=1536 WHERE (`entry`='124719');
UPDATE `creature_template` SET `spell8`='254446', flags_extra=1536 WHERE (`entry`='124555');
UPDATE `creature_template` SET `spell8`='254437', flags_extra=1536 WHERE (`entry`='124625');
UPDATE `creature_template` SET `spell8`='254443', flags_extra=1536 WHERE (`entry`='124592');
UPDATE `creature_template` SET `ScriptName`='npc_fiery_trickster' WHERE (`entry`='124551');
UPDATE `creature_template` SET `ScriptName`='npc_felblaze_maniacr' WHERE (`entry`='124552');
UPDATE `creature_template` SET `ScriptName`='npc_slumbering_gasp' WHERE (`entry`='124537');

DELETE FROM spell_script_names WHERE spell_id in (247549,247590,247739,247332,247410,247437,247362,247379,247443,247441,247069);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('247549', 'spell_beguiling_charm'),
('247590', 'spell_sadist'),
('247739', 'spell_drain'),
('247332', 'spell_eye_sore'),
('247410', 'spell_soul_cleave'),
('247437', 'spell_argus_seed_of_destruction'),
('247362', 'spell_infected_claws'),
('247379', 'spell_slimbering_gasp'),
('247441', 'spell_folnuna_nausea'),
('247069', 'spell_enter_rift');

DELETE FROM `graveyard_zone` WHERE `ID` in (6203,6205,6206,6207,6208,6209,6280,6278,6281,6283,6284,6286);
INSERT INTO `graveyard_zone` (`ID`, `GhostZone`, `Faction`) VALUES
('6203', '9126', '0'),
('6205', '9102', '0'),
('6206', '9128', '0'),
('6207', '9100', '0'),
('6208', '9127', '0'),
('6209', '9180', '0'),
('6280', '9298', '0'),
('6278', '9295', '0'),
('6281', '9299', '0'),
('6283', '9297', '0'),
('6284', '9300', '0'),
('6286', '9296', '0');

SET @CGUID := 280000971;

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+11;
INSERT INTO `creature` (`guid`,`id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID+0, '127536', '1669', '8899', '9149', '1', '0', '0', '0', '0', '-2694.84', '8982.61', '-137.702', '0.673246', '300', '0', '0', '1039267', '0', '0', '0', '0', '0'),
(@CGUID+1, '127533', '1669', '8899', '9155', '1', '0', '0', '0', '0', '-2915.19', '9377.31', '-161.265', '1.97546', '300', '0', '0', '87', '0', '0', '0', '0', '0'),
(@CGUID+2, '127535', '1669', '8899', '9152', '1', '0', '0', '0', '0', '-3220.1', '9105.6', '-164.47', '2.32103', '300', '0', '0', '87', '0', '0', '0', '0', '0'),
(@CGUID+3, '127528', '1669', '8574', '9038', '1', '0', '0', '0', '0', '2017.28', '1519.62', '408.587', '5.30823', '300', '0', '0', '87', '0', '0', '0', '0', '0'),
(@CGUID+4, '127532', '1669', '8701', '8701', '1', '0', '0', '0', '0', '5518.82', '10070.2', '-85.2857', '0.875101', '300', '0', '0', '1039267', '0', '0', '0', '0', '0'),
(@CGUID+5, '127531', '1669', '8899', '9161', '1', '0', '0', '0', '0', '-2473.9', '9172.28', '-158.575', '4.65445', '300', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+6, '124719', '1779', '9296', '9296', '1', '0', '0', '0', '0', '-9932.47', '-4639.48', '80.5446', '4.56194', '1', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+7, '124625', '1779', '9300', '9300', '1', '0', '0', '0', '0', '5341.94', '-9597.98', '3.35534', '4.64909', '1', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+8, '124492', '1779', '9298', '9298', '1', '0', '0', '0', '0', '-9889.12', '2704.07', '2.14682', '2.41421', '1', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+9, '124592', '1779', '9297', '9297', '1', '0', '0', '0', '0', '5667.17', '-1645.01', '3.0861', '2.20131', '1', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+10, '124555', '1779', '9299', '9299', '1', '0', '0', '0', '0', '-1361.62', '8165.9', '67.069', '1.90339', '1', '0', '0', '0', '0', '0', '0', '0', '0'),
(@CGUID+11, '124514', '1779', '9295', '9295', '1', '0', '0', '0', '0', '4398.7', '6490.09', '40.3561', '0.453682', '1', '0', '0', '0', '0', '0', '0', '0', '0');

DELETE FROM `game_event_creature` WHERE `eventEntry` IN (113,114,115);
INSERT INTO `game_event_creature` (`eventEntry`, `guid`) VALUES
(113,@CGUID+3),
(113,@CGUID+4),
(113,@CGUID+10),
(113,@CGUID+11),
(114,@CGUID+0),
(114,@CGUID+2),
(114,@CGUID+7),
(114,@CGUID+9),
(115,@CGUID+8),
(115,@CGUID+6),
(115,@CGUID+1),
(115,@CGUID+5);

DELETE FROM creature_loot_template WHERE entry in (124719,124625,124492,124592,124555,124514);
INSERT INTO `creature_loot_template` (`entry`, `item`, `Chance`, `lootmode`, `groupid`, `MinCount`, `MaxCount`) VALUES
('124514', '152346', '0', '23', '0', '1', '1'),
('124514', '152053', '0', '23', '0', '1', '1'),
('124514', '152376', '0', '23', '0', '1', '1'),
('124514', '152369', '0', '23', '0', '1', '1'),
('124514', '152371', '0', '23', '0', '1', '1'),
('124514', '152354', '0', '23', '0', '1', '1'),
('124514', '152358', '0', '23', '0', '1', '1'),
('124514', '152356', '0', '23', '0', '1', '1'),
('124514', '152360', '0', '23', '0', '1', '1'),
('124514', '152372', '0', '23', '0', '1', '1'),
('124514', '152380', '0', '23', '0', '1', '1'),
('124514', '152349', '0', '23', '0', '1', '1'),
('124514', '152359', '0', '23', '0', '1', '1'),
('124514', '-1508', '100', '0', '0', '100', '100'),
('124514', '151568', '100', '0', '0', '5', '5'),
('124514', '153014', '-100', '0', '0', '20', '20'),
('124555', '152348', '0', '23', '0', '1', '1'),
('124555', '152292', '0', '23', '0', '1', '1'),
('124555', '152361', '0', '23', '0', '1', '1'),
('124555', '152379', '0', '23', '0', '1', '1'),
('124555', '152353', '0', '23', '0', '1', '1'),
('124555', '152366', '0', '23', '0', '1', '1'),
('124555', '152365', '0', '23', '0', '1', '1'),
('124555', '152350', '0', '23', '0', '1', '1'),
('124555', '152375', '0', '23', '0', '1', '1'),
('124555', '152368', '0', '23', '0', '1', '1'),
('124555', '152372', '0', '23', '0', '1', '1'),
('124555', '152377', '0', '23', '0', '1', '1'),
('124555', '152362', '0', '23', '0', '1', '1'),
('124555', '152352', '0', '23', '0', '1', '1'),
('124555', '-1508', '100', '0', '0', '100', '100'),
('124555', '151568', '100', '0', '0', '5', '5'),
('124555', '153014', '-100', '0', '0', '20', '20'),
('124592', '152290', '0', '23', '0', '1', '1'),
('124592', '152344', '0', '23', '0', '1', '1'),
('124592', '152361', '0', '23', '0', '1', '1'),
('124592', '152371', '0', '23', '0', '1', '1'),
('124592', '152378', '0', '23', '0', '1', '1'),
('124592', '152353', '0', '23', '0', '1', '1'),
('124592', '152381', '0', '23', '0', '1', '1'),
('124592', '152350', '0', '23', '0', '1', '1'),
('124592', '152368', '0', '23', '0', '1', '1'),
('124592', '152380', '0', '23', '0', '1', '1'),
('124592', '152364', '0', '23', '0', '1', '1'),
('124592', '152370', '0', '23', '0', '1', '1'),
('124592', '152349', '0', '23', '0', '1', '1'),
('124592', '152359', '0', '23', '0', '1', '1'),
('124592', '-1508', '100', '0', '0', '100', '100'),
('124592', '151568', '100', '0', '0', '5', '5'),
('124592', '153014', '-100', '0', '0', '20', '20'),
('124492', '152347', '0', '23', '0', '1', '1'),
('124492', '152351', '0', '23', '0', '1', '1'),
('124492', '152369', '0', '23', '0', '1', '1'),
('124492', '152354', '0', '23', '0', '1', '1'),
('124492', '152358', '0', '23', '0', '1', '1'),
('124492', '152373', '0', '23', '0', '1', '1'),
('124492', '152381', '0', '23', '0', '1', '1'),
('124492', '152360', '0', '23', '0', '1', '1'),
('124492', '152355', '0', '23', '0', '1', '1'),
('124492', '152377', '0', '23', '0', '1', '1'),
('124492', '152367', '0', '23', '0', '1', '1'),
('124492', '152374', '0', '23', '0', '1', '1'),
('124492', '-1508', '100', '0', '0', '100', '100'),
('124492', '151568', '100', '0', '0', '5', '5'),
('124492', '153014', '-100', '0', '0', '20', '20'),
('124719', '152345', '0', '23', '0', '1', '1'),
('124719', '152376', '0', '23', '0', '1', '1'),
('124719', '152363', '0', '23', '0', '1', '1'),
('124719', '152378', '0', '23', '0', '1', '1'),
('124719', '152356', '0', '23', '0', '1', '1'),
('124719', '152365', '0', '23', '0', '1', '1'),
('124719', '152360', '0', '23', '0', '1', '1'),
('124719', '152368', '0', '23', '0', '1', '1'),
('124719', '152355', '0', '23', '0', '1', '1'),
('124719', '152370', '0', '23', '0', '1', '1'),
('124719', '152352', '0', '23', '0', '1', '1'),
('124719', '152367', '0', '23', '0', '1', '1'),
('124719', '152374', '0', '23', '0', '1', '1'),
('124719', '-1508', '100', '0', '0', '100', '100'),
('124719', '151568', '100', '0', '0', '5', '5'),
('124719', '153014', '-100', '0', '0', '20', '20'),
('124625', '152294', '0', '23', '0', '1', '1'),
('124625', '152030', '0', '23', '0', '1', '1'),
('124625', '152351', '0', '23', '0', '1', '1'),
('124625', '152379', '0', '23', '0', '1', '1'),
('124625', '152354', '0', '23', '0', '1', '1'),
('124625', '152363', '0', '23', '0', '1', '1'),
('124625', '152378', '0', '23', '0', '1', '1'),
('124625', '152366', '0', '23', '0', '1', '1'),
('124625', '152373', '0', '23', '0', '1', '1'),
('124625', '152372', '0', '23', '0', '1', '1'),
('124625', '152364', '0', '23', '0', '1', '1'),
('124625', '152362', '0', '23', '0', '1', '1'),
('124625', '152374', '0', '23', '0', '1', '1'),
('124625', '152349', '0', '23', '0', '1', '1'),
('124625', '-1508', '100', '0', '0', '100', '100'),
('124625', '151568', '100', '0', '0', '5', '5'),
('124625', '153014', '-100', '0', '0', '20', '20');

DELETE FROM `quest_template` WHERE `ID` IN (49171,49166,49168,49169,49167,49170);
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MaxScalingLevel`, `QuestPackageID`, `MinLevel`, `QuestSortID`, `QuestInfoID`, `SuggestedGroupNum`, `RewardNextQuest`, `RewardXPDifficulty`, `RewardXPMultiplier`, `RewardMoney`, `RewardMoneyDifficulty`, `RewardMoneyMultiplier`, `RewardBonusMoney`, `RewardDisplaySpell1`, `RewardDisplaySpell2`, `RewardDisplaySpell3`, `RewardSpell`, `RewardHonor`, `RewardKillHonor`, `StartItem`, `RewardArtifactXPDifficulty`, `RewardArtifactXPMultiplier`, `RewardArtifactCategoryID`, `Flags`, `FlagsEx`, `RewardItem1`, `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `RewardItem3`, `RewardAmount3`, `RewardItem4`, `RewardAmount4`, `ItemDrop1`, `ItemDropQuantity1`, `ItemDrop2`, `ItemDropQuantity2`, `ItemDrop3`, `ItemDropQuantity3`, `ItemDrop4`, `ItemDropQuantity4`, `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemDisplayID1`, `RewardChoiceItemID2`, `RewardChoiceItemQuantity2`, `RewardChoiceItemDisplayID2`, `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`, `RewardChoiceItemDisplayID3`, `RewardChoiceItemID4`, `RewardChoiceItemQuantity4`, `RewardChoiceItemDisplayID4`, `RewardChoiceItemID5`, `RewardChoiceItemQuantity5`, `RewardChoiceItemDisplayID5`, `RewardChoiceItemID6`, `RewardChoiceItemQuantity6`, `RewardChoiceItemDisplayID6`, `POIContinent`, `POIx`, `POIy`, `POIPriority`, `RewardTitle`, `RewardArenaPoints`, `RewardSkillLineID`, `RewardNumSkillUps`, `PortraitGiver`, `PortraitTurnIn`, `RewardFactionID1`, `RewardFactionValue1`, `RewardFactionOverride1`, `RewardFactionCapIn1`, `RewardFactionID2`, `RewardFactionValue2`, `RewardFactionOverride2`, `RewardFactionCapIn2`, `RewardFactionID3`, `RewardFactionValue3`, `RewardFactionOverride3`, `RewardFactionCapIn3`, `RewardFactionID4`, `RewardFactionValue4`, `RewardFactionOverride4`, `RewardFactionCapIn4`, `RewardFactionID5`, `RewardFactionValue5`, `RewardFactionOverride5`, `RewardFactionCapIn5`, `RewardFactionFlags`, `RewardCurrencyID1`, `RewardCurrencyQty1`, `RewardCurrencyID2`, `RewardCurrencyQty2`, `RewardCurrencyID3`, `RewardCurrencyQty3`, `RewardCurrencyID4`, `RewardCurrencyQty4`, `AcceptedSoundKitID`, `CompleteSoundKitID`, `AreaGroupID`, `TimeAllowed`, `AllowableRaces`, `QuestRewardID`, `Expansion`, `LogTitle`, `LogDescription`, `QuestDescription`, `AreaDescription`, `PortraitGiverText`, `PortraitGiverName`, `PortraitTurnInText`, `PortraitTurnInName`, `QuestCompletionLog`, `VerifiedBuild`) VALUES
(49171, 3, 110, 255, 0, 110, 9295, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972),
(49166, 3, 110, 255, 0, 110, 9297, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972),
(49168, 3, 110, 255, 0, 110, 9296, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972),
(49169, 3, 110, 255, 0, 110, 9299, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972),
(49167, 3, 110, 255, 0, 110, 9300, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972),
(49170, 3, 110, 255, 0, 110, 9298, 150, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 33555200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0, 0, 18446744073709551615, 0, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 26972);
