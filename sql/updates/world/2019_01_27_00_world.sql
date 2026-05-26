update `npc_spellclick_spells` set `cast_flags` = 1 where `npc_entry` in (103658, 103655, 96659, 99732, 99731, 99709);
update `creature_template` set `npcflag` = 3 where entry = 92980;

DELETE FROM `areatrigger_template` WHERE `Id` IN (11010, 11231, 9701, 11551, 11552, 10288, 9991, 9856, 12504, 10259);
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `VerifiedBuild`) VALUES
(11010, 4, 0, 3, 3, 3, 3, 0.3, 0.3, 23857),
(11231, 0, 0, 10, 10, 0, 0, 0, 0, 23857),
(9701, 0, 1, 5, 5, 0, 0, 0, 0, 23857),
(11551, 0, 1024, 3, 3, 0, 0, 0, 0, 23857),
(11552, 0, 1, 5, 5, 0, 0, 0, 0, 23857),
(10288, 4, 2, 0.5, 0.5, 1.5, 1.5, 0.3, 0.3, 23857),
(9991, 0, 0, 4, 4, 0, 0, 0, 0, 23857),
(9856, 0, 0, 2, 2, 0, 0, 0, 0, 23857),
(12504, 0, 0, 1, 1, 0, 0, 0, 0, 23857),
(10259, 4, 4, 8, 8, 6, 6, 0.3, 0.3, 23857);

set @GROUP_ID := 0;
set @ID := 0;
Delete From `creature_text` where `CreatureID` in (92980, 92986, 93105, 93112, 93127,93221, 93693, 93759, 93802, 94435, 94654, 95048,96400, 96420, 96436, 96441, 96494, 96499, 96503, 96652, 96653, 96654, 96655, 96847, 96877, 96884, 96888, 97014, 97058, 97069, 97244, 97303, 97370, 97614, 97615, 97616, 97617, 97618, 97619, 97620, 97622, 97676, 97706, 98711, 98986, 99045,99631, 99632, 99915, 102724);

