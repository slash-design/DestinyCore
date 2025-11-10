-- ENABLED ALLIED RACES
DELETE FROM `race_unlock_requirement`
WHERE `raceID` IN ('27', '28', '29', '30');

INSERT INTO `race_unlock_requirement` (`raceID`, `expansion`, `achievementId`) VALUES
('27', '6', '0'),
('28', '6', '0'),
('29', '6', '0'),
('30', '6', '0');
