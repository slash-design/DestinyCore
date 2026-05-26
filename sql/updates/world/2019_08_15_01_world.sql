SET @CGUID = 450163; -- 171
SET @OGUID = 104436; -- 55
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+170;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 97504, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3602.512, 5946.425, 297.362, 0.394358, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Wraithtalon (Area: -Unknown- - Difficulty: 0)
(@CGUID+1, 107535, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3467.6, 5577.44, 319.539, 0.67738, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+2, 107548, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3490.48, 5680.98, 309.559, 4.95402, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+3, 107389, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3473.12, 5683.46, 308.579, 1.46073, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+4, 107548, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3434.97, 5646.15, 310.543, 4.97082, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+5, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3441.18, 5698.06, 310.885, 3.38092, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+6, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3448.59, 5649.42, 309.339, 5.08829, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+7, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3358.23, 5624.63, 313.39, 5.02064, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+8, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3631.607, 5839.101, 228.8943, 3.580315, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+9, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3466.1, 5523.07, 317.052, 5.84637, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+10, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3384.58, 5653.3, 312.288, 1.32173, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+11, 107391, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3395.84, 5589.59, 312.665, 1.66155, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+12, 107520, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3399.29, 5574.33, 310.987, 4.17134, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Verstok Darkbough (Area: -Unknown- - Difficulty: 0)
(@CGUID+13, 120341, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3730.41, 5874.91, 347.308, 1.95224, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Trickster (Area: -Unknown- - Difficulty: 0)
(@CGUID+14, 107568, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3475.85, 5618.17, 338.473, 2.45415, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Skyterror (Area: -Unknown- - Difficulty: 0)
(@CGUID+15, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3398.82, 5569.52, 311.202, 1.30076, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+16, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3491.38, 5674.63, 308.021, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+17, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3404.19, 5666.47, 313.285, 1.73909, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+18, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3403.88, 5671.96, 312.767, 4.68756, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+19, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3451.77, 5645.42, 309.286, 2.4467, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+20, 107392, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3398.24, 5571.24, 311.1, 3.19521, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Delandros Shimmermoon (Area: -Unknown- - Difficulty: 0)
(@CGUID+21, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3458.33, 5572.03, 318.206, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+22, 107391, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3409.95, 5570.2, 312.568, 5.79366, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+23, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3466.3, 5517.26, 316.345, 0.545567, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+24, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3511.733, 5930.4, 287.1431, 1.126014, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+25, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3524.89, 5893.79, 269.772, 5.45207, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+26, 107388, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3396.51, 5570.83, 311.059, 1.76556, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+27, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3474.69, 5688.33, 309.085, 4.10232, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+28, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3493.71, 5680.85, 309.738, 4.41899, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+29, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3472.73, 5574.31, 320.071, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+30, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3472.18, 5521.39, 319.522, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+31, 107568, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3424.04, 5622.98, 314.557, 5.2764, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 26124), -- Felflame Skyterror (Area: -Unknown- - Difficulty: 0) (possible waypoints or random movement)
(@CGUID+32, 107566, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3390.69, 5577.07, 310.492, 5.21978, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Wrathblade Invader (Area: -Unknown- - Difficulty: 0)
(@CGUID+33, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3436.79, 5695.45, 310.825, 0.739331, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+34, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3420.26, 5633.5, 310.176, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+35, 120341, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3732.65, 5820.7, 349.555, 3.29587, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Trickster (Area: -Unknown- - Difficulty: 0)
(@CGUID+36, 107390, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3397.51, 5581.92, 311.252, 1.11789, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+37, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3399.77, 5673.06, 312.307, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+38, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3386.09, 5662.42, 312.367, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+39, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3389.86, 5652.5, 313.345, 2.00916, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+40, 107568, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3522.24, 5636.79, 321.86, 4.48427, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Skyterror (Area: -Unknown- - Difficulty: 0)
(@CGUID+41, 107512, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3383.64, 5589.63, 310.771, 5.41906, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Dreamgrove Hippogryph (Area: -Unknown- - Difficulty: 0)
(@CGUID+42, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3437.71, 5588.23, 310.884, 1.20296, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+43, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3465.5, 5519.89, 316.421, 0.0982211, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+44, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3355.99, 5622.31, 314.028, 6.19906, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+45, 107708, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3477.62, 5541.45, 323.378, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Little Bunny (Area: -Unknown- - Difficulty: 0)
(@CGUID+46, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3393.71, 5654.87, 314.014, 2.37459, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+47, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3507.483, 5930.625, 284.3717, 1.840144, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+48, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3521.18, 5892.91, 270.129, 4.10747, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+49, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3487.99, 5679.82, 308.834, 5.40137, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+50, 107551, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3440.87, 5691.24, 310.732, 2.55716, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Wrathblade Invader (Area: -Unknown- - Difficulty: 0)
(@CGUID+51, 107390, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3406.32, 5566.58, 312.275, 5.79366, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+52, 97338, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3592.27, 5975.2, 263.953, 4.20555, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Terrorvine Lasher (Area: -Unknown- - Difficulty: 0)
(@CGUID+53, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3433, 5643.59, 310.379, 6.03566, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+54, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3440.88, 5588.84, 310.476, 2.02996, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0)
(@CGUID+55, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3508.47, 5670.87, 310.782, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+56, 107551, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3520.65, 5496.23, 325.289, 1.02198, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Wrathblade Invader (Area: -Unknown- - Difficulty: 0)
(@CGUID+57, 107551, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3509.77, 5682.41, 309.364, 5.54668, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Wrathblade Invader (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+58, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3468.58, 5717.62, 309.217, 1.22377, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+59, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3465.49, 5720.75, 309.229, 0.144827, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+60, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3454.32, 5727.83, 309.217, 1.82605, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+61, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3454.06, 5732.91, 311.656, 4.763526, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+62, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3470.34, 5723.52, 309.598, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+63, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3517.93, 5691.38, 311.632, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+64, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3530.49, 5682.17, 311.757, 1.172102, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+65, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3532.47, 5686.87, 313.109, 4.51741, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+66, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3552.26, 5654.06, 312.964, 2.46867, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+67, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3549, 5657.98, 312.643, 5.406134, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+68, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3567.95, 5653.6, 313.783, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+69, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3557.92, 5696.96, 317.453, 1.68393, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+70, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3557.14, 5702.92, 317.603, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+71, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3562.54, 5698.42, 318.143, 2.70592, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+72, 107548, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3562.9, 5701.38, 318.382, 3.02702, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Felflame Minion (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+73, 107535, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3582.53, 5683.63, 318.426, 1.303021, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Eredar Soul Lasher (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+74, 107389, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3583.88, 5688.55, 319.304, 4.65, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+75, 107480, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3556.25, 5538.02, 326.526, 3.14979, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fangs of Ashamane (Area: -Unknown- - Difficulty: 0)
(@CGUID+76, 95071, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3212.75, 6148.94, 238.235, 1.08668, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rotwood Sapling (Area: -Unknown- - Difficulty: 0)
(@CGUID+77, 95071, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3212.58, 6154, 238.027, 6.22769, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rotwood Sapling (Area: -Unknown- - Difficulty: 0)
(@CGUID+78, 95071, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3213.83, 6159.51, 237.928, 5.18623, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Rotwood Sapling (Area: -Unknown- - Difficulty: 0)
(@CGUID+79, 107523, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3644.23, 5660.67, 330.124, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Legion Flames (Area: -Unknown- - Difficulty: 0) (Auras: 213382 - -Unknown-)
(@CGUID+80, 120321, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3673.13, 5772.67, 352.233, 3.54598, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vile Soulwarden (Area: -Unknown- - Difficulty: 0)
(@CGUID+81, 106467, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3673.85, 5774.283, 352.9125, 6.263169, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vale Bear (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+82, 106467, 1220,  7558, 8164, 0, 0, 0, 0, 0, 3692.54, 5764.51, 356.985, 1.52961, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vale Bear (Area: -Unknown- - Difficulty: 0) (Auras: 145953 - -Unknown-)
(@CGUID+83, 107593, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3531.19, 5538.57, 323.496, 3.10763, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Algromon (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+84, 107391, 1220, 7558, 8164, 0, '6511', 0, 0, 0, 3482.09, 5538.6, 323.406, 6.21463, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+85, 107391, 1220, 7558,8164, 0, '6511', 0, 0, 0, 3455.99, 5541.02, 312.994, 2.96981, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+86, 107391, 1220, 7558, 8164, 0, '6511', 0, 0, 0, 3476.73, 5537.99, 323.378, 2.89427, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+87, 107390, 1220, 7558, 8164, 0, '6511', 0, 0, 0, 3456.6, 5546.12, 313.312, 3.04608, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+88, 107392, 1220, 7558, 8164, 0, '6511', 0, 0, 0, 3553.17, 5537.87, 324.879, 3.16424, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Delandros Shimmermoon (Area: -Unknown- - Difficulty: 0)
(@CGUID+89, 107390, 1220, 7558, 8164, 0, '6511', 0, 0, 0, 3476.91, 5545.04, 323.378, 3.04608, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ashen Druid (Area: -Unknown- - Difficulty: 0)
(@CGUID+90, 107729, 1220, 7558, 8164, 0, 0, 0, 0, 0, 3553.56, 5542.58, 324.707, 3.63773, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ebonfang (Area: -Unknown- - Difficulty: 0)


(@CGUID+93, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2103.51, 5433.3, 47.3837, 1.21731, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+94, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2036.68, 5386.83, 47.3837, 0.345293, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+95, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1960.76, 5509.95, 53.4083, 4.52589, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+96, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1995.05, 5440.04, 45.3844, 1.98558, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+97, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2113.91, 5404.29, 48.9512, 3.90148, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+98, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2040.33, 5406.62, 47.3837, 2.95467, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+99, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2032.84, 5447.72, 48.0245, 3.79526, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+100, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2015.5, 5364.25, 45.467, 5.99107, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+101, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1945.5, 5531.77, 55.3492, 2.7021, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+102, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1977.88, 5442.35, 45.467, 1.03804, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+103, 108197, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2083.74, 5384.62, 47.4254, 0.183137, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Leyhunter (Area: -Unknown- - Difficulty: 0) (Auras: 214175 - -Unknown-)
(@CGUID+104, 107759, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1937.84, 5566.95, 60.8955, 4.85239, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ebonfang (Area: -Unknown- - Difficulty: 0)
(@CGUID+105, 107791, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2126.44, 5388.03, 52.5094, 6.01743, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Door (Area: -Unknown- - Difficulty: 0)
(@CGUID+106, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2009.29, 5449.15, 45.467, 2.40285, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+107, 107789, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2107.45, 5385.14, 48.7787, 1.43373, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Switch (Area: -Unknown- - Difficulty: 0)
(@CGUID+108, 108195, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2107.04, 5485.83, -21.4726, 4.39825, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Objective (Area: -Unknown- - Difficulty: 0) (Auras: 199866 - -Unknown-)
(@CGUID+109, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2102.92, 5419.92, 47.3837, 2.86136, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+110, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2086.16, 5353.94, 48.8107, 0.25274, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+111, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2059.81, 5422.69, 47.3837, 2.3579, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+112, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2020.07, 5412.44, 47.3837, 1.61915, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+113, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2055.57, 5366.23, 47.3837, 2.93566, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+114, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1966.82, 5473.39, 53.4336, 1.18122, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: 130966 - -Unknown-)
(@CGUID+115, 108193, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1988.62, 5451.39, 46.053, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Objective (Area: -Unknown- - Difficulty: 0) (Auras: 199866 - -Unknown-)
(@CGUID+116, 108028, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2067.7, 5402.16, -34.7816, 6.23373, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Webbed Victim (Area: -Unknown- - Difficulty: 0)
(@CGUID+117, 108197, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2034.73, 5423.01, 47.4192, 3.5438, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Leyhunter (Area: -Unknown- - Difficulty: 0) (Auras: 214175 - -Unknown-)
(@CGUID+118, 108027, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2076.1, 5415.19, -37.4753, 0.941694, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Webmistress Shinaris (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+119, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2090.38, 5380.6, 47.3837, 2.79723, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+120, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2050, 5409.38, 47.4192, 3.14159, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0) (Auras: )
(@CGUID+121, 107763, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2043.38, 5454.42, 48.3513, 3.64087, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Withered Creeper (Area: -Unknown- - Difficulty: 0)
(@CGUID+122, 21252, 1612, 8188, 7637, 12, 0, 0, 0, 0, 1959.88, 5440.36, 43.3538, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- World Trigger (Not Immune PC) (Area: -Unknown- - Difficulty: 0)
(@CGUID+123, 107788, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2112, 5401.05, 48.7801, 5.96162, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Switch (Area: -Unknown- - Difficulty: 0)

(@CGUID+125, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2140.74, 5500.67, 1.77052, 5.34765, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+126, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2149.26, 5500.23, 1.77052, 0.628436, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+127, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2251.23, 5519.78, 1.72413, 1.63068, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+128, 108031, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2181.36, 5452.88, 35.3694, 3.79058, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Target (Area: -Unknown- - Difficulty: 12)
(@CGUID+129, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2184.53, 5531.19, 1.72414, 0.959244, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+130, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2199, 5472.62, 1.53045, 3.79901, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+131, 108031, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2212.23, 5453.44, 31.1404, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Target (Area: -Unknown- - Difficulty: 12)
(@CGUID+132, 108033, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2212.31, 5518.12, 16.5078, 4.69189, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Walker (Area: -Unknown- - Difficulty: 12) (Auras: 214451 - -Unknown-)
(@CGUID+133, 108031, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2175.54, 5498.46, 35.1461, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Target (Area: -Unknown- - Difficulty: 12)
(@CGUID+134, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2251.18, 5459.03, 3.72217, 3.3404, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+135, 107855, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2200.28, 5399.34, 36.9593, 5.05154, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vindictive Spirit (Area: -Unknown- - Difficulty: 12)
(@CGUID+136, 108033, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2181.41, 5452.9, 12.7894, 3.78882, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Walker (Area: -Unknown- - Difficulty: 12) (Auras: 214451 - -Unknown-)
(@CGUID+137, 108193, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2264.54, 5455.17, 8.2329, 2.73318, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Objective (Area: -Unknown- - Difficulty: 12) (Auras: 199866 - -Unknown-)
(@CGUID+138, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2241.73, 5458.98, 3.15911, 3.8347, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+139, 108034, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2232.76, 5466.06, 2.22286, 5.99469, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Reaver (Area: -Unknown- - Difficulty: 12)
(@CGUID+140, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2204.2, 5502.63, 1.73499, 1.54575, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+141, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2194.42, 5472.52, 1.53045, 0.554736, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+142, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2175.17, 5413.91, 1.72413, 6.20503, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+143, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2247.22, 5462.91, 3.31657, 0.797913, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+144, 108033, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2212.57, 5454.93, 17.0521, 0.961501, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Walker (Area: -Unknown- - Difficulty: 12) (Auras: 214451 - -Unknown-)
(@CGUID+145, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2208.67, 5500.99, 1.73498, 6.05755, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+146, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2148.16, 5494.54, 1.77052, 5.84195, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+147, 108033, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2175.54, 5498.46, 17.6662, 4.69189, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Walker (Area: -Unknown- - Difficulty: 12) (Auras: 214451 - -Unknown-)
(@CGUID+148, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2168.79, 5457.73, 1.735, 6.20503, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+149, 108031, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2212.31, 5518.17, 29.7596, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Web Target (Area: -Unknown- - Difficulty: 12)
(@CGUID+150, 107937, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2299.58, 5413.99, 14.5122, 0.406842, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Clear Platform (Area: -Unknown- - Difficulty: 12) (Auras: 199866 - -Unknown-)
(@CGUID+151, 107855, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2211.68, 5345.21, 36.9593, 2.53277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vindictive Spirit (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+152, 108035, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2137.24, 5495.42, 1.77052, 2.80516, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fal'dorei Spiderling (Area: -Unknown- - Difficulty: 12)
(@CGUID+153, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2219.06, 5451.43, 1.73499, 6.20503, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+154, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2162.36, 5475.47, 1.73499, 1.95195, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+155, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2193.55, 5484.21, 1.5346, 5.11519, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+156, 107855, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2163.17, 5354.54, 36.9593, 0.774155, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Vindictive Spirit (Area: -Unknown- - Difficulty: 12)
(@CGUID+157, 107944, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2262.37, 5377.63, 35.1535, 0.700234, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Landing Zone (Area: -Unknown- - Difficulty: 12)
(@CGUID+158, 108194, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2258.75, 5350.1, 41.2743, 2.9341, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Objective (Area: -Unknown- - Difficulty: 12)
(@CGUID+159, 108210, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2195.26, 5441.51, 2.1194, 2.21752, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Spider Eggs (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+160, 12999, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2068.31, 5444.19, 47.3004, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- World Invisible Trigger (Area: -Unknown- - Difficulty: 12)
(@CGUID+161, 107786, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2068.25, 5444.24, 50.2027, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Switch (Area: -Unknown- - Difficulty: 12)
(@CGUID+162, 107790, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2069.03, 5437.92, 47.3837, 5.84708, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ravenous Withered (Area: -Unknown- - Difficulty: 12) (Auras: 214029 - -Unknown-)
(@CGUID+163, 12999, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2047.35, 5372.54, 47.3004, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- World Invisible Trigger (Area: -Unknown- - Difficulty: 12)
(@CGUID+164, 107787, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2047.41, 5372.56, 49.9839, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Switch (Area: -Unknown- - Difficulty: 12)
(@CGUID+165, 107790, 1612, 8188, 7637, 12, 0, 0, 0, 0, 2052.64, 5377.08, 47.3837, 0.142737, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ravenous Withered (Area: -Unknown- - Difficulty: 12) (Auras: 214029 - -Unknown-)
(@CGUID+166, 108003, 1612, 8189, 7637, 12, 0, 0, 0, 0, 2348.55, 5433.02, 14.04, 3.59405, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Verstok Darkbough (Area: -Unknown- - Difficulty: 12) (Auras: )
(@CGUID+167, 108053, 1612, 8189, 7637, 12, 0, 0, 0, 0, 2067.69, 5402.16, -34.7808, 0.829174, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Verstok Darkbough (Area: -Unknown- - Difficulty: 12)
(@CGUID+168, 107385, 1612, 8189, 7637, 12, 0, 0, 0, 0, 2063.92, 5403.93, -34.7028, 0.715101, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Fangs of Ashamane (Area: -Unknown- - Difficulty: 12)
(@CGUID+169, 108115, 1612, 8189, 7637, 12, 0, 0, 0, 0, 2056.5, 5406.72, -34.8994, 5.90314, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Ebonfang (Area: -Unknown- - Difficulty: 12)
(@CGUID+170, 110768, 1220, 7846, 8076, 1, 0, 0, 0, 0, 3924.95, 7440.48, 29.6266, 5.50495, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Image of Kalec (Area: -Unknown- - Difficulty: 12)

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+170;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES
(@CGUID+0, 0, 0, 50331648, 0, 0, 0, 0, 0, ""), -- Wraithtalon
(@CGUID+1, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+2, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+3, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+4, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+5, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+6, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+7, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+8, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+9, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+10, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+11, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+12, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Verstok Darkbough
(@CGUID+13, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Trickster
(@CGUID+14, 0, 0, 33554432, 0, 0, 0, 0, 0, ""), -- Felflame Skyterror
(@CGUID+15, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+16, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+17, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+18, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+19, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+20, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Delandros Shimmermoon
(@CGUID+21, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+22, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+23, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+24, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+25, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+26, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+27, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+28, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+29, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+30, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+31, 0, 0, 33554432, 0, 0, 0, 0, 0, ""), -- Felflame Skyterror
(@CGUID+32, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Wrathblade Invader
(@CGUID+33, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+34, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+35, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Trickster
(@CGUID+36, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+37, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+38, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+39, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+40, 0, 0, 33554432, 0, 0, 0, 0, 0, ""), -- Felflame Skyterror
(@CGUID+41, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Dreamgrove Hippogryph
(@CGUID+42, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+43, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+44, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+45, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Little Bunny
(@CGUID+46, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+47, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+48, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+49, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+50, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Wrathblade Invader
(@CGUID+51, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+52, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Terrorvine Lasher
(@CGUID+53, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+54, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+55, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+56, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Wrathblade Invader
(@CGUID+57, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Wrathblade Invader
(@CGUID+58, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+59, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+60, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+61, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+62, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+63, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+64, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+65, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+66, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+67, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+68, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+69, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+70, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+71, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+72, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Felflame Minion
(@CGUID+73, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Eredar Soul Lasher
(@CGUID+74, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+75, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fangs of Ashamane
(@CGUID+76, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Rotwood Sapling
(@CGUID+77, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Rotwood Sapling
(@CGUID+78, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Rotwood Sapling
(@CGUID+79, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- Legion Flames - 213382 - -Unknown-
(@CGUID+80, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Vile Soulwarden
(@CGUID+81, 0, 0, 0, 0, 0, 0, 0, 0, ""), -- Vale Bear
(@CGUID+82, 0, 0, 0, 0, 0, 0, 0, 0, "145953"), -- Vale Bear - 145953 - -Unknown-
(@CGUID+83, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Algromon
(@CGUID+84, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+85, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+86, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+87, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+88, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Delandros Shimmermoon
(@CGUID+89, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ashen Druid
(@CGUID+90, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ebonfang

(@CGUID+93, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+94, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+95, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+96, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+97, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+98, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+99, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+100, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+101, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+102, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+103, 0, 0, 0, 0, 0, 0, 0, 0, "214175"), -- Withered Leyhunter - 214175 - -Unknown-
(@CGUID+104, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ebonfang
(@CGUID+105, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Door
(@CGUID+106, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+107, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Switch
(@CGUID+108, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- Objective - 199866 - -Unknown-
(@CGUID+109, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+110, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+111, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+112, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+113, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+114, 0, 0, 262144, 0, 0, 0, 0, 0, "130966"), -- Withered Creeper - 130966 - -Unknown-
(@CGUID+115, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- Objective - 199866 - -Unknown-
(@CGUID+116, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Webbed Victim
(@CGUID+117, 0, 0, 0, 0, 0, 0, 0, 0, "214175"), -- Withered Leyhunter - 214175 - -Unknown-
(@CGUID+118, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Webmistress Shinaris
(@CGUID+119, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+120, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+121, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Withered Creeper
(@CGUID+122, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- World Trigger (Not Immune PC)
(@CGUID+123, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Switch

(@CGUID+125, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+126, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+127, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+128, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Web Target
(@CGUID+129, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+130, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+131, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Web Target
(@CGUID+132, 0, 0, 50331648, 0, 0, 0, 0, 0, "214451"), -- Fal'dorei Web Walker - 214451 - -Unknown-
(@CGUID+133, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Web Target
(@CGUID+134, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+135, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Vindictive Spirit
(@CGUID+136, 0, 0, 50331648, 0, 0, 0, 0, 0, "214451"), -- Fal'dorei Web Walker - 214451 - -Unknown-
(@CGUID+137, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- Objective - 199866 - -Unknown-
(@CGUID+138, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+139, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Reaver
(@CGUID+140, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+141, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+142, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+143, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+144, 0, 0, 50331648, 0, 0, 0, 0, 0, "214451"), -- Fal'dorei Web Walker - 214451 - -Unknown-
(@CGUID+145, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+146, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+147, 0, 0, 50331648, 0, 0, 0, 0, 0, "214451"), -- Fal'dorei Web Walker - 214451 - -Unknown-
(@CGUID+148, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+149, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Web Target
(@CGUID+150, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- Clear Platform - 199866 - -Unknown-
(@CGUID+151, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Vindictive Spirit
(@CGUID+152, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fal'dorei Spiderling
(@CGUID+153, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+154, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+155, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+156, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Vindictive Spirit
(@CGUID+157, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Landing Zone
(@CGUID+158, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Objective
(@CGUID+159, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Spider Eggs
(@CGUID+160, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- World Invisible Trigger
(@CGUID+161, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Switch
(@CGUID+162, 0, 0, 0, 0, 0, 0, 0, 0, "214029"), -- Ravenous Withered - 214029 - -Unknown-
(@CGUID+163, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- World Invisible Trigger
(@CGUID+164, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Switch
(@CGUID+165, 0, 0, 0, 0, 0, 0, 0, 0, "214029"), -- Ravenous Withered - 214029 - -Unknown-
(@CGUID+166, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Verstok Darkbough
(@CGUID+167, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Verstok Darkbough
(@CGUID+168, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Fangs of Ashamane
(@CGUID+169, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- Ebonfang
(@CGUID+170, 0, 0, 0, 1, 0, 0, 0, 0, ""); -- Image of Kalec

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+54;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 249703, 1220,  7558, 8164, 0, 0, 0, 3399.21, 5563.23, 310.56, 0.64715, 0, 0, 0.3179579, 0.9481049, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+1, 249703, 1220,  7558, 8164, 0, 0, 0, 3435.27, 5686.77, 309.622, 4.30322, 0, 0, -0.8360157, 0.5487055, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+2, 249703, 1220,  7558, 8164, 0, 0, 0, 3345.7, 5707.26, 308.539, 2.61913, 0, 0, 0.9660721, 0.2582726, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+3, 249703, 1220,  7558, 8164, 0, 0, 0, 3463.22, 5502.12, 309.95, 2.68696, 0, 0, 0.9742746, 0.225364, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+4, 249703, 1220,  7558, 8164, 0, 0, 0, 3385.72, 5531.14, 312.96, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+5, 246489, 1220,  7558, 8164, 0, 0, 0, 3424.13, 5605.18, 307.536, 3.14159, 0, 0, -1, 0, 7200, 255, 1, 26124), -- Mossgill Perch School (Area: -Unknown- - Difficulty: 0)
(@OGUID+6, 249703, 1220,  7558, 8164, 0, 0, 0, 3428.55, 5666.05, 309.42, 6.27845, 0, 0, -0.002366066, 0.9999972, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+7, 249703, 1220,  7558, 8164, 0, 0, 0, 3401.66, 5508.42, 310.819, 3.43845, 0, 0, -0.9890041, 0.1478878, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+8, 249703, 1220,  7558, 8164, 0, 0, 0, 3378.37, 5624.52, 308.28, 3.29378, 0, 0, -0.9971056, 0.07602894, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+9, 249703, 1220,  7558, 8164, 0, 0, 0, 3415.08, 5641.19, 310.732, 4.78733, 0, 0, -0.6801205, 0.7331004, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+10, 249703, 1220,  7558, 8164, 0, 0, 0, 3423.73, 5581.54, 310.867, 3.89876, 0, 0, -0.9291887, 0.3696056, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+11, 249703, 1220,  7558, 8164, 0, 0, 0, 3341.95, 5660.66, 310.048, 3.01487, 0, 0, 0.9979925, 0.06333179, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+12, 249703, 1220,  7558, 8164, 0, 0, 0, 3411.31, 5606.78, 309.497, 0.852223, 0, 0, 0.4133329, 0.91058, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+13, 249703, 1220,  7558, 8164, 0, 0, 0, 3491.36, 5476.63, 310.098, 2.94905, 0, 0, 0.995369, 0.09612818, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+14, 249514, 1220,  7558, 8164, 0, 0, 0, 3545.36, 5558.81, 324.829, 5.88577, 0, 0, -0.197402, 0.9803226, 7200, 0, 1, 26124), -- Dreamer's Tear (Area: -Unknown- - Difficulty: 0)
(@OGUID+15, 249703, 1220,  7558, 8164, 0, 0, 0, 3321.29, 5729.05, 308.126, 0.811178, 0, 0, 0.3945599, 0.9188702, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+16, 249703, 1220,  7558, 8164, 0, 0, 0, 3556.45, 5640, 309.161, 3.00667, 0, 0, 0.9977245, 0.06742217, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+17, 249703, 1220,  7558, 8164, 0, 0, 0, 3396, 5777.8, 308.9, 5.80807, 0, 0, -0.2353287, 0.9719158, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+18, 250643, 1612, 8188, 7637, 12, 0, 0, 1989.03, 5323.86, 75.6003, 3.67756, 0, 0, -0.9643068, 0.2647874, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 12)
(@OGUID+19, 249494, 1612, 8188, 7637, 12, 0, 0, 2296.26, 5429.74, 15.0776, 0.0550588, 0, 0, 0.02752495, 0.9996211, 7200, 255, 1, 26124), -- Legion Gate (Area: -Unknown- - Difficulty: 12)
(@OGUID+20, 249703, 1220,  7558, 8164, 0, 0, 0, 3299.56, 5721.07, 310.425, 4.91185, 0, 0, -0.6331892, 0.7739971, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+21, 242646, 1220,  7558, 8164, 0, 0, 0, 3473.06, 5841.04, 273.084, 0.305415, 0, 0, 0.1521139, 0.988363, 7200, 255, 1, 26124), -- Small Treasure Chest (Area: -Unknown- - Difficulty: 0)
(@OGUID+22, 250876, 1612, 8188, 7637, 12, 0, 0, 2249.22, 5333.64, 36.9649, 1.32601, 0, 0, 0.6154871, 0.788147, 7200, 255, 1, 26124), -- Door (Area: -Unknown- - Difficulty: 12)
(@OGUID+23, 249703, 1220,  7558, 8164, 0, 0, 0, 3636.16, 5626.74, 308.709, 1.5657, 0, 0, 0.7053022, 0.7089067, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+24, 249703, 1220,  7558, 8164, 0, 0, 0, 3629.51, 5564.02, 310.773, 4.52554, 0, 0, -0.7699881, 0.6380583, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+25, 249703, 1220,  7558, 8164, 0, 0, 0, 3669.61, 5615.93, 308.233, 1.40306, 0, 0, 0.6453867, 0.763856, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+26, 253936, 1220,  7558, 8164, 0, 0, 0, 3610.54, 5812.39, 319.653, 2.39683, 0, 0, 0.9314632, 0.3638355, 7200, 255, 1, 26124), -- 253936 (Area: -Unknown- - Difficulty: 0)
(@OGUID+27, 254140, 1220,  7558, 8164, 0, 0, 0, 3601.83, 5817.22, 317.896, 6.2578, 0, 0, -0.01269054, 0.9999195, 7200, 255, 1, 26124), -- 254140 (Area: -Unknown- - Difficulty: 0)
(@OGUID+28, 249703, 1220,  7558, 8164, 0, 0, 0, 3685.44, 5604.56, 308.375, 1.828, 0, 0, 0.7919521, 0.6105832, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+29, 253936, 1220,  7558, 8164, 0, 0, 0, 3608.19, 5819.11, 318.44, 0.345252, 0, 0, 0.1717691, 0.9851372, 7200, 255, 1, 26124), -- 253936 (Area: -Unknown- - Difficulty: 0)
(@OGUID+30, 249703, 1220,  7558, 8164, 0, 0, 0, 3524.51, 5435.03, 308.282, 2.51429, 0, 0, 0.9512129, 0.3085353, 7200, 255, 1, 26124), -- Poisonous Mushroom (Area: -Unknown- - Difficulty: 0)
(@OGUID+31, 250824, 1612, 8188, 7637, 12, 0, 0, 2259.11, 5350.02, 36.2768, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Platform (Area: -Unknown- - Difficulty: 12)
(@OGUID+32, 250643, 1612, 8188, 7637, 12, 0, 0, 1979.55, 5590.26, 88.9987, 0.667748, 0, 0, 0.3277054, 0.94478, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+33, 250643, 1612, 8188, 7637, 12, 0, 0, 1875.43, 5612.58, 81.748, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+34, 250643, 1612, 8188, 7637, 12, 0, 0, 1867.84, 5412.77, 42.2482, 3.76205, 0, 0, -0.9522638, 0.3052762, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+35, 250644, 1612, 8188, 7637, 12, 0, 0, 2006.29, 5440.77, 45.5971, 4.15937, 0, 0, -0.8732862, 0.4872075, 7200, 255, 1, 26124), -- Ground Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+36, 250643, 1612, 8188, 7637, 12, 0, 0, 1990.98, 5445.27, 100.602, 0.667748, 0, 0, 0.3277054, 0.94478, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+37, 250643, 1612, 8188, 7637, 12, 0, 0, 1883.51, 5587.5, 86.0808, 0.667748, 0, 0, 0.3277054, 0.94478, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+38, 250640, 1612, 8188, 7637, 12, 0, 0, 2107.45, 5385.14, 48.7787, 1.43373, 0, 0, 0.6570244, 0.7538694, 7200, 255, 1, 26124), -- Orb Light (Area: -Unknown- - Difficulty: 0)
(@OGUID+39, 250645, 1612, 8188, 7637, 12, 0, 0, 2045.71, 5423.81, 47.439, 3.65083, 0, 0, -0.9677591, 0.2518775, 7200, 255, 1, 26124), -- Ground Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+40, 250632, 1612, 8188, 7637, 12, 0, 0, 2124.35, 5388.79, 52.2582, 6.01743, 0, 0, -0.1324863, 0.9911848, 7200, 255, 1, 26124), -- Door (Area: -Unknown- - Difficulty: 0)
(@OGUID+41, 250638, 1612, 8188, 7637, 12, 0, 0, 2068.31, 5444.19, 47.3004, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Ruined Switch (Area: -Unknown- - Difficulty: 0)
(@OGUID+42, 250641, 1612, 8188, 7637, 12, 0, 0, 2047.35, 5372.54, 47.3004, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Ruined Switch (Area: -Unknown- - Difficulty: 0)
(@OGUID+43, 250643, 1612, 8188, 7637, 12, 0, 0, 1895.77, 5448.07, 22.543, 3.03315, 0, 0, 0.9985304, 0.05419482, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+44, 250643, 1612, 8188, 7637, 12, 0, 0, 1884.32, 5530.69, 63.7203, 1.37355, 0, 0, 0.6340466, 0.7732949, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+45, 250643, 1612, 8188, 7637, 12, 0, 0, 1897.8, 5351.35, 72.4881, 3.04184, 0, 0, 0.9987564, 0.04985612, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+46, 250645, 1612, 8188, 7637, 12, 0, 0, 2093.5, 5406.62, 48.0354, 1.7023, 0, 0, 0.752038, 0.6591197, 7200, 255, 1, 26124), -- Ground Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+47, 250644, 1612, 8188, 7637, 12, 0, 0, 1980.76, 5411.02, 45.5971, 1.01762, 0, 0, 0.4871378, 0.8733251, 7200, 255, 1, 26124), -- Ground Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+48, 250643, 1612, 8188, 7637, 12, 0, 0, 2010.06, 5519.06, 101.877, 0.667748, 0, 0, 0.3277054, 0.94478, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+49, 250643, 1612, 8188, 7637, 12, 0, 0, 1934.19, 5633.16, 78.6507, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+50, 250643, 1612, 8188, 7637, 12, 0, 0, 1961.15, 5368.46, 99.2253, 2.45122, 0, 0, 0.9410124, 0.3383721, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+51, 250643, 1612, 8188, 7637, 12, 0, 0, 1899.61, 5508.52, 22.543, 3.94681, 0, 0, -0.9200411, 0.3918219, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+52, 250643, 1612, 8188, 7637, 12, 0, 0, 1831.12, 5464.15, 42.2482, 4.23812, 0, 0, -0.8534307, 0.5212063, 7200, 255, 1, 26124), -- Fog (Area: -Unknown- - Difficulty: 0)
(@OGUID+53, 250639, 1612, 8188, 7637, 12, 0, 0, 2112, 5401.05, 48.7801, 5.96162, 0, 0, -0.1600904, 0.9871024, 7200, 255, 1, 26124), -- Orb Light (Area: -Unknown- - Difficulty: 0)
(@OGUID+54, 250386, 1612, 8189, 7637, 12, 0, 0, 2063.78, 5403.75, -34.7747, 0.7151, 0, 0, 0.3499794, 0.9367574, 7200, 100, 1, 26124); -- Fangs of Ashamane (Area: -Unknown- - Difficulty: 12)

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+54;

DELETE FROM `creature_template_addon` WHERE `entry` IN (108115 /*108115 (Ebonfang)*/, 107385 /*107385 (Fangs of Ashamane)*/, 108053 /*108053 (Verstok Darkbough)*/, 108003 /*108003 (Verstok Darkbough)*/, 107787 /*107787 (Switch)*/, 107790 /*107790 (Ravenous Withered) - -Unknown-*/, 107786 /*107786 (Switch)*/, 108194 /*108194 (Objective)*/, 107944 /*107944 (Landing Zone)*/, 107937 /*107937 (Clear Platform) - -Unknown-*/, 108034 /*108034 (Fal'dorei Reaver)*/, 107855 /*107855 (Vindictive Spirit)*/, 108033 /*108033 (Fal'dorei Web Walker) - -Unknown-*/, 108031 /*108031 (Fal'dorei Web Target)*/, 108210 /*108210 (Spider Eggs)*/, 108035 /*108035 (Fal'dorei Spiderling) - -Unknown-*/, 107788 /*107788 (Switch)*/, 108027 /*108027 (Webmistress Shinaris)*/, 108028 /*108028 (Webbed Victim)*/, 108193 /*108193 (Objective) - -Unknown-*/, 108195 /*108195 (Objective) - -Unknown-*/, 107789 /*107789 (Switch)*/, 107791 /*107791 (Door)*/, 107759 /*107759 (Ebonfang)*/, 108197 /*108197 (Withered Leyhunter) - -Unknown-*/, 107763 /*107763 (Withered Creeper)*/, 107729 /*107729 (Ebonfang)*/, 107593 /*107593 (Algromon)*/, 120321 /*120321 (Vile Soulwarden)*/, 106419 /*106419 (Bloodspike Needler) - -Unknown-*/, 107480 /*107480 (Fangs of Ashamane)*/, 107551 /*107551 (Wrathblade Invader)*/, 107708 /*107708 (Little Bunny)*/, 107512 /*107512 (Dreamgrove Hippogryph)*/, 107390 /*107390 (Ashen Druid)*/, 107566 /*107566 (Wrathblade Invader)*/, 107388 /*107388 (Ashen Druid)*/, 107392 /*107392 (Delandros Shimmermoon)*/, 107523 /*107523 (Legion Flames) - -Unknown-*/, 107568 /*107568 (Felflame Skyterror)*/, 120341 /*120341 (Felflame Trickster)*/, 107520 /*107520 (Verstok Darkbough)*/, 107391 /*107391 (Ashen Druid)*/, 107389 /*107389 (Ashen Druid)*/, 107535 /*107535 (Eredar Soul Lasher)*/, 107548 /*107548 (Felflame Minion)*/);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES
(108115, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108115 (Ebonfang)
(107385, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107385 (Fangs of Ashamane)
(108053, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108053 (Verstok Darkbough)
(108003, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108003 (Verstok Darkbough)
(107787, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107787 (Switch)
(107790, 0, 0, 0, 0, 0, 0, 0, 0, "214029"), -- 107790 (Ravenous Withered) - -Unknown-
(107786, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107786 (Switch)
(108194, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108194 (Objective)
(107944, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107944 (Landing Zone)
(107937, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- 107937 (Clear Platform) - -Unknown-
(108034, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108034 (Fal'dorei Reaver)
(107855, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107855 (Vindictive Spirit)
(108033, 0, 0, 50331648, 0, 0, 0, 0, 0, "214451"), -- 108033 (Fal'dorei Web Walker) - -Unknown-
(108031, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108031 (Fal'dorei Web Target)
(108210, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108210 (Spider Eggs)
(108035, 0, 0, 0, 1, 0, 0, 0, 0, "46598"), -- 108035 (Fal'dorei Spiderling) - -Unknown-
(107788, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107788 (Switch)
(108027, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108027 (Webmistress Shinaris)
(108028, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 108028 (Webbed Victim)
(108193, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- 108193 (Objective) - -Unknown-
(108195, 0, 0, 0, 0, 0, 0, 0, 0, "199866"), -- 108195 (Objective) - -Unknown-
(107789, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107789 (Switch)
(107791, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107791 (Door)
(107759, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107759 (Ebonfang)
(108197, 0, 0, 0, 0, 0, 0, 0, 0, "214175"), -- 108197 (Withered Leyhunter) - -Unknown-
(107763, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107763 (Withered Creeper)
(107729, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107729 (Ebonfang)
(107593, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107593 (Algromon)
(120321, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 120321 (Vile Soulwarden)
(106419, 0, 0, 0, 1, 0, 0, 0, 0, "46598"), -- 106419 (Bloodspike Needler) - -Unknown-
(107480, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107480 (Fangs of Ashamane)
(107551, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107551 (Wrathblade Invader)
(107708, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107708 (Little Bunny)
(107512, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107512 (Dreamgrove Hippogryph)
(107390, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107390 (Ashen Druid)
(107566, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107566 (Wrathblade Invader)
(107388, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107388 (Ashen Druid)
(107392, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107392 (Delandros Shimmermoon)
(107523, 0, 0, 0, 0, 0, 0, 0, 0, "213382"), -- 107523 (Legion Flames) - -Unknown-
(107568, 0, 0, 33554432, 0, 0, 0, 0, 0, ""), -- 107568 (Felflame Skyterror)
(120341, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 120341 (Felflame Trickster)
(107520, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107520 (Verstok Darkbough)
(107391, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107391 (Ashen Druid)
(107389, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107389 (Ashen Druid)
(107535, 0, 0, 0, 1, 0, 0, 0, 0, ""), -- 107535 (Eredar Soul Lasher)
(107548, 0, 0, 0, 1, 0, 0, 0, 0, ""); -- 107548 (Felflame Minion)
