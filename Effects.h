#pragma once
#include "RuleManager.h"

namespace OIF::Effects
{
    void RemoveItem(const RuleContext& ctx);
    void DisableItem(const RuleContext& ctx);
    void EnableItem(const RuleContext& ctx);
    void SpawnItem(const RuleContext& ctx, const std::vector<ItemSpawnData>& itemsData);
    void SpawnSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData);
    void SpawnSpellOnItem(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData);
	void ApplySpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData);
    void SpawnActor(const RuleContext& ctx, const std::vector<ActorSpawnData>& actorsData);
    void SpawnImpactDataSet(const RuleContext& ctx, const std::vector<ImpactDataSetSpawnData>& impactsData);
    void SpawnImpact(const RuleContext& ctx, const std::vector<ImpactSpawnData>& impactsData);
    void SpawnExplosion(const RuleContext& ctx, const std::vector<ExplosionSpawnData>& explosionsData);
    void SwapItem(const RuleContext& ctx, const std::vector<ItemSpawnData>& itemsData);
    void PlaySound(const RuleContext& ctx, const std::vector<SoundSpawnData>& soundsData);
    void SpillInventory(const RuleContext& ctx);
    void SwapActor(const RuleContext& ctx, const std::vector<ActorSpawnData>& actorsData);
    void SpawnLeveledItem(const RuleContext& ctx, const std::vector<LvlItemSpawnData>& itemsData);
    void SwapLeveledItem(const RuleContext& ctx, const std::vector<LvlItemSpawnData>& itemsData);
    void SpawnLeveledSpell(const RuleContext& ctx, const std::vector<LvlSpellSpawnData>& spellsData);
    void SpawnLeveledSpellOnItem(const RuleContext& ctx, const std::vector<LvlSpellSpawnData>& spellsData);
    void SpawnLeveledActor(const RuleContext& ctx, const std::vector<LvlActorSpawnData>& actorsData);
    void SwapLeveledActor(const RuleContext& ctx, const std::vector<LvlActorSpawnData>& actorsData);
    void ApplyIngestible(const RuleContext& ctx, const std::vector<IngestibleApplyData>& ingestiblesData);
    void ApplyOtherIngestible(const RuleContext& ctx, const std::vector<IngestibleApplyData>& ingestiblesData);
    void SpawnLight(const RuleContext& ctx, const std::vector<LightSpawnData>& lightsData);
    void RemoveLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData);
    void EnableLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData);
    void DisableLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData);
    void PlayIdle(const RuleContext& ctx, const std::vector<PlayIdleData>& idleData);
    void SpawnEffectShader(const RuleContext& ctx, const std::vector<EffectShaderSpawnData>& effectShadersData);
    void SpawnEffectShaderOnItem(const RuleContext& ctx, const std::vector<EffectShaderSpawnData>& effectShadersData);
    void ToggleNode(const RuleContext& ctx, const std::vector<NodeData>& nodeData);
    //void ToggleShaderFlag(const RuleContext& ctx, const std::vector<ShaderFlagData>& shaderFlagsData);
    void UnlockItem(const RuleContext& ctx);
    void LockItem(const RuleContext& ctx);
    void ActivateItem(const RuleContext& ctx);
    void AddContainerItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData);
    void AddActorItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData);
    void RemoveContainerItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData);
    void RemoveActorItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData);
    void AddActorSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData);
    void RemoveActorSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData);
    void AddActorPerk(const RuleContext& ctx, const std::vector<PerkData>& perksData);
    void RemoveActorPerk(const RuleContext& ctx, const std::vector<PerkData>& perksData);
    void SpawnArtObject(const RuleContext& ctx, const std::vector<ArtObjectData>& artObjectData);
    void SpawnArtObjectOnItem(const RuleContext& ctx, const std::vector<ArtObjectData>& artObjectData);
    void ExecuteConsoleCommand(const RuleContext& ctx, const std::vector<StringData>& commandsData);
    void ExecuteConsoleCommandOnItem(const RuleContext& ctx, const std::vector<StringData>& commandsData);
	void ExecuteConsoleCommandOnSource(const RuleContext& ctx, const std::vector<StringData>& commandsData);
    void ShowNotification(const RuleContext& ctx, const std::vector<StringData>& notificationsData);
    void ShowMessageBox(const RuleContext& ctx, const std::vector<StringData>& messagesData);
    void AddBounty(const RuleContext& ctx, const std::vector<BountyData>& bountyData);
    void ScaleObject(const RuleContext& ctx, const std::vector<ScaleObjectData>& scaleData);
    void RestoreObjectScale(const RuleContext& ctx, const std::vector<RestoreScaleData>& restoreData);
    void ModActorValue(const RuleContext& ctx, const std::vector<ModActorValueData>& avData);
    void ApplyForce(const RuleContext& ctx, const std::vector<ForceApplyData>& forceData);
    void SpawnHazard(const RuleContext& ctx, const std::vector<HazardSpawnData>& hazardsData);
    void SwapHazard(const RuleContext& ctx, const std::vector<HazardSpawnData>& hazardsData);
    void DropHarvest(const RuleContext& ctx, const std::vector<HarvestSpawnData>& harvestsData);
    void ChangeWeather(const RuleContext& ctx, const std::vector<WeatherChangeData>& weatherData);
    void SwapBaseObject(const RuleContext& ctx, const std::vector<BaseObjectSwapData>& swapData);
}
