/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DBCfmt.h"
#include "ItemPrototype.h"
#include <iostream>
#include <fstream>

#include <map>

static std::unordered_map<uint32, std::vector<CriteriaTreeEntry const*> > sCriteriaTreeList;
static std::unordered_map<uint32, std::vector<ModifierTreeEntry const*> > sModifierTreeList;
static std::unordered_map<uint32, std::list<uint32> > sSpellProcsPerMinuteModEntryList;
static std::unordered_map<uint32, AchievementEntry const* > sAchievementParentList;
static std::unordered_map<uint32, std::list<uint32> > sItemSpecsList;
static std::unordered_map<uint32, uint32 > sRevertLearnSpellList;
static std::unordered_map<uint32, uint32 > sReversTriggerSpellList;
static std::unordered_map<uint32, std::vector<ItemSpecOverrideEntry const*>> _itemSpecOverrides;

typedef std::tuple<int16, int8, int32> WMOAreaTableKey;
typedef std::map<WMOAreaTableKey, WMOAreaTableEntry const*> WMOAreaInfoByTripple;
static WMOAreaInfoByTripple sWMOAreaInfoByTripple;

DBCStorage <AreaTableEntry> sAreaTableStore(AreaTableEntryfmt);
DBCStorage <AreaGroupEntry> sAreaGroupStore(AreaGroupEntryfmt);
DBCStorage <AreaPOIEntry> sAreaPOIStore(AreaPOIEntryfmt);

DBCStorage <AchievementEntry> sAchievementStore(Achievementfmt);
DBCStorage <CriteriaEntry> sCriteriaStore(Criteriafmt);
DBCStorage <CriteriaTreeEntry> sCriteriaTreeStore(CriteriaTreefmt);
DBCStorage <ModifierTreeEntry> sModifierTreeStore(ModifierTreefmt);
DBCStorage <AreaTriggerEntry> sAreaTriggerStore(AreaTriggerEntryfmt);
DBCStorage <ArmorLocationEntry> sArmorLocationStore(ArmorLocationfmt);
DBCStorage <AuctionHouseEntry> sAuctionHouseStore(AuctionHouseEntryfmt);
DBCStorage <BankBagSlotPricesEntry> sBankBagSlotPricesStore(BankBagSlotPricesEntryfmt);
BannedAddonDataMap  sBannedAddonDataMap;
DBCStorage <BattlemasterListEntry> sBattlemasterListStore(BattlemasterListEntryfmt);
DBCStorage <BarberShopStyleEntry> sBarberShopStyleStore(BarberShopStyleEntryfmt);
DBCStorage <CharStartOutfitEntry> sCharStartOutfitStore(CharStartOutfitEntryfmt);
DBCStorage <CharTitlesEntry> sCharTitlesStore(CharTitlesEntryfmt);
DBCStorage <ChatChannelsEntry> sChatChannelsStore(ChatChannelsEntryfmt);
DBCStorage <ChrClassesEntry> sChrClassesStore(ChrClassesEntryfmt);
DBCStorage <ChrRacesEntry> sChrRacesStore(ChrRacesEntryfmt);
DBCStorage <ChrPowerTypesEntry> sChrPowerTypesStore(ChrClassesXPowerTypesfmt);
DBCStorage <ChrSpecializationsEntry> sChrSpecializationsStore(ChrSpecializationsfmt);

DBCStorage <CinematicSequencesEntry> sCinematicSequencesStore(CinematicSequencesEntryfmt);
DBCStorage <CreatureDisplayInfoEntry> sCreatureDisplayInfoStore(CreatureDisplayInfofmt);
DBCStorage <CreatureFamilyEntry> sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage <CreatureModelDataEntry> sCreatureModelDataStore(CreatureModelDatafmt);
DBCStorage <CreatureSpellDataEntry> sCreatureSpellDataStore(CreatureSpellDatafmt);
DBCStorage <CreatureTypeEntry> sCreatureTypeStore(CreatureTypefmt);
DBCStorage <CurrencyTypesEntry> sCurrencyTypesStore(CurrencyTypesfmt);

DBCStorage <DestructibleModelDataEntry> sDestructibleModelDataStore(DestructibleModelDatafmt);
DBCStorage <DifficultyEntry> sDifficultyStore(Difficultyfmt);
DBCStorage <DungeonEncounterEntry> sDungeonEncounterStore(DungeonEncounterfmt);
DBCStorage <DurabilityQualityEntry> sDurabilityQualityStore(DurabilityQualityfmt);
DBCStorage <DurabilityCostsEntry> sDurabilityCostsStore(DurabilityCostsfmt);

DBCStorage <EmotesEntry> sEmotesStore(EmotesEntryfmt);
DBCStorage <EmotesTextEntry> sEmotesTextStore(EmotesTextEntryfmt);

typedef std::map<uint32, SimpleFactionsList> FactionTeamMap;
static FactionTeamMap sFactionTeamMap;
DBCStorage <FactionEntry> sFactionStore(FactionEntryfmt);
DBCStorage <FactionTemplateEntry> sFactionTemplateStore(FactionTemplateEntryfmt);

DBCStorage <GameObjectDisplayInfoEntry> sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage <GemPropertiesEntry> sGemPropertiesStore(GemPropertiesEntryfmt);
DBCStorage <GlyphPropertiesEntry> sGlyphPropertiesStore(GlyphPropertiesfmt);
DBCStorage <GlyphSlotEntry> sGlyphSlotStore(GlyphSlotfmt);

DBCStorage <GtBarberShopCostBaseEntry>    sGtBarberShopCostBaseStore(GtBarberShopCostBasefmt);
DBCStorage <GtCombatRatingsEntry>         sGtCombatRatingsStore(GtCombatRatingsfmt);
DBCStorage <GtChanceToMeleeCritBaseEntry> sGtChanceToMeleeCritBaseStore(GtChanceToMeleeCritBasefmt);
DBCStorage <GtChanceToMeleeCritEntry>     sGtChanceToMeleeCritStore(GtChanceToMeleeCritfmt);
DBCStorage <GtChanceToSpellCritBaseEntry> sGtChanceToSpellCritBaseStore(GtChanceToSpellCritBasefmt);
DBCStorage <GtChanceToSpellCritEntry>     sGtChanceToSpellCritStore(GtChanceToSpellCritfmt);
DBCStorage <GtItemSocketCostPerLevelEntry> sGtItemSocketCostPerLevelStore(GtItemSocketCostPerLevelfmt);
DBCStorage <GtOCTClassCombatRatingScalarEntry> sGtOCTClassCombatRatingScalarStore(GtOCTClassCombatRatingScalarfmt);
DBCStorage <GtOCTRegenHPEntry>            sGtOCTRegenHPStore(GtOCTRegenHPfmt);
//DBCStorage <GtOCTRegenMPEntry>            sGtOCTRegenMPStore(GtOCTRegenMPfmt);  -- not used currently
DBCStorage <gtOCTHpPerStaminaEntry>       sGtOCTHpPerStaminaStore(GtOCTHpPerStaminafmt);
DBCStorage <GtRegenMPPerSptEntry>         sGtRegenMPPerSptStore(GtRegenMPPerSptfmt);
DBCStorage <GtSpellScalingEntry>          sGtSpellScalingStore(GtSpellScalingfmt);
DBCStorage <GtOCTBaseHPByClassEntry>      sGtOCTBaseHPByClassStore(GtOCTBaseHPByClassfmt);
DBCStorage <GtOCTBaseMPByClassEntry>      sGtOCTBaseMPByClassStore(GtOCTBaseMPByClassfmt);
DBCStorage <GtBattlePetTypeDamageModEntry>      sGtBattlePetTypeDamageModStore(GtBattlePetTypeDamageModfmt);
DBCStorage <GuildPerkSpellsEntry>         sGuildPerkSpellsStore(GuildPerkSpellsfmt);
//DBCStorage <GtNpcDamageByClassEntry>     sGtNpcDamageByClassExp1Store(GtNpcDamageByClassfmt);
//DBCStorage <GtNpcDamageByClassEntry>     sGtNpcDamageByClassExp2Store(GtNpcDamageByClassfmt);
//DBCStorage <GtNpcDamageByClassEntry>     sGtNpcDamageByClassExp3Store(GtNpcDamageByClassfmt);
//DBCStorage <GtNpcDamageByClassEntry>     sGtNpcDamageByClassExp4Store(GtNpcDamageByClassfmt);

DBCStorage <HolidaysEntry>                sHolidaysStore(Holidaysfmt);

