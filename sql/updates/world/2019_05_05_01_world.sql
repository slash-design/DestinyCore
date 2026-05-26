UPDATE `game_event_creature` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (132028, 132025, 132024, 132027, 132026, 132029, 132001, 132002, 132003, 132004);

UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88182, 88176, 88196, 88178, 88174, 88175, 88177, 88184, 88189, 88180, 88195, 88179);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88445, 88444, 88183, 88188, 88181, 88190, 88191, 88185, 88193, 88505, 88504, 88448);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88481, 88192, 88186, 88194, 88469, 88468, 88485, 88486, 88449, 88450, 88451, 88452);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88470, 88476, 88446, 88447, 88503, 88502, 88501, 88500, 88499, 88453, 88498, 88454);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88455, 88456, 88457, 88458, 88459, 88460, 88461, 88462, 88463, 88464, 88465, 88466);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88467, 88483, 88482, 88484, 88487, 88488, 88489, 88490, 88492, 88493, 88494, 88495);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` IN (88496, 88497, 88491, 88187, 88477, 88471, 88479, 88473, 88472, 88478, 88480, 88474);
UPDATE `game_event_gameobject` SET `eventEntry` = 26 WHERE `eventEntry` = 41 AND `guid` = 88475;

DELETE FROM `game_event_gameobject` WHERE `guid` IN (88216, 88217, 88218, 88197, 88219, 88198, 88199, 88215, 88214, 88213, 88212, 88201, 88211, 88210, 88209, 88208, 88207, 88206, 88205, 88204, 88203, 88202, 88200) AND eventEntry = 41;

DELETE FROM `creature` WHERE `guid` IN (132011, 132010, 132009, 132008) AND `id` = 34823;
DELETE FROM `creature` WHERE `guid` IN (132023, 132021, 132019, 132017) AND `id` = 34824;
DELETE FROM `creature` WHERE `guid` IN (132006, 132007) AND `id` = 34812;
DELETE FROM `creature` WHERE `guid` IN (132013, 132014, 132015, 132012) AND `id` = 34819;
DELETE FROM `creature` WHERE `guid` IN (132016, 132022, 132020) AND `id` = 34822;

UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` = 25535;
DELETE FROM `creature` WHERE `guid` IN (353293, 353298, 353290, 370205, 370206);

DELETE FROM `game_event_creature` WHERE `guid` IN (353303, 370320, 370261, 353288, 370258, 353293, 353298, 353290, 353289, 370205, 370206) AND `eventEntry` = 1;
INSERT INTO `game_event_creature` (`eventEntry`, `guid`) VALUES
(1, 353303),
(1, 370320),
(1, 370261),
(1, 353288),
(1, 370258),
(1, 353289);

DELETE FROM `game_event_gameobject` WHERE `guid` IN (20373621, 20373673) AND `eventEntry` = 3;
INSERT INTO `game_event_gameobject` (`eventEntry`, `guid`) VALUES
(3, 20373621),
(3, 20373673);
