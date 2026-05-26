UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 7868 AND `id` = 28132;
UPDATE `creature_template_addon` SET `path_id` = 78680 WHERE `entry` = 28132;
DELETE FROM `waypoint_data` WHERE `id` = 78680;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(78680, 1, 2244.68, 764.539, 52.5095, 0, 0, 0, 0, 100, 0),
(78680, 2, 2236.47, 753.143, 50.393, 0, 0, 0, 0, 100, 0),
(78680, 3, 2225.68, 739.29, 46.6969, 0, 0, 0, 0, 100, 0),
(78680, 4, 2211.67, 721.294, 42.1334, 0, 0, 0, 0, 100, 0),
(78680, 5, 2198.72, 704.657, 38.011, 0, 0, 0, 0, 100, 0),
(78680, 6, 2190.52, 694.089, 35.7325, 0, 0, 0, 0, 100, 0),
(78680, 7, 2183.08, 674.703, 34.0593, 0, 0, 0, 0, 100, 0),
(78680, 8, 2173.39, 632.888, 34.6719, 0, 0, 0, 0, 100, 0),
(78680, 9, 2168.3, 607.052, 39.6214, 0, 0, 0, 0, 100, 0),
(78680, 10, 2163.32, 581.173, 43.7444, 0, 0, 0, 0, 100, 0),
(78680, 11, 2159.62, 564, 47.4939, 0, 0, 0, 0, 100, 0),
(78680, 12, 2141.24, 542.692, 53.0619, 0, 0, 0, 0, 100, 0),
(78680, 13, 2124.71, 523.092, 56.7632, 0, 0, 0, 0, 100, 0),
(78680, 14, 2107.71, 502.939, 60.724, 0, 0, 0, 0, 100, 0),
(78680, 15, 2088.84, 477.598, 65.5944, 0, 0, 0, 0, 100, 0),
(78680, 16, 2079.38, 444.132, 66.2309, 0, 0, 0, 0, 100, 0),
(78680, 17, 2059.59, 399.192, 66.0609, 0, 0, 0, 0, 100, 0),
(78680, 18, 2001.85, 286.858, 66.2023, 0, 0, 0, 0, 100, 0),
(78680, 19, 2062.24, 395.618, 66.0537, 0, 0, 0, 0, 100, 0),
(78680, 20, 2085.58, 467.872, 65.9744, 0, 0, 0, 0, 100, 0),
(78680, 21, 2089.69, 478.01, 65.4738, 0, 0, 0, 0, 100, 0),
(78680, 22, 2101, 493.283, 62.5791, 0, 0, 0, 0, 100, 0),
(78680, 23, 2112.49, 510.128, 59.287, 0, 0, 0, 0, 100, 0),
(78680, 24, 2133.05, 530.736, 55.4164, 0, 0, 0, 0, 100, 0),
(78680, 25, 2147.58, 549.711, 51.1055, 0, 0, 0, 0, 100, 0),
(78680, 26, 2157.2, 562.289, 48.1764, 0, 0, 0, 0, 100, 0),
(78680, 27, 2162.51, 579.023, 44.2148, 0, 0, 0, 0, 100, 0),
(78680, 28, 2169.09, 604.56, 39.9684, 0, 0, 0, 0, 100, 0),
(78680, 29, 2175.59, 641.93, 33.7709, 0, 0, 0, 0, 100, 0),
(78680, 30, 2187.72, 685.245, 34.7087, 0, 0, 0, 0, 100, 0),
(78680, 31, 2203.61, 708.445, 39.2031, 0, 0, 0, 0, 100, 0),
(78680, 32, 2217.31, 728.868, 44.0861, 0, 0, 0, 0, 100, 0),
(78680, 33, 2231.65, 748.013, 48.9763, 0, 0, 0, 0, 100, 0),
(78680, 34, 2244.76, 765.873, 52.65, 0, 0, 0, 0, 100, 0),
(78680, 35, 2257.86, 783.715, 54.0561, 0, 0, 0, 0, 100, 0);

UPDATE `creature_template` SET `AIName` = "SmartAI" WHERE `entry` = 70005;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 70005 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(70005, 0, 0, 0, 0, 0, 100, 0, 4000, 6000, 14600, 18800, 11, 138765, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Young Primal Devilsaur - In Combat - Cast Spell 'Crush'");

DELETE FROM `smart_scripts` WHERE `entryorguid` = 70008 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(70008, 0, 0, 0, 0, 0, 100, 0, 8000, 8000, 15000, 25000, 11, 138766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Primal Devilsaur - In Combat - Cast Spell 'Piercing Roar'"),
(70008, 0, 1, 0, 2, 0, 100, 0, 0, 40, 22000, 25000, 11, 223971, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Primal Devilsaur - 0-40% HP - Cast Spell 'Hunter's Rush'");

DELETE FROM `smart_scripts` WHERE `entryorguid` = 69925 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(69925, 0, 0, 0, 2, 0, 100, 1, 0, 60, 0, 0, 11, 138771, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Zandalari Dinomancer - 0-60% HP - Cast Spell 'Dino Form' (No Repeat)"),
(69925, 0, 1, 0, 2, 0, 100, 0, 0, 40, 22000, 25000, 11, 138770, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Zandalari Dinomancer - 0-40% HP - Cast Spell 'Dino-Mending'");

DELETE FROM `creature` WHERE `guid` = 10574639 AND `id` = 69926;
UPDATE `creature_template` SET `unit_flags` = 256 WHERE `entry` = 69926;

DELETE FROM `creature_text` WHERE `CreatureID` IN (69926, 69161);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(69926, 0, 0, "How dare you interupt our preparations! The Zandalari will not be stopped! Not this time!", 14, 0, 100, 5, 0, 0, 0, 0, "Dohaman the Beast Lord"),
(69926, 1, 0, "Destroy them! I command you! No, stop, STOP!", 14, 0, 100, 22, 0, 0, 0, 0, "Dohaman the Beast Lord"),

(69161, 0, 0, "%s eats Dohaman.", 16, 0, 100, 0, 0, 0, 0, 0, "Oondasta");