DBCStorage <ImportPriceArmorEntry>        sImportPriceArmorStore(ImportPriceArmorfmt);
DBCStorage <ImportPriceQualityEntry>      sImportPriceQualityStore(ImportPriceQualityfmt);
DBCStorage <ImportPriceShieldEntry>       sImportPriceShieldStore(ImportPriceShieldfmt);
DBCStorage <ImportPriceWeaponEntry>       sImportPriceWeaponStore(ImportPriceWeaponfmt);
DBCStorage <ItemPriceBaseEntry>           sItemPriceBaseStore(ItemPriceBasefmt);
DBCStorage <ItemReforgeEntry>             sItemReforgeStore(ItemReforgefmt);
DBCStorage <ItemArmorQualityEntry>        sItemArmorQualityStore(ItemArmorQualityfmt);
DBCStorage <ItemArmorShieldEntry>         sItemArmorShieldStore(ItemArmorShieldfmt);
DBCStorage <ItemArmorTotalEntry>          sItemArmorTotalStore(ItemArmorTotalfmt);
DBCStorage <ItemClassEntry>               sItemClassStore(ItemClassfmt);
DBCStorage <ItemBagFamilyEntry>           sItemBagFamilyStore(ItemBagFamilyfmt);
DBCStorage <ItemDamageEntry>              sItemDamageAmmoStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageOneHandStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageOneHandCasterStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageRangedStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageThrownStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageTwoHandStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageTwoHandCasterStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageWandStore(ItemDamagefmt);
DBCStorage <ItemDisenchantLootEntry>      sItemDisenchantLootStore(ItemDisenchantLootfmt);
//DBCStorage <ItemDisplayInfoEntry>       sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt); -- not used currently
DBCStorage <ItemLimitCategoryEntry>       sItemLimitCategoryStore(ItemLimitCategoryEntryfmt);
DBCStorage <ItemRandomPropertiesEntry>    sItemRandomPropertiesStore(ItemRandomPropertiesfmt);
DBCStorage <ItemRandomSuffixEntry>        sItemRandomSuffixStore(ItemRandomSuffixfmt);
DBCStorage <ItemSetEntry>                 sItemSetStore(ItemSetEntryfmt);
DBCStorage <ItemSpecEntry>                sItemSpecStore(ItemSpecEntryfmt);
DBCStorage <ItemSpecOverrideEntry>        sItemSpecOverrideStore(ItemSpecOverrideEntryfmt);
//std::map<uint32, 

DBCStorage <LanguageWordsEntry>           sLanguageWordsStore(LanguageWordsEntryfmt);
std::map<uint32 /*lang id*/, LanguageWordsMap> sLanguageWordsMapStore;
DBCStorage <LFGDungeonEntry> sLFGDungeonStore(LFGDungeonEntryfmt);
DBCStorage <LiquidTypeEntry> sLiquidTypeStore(LiquidTypefmt);
DBCStorage <LockEntry> sLockStore(LockEntryfmt);

DBCStorage <MailTemplateEntry> sMailTemplateStore(MailTemplateEntryfmt);
DBCStorage <MapEntry> sMapStore(MapEntryfmt);

// DBC used only for initialization sMapDifficultyMap at startup.
DBCStorage <MapDifficultyEntry> sMapDifficultyStore(MapDifficultyEntryfmt); // only for loading
MapDifficultyMap sMapDifficultyMap;

DBCStorage <MovieEntry> sMovieStore(MovieEntryfmt);
DBCStorage <MountCapabilityEntry> sMountCapabilityStore(MountCapabilityfmt);
DBCStorage <MountTypeEntry> sMountTypeStore(MountTypefmt);

DBCStorage <NameGenEntry> sNameGenStore(NameGenfmt);
NameGenVectorArraysMap sGenNameVectoArraysMap;

DBCStorage <OverrideSpellDataEntry> sOverrideSpellDataStore(OverrideSpellDatafmt);

DBCStorage <PvPDifficultyEntry> sPvPDifficultyStore(PvPDifficultyfmt);

DBCStorage <QuestSortEntry> sQuestSortStore(QuestSortEntryfmt);
DBCStorage <QuestXPEntry>   sQuestXPStore(QuestXPfmt);
DBCStorage <QuestFactionRewEntry>  sQuestFactionRewardStore(QuestFactionRewardfmt);
DBCStorage <RandomPropertiesPointsEntry> sRandomPropertiesPointsStore(RandomPropertiesPointsfmt);
DBCStorage <ResearchBranchEntry> sResearchBranchStore(ResearchBranchfmt);
DBCStorage <ResearchProjectEntry> sResearchProjectStore(ResearchProjectfmt);
DBCStorage <ResearchSiteEntry> sResearchSiteStore(ResearchSitefmt);
std::set<ResearchProjectEntry const*> sResearchProjectSet;
ResearchSiteDataMap sResearchSiteDataMap;
DBCStorage <QuestPOIBlobEntry> sQuestPOIBlobStore(QuestPOIBlobfmt);
DBCStorage <QuestPOIPointEntry> sQuestPOIPointStore(QuestPOIPointfmt);
DBCStorage <ScalingStatDistributionEntry> sScalingStatDistributionStore(ScalingStatDistributionfmt);
DBCStorage <ScalingStatValuesEntry> sScalingStatValuesStore(ScalingStatValuesfmt);

std::set<uint32> sScenarioCriteriaTreeStore;
DBCStorage <ScenarioEntry> sScenarioStore(Scenariofmt);
DBCStorage <ScenarioStepEntry> sScenarioStepStore(ScenarioStepfmt);

DBCStorage <SkillLineEntry> sSkillLineStore(SkillLinefmt);
DBCStorage <SkillLineAbilityEntry> sSkillLineAbilityStore(SkillLineAbilityfmt);

DBCStorage <SoundEntriesEntry> sSoundEntriesStore(SoundEntriesfmt);

DBCStorage <SpecializationSpellEntry> sSpecializationSpellStore(SpecializationSpellsfmt);

DBCStorage <SpellItemEnchantmentEntry> sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage <SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore(SpellItemEnchantmentConditionfmt);
DBCStorage <SpellEntry> sSpellStore(SpellEntryfmt);
SpellCategoryStore sSpellCategoryStore;
SpellSkillingList sSpellSkillingList;
PetFamilySpellsStore sPetFamilySpellsStore;

DBCStorage <SpellScalingEntry> sSpellScalingStore(SpellScalingEntryfmt);
DBCStorage <SpellTotemsEntry> sSpellTotemsStore(SpellTotemsEntryfmt);
DBCStorage <SpellTargetRestrictionsEntry> sSpellTargetRestrictionsStore(SpellTargetRestrictionsEntryfmt);
DBCStorage <SpellMiscEntry> sSpellMiscStore(SpellMiscEntryfmt);
DBCStorage <SpellPowerEntry> sSpellPowerStore(SpellPowerEntryfmt);
DBCStorage <PowerDisplayEntry> sPowerDisplayStore(PowerDisplayEntryfmt);
DBCStorage <SpellLevelsEntry> sSpellLevelsStore(SpellLevelsEntryfmt);
DBCStorage <SpellInterruptsEntry> sSpellInterruptsStore(SpellInterruptsEntryfmt);
DBCStorage <SpellEquippedItemsEntry> sSpellEquippedItemsStore(SpellEquippedItemsEntryfmt);
DBCStorage <SpellClassOptionsEntry> sSpellClassOptionsStore(SpellClassOptionsEntryfmt);
DBCStorage <SpellCooldownsEntry> sSpellCooldownsStore(SpellCooldownsEntryfmt);
DBCStorage <SpellAuraOptionsEntry> sSpellAuraOptionsStore(SpellAuraOptionsEntryfmt);
DBCStorage <SpellProcsPerMinuteEntry> sSpellProcsPerMinuteStore(SpellProcsPerMinuteEntryfmt);
DBCStorage <SpellProcsPerMinuteModEntry> sSpellProcsPerMinuteModStore(SpellProcsPerMinuteModEntryfmt);

DBCStorage <SpellAuraRestrictionsEntry> sSpellAuraRestrictionsStore(SpellAuraRestrictionsEntryfmt);
SpellRestrictionDiffMap sSpellRestrictionDiffMap;

DBCStorage <SpellCastingRequirementsEntry> sSpellCastingRequirementsStore(SpellCastingRequirementsEntryfmt);

SpellEffectMap sSpellEffectMap;
SpellEffectDiffMap sSpellEffectDiffMap;
SpellTotemMap sSpellTotemMap;