INSERT Ignore INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `comment`) VALUES
(90247, @GROUP_ID+0, @ID, 'It will be as you say.', 12, 0, 100, 1, 0, 55044, 0, 'Battlelord Gaardoun to Player'),
(90247, @GROUP_ID+1, @ID, 'Ashtongue with me!', 14, 0, 100, 15, 0, 55045, 0, 'Battlelord Gaardoun to Player'),
(92718, @GROUP_ID+0, @ID, 'I will need your help to stop Gul\'dan. Free the other Illidari, quickly!', 12, 0, 100, 0, 0, 57338, 0, 'Maiev Shadowsong to Player'),
(92980, @GROUP_ID+0, @ID, 'Why would Maiev free us? There could only be one reason...', 12, 0, 100, 0, 0, 57387, 0, 'Kayn Sunfury to Player'),
(92980, @GROUP_ID+1, @ID, 'The demons aim to destroy our world. We need to release our allies.', 12, 0, 100, 0, 0, 57340, 0, 'Kayn Sunfury to Player'),
(92986, @GROUP_ID+0, @ID, 'The Legion is here.', 12, 0, 100, 0, 0, 57388, 0, 'Altruis the Sufferer to Player'),
(92986, @GROUP_ID+1, @ID, 'Time for answers later. There are demons to kill.', 12, 0, 100, 0, 0, 57339, 0, 'Altruis the Sufferer to Player'),
(93105, @GROUP_ID+0, @ID, 'My eyes... My eyes!', 14, 0, 100, 0, 0, 0, 0, 'Inquisitor Baleful to Coilskar Sea-Caller'),
(93105, @GROUP_ID+1, @ID, 'I can see your secrets...', 12, 0, 100, 0, 0, 55222, 0, 'Inquisitor Baleful to Player'),
(93105, @GROUP_ID+2, @ID, 'Enough! My colossal infernal will crush you.', 14, 0, 100, 0, 0, 55224, 0, 'Inquisitor Baleful'),
(93117, @GROUP_ID+0, @ID, 'The pain!', 12, 0, 100, 0, 0, 55226, 0, 'Izal Whitemoon to Player'),
(93117, @GROUP_ID+1, @ID, 'A slight setback. I assure you it will not happen again.', 12, 0, 100, 274, 0, 55227, 0, 'Izal Whitemoon to Player'),
(93127, @GROUP_ID+0, @ID, 'The spider fortress, the soul engine, and the forge are their primary targets. The Servants of Illidan must succeed.', 12, 0, 100, 1, 0, 55263, 0, 'Kayn Sunfury to Player'),
(93127, @GROUP_ID+1, @ID, 'We will not last much longer if those devastators are not destroyed!', 12, 0, 100, 5, 0, 55260, 0, 'Kayn Sunfury to Player'),
(93127, @GROUP_ID+2, @ID, 'Teach us what you have learned, $n.', 12, 0, 100, 6, 0, 55265, 0, 'Kayn Sunfury to Player'),
(93127, @GROUP_ID+3, @ID, 'I can feel it coursing through my veins. Altering my body.', 12, 0, 100, 0, 0, 55258, 0, 'Kayn Sunfury to Player'),
(93127, @GROUP_ID+4, @ID, 'No... Mannethrel.', 12, 0, 100, 0, 0, 55262, 0, 'Kayn Sunfury to Player'),
(93127, @GROUP_ID+5, @ID, 'Come on, let\'s get up there!', 12, 0, 100, 0, 0, 55257, 0, 'Kayn Sunfury to Player'),
(93221, @GROUP_ID+0, @ID, 'They will die.', 14, 0, 100, 0, 0, 55133, 0, 'Doom Commander Beliash to Player'),
(93221, @GROUP_ID+1, @ID, 'You won\'t survive Inferno Peak...', 12, 0, 100, 0, 0, 55134, 0, 'Doom Commander Beliash to Player'),
(93230, @GROUP_ID+0, @ID, 'So weak... hard to contain... my energy.', 12, 0, 100, 0, 0, 55017, 0, 'Mannethrel Darkstar to Player'),
(93230, @GROUP_ID+1, @ID, 'I almost lost the fight against the fel energy inside me.', 12, 0, 100, 6, 0, 55311, 0, 'Mannethrel Darkstar to Player'),
(93693, @GROUP_ID+0, @ID, 'A fine task for the Coilskar. I am in agreement.', 12, 0, 100, 396, 0, 55295, 0, 'Lady S\'theno to Player'),
(93759, @GROUP_ID+0, @ID, 'I sense greater power within you, $n. Have you stolen a demon\'s essence?', 12, 0, 100, 0, 0, 55150, 0, 'Jace Darkweaver to Player'),
(93759, @GROUP_ID+1, @ID, 'Use the crucible to complete the ritual.', 12, 0, 100, 0, 0, 55242, 0, 'Jace Darkweaver to Player'),
(93802, @GROUP_ID+0, @ID, 'Come forth, my children. Feast upon our enemies.', 14, 0, 100, 0, 0, 55069, 0, 'Brood Queen Tyranna'),
(93802, @GROUP_ID+1, @ID, 'None can resist my kiss.', 14, 0, 100, 0, 0, 55068, 0, 'Brood Queen Tyranna'),
(93802, @GROUP_ID+2, @ID, 'My Lord Sargeras, no!!!', 14, 0, 100, 0, 0, 55055, 0, 'Brood Queen Tyranna to Player'),
(94377, @GROUP_ID+0, @ID, 'I... will not... give in!', 14, 0, 100, 0, 0, 55191, 0, 'Cyana Nightglaive to Player'),
(94377, @GROUP_ID+1, @ID, 'I will never be taken prisoner again. Never!', 12, 0, 100, 5, 0, 55089, 0, 'Cyana Nightglaive to Player'),
(94400, @GROUP_ID+0, @ID, 'I will destroy all of you.', 12, 0, 100, 0, 0, 55052, 0, 'Belath Dawnblade to Player'),
(94400, @GROUP_ID+1, @ID, 'I will kill every last demon in my way.', 12, 0, 100, 5, 0, 55053, 0, 'Belath Dawnblade to Player'),
(94410, @GROUP_ID+0, @ID, '$n, over here.', 12, 0, 100, 0, 0, 55011, 0, 'Allari the Souleater to Player'),
(94410, @GROUP_ID+1, @ID, 'Good luck. I will make my way up to the foothold as soon as I am done with this demon.', 12, 0, 100, 0, 0, 55012, 0, 'Allari the Souleater to Player'),
(94435, @GROUP_ID+0, @ID, 'We will see it done. I urge you to remember that the Black Temple is under attack.', 12, 0, 100, 1, 0, 55314, 0, 'Matron Mother Malevolence to Player'),
(95048, @GROUP_ID+0, @ID, 'Deal with these insects, Beliash.', 12, 0, 100, 0, 0, 55071, 0, 'Brood Queen Tyranna to Player'),
(95226, @GROUP_ID+0, @ID, 'Into my cage you go.', 12, 0, 100, 0, 0, 55017, 0, 'Anguish Jailer to Player'),
(95226, @GROUP_ID+1, @ID, 'I\'ll crack open your flesh and feed upon your soul.', 12, 0, 100, 0, 0, 55016, 0, 'Anguish Jailer to Player'),
(95226, @GROUP_ID+2, @ID, 'A new prisoner for the taking.', 12, 0, 100, 0, 0, 55014, 0, 'Anguish Jailer to Player'),
(96279, @GROUP_ID+0, @ID, 'I am your judge, jury, and executioner.', 12, 0, 100, 0, 0, 55020, 0, 'Mardum Executioner to Player'),
(96402, @GROUP_ID+0, @ID, 'Death comes soon.', 12, 0, 100, 0, 0, 55332, 0, 'Hulking Forgefiend to Player'),
(96420, @GROUP_ID+0, @ID, 'They outnumber us by an order of magnitude. Can we defeat them?', 12, 0, 100, 6, 0, 55092, 0, 'Cyana Nightglaive to Player'),
(96420, @GROUP_ID+1, @ID, 'More... I want more power!', 12, 0, 100, 0, 0, 55094, 0, 'Cyana Nightglaive to Player'),
(96436, @GROUP_ID+0, @ID, '$n, will you use your spectral sight on the cave entrance? Something\'s not right.', 12, 0, 100, 397, 0, 55233, 0, 'Jace Darkweaver to Player'),
(96436, @GROUP_ID+1, @ID, 'They won\'t escape us. Illidari, attack!', 14, 0, 100, 397, 0, 55234, 0, 'Jace Darkweaver to Player'),
(96441, @GROUP_ID+0, @ID, 'My axe never misses its mark.', 14, 0, 100, 0, 0, 56973, 0, 'Fel Lord Caza to Axe Throw Stalker'),
(96441, @GROUP_ID+1, @ID, 'Damn your spectral sight!', 14, 0, 100, 0, 0, 55172, 0, 'Fel Lord Caza to Player'),
(96441, @GROUP_ID+2, @ID, 'I would have sundered your forces...', 12, 0, 100, 0, 0, 55173, 0, 'Fel Lord Caza to Player'),
(96494, @GROUP_ID+0, @ID, 'For the Legion!', 12, 0, 100, 0, 0, 55192, 0, 'Felguard Butcher to Demon Hunter'),
(96494, @GROUP_ID+1, @ID, 'The fel you wield will not be enough.', 12, 0, 100, 0, 0, 55190, 0, 'Felguard Butcher to Player'),
(96494, @GROUP_ID+2, @ID, 'Demon hunters? How did you get here?', 12, 0, 100, 0, 0, 55187, 0, 'Felguard Butcher to Demon Hunter'),
(96494, @GROUP_ID+3, @ID, 'Die, Illidari fool.', 12, 0, 100, 0, 0, 55189, 0, 'Felguard Butcher to Demon Hunter'),
(96494, @GROUP_ID+4, @ID, 'You dare attack us here?!', 12, 0, 100, 0, 0, 55192, 0, 'Felguard Butcher to Coilskar Sea-Caller'),
(96494, @GROUP_ID+5, @ID, 'Invaders. Warn the Brood Queen!', 12, 0, 100, 0, 0, 55186, 0, 'Felguard Butcher to Player'),
(96499, @GROUP_ID+0, @ID, 'The fel lord is just ahead.', 12, 0, 100, 0, 0, 55193, 0, 'Jace Darkweaver to Player'),
(96503, @GROUP_ID+0, @ID, 'My thanks. Come, my brothers... back to the battle!', 12, 0, 100, 0, 0, 55076, 0, 'Coilskar Sea-Caller to General Volroth'),
(96652, @GROUP_ID+0, @ID, '$n, wait... I can\'t contain the power.', 12, 0, 100, 0, 0, 56970, 0, 'Mannethrel Darkstar to Player'),
(96652, @GROUP_ID+1, @ID, 'Aaaaggggghhhhh!', 14, 0, 100, 0, 0, 55312, 0, 'Mannethrel Darkstar to Player'),
(96653, @GROUP_ID+0, @ID, 'Good luck to all of you. I will get the rest of us up there, $n.', 12, 0, 100, 66, 0, 55228, 0, 'Izal Whitemoon to Player'),
(96654, @GROUP_ID+0, @ID, '$n, you made it through!', 14, 0, 100, 22, 0, 55054, 0, 'Belath Dawnblade to Player'),
(96655, @GROUP_ID+0, @ID, 'Last I saw, Kor\'vas cut a swath of destruction on her way across the bridge.', 12, 0, 100, 1, 0, 55010, 0, 'Allari the Souleater to Player'),
(96655, @GROUP_ID+1, @ID, 'The demonic energy... I feel deadlier.', 12, 0, 100, 0, 0, 55006, 0, 'Allari the Souleater to Player'),
(96847, @GROUP_ID+0, @ID, 'I hope Maiev was right about freeing you, demon hunter. Go on - I will hold the line.', 12, 0, 100, 0, 0, 57291, 0, 'Drelanim Whisperwind to Player'),
(96877, @GROUP_ID+0, @ID, 'We will destroy the Burning Legion.', 12, 0, 100, 0, 0, 55037, 0, 'Ashtongue Stalker to Player'),
(96877, @GROUP_ID+1, @ID, 'The Legion must be stopped.', 12, 0, 100, 0, 0, 55043, 0, 'Ashtongue Stalker to Player'),
(96884, @GROUP_ID+0, @ID, 'I have come to your aid.', 12, 0, 100, 0, 0, 55083, 0, 'Coilskar Sea-Caller to Player'),
(96884, @GROUP_ID+1, @ID, 'Legion magic is nothing compared to that of the naga.', 12, 0, 100, 0, 0, 55080, 0, 'Coilskar Sea-Caller to Player'),
(96884, @GROUP_ID+2, @ID, 'The Legion must be stopped before they burn our world.', 12, 0, 100, 0, 0, 55085, 0, 'Coilskar Sea-Caller to Player'),
(96884, @GROUP_ID+3, @ID, 'Lord Illidan leads and the Coilskar follow.', 12, 0, 100, 0, 0, 55081, 0, 'Coilskar Sea-Caller to Player'),
(96884, @GROUP_ID+4, @ID, 'Lady S\'theno requested I join you.', 12, 0, 100, 0, 0, 55078, 0, 'Coilskar Sea-Caller to Player'),
(96888, @GROUP_ID+0, @ID, 'This demon will die from a thousand cuts.', 12, 0, 100, 0, 0, 55354, 0, 'Shivarra Destroyer to Player'),
(97034, @GROUP_ID+0, @ID, 'I live to serve.', 12, 0, 100, 0, 0, 55363, 0, 'Fury Champion to Demon Hunter'),
(97034, @GROUP_ID+1, @ID, 'My blade will cut through you.', 12, 0, 100, 0, 0, 55371, 0, 'Fury Champion to Demon Hunter'),
(97058, @GROUP_ID+0, @ID, 'My swarm will feast on your soul!', 14, 0, 100, 0, 0, 55088, 0, 'Count Nefarious'),
(97058, @GROUP_ID+1, @ID, '|TInterface\\Icons\\spell_shadow_carrionswarm:20|t%s begins to cast |cFFFF0000|Hspell:198947|h[Carrion Storm]|h|r!', 41, 0, 100, 0, 0, 55088, 0, 'Count Nefarious'),
(97058, @GROUP_ID+2, @ID, 'Tyranna... help.', 12, 0, 100, 0, 0, 55087, 0, 'Count Nefarious to Ashtongue Stalker'),
(97058, @GROUP_ID+3, @ID, 'Oh good, another soul for my engine.', 12, 0, 100, 0, 0, 55086, 0, 'Count Nefarious'),
(97059, @GROUP_ID+0, @ID, 'My queen\'s brood will hatch soon.', 14, 0, 100, 0, 0, 55283, 0, 'King Voras'),
(97059, @GROUP_ID+1, @ID, 'They\'ve slain me, my queen...', 14, 0, 100, 0, 0, 55282, 0, 'King Voras to Player'),
(97059, @GROUP_ID+2, @ID, '|TInterface\\Icons\\inv_misc_monsterspidercarapace_01:20|tNearby |cFFFF0000|Hspell:198235|h[Spider Eggs]|h|r will hatch soon if not destroyed!.', 41, 0, 100, 0, 0, 55283, 0, 'King Voras'),
(97244, @GROUP_ID+0, @ID, 'Grab the keystone! Let\'s get to the portal down below and back to the Black Temple!', 14, 0, 100, 0, 0, 55268, 0, 'Kayn Sunfury to Brood Queen Tyranna'),
(97303, @GROUP_ID+0, @ID, 'Hurry, $n. Use the keystone to activate the portal. We must rejoin the fight at the Black Temple.', 12, 0, 100, 25, 0, 55259, 0, 'Kayn Sunfury to Player'),
(97370, @GROUP_ID+0, @ID, 'You cannot have them. They are mine!', 14, 0, 100, 0, 0, 55199, 0, 'General Volroth to Player'),
(97370, @GROUP_ID+1, @ID, 'My fire will leave only your bones behind.', 14, 0, 100, 0, 0, 55201, 0, 'General Volroth'),
(97370, @GROUP_ID+2, @ID, 'Impossible!', 14, 0, 100, 0, 0, 55200, 0, 'General Volroth to Player'),
(97459, @GROUP_ID+0, @ID, 'My forces are decimated. I have failed you.', 12, 0, 100, 1, 0, 55050, 0, 'Battlelord Gaardoun'),
(97459, @GROUP_ID+1, @ID, 'I swear to give my life if that is what it takes.', 12, 0, 100, 66, 0, 55051, 0, 'Battlelord Gaardoun'),
(97676, @GROUP_ID+0, @ID, 'Beware, $n. There is a dreadlord inside that Soul Engine.', 12, 0, 100, 0, 0, 55304, 0, 'Lady S\'theno to Player'),
(98229, @GROUP_ID+0, @ID, 'Activate all three gateways and summon in the rest of our forces.', 12, 0, 100, 0, 0, 55244, 0, 'Kayn Sunfury to Player'),
(98229, @GROUP_ID+1, @ID, 'I\'ll cut a hole through the demons and meet you in the volcano.', 12, 0, 100, 0, 0, 55245, 0, 'Kayn Sunfury to Player'),
(98354, @GROUP_ID+0, @ID, 'I will join you. Let\'s kill some more demons.', 12, 0, 100, 0, 0, 55285, 0, 'Kor\'vas Bloodthorn to Player'),
(98459, @GROUP_ID+0, @ID, 'You know, I should be in command.', 12, 0, 100, 0, 0, 55280, 0, 'Kayn Sunfury to Player'),
(98483, @GROUP_ID+0, @ID, 'Tasty meats for me to eats.', 12, 0, 100, 0, 0, 55207, 0, 'Hellish Imp to Player'),
(98486, @GROUP_ID+0, @ID, 'My life for the Legion.', 12, 0, 100, 0, 0, 55364, 0, 'Wrath Warrior to Player'),
(98486, @GROUP_ID+1, @ID, 'We will cleanse the universe in fire.', 12, 0, 100, 0, 0, 55363, 0, 'Wrath Warrior to Player'),
(98711, @GROUP_ID+0, @ID, 'The imp mother is in that cave, right there.', 12, 0, 100, 0, 0, 55290, 0, 'Kor\'vas Bloodthorn to Player'),
(98986, @GROUP_ID+0, @ID, 'My fel secrets will consume you!', 14, 0, 100, 0, 0, 55340, 0, 'Prolifica'),
(98986, @GROUP_ID+1, @ID, 'They\'re stealing our secrets, my queen...', 14, 0, 100, 0, 0, 55339, 0, 'Prolifica to Player'),
(99045, @GROUP_ID+0, @ID, 'Whatever you choose, it will feel good to use more of the Legion\'s power against them.', 12, 0, 100, 273, 0, 55293, 0, 'Kor\'vas Bloodthorn to Player'),
(99045, @GROUP_ID+1, @ID, 'I could destroy the Legion on my own!', 12, 0, 100, 0, 0, 55292, 0, 'Kor\'vas Bloodthorn to Player'),
(99631, @GROUP_ID+0, @ID, 'I am awake? How is this possible?', 12, 0, 100, 0, 0, 57294, 0, 'Kayn Sunfury to Player'),
(99632, @GROUP_ID+0, @ID, 'How many years were wasted in that cell?', 12, 0, 100, 0, 0, 57343, 0, 'Altruis the Sufferer to Player'),
(99915, @GROUP_ID+0, @ID, '$n, we have a huge problem here.', 12, 0, 100, 0, 0, 55342, 0, 'Sevis Brightflame to Player'),
(99915, @GROUP_ID+1, @ID, 'Your sacrifice will NOT be in vain!', 12, 0, 100, 0, 0, 55343, 0, 'Sevis Brightflame to Player'),
(99916, @GROUP_ID+0, @ID, 'Plenty of souls were sacrificed here. The activator is powered.', 12, 0, 100, 273, 0, 55344, 0, 'Sevis Brightflame to Player'),
(99916, @GROUP_ID+1, @ID, 'I will move ahead and find Allari.', 12, 0, 100, 0, 0, 55346, 0, 'Sevis Brightflame to Player'),
(99917, @GROUP_ID+0, @ID, '$n, hurry! You don\'t have much time.', 12, 0, 100, 3, 0, 55345, 0, 'Sevis Brightflame to Player'),
(99917, @GROUP_ID+1, @ID, 'Sevis looks down at the dying broken mystic.', 16, 0, 100, 3, 0, 55345, 0, 'Sevis Brightflame to Player'),
(99917, @GROUP_ID+2, @ID, 'I will see you at the final gateway.', 12, 0, 100, 1, 0, 55347, 0, 'Sevis Brightflame to Player'),
(100545, @GROUP_ID+0, @ID, 'For Lord Illidan!', 12, 0, 100, 0, 0, 57122, 0, 'Demon Hunter to Player'),
(100545, @GROUP_ID+1, @ID, 'Thank you, $n. Back into the fight.', 12, 0, 100, 0, 0, 57126, 0, 'Demon Hunter to Player'),
(100548, @GROUP_ID+0, @ID, 'We WILL take the keystone!', 12, 0, 100, 0, 0, 57108, 0, 'Demon Hunter to Player'),
(100548, @GROUP_ID+1, @ID, 'Die demons!', 12, 0, 100, 0, 0, 57109, 0, 'Demon Hunter to Player'),
(100982, @GROUP_ID+0, @ID, 'I will meet you down at the gateway on the Molten Shore.', 12, 0, 100, 0, 0, 55352, 0, 'Sevis Brightflame to Player'),
(102726, @GROUP_ID+0, @ID, 'Finish them. Recast the cloaking spell.', 12, 0, 100, 0, 0, 56976, 0, 'Eredar Sorcerer to Demon Hunter');

DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=1;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 1, 4446.45, -295.896, -235.339, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=2;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 2, 4456.69, -311.004, -207.667, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=3;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 3, 4460.65, -321.54, -187.735, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=4;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 4, 4454.37, -328.542, -165.568, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=5;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 5, 4447.92, -327.422, -153.965, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=6;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 6, 4448.48, -319.545, -144.887, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=7;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 7, 4454.76, -318.391, -127.121, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=8;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 8, 4455.59, -325.278, -107.721, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=9;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 9, 4449.19, -328.938, -85.2078, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=10;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 10, 4444.79, -325.208, -66.8808, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=11;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 11, 4446.59, -317.991, -27.7739, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=12;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 12, 4451.85, -316.278, -6.79571, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=13;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 13, 4454.85, -320.903, 38.564, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=14;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 14, 4454.28, -324.109, 68.72, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=15;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 15, 4451.35, -326.188, 91.9784, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=16;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 16, 4451.14, -327.823, 136.227, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=17;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 17, 4451.24, -351.073, 129.361, NULL);

DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 1, 4325.83, -585.576, -281.745, '4325.832 Y: -585.5758 Z: -281.7451');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 2, 4325.83, -582.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 3, 4325.83, -582.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 4, 4325.83, -579.576, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 5, 4325.83, -576.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');

DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 1, 4446.45, -295.896, -235.339, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 2, 4456.69, -311.004, -207.667, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 3, 4460.65, -321.54, -187.735, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 4, 4454.37, -328.542, -165.568, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 5, 4447.92, -327.422, -153.965, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 6, 4448.48, -319.545, -144.887, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 7, 4454.76, -318.391, -127.121, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=8;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 8, 4455.59, -325.278, -107.721, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=9;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 9, 4449.19, -328.938, -85.2078, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=10;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 10, 4444.79, -325.208, -66.8808, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=11;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 11, 4446.59, -317.991, -27.7739, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=12;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 12, 4451.85, -316.278, -6.79571, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=13;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 13, 4454.85, -320.903, 38.564, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=14;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 14, 4454.28, -324.109, 68.72, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=15;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 15, 4451.35, -326.188, 91.9784, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=16;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 16, 4451.14, -327.823, 136.227, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=17;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 17, 4451.24, -351.073, 129.361, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 1, 4337.47, -590.45, -281.415, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 2, 4336.72, -590.7, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 3, 4334.97, -590.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 4, 4333.97, -590.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 5, 4332.72, -590.7, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 6, 4331.97, -590.2, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 7, 4331.97, -589.45, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=8;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 8, 4332.22, -588.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 1, 4315.13, -590.317, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 2, 4316.88, -590.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 3, 4317.88, -590.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 4, 4318.88, -590.816, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 5, 4319.63, -590.067, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 6, 4320.13, -589.317, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 7, 4319.88, -588.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 1, 4331.29, -587.508, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 2, 4331.29, -586.508, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 3, 4331.29, -585.758, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 4, 4331.04, -584.758, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 5, 4330.54, -584.258, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 6, 4328.46, -583.66, -281.729, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963101 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963101, 1, 4330.08, -548.288, -281.75, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 1, 4319.15, -586.838, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 2, 4320.9, -586.338, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 3, 4321.9, -585.088, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 4, 4323.64, -583.668, -281.753, NULL);

