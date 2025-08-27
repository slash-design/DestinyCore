-- Dumping structure for table group_finder_activity
DROP TABLE IF EXISTS `group_finder_activity`;
CREATE TABLE IF NOT EXISTS `group_finder_activity` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `FullName` text,
  `ShortName` text,
  `MinGearLevelSuggestion` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `GroupFinderCategoryID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupFinderActivityGrpID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevelSuggestion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DisplayType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_activity: 0 rows
DELETE FROM `group_finder_activity`;
/*!40000 ALTER TABLE `group_finder_activity` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_activity` ENABLE KEYS */;

-- Dumping structure for table group_finder_activity_locale
DROP TABLE IF EXISTS `group_finder_activity_locale`;
CREATE TABLE IF NOT EXISTS `group_finder_activity_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `FullName_lang` text,
  `ShortName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_activity_locale: 0 rows
DELETE FROM `group_finder_activity_locale`;
/*!40000 ALTER TABLE `group_finder_activity_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_activity_locale` ENABLE KEYS */;

-- Dumping structure for table group_finder_activity_grp
DROP TABLE IF EXISTS `group_finder_activity_grp`;
CREATE TABLE IF NOT EXISTS `group_finder_activity_grp` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Name` text,
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_activity_grp: 0 rows
DELETE FROM `group_finder_activity_grp`;
/*!40000 ALTER TABLE `group_finder_activity_grp` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_activity_grp` ENABLE KEYS */;

-- Dumping structure for table group_finder_activity_grp_locale
DROP TABLE IF EXISTS `group_finder_activity_grp_locale`;
CREATE TABLE IF NOT EXISTS `group_finder_activity_grp_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_activity_grp_locale: 0 rows
DELETE FROM `group_finder_activity_grp_locale`;
/*!40000 ALTER TABLE `group_finder_activity_grp_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_activity_grp_locale` ENABLE KEYS */;

-- Dumping structure for table group_finder_category
DROP TABLE IF EXISTS `group_finder_category`;
CREATE TABLE IF NOT EXISTS `group_finder_category` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Name` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_category: 0 rows
DELETE FROM `group_finder_category`;
/*!40000 ALTER TABLE `group_finder_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_category` ENABLE KEYS */;

-- Dumping structure for table group_finder_category_locale
DROP TABLE IF EXISTS `group_finder_category_locale`;
CREATE TABLE IF NOT EXISTS `group_finder_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table group_finder_category_locale: 0 rows
DELETE FROM `group_finder_category_locale`;
/*!40000 ALTER TABLE `group_finder_category_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_finder_category_locale` ENABLE KEYS */;