DBCStorage <SpellCastTimesEntry> sSpellCastTimesStore(SpellCastTimefmt);
DBCStorage <SpellCategoriesEntry> sSpellCategoriesStore(SpellCategoriesEntryfmt);
DBCStorage <SpellCategoryEntry> sSpellCategoryStores(SpellCategoryEntryfmt);
DBCStorage <SpellEffectEntry> sSpellEffectStore(SpellEffectEntryfmt);
DBCStorage <SpellEffectScalingEntry> sSpellEffectScalingStore(SpellEffectScalingEntryfmt);
DBCStorage <SpellDurationEntry> sSpellDurationStore(SpellDurationfmt);
DBCStorage <SpellFocusObjectEntry> sSpellFocusObjectStore(SpellFocusObjectfmt);
DBCStorage <SpellRadiusEntry> sSpellRadiusStore(SpellRadiusfmt);
DBCStorage <SpellRangeEntry> sSpellRangeStore(SpellRangefmt);
DBCStorage <SpellRuneCostEntry> sSpellRuneCostStore(SpellRuneCostfmt);
DBCStorage <SpellShapeshiftEntry> sSpellShapeshiftStore(SpellShapeshiftEntryfmt);
DBCStorage <SpellShapeshiftFormEntry> sSpellShapeshiftFormStore(SpellShapeshiftFormfmt);
DBCStorage <StableSlotPricesEntry> sStableSlotPricesStore(StableSlotPricesfmt);
DBCStorage <SummonPropertiesEntry> sSummonPropertiesStore(SummonPropertiesfmt);

DBCStorage <TalentEntry> sTalentStore(TalentEntryfmt);
TalentSpellList sTalentSpellList;
DBCStorage <TaxiNodesEntry> sTaxiNodesStore(TaxiNodesEntryfmt);
TaxiMask sTaxiNodesMask;
TaxiMask sOldContinentsNodesMask;
TaxiMask sHordeTaxiNodesMask;
TaxiMask sAllianceTaxiNodesMask;
TaxiMask sDeathKnightTaxiNodesMask;

// DBC used only for initialization sTaxiPathSetBySource at startup.
TaxiPathSetBySource sTaxiPathSetBySource;
DBCStorage <TaxiPathEntry> sTaxiPathStore(TaxiPathEntryfmt);

// DBC used only for initialization sTaxiPathNodeStore at startup.
TaxiPathNodesByPath sTaxiPathNodesByPath;
static DBCStorage <TaxiPathNodeEntry> sTaxiPathNodeStore(TaxiPathNodeEntryfmt);

DBCStorage <TeamContributionPointsEntry> sTeamContributionPointsStore(TeamContributionPointsfmt);
DBCStorage <TotemCategoryEntry> sTotemCategoryStore(TotemCategoryEntryfmt);

TransportAnimationsByEntry sTransportAnimationsByEntry;
DBCStorage <TransportAnimationEntry> sTransportAnimationStore(TransportAnimationEntryfmt);
DBCStorage <UnitPowerBarEntry> sUnitPowerBarStore(UnitPowerBarEntryfmt);
DBCStorage <VehicleEntry> sVehicleStore(VehicleEntryfmt);
DBCStorage <VehicleSeatEntry> sVehicleSeatStore(VehicleSeatEntryfmt);
DBCStorage <WMOAreaTableEntry> sWMOAreaTableStore(WMOAreaTableEntryfmt);
DBCStorage <WorldMapAreaEntry> sWorldMapAreaStore(WorldMapAreaEntryfmt);
DBCStorage <WorldMapOverlayEntry> sWorldMapOverlayStore(WorldMapOverlayEntryfmt);
DBCStorage <WorldSafeLocsEntry> sWorldSafeLocsStore(WorldSafeLocsEntryfmt);
DBCStorage <PhaseEntry> sPhaseStores(PhaseEntryfmt);
typedef std::map<uint32, std::set<uint32> > SpecializationMasterySpellsMap;
SpecializationMasterySpellsMap sSpecializationMasterySpells;

std::set<uint32> const* GetSpecializationMasterySpells(uint32 specId)
{
    SpecializationMasterySpellsMap::const_iterator itr = sSpecializationMasterySpells.find(specId);
    if (itr == sSpecializationMasterySpells.end())
        return NULL;

    return &itr->second;
}

typedef std::list<std::string> StoreProblemList;

uint32 DBCFileCount = 0;

static bool LoadDBC_assert_print(uint32 fsize, uint32 rsize, const std::string& filename)
{
    TC_LOG_ERROR("server", "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DBCFileCount;
    std::string dbcFilename = dbcPath + filename;
    SqlDbc * sql = NULL;
    if (customFormat)
        sql = new SqlDbc(&filename, customFormat, customIndexName, storage.GetFormat());

    if (storage.Load(dbcFilename.c_str(), sql))
    {
        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDbcLocales & (1 << i)))
                continue;

            std::string localizedName(dbcPath);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str()))
                availableDbcLocales &= ~(1<<i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100, " (exists, but has %u fields instead of " SIZEFMTD ") Possible wrong client version.", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errors.push_back(dbcFilename + buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }

    delete sql;
}

