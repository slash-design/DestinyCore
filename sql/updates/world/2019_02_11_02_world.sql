-- Quests: 24959, 28608, 26799, 28652, 24960, 25089, 26800, 28653, 26801, 28651, 24961, 28672, 26802, 24973, 24970, 24971
UPDATE `creature` SET `position_z` = 137.008 WHERE `guid` = 192768;
UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` = 49044;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49044 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(49044, 0, 0, 1, 19, 0, 100, 0, 24959, 0, 0, 0, 11, 73524, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Agatha - On Quest Accept (24959) - Cast Spell"),
(49044, 0, 1, 0, 61, 0, 100, 512, 0, 0, 0, 0, 28, 73523, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Agatha - On Quest Accept (24959) (Link) - Remove Aura");

UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` IN (192771, 192713, 192770, 192714, 192712, 192625);
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0, `position_x` = 1674.95, `position_y` = 1683.32, `position_z` = 139.203 WHERE `id` = 50372;
UPDATE `creature_template` SET `InhabitType` = 3, `AIName` = "", `ScriptName` = "npc_aradne" WHERE `entry` = 50372;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 50372 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50372*100 AND `source_type` = 9;
DELETE FROM `waypoints` WHERE `entry` = 50372;
UPDATE `creature_template_addon` SET `path_id` = 50372*100 WHERE `entry` = 50372;
DELETE FROM `waypoint_data` WHERE `id` = 50372*100;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(50372*100, 1, 1674.95, 1683.32, 139.203, 0, 0, 0),
(50372*100, 2, 1676.98, 1681.87, 138.799, 0, 0, 0),
(50372*100, 3, 1679.82, 1681.69, 138.267, 0, 0, 0),
(50372*100, 4, 1681.72, 1683.03, 137.959, 0, 0, 0),
(50372*100, 5, 1683.93, 1685.34, 137.606, 0, 10000, 0),
(50372*100, 6, 1686.35, 1686.91, 137.154, 0, 0, 0),
(50372*100, 7, 1688.19, 1690.18, 136.926, 0, 0, 0),
(50372*100, 8, 1689.24, 1693.53, 136.947, 0, 0, 0),
(50372*100, 9, 1691.95, 1695.14, 136.673, 0, 0, 0),
(50372*100, 10, 1695.06, 1695.14, 136.008, 0, 0, 0),
(50372*100, 11, 1697.34, 1693.19, 135.244, 0, 0, 0),
(50372*100, 12, 1698, 1691.2, 135.049, 0, 0, 0),
(50372*100, 13, 1698.51, 1688.87, 134.864, 0, 10000, 0),
(50372*100, 14, 1698.55, 1685.78, 134.671, 0, 0, 0),
(50372*100, 15, 1698.47, 1683.06, 134.616, 0, 0, 0),
(50372*100, 16, 1697.66, 1680.8, 134.579, 0, 10000, 0),
(50372*100, 17, 1699.37, 1684.32, 134.532, 0, 0, 0),
(50372*100, 18, 1702.7, 1690.31, 134.334, 0, 0, 0),
(50372*100, 19, 1704.96, 1695.01, 134.167, 0, 0, 0),
(50372*100, 20, 1706.95, 1696.84, 133.934, 0, 0, 0),
(50372*100, 21, 1707.11, 1700.6, 134.225, 0, 10000, 0),
(50372*100, 22, 1710.43, 1700.99, 133.2, 0, 0, 0),
(50372*100, 23, 1716.15, 1701.67, 131.765, 0, 0, 0),
(50372*100, 24, 1721.19, 1702.27, 130.121, 0, 0, 0),
(50372*100, 25, 1726.76, 1703.41, 128.53, 0, 0, 0),
(50372*100, 26, 1730.24, 1703.47, 128.115, 0, 0, 0),
(50372*100, 27, 1733.31, 1703.32, 128.178, 0, 10000, 0),
(50372*100, 28, 1726.35, 1702.37, 128.503, 0, 0, 0),
(50372*100, 29, 1719.89, 1698.69, 130.855, 0, 0, 0),
(50372*100, 30, 1710.81, 1691.48, 133.626, 0, 0, 0),
(50372*100, 31, 1703.63, 1685.48, 134.024, 0, 0, 0),
(50372*100, 32, 1697.91, 1681.32, 134.569, 0, 10000, 0),
(50372*100, 33, 1698.1, 1683.77, 134.69, 0, 0, 0),
(50372*100, 34, 1698.15, 1688.19, 134.898, 0, 10000, 0),
(50372*100, 35, 1699.09, 1691.26, 134.879, 0, 0, 0),
(50372*100, 36, 1701.96, 1695.36, 134.619, 0, 0, 0),
(50372*100, 37, 1705.86, 1696.63, 134.068, 0, 0, 0),
(50372*100, 38, 1706.74, 1700.46, 134.263, 0, 10000, 0),
(50372*100, 39, 1700.76, 1698.66, 135.131, 0, 0, 0),
(50372*100, 40, 1696.39, 1697.44, 135.9, 0, 0, 0),
(50372*100, 41, 1691.82, 1695.35, 136.723, 0, 0, 0),
(50372*100, 42, 1687.56, 1692.08, 137.147, 0, 0, 0),
(50372*100, 43, 1686.83, 1690.55, 137.241, 0, 0, 0);

