-- Fix Quest 29548 "The Mission": show the Stormwind Skyfire and send Rogers' gossip to Pandaria.
SET @STORMWIND_SKYFIRE_TERRAIN := 1066;
SET @WRONG_STORMWIND_SKYFIRE_TERRAIN := 1064;
SET @JADE_FOREST_SKYFIRE_TERRAIN := 971;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=66292;

DELETE FROM `terrain_swap_defaults` WHERE `MapId`=0 AND `TerrainSwapMap`=@WRONG_STORMWIND_SKYFIRE_TERRAIN;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=25 AND `SourceEntry`=@WRONG_STORMWIND_SKYFIRE_TERRAIN;

DELETE FROM `terrain_swap_defaults` WHERE `MapId`=0 AND `TerrainSwapMap`=@STORMWIND_SKYFIRE_TERRAIN;
INSERT INTO `terrain_swap_defaults` (`MapId`,`TerrainSwapMap`,`Comment`) VALUES
(0,@STORMWIND_SKYFIRE_TERRAIN,'StormwindGunshipPandariaStartArea');

DELETE FROM `terrain_swap_defaults` WHERE `MapId`=870 AND `TerrainSwapMap`=@JADE_FOREST_SKYFIRE_TERRAIN;
INSERT INTO `terrain_swap_defaults` (`MapId`,`TerrainSwapMap`,`Comment`) VALUES
(870,@JADE_FOREST_SKYFIRE_TERRAIN,'JadeForestSkyfire');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=25 AND `SourceEntry` IN (@STORMWIND_SKYFIRE_TERRAIN,@JADE_FOREST_SKYFIRE_TERRAIN);
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
 `ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,
 `ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(25,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,47,0,29548,8 | 2,0,0,0,0,'','Show Stormwind Skyfire terrain while The Mission is in progress'),
(25,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,47,0,29548,64,0,1,0,0,'','Hide Stormwind Skyfire terrain after The Mission is rewarded'),
(25,0,@JADE_FOREST_SKYFIRE_TERRAIN,0,0,47,0,29548,8 | 2 | 64,0,0,0,0,'','Show Jade Forest Skyfire terrain after The Mission starts'),
(25,0,@JADE_FOREST_SKYFIRE_TERRAIN,0,0,47,0,31733,64,0,1,0,0,'','Hide Jade Forest Skyfire terrain after Touching Ground is rewarded');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=66292 AND `SourceEntry`=0;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
 `ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,
 `ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(15,66292,0,0,0,22,0,0,0,0,0,0,0,'','Show Rogers departure gossip only in Stormwind'),
(15,66292,0,0,0,47,0,29548,8 | 2,0,0,0,0,'','Show Rogers departure gossip while The Mission is in progress');

DELETE FROM `creature` WHERE `guid` IN (280000662,280000663,280000664,280000667,280000668,280000669,280000673,280000674);
INSERT INTO `creature`
(`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnDifficulties`,`phaseUseFlags`,`PhaseId`,`PhaseGroup`,`terrainSwapMap`,
 `modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,
 `currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`unit_flags2`,`unit_flags3`,
 `dynamicflags`,`ScriptName`,`VerifiedBuild`)
VALUES
(280000662,66292,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8395.43,1376.62,135.508,0.00165176,300,0,0,26268,0,0,0,0,0,0,0,'',0),
(280000663,66300,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8389.87,1372.73,135.508,5.69337,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000664,66441,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8389.82,1380.67,135.508,0.57296,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000667,66563,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8402.18,1372.66,135.508,3.98795,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000668,66568,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8402.35,1380.66,135.508,2.36401,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000669,66569,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8395.62,1366.18,135.508,4.71399,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000673,66300,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8395.55,1387.21,135.508,1.57654,300,0,0,1,0,0,0,0,0,0,0,'',0),
(280000674,66441,0,1519,4411,'0',0,0,0,@STORMWIND_SKYFIRE_TERRAIN,0,0,-8406.68,1376.64,135.508,3.14515,300,0,0,1,0,0,0,0,0,0,0,'',0);

UPDATE `creature` SET `position_x`=-664.501, `position_y`=-1485.42, `position_z`=131.4, `orientation`=4.44604 WHERE `guid`=10646285 AND `id`=66292;

DELETE FROM `smart_scripts` WHERE `entryorguid`=66292 AND `source_type`=0;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,
 `event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param_string`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,
 `target_type`,`target_param1`,`target_param2`,`target_param3`,
 `target_x`,`target_y`,`target_z`,`target_o`,`comment`)
VALUES
(66292,0,0,1,62,0,100,0,66292,0,0,0,'',85,131057,0,0,0,0,0,7,0,0,0,0,0,0,0,'Sky Admiral Rogers - On Gossip Select - Give Quest Credit'),
(66292,0,1,2,61,0,100,0,0,0,0,0,'',68,119,0,0,0,0,0,7,0,0,0,0,0,0,0,'Sky Admiral Rogers - Link - Play Pandaria Alliance Intro'),
(66292,0,2,3,61,0,100,0,0,0,0,0,'',72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Sky Admiral Rogers - Link - Close Gossip'),
(66292,0,3,0,61,0,100,0,0,0,0,0,'',62,870,0,0,0,0,0,7,0,0,0,-664.501,-1485.42,131.4,4.44604,'Sky Admiral Rogers - Link - Teleport To Jade Forest Skyfire'),
(66292,0,4,0,20,0,100,0,29548,0,0,0,'',62,870,0,0,0,0,0,7,0,0,0,-664.501,-1485.42,131.4,4.44604,'Sky Admiral Rogers - On Quest Reward - Teleport To Jade Forest Skyfire');