void LoadDBCStores(const std::string& dataPath)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath+"dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

    LoadDBC(availableDbcLocales, bad_dbc_files, sAreaTableStore,              dbcPath, "AreaTable.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sAchievementStore,            dbcPath, "Achievement.dbc", &CustomAchievementfmt, &CustomAchievementIndex);//14545
    //LoadDBC(availableDbcLocales, bad_dbc_files, sAchievementCriteriaStore,    dbcPath, "Achievement_Criteria.dbc", &CustomAchievementCriteriafmt, &CustomAchievementCriteriaIndex);//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCriteriaStore,               dbcPath, "Criteria.dbc", &CustomCriteriafmt, &CustomCriteriaIndex);//16048
    LoadDBC(availableDbcLocales, bad_dbc_files, sCriteriaTreeStore,           dbcPath, "CriteriaTree.dbc", &CustomCriteriaTreefmt, &CustomCriteriaTreeIndex);//16048
    LoadDBC(availableDbcLocales, bad_dbc_files, sModifierTreeStore,           dbcPath, "ModifierTree.dbc");//16048

    for (uint32 i = 0; i < sAchievementStore.GetNumRows(); ++i)
    {
        if (AchievementEntry const* as = sAchievementStore.LookupEntry(i))
            if (as->CriteriaTree > 0)
                sAchievementParentList[as->CriteriaTree] = as;
    }

    for (uint32 i = 0; i < sCriteriaTreeStore.GetNumRows(); ++i)
    {
        if (CriteriaTreeEntry const* ct = sCriteriaTreeStore.LookupEntry(i))
            if (ct->Parent > 0)
                sCriteriaTreeList[ct->Parent].push_back(ct);
    }

    for (uint32 i = 0; i < sModifierTreeStore.GetNumRows(); ++i)
    {
        if (ModifierTreeEntry const* mt = sModifierTreeStore.LookupEntry(i))
            if (mt->Parent > 0)
                sModifierTreeList[mt->Parent].push_back(mt);
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sAreaTriggerStore,            dbcPath, "AreaTrigger.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sAreaGroupStore,              dbcPath, "AreaGroup.dbc");//14545
    //LoadDBC(availableDbcLocales, bad_dbc_files, sAreaPOIStore,                dbcPath, "AreaPOI.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sAuctionHouseStore,           dbcPath, "AuctionHouse.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sArmorLocationStore,          dbcPath, "ArmorLocation.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sBankBagSlotPricesStore,      dbcPath, "BankBagSlotPrices.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sBattlemasterListStore,       dbcPath, "BattleMasterList.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sBarberShopStyleStore,        dbcPath, "BarberShopStyle.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCharStartOutfitStore,        dbcPath, "CharStartOutfit.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCharTitlesStore,             dbcPath, "CharTitles.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sChatChannelsStore,           dbcPath, "ChatChannels.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrClassesStore,             dbcPath, "ChrClasses.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrRacesStore,               dbcPath, "ChrRaces.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrPowerTypesStore,          dbcPath, "ChrClassesXPowerTypes.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrSpecializationsStore,     dbcPath, "ChrSpecialization.dbc");//16135
    //LoadDBC(availableDbcLocales, bad_dbc_files, sCinematicSequencesStore,     dbcPath, "CinematicSequences.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureDisplayInfoStore,    dbcPath, "CreatureDisplayInfo.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureFamilyStore,         dbcPath, "CreatureFamily.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureModelDataStore,      dbcPath, "CreatureModelData.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureSpellDataStore,      dbcPath, "CreatureSpellData.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureTypeStore,           dbcPath, "CreatureType.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sCurrencyTypesStore,          dbcPath, "CurrencyTypes.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sDestructibleModelDataStore,  dbcPath, "DestructibleModelData.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sDifficultyStore,             dbcPath, "Difficulty.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sDungeonEncounterStore,       dbcPath, "DungeonEncounter.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sDurabilityCostsStore,        dbcPath, "DurabilityCosts.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sDurabilityQualityStore,      dbcPath, "DurabilityQuality.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sEmotesStore,                 dbcPath, "Emotes.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sEmotesTextStore,             dbcPath, "EmotesText.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sFactionStore,                dbcPath, "Faction.dbc");//14545
    for (uint32 i=0; i<sFactionStore.GetNumRows(); ++i)
    {
        FactionEntry const* faction = sFactionStore.LookupEntry(i);
        if (faction && faction->team)
        {
            SimpleFactionsList &flist = sFactionTeamMap[faction->team];
            flist.push_back(i);
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sFactionTemplateStore,        dbcPath, "FactionTemplate.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGameObjectDisplayInfoStore,  dbcPath, "GameObjectDisplayInfo.dbc");//14545
    for (uint32 i = 0; i < sGameObjectDisplayInfoStore.GetNumRows(); ++i)
    {
        if (GameObjectDisplayInfoEntry const* info = sGameObjectDisplayInfoStore.LookupEntry(i))
        {
            if (info->maxX < info->minX)
                std::swap(*(float*)(&info->maxX), *(float*)(&info->minX));
            if (info->maxY < info->minY)
                std::swap(*(float*)(&info->maxY), *(float*)(&info->minY));
            if (info->maxZ < info->minZ)
                std::swap(*(float*)(&info->maxZ), *(float*)(&info->minZ));
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sGemPropertiesStore,          dbcPath, "GemProperties.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGlyphPropertiesStore,        dbcPath, "GlyphProperties.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGlyphSlotStore,              dbcPath, "GlyphSlot.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtBarberShopCostBaseStore,   dbcPath, "gtBarberShopCostBase.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtCombatRatingsStore,        dbcPath, "gtCombatRatings.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToMeleeCritBaseStore, dbcPath, "gtChanceToMeleeCritBase.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToMeleeCritStore,    dbcPath, "gtChanceToMeleeCrit.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToSpellCritBaseStore, dbcPath, "gtChanceToSpellCritBase.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToSpellCritStore,    dbcPath, "gtChanceToSpellCrit.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtItemSocketCostPerLevelStore,dbcPath,"gtItemSocketCostPerLevel.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTClassCombatRatingScalarStore,    dbcPath, "gtOCTClassCombatRatingScalar.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTHpPerStaminaStore,      dbcPath, "gtOCTHpPerStamina.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtRegenMPPerSptStore,        dbcPath, "gtRegenMPPerSpt.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtSpellScalingStore,        dbcPath, "gtSpellScaling.dbc");//15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTBaseHPByClassStore,        dbcPath, "gtOCTBaseHPByClass.dbc");//15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTBaseMPByClassStore,        dbcPath, "gtOCTBaseMPByClass.dbc");//15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtBattlePetTypeDamageModStore,        dbcPath, "gtBattlePetTypeDamageMod.dbc");//15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sGuildPerkSpellsStore,        dbcPath, "GuildPerkSpells.dbc");//15595
    //LoadDBC(availableDbcLocales, bad_dbc_files, sGtNpcDamageByClassStore,        dbcPath, "gtNpcDamageByClass.dbc");//20810
    //LoadDBC(availableDbcLocales, bad_dbc_files, sGtNpcDamageByClassExp1Store,    dbcPath, "gtNpcDamageByClassExp1.dbc");//20810
    //LoadDBC(availableDbcLocales, bad_dbc_files, sGtNpcDamageByClassExp2Store,    dbcPath, "gtNpcDamageByClassExp2.dbc");//20810
    //LoadDBC(availableDbcLocales, bad_dbc_files, sGtNpcDamageByClassExp3Store,    dbcPath, "gtNpcDamageByClassExp3.dbc");//20810
    //LoadDBC(availableDbcLocales, bad_dbc_files, sGtNpcDamageByClassExp4Store,    dbcPath, "gtNpcDamageByClassExp4.dbc");//20810

    LoadDBC(availableDbcLocales, bad_dbc_files, sHolidaysStore,               dbcPath, "Holidays.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sImportPriceArmorStore,       dbcPath, "ImportPriceArmor.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sImportPriceQualityStore,     dbcPath, "ImportPriceQuality.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sImportPriceShieldStore,      dbcPath, "ImportPriceShield.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sImportPriceWeaponStore,      dbcPath, "ImportPriceWeapon.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemPriceBaseStore,          dbcPath, "ItemPriceBase.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemReforgeStore,            dbcPath, "ItemReforge.dbc"); // 15595
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemBagFamilyStore,          dbcPath, "ItemBagFamily.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemClassStore,              dbcPath, "ItemClass.dbc"); // 15595
    //LoadDBC(dbcCount, availableDbcLocales, bad_dbc_files, sItemDisplayInfoStore,        dbcPath, "ItemDisplayInfo.dbc");     -- not used currently
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemLimitCategoryStore,      dbcPath, "ItemLimitCategory.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemRandomPropertiesStore,   dbcPath, "ItemRandomProperties.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemRandomSuffixStore,       dbcPath, "ItemRandomSuffix.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemSetStore,                dbcPath, "ItemSet.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemSpecStore,               dbcPath, "ItemSpec.dbc");//17538
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemSpecOverrideStore,       dbcPath, "ItemSpecOverride.dbc", &CustomItemSpecOverrideEntryfmt, &CustomItemSpecOverrideEntryIndex);//17538

    for (uint32 i = 0; i < sItemSpecOverrideStore.GetNumRows(); ++i)
    {
        if (ItemSpecOverrideEntry const* entry = sItemSpecOverrideStore.LookupEntry(i))
        {
            sItemSpecsList[entry->ItemID].push_back(entry->SpecID);
            _itemSpecOverrides[entry->ItemID].push_back(entry);
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorQualityStore,       dbcPath, "ItemArmorQuality.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorShieldStore,        dbcPath, "ItemArmorShield.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorTotalStore,         dbcPath, "ItemArmorTotal.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageAmmoStore,         dbcPath, "ItemDamageAmmo.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageOneHandStore,      dbcPath, "ItemDamageOneHand.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageOneHandCasterStore,dbcPath, "ItemDamageOneHandCaster.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageRangedStore,       dbcPath, "ItemDamageRanged.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageThrownStore,       dbcPath, "ItemDamageThrown.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageTwoHandStore,      dbcPath, "ItemDamageTwoHand.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageTwoHandCasterStore,dbcPath, "ItemDamageTwoHandCaster.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageWandStore,         dbcPath, "ItemDamageWand.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDisenchantLootStore,     dbcPath, "ItemDisenchantLoot.dbc", &CustomtemDisenchantLootEntryfmt, &CustomtemDisenchantLootEntryIndex);

    LoadDBC(availableDbcLocales, bad_dbc_files, sLanguageWordsStore,          dbcPath, "LanguageWords.dbc");
    for (uint32 i = 0; i < sLanguageWordsStore.GetNumRows(); ++i)
    {
        LanguageWordsEntry const* entry = sLanguageWordsStore.LookupEntry(i);
        if (!entry)
            continue;

        sLanguageWordsMapStore[entry->langId][strlen(entry->word)].push_back(entry->word);
    }
    LoadDBC(availableDbcLocales, bad_dbc_files, sLFGDungeonStore,             dbcPath, "LfgDungeons.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sLiquidTypeStore,             dbcPath, "LiquidType.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sLockStore,                   dbcPath, "Lock.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sPhaseStores,                 dbcPath, "Phase.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sMailTemplateStore,           dbcPath, "MailTemplate.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sMapStore,                    dbcPath, "Map.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sMapDifficultyStore,          dbcPath, "MapDifficulty.dbc", &CustomMapDifficultyEntryfmt, &CustomMapDifficultyEntryIndex);//17538
    // fill data
    for (uint32 i = 0; i < sMapDifficultyStore.GetNumRows(); ++i)
    {
        if (MapDifficultyEntry const* entry = sMapDifficultyStore.LookupEntry(i))
        {
            if (!sMapStore.LookupEntry(entry->MapId))
            {
                TC_LOG_INFO("server", "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant map %u.", entry->MapId);
                continue;
            }
            if (entry->Difficulty && !sDifficultyStore.LookupEntry(entry->Difficulty))
            {
                TC_LOG_INFO("server", "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant difficulty %u.", entry->Difficulty);
                continue;
            }
            sMapDifficultyMap[MAKE_PAIR32(entry->MapId, entry->Difficulty)] = MapDifficulty(entry->resetTime, entry->maxPlayers, entry->areaTriggerText[0] > 0);
        }
    }
    sMapDifficultyStore.Clear();

    LoadDBC(availableDbcLocales, bad_dbc_files, sMountCapabilityStore,        dbcPath, "MountCapability.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sMountTypeStore,              dbcPath, "MountType.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sNameGenStore,                dbcPath, "NameGen.dbc");//14545
    for (uint32 i = 0; i < sNameGenStore.GetNumRows(); ++i)
        if (NameGenEntry const* entry = sNameGenStore.LookupEntry(i))
            sGenNameVectoArraysMap[entry->race].stringVectorArray[entry->gender].push_back(std::string(entry->name));
    sNameGenStore.Clear();

    LoadDBC(availableDbcLocales, bad_dbc_files, sMovieStore,                  dbcPath, "Movie.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sOverrideSpellDataStore,      dbcPath, "OverrideSpellData.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sPvPDifficultyStore,          dbcPath, "PvpDifficulty.dbc");//14545
    for (uint32 i = 0; i < sPvPDifficultyStore.GetNumRows(); ++i)
        if (PvPDifficultyEntry const* entry = sPvPDifficultyStore.LookupEntry(i))
            if (entry->bracketId > MAX_BATTLEGROUND_BRACKETS)
                ASSERT(false && "Need update MAX_BATTLEGROUND_BRACKETS by DBC data");

    LoadDBC(availableDbcLocales, bad_dbc_files, sQuestXPStore,                dbcPath, "QuestXP.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sQuestFactionRewardStore,     dbcPath, "QuestFactionReward.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sQuestPOIPointStore,          dbcPath, "QuestPOIPoint.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sQuestSortStore,              dbcPath, "QuestSort.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sRandomPropertiesPointsStore, dbcPath, "RandPropPoints.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sResearchBranchStore,         dbcPath, "ResearchBranch.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sResearchProjectStore,        dbcPath, "ResearchProject.dbc");//14545
    for (uint32 i = 0; i < sResearchProjectStore.GetNumRows(); ++i)
    {
        ResearchProjectEntry const* rp = sResearchProjectStore.LookupEntry(i);
        if (!rp || !rp->IsVaid())
            continue;

        sResearchProjectSet.insert(rp);
    }
    LoadDBC(availableDbcLocales, bad_dbc_files, sResearchSiteStore,           dbcPath, "ResearchSite.dbc");//14545
    for (uint32 i = 0; i < sResearchSiteStore.GetNumRows(); ++i)
    {
        ResearchSiteEntry const* rs = sResearchSiteStore.LookupEntry(i);
        if (!rs || !rs->IsValid())
            continue;

        ResearchSiteData& data = sResearchSiteDataMap[rs->ID];

        data.entry = rs;

        for (uint32 i = 0; i < sQuestPOIPointStore.GetNumRows(); ++i)
            if (QuestPOIPointEntry const* poi = sQuestPOIPointStore.LookupEntry(i))
                if (poi->POIId == rs->POIid)
                    data.points.push_back(ResearchPOIPoint(poi->x, poi->y));

        if (data.points.size() == 0)
            TC_LOG_DEBUG("server","Research site %u POI %u map %u has 0 POI points in DBC!", rs->ID, rs->POIid, rs->MapID);
    }

    //LoadDBC(availableDbcLocales, bad_dbc_files, sQuestPOIBlobStore,           dbcPath, "QuestPOIBlob.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sScenarioStore,               dbcPath, "Scenario.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sScenarioStepStore,           dbcPath, "ScenarioStep.dbc");
    for (uint32 i = 0; i < sScenarioStepStore.GetNumRows(); ++i)
    {
        ScenarioStepEntry const* entry = sScenarioStepStore.LookupEntry(i);
        if (!entry || !entry->m_criteriaTreeId)
            continue;

        if (!sCriteriaTreeStore.LookupEntry(entry->m_criteriaTreeId))
            continue;

        sScenarioCriteriaTreeStore.insert(entry->m_criteriaTreeId);
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sScalingStatDistributionStore, dbcPath, "ScalingStatDistribution.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sScalingStatValuesStore,      dbcPath, "ScalingStatValues.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSkillLineStore,              dbcPath, "SkillLine.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSkillLineAbilityStore,       dbcPath, "SkillLineAbility.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSoundEntriesStore,           dbcPath, "SoundEntries.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpecializationSpellStore,    dbcPath, "SpecializationSpells.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellStore,                  dbcPath, "Spell.dbc", &CustomSpellEntryfmt, &CustomSpellEntryIndex);//
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellMiscStore,              dbcPath, "SpellMisc.dbc", &CustomSpellMiscEntryfmt, &CustomSpellMiscEntryIndex);//18414

    for (uint32 i = 0; i < sSpecializationSpellStore.GetNumRows(); ++i)
    {
        SpecializationSpellEntry const* entry = sSpecializationSpellStore.LookupEntry(i);
        if (!entry)
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(entry->LearnSpell);
        if (!spellInfo)
            continue;

        SpellMiscEntry const* spellMisc = sSpellMiscStore.LookupEntry(spellInfo->SpellMiscId);
        if (!spellMisc)
            continue;

        if (spellMisc->AttributesEx8 & SPELL_ATTR8_MASTERY_SPECIALIZATION)
            sSpecializationMasterySpells[entry->SpecializationEntry].insert(spellInfo->Id);
    }

    for (uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        SpellCategoriesEntry const* spell = sSpellCategoriesStore.LookupEntry(i);
        if (spell && spell->Category)
            sSpellCategoryStore[spell->Category].insert(i);
    }

    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
        if (!skillLine)
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
        if (!spellInfo)
            continue;

        SpellMiscEntry const* spellMisc = sSpellMiscStore.LookupEntry(spellInfo->SpellMiscId);
        if (!spellMisc)
            continue;

        SpellLevelsEntry const* levels = sSpellLevelsStore.LookupEntry(spellInfo->SpellLevelsId);
        if (levels && levels->spellLevel)
            continue;

        if (spellMisc->Attributes & SPELL_ATTR0_PASSIVE)
        {
            for (uint32 i = 0; i < sCreatureFamilyStore.GetNumRows(); ++i)
            {
                CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(i);
                if (!cFamily)
                    continue;

                if (skillLine->skillId != cFamily->skillLine[0] && skillLine->skillId != cFamily->skillLine[1])
                    continue;

                if (skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                sPetFamilySpellsStore[i].insert(spellInfo->Id);
            }
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellScalingStore,           dbcPath,"SpellScaling.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellTotemsStore,            dbcPath,"SpellTotems.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellTargetRestrictionsStore,dbcPath,"SpellTargetRestrictions.dbc");//14545

    for(uint32 i = 1; i < sSpellTargetRestrictionsStore.GetNumRows(); ++i)
        if(SpellTargetRestrictionsEntry const *restriction = sSpellTargetRestrictionsStore.LookupEntry(i))
            sSpellRestrictionDiffMap[restriction->SpellId].restrictions.insert(restriction);

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellPowerStore,             dbcPath,"SpellPower.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sPowerDisplayStore,           dbcPath,"PowerDisplay.dbc");//17538
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellLevelsStore,            dbcPath,"SpellLevels.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellInterruptsStore,        dbcPath,"SpellInterrupts.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEquippedItemsStore,     dbcPath,"SpellEquippedItems.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellClassOptionsStore,      dbcPath,"SpellClassOptions.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCooldownsStore,         dbcPath,"SpellCooldowns.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellAuraOptionsStore,       dbcPath,"SpellAuraOptions.dbc", &CustomSpellAuraOptionsEntryfmt, &CustomSpellAuraOptionsEntryIndex);//17538
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellProcsPerMinuteStore,    dbcPath,"SpellProcsPerMinute.dbc");//17538
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellProcsPerMinuteModStore, dbcPath,"SpellProcsPerMinuteMod.dbc");//17538
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellAuraRestrictionsStore,  dbcPath,"SpellAuraRestrictions.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCastingRequirementsStore, dbcPath,"SpellCastingRequirements.dbc");//14545

    for (uint32 i = 0; i < sSpellProcsPerMinuteModStore.GetNumRows(); ++i)
    {
        if(SpellProcsPerMinuteModEntry const* sppm = sSpellProcsPerMinuteModStore.LookupEntry(i))
            sSpellProcsPerMinuteModEntryList[sppm->ProcsPerMinuteId].push_back(i);
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCategoriesStore,        dbcPath,"SpellCategories.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCategoryStores,          dbcPath,"SpellCategory.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEffectStore,            dbcPath,"SpellEffect.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEffectScalingStore,     dbcPath,"SpellEffectScaling.dbc");//16048

    for(uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
    {
        if(SpellEffectEntry const *spellEffect = sSpellEffectStore.LookupEntry(i))
        {
            if(spellEffect->EffectIndex > MAX_SPELL_EFFECTS)
                continue;

            if(spellEffect->EffectDifficulty)
                sSpellEffectDiffMap[spellEffect->EffectSpellId].effects[MAKE_PAIR16(spellEffect->EffectIndex, spellEffect->EffectDifficulty)] = spellEffect;
            else
                sSpellEffectMap[spellEffect->EffectSpellId].effects[spellEffect->EffectIndex] = spellEffect;

            if(spellEffect->Effect == SPELL_EFFECT_LEARN_SPELL)
                sRevertLearnSpellList[spellEffect->EffectTriggerSpell] = spellEffect->EffectSpellId;

            if(spellEffect->EffectTriggerSpell)
                sReversTriggerSpellList[spellEffect->EffectTriggerSpell] = spellEffect->EffectSpellId;
        }
    }

    for (uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        if (SpellEntry const * spell = sSpellStore.LookupEntry(i))
            if (const SpellEffectEntry* spellEffect = spell->GetSpellEffect(EFFECT_1))
                if (spellEffect->Effect == SPELL_EFFECT_SKILL && IsProfessionSkill(spellEffect->EffectMiscValue))
                    sSpellSkillingList.push_back(spell);
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCastTimesStore,         dbcPath, "SpellCastTimes.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellDurationStore,          dbcPath, "SpellDuration.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellFocusObjectStore,       dbcPath, "SpellFocusObject.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellItemEnchantmentStore,   dbcPath, "SpellItemEnchantment.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellItemEnchantmentConditionStore, dbcPath, "SpellItemEnchantmentCondition.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellRadiusStore,            dbcPath, "SpellRadius.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellRangeStore,             dbcPath, "SpellRange.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellRuneCostStore,          dbcPath, "SpellRuneCost.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellShapeshiftStore,        dbcPath, "SpellShapeshift.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellShapeshiftFormStore,    dbcPath, "SpellShapeshiftForm.dbc");//14545
    //LoadDBC(availableDbcLocales, bad_dbc_files, sStableSlotPricesStore,       dbcPath, "StableSlotPrices.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sSummonPropertiesStore,       dbcPath, "SummonProperties.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sTalentStore,                 dbcPath, "Talent.dbc");//16135
    for (uint32 i = 1; i < sTalentStore.GetNumRows(); ++i)
        if (TalentEntry const* entry = sTalentStore.LookupEntry(i))
            sTalentSpellList.insert(entry->spellId);

    LoadDBC(availableDbcLocales, bad_dbc_files, sTaxiNodesStore,              dbcPath, "TaxiNodes.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sTaxiPathStore,               dbcPath, "TaxiPath.dbc");//14545
    for (uint32 i = 1; i < sTaxiPathStore.GetNumRows(); ++i)
        if (TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->from][entry->to] = TaxiPathBySourceAndDestination(entry->ID, entry->price);
    uint32 pathCount = sTaxiPathStore.GetNumRows();

    //## TaxiPathNode.dbc ## Loaded only for initialization different structures
    LoadDBC(availableDbcLocales, bad_dbc_files, sTaxiPathNodeStore,           dbcPath, "TaxiPathNode.dbc");//14545
    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount, 0);                        // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
        {
            if (pathLength[entry->path] < entry->index + 1)
                pathLength[entry->path] = entry->index + 1;
        }
    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (uint32 i = 1; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);
    // fill data
    for (uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
            sTaxiPathNodesByPath[entry->path].set(entry->index, entry);

    // Initialize global taxinodes mask
    // include existed nodes that have at least single not spell base (scripted) path
    {
        std::set<uint32> spellPaths;
        for (uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
            if (SpellEffectEntry const* sInfo = sSpellEffectStore.LookupEntry (i))
                if (sInfo->Effect == SPELL_EFFECT_SEND_TAXI)
                    spellPaths.insert(sInfo->EffectMiscValue);

        memset(sTaxiNodesMask, 0, sizeof(sTaxiNodesMask));
        memset(sOldContinentsNodesMask, 0, sizeof(sOldContinentsNodesMask));
        memset(sHordeTaxiNodesMask, 0, sizeof(sHordeTaxiNodesMask));
        memset(sAllianceTaxiNodesMask, 0, sizeof(sAllianceTaxiNodesMask));
        memset(sDeathKnightTaxiNodesMask, 0, sizeof(sDeathKnightTaxiNodesMask));
        for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if (!node)
                continue;

            TaxiPathSetBySource::const_iterator src_i = sTaxiPathSetBySource.find(i);
            if (src_i != sTaxiPathSetBySource.end() && !src_i->second.empty())
            {
                bool ok = false;
                for (TaxiPathSetForSource::const_iterator dest_i = src_i->second.begin(); dest_i != src_i->second.end(); ++dest_i)
                {
                    if (!dest_i->second.price)
                        continue;

                    // not spell path
                    if (spellPaths.find(dest_i->second.ID) == spellPaths.end())
                    {
                        ok = true;
                        break;
                    }
                }

                if (!ok)
                    continue;
            }

            // valid taxi network node
            uint8  field   = (uint8)((i - 1) / 8);
            uint32 submask = 1 << ((i-1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] && node->MountCreatureID[0] != 32981)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[1] && node->MountCreatureID[1] != 32981)
                sAllianceTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] == 32981 || node->MountCreatureID[1] == 32981)
                sDeathKnightTaxiNodesMask[field] |= submask;

            // old continent node (+ nodes virtually at old continents, check explicitly to avoid loading map files for zone info)
            if (node->map_id < 2 || i == 82 || i == 83 || i == 93 || i == 94)
                sOldContinentsNodesMask[field] |= submask;

            // fix DK node at Ebon Hold and Shadow Vault flight master
            if (i == 315 || i == 333)
                ((TaxiNodesEntry*)node)->MountCreatureID[1] = 32981;
        }
    }

    //LoadDBC(availableDbcLocales, bad_dbc_files, sTeamContributionPointsStore, dbcPath, "TeamContributionPoints.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sTotemCategoryStore,          dbcPath, "TotemCategory.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sTransportAnimationStore,     dbcPath, "TransportAnimation.dbc");//17538
    for (uint32 i = 0; i < sTransportAnimationStore.GetNumRows(); ++i)
        if (TransportAnimationEntry const* entry = sTransportAnimationStore.LookupEntry(i))
            sTransportAnimationsByEntry[entry->transportEntry][entry->timeFrame] = entry;

    LoadDBC(availableDbcLocales, bad_dbc_files, sUnitPowerBarStore, dbcPath, "UnitPowerBar.dbc");//17538


    LoadDBC(availableDbcLocales, bad_dbc_files, sVehicleStore,                dbcPath, "Vehicle.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sVehicleSeatStore,            dbcPath, "VehicleSeat.dbc");//14545

    LoadDBC(availableDbcLocales, bad_dbc_files, sWMOAreaTableStore,           dbcPath, "WMOAreaTable.dbc");//14545
    for (uint32 i = 0; i < sWMOAreaTableStore.GetNumRows(); ++i)
        if (WMOAreaTableEntry const* entry = sWMOAreaTableStore.LookupEntry(i))
            sWMOAreaInfoByTripple[WMOAreaTableKey(entry->rootId, entry->adtId, entry->groupId)] = entry;
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldMapAreaStore,           dbcPath, "WorldMapArea.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldMapOverlayStore,        dbcPath, "WorldMapOverlay.dbc");//14545
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldSafeLocsStore,          dbcPath, "WorldSafeLocs.dbc");//14545

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        TC_LOG_ERROR("server", "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files (%d) not found by path: %sdbc", DBCFileCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("server", "Some required *.dbc files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    // Check loaded DBC files proper version
    if (!sAreaTableStore.LookupEntry(6863)          ||     // last area (areaflag) added in 5.4.8 (18414)
        !sCharTitlesStore.LookupEntry(389)     ||     // last char title added in 5.4.8 (18414)
        !sGemPropertiesStore.LookupEntry(2467) ||     // last gem property added in 5.4.8 (18414)
        !sMapStore.LookupEntry(1173)           ||     // last map added in 5.4.8 (18414)
        !sSpellStore.LookupEntry(163227)       )      // last spell added in 5.4.8 (18414)
    {
        TC_LOG_ERROR("server", "You have _outdated_ DBC files. Please extract correct versions from current using client.");
        exit(1);
    }

    TC_LOG_INFO("server", ">> Initialized %d DBC data stores in %u ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));
}

const std::string* GetRandomCharacterName(uint8 race, uint8 gender)
{
    uint32 size = sGenNameVectoArraysMap[race].stringVectorArray[gender].size();
    uint32 randPos = urand(0,size-1);

    return &sGenNameVectoArraysMap[race].stringVectorArray[gender][randPos];
}

SimpleFactionsList const* GetFactionTeamList(uint32 faction)
{
    FactionTeamMap::const_iterator itr = sFactionTeamMap.find(faction);
    if (itr != sFactionTeamMap.end())
        return &itr->second;

    return NULL;
}

std::list<uint32> GetItemSpecsList(uint32 ItemID)
{
    return sItemSpecsList[ItemID];
}

void AddSpecdtoItem(uint32 ItemID, uint32 SpecID)
{
    sItemSpecsList[ItemID].push_back(SpecID);
}

std::vector<ItemSpecOverrideEntry const*> const* GetItemSpecOverrides(uint32 itemId)
{
    auto itr = _itemSpecOverrides.find(itemId);
    if (itr != _itemSpecOverrides.end())
        return &itr->second;

    return nullptr;
}

AchievementEntry const* GetsAchievementByTreeList(uint32 criteriaTree)
{
    std::unordered_map<uint32, AchievementEntry const* >::const_iterator itr = sAchievementParentList.find(criteriaTree);
    if(itr != sAchievementParentList.end())
        return itr->second;
    return 0;
}


uint32 GetLearnSpell(uint32 trigerSpell)
{
    std::unordered_map<uint32, uint32 >::const_iterator itr = sRevertLearnSpellList.find(trigerSpell);
    if(itr != sRevertLearnSpellList.end())
        return itr->second;
    return 0;
}

uint32 GetSpellByTrigger(uint32 trigerSpell)
{
    std::unordered_map<uint32, uint32 >::const_iterator itr = sReversTriggerSpellList.find(trigerSpell);
    if(itr != sReversTriggerSpellList.end())
        return itr->second;
    return 0;
}

std::vector<CriteriaTreeEntry const*> const* GetCriteriaTreeList(uint32 parent)
{
    std::unordered_map<uint32, std::vector<CriteriaTreeEntry const*> >::const_iterator itr = sCriteriaTreeList.find(parent);
    if(itr != sCriteriaTreeList.end())
        return &itr->second;
    return NULL;
}

std::vector<ModifierTreeEntry const*> const* GetModifierTreeList(uint32 parent)
{
    std::unordered_map<uint32, std::vector<ModifierTreeEntry const*> >::const_iterator itr = sModifierTreeList.find(parent);
    if(itr != sModifierTreeList.end())
        return &itr->second;
    return NULL;
}

std::list<uint32> const* GetSpellProcsPerMinuteModList(uint32 PerMinId)
{
    std::unordered_map<uint32, std::list<uint32> >::const_iterator itr = sSpellProcsPerMinuteModEntryList.find(PerMinId);
    if(itr != sSpellProcsPerMinuteModEntryList.end())
        return &itr->second;
    return NULL;
}

char const* GetPetName(uint32 petfamily, uint32 /*dbclang*/)
{
    if (!petfamily)
        return NULL;
    CreatureFamilyEntry const* pet_family = sCreatureFamilyStore.LookupEntry(petfamily);
    if (!pet_family)
        return NULL;
    return pet_family->Name ? pet_family->Name : NULL;
}

SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty)
{
    if(spellId == 9262) //hack fix Segmentation fault
       return NULL;

    if(difficulty)
    {
        uint16 index = MAKE_PAIR16(effect, difficulty);
        SpellEffectDiffMap::const_iterator itr = sSpellEffectDiffMap.find(spellId);
        if(itr != sSpellEffectDiffMap.end())
        {
            SpellEffectsMap const* effects = &itr->second.effects;
            SpellEffectsMap::const_iterator itrsecond = effects->find(index);
            if(itrsecond != effects->end())
                return itrsecond->second;
        }
    }
    else
    {
        SpellEffectMap::const_iterator itr = sSpellEffectMap.find(spellId);
        if(itr != sSpellEffectMap.end())
            if(itr->second.effects[effect])
                return itr->second.effects[effect];
    }

    return NULL;
}

SpellEffectScalingEntry const* GetSpellEffectScalingEntry(uint32 effectId)
{
    return sSpellEffectScalingStore.LookupEntry(effectId);
}

SpellTotemsEntry const* GetSpellTotemEntry(uint32 spellId, uint8 totem)
{
    SpellTotemMap::const_iterator itr = sSpellTotemMap.find(spellId);
    if(itr == sSpellTotemMap.end())
        return NULL;

    return itr->second.totems[totem];
}

SpellTargetRestrictionsEntry const *GetSpellTargetRestrioctions(uint32 spellId, uint16 difficulty)
{
    SpellRestrictionDiffMap::const_iterator itr = sSpellRestrictionDiffMap.find(spellId);
    if(itr != sSpellRestrictionDiffMap.end())
    {
        SpellRestrictionMap const* restr = &itr->second.restrictions;
        for (SpellRestrictionMap::const_iterator sr = restr->begin(); sr != restr->end(); ++sr)
        {
            if ((*sr)->m_difficultyID == difficulty)
                return *sr;
        }
    }

    return NULL;
}

TalentSpellPos const* GetTalentSpellPos(uint32 spellId)
{
    return NULL;
    /*TalentSpellPosMap::const_iterator itr = sTalentSpellPosMap.find(spellId);
    if (itr == sTalentSpellPosMap.end())
        return NULL;

    return &itr->second;*/
}

uint32 GetTalentSpellCost(uint32 spellId)
{
    if (TalentSpellPos const* pos = GetTalentSpellPos(spellId))
        return pos->rank+1;

    return 0;
}

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid)
{
    auto i = sWMOAreaInfoByTripple.find(WMOAreaTableKey(int16(rootid), int8(adtid), groupid));
    if (i != sWMOAreaInfoByTripple.end())
        return i->second;

    return nullptr;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if (mapid != 530 && mapid != 571 && mapid != 732)   // speed for most cases
        return mapid;

    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->virtual_map_id >= 0 ? wma->virtual_map_id : wma->map_id;

    return mapid;
}

int32 GetMapFromZone(uint32 zoneId)
{
    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->virtual_map_id >= 0 ? wma->virtual_map_id : wma->map_id;

    return -1;
}

uint32 GetMaxLevelForExpansion(uint32 expansion)
{
    switch (expansion)
    {
        case CONTENT_1_60:
            return 60;
        case CONTENT_61_70:
            return 70;
        case CONTENT_71_80:
            return 80;
        case CONTENT_81_85:
            return 85;
        case CONTENT_86_90:
            return 90;
        default:
            break;

    }
    return 0;
}

/*
Used only for calculate xp gain by content lvl.
Calculation on Gilneas and group maps of LostIslands calculated as CONTENT_1_60.
*/
ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId)
{
    mapid = GetVirtualMapForMapAndZone(mapid, zoneId);
    if (mapid < 2)
        return CONTENT_1_60;

    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    if (!mapEntry)
        return CONTENT_1_60;

    // no need enum all maps from phasing
    if (mapEntry->rootPhaseMap >= 0)
        mapid = mapEntry->rootPhaseMap;

    switch (mapid)
    {
        case 648:   //LostIslands
        case 654:   //Gilneas
            return CONTENT_1_60;
        default:
            return ContentLevels(mapEntry->Expansion());
    }
}

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if (requiredTotemCategoryId == 0)
        return true;
    if (itemTotemCategoryId == 0)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if (!itemEntry)
        return false;
    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if (!reqEntry)
        return false;

    if (itemEntry->categoryType != reqEntry->categoryType)
        return false;

    return (itemEntry->categoryMask & reqEntry->categoryMask) == reqEntry->categoryMask;
}

void Zone2MapCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    std::swap(x, y);                                         // at client map coords swapped
    x = x*((maEntry->x2-maEntry->x1)/100)+maEntry->x1;
    y = y*((maEntry->y2-maEntry->y1)/100)+maEntry->y1;      // client y coord from top to down
}

void Map2ZoneCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    x = (x-maEntry->x1)/((maEntry->x2-maEntry->x1)/100);
    y = (y-maEntry->y1)/((maEntry->y2-maEntry->y1)/100);    // client y coord from top to down
    std::swap(x, y);                                         // client have map coords swapped
}

MapDifficulty const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty)
{
    MapDifficultyMap::const_iterator itr = sMapDifficultyMap.find(MAKE_PAIR32(mapId, difficulty));
    return itr != sMapDifficultyMap.end() ? &itr->second : NULL;
}