UPDATE `creature_template` SET `unit_flags` = 33554688, `modelid1` = 25268, `modelid2` = 0, `AIName` = "", `ScriptName` = "npc_deathknell_grave_target" WHERE `entry` = 50373;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50373 AND `source_type` = 0;
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `id` = 50373;

DELETE FROM `conditions` WHERE `SourceEntry` = 93446 AND `SourceTypeOrReferenceId` = 13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 93446, 0, 0, 31, 0, 3, 50373, 0, 0, 0, 0, "", "Raise Undead target Deathknell Grave Target");

UPDATE `creature_template` SET `AIName` = "", `ScriptName` = "npc_risen_dead" WHERE `entry` = 50374;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50374 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (50374*100, 50374*100+1, 50374*100+2) AND `source_type` = 9;
DELETE FROM `waypoints` WHERE `entry` IN (5037400, 5037401);
DELETE FROM `creature_text` WHERE `CreatureID` = 50374;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(50374, 0, 0, "What... what's happening to me? Why have you done this to me?", 12, 0, 100, 6, 0, 0, "Risen Dead", 50194),
(50374, 1, 0, "I should be dead and to death I will return!", 12, 0, 100, 5, 0, 0, "Risen Dead", 50195),
(50374, 2, 0, "I never asked for this! Leave me alone!", 12, 0, 100, 5, 0, 0, "Risen Dead", 50201),
(50374, 3, 0, "What magic is this that turns back the hand of death?", 12, 0, 100, 6, 0, 0, "Risen Dead", 50197),
(50374, 4, 0, "If the Banshee Queen has offered me this chance, I will gladly serve.", 12, 0, 100, 1, 0, 0, "Risen Dead", 50200),
(50374, 5, 0, "This is Lady Sylvanas's doing?", 12, 0, 100, 6, 0, 0, "Risen Dead", 50196),
(50374, 6, 0, "What use has the Dark Lady for me?", 12, 0, 100, 6, 0, 0, "Risen Dead", 50199),
(50374, 7, 0, "I... I don't remember... Why have you done this?", 12, 0, 100, 6, 0, 0, "Risen Dead", 50198),
(50374, 8, 0, "%s becomes enraged!", 16, 0, 100, 0, 0, 0, "Risen Dead", 7798);

DELETE FROM `creature` WHERE `guid` = 193848 AND `id` = 50414;
DELETE FROM `waypoint_data` WHERE `id` IN (50374*100, (50374*100)+1);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(50374*100, 1, 1702, 1677.55, 134.298, 0, 0, 1),
(50374*100, 2, 1693.66, 1670.64, 134.032, 0, 0, 1),
(50374*100, 3, 1686.21, 1661.74, 133.171, 0, 0, 1),
(50374*100, 4, 1681.66, 1655.15, 136.316, 0, 0, 1),
(50374*100, 5, 1676.76, 1643.16, 139.253, 0, 0, 1),
(50374*100, 6, 1669.11, 1631.08, 139.495, 0, 0, 1),
(50374*100, 7, 1664.203, 1620.63, 140.526, 0, 0, 1),
((50374*100)+1, 1, 1695.44, 1672.29, 133.945, 0, 0, 0),
((50374*100)+1, 2, 1691.08, 1660.22, 131.406, 0, 0, 0),
((50374*100)+1, 3, 1709.56, 1643.45, 124.713, 0, 0, 0),
((50374*100)+1, 4, 1723.48, 1633.31, 120.426, 0, 0, 0);