DELETE FROM `creature_questender` WHERE `id`=92980 AND `quest`=38690;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (92980, 38690);
DELETE FROM `creature_questender` WHERE `id`=92984 AND `quest`=38723;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (92984, 38723);
DELETE FROM `creature_questender` WHERE `id`=92984 AND `quest`=40253;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (92984, 40253);
DELETE FROM `creature_questender` WHERE `id`=97643 AND `quest`=39682;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (97643, 39682);

DELETE FROM `creature_queststarter` WHERE `id`=92718 AND `quest`=38672;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92718, 38672);
DELETE FROM `creature_queststarter` WHERE `id`=92718 AND `quest`=38723;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92718, 38723);
DELETE FROM `creature_queststarter` WHERE `id`=92718 AND `quest`=40253;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92718, 40253);
DELETE FROM `creature_queststarter` WHERE `id`=92718 AND `quest`=41226;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92718, 41226);
DELETE FROM `creature_queststarter` WHERE `id`=92980 AND `quest`=38690;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92980, 38690);
DELETE FROM `creature_queststarter` WHERE `id`=92984 AND `quest`=39682;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92984, 39682);
DELETE FROM `creature_queststarter` WHERE `id`=96675 AND `quest`=39683;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (96675, 39683);
DELETE FROM `creature_queststarter` WHERE `id`=96675 AND `quest`=39686;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (96675, 39686);
DELETE FROM `creature_queststarter` WHERE `id`=96675 AND `quest`=40254;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (96675, 40254);
DELETE FROM `creature_queststarter` WHERE `id`=97643 AND `quest`=39684;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (97643, 39684);
DELETE FROM `creature_queststarter` WHERE `id`=97643 AND `quest`=39685;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (97643, 39685);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=133511 AND `spell_effect`=133509 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (133511, 133509, 0, 'Make the bar go up');