// TODO
MapDifficulty const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty)
{
    uint32 tmpDiff = difficulty;
    MapDifficulty const* mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff));
    if (!mapDiff)
    {
        DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(tmpDiff);
        if (!difficultyEntry || !difficultyEntry->m_fallbackDifficultyID)
            tmpDiff = REGULAR_DIFFICULTY;
        else
            tmpDiff = difficultyEntry->m_fallbackDifficultyID;

        // pull new data
        mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
        if (!mapDiff)
        {
            tmpDiff = MAN10_DIFFICULTY;
            mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // 10 normal
        }
    }

    difficulty = Difficulty(tmpDiff);
    return mapDiff;
}

PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level)
{
    PvPDifficultyEntry const* maxEntry = NULL;              // used for level > max listed level case
    for (uint32 i = 0; i < sPvPDifficultyStore.GetNumRows(); ++i)
    {
        if (PvPDifficultyEntry const* entry = sPvPDifficultyStore.LookupEntry(i))
        {
            // skip unrelated and too-high brackets
            if (entry->mapId != mapid || entry->minLevel > level)
                continue;

            // exactly fit
            if (entry->maxLevel >= level)
                return entry;

            // remember for possible out-of-range case (search higher from existed)
            if (!maxEntry || maxEntry->maxLevel < entry->maxLevel)
                maxEntry = entry;
        }
    }

    return maxEntry;
}

PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id)
{
    for (uint32 i = 0; i < sPvPDifficultyStore.GetNumRows(); ++i)
        if (PvPDifficultyEntry const* entry = sPvPDifficultyStore.LookupEntry(i))
            if (entry->mapId == mapid && entry->GetBracketId() == id)
                return entry;

    return NULL;
}

std::vector<uint32> const* GetTalentTreePrimarySpells(uint32 talentTree)
{
    return NULL;
    /*TalentTreePrimarySpellsMap::const_iterator itr = sTalentTreePrimarySpellsMap.find(talentTree);
    if (itr == sTalentTreePrimarySpellsMap.end())
        return NULL;

    return &itr->second;*/
}

uint32 GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->Type;

    return 0;
}

uint32 ScalingStatValuesEntry::GetStatMultiplier(uint32 inventoryType) const
{
    if (inventoryType < MAX_INVTYPE)
    {
        switch (inventoryType)
        {
            case INVTYPE_NON_EQUIP:
            case INVTYPE_BODY:
            case INVTYPE_BAG:
            case INVTYPE_TABARD:
            case INVTYPE_AMMO:
            case INVTYPE_QUIVER:
                return 0;
            case INVTYPE_HEAD:
            case INVTYPE_CHEST:
            case INVTYPE_LEGS:
            case INVTYPE_2HWEAPON:
            case INVTYPE_ROBE:
                return StatMultiplier[0];
            case INVTYPE_SHOULDERS:
            case INVTYPE_WAIST:
            case INVTYPE_FEET:
            case INVTYPE_HANDS:
            case INVTYPE_TRINKET:
                return StatMultiplier[1];
            case INVTYPE_NECK:
            case INVTYPE_WRISTS:
            case INVTYPE_FINGER:
            case INVTYPE_SHIELD:
            case INVTYPE_CLOAK:
            case INVTYPE_HOLDABLE:
                return StatMultiplier[2];
            case INVTYPE_RANGED:
            case INVTYPE_THROWN:
            case INVTYPE_RANGEDRIGHT:
            case INVTYPE_RELIC:
                return StatMultiplier[3];
            case INVTYPE_WEAPON:
            case INVTYPE_WEAPONMAINHAND:
            case INVTYPE_WEAPONOFFHAND:
                return StatMultiplier[4];
            default:
                break;
        }
    }
    return 0;
}

