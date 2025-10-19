DROP TABLE IF EXISTS `area_p_o_i`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `area_p_o_i` (
  `ID` int NOT NULL DEFAULT '0',
  `Name` text CHARACTER SET utf8mb3,
  `Description` text CHARACTER SET utf8mb3,
  `Flags` int NOT NULL DEFAULT '0',
  `Pos1` float NOT NULL DEFAULT '0',
  `Pos2` float NOT NULL DEFAULT '0',
  `Pos3` float NOT NULL DEFAULT '0',
  `PoiDataType` int NOT NULL DEFAULT '0',
  `PoiData` int NOT NULL DEFAULT '0',
  `ContinentID` smallint unsigned NOT NULL DEFAULT '0',
  `AreaID` smallint unsigned NOT NULL DEFAULT '0',
  `WorldStateID` smallint unsigned NOT NULL DEFAULT '0',
  `Importance` tinyint unsigned NOT NULL DEFAULT '0',
  `Icon` tinyint unsigned NOT NULL DEFAULT '0',
  `PlayerConditionID` int NOT NULL DEFAULT '0',
  `PortLocID` int NOT NULL DEFAULT '0',
  `UiTextureAtlasMemberID` int NOT NULL DEFAULT '0',
  `MapFloor` int NOT NULL DEFAULT '0',
  `WmoGroupID` int NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_p_o_i`
--

LOCK TABLES `area_p_o_i` WRITE;
/*!40000 ALTER TABLE `area_p_o_i` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_p_o_i` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_p_o_i_locale`
--

DROP TABLE IF EXISTS `area_p_o_i_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `area_p_o_i_locale` (
  `ID` int unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) CHARACTER SET utf8mb3 NOT NULL,
  `Name_lang` text CHARACTER SET utf8mb3,
  `Description_lang` text CHARACTER SET utf8mb3,
  `VerifiedBuild` smallint NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_p_o_i_locale`
--

LOCK TABLES `area_p_o_i_locale` WRITE;
/*!40000 ALTER TABLE `area_p_o_i_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_p_o_i_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_p_o_i_state`
--

DROP TABLE IF EXISTS `area_p_o_i_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `area_p_o_i_state` (
  `ID` int NOT NULL DEFAULT '0',
  `Description` text CHARACTER SET utf8mb3,
  `WorldStateValue` tinyint unsigned NOT NULL DEFAULT '0',
  `IconEnumValue` tinyint unsigned NOT NULL DEFAULT '0',
  `UiTextureAtlasMemberID` int NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_p_o_i_state`
--

LOCK TABLES `area_p_o_i_state` WRITE;
/*!40000 ALTER TABLE `area_p_o_i_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_p_o_i_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_p_o_i_state_locale`
--

DROP TABLE IF EXISTS `area_p_o_i_state_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `area_p_o_i_state_locale` (
  `ID` int unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) CHARACTER SET utf8mb3 NOT NULL,
  `Description_lang` text CHARACTER SET utf8mb3,
  `VerifiedBuild` smallint NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_p_o_i_state_locale`
--

LOCK TABLES `area_p_o_i_state_locale` WRITE;
/*!40000 ALTER TABLE `area_p_o_i_state_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_p_o_i_state_locale` ENABLE KEYS */;
UNLOCK TABLES;