DELETE FROM `spell_areatrigger` WHERE (`AreaTriggerId`=11010) OR (`AreaTriggerId`=11231) OR (`AreaTriggerId`=9701) OR (`AreaTriggerId`=11551) OR (`AreaTriggerId`=11552) OR (`AreaTriggerId`=10288) OR (`AreaTriggerId`=9991) OR (`AreaTriggerId`=9856) OR (`AreaTriggerId`=12504) OR (`AreaTriggerId`=10259);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(6343, 11010, 0, 0, 0, 0, 0, 0, 10000, 23857), -- SpellId : 225717
(6659, 11231, 0, 0, 0, 0, 0, 0, 30000, 23857), -- SpellId : 209788
(4915, 9701, 0, 0, 0, 0, 35, 0, 600000, 23857), -- SpellId : 193238
(7082, 11551, 0, 0, 0, 0,35, 45804, 600000, 23857), -- SpellId : 213212
(7083, 11552, 0, 0, 0, 0,35, 998, 600000, 23857), -- SpellId : 213214
(5572, 10288, 0, 1669, 0, 0, 0, 12000, 12000, 23857), -- SpellId : 199761
(5269, 9991, 0, 0, 0, 0, 0, 0, 150000, 23857), -- SpellId : 196503
(5130, 9856, 0, 0, 0, 0, 0, 7177, 600000, 23857), -- SpellId : 202202
(8318, 12504, 0, 0, 0, 0, 0, 0, 15000, 23857), -- SpellId : 223473
(5548 , 10259, 0, 0, 0, 0, 0, 0, 0, 23857); -- SpellId : 199477

-- ServerToClient: SMSG_INIT_WORLD_STATES (0x278B) Length: 16 ConnIdx: 1 Time: 04/02/2017 11:09:18.349 Number: 705
-- MapID: 1468 (1468)
-- AreaId: 7814 (7814)
-- SubareaID: 7819 (7819)
-- Field Count: 0

