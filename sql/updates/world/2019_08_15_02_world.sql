SET @CGUID = 447837; -- 83
SET @OGUID = 103338; -- 50
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+82;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 104535, 571, 394, 394, 1, 0, 0, 0, 0, 4227.63, -3802.89, 119.271, 4.83333, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Lea Stonepaw (Area: -Unknown- - Difficulty: 0)
(@CGUID+1, 104851, 571, 8085, 394, 1, 0, 0, 0, 0, 4187.6, -3871.86, 178.464, 5.37341, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+2, 104851, 571, 8085, 394, 1, 0, 0, 0, 0, 4139.25, -3677.24, 179.831, 3.29834, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+3, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 4022.1, -3842.5, 222.86, 0.0940309, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+4, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 4102.8, -3872.74, 217.753, 2.72192, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+5, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 4113.72, -3911.9, 173.402, 1.52762, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+6, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 4001.09, -3917.46, 197.533, 3.29834, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+7, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 3957.32, -3800.51, 150.406, 2.25958, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+8, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 3899.83, -3845.08, 179.275, 2.71263, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)
(@CGUID+9, 104851, 571, 395, 394, 1, 0, 0, 0, 0, 3897.84, -3866.11, 178.889, 1.7892, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dead Frostpaw Shaman (Area: -Unknown- - Difficulty: 0)

(@CGUID+14, 104385, 571, 8085, 394, 1, 0, 0, 0, 0, 4201.15, -3761.94, 123.532, 5.24168, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ursol (Area: -Unknown- - Difficulty: 0)
(@CGUID+15, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4226.54, -3766.85, 120.777, 3.28092, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+16, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4199.62, -3795.98, 119.971, 1.46483, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+17, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4230.49, -3781.02, 119.772, 3.02567, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+18, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4188.57, -3786.35, 123.167, 0.495645, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+19, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4219.37, -3793.39, 119.442, 2.2442, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+20, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4184.76, -3765.83, 126.547, 6.23714, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+21, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4184.56, -3751.72, 127.383, 5.56564, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+22, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4209.73, -3747.04, 125.19, 4.29097, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+23, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4197.57, -3742.62, 125.382, 4.92713, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+24, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4226.54, -3766.85, 120.777, 3.28092, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+25, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4230.49, -3781.02, 119.772, 3.02567, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+26, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4199.62, -3795.98, 119.971, 1.46483, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+27, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4188.57, -3786.35, 123.167, 0.495645, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+28, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4184.56, -3751.72, 127.383, 5.56564, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+29, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4219.37, -3793.39, 119.442, 2.2442, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+30, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4184.76, -3765.83, 126.547, 6.23714, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+31, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4209.73, -3747.04, 125.19, 4.29097, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+32, 104936, 571, 8085, 394, 1, 6069, 0, 0, 0, 4197.57, -3742.62, 125.382, 4.92713, 120, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ancestral Warrior (Area: -Unknown- - Difficulty: 0) (Auras: 207266 - -Unknown-)
(@CGUID+33, 101386, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12394.9, -13121.9, 321.666, 3.30109, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Defiler (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+34, 101386, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12459.4, -13067.8, 339.22, 0.913479, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Defiler (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+35, 101386, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12393, -13106.8, 321.796, 3.22648, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Defiler (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+36, 105243, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12412, -12977.4, 318.158, 1.5677, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Lea Stonepaw (Area: -Unknown- - Difficulty: 0) (Auras: 186588 - -Unknown-)
(@CGUID+37, 101386, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12411.3, -13010.3, 320.769, 1.27476, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Defiler (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+38, 59113, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12286.4, -13101.3, 328.786, 3.34025, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Generic Bunny (Area: -Unknown- - Difficulty: 0)
(@CGUID+39, 105294, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12192, -13090.8, 328.359, 0.366053, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+40, 101362, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12174.7, -13084.8, 329.989, 3.4423, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spirit of Ursoc (Area: -Unknown- - Difficulty: 12) (Auras: 208642 - -Unknown-)
(@CGUID+41, 105331, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12174.6, -13118.8, 333.618, 1.05888, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Claws of Ursoc (Area: -Unknown- - Difficulty: 12)
(@CGUID+42, 101742, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12194.4, -13118.4, 328.111, 6.09662, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Lea Stonepaw (Area: -Unknown- - Difficulty: 12) (Auras: 208707 - -Unknown-)
(@CGUID+43, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12216.2, -13094.6, 326.501, 5.53722, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+44, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12205.6, -13093.1, 327.139, 0.235779, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+45, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12204, -13121.8, 335.329, 0.215165, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+46, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12203.3, -13108.5, 329.291, 5.95292, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+47, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12203.9, -13140.9, 329.291, 6.15803, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+48, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12201, -13127.9, 329.291, 6.15803, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+49, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12223.6, -13117.9, 325.881, 0.388932, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+50, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12194.4, -13119.5, 329.291, 5.95292, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+51, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12193.2, -13096.3, 329.333, 6.02198, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+52, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12210, -13080.7, 327.475, 5.14374, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+53, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12213.5, -13132.7, 329.291, 6.15803, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+54, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12210.3, -13112.8, 329.291, 5.95292, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+55, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12245.2, -13102.7, 329.265, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+56, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12213.5, -13124.2, 329.455, 0.624552, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+57, 101393, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12194.2, -13125.2, 328.366, 2.98783, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Mylune (Area: -Unknown- - Difficulty: 12) (Auras: 208635 - -Unknown-)
(@CGUID+58, 101754, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12192.9, -13109.2, 328.114, 2.90615, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Verdant Warder (Area: -Unknown- - Difficulty: 12) (Auras: 208648 - -Unknown-)
(@CGUID+59, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12216.2, -13094.6, 326.501, 5.53722, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+60, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12205.6, -13093.1, 327.139, 0.235779, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+61, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12204, -13121.8, 335.329, 0.215165, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+62, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12203.3, -13108.5, 329.291, 5.95292, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+63, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12203.9, -13140.9, 329.291, 6.15803, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+64, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12224.8, -13092.7, 326.7, 5.70283, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+65, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12223.6, -13117.9, 325.881, 0.388932, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+66, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12214, -13129, 326.56, 0.872626, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+67, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12224.8, -13092.7, 326.7, 5.70283, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+68, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12214, -13129, 326.56, 0.872626, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+69, 101389, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12194.7, -13140.3, 328.84, 1.68168, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Corrupted Defender (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+70, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12205.6, -13093.1, 327.139, 0.235779, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+71, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12223.6, -13117.9, 325.881, 0.388932, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+72, 101384, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12229.3, -13093.3, 326.814, 6.11839, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Blightborn Sludge (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+73, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12224.8, -13092.7, 326.7, 5.70283, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+74, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12214, -13129, 326.56, 0.872626, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+75, 101388, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12229.3, -13093.3, 326.814, 6.11839, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rothoof Shadowstalker (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+76, 101389, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12212.2, -13146.8, 327.373, 0.958324, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Corrupted Defender (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+77, 101389, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12230, -13123.3, 325.96, 0.3465, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Corrupted Defender (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+78, 101389, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12229.3, -13093.3, 326.814, 6.11839, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Corrupted Defender (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+79, 101389, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12194.7, -13140.3, 328.84, 1.68168, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Corrupted Defender (Area: -Unknown- - Difficulty: 12) (Auras: 208637 - -Unknown-)
(@CGUID+80, 101403, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12215.6, -13097.5, 326.383, 0.351951, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Shade of Xavius (Area: -Unknown- - Difficulty: 12) (Auras: 208728 - -Unknown-)
(@CGUID+81, 101390, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12219.9, -13106.9, 326.054, 0.336237, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Arch-Desecrator Malithar (Area: -Unknown- - Difficulty: 12) (Auras: 208720 - -Unknown-)
(@CGUID+82, 105331, 1536, 7974, 7974, 4096, 0, 0, 0, 0, -12174.6, -13118.8, 333.618, 1.05888, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Claws of Ursoc (Area: -Unknown- - Difficulty: 12)

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+82;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES
(@CGUID+0, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Lea Stonepaw
(@CGUID+1, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+2, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+3, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+4, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+5, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+6, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+7, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+8, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman
(@CGUID+9, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dead Frostpaw Shaman

(@CGUID+14, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ursol
(@CGUID+15, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+16, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+17, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+18, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+19, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+20, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+21, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+22, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+23, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+24, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+25, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+26, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+27, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+28, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+29, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+30, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+31, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+32, 0, 0, 0, 1, 0, 0, 0, 0, "207266"), -- Ancestral Warrior - 207266 - -Unknown-
(@CGUID+33, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Defiler
(@CGUID+34, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Defiler
(@CGUID+35, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Defiler
(@CGUID+36, 0, 0, 0, 1, 0, 0, 0, 0, "186588"), -- Lea Stonepaw - 186588 - -Unknown-
(@CGUID+37, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Defiler
(@CGUID+38, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Generic Bunny
(@CGUID+39, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Shadowstalker
(@CGUID+40, 0, 0, 0, 1, 0, 0, 0, 0, "208642"), -- Spirit of Ursoc - 208642 - -Unknown-
(@CGUID+41, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Claws of Ursoc
(@CGUID+42, 0, 0, 0, 257, 0, 0, 0, 0, "208707"), -- Lea Stonepaw - 208707 - -Unknown-
(@CGUID+43, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+44, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+45, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+46, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+47, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+48, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+49, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+50, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+51, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+52, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+53, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+54, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+55, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Shadowstalker
(@CGUID+56, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+57, 0, 0, 0, 1, 0, 0, 0, 0, "208635"), -- Mylune - 208635 - -Unknown-
(@CGUID+58, 0, 0, 0, 1, 0, 0, 0, 0, "208648"), -- Verdant Warder - 208648 - -Unknown-
(@CGUID+59, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+60, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+61, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+62, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Blightborn Sludge
(@CGUID+63, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+64, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+65, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+66, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Rothoof Shadowstalker
(@CGUID+67, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+68, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+69, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Corrupted Defender - 208637 - -Unknown-
(@CGUID+70, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+71, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+72, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Blightborn Sludge - 208637 - -Unknown-
(@CGUID+73, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+74, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+75, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Rothoof Shadowstalker - 208637 - -Unknown-
(@CGUID+76, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Corrupted Defender - 208637 - -Unknown-
(@CGUID+77, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Corrupted Defender
(@CGUID+78, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Corrupted Defender
(@CGUID+79, 0, 0, 0, 1, 0, 0, 0, 0, "208637"), -- Corrupted Defender - 208637 - -Unknown-
(@CGUID+80, 0, 0, 0, 1, 0, 0, 0, 0, "208728"), -- Shade of Xavius - 208728 - -Unknown-
(@CGUID+81, 0, 0, 0, 1, 0, 0, 0, 0, "208720"), -- Arch-Desecrator Malithar - 208720 - -Unknown-
(@CGUID+82, 0, 0, 0, 1, 0, 0, 0, 0, ""); -- Claws of Ursoc

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+49;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 248583, 1220, 7846, 7558, 1, 0, 0, 4124.14, 7475.27, 50.165, 2.27911, 0, 0, 0.9084473, 0.4179995, 7200, 255, 1, 26124), -- Flask of Moonwell Water (Area: -Unknown- - Difficulty: 1)
(@OGUID+1, 248583, 1220, 7846, 7558, 1, 0, 0, 4126.43, 7473.34, 49.7535, 2.50294, 0, 0, 0.9494467, 0.3139284, 7200, 255, 1, 26124), -- Flask of Moonwell Water (Area: -Unknown- - Difficulty: 1)
(@OGUID+2, 248583, 1220, 7846, 7558, 1, 0, 0, 4128.81, 7471.65, 50.2382, 0.339173, 0, 0, 0.1687746, 0.9856547, 7200, 255, 1, 26124), -- Flask of Moonwell Water (Area: -Unknown- - Difficulty: 1)
(@OGUID+3, 188691, 571, 394, 394, 1, 0, 0, 4223.33, -3729.33, 123.555, 2.18166, 0, 0, 0.8870096, 0.4617509, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+4, 248057, 571, 394, 394, 1, 0, 0, 4235.03, -3813.52, 118.004, 2.16138, 0, 0, 0.8822823, 0.4707207, 120, 255, 1, 26124), -- Bear Totem (Area: -Unknown- - Difficulty: 0)
(@OGUID+5, 188691, 571, 394, 394, 1, 0, 0, 4224.08, -3781.8, 120.179, 2.40855, 0, 0, 0.9335794, 0.3583705, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+6, 248376, 571, 394, 394, 1, 0, 0, 4226.61, -3803, 119.913, 3.74285, 0, 0, -0.9551506, 0.2961204, 120, 255, 1, 26124), -- Spell Focus (Area: -Unknown- - Difficulty: 0)
(@OGUID+7, 188691, 571, 394, 394, 1, 0, 0, 4229, -3805.32, 119.108, 3.08918, 0, 0, 0.9996567, 0.02620165, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+8, 248066, 571, 394, 394, 1, 0, 0, 4228.55, -3805.61, 119.17, 3.74285, 0, 0, -0.9551506, 0.2961204, 120, 255, 1, 26124), -- Stone Monolith (Area: -Unknown- - Difficulty: 0)
(@OGUID+9, 188691, 571, 394, 394, 1, 0, 0, 4197.03, -3797.06, 120.007, 5.5676, 0, 0, -0.3502073, 0.9366722, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+10, 248056, 571, 394, 394, 1, 0, 0, 4174.44, -3753.49, 129.303, 2.69924, 0, 0, 0.9756393, 0.2193807, 120, 255, 1, 26124), -- Bear Totem (Area: -Unknown- - Difficulty: 0)
(@OGUID+11, 188691, 571, 394, 394, 1, 0, 0, 4186.33, -3782.27, 124.754, 6.230826, 0, 0, -0.02617645, 0.9996573, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+12, 248056, 571, 394, 394, 1, 0, 0, 4171.1, -3729.36, 132.264, 3.83447, 0, 0, -0.940587, 0.3395527, 120, 255, 1, 26124), -- Bear Totem (Area: -Unknown- - Difficulty: 0)
(@OGUID+13, 188691, 571, 394, 394, 1, 0, 0, 4199.32, -3858.68, 179.16, 4.398235, 0, 0, -0.8090153, 0.5877876, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+14, 188691, 571, 394, 394, 1, 0, 0, 4148.61, -3838.19, 181.82, 2.91469, 0, 0, 0.9935713, 0.1132084, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+15, 188691, 571, 395, 394, 1, 0, 0, 4210.81, -3882.56, 178.463, 0.471238, 0, 0, 0.2334442, 0.9723702, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)

(@OGUID+18, 188691, 571, 394, 394, 1, 0, 0, 4181.22, -3888.27, 178.762, 5.811947, 0, 0, -0.2334442, 0.9723702, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+19, 248056, 571, 395, 394, 1, 0, 0, 4111.46, -3756.94, 145.324, 3.04211, 0, 0, 0.9987631, 0.04972204, 120, 255, 1, 26124), -- Bear Totem (Area: -Unknown- - Difficulty: 0)
(@OGUID+20, 248056, 571, 395, 394, 1, 0, 0, 4106.3, -3742.42, 145.049, 3.83542, 0, 0, -0.9404259, 0.3399988, 120, 255, 1, 26124), -- Bear Totem (Area: -Unknown- - Difficulty: 0)
(@OGUID+21, 188691, 571, 395, 394, 1, 0, 0, 4094.76, -3772.1, 24.0171, 0.698132, 0, 0, 0.34202, 0.9396927, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+22, 188691, 571, 395, 394, 1, 0, 0, 4083.96, -3738.93, 22.1431, 5.166175, 0, 0, -0.5299187, 0.8480484, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+23, 188691, 571, 395, 394, 1, 0, 0, 4078.99, -3785.29, 223.338, 3.630296, 0, 0, -0.970294, 0.2419288, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+24, 190014, 571, 395, 394, 1, 0, 0, 4112.97, -3675.15, 179.141, 0.34034, 0, 0, 0.1693497, 0.985556, 120, 100, 1, 26124), -- Bonfire (Area: -Unknown- - Difficulty: 0)
(@OGUID+25, 2066, 571, 395, 394, 1, 0, 0, 4112.97, -3675.15, 179.141, 0.34034, 0, 0, 0.1693497, 0.985556, 120, 0, 1, 26124), -- Bonfire Damage (Area: -Unknown- - Difficulty: 0)
(@OGUID+26, 188691, 571, 395, 394, 1, 0, 0, 4086.05, -3816.69, 34.5203, 3.08918, 0, 0, 0.9996567, 0.02620165, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+27, 188691, 571, 395, 394, 1, 0, 0, 4082.2, -3817.28, 35.3875, 3.263776, 0, 0, -0.9981337, 0.06106737, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+28, 188691, 571, 395, 394, 1, 0, 0, 4048.56, -3722.74, 25.3847, 4.206245, 0, 0, -0.8616285, 0.5075394, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+29, 188691, 571, 395, 394, 1, 0, 0, 4052.26, -3776.41, 1.97152, 0.401425, 0, 0, 0.1993666, 0.979925, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+30, 188691, 571, 395, 394, 1, 0, 0, 4064.06, -3801.81, 14.0096, 5.166175, 0, 0, -0.5299187, 0.8480484, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+31, 188691, 571, 395, 394, 1, 0, 0, 4012.25, -3750.74, 1.64791, 0.645772, 0, 0, 0.3173046, 0.9483237, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+32, 188691, 571, 395, 394, 1, 0, 0, 4007.56, -3715.48, 222.234, 6.126106, 0, 0, -0.07845879, 0.9969174, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+33, 300201, 571, 395, 394, 1, 0, 0, 4023.4, -3773.88, 115.682, 4.3528, 0, 0, -0.8221579, 0.5692596, 120, 0, 1, 26124), -- TEMP Vordrassil's Sapling (Area: -Unknown- - Difficulty: 0)
(@OGUID+34, 188691, 571, 395, 394, 1, 0, 0, 3972.38, -3734.41, 37.0013, 6.021388, 0, 0, -0.1305246, 0.9914451, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+35, 188691, 571, 395, 394, 1, 0, 0, 4063.88, -3836.23, 40.5776, 1.93731, 0, 0, 0.8241243, 0.5664089, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+36, 188691, 571, 395, 394, 1, 0, 0, 4025.99, -3826.5, 11.2314, 3.735005, 0, 0, -0.9563046, 0.2923723, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+37, 188691, 571, 395, 394, 1, 0, 0, 3960.46, -3757.11, 39.5703, 4.415686, 0, 0, -0.8038559, 0.5948241, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+38, 188691, 571, 395, 394, 1, 0, 0, 3958.02, -3817.92, 39.7661, 4.904376, 0, 0, -0.6360779, 0.7716249, 120, 100, 1, 26124), -- Vordrassil's Seed (Area: -Unknown- - Difficulty: 0)
(@OGUID+39, 248824, 1536, 7974, 7974, 4096, 0, 0, -12167, -13112.9, 328.608, 5.75154, 0, 0, -0.2627029, 0.9648768, 7200, 255, 1, 26124), -- Stone (Area: -Unknown- - Difficulty: 12)
(@OGUID+40, 248824, 1536, 7974, 7974, 4096, 0, 0, -12182.1, -13126.2, 328.608, 0.685053, 0, 0, 0.3358679, 0.9419091, 7200, 255, 1, 26124), -- Stone (Area: -Unknown- - Difficulty: 12)
(@OGUID+41, 248823, 1536, 7974, 7974, 4096, 0, 0, -12179.4, -13130.4, 328.608, 4.57938, 0, 0, -0.7525339, 0.6585535, 7200, 255, 1, 26124), -- Ancient Barrier (Area: -Unknown- - Difficulty: 12)
(@OGUID+42, 248823, 1536, 7974, 7974, 4096, 0, 0, -12184.2, -13123.2, 328.608, 6.21777, 0, 0, -0.03270149, 0.9994652, 7200, 255, 1, 26124), -- Ancient Barrier (Area: -Unknown- - Difficulty: 12)
(@OGUID+43, 248824, 1536, 7974, 7974, 4096, 0, 0, -12180.8, -13111.2, 328.608, 1.94985, 0, 0, 0.8276596, 0.5612304, 7200, 255, 1, 26124), -- Stone (Area: -Unknown- - Difficulty: 12)
(@OGUID+44, 246040, 1536, 7974, 7974, 4096, 0, 0, -12175.6, -13118.3, 330.027, 6.24611, 0, 0, -0.01853657, 0.9998282, 7200, 255, 1, 26124), -- Aura Yellow Huge (1.15) (Area: -Unknown- - Difficulty: 12)
(@OGUID+45, 248825, 1536, 7974, 7974, 4096, 0, 0, -12166.4, -13121.6, 328.608, 6.00795, 0, 0, -0.1371832, 0.9905457, 7200, 255, 1, 26124), -- Stone (Area: -Unknown- - Difficulty: 12)
(@OGUID+46, 248824, 1536, 7974, 7974, 4096, 0, 0, -12169, -13127.5, 328.608, 3.54047, 0, 0, -0.9801779, 0.1981195, 7200, 255, 1, 26124), -- Stone (Area: -Unknown- - Difficulty: 12)
(@OGUID+47, 248827, 1536, 7974, 7974, 4096, 0, 0, -12175.3, -13119.1, 328.608, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Cylinder Collision (Area: -Unknown- - Difficulty: 12)
(@OGUID+48, 248823, 1536, 7974, 7974, 4096, 0, 0, -12168.9, -13109.3, 328.608, 1.28258, 0, 0, 0.5982294, 0.8013248, 7200, 255, 1, 26124), -- Ancient Barrier (Area: -Unknown- - Difficulty: 12)
(@OGUID+49, 248853, 1536, 7974, 7974, 4096, 0, 0, -12174.6, -13118.8, 329.647, 2.91085, 0, 0, 0.9933519, 0.1151172, 7200, 100, 1, 26124); -- Claws of Ursoc (Area: -Unknown- - Difficulty: 12)

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+49;