uint32 ScalingStatValuesEntry::GetArmor(uint32 inventoryType, uint32 armorType) const
{
    if (inventoryType <= INVTYPE_ROBE && armorType < 4)
    {
        switch (inventoryType)
        {
            case INVTYPE_NON_EQUIP:
            case INVTYPE_NECK:
            case INVTYPE_BODY:
            case INVTYPE_FINGER:
            case INVTYPE_TRINKET:
            case INVTYPE_WEAPON:
            case INVTYPE_SHIELD:
            case INVTYPE_RANGED:
            case INVTYPE_2HWEAPON:
            case INVTYPE_BAG:
            case INVTYPE_TABARD:
                break;
            case INVTYPE_SHOULDERS:
                return Armor[0][armorType];
            case INVTYPE_CHEST:
            case INVTYPE_ROBE:
                return Armor[1][armorType];
            case INVTYPE_HEAD:
                return Armor[2][armorType];
            case INVTYPE_LEGS:
                return Armor[3][armorType];
            case INVTYPE_FEET:
                return Armor[4][armorType];
            case INVTYPE_WAIST:
                return Armor[5][armorType];
            case INVTYPE_HANDS:
                return Armor[6][armorType];
            case INVTYPE_WRISTS:
                return Armor[7][armorType];
            case INVTYPE_CLOAK:
                return CloakArmor;
            default:
                break;
        }
    }
    return 0;
}

