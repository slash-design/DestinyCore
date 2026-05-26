-- Eastern Kingdoms
-- Zone: Elwynn Forest
DELETE FROM gameobject WHERE guid = 184384 AND id = 142075;
UPDATE creature_addon SET path_id = 1802240, emote = 0 WHERE guid = 180224;
UPDATE creature SET position_x = -9462.29, position_y = 87.8151, position_z = 58.4211, orientation = 0.733038, spawndist = 0, MovementType = 2 WHERE guid = 180224;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 2046;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2046 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2046*100, 2046*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(2046, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 2046*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 1 Reached - Start Script"),
(2046, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 2046*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 3 Reached - Start Script"),
(2046*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2.37365, "Andrew Krighton - On Script - Set Orientation (2.37365)"),
(2046*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (233)"),
(2046*100, 9, 2, 0, 0, 0, 100, 0, 17000, 17000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (0)"),
(2046*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.733038, "Andrew Krighton - On Script - Set Orientation (0.733038)");
DELETE FROM waypoint_data WHERE id = 1802240;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1802240, 1, -9464.05957, 88.933998, 58.3437, 0),
(1802240, 2, -9464.05957, 88.933998, 58.3437, 20000),
(1802240, 3, -9462.29, 87.8151, 58.4211, 0),
(1802240, 4, -9462.29, 87.8151, 58.4211, 60000);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 177313 AND id = 46982;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274684, 274851, 274366, 274636, 274555, 274809, 274883, 274849, 274686, 274374, 274884, 274846, 274691, 274378, 274377);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274770, 274431, 274816, 274815, 274808, 274381, 274380, 274383);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274735, 274369, 274848, 274364, 274885, 274631, 274553, 274633, 274632, 274688, 274733, 274556);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274635, 274685, 274728, 274813, 274371, 274807, 274806, 274774, 274845);
DELETE FROM creature WHERE guid = 318123 AND id = 20075;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274692, 274850, 274768, 274557, 274854, 274637, 274847, 274810, 274730, 274689);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274551, 274771, 274769, 274729, 274552, 274772, 274773, 274844, 274687);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274640, 274367, 274736, 274554, 274690, 274365, 274638, 274811, 274732, 274370, 274734, 274775, 274778, 274639, 274812);
UPDATE creature_addon SET path_id = 1776330, emote = 0 WHERE guid = 177633;
UPDATE creature SET position_x = -9464.87, position_y = 93.9896, position_z = 58.5271, orientation = 0, spawndist = 0, MovementType = 2 WHERE guid = 177633;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 54;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 54 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (54*100, 54*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(54, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 54*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 1 Reached - Start Script"),
(54, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 54*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 3 Reached - Start Script"),
(54*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.811598, "Corina Steele - On Script - Set Orientation (0.811598)"),
(54*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (233)"),
(54*100, 9, 2, 0, 0, 0, 100, 0, 27000, 27000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 3.124597, "Corina Steele - On Script - Set Orientation (3.124597)"),
(54*100+1, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 69, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (69)"),
(54*100+1, 9, 2, 0, 0, 0, 100, 0, 16000, 16000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.00000, "Corina Steele - On Script - Set Orientation (0.00000)");
DELETE FROM waypoint_data WHERE id = 1776330;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1776330, 1, -9460.31543, 101.147774, 58.344334, 0),
(1776330, 2, -9460.31543, 101.147774, 58.344334, 30000),
(1776330, 3, -9464.87, 93.9896, 58.5271, 0),
(1776330, 4, -9464.87, 93.9896, 58.5271, 80000);
DELETE FROM `waypoints` WHERE `entry` = 3848;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(3848, 1, 2954.25,-3215.41,169.205, 'Kayneth Stillwind '),
(3848, 2, 2966.65,-3213.95,168.914, 'Kayneth Stillwind '),
(3848, 3, 2954.23,-3215.45,169.206, 'Kayneth Stillwind ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3848 AND `source_type` = 0 AND id >= 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3848*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3848,0,1,0,25,0,100,0,0,0,0,0,53,0,3848,1,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Reset - Start Waypoint"),
(3848,0,2,0,40,0,100,0,1,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 1 Reached - Pause Waypoint"),
(3848,0,3,4,40,0,100,0,2,3848,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 2 Reached - Pause Waypoint"),
(3848,0,4,0,61,0,100,0,2,3848,0,0,80,3848*100,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 2 Reached - Start Script"),
(3848,0,5,0,40,0,100,0,3,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 3 Reached - Pause Waypoint"),
(3848*100,9,0,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.965562,"Kayneth Stillwind - On Script - Set Orientation 0.965562"),
(3848*100,9,1,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 69"),
(3848*100,9,2,0,0,0,100,0,3000,3000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 0"),
(3848*100,9,3,0,0,0,100,0,3000,3000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Say Line 0");
DELETE FROM `creature_text` WHERE `CreatureID` = 3848;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES 
(3848, 0, 0, 'Putting the bottles away, %s sighs.', 16, 0, 100, 0, 0, 0, 'Kayneth Stillwind', 14108);
UPDATE creature SET position_x = 905.604, position_y = -403.547, position_z = -48.7295, spawndist = 0, MovementType = 2 WHERE guid = 321579 AND id = 9056;
DELETE FROM creature_addon WHERE guid = 321579;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(321579, 3215790, 1);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 9056 AND `source_type` = 0 AND `id` >= 5;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (9056*100, 9056*100+1, 9056*100+2) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(9056,0,5,0,34,0,100,0,2,11,0,0,80,9056*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 11 Reached - Start Script"),
(9056,0,6,0,34,0,100,0,2,18,0,0,80,9056*100+1,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 18 Reached - Start Script"),
(9056,0,7,0,34,0,100,0,2,23,0,0,80,9056*100+2,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 23 Reached - Start Script"),
(9056*100,9,0,0,0,0,100,0,3000,3000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (133)"),
(9056*100,9,1,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (0)"),
(9056*100+1,9,0,0,0,0,100,0,3000,3000,0,0,17,173,0,0,0,0,0,0,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (173)"),
(9056*100+1,9,1,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (0)"),
(9056*100+2,9,0,0,0,0,100,0,1000,1000,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Unit Field Bytes 1 (8)"),
(9056*100+2,9,1,0,0,0,100,0,4000,4000,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Unit Field Bytes 1 (0)");
DELETE FROM `waypoint_data` WHERE `id` = 3215790;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES 
(3215790, 1, 975.107, -354.152, -69.1219, 0, 0),
(3215790, 2, 984.444, -363.944, -65.9066, 0, 0),
(3215790, 3, 984.38, -372.827, -66.4086, 0, 0),
(3215790, 4, 976.479, -381.185, -63.9267, 0, 0),
(3215790, 5, 962.877, -395.447, -60.8377, 0, 0),
(3215790, 6, 950.417, -408.589, -57.1351, 0, 0),
(3215790, 7, 941.736, -417.331, -55.0396, 0, 0),
(3215790, 8, 931.446, -413.318, -55.3833, 0, 0),
(3215790, 9, 923.409, -403.986, -51.104, 0, 0),
(3215790, 10, 914.499, -394.179, -49.4412, 0, 0),
(3215790, 11, 905.604, -403.547, -48.7295, 0, 0),
(3215790, 12, 905.604, -403.547, -48.7295, 0, 15000),
(3215790, 13, 912.664, -392.947, -49.2744, 0, 0),
(3215790, 14, 918.441, -398, -49.6367, 0, 0),
(3215790, 15, 926.414, -407.296, -52.6216, 0, 0),
(3215790, 16, 930.038, -418.35, -55.3877, 0, 0),
(3215790, 17, 930.151, -424.848, -55.8645, 0, 0),
(3215790, 18, 927.594, -433.715, -56.5236, 0, 0),
(3215790, 19, 927.594, -433.715, -56.5236, 0, 15000),
(3215790, 20, 936.123, -427.94, -56.1072, 0, 0),
(3215790, 21, 939.641, -426.605, -55.7614, 0, 0),
(3215790, 22, 944.406, -426.659, -54.9984, 0, 0),
(3215790, 23, 946.326, -428.053, -54.6023, 0, 0),
(3215790, 24, 946.326, -428.053, -54.6023, 0, 8000),
(3215790, 25, 943.776, -418.105, -54.9468, 0, 0),
(3215790, 26, 947.044, -410.439, -55.9664, 0, 0),
(3215790, 27, 955.107, -403.439, -59.5107, 0, 0),
(3215790, 28, 961.657, -396.63, -60.8377, 0, 0),
(3215790, 29, 968.284, -389.058, -60.8377, 0, 0),
(3215790, 30, 978.015, -379.612, -64.6384, 0, 0),
(3215790, 31, 986.547, -370.816, -66.5624, 0, 0),
(3215790, 32, 974.031, -355.625, -69.1521, 0, 0),
(3215790, 33, 967.597, -349.477, -71.3905, 0, 0),
(3215790, 34, 963.267, -343.735, -71.7394, 0, 0);
UPDATE creature_template_addon SET emote = 214 WHERE entry = 46983;
DELETE FROM creature WHERE guid = 10297;
INSERT INTO creature (guid, id, map, zoneId, areaId, position_x, position_y, position_z, orientation) VALUES
(10297, 46985, 0, 12, 87, -9462.1, 126.892, 58.962, 4.66003);
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 46983;
DELETE FROM smart_scripts WHERE entryorguid = 46983;
INSERT INTO smart_scripts (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46983, 0, 0, 0, 1, 0, 100, 0, 1, 1, 10000, 10000, 11, 45425, 0, 0, 0, 0, 0, 10, 10297, 0, 0, 0, 0, 0, 0, "Benjamin Foxworthy - OOC - Cast Spell 'Shot'");
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (179030, 178999, 179035, 179049, 179051, 179047);
DELETE FROM `creature_text` WHERE `CreatureID` IN (49869, 951);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(49869, 0, 0, "HELP!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49897),
(49869, 0, 1, "I need a heal!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49898),
(49869, 0, 2, "I could use a heal, brother!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49895),
(49869, 0, 3, "Make yourself useful and heal me, Paxton!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49896),
(49869, 1, 0, "Come, monsters! We will crush you!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49804),
(49869, 1, 1, "I'll kill a hundred more of your battle worgs!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49807),
(49869, 1, 2, "Look alive, men! There are orcs and worgs about!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49809),
(49869, 1, 3, "Your filthy dogs won't be enough!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49806),
(49869, 1, 4, "Your worgs are no match for the might of Stormwind!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49808),
(951, 0, 0, 'BY THE LIGHT BE RENEWED!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49892),
(951, 0, 1, 'AND I LAY MY HANDS UPON YOU!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49890),
(951, 0, 2, 'Let the Holy Light embrace you!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49891),
(951, 1, 0, 'Be healed, $g brother:sister;!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49889),
(951, 1, 1, 'FIGHT ON, $G BROTHER:SISTER;!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49893);
DELETE FROM `waypoint_data` WHERE `id` = 951*100;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES
(951*100, 1, -8833.074, -152.085, 80.349, 0),
(951*100, 2, -8830.21, -146.632, 80.5787, 0),
(951*100, 3, -8826.04, -146.105, 80.6188, 0),
(951*100, 4, -8822.08, -146.456, 80.877, 0),
(951*100, 5, -8817.2, -150.924, 81.5878, 0),
(951*100, 6, -8813.55, -153.822, 82.0283, 8000),
(951*100, 7, -8811.99, -159.78, 81.614, 0),
(951*100, 8, -8810.78, -164.51, 81.306, 0),
(951*100, 9, -8813.96, -163.8, 81.108, 0),
(951*100, 10, -8821.86, -155.42, 80.585, 0),
(951*100, 11, -8833.074, -152.085, 80.349, 0),
(951*100, 12, -8830.21, -146.632, 80.5787, 0),
(951*100, 13, -8826.04, -146.105, 80.6188, 0),
(951*100, 14, -8822.08, -146.456, 80.877, 8000),
(951*100, 15, -8817.2, -150.924, 81.5878, 0),
(951*100, 16, -8813.55, -153.822, 82.0283, 0),
(951*100, 17, -8811.99, -159.78, 81.614, 0),
(951*100, 18, -8810.78, -164.51, 81.306, 0),
(951*100, 19, -8813.96, -163.8, 81.108, 0),
(951*100, 20, -8821.86, -155.42, 80.585, 0),
(951*100, 21, -8833.074, -152.085, 80.349, 8000),
(951*100, 22, -8830.21, -146.632, 80.5787, 0),
(951*100, 23, -8826.04, -146.105, 80.6188, 0),
(951*100, 24, -8822.08, -146.456, 80.877, 0),
(951*100, 25, -8817.2, -150.924, 81.5878, 0),
(951*100, 26, -8813.55, -153.822, 82.0283, 0),
(951*100, 27, -8811.99, -159.78, 81.614, 0),
(951*100, 28, -8810.78, -164.51, 81.306, 0),
(951*100, 29, -8813.96, -163.8, 81.108, 0),
(951*100, 30, -8821.86, -155.42, 80.585, 0),
(951*100, 31, -8833.074, -152.085, 80.349, 8000),
(951*100, 32, -8830.21, -146.632, 80.5787, 0),
(951*100, 33, -8826.04, -146.105, 80.6188, 0),
(951*100, 34, -8822.08, -146.456, 80.877, 0),
(951*100, 35, -8817.2, -150.924, 81.5878, 0),
(951*100, 36, -8813.55, -153.822, 82.0283, 0),
(951*100, 37, -8811.99, -159.78, 81.614, 0),
(951*100, 38, -8810.78, -164.51, 81.306, 8000),
(951*100, 39, -8813.96, -163.8, 81.108, 0),
(951*100, 40, -8821.86, -155.42, 80.585, 0),
(951*100, 41, -8833.074, -152.085, 80.349, 0),
(951*100, 42, -8830.21, -146.632, 80.5787, 0),
(951*100, 43, -8826.04, -146.105, 80.6188, 0),
(951*100, 44, -8822.08, -146.456, 80.877, 0),
(951*100, 45, -8817.2, -150.924, 81.5878, 0),
(951*100, 46, -8813.55, -153.822, 82.0283, 0),
(951*100, 47, -8811.99, -159.78, 81.614, 0),
(951*100, 48, -8810.78, -164.51, 81.306, 8000),
(951*100, 49, -8813.96, -163.8, 81.108, 0),
(951*100, 50, -8821.86, -155.42, 80.585, 0),
(951*100, 51, -8833.074, -152.085, 80.349, 0),
(951*100, 52, -8830.21, -146.632, 80.5787, 0),
(951*100, 53, -8826.04, -146.105, 80.6188, 0),
(951*100, 54, -8822.08, -146.456, 80.877, 0),
(951*100, 55, -8817.2, -150.924, 81.5878, 0),
(951*100, 56, -8813.55, -153.822, 82.0283, 8000),
(951*100, 57, -8811.99, -159.78, 81.614, 0),
(951*100, 58, -8810.78, -164.51, 81.306, 8000),
(951*100, 59, -8813.96, -163.8, 81.108, 0),
(951*100, 60, -8821.86, -155.42, 80.585, 0);
UPDATE creature_template SET unit_flags = 2, flags_extra = 2, AIName = "", ScriptName = "npc_brother_paxton" WHERE entry = 951;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 951 AND `source_type` = 0;
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 177921 AND id = 951;
UPDATE creature_template_addon SET path_id = 951*100 WHERE entry = 951;
UPDATE creature_template_addon SET emote = 483 WHERE entry IN (197, 42216);
UPDATE creature SET modelid = 32727 WHERE guid = 177877 AND id = 42216;
UPDATE creature_template SET unit_flags2 = 0, AIName = "", ScriptName = "npc_stormwind_injured_soldier" WHERE entry = 50047;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50047 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50047*100 AND `source_type` = 9;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 50047 AND `SourceEntry` = 93072;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`Comment`) VALUES
(18,50047,93072,0,9,0,28812,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,1,9,0,28813,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,2,9,0,29082,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,3,9,0,28811,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,4,9,0,28810,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,5,9,0,28808,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,6,9,0,28809,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,7,9,0,28806,0,0,0,'Required quest active for spellclick');
DELETE FROM `creature_text` WHERE `CreatureID` = 50039;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(50039, 0, 0, "We'll kill anybody for the right price!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49837),
(50039, 0, 1, "Time to join your friends, kissin' the dirt!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49838),
(50039, 0, 2, "DIE!!!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49839),
(50039, 0, 3, "We're gonna burn this place to the ground!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49840);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE id = 49874;
UPDATE creature_template SET AIName = "", ScriptName = "" WHERE entry = 49874;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49874 AND `source_type` = 0;
DELETE FROM creature_template_addon WHERE entry = 49874;
UPDATE creature SET MovementType = 2 WHERE guid IN (178204, 178233, 178240) AND id = 49874;
UPDATE creature SET MovementType = 2, ScriptName = "npc_blackrock_spy" WHERE guid IN (178280, 178205, 178460, 178347, 178342, 178345, 178184, 178475, 178248) AND id = 49874;
UPDATE creature SET position_x = -8901.361, position_y = -42.065, position_z = 87.123, orientation = 5.2409 WHERE guid = 178205 AND id = 49874;
UPDATE creature SET position_x = -8826.83, position_y = -77.074, position_z = 86.163, orientation = 5.638 WHERE guid = 178347 AND id = 49874;
UPDATE creature SET position_x = -8837.45, position_y = -119.131, position_z = 80.527, orientation = 5.968 WHERE guid = 178342 AND id = 49874;
UPDATE creature SET position_x = -8822.1, position_y = -102.17, position_z = 84.22, orientation = 6.282 WHERE guid = 178345 AND id = 49874;
UPDATE creature SET position_x = -8980.23, position_y = -43.169, position_z = 90.891, orientation = 5.571 WHERE guid = 178484 AND id = 49874;
UPDATE creature SET position_x = -8986.609, position_y = -257.11, position_z = 72.776, orientation = 1.076 WHERE guid = 178432 AND id = 49874;
UPDATE creature SET position_x = -8965.578, position_y = -178.499, position_z = 81.464, orientation = 0.771 WHERE guid = 178249 AND id = 49874;
UPDATE creature SET position_x = -8930.5996, position_y = -246.447, position_z = 79.099, orientation = 1.738 WHERE guid = 178254 AND id = 49874;
DELETE FROM creature_addon WHERE guid IN (178280, 178205, 178204, 178460, 178347, 178242, 178342, 178345, 178341, 178238, 178233, 178184, 178484, 178250, 178475, 178240, 178432, 178249, 178248, 178254, 178340, 178271);
INSERT INTO creature_addon (guid, path_id, bytes1, bytes2, emote, auras) VALUES
(178280, 178280*10, 0, 1, 0, ""),
(178205, 178205*10, 0, 1, 0, ""),
(178204, 178204*10, 0, 1, 0, "92857"),
(178460, 178460*10, 0, 1, 0, ""),
(178347, 178347*10, 0, 1, 0, ""),
(178242, 0, 8, 1, 0, "80676"),
(178342, 178342*10, 0, 1, 0, ""),
(178345, 178345*10, 0, 1, 0, ""),
(178341, 0, 8, 1, 0, "80676"),
(178238, 0, 8, 1, 0, "80676"),
(178233, 178233*10, 0, 1, 0, "92857"),
(178184, 178184*10, 0, 1, 0, ""),
(178484, 0, 8, 1, 0, "80676"),
(178250, 0, 8, 1, 0, "80676"),
(178475, 178475*10, 0, 1, 0, ""),
(178240, 178240*10, 0, 1, 0, "92857"),
(178432, 0, 8, 1, 0, "80676"),
(178249, 0, 8, 1, 0, "80676"),
(178248, 178248*10, 0, 1, 0, ""),
(178254, 0, 8, 1, 0, "80676"),
(178340, 0, 8, 1, 0, "80676"),
(178271, 0, 8, 1, 0, "80676");
DELETE FROM `waypoint_data` WHERE `id` IN (178280*10, 178205*10, 178204*10, 178460*10, 178347*10, 178342*10, 178345*10, 178233*10, 178184*10, 178475*10, 178240*10, 178248*10);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(178280*10, 1, -8928.65, -68.88, 89.889, 0, 0),
(178280*10, 2, -8921.778, -72.851, 88.831, 5.255, 6000),
(178280*10, 3, -8928.65, -68.88, 89.889, 0, 0),
(178280*10, 4, -8936.066, -76.566, 88.553, 5.1663, 6000),
(178205*10, 1, -8901.361, -42.065, 87.123, 0, 0),
(178205*10, 2, -8897.179, -53.335, 87.453, 4.6251, 6000),
(178205*10, 3, -8901.361, -42.065, 87.123, 0, 0),
(178205*10, 4, -8920.372, -45.709, 89.271, 4.8129, 6000),
(178204*10, 1, -8924.495, -42.766, 90.559, 0, 0),
(178204*10, 2, -8937.65, -34.89, 93.718, 0, 0),
(178204*10, 3, -8948.479, -41.211, 91.784, 0, 0),
(178204*10, 4, -8943.493, -47.925, 91.344, 0, 0),
(178204*10, 5, -8916.122, -57.985, 87.113, 0, 0),
(178204*10, 6, -8900.264, -60.966, 86.637, 0, 0),
(178204*10, 7, -8892.944, -48.276, 87.02, 0, 0),
(178204*10, 8, -8906.961, -40.336, 88.76, 0, 0),
(178460*10, 1, -8871.169, -93.006, 82.75, 0, 0),
(178460*10, 2, -8869.837, -109.285, 81.046, 3.5944, 6000),
(178460*10, 3, -8871.169, -93.006, 82.75, 0, 0),
(178460*10, 4, -8883.453, -94.134, 84.15, 3.9439, 6000),
(178347*10, 1, -8826.83, -77.074, 86.163, 0, 0),
(178347*10, 2, -8831.75, -76.168, 85.896, 3.839, 6000),
(178347*10, 3, -8826.83, -77.074, 86.163, 0, 0),
(178347*10, 4, -8822.304, -87.685, 84.942, 3.796, 6000),
(178342*10, 1, -8837.45, -119.131, 80.527, 0, 0),
(178342*10, 2, -8832.41, -121.71, 80.772, 5.097, 6000),
(178342*10, 3, -8837.45, -119.131, 80.527, 0, 0),
(178342*10, 4, -8843.257, -120.911, 80.562, 3.474, 6000),
(178345*10, 1, -8822.1, -102.17, 84.22, 0, 0),
(178345*10, 2, -8816.05, -106.07, 83.22, 4.537, 6000),
(178345*10, 3, -8822.1, -102.17, 84.22, 0, 0),
(178345*10, 4, -8829.87, -104.307, 83.68, 4.827, 6000),
(178233*10, 1, -8852.61, -48.558, 85.448, 0, 0),
(178233*10, 2, -8862.82, -67.132, 85.453, 0, 0),
(178233*10, 3, -8866.67, -76.237, 83.728, 0, 0),
(178233*10, 4, -8850.55, -85.159, 83.776, 0, 0),
(178233*10, 5, -8823.33, -64.355, 88.549, 0, 0),
(178233*10, 6, -8823.47, -26.203, 89.645, 0, 0),
(178233*10, 7, -8834.408, -22.7703, 88.771, 0, 0),
(178233*10, 8, -8839.365, -29.207, 88.939, 0, 0),
(178184*10, 1, -8962.82, -61.281, 92.232, 0, 0),
(178184*10, 2, -8959.72, -65.52, 92.061, 5.257, 6000),
(178184*10, 3, -8962.82, -61.281, 92.232, 0, 0),
(178184*10, 4, -8970.68, -61.11, 91.473, 3.632, 6000),
(178475*10, 1, -9022.95, -180.919, 76.628, 0, 0),
(178475*10, 2, -9002.93, -187.57, 76.652, 0.916, 6000),
(178475*10, 3, -9022.95, -180.919, 76.628, 0, 0),
(178475*10, 4, -9018.98, -165.101, 79.339, 0.462, 6000),
(178240*10, 1, -9048.471, -173.768, 76.868, 0, 0),
(178240*10, 2, -9057.178, -184.681, 75.1504, 0, 0),
(178240*10, 3, -9049.224, -197.697, 72.661, 0, 0),
(178240*10, 4, -9011.597, -216.476, 71.143, 0, 0),
(178240*10, 5, -8987.513, -211.775, 73.592, 0, 0),
(178240*10, 6, -8996.908, -186.188, 76.552, 0, 0),
(178240*10, 7, -9015.125, -185.63, 77.044, 0, 0),
(178240*10, 8, -9021.097, -176.85, 76.993, 0, 0),
(178248*10, 1, -8975.58, -210.867, 74.394, 0, 0),
(178248*10, 2, -8972.08, -211.329, 74.651, 0.9808, 6000),
(178248*10, 3, -8975.58, -210.867, 74.394, 0, 0),
(178248*10, 4, -8984.532, -199.957, 74.765, 0.8002, 6000);
DELETE FROM `creature_text` WHERE `CreatureID` IN (49874, 42937);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(49874, 0, 0, "Orc KILL $r!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42876),
(49874, 0, 1, "Blackrock take forest!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42879),
(49874, 0, 2, "The grapes were VERY TASTY!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42880),
(49874, 0, 3, "Beg for life!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42877),
(49874, 0, 4, "Eat you!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42878),
(42937, 0, 0, "Orc KILL $r!", 12, 0, 50, 0, 0, 0, 'Blackrock Invader', 42876),
(42937, 0, 1, "Blackrock take forest!", 12, 0, 50, 0, 0, 0, 'Blackrock Invader', 42879),
(42937, 0, 2, "The grapes were VERY TASTY!", 12, 0, 50, 0, 0, 0, 'Blackrock Invader', 42880),
(42937, 0, 3, "Beg for life!", 12, 0, 50, 0, 0, 0, 'Blackrock Invader', 42877),
(42937, 0, 4, "Eat you!", 12, 0, 50, 0, 0, 0, 'Blackrock Invader', 42878);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 302867 AND id = 62;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42940 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(42940, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 11, 80175, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Northshire Vineyards Fire Trigger - Cast Aura 'Vineyard Fire'"),
(42940, 0, 1, 2, 8, 0, 100, 1, 80208, 0, 0, 0, 11, 80223, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Northshire Vineyards Fire Trigger - Cast Effect 'Steam' (No Repeat)"),
(42940, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 41, 1900, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Northshire Vineyards Fire Trigger - Forced Despawn (No Repeat)");
DELETE FROM `smart_scripts` WHERE `entryorguid` = 197 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(197, 0, 0, 2, 19, 0, 100, 0, 54, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Marshal McBride - On Quest 'Report to Goldshire' Taken - Say Line 0"),
(197, 0, 1, 0, 52, 0, 100, 0, 0, 197, 0, 0, 5, 66, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Marshal McBride - On Text Line 0 Over - Play Emote (66)"),
(197, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 5, 66, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Marshal McBride - On Quest 'Report to Goldshire' (Link) Taken - Play Emote (66)");
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 177848 AND id = 448;
DELETE FROM `creature_text` WHERE `CreatureID` IN (448, 46941);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(448, 0, 0, "More bones to gnaw on...", 12, 0, 100, 0, 0, 0, 1871, 0, 'Hogger'),
(448, 0, 1, "Grrrr... fresh meat!", 12, 0, 100, 0, 0, 0, 1870, 0, 'Hogger'),
(448, 1, 0, "Yipe!  Help Hogger!", 14, 0, 0, 0, 0, 0, 46936, 0, 'Hogger'),
(448, 2, 0, "No hurt Hogger!", 14, 0, 0, 0, 0, 0, 47000, 0, 'Hogger'),
(448, 3, 0, "Hogger is stunned!", 41, 0, 0, 0, 0, 0, 46940, 0, 'Hogger'),
(448, 4, 0, "Hogger is eating!  Stop him!", 41, 0, 0, 0, 0, 0, 46939, 0, 'Hogger'),
(448, 5, 0, "Grrr...", 12, 0, 0, 0, 0, 0, 46937, 0, 'Hogger'),
(448, 6, 0, "Nooooo...", 12, 0, 0, 0, 0, 0, 46938, 0, 'Hogger'),
(46941, 0, 0, "Right away, General!", 12, 0, 0, 0, 0, 0, 147279, 0, 'High Sorcerer Andromath');
UPDATE creature_template SET AIName = "", ScriptName = "npc_hogger" WHERE entry = 448;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 448 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (94, 116) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(94, 0, 0, 0, 67, 0, 100, 0, 3900, 6900, 0, 0, 11, 37685, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Cutpurse - Is Behind Target - Cast Spell 'Backstab'"),
(94, 0, 1, 0, 4, 0, 35, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Cutpurse - On Aggro - Say Text Line 0 (No Repeat)"),
(116, 0, 0, 0, 0, 0, 100, 0, 8500, 10500, 37100, 42100, 11, 8646, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Bandit - In Combat - Cast Spell 'Snap Kick'"),
(116, 0, 1, 0, 4, 0, 35, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Bandit - On Aggro - Say Text Line 0 (No Repeat)");
DELETE FROM `creature_text` WHERE `CreatureID` IN (94, 116);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(94, 0, 0, "Give me all your gold!", 12, 0, 100, 0, 0, 0, 42881, 0, 'Cutpurse'),
(94, 0, 1, "You're in the wrong place at the wrong time, $g pal:missy;!", 12, 0, 100, 0, 0, 0, 42883, 0, 'Cutpurse'),
(94, 0, 2, "No one here gets out alive!", 12, 0, 100, 0, 0, 0, 42882, 0, 'Cutpurse'),
(94, 0, 3, "Your money or your life!", 12, 0, 100, 0, 0, 0, 42884, 0, 'Cutpurse'),
(94, 0, 4, "Fate has brought you to me!", 12, 0, 100, 0, 0, 0, 42885, 0, 'Cutpurse'),
(116, 0, 0, "Give me all your gold!", 12, 0, 100, 0, 0, 0, 42881, 0, 'Bandit'),
(116, 0, 1, "You're in the wrong place at the wrong time, $g pal:missy;!", 12, 0, 100, 0, 0, 0, 42883, 0, 'Bandit'),
(116, 0, 2, "No one here gets out alive!", 12, 0, 100, 0, 0, 0, 42882, 0, 'Bandit'),
(116, 0, 3, "Your money or your life!", 12, 0, 100, 0, 0, 0, 42884, 0, 'Bandit'),
(116, 0, 4, "Fate has brought you to me!", 12, 0, 100, 0, 0, 0, 42885, 0, 'Bandit');
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2, `position_x` = -9946.83, `position_y` = 408.503, `position_z` = 34.89249 WHERE `guid` = 180100 AND id = 330;
DELETE FROM creature_template_addon WHERE entry = 330;
DELETE FROM `creature_addon` WHERE `guid` = 180100;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(180100, 180100*10, 0, 0, 1, 0, "87519");
DELETE FROM `waypoint_data` WHERE `id` = 180100*10;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(180100*10,1,-9946.83,408.503,34.89249,0,0,0),
(180100*10,2,-9934.89,375.823,35.15996,0,0,0),
(180100*10,3,-9905.29,378.352,35.28614,0,0,0),
(180100*10,4,-9889.87,404.354,35.3951,0,0,0),
(180100*10,5,-9911.25,423.392,35.39784,0,0,0);
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 251;
UPDATE creature SET spawntimesecs = 60 WHERE guid = 179761 AND id = 251;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (253, 251) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (253*100, 251*100) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(253, 0, 0, 0, 20, 0, 100, 0, 112, 0, 0, 0, 80, 253*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "William Pestle - On Quest 'Collecting Kelp' Finished - Run Script"),
(253*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "William Pestle - On Script - Say Text Line 0"),
(253*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1.45219, "William Pestle - On Script - Set Orientation (1.45219)"),
(253*100, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 69, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "William Pestle - On Script - Play Emote (69)"),
(253*100, 9, 3, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, "William Pestle - On Script - Set Orientation (Closest Player)"),
(253*100, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "William Pestle - On Script - Say Text Line 1"),
(251, 0, 0, 0, 20, 0, 100, 0, 114, 0, 0, 0, 80, 251*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Maybell Maclure - On Quest 'The Escape' Finished - Run Script"),
(251*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 5.39307, "Maybell Maclure - On Script - Set Orientation (5.39307)"),
(251*100, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Say Text Line 0"),
(251*100, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Play Emote (7)"),
(251*100, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "On Script - Despawn Instant");
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 178007 AND id = 12423;
DELETE FROM creature_addon WHERE guid IN (177315, 318773);
UPDATE `creature_template` SET `AIName` = "SmartAI" WHERE `entry` IN (913, 927);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (913, 927) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(913, 0, 0, 0, 60, 0, 100, 0, 7000, 12000, 7000, 12000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lyria Du Lac - On Update - Play Random Emote (396, 273, 274, 6)"),
(927, 0, 0, 0, 60, 0, 100, 0, 7000, 12000, 7000, 12000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Brother Wilhelm - On Update - Play Random Emote (396, 273, 274, 6)");
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2, `position_x` = -9427.082, `position_y` = 124.8642, `position_z` = 59.36305 WHERE `guid` = 177928 AND id = 797;
DELETE FROM creature_template_addon WHERE entry = 797;
DELETE FROM `creature_addon` WHERE `guid` = 177928;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(177928, 177928*10, 0, 0, 1, 0, "");
DELETE FROM `waypoint_data` WHERE `id` = 177928*10;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`) VALUES
(177928*10,1,-9427.082,124.8642,59.36305,0,46000,1),
(177928*10,2,-9446.757,132.8065,58.79227,0,0,1),
(177928*10,3,-9453.114,128.8822,58.91727,0,0,1),
(177928*10,4,-9460.528,123.3364,59.0911,0,0,1),
(177928*10,5,-9465.454,112.1725,57.79227,0,0,1),
(177928*10,6,-9464.705,105.2042,58.28145,0,0,1),
(177928*10,7,-9460.628,94.35499,58.54224,0,46000,1),
(177928*10,8,-9464.572,107.5604,57.637,0,0,1),
(177928*10,9,-9463.494,121.7567,58.65861,0,0,1),
(177928*10,10,-9456.734,126.0858,59.16288,0,0,1),
(177928*10,11,-9444.896,135.8292,58.48393,0,0,1),
(177928*10,12,-9438.719,134.7248,58.30473,0,0,1);

UPDATE creature_template SET unit_flags = 0 WHERE entry = 42218;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2, `position_x` = -9620.014, `position_y` = -829.742, `position_z` = 43.698, `orientation` = 5.452 WHERE `guid` = 178787 AND id = 42218;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 178448 AND id = 42218;
UPDATE `creature` SET `position_x` = -9397.833, `position_y` = 94.721, `position_z` = 58.733, `orientation` = 0.429 WHERE `guid` = 178445 AND id = 42218;
UPDATE `creature` SET `position_x` = -9401.03, `position_y` = 92.905, `position_z` = 58.486, `orientation` = 0.532 WHERE `guid` = 179869 AND id = 42218;
UPDATE `creature` SET `position_x` = -9405.197, `position_y` = 90.268, `position_z` = 58.058, `orientation` = 0.571 WHERE `guid` = 179871 AND id = 42218;
UPDATE `creature` SET `position_x` = -9407.035, `position_y` = 93.127, `position_z` = 57.9007, `orientation` = 0.571 WHERE `guid` = 179874 AND id = 42218;
UPDATE `creature` SET `position_x` = -9403.378, `position_y` = 95.546, `position_z` = 58.344, `orientation` = 0.5909 WHERE `guid` = 179873 AND id = 42218;
UPDATE `creature` SET `position_x` = -9405.666, `position_y` = 98.953, `position_z` = 58.618, `orientation` = 0.5909 WHERE `guid` = 179877 AND id = 42218;
UPDATE `creature` SET `position_x` = -9408.952, `position_y` = 96.712, `position_z` = 58.293, `orientation` = 0.5988 WHERE `guid` = 179876 AND id = 42218;

DELETE FROM `creature_addon` WHERE `guid` IN (178787, 178789, 178792, 178795, 178791, 178794, 178788, 178790, 178793);
DELETE FROM `creature_addon` WHERE `guid` IN (178448, 178445, 179869, 178449, 179871, 179874, 179873, 179877, 179876);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(178787, 178787*10, 29284, 0, 1, 0, "32609"),
(178789, 0, 29284, 0, 1, 0, "32609"),
(178792, 0, 29284, 0, 1, 0, "32609"),
(178795, 0, 29284, 0, 1, 0, "32609"),
(178791, 0, 29284, 0, 1, 0, "32609"),
(178794, 0, 29284, 0, 1, 0, "32609"),
(178788, 0, 29284, 0, 1, 0, "32609"),
(178790, 0, 29284, 0, 1, 0, "32609"),
(178793, 0, 29284, 0, 1, 0, "32609"),

(178448, 178448*10, 29284, 0, 1, 0, "32609"),
(178445, 0, 29284, 0, 1, 0, "32609"),
(179869, 0, 29284, 0, 1, 0, "32609"),
(178449, 0, 29284, 0, 1, 0, "32609"),
(179871, 0, 29284, 0, 1, 0, "32609"),
(179874, 0, 29284, 0, 1, 0, "32609"),
(179873, 0, 29284, 0, 1, 0, "32609"),
(179877, 0, 29284, 0, 1, 0, "32609"),
(179876, 0, 29284, 0, 1, 0, "32609");

DELETE FROM `waypoint_data` WHERE `id` IN (178787*10, 178448*10);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(178787*10, 1, -9599.69, -850.821, 43.5444, 0, 0, 0, 0, 100, 0),
(178787*10, 2, -9588.24, -864.942, 43.7228, 0, 0, 0, 0, 100, 0),
(178787*10, 3, -9582.6, -883.82, 43.8299, 0, 0, 0, 0, 100, 0),
(178787*10, 4, -9581.03, -902.532, 43.6936, 0, 0, 0, 0, 100, 0),
(178787*10, 5, -9583.35, -918.922, 43.7452, 0, 0, 0, 0, 100, 0),
(178787*10, 6, -9588.3, -932.881, 43.4867, 0, 0, 0, 0, 100, 0),
(178787*10, 7, -9603.29, -951.21, 43.8373, 0, 0, 0, 0, 100, 0),
(178787*10, 8, -9615.51, -970.789, 43.569, 0, 0, 0, 0, 100, 0),
(178787*10, 9, -9620.19, -988.721, 43.1439, 0, 0, 0, 0, 100, 0),
(178787*10, 10, -9620.95, -999.542, 42.2882, 0, 0, 0, 0, 100, 0),
(178787*10, 11, -9619.82, -982.894, 43.5259, 0, 0, 0, 0, 100, 0),
(178787*10, 12, -9615.19, -968.207, 43.6481, 0, 0, 0, 0, 100, 0),
(178787*10, 13, -9609.2, -957.915, 43.6405, 0, 0, 0, 0, 100, 0),
(178787*10, 14, -9594.76, -939.444, 43.78, 0, 0, 0, 0, 100, 0),
(178787*10, 15, -9587.18, -926.721, 43.3948, 0, 0, 0, 0, 100, 0),
(178787*10, 16, -9582.71, -912.978, 43.7226, 0, 0, 0, 0, 100, 0),
(178787*10, 17, -9581.16, -893.448, 43.423, 0, 0, 0, 0, 100, 0),
(178787*10, 18, -9584.19, -876.811, 43.8177, 0, 0, 0, 0, 100, 0),
(178787*10, 19, -9590.09, -863.866, 43.7094, 0, 0, 0, 0, 100, 0),
(178787*10, 20, -9599.96, -850.428, 43.5915, 0, 0, 0, 0, 100, 0),
(178787*10, 21, -9629.99, -818.932, 43.8425, 0, 0, 0, 0, 100, 0),
(178787*10, 22, -9646.04, -799.35, 43.4826, 0, 0, 0, 0, 100, 0),
(178787*10, 23, -9649.72, -790.901, 43.6229, 0, 0, 0, 0, 100, 0),
(178787*10, 24, -9652, -770.971, 44.0047, 0, 0, 0, 0, 100, 0),
(178787*10, 25, -9652.45, -759.318, 44.4417, 0, 0, 0, 0, 100, 0),
(178787*10, 26, -9656.07, -740.056, 44.5474, 0, 0, 0, 0, 100, 0),
(178787*10, 27, -9654.37, -721.701, 44.5719, 0, 0, 0, 0, 100, 0),
(178787*10, 28, -9646.76, -690.187, 47.0927, 0, 0, 0, 0, 100, 0),
(178787*10, 29, -9642.34, -664.429, 49.2252, 0, 0, 0, 0, 100, 0),
(178787*10, 30, -9633.85, -649.793, 49.7451, 0, 0, 0, 0, 100, 0),
(178787*10, 31, -9624.54, -633.222, 51.4311, 0, 0, 0, 0, 100, 0),
(178787*10, 32, -9621.56, -618.349, 52.0303, 0, 0, 0, 0, 100, 0),
(178787*10, 33, -9618.98, -570.205, 54.3546, 0, 0, 0, 0, 100, 0),
(178787*10, 34, -9614.93, -542.839, 54.3399, 0, 0, 0, 0, 100, 0),
(178787*10, 35, -9605.82, -524.117, 55.7588, 0, 0, 0, 0, 100, 0),
(178787*10, 36, -9594.49, -505.6, 57.421, 0, 0, 0, 0, 100, 0),
(178787*10, 37, -9588.86, -485.568, 57.7277, 0, 0, 0, 0, 100, 0),
(178787*10, 38, -9590.32, -471.729, 57.7572, 0, 0, 0, 0, 100, 0),
(178787*10, 39, -9611.19, -428.135, 57.3888, 0, 0, 0, 0, 100, 0),
(178787*10, 40, -9618.03, -399.727, 57.8593, 0, 0, 0, 0, 100, 0),
(178787*10, 41, -9619.88, -375.817, 57.6191, 0, 0, 0, 0, 100, 0),
(178787*10, 42, -9620.29, -317.664, 57.5491, 0, 0, 0, 0, 100, 0),
(178787*10, 43, -9615.45, -280.3, 57.8475, 0, 0, 0, 0, 100, 0),
(178787*10, 44, -9605.82, -244.049, 57.2141, 0, 0, 0, 0, 100, 0),
(178787*10, 45, -9588.13, -200.957, 57.6191, 0, 0, 0, 0, 100, 0),
(178787*10, 46, -9575.05, -161.134, 57.5732, 0, 0, 0, 0, 100, 0),
(178787*10, 47, -9552.48, -133.648, 57.3999, 0, 0, 0, 0, 100, 0),
(178787*10, 48, -9548.82, -114.38, 57.6067, 0, 0, 0, 0, 100, 0),
(178787*10, 49, -9548.03, -69.6535, 57.3625, 0, 0, 0, 0, 100, 0),
(178787*10, 50, -9539.99, -35.3127, 56.4262, 0, 0, 0, 0, 100, 0),
(178787*10, 51, -9497.92, 49.2135, 55.914, 0, 0, 0, 0, 100, 0),
(178787*10, 52, -9506.98, 28.7376, 56.4141, 0, 0, 0, 0, 100, 0),
(178787*10, 53, -9536.67, -29.7596, 55.7819, 0, 0, 0, 0, 100, 0),
(178787*10, 54, -9542.28, -41.8712, 56.5395, 0, 0, 0, 0, 100, 0),
(178787*10, 55, -9546.89, -63.4407, 57.0751, 0, 0, 0, 0, 100, 0),
(178787*10, 56, -9549.25, -91.5616, 57.6199, 0, 0, 0, 0, 100, 0),
(178787*10, 57, -9550.29, -126.391, 57.4935, 0, 0, 0, 0, 100, 0),
(178787*10, 58, -9556.88, -140.982, 57.3447, 0, 0, 0, 0, 100, 0),
(178787*10, 59, -9572.89, -156.212, 57.3732, 0, 0, 0, 0, 100, 0),
(178787*10, 60, -9582.26, -182.315, 57.5975, 0, 0, 0, 0, 100, 0),
(178787*10, 61, -9587.26, -199.332, 57.5504, 0, 0, 0, 0, 100, 0),
(178787*10, 62, -9607.07, -248.701, 57.2221, 0, 0, 0, 0, 100, 0),
(178787*10, 63, -9615.86, -281.367, 57.8163, 0, 0, 0, 0, 100, 0),
(178787*10, 64, -9620.34, -319.485, 57.5662, 0, 0, 0, 0, 100, 0),
(178787*10, 65, -9620.85, -366.85, 57.3349, 0, 0, 0, 0, 100, 0),
(178787*10, 66, -9617.54, -405.323, 57.3193, 0, 0, 0, 0, 100, 0),
(178787*10, 67, -9610.85, -430.099, 57.3968, 0, 0, 0, 0, 100, 0),
(178787*10, 68, -9591.9, -467.184, 57.761, 0, 0, 0, 0, 100, 0),
(178787*10, 69, -9589.38, -486.818, 57.7368, 0, 0, 0, 0, 100, 0),
(178787*10, 70, -9595.13, -506.407, 57.4069, 0, 0, 0, 0, 100, 0),
(178787*10, 71, -9611.46, -531.539, 54.7528, 0, 0, 0, 0, 100, 0),
(178787*10, 72, -9615.86, -547.487, 54.3732, 0, 0, 0, 0, 100, 0),
(178787*10, 73, -9621.22, -594.189, 53.3917, 0, 0, 0, 0, 100, 0),
(178787*10, 74, -9623.18, -627.03, 51.6469, 0, 0, 0, 0, 100, 0),
(178787*10, 75, -9630.86, -645.813, 50.4042, 0, 0, 0, 0, 100, 0),
(178787*10, 76, -9642.42, -664.445, 49.2229, 0, 0, 0, 0, 100, 0),
(178787*10, 77, -9649.59, -700.208, 46.1331, 0, 0, 0, 0, 100, 0),
(178787*10, 78, -9655.82, -723.051, 44.5423, 0, 0, 0, 0, 100, 0),
(178787*10, 79, -9656.38, -740.183, 44.5549, 0, 0, 0, 0, 100, 0),
(178787*10, 80, -9651.36, -766.415, 44.1197, 0, 0, 0, 0, 100, 0),
(178787*10, 81, -9650.83, -788.338, 43.8092, 0, 0, 0, 0, 100, 0),
(178787*10, 82, -9641.23, -807.485, 43.6331, 0, 0, 0, 0, 100, 0),
(178787*10, 83, -9633.3, -816.971, 43.8162, 0, 0, 0, 0, 100, 0),
(178787*10, 84, -9621.53, -828.625, 43.6619, 0, 0, 0, 0, 100, 0),

(178448*10, 1, -9383.01, 104.414, 59.8336, 0, 0, 0, 0, 100, 0),
(178448*10, 2, -9368.5, 108.266, 60.847, 0, 0, 0, 0, 100, 0),
(178448*10, 3, -9349.11, 116.921, 62.5884, 0, 0, 0, 0, 100, 0),
(178448*10, 4, -9325.23, 131.598, 64.1205, 0, 0, 0, 0, 100, 0),
(178448*10, 5, -9310.44, 140.783, 65.4548, 0, 0, 0, 0, 100, 0),
(178448*10, 6, -9290.87, 149.02, 66.4885, 0, 0, 0, 0, 100, 0),
(178448*10, 7, -9270.26, 151.894, 67.0837, 0, 0, 0, 0, 100, 0),
(178448*10, 8, -9256.19, 157.498, 67.6996, 0, 0, 0, 0, 100, 0),
(178448*10, 9, -9246.62, 165.255, 67.9183, 0, 0, 0, 0, 100, 0),
(178448*10, 10, -9233.35, 184.822, 67.6321, 0, 0, 0, 0, 100, 0),
(178448*10, 11, -9219.51, 202.75, 68.8622, 0, 0, 0, 0, 100, 0),
(178448*10, 12, -9192.92, 229.029, 71.7499, 0, 0, 0, 0, 100, 0),
(178448*10, 13, -9184.31, 246.544, 73.1934, 0, 0, 0, 0, 100, 0),
(178448*10, 14, -9180.71, 263.004, 74.4242, 0, 0, 0, 0, 100, 0),
(178448*10, 15, -9179.52, 281.933, 75.9409, 0, 0, 0, 0, 100, 0),
(178448*10, 16, -9180.79, 313.479, 79.2692, 0, 0, 0, 0, 100, 0),
(178448*10, 17, -9178.27, 330.988, 82.0722, 0, 0, 0, 0, 100, 0),
(178448*10, 18, -9171.33, 344.613, 85.3042, 0, 0, 0, 0, 100, 0),
(178448*10, 19, -9159.41, 358.652, 89.0692, 0, 0, 0, 0, 100, 0),
(178448*10, 20, -9140.27, 378.555, 90.6682, 0, 0, 0, 0, 100, 0),
(178448*10, 21, -9125.48, 390.891, 91.5189, 0, 0, 0, 0, 100, 0),
(178448*10, 22, -9109.93, 402.471, 92.6044, 0, 0, 0, 0, 100, 0),
(178448*10, 23, -9123.98, 392.407, 91.7277, 0, 0, 0, 0, 100, 0),
(178448*10, 24, -9143.37, 375.207, 90.6907, 0, 0, 0, 0, 100, 0),
(178448*10, 25, -9165.3, 350.741, 87.2025, 0, 0, 0, 0, 100, 0),
(178448*10, 26, -9176.93, 333.816, 82.8446, 0, 0, 0, 0, 100, 0),
(178448*10, 27, -9180.21, 319.028, 79.9468, 0, 0, 0, 0, 100, 0),
(178448*10, 28, -9181.04, 302.769, 78.4457, 0, 0, 0, 0, 100, 0),
(178448*10, 29, -9179.68, 269.244, 74.9092, 0, 0, 0, 0, 100, 0),
(178448*10, 30, -9183.16, 250.743, 73.4896, 0, 0, 0, 0, 100, 0),
(178448*10, 31, -9189.66, 233.375, 71.9653, 0, 0, 0, 0, 100, 0),
(178448*10, 32, -9196.51, 224.529, 71.5633, 0, 0, 0, 0, 100, 0),
(178448*10, 33, -9208.71, 213.096, 70.4991, 0, 0, 0, 0, 100, 0),
(178448*10, 34, -9230.81, 188.217, 67.7736, 0, 0, 0, 0, 100, 0),
(178448*10, 35, -9246.16, 165.613, 67.9092, 0, 0, 0, 0, 100, 0),
(178448*10, 36, -9258.37, 155.716, 67.6081, 0, 0, 0, 0, 100, 0),
(178448*10, 37, -9270.76, 151.579, 67.0683, 0, 0, 0, 0, 100, 0),
(178448*10, 38, -9291.52, 148.527, 66.4542, 0, 0, 0, 0, 100, 0),
(178448*10, 39, -9305.09, 143.073, 65.763, 0, 0, 0, 0, 100, 0),
(178448*10, 40, -9341.06, 122.068, 63.2984, 0, 0, 0, 0, 100, 0),
(178448*10, 41, -9353.72, 113.916, 62.0959, 0, 0, 0, 0, 100, 0),
(178448*10, 42, -9372.01, 106.995, 60.6684, 0, 0, 0, 0, 100, 0),
(178448*10, 43, -9396.75, 99.7925, 58.7783, 0, 0, 0, 0, 100, 0),
(178448*10, 44, -9416.34, 86.9031, 56.9933, 0, 0, 0, 0, 100, 0),
(178448*10, 45, -9435.53, 72.0034, 56.0894, 0, 0, 0, 0, 100, 0),
(178448*10, 46, -9420.2, 83.8999, 56.6448, 0, 0, 0, 0, 100, 0),
(178448*10, 47, -9401.91, 96.6638, 58.4878, 0, 0, 0, 0, 100, 0);

DELETE FROM `creature_formations` WHERE `leaderGUID` IN (178787, 178448);
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
(178787, 178787, 0, 0, 515),
(178787, 178789, 3, 90, 515),
(178787, 178792, 5, 45, 515),
(178787, 178795, 8, 20, 515),
(178787, 178791, 4, 360, 515),
(178787, 178794, 8, 360, 515),
(178787, 178788, 3, 270, 515),
(178787, 178790, 5, 315, 515),
(178787, 178793, 8, 340, 515),

(178448, 178448, 0, 0, 515),
(178448, 178445, 3, 90, 515),
(178448, 179869, 5, 45, 515),
(178448, 179871, 8, 20, 515),
(178448, 179873, 4, 360, 515),
(178448, 179874, 8, 360, 515),
(178448, 178449, 3, 270, 515),
(178448, 179877, 5, 315, 515),
(178448, 179876, 8, 340, 515);

DELETE FROM creature WHERE guid = 302899 AND id = 583;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry IN (5917, 583);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (5917, 583) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(5917, 0, 0, 0, 34, 0, 100, 0, 2, 5, 0, 0, 12, 583, 4, 180000, 0, 0, 0, 8, 0, 0, 0, -9232.11, 342.473, 74.4399, 4.31658, "Clara Charles - On WP 5 Reached - Summon Creature 'Defias Ambusher'"),

(583, 0, 0, 0, 4, 0, 30, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Defias Ambusher - On Aggro - Say Line 0 (No Repeat)"),
(583, 0, 1, 0, 54, 0, 100, 1, 0, 0, 0, 0, 53, 0, 583, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, "Defias Ambusher - On Just Summoned - Start WP (No Repeat)"),
(583, 0, 2, 0, 40, 0, 100, 0, 5, 583, 0, 0, 54, 7000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Defias Ambusher - On WP 5 Reached - WP Pause (7000 ms)"),
(583, 0, 4, 0, 40, 0, 100, 0, 11, 583, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Defias Ambusher - On WP 11 Reached - Despawn (2000 ms)");

UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 177192 AND id = 5917;
DELETE FROM creature_template_addon WHERE entry = 5917;
DELETE FROM creature_addon WHERE guid = 177192;
INSERT INTo creature_addon (guid, path_id, bytes1, bytes2) VALUES
(177192, 177192*10, 0, 4097);

DELETE FROM waypoint_data WHERE id = 177192*10;
INSERT INTO waypoint_data (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(177192*10, 1, -9266.12, 354.4, 76.6919, 2.2945, 60000, 0, 0, 100, 0),
(177192*10, 2, -9265.77, 353.952, 76.692, 5.37954, 0, 0, 0, 100, 0),
(177192*10, 3, -9259.54, 346.55, 75.7548, 5.44709, 0, 0, 0, 100, 0),
(177192*10, 4, -9240.37, 343.05, 74.8524, 0.194343, 0, 0, 0, 100, 0),
(177192*10, 5, -9225.87, 355.227, 73.4195, 0.68129, 0, 0, 0, 100, 0),
(177192*10, 6, -9215.68, 363.51, 72.1801, 1.00959, 30000, 0, 0, 100, 0),
(177192*10, 7, -9223.65, 358.136, 72.7038, 3.73649, 0, 0, 0, 100, 0),
(177192*10, 8, -9251.11, 339.555, 75.0156, 3.73649, 0, 0, 0, 100, 0),
(177192*10, 9, -9259.98, 347.165, 75.7808, 2.28743, 0, 0, 0, 100, 0),
(177192*10, 10, -9265.62, 353.631, 76.6927, 2.28743, 0, 0, 0, 100, 0),
(177192*10, 11, -9266.12, 354.4, 76.6919, 2.2945, 180000, 0, 0, 100, 0);

DELETE FROM waypoints WHERE entry = 583;
INSERT INTO waypoints (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(583, 1, -9232.11, 342.473, 74.4399, 'Defias Ambusher'),
(583, 2, -9227.82, 341.992, 73.9688, 'Defias Ambusher'),
(583, 3, -9221.57, 347.109, 73.9924, 'Defias Ambusher'),
(583, 4, -9216.57, 357.231, 72.8203, 'Defias Ambusher'),
(583, 5, -9216.18, 362.686, 72.1863, 'Defias Ambusher'),
(583, 6, -9219.08, 353.811, 73.1801, 'Defias Ambusher'),
(583, 7, -9222.54, 341.161, 73.8594, 'Defias Ambusher'),
(583, 8, -9224.02, 326.395, 73.0095, 'Defias Ambusher'),
(583, 9, -9226.26, 316.182, 74.112, 'Defias Ambusher'),
(583, 10, -9231.02, 306.332, 74.406, 'Defias Ambusher'),
(583, 11, -9239.83, 305.172, 74.2633, 'Defias Ambusher');




-- 177459 - WPs
-- ID: 11328 - WPs + Emotes
-- GUID: 179023 - OOC Texts

-- WP Childs
-- WP Murlocs
-- WP Kobolds

-- Other
DELETE FROM `areatrigger` WHERE `guid` IN (48,49);
INSERT INTO `areatrigger` (`guid`, `id`, `map_id`, `spawn_mask`, `position_x`, `position_y`, `position_z`, `ScriptName`) VALUES 
(49, 100007, 1712, 245760, -3442.35, 9521.4, 9.57194, ""),
(48, 100010, 1669, 1, -3217.97, 9423.45, -172.491, "");
