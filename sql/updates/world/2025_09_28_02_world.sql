update `creature_template` set `gossip_menu_id`=18778 where `entry`= 97359;

DELETE FROM `gossip_menu_option` WHERE `MenuID`=18778;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionType`, `OptionNpcflag`, `OptionBroadcastTextID`, `VerifiedBuild`) VALUES 
(18778, 0, 1, 'I would like to buy from you.', 3, 128, 14967, 26972),
(18778, 1, 0, 'Raethan, call off your guards.\n\n[Cost: 50 |TInterface\\Icons\\achievement_reputation_kirintor_offensive.blp:12|t Sightless Eyes]', 1, 1, 99085, 26972),
(18778, 2, 0, 'Raethan, call back your guards.\n\n[Cost: 50 |TInterface\\Icons\\achievement_reputation_kirintor_offensive.blp:12|t Sightless Eyes]', 1, 1, 99086, 26972),
(18778, 3, 0, 'I want to hire a personal bodyguard.', 1, 1, 105615, 26972);

DELETE FROM `gossip_menu_option_box` WHERE `MenuId`=18778 AND `OptionIndex`=3;
INSERT INTO `gossip_menu_option_box` (`MenuId`, `OptionIndex`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextId`) VALUES 
(18778, 3, 0, 50000, 'An Underbelly Guard will protect you from other players\' attacks for 5 minutes.\r\n\r\nDo you want to hire a bodyguard?', 105617);

DELETE FROM `creature_text` WHERE `CreatureID`=97359;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `TextRange`, `comment`) VALUES 
(97359, 0, 0, 'Sewer guards, time to take a break!', 41, 0, 100, 0, 0, 0, 99082, 0, 'Raethan to Player'),
(97359, 1, 0, 'Sewer guards! Return to your posts!', 41, 0, 100, 0, 0, 0, 99083, 0, 'Raethan to Player');

DELETE FROM `game_event` WHERE `eventEntry` IN (104,105,106,107,108,109,110,111,112);
INSERT INTO `game_event` (`eventEntry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`, `world_event`, `announce`) VALUES 
(104, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - PVP On', 0, 2),
(105, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 10, 2592000, 0, 'Dalaran_Underbelly - PVP Off', 0, 2),
(106, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Bats', 0, 2),
(107, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Demons', 0, 2),
(108, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Sister', 0, 2),
(109, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Crocodile', 0, 2),
(110, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Banquet', 0, 2),
(111, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Turtle', 0, 2),
(112, '2000-01-01 00:00:00', '2035-01-01 02:00:00', 2592000, 5, 0, 'Dalaran_Underbelly - Shimmering Whorl', 0, 2);

-- 103112
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 103112;

DELETE FROM `smart_scripts` WHERE `entryorguid`=103112 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(103112, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 11, 203894, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'on just spawned - cast spell 203894 on owner'),
(103112, 0, 1, 0, 60, 0, 100, 0, 300000, 300000, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'in 5 minutes - despawn');

UPDATE `creature_template_addon` SET `auras` = '203894' WHERE `entry` = 103112;

DELETE FROM `creature_equip_template` WHERE `CreatureID`=103112 AND `ID`=1;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES 
(103112, 1, 43618, 0, 0, 0, 0, 0, 0, 0, 0, 26972);

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_kloaka_call_some_adds';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(220021, 'spell_kloaka_call_some_adds'),
(220236, 'spell_kloaka_call_some_adds'),
(220237, 'spell_kloaka_call_some_adds'),
(220253, 'spell_kloaka_call_some_adds'),
(220260, 'spell_kloaka_call_some_adds'),
(220265, 'spell_kloaka_call_some_adds'),
(220266, 'spell_kloaka_call_some_adds');