uint32 ScalingStatValuesEntry::GetDPSAndDamageMultiplier(uint32 subClass, bool isCasterWeapon, float* damageMultiplier) const
{
    if (!isCasterWeapon)
    {
        switch (subClass)
        {
            case ITEM_SUBCLASS_WEAPON_AXE:
            case ITEM_SUBCLASS_WEAPON_MACE:
            case ITEM_SUBCLASS_WEAPON_SWORD:
            case ITEM_SUBCLASS_WEAPON_DAGGER:
            case ITEM_SUBCLASS_WEAPON_THROWN:
                *damageMultiplier = 0.3f;
                return dpsMod[0];
            case ITEM_SUBCLASS_WEAPON_AXE2:
            case ITEM_SUBCLASS_WEAPON_MACE2:
            case ITEM_SUBCLASS_WEAPON_POLEARM:
            case ITEM_SUBCLASS_WEAPON_SWORD2:
            case ITEM_SUBCLASS_WEAPON_STAFF:
            case ITEM_SUBCLASS_WEAPON_FISHING_POLE:
                *damageMultiplier = 0.2f;
                return dpsMod[1];
            case ITEM_SUBCLASS_WEAPON_BOW:
            case ITEM_SUBCLASS_WEAPON_GUN:
            case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                *damageMultiplier = 0.3f;
                return dpsMod[4];
            case ITEM_SUBCLASS_WEAPON_Obsolete:
            case ITEM_SUBCLASS_WEAPON_EXOTIC:
            case ITEM_SUBCLASS_WEAPON_EXOTIC2:
            case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:
            case ITEM_SUBCLASS_WEAPON_MISCELLANEOUS:
            case ITEM_SUBCLASS_WEAPON_SPEAR:
            case ITEM_SUBCLASS_WEAPON_WAND:
                break;
        }
    }
    else
    {
        if (subClass <= ITEM_SUBCLASS_WEAPON_WAND)
        {
            uint32 mask = 1 << subClass;
            // two-handed weapons
            if (mask & 0x562)
            {
                *damageMultiplier = 0.2f;
                return dpsMod[3];
            }

            if (mask & (1 << ITEM_SUBCLASS_WEAPON_WAND))
            {
                *damageMultiplier = 0.3f;
                return dpsMod[5];
            }
        }
        *damageMultiplier = 0.3f;
        return dpsMod[2];
    }
    return 0;
}

float GetCurrencyPrecision(uint32 currencyId)
{
    CurrencyTypesEntry const * entry = sCurrencyTypesStore.LookupEntry(currencyId);

    return entry ? entry->GetPrecision() : 1.0f;
}

ResearchSiteEntry const* GetResearchSiteEntryById(uint32 id)
{
    ResearchSiteDataMap::const_iterator itr = sResearchSiteDataMap.find(id);
    if (itr == sResearchSiteDataMap.end())
        return NULL;

    return itr->second.entry;
}

LanguageWordsMap const* GetLanguageWordMap(uint32 lang_id)
{
    std::map<uint32, LanguageWordsMap>::const_iterator itr = sLanguageWordsMapStore.find(lang_id);
    return itr != sLanguageWordsMapStore.end() ? &itr->second : NULL;
}

std::vector<std::string> const* GetLanguageWordsBySize(uint32 lang_id, uint32 size)
{
    LanguageWordsMap const* wordMap = GetLanguageWordMap(lang_id);
    if (!wordMap)
        return NULL;

    std::map<uint32, std::vector<std::string> >::const_iterator itr = wordMap->find(size);
    return itr != wordMap->end() ? &itr->second : NULL;
}

bool MapEntry::IsDifficultyModeSupported(uint32 difficulty) const
{
    return IsValidDifficulty(difficulty, IsRaid());
}

bool IsValidDifficulty(uint32 diff, bool isRaid)
{
    if (diff == NONE_DIFFICULTY)
        return true;

    switch (diff)
    {
        case REGULAR_DIFFICULTY:
        case HEROIC_DIFFICULTY:
        case NORMAL_SCENARIO_DIFFICULTY:
        case HEROIC_SCENARIO_DIFFICULTY:
        case CHALLENGE_MODE_DIFFICULTY:
            return !isRaid;
        default:
            break;
    }

    return isRaid;
}

bool IsScenarioCriteriaTree(uint32 criteriaTreeId)
{
    return sScenarioCriteriaTreeStore.find(criteriaTreeId) != sScenarioCriteriaTreeStore.end();
}

