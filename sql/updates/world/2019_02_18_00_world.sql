UPDATE `creature_template` SET `AIName` = "SmartAI", `InhabitType` = 4 WHERE `entry` = 39048;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 39048 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 39048*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(39048, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 80, 39048*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sylvanas Lamenter - On Summoned - Start Script'),
(39048*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 37090, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sylvanas Lamenter - On Script - Cast Lament of the Highborne: Highborne Aura'),
(39048*100, 9, 1, 0, 0, 0, 100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sylvanas Lamenter - On Script - Set Run Off"),
(39048*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 216, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sylvanas Lamenter - On Script - Rise up"),
(39048*100, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 4, 15095, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sylvanas Lamenter - On Script - Play Sound Lament of the Highborne');

UPDATE `creature_template` SET `VehicleId` = 732, `unit_flags` = 33554432, `InhabitType` = 4 WHERE `entry` = 40246;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (74890, -75731);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(74890, 75055, 0, "Instant Statue"),
(-75731, -75055, 0, "Instant Statue"),
(-75731, -74890, 0, "Instant Statue");
DELETE FROM `spell_custom_attr` WHERE `entry` = 75731;
INSERT INTO `spell_custom_attr` (`entry`, `attributes`) VALUES
(75731, 4096);

UPDATE `creature_template` SET `ScriptName` = "npc_puzzle_box_of_yogg_saron" WHERE `entry` = 49634;
DELETE FROM `creature_text` WHERE `CreatureID` = 49634;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES 
(49634, 0, 0, "At the bottom of the ocean even light must die.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49628),
(49634, 0, 1, "The silent, sleeping, staring houses in the backwoods always dream. It would be merciful to tear them down.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49614),
(49634, 0, 2, "There is no sharp distinction between the real and the unreal.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49606),
(49634, 0, 3, "Even death may die.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49619),
(49634, 0, 4, "There is a little lamb lost in dark woods.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49624),
(49634, 0, 5, "All places, all things have souls. All souls can be devoured.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49605),
(49634, 0, 6, "What can change the nature of a man?", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49634),
(49634, 0, 7, "The stars sweep chill currents that make men shiver in the dark.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49616),
(49634, 0, 8, "You will all be alone in the end.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49622),
(49634, 0, 9, "Do you dream while you sleep or is it an escape from the horrors of reality?", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49609),
(49634, 0, 10, "Look around. They will all betray you. Flee screaming into the black forest.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49627),
(49634, 0, 11, "In the land of Ny'alotha there is only sleep.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49608),
(49634, 0, 12, "In the sleeping city of Ny'alotha walk only mad things.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49611),
(49634, 0, 13, "Ny'alotha is a city of old, terrible, unnumbered crimes.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49618),
(49634, 0, 14, "Y'knath k'th'rygg k'yi mrr'ungha gr'mula.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49604),
(49634, 0, 15, "The void sucks at your soul. It is content to feast slowly.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49601),
(49634, 0, 16, "The drowned god's heart is black ice.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49623),
(49634, 0, 17, "It is standing right behind you. Do not move. Do not breathe.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49632),
(49634, 0, 18, "Have you had the dream again? A black goat with seven eyes that watches from the outside.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49626),
(49634, 0, 19, "In the sunken city, he lays dreaming.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49603),
(49634, 0, 20, "Open me! Open me! Open me! Then only will you know peace.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49629),
(49634, 0, 21, "You resist. You cling to your life as if it actually matters. You will learn.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49636),
(49634, 0, 22, "The tortured spirits of your ancestors cling to you, screaming in silence. Apparently they are quite numerous.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49633),
(49634, 0, 23, "The fish know all the secrets. They know the cold. They know the dark.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49631),
(49634, 0, 24, "The giant rook watches from the dead trees. Nothing breathes beneath his shadow.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 49625),
(49634, 0, 25, "Beneath the shadow of the darkened spire, there is no light, no mercy, only void, and the chaos within.", 15, 0, 100, 0, 0, 0, 'Puzzle Box of Yogg-Saron', 147278);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (160331, 187150, 187174, 192225);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(160331, 'spell_gen_blood_elf_illusion'),
(187150, 'spell_gen_jewel_of_hellfire_trigger'),
(187174, 'spell_gen_jewel_of_hellfire'),
(192225, 'spell_gen_coin_of_many_faces');

UPDATE `gameobject_template` SET `AIName` = "SmartGameObjectAI" WHERE `entry` = 190351;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 190351 AND `source_type` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(190351, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 85, 50493 , 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "D.I.S.C.O. - On Gossip Hello - Cast Listening to Music");
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-50314, -50493, 50493);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-50314, -50493, 0, 'Disco Ball'),
(-50493, -50314, 0, 'Disco Ball (Listening to Music)'),
(50493, 50314, 0, 'Disco Ball (Listening to Music)');
