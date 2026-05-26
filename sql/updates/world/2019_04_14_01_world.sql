-- Eastern Kingdoms
-- Zone: Westfall
DELETE FROM `creature` WHERE `guid` = 214354 AND `id` = 42309;
DELETE FROM `creature_addon` WHERE `guid` = 214354;

DELETE FROM `creature_text` WHERE `CreatureID` IN (42387, 42308, 42309, 42745);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(42387, 0, 0, "Did you... Did you meet her?", 12, 0, 100, 0, 0, 0, 'Thug', 42439),
(42387, 1, 0, "Yep. She's for real.", 12, 0, 100, 0, 0, 0, 'Thug', 42440),
(42387, 2, 0, "She wanted me to tell you lugs that she appreciates the job that we did for her on the Furlbrows. Gave me a pile o' gold to split with you all.", 12, 0, 100, 0, 0, 0, 'Thug', 42441),
(42387, 3, 0, "See her face? Is it really...", 12, 0, 100, 0, 0, 0, 'Thug', 42442),
(42387, 4, 0, "Whoa, what do we have here? Looks like we have ourselves an eavesdropper, boys.", 12, 0, 100, 0, 0, 0, 'Thug', 42443),
(42387, 5, 0, "Only one thing to do with a lousy, good-for-nothin eavesdropper.", 12, 0, 100, 0, 0, 0, 'Thug', 42444),
(42387, 6, 0, "DIE!", 12, 0, 100, 0, 0, 0, 'Thug', 42445),
(42387, 7, 0, "Hurry back to the Furlbrow's Cottage!", 41, 0, 100, 0, 0, 0, 'Text Emote', 42446),

(42308, 0, 0, "No kidding, rookie...", 12, 0, 100, 273, 0, 0, 'Lieutenant Horatio Laine', 42227),
(42308, 1, 0, "Looks like they really put the cart...", 12, 0, 100, 1, 0, 0, 'Lieutenant Horatio Laine', 42228),
(42308, 2, 0, "...before the horse.", 12, 0, 100, 1, 0, 0, 'Lieutenant Horatio Laine', 42229),

(42309, 0, 0, "It's a bloodbath, lieutenant. They've been murdered.", 12, 0, 100, 1, 0, 0, 'Stormwind Investigator', 42224),
(42309, 1, 0, "Given the body temperature, I'd say they've been dead no more than 6 hours.", 12, 0, 100, 1, 0, 0, 'Stormwind Investigator', 42225),

(42745, 0, 0, "Damn shame what they did to Old Blanchy...", 12, 0, 100, 1, 0, 0, 'Stormwind Investigator', 42226);