UPDATE `creature_template` SET `AIName` = "", `ScriptName` = "npc_undertaker_mordo" WHERE `entry` = 1568;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1568 AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = "" WHERE `entry` = 50414;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50414 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50414*100 AND `source_type` = 9;
DELETE FROM `waypoints` WHERE `entry` IN (50414, 504140);

DELETE FROM `waypoint_data` WHERE `id` IN (50414*100, (50414*100)+1);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(50414*100, 1, 1694.16, 1677.08, 134.786, 0, 0, 0),
(50414*100, 2, 1691.29, 1675.84, 135.295, 0, 0, 0),
((50414*100)+1, 1, 1691.29, 1675.85, 135.295, 0, 0, 0),
((50414*100)+1, 2, 1690.59, 1661.87, 131.92, 0, 0, 0),
((50414*100)+1, 3, 1719.38, 1633.98, 121.065, 0, 0, 0);

UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` = 49129;
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 192757;
UPDATE `creature_template_addon` SET `path_id` = 192757*100 WHERE `entry` = 49129;

DELETE FROM `waypoint_data` WHERE `id` IN (192757*100, 3250850);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(192757*100, 1, 1725.01, 1667.63, 137.94, 0, 0, 1),
(192757*100, 2, 1710.03, 1656.84, 140.835, 0, 0, 1),
(192757*100, 3, 1698.03, 1652.92, 146.397, 0, 0, 1),
(192757*100, 4, 1702.94, 1634.51, 140.377, 0, 0, 1),
(192757*100, 5, 1716.34, 1617.19, 134.848, 0, 0, 1),
(192757*100, 6, 1731.1, 1618.59, 133.88, 0, 0, 1),
(192757*100, 7, 1747.64, 1639.03, 128.976, 0, 0, 1),
(192757*100, 8, 1746.05, 1657.5, 131.738, 0, 0, 1),
(192757*100, 9, 1744.97, 1676.7, 139.807, 0, 0, 1),
(192757*100, 10, 1737.38, 1689.39, 142.346, 0, 0, 1),
(192757*100, 11, 1721.84, 1699.9, 145.727, 0, 0, 1),
(192757*100, 12, 1702.88, 1701.67, 147.952, 0, 0, 1),
(192757*100, 13, 1684.23, 1697.14, 151.702, 0, 0, 1),
(192757*100, 14, 1679.76, 1677.61, 157.186, 0, 0, 1),
(192757*100, 15, 1688.75, 1665.62, 152.315, 0, 0, 1),
(192757*100, 16, 1706.31, 1666.57, 144.594, 0, 0, 1),
(192757*100, 17, 1721.01, 1675.69, 138.709, 0, 0, 1);

UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` = 49128;
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 192760;
UPDATE `creature_template_addon` SET `path_id` = 192760*100 WHERE `entry` = 49128;

DELETE FROM `waypoint_data` WHERE `id` = 192760*100;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(192760*100, 1, 1757.78, 1593.55, 116.917, 0, 0, 1),
(192760*100, 2, 1758.89, 1604.89, 127.626, 0, 0, 1),
(192760*100, 3, 1754.94, 1620.19, 131.002, 0, 0, 1),
(192760*100, 4, 1741.96, 1635.04, 141.067, 0, 0, 1),
(192760*100, 5, 1722.93, 1649.94, 144.151, 0, 0, 1),
(192760*100, 6, 1703.4, 1655.41, 156.222, 0, 0, 1),
(192760*100, 7, 1683.45, 1647.61, 158.232, 0, 0, 1),
(192760*100, 8, 1678.86, 1635.12, 154.974, 0, 0, 1),
(192760*100, 9, 1681.15, 1625.05, 153.188, 0, 0, 1),
(192760*100, 10, 1699.46, 1616.94, 146.098, 0, 0, 1),
(192760*100, 11, 1701.61, 1601.05, 142.797, 0, 0, 1),
(192760*100, 12, 1697.91, 1578.1, 144.346, 0, 0, 1),
(192760*100, 13, 1705.4, 1557.76, 143.995, 0, 0, 1),
(192760*100, 14, 1719.17, 1551.01, 136.678, 0, 0, 1),
(192760*100, 15, 1736.93, 1555, 129.218, 0, 0, 1),
(192760*100, 16, 1746.94, 1565.07, 123.072, 0, 0, 1),
(192760*100, 17, 1755.16, 1577.36, 119.816, 0, 0, 1);