-- ServerToClient: SMSG_PHASE_SHIFT_CHANGE (0x2577) Length: 50 ConnIdx: 1 Time: 04/02/2017 11:09:18.349 Number: 704
-- Client: Full: 0x08338C000000000000000000002801F1 Player/0 R3299/S0 Map: 0 Low: 2621937
-- PhaseShiftFlags: 16
-- PhaseShiftCount: 5
-- PersonalGUID: Full: 0x0
-- [0] PhaseFlags: 0
-- [0] Id: 5128
-- [1] PhaseFlags: 1
-- [1] Id: 5158
-- [2] PhaseFlags: 1
-- [2] Id: 5401
-- [3] PhaseFlags: 1
-- [3] Id: 5407
-- [4] PhaseFlags: 1
-- [4] Id: 5976
-- VisibleMapIDsCount: 0
-- PreloadMapIDsCount: 0
-- UiWorldMapAreaIDSwap: 0

update `creature_template` set `KillCredit1` = 106241 where `entry` = 92290;
update `creature_template` set `KillCredit1` = 106241 where `entry` = 97632;

update `creature_template` set `VehicleId` = 4372 where `entry` = 99443;
update `creature_template` set `ScriptName` = "npc_vault_of_the_wardens_vampiric_felbat" where `entry` = 99443;
update `creature_template` set `KillCredit1` = 106254 where `entry` = 96682;

update `gameobject` set `state` = 1 where `id` = 244588;
update `creature` set `npcflag` = 3 where `guid` = 20542908;
update `creature_template` set `InhabitType` = 4 where `entry` = 99443;
update `creature_template` set `speed_walk` = 7.5 where `entry` = 99443;
update `creature_template` set `speed_run` = 7.5 where `entry` = 99443;

