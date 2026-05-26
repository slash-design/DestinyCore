-- Outland
-- Zone: Shadowmoon Valley

-- Painmistress Gabrissa SAI
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 83283 AND `id` = 21309;
UPDATE `creature_template` SET `AIName` = "SmartAI" WHERE `entry` = 21309;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 21309 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`= 2130900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(21309,0,0,0,0,0,100,0,5000,7000,20000,25000,11,38048,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - In Combat - Cast Spell 'Curse of Pain'"),
(21309,0,1,0,0,0,100,0,6000,7000,17000,20000,11,38169,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - In Combat - Cast Spell 'Subservience'"),
(21309,0,2,0,25,0,100,0,0,0,0,0,53,0,21309,1,0,0,2,0,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Reset - Start Waypoint"),
(21309,0,3,0,40,0,100,0,5,21309,0,0,80,2130900,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On WP 5 Reached - Start Script"),
(21309,0,4,5,40,0,100,0,9,21309,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On WP 9 Reached - Pause WP (20000 ms)"),
(21309,0,5,0,61,0,100,0,9,21309,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.37533,"Painmistress Gabrissa - On WP 9 Reached (Link) - Set Orientation (3.37533)"),
(21309,0,6,0,4,0,100,0,0,0,0,0,49,0,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Aggro - Start Attacking"),

(2130900,9,0,0,0,0,100,0,0,0,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Script - Pause WP (6000 ms)"),
(2130900,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.60702,"Painmistress Gabrissa - On Script - Set Orientation (3.60702)"),
(2130900,9,2,0,0,0,100,0,1000,1000,0,0,11,36578,0,0,0,0,0,10,122773,21310,0,0,0,0,0,"Painmistress Gabrissa - On Script - Cast 'Fool's Fortune'"),
(2130900,9,3,0,0,0,100,0,4000,4000,0,0,92,0,36578,1,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Script - Interrupt Spell 'Fool's Fortune'");

DELETE FROM `waypoints` WHERE `entry` = 21309;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(21309, 1, -3697.1, 2669.77,  107.186, 'Painmistress Gabrissa'),
(21309, 2, -3697.68, 2673.34, 108.569, 'Painmistress Gabrissa'),
(21309, 3, -3692.22, 2678.74, 108.66,  'Painmistress Gabrissa'),
(21309, 4, -3690.04, 2682.79, 108.752, 'Painmistress Gabrissa'),
(21309, 5, -3692.89, 2684.18, 108.485, 'Painmistress Gabrissa'),
(21309, 6, -3690.92, 2682.03, 108.744, 'Painmistress Gabrissa'),
(21309, 7, -3694.07, 2677.51, 108.537, 'Painmistress Gabrissa'),
(21309, 8, -3697.53, 2673.77, 108.581, 'Painmistress Gabrissa'),
(21309, 9, -3693.46, 2668.44, 107.174, 'Painmistress Gabrissa');

-- Lothros WP
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 83282 AND `id` = 21928;
DELETE FROM `creature_template_addon` WHERE `entry` = 21928;
DELETE FROM `creature_addon` WHERE `guid` = 83282;
INSERT INTO `creature_addon` (`guid`, `path_id`, `bytes2`) VALUES
(83282, 832820, 1);

DELETE FROM `waypoint_data` WHERE `id` = 832820;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(832820, 1, -3786.7, 2670.05, 101.234, 0, 0, 0, 0, 100, 0),
(832820, 2, -3780.58, 2686.94, 101.002, 0, 0, 0, 0, 100, 0),
(832820, 3, -3787.51, 2672.69, 101.089, 0, 0, 0, 0, 100, 0),
(832820, 4, -3799, 2670.99, 101.33, 0, 0, 0, 0, 100, 0),
(832820, 5, -3805.09, 2676, 100.937, 0, 0, 0, 0, 100, 0),
(832820, 6, -3821.13, 2697.37, 101.103, 0, 0, 0, 0, 100, 0),
(832820, 7, -3802.29, 2672.35, 101.077, 0, 0, 0, 0, 100, 0),
(832820, 8, -3793.76, 2669.05, 101.634, 0, 0, 0, 0, 100, 0),
(832820, 9, -3788.45, 2672.2, 101.092, 0, 0, 0, 0, 100, 0),
(832820, 10, -3778.92, 2687.8, 101.164, 0, 0, 0, 0, 100, 0),
(832820, 11, -3785.84, 2674.82, 100.917, 0, 0, 0, 0, 100, 0),
(832820, 12, -3783.46, 2669.85, 101.156, 0, 0, 0, 0, 100, 0),
(832820, 13, -3770.29, 2669.09, 100.84, 0, 0, 0, 0, 100, 0),
(832820, 14, -3718.1, 2675.63, 105.861, 0, 0, 0, 0, 100, 0),
(832820, 15, -3779.27, 2669.86, 101.105, 0, 0, 0, 0, 100, 0);