UPDATE `creature_template` SET `ScriptName` = "npc_mindless_zombie" WHERE `entry` = 1501;
UPDATE `creature_template` SET `AIName` = "", `ScriptName` = "npc_darnell" WHERE `entry` = 49141;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49141 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49141*100, 49141*100+1, 49141*100+2, 49141*100+3, 49141*100+4);
DELETE FROM `waypoints` WHERE `entry` = 49141;
DELETE FROM `creature` WHERE `guid` = 280000497 AND `id` = 38895;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2307 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2307*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(2307, 0, 0, 0, 19, 0, 100, 512, 24960, 0, 0, 0, 80, 2307*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Caretaker Caice - On Quest Accept (24960) - Start Script"),
(2307, 0, 1, 0, 19, 0, 100, 0, 25089, 0, 0, 0, 85, 91938, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Caretaker Caice - On Quest Accept (25089) - Cast Spell To Summon"),
(2307*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12, 49231, 3, 120000, 0, 0, 0, 8, 0, 0, 0, 1740.36, 1704.34, 128.765, 1.55296, "Caretaker Caice - On Script - Summon Creature"),
(2307*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12, 38895, 3, 120000, 0, 0, 0, 8, 0, 0, 0, 1757.48, 1666.03, 121.208, 0.02929, "Caretaker Caice - On Script - Summon Creature"),
(2307*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12, 49230, 3, 120000, 0, 0, 0, 8, 0, 0, 0, 1753.89, 1640.63, 117.495, 0, "Caretaker Caice - On Script - Summon Creature");

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49231 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49231*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(49231, 0, 0, 0, 62, 0, 100, 512, 12489, 0, 0, 0, 80, 49231*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Valdred Moray - On Gossip Select - Start Script"),
(49231, 0, 1, 0, 54, 0, 100, 512, 0, 0, 0, 0, 75, 68442, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Valdred Moray - On Just Summoned - Add Aura"),
(49231*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Valdred Moray - On Script - Close Gossip"),
(49231*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 85, 91876, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Valdred Moray - On Script - Cast Spell 'Summon Valdred'"),
(49231*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Valdred Moray - On Script - Despawn");

-- UPDATE `npc_text` SET `BroadcastTextID0` = 49349 WHERE `ID` = 17570;
-- UPDATE `npc_text` SET `BroadcastTextID0` = 49348 WHERE `ID` = 17569;
-- UPDATE `npc_text` SET `BroadcastTextID0` = 49350 WHERE `ID` = 17571;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 38895 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 38895*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(38895, 0, 0, 0, 62, 0, 100, 512, 12484, 0, 0, 0, 80, 38895*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Select - Start Script"),
(38895, 0, 1, 0, 54, 0, 100, 512, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Just Summoned - Set Emote State (431)"),
(38895*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Close Gossip"),
(38895*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 85, 91874, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Cast Spell 'Summon Lilian'"),
(38895*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Despawn");

-- UPDATE `npc_text` SET `BroadcastTextID0` = 49334 WHERE `ID` = 17564;
-- UPDATE `npc_text` SET `BroadcastTextID0` = 49335 WHERE `ID` = 17565;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49230 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49230*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(49230, 0, 0, 0, 62, 0, 100, 512, 12486, 0, 0, 0, 80, 49230*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Gossip Select - Start Script"),
(49230*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Script - Close Gossip"),
(49230*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 85, 91873, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Script - Cast Spell 'Summon Marshal'"),
(49230*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Script - Despawn");

-- UPDATE `npc_text` SET `BroadcastTextID0` = 49343 WHERE `ID` = 17566;
-- UPDATE `npc_text` SET `BroadcastTextID0` = 49344 WHERE `ID` = 17567;

UPDATE `creature` SET `position_x` = 1816.2699, `position_y` = 1589.7817, `position_z` = 96.536, `orientation` = 6.1166 WHERE `guid` = 192714;
UPDATE `creature` SET `position_x` = 1685.6896, `position_y` = 1647.7761, `position_z` = 137.354, `orientation` = 1.6925 WHERE `guid` = 192713;
UPDATE `creature` SET `position_x` = 1708.0999, `position_y` = 1644.4699, `position_z` = 126.151, `orientation` = 5.5768 WHERE `guid` = 192770;
UPDATE `creature` SET `position_x` = 1689.6483, `position_y` = 1674.8232, `position_z` = 135.612, `orientation` = 0.1570 WHERE `guid` = 192771;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-192770, -192714);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-192770, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 45, 1, 1, 0, 0, 0, 0, 11, 49337, 8, 0, 0, 0, 0, 0, 'Generic Bunny - PRK - On Update OOC (1000 - 1000) - Set Data 1 1 To Creature ID: 49337'),
(-192714, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1000, 1000, 45, 2, 2, 0, 0, 0, 0, 11, 49337, 5, 0, 0, 0, 0, 0, 'Generic Bunny - PRK - On Update OOC (1000 - 1000) - Set Data 2 2 To Creature ID: 49337');

DELETE FROM `waypoints` WHERE `entry` = 49337;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49337 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49337*100, (49337*100)+1, (49337*100)+2) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49337, 0, 0, 4, 54, 0, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Just Summoned - Say Text Line 0 (No Repeat)"),
(49337, 0, 1, 0, 38, 0, 100, 1, 1, 1, 0, 0, 80, 49337*100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Data Set 1 1 - Start Script (No Repeat)"),
(49337, 0, 2, 0, 38, 0, 100, 1, 2, 2, 0, 0, 80, (49337*100)+1, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Data Set 2 2 - Start Script (No Repeat)"),
(49337, 0, 3, 4, 54, 0, 100, 1, 0, 0, 0, 0, 80, (49337*100)+2, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Just Summoned - Start Script (No Repeat)"),
(49337, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Just Summoned (Link) - Set React State (Passive)"),
(49337, 0, 5, 0, 8, 0, 100, 0, 46598, 0, 0, 0, 11, 91935, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Spell Hit (46598) - Cast Spell Self (91935)"),
(49337*100, 9, 0, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Say Text Line 1"),
(49337*100, 9, 1, 0, 0, 0, 100, 0, 500, 500, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1783.75, 1595.59, 105.206, 0, "Darnell - On Script - Move To Position"),
(49337*100, 9, 2, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 29, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Set Follow Owner"),
((49337*100)+1, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Say Text Line 2"),
((49337*100)+1, 9, 1, 0, 0, 0, 100, 0, 500, 500, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1861.56, 1603.44, 98.11, 0, "Darnell - On Script - Move To Position"),
((49337*100)+1, 9, 2, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Say Text Line 3"),
((49337*100)+1, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 29, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Set Follow Owner"),
((49337*100)+2, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Say Text Line 4"),
((49337*100)+2, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, "Darnell - On Script - Say Text Line 5");

DELETE FROM `smart_scripts` WHERE `entryorguid` = 1740 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (1740*100, 1740*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(1740, 0, 0, 0, 19, 0, 100, 512, 26800, 0, 0, 0, 80, 1740*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Saltain - On Quest Accept (26800) - Start Script"),
(1740, 0, 1, 0, 20, 0, 100, 512, 26800, 0, 0, 0, 80, 1740*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Saltain - On Quest Reward (26800) - Start Script"),
(1740, 0, 2, 0, 20, 0, 100, 512, 25089, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 19, 49337, 20, 0, 0, 0, 0, 0, "Saltain - On Quest Reward (25089) - Despawn (Creature ID: 49337)"),
(1740*100, 9, 0, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 85, 91938, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Saltain - On Script - Cast Spell To Summon"),
(1740*100+1, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 19, 49337, 20, 0, 0, 0, 0, 0, "Saltain - On Script - Despawn (Creature ID: 49337)"),
(1740*100+1, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 11, 49340, 20, 0, 0, 0, 0, 0, "Saltain - On Script - Despawn (Creature ID: 49340)");

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49340 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(49340, 0, 0, 1, 75, 0, 100, 513, 0, 49337, 3, 0, 86, 91945, 2, 19, 49337, 20, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corpse - On Distance Creature (49337) - Cross Cast Spell"),
(49340, 0, 1, 0, 61, 0, 100, 512, 0, 0, 0, 0, 11, 46598, 2, 0, 0, 0, 0, 19, 49337, 20, 0, 0, 0, 0, 0, "Corpse - On Distance Creature (49337) (Link) - Cast Spell");

DELETE FROM `creature_queststarter` WHERE `quest` = 26800 AND `id` = 49340;
DELETE FROM `conditions` WHERE `SourceEntry` = 49337 AND `SourceTypeOrReferenceId` = 22;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 49337, 0, 0, 9, 0, 26800, 0, 0, 1, 0, 0, "", "SAI only when player has not take quest (26800)"),
-- (22, 2, 49337, 0, 0, 8, 0, 25089, 0, 0, 1, 0, 0, "", "SAI only when player has not rewarded quest (25089)"),
-- (22, 3, 49337, 0, 0, 8, 0, 25089, 0, 0, 1, 0, 0, "", "SAI only when player has not rewarded quest (25089)"),
(22, 4, 49337, 0, 0, 9, 0, 26800, 0, 0, 0, 0, 0, "", "SAI only when player has take quest (26800)");

-- 91935
DELETE FROM `creature_queststarter` WHERE `quest` = 24961 AND `id` = 38910;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 38910 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38910*100, 38910*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(38910, 0, 0, 1, 62, 1, 100, 512, 11133, 0, 0, 0, 80, 38910*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Run Script (Phase 1)"),
(38910, 0, 1, 8, 61, 1, 100, 512, 11133, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Close Gossip (Phase 1)"),
(38910, 0, 2, 3, 62, 2, 100, 512, 11133, 0, 0, 0, 80, 38910*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Run Script (Phase 2)"),
(38910, 0, 3, 9, 61, 2, 100, 512, 11133, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Close Gossip (Phase 2)"),
(38910, 0, 4, 0, 25, 0, 100, 0, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Reset - Set Emote State 431"),
(38910, 0, 5, 0, 25, 0, 100, 512, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Reset - Set Event Phase 1"),
(38910, 0, 6, 0, 40, 0, 100, 0, 11, 3891000, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Waypoint 11 Reached - Set Emote State 431"),
(38910, 0, 7, 0, 40, 0, 100, 0, 11, 3891001, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Waypoint 11 Reached - Set Emote State 431"),
(38910, 0, 8, 0, 61, 0, 100, 512, 11133, 0, 0, 0, 85, 73210, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Invoker Cast 'Show Mirror' (Phase 1)"),
(38910, 0, 9, 0, 61, 0, 100, 512, 11133, 0, 0, 0, 85, 73210, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Gossip Option 0 Selected - Invoker Cast 'Show Mirror' (Phase 1)"),
(38910*100, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Orientation"),
(38910*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Emote State (26)"),
(38910*100, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 18, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Play Emote (18)"),
(38910*100, 9, 3, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Say Line 0"),
(38910*100, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 33, 38910, 0, 0, 0, 0, 0, 17, 0, 30, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Quest Credit 'The Truth of the Grave'"),
(38910*100, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 53, 1, 3891000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Start Waypoint"),
(38910*100, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Event Phase 2"),
(38910*100+1, 9, 0, 0, 0, 0, 100, 0, 500, 500, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Orientation"),
(38910*100+1, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Emote State (26)"),
(38910*100+1, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 5, 18, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Play Emote (18)"),
(38910*100+1, 9, 3, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Say Line 0"),
(38910*100+1, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 33, 38910, 0, 0, 0, 0, 0, 17, 0, 30, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Quest Credit 'The Truth of the Grave'"),
(38910*100+1, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 53, 1, 3891001, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Start Waypoint"),
(38910*100+1, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lilian Voss - On Script - Set Event Phase 1");

UPDATE `creature_text` SET `emote` = 18 WHERE `CreatureID` = 38910 AND `GroupID` = 0 AND `ID` = 0;
UPDATE `creature` SET `spawntimesecs` = 60 WHERE `id` = 49428;
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id` = 49428);
DELETE FROM `creature_template_addon` WHERE `entry` = 49428;
INSERT INTO `creature_template_addon` (`entry`, `bytes2`, `emote`) VALUES
(49428, 1, 333);

UPDATE `creature` SET `spawndist` = 3, `MovementType` = 1 WHERE `id` IN (49422, 49423);
UPDATE `creature_template` SET `unit_flags` = 33280, `AIName` = "SmartAI" WHERE `entry` = 49424;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49422, 49423, 49424) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(49422, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Berserker - On Aggro - Say Text Line 0 (No Repeat)"),
(49422, 0, 1, 2, 2, 0, 100, 1, 0, 30, 0, 0, 11, 63227, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Berserker - 0-30% HP - Cast Spell 'Enrage' (No Repeat)"),
(49422, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Berserker - 0-30% HP (Link) - Say Text Line 1 (No Repeat)"),
(49422, 0, 3, 0, 0, 0, 100, 0, 4000, 7000, 15000, 18000, 11, 79881, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Berserker - In Combat - Cast Spell 'Slam'"),
(49422, 0, 4, 0, 25, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 49428, 50, 0, 0, 0, 0, 0, "Rotbrain Berserker - On Reset - Start Attack (Creature ID: 49428, 50 Yards)"),
(49422, 0, 5, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 49, 0, 0, 0, 0, 0, 0, 11, 49428, 50, 0, 0, 0, 0, 0, "Rotbrain Berserker - OOC (10000 - 10000) - Start Attack (Creature ID: 49428, 50 Yards)"),
(49423, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Magus - On Aggro - Say Text Line 0 (No Repeat)"),
(49423, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 11, 9053, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Magus - In Combat - Cast Spell 'Fireball'"),
(49423, 0, 2, 0, 0, 0, 100, 0, 7000, 9000, 18000, 25000, 11, 11962, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Rotbrain Magus - In Combat - Cast Spell 'Immolate'"),
(49423, 0, 3, 0, 25, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 49428, 50, 0, 0, 0, 0, 0, "Rotbrain Magus - On Reset - Start Attack (Creature ID: 49428, 50 Yards)"),
(49423, 0, 4, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 49, 0, 0, 0, 0, 0, 0, 11, 49428, 50, 0, 0, 0, 0, 0, "Rotbrain Magus - OOC (10000 - 10000) - Start Attack (Creature ID: 49428, 50 Yards)"),
(49424, 0, 0, 1, 4, 0, 100, 1, 0, 0, 0, 0, 11, 83015, 0, 0, 0, 0, 0, 21, 25, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Aggro - Cast Spell 'Heroic Leap' (No Repeat)"),
(49424, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - On Aggro (Link) - Say Text Line 0 (No Repeat)"),
(49424, 0, 2, 0, 0, 0, 100, 1, 5000, 9000, 6000, 10000, 11, 33239, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Marshal Redpath - In Combat - Cast Spell 'Whirlwind'");

DELETE FROM `creature_text` WHERE `CreatureID` IN (49422, 49423, 49424);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(49422, 0, 0, "I'll kill you!", 12, 0, 15, 0, 0, 0, "Rotbrain Berserker", 50324),
(49422, 0, 1, "Look what they've done to me!", 12, 0, 15, 0, 0, 0, "Rotbrain Berserker", 50325),
(49422, 0, 2, "Who am I?", 12, 0, 15, 0, 0, 0, "Rotbrain Berserker", 50320),
(49422, 1, 0, "%s becomes enraged!", 16, 0, 100, 0, 0, 0, "Rotbrain Berserker", 7798),
(49423, 0, 0, "I'll kill you!", 12, 0, 15, 0, 0, 0, "Rotbrain Magus", 50324),
(49423, 0, 1, "Look what they've done to me!", 12, 0, 15, 0, 0, 0, "Rotbrain Magus", 50325),
(49423, 0, 2, "Who am I?", 12, 0, 15, 0, 0, 0, "Rotbrain Magus", 50320),
(49424, 0, 0, "BLEEAAAGGHHH! I'm a monster, don't look at me!", 14, 0, 100, 0, 0, 0, "Rotbrain Magus", 50326);

DELETE FROM `creature_sparring_template` WHERE `AttackerEntry` IN (49428, 49422, 49423, 49424);
INSERT INTO `creature_sparring_template` (`AttackerEntry`, `VictimEntry`, `HealthLimitPct`) VALUES
(49428, 49422, 75),
(49422, 49428, 75),
(49428, 49423, 75),
(49423, 49428, 75),
(49428, 49424, 99),
(49424, 49428, 75);

UPDATE `creature` SET `position_x` = 2306.052, `position_y` = 1370.719, `position_z` = 33.333, `orientation` = 3.9777 WHERE `guid` = 193587 AND `id` = 1934;
UPDATE `creature` SET `position_x` = 2336.662, `position_y` = 1389.882, `position_z` = 33.333, `orientation` = 1.4762 WHERE `guid` = 193879 AND `id` = 1934;
UPDATE `creature` SET `position_x` = 2307.501, `position_y` = 1444.389, `position_z` = 33.333, `orientation` = 5.0343 WHERE `guid` = 193586 AND `id` = 1934;
UPDATE `creature` SET `position_x` = 2283.967, `position_y` = 1414.648, `position_z` = 33.333, `orientation` = 0.0863 WHERE `guid` = 194076 AND `id` = 1934;
UPDATE `creature` SET `position_x` = 2286.474, `position_y` = 1392.534, `position_z` = 33.333, `orientation` = 4.5552 WHERE `guid` = 193874 AND `id` = 1934;
UPDATE `creature` SET `position_x` = 2283.378, `position_y` = 1178.46, `position_z` = 31.442, `orientation` = 2.4767 WHERE `guid` = 194060 AND `id` = 1934;

UPDATE `gameobject` SET `position_z` = 33.8887 WHERE `guid` = 14292 AND `id` = 153473;
UPDATE `creature` SET `position_x` = 2457.60003, `position_y` = 1277.307, `position_z` = 35.001, `orientation` = 5.4107 WHERE `guid` = 193793 AND `id` = 1547;
UPDATE `gameobject` SET `position_x` = 2247.0612, `position_y` = 741.997, `position_z` = 34.587 WHERE `guid` = 95670 AND `id` = 1617;
UPDATE `gameobject` SET `position_x` = 2235.9467, `position_y` = 734.772, `position_z` = 35.088 WHERE `guid` = 95671 AND `id` = 1617;
UPDATE `creature` SET `position_x` = 2314.4924, `position_y` = 1013.669, `position_z` = 40.146, `orientation` = 1.7227 WHERE `guid` = 193796 AND `id` = 1547;
UPDATE `gameobject` SET `position_x` = 2249.2399, `position_y` = 1247.213, `position_z` = 31.443 WHERE `guid` = 95719 AND `id` = 1617;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 1521 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(1521, 0, 0, 0, 1, 0, 100, 0, 60000, 60000, 180000, 180000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Gretchen Dedmar - OOC - Say Text Line 0");

DELETE FROM `creature_text` WHERE `CreatureID` = 1521;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(1521, 0, 0, "So cold...", 12, 0, 100, 24, 0, 0, "Gretchen Dedmar", 39007),
(1521, 0, 1, "The cold dark beckons...", 12, 0, 100, 24, 0, 0, "Gretchen Dedmar", 39008),
(1521, 0, 2, "Save me from the cold!", 12, 0, 100, 24, 0, 0, "Gretchen Dedmar", 39009);

UPDATE `creature` SET `spawndist` = 1, `MovementType` = 1 WHERE `guid` = 192642 AND `id` = 3549;
UPDATE `creature_template` SET `npcflag` = 0 WHERE `entry` = 7980;