-- Maiev Shadowsong 92718 SAI
SET @ENTRY := 92718;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,38672,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Say Line 0"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,53,1,92718,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest Taken - Start Waypoint"),
(@ENTRY,0,2,0,58,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,3,0,19,0,100,0,40253,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Run Script"),
(@ENTRY,0,4,0,19,0,100,0,38723,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Run Script");

-- Actionlist SAI
SET @ENTRY := 9271800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,85,223661,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Script - Invoker Cast 'Spell Id223661'"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,85,187864,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Script - Invoker Cast 'Spell Id187864'"),
(@ENTRY,9,2,0,0,0,100,0,100000,100000,0,0,62,1468,0,0,0,0,0,7,0,0,0,4084.27,-298.11,-282.07,3.11803,"Maiev Shadowsong 92718 - On Script - Teleport");

-- Kayn's Cell 103658 SAI
SET @ENTRY := 103658;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,11,0,100,0,0,0,0,0,82,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Respawn - Add Npc Flag Spellclick"),
(@ENTRY,0,1,2,73,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Spellclick - Run Script"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Spellclick - Store Targetlist"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,128,0,1,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn");

-- Actionlist SAI
SET @ENTRY := 10365800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,128,3761,1,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Despawn Instant"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,99631,5,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Set Data 0 1"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,99631,5,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Send Target 1");

-- Kayn Sunfury 99631 SAI
SET @ENTRY := 99631;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,53,0,99631,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 1 - Start Waypoint"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 0 - Say Line 0"),
(@ENTRY,0,2,3,40,0,100,0,8,99631,0,0,17,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint 8 Reached - Set Emote State 16"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint 0 Reached - Run Script"),
(@ENTRY,0,4,6,58,0,100,0,0,@ENTRY*100+00,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,33,99326,0,0,0,0,0,12,1,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Quest Credit 'Kill Credit: Reunion Finished - Kayn 99326'"),
(@ENTRY,0,7,0,38,0,100,0,0,2,0,0,53,1,@ENTRY*100+01,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 2 - Start Waypoint"),
(@ENTRY,0,8,9,58,0,100,0,0,@ENTRY*100+01,0,0,45,0,1,0,0,0,0,19,92980,10,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Set Data 0 1"),
(@ENTRY,0,9,0,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9963100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,53,0,9963100,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Start Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Set Emote State 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,33,112276,0,0,0,0,0,12,1,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Quest Credit 'Kill Credit: Kayn Picked up Weapons 112276'");

-- Altruis's Cell 103655 SAI
SET @ENTRY := 103655;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,3,11,0,100,0,0,0,0,0,82,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn - Add Npc Flag Spellclick"),
(@ENTRY,0,1,2,73,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Spellclick - Run Script"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Spellclick - Store Targetlist"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,128,0,1,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn");

-- Actionlist SAI
SET @ENTRY := 10365500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,128,3761,1,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Despawn Instant"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,99632,20,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Set Data 0 1"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,99632,20,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Send Target 1");

-- Altruis the Sufferer 99632 SAI
SET @ENTRY := 99632;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,53,0,99632,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 1 - Start Waypoint"),
(@ENTRY,0,1,5,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 0 - Say Line 0"),
(@ENTRY,0,2,3,40,0,100,0,7,99632,0,0,17,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint 7 Reached - Set Emote State 16"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint 0 Reached - Run Script"),
(@ENTRY,0,4,6,58,0,100,0,0,@ENTRY*100+00,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,33,112287,0,0,0,0,0,12,1,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Quest Credit 'Kill Credit: Reunion Finished - Altruis 112287'"),
(@ENTRY,0,7,0,38,0,100,0,0,2,0,0,53,1,@ENTRY*100+01,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 2 - Start Waypoint"),
(@ENTRY,0,8,6,58,0,100,0,0,@ENTRY*100+01,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9963200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,53,0,9963200,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Start Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Set Emote State 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,33,112277,0,0,0,0,0,12,1,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Quest Credit 'Kill Credit: Altruis Picked up Weapons 112277'");

-- Lever SAI
SET @ENTRY := 244923;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,1,20,0,100,0,38672,0,0,0,45,0,2,0,0,0,0,10,20542674,99631,0,0,0,0,0,"Lever - On Quest '' Finished - Set Data 0 2"),
(@ENTRY,1,1,0,61,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542658,99632,0,0,0,0,0,"Lever - On Quest Finished - Set Data 0 2");

-- Kayn Sunfury 92980 SAI
SET @ENTRY := 92980;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,80,0,@ENTRY*100+00,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92980 - On Data Set 0 1 - Run Script");

-- Actionlist SAI
SET @ENTRY := 9298000;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,45,0,1,0,0,0,0,19,92986,10,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Set Data 0 1"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,45,0,1,0,0,0,0,19,96847,20,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Set Data 0 1");

-- Altruis the Sufferer 92986 SAI
SET @ENTRY := 92986;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,2,19,0,100,0,38689,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest '' Taken - Say Line 1"),
(@ENTRY,0,1,0,38,0,100,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Data Set 0 1 - Say Line 0"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,85,133508,0,0,0,0,0,7,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest Taken - Invoker Cast 'Spell Id133508'"),
(@ENTRY,0,3,0,20,0,100,0,38689,0,0,0,85,133510,0,0,0,0,0,16,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest '' Finished - Invoker Cast 'Spell Id133510'");

-- Drelanim Whisperwind 96847 SAI
SET @ENTRY := 96847;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Drelanim Whisperwind 96847 - On Data Set 0 1 - Say Line 0");

-- hackfix i got no idea how to trigger the quest credit else
-- Fel Shocktrooper 92776 SAI
SET @ENTRY := 92776;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,3,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,4,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,5,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,6,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,7,0,32,0,50,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,8,0,32,0,60,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,9,0,32,0,70,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,10,0,32,0,80,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'");

-- Savage Felstalker 92782 SAI
SET @ENTRY := 92782;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,3,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,4,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,5,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,6,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,7,0,32,0,50,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,8,0,32,0,60,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,9,0,32,0,70,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,10,0,32,0,80,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'");

-- Kayn Sunfury 92984 SAI
SET @ENTRY := 92984;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,19,33344,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 1 - Remove Flag Immune To NPC's"),
(@ENTRY,0,1,3,61,0,100,0,0,0,0,0,2,2804,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 0 - Set Faction 2804"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,92990,20,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 0 - Start Attacking"),
(@ENTRY,0,4,0,38,0,100,0,0,2,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 2 - Run Script"),
(@ENTRY,0,5,0,34,0,100,0,8,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Reached Point 0 - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9298400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,5000,5000,0,0,69,0,0,0,0,0,0,10,20542908,92984,0,0,0,0,0,"Kayn Sunfury 92984 - On Script - Move To Closest Creature 'Kayn Sunfury'");

-- Altruis the Sufferer 92985 SAI
SET @ENTRY := 92985;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,19,33344,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 1 - Remove Flag Immune To NPC's"),
(@ENTRY,0,1,3,61,0,100,0,0,0,0,0,2,2804,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 0 - Set Faction 2804"),
(@ENTRY,0,2,0,11,0,100,0,0,0,0,0,81,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Respawn - Set Npc Flags Gossip & Questgiver"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,97632,20,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 0 - Start Attacking"),
(@ENTRY,0,4,0,38,0,100,0,0,2,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 2 - Run Script"),
(@ENTRY,0,5,0,34,0,100,0,8,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Reached Point 0 - Despawn Instant");

-- Sledge 92990 SAI
SET @ENTRY := 92990;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,4,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,20542913,92984,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,20542914,92985,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,2,3,4,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Sledge 92990 - On Aggro - Store Targetlist"),
(@ENTRY,0,3,4,61,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,97632,20,0,0,0,0,0,"Sledge 92990 - On Aggro - Send Target 1"),
(@ENTRY,0,4,0,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,97632,20,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,5,6,6,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542913,92984,0,0,0,0,0,"Sledge 92990 - On Just Died - Set Data 0 2"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542914,92985,0,0,0,0,0,"Sledge 92990 - On Just Died - Set Data 0 2");

-- Actionlist SAI
SET @ENTRY := 9298500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,5000,5000,0,0,69,0,0,0,0,0,0,10,20542909,92985,0,0,0,0,0,"Altruis the Sufferer 92985 - On Script - Move To Closest Creature 'Altruis the Sufferer'");

-- Crusher 97632 SAI
SET @ENTRY := 97632;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,49,0,0,0,0,0,0,12,1,0,0,0,0,0,0,"Crusher 97632 - On Data Set 0 1 - Start Attacking");

-- Countermeasures 99732 SAI
SET @ENTRY := 99732;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99732,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");

-- Countermeasures 99731 SAI
SET @ENTRY := 99731;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99731,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");

-- Countermeasures 99709 SAI
SET @ENTRY := 99709;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99709,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");
