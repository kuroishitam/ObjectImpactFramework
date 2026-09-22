#pragma once
#include <shared_mutex>
#include <future>
#include <set>
#include <utility>

namespace OIF
{

	// Adapted from RavenKZP's Frostwalker (https://github.com/RavenKZP/Frostwalker), which
	// itself credits po3's Splashes-of-Skyrim for the underlying idea. Rather than asking a
	// specific TESObjectCELL for its water height - which only gives the right answer if
	// you already happen to be holding the cell that actually contains a_pos, not guaranteed
	// for e.g. a long-range shot that traveled well outside the shooter's own cell - this
	// walks TESWaterSystem's global list of currently active water volumes and finds the one
	// whose horizontal (x/y) bounds actually contain a_pos, independent of any particular
	// cell. Defined in EventSinks.cpp; declared here so RuleManager.cpp's isInWater filter
	// can share the exact same lookup used by the HitWater/HitGround event hooks.
	bool FindWaterHeightAtPosition(const RE::NiPoint3& a_pos, float& a_outHeight);

	// Collects every distinct physical Havok material found on a_target's collision bodies
	// (the whole 3D subtree is searched, not just the root node - most multi-part statics keep
	// their collision on child nodes). Chain: NiAVObject::GetCollisionObject() ->
	// bhkCollisionObject -> bhkRigidBody -> hkpRigidBody (native) -> hkpShape ->
	// hkpShape::userData (the bhkShape wrapper, where RE::MATERIAL_ID actually lives).
	// Returns false (a_outMaterials empty) if none could be read - e.g. 3D not loaded, no
	// Havok collision, or unset materials. Defined in EventSinks.cpp, declared here so
	// RuleManager.cpp's materials/materialsnot filter can use it.
	bool ResolveTargetMaterials(RE::TESObjectREFR* a_target, std::vector<RE::MATERIAL_ID>& a_outMaterials);

	// Casts a wide vertical Havok ray (a_verticalRange units up and down, terrain
	// collision layer only) through a_pos and, if it hits, overwrites a_pos.z with the
	// terrain height found there. Returns false (a_pos untouched) if nothing on the
	// terrain layer was hit - typically indoors, where there is no terrain layer to
	// hit at all. Defined in EventSinks.cpp; declared here so Effects.cpp's Spawn()
	// can use the same lookup to keep spawned objects from sinking below ground.
	bool FindGroundZAtPosition(RE::TESObjectCELL* a_cell, RE::NiPoint3& a_pos, float a_verticalRange = 300.0f);

	// Like FindGroundZAtPosition but across every solid collision layer (terrain AND
	// static/animstatic/ground/clutter-large/trap), returning the HIGHEST surface found
	// at a_probe's (x,y) within the band [probe.z - a_rangeBelow, probe.z + a_rangeAbove]
	// rather than overwriting a position. Most of Skyrim's visible ground near water is
	// static mesh rather than the terrain heightmap, so a terrain-only probe is not a
	// usable "is there ground here" test. The two ranges are separate so callers can
	// search downward only - the water gate needs the bed, not the bridge above it.
	// Defined in EventSinks.cpp.
	bool FindSolidSurfaceZAtPosition(RE::TESObjectCELL* a_cell, const RE::NiPoint3& a_probe, float a_rangeAbove, float a_rangeBelow, float& a_outZ);

	// True when solid ground at a_pos's (x,y) reaches up to (or above) a_waterHeight -
	// i.e. there is no real water depth here, so this spot is shore, not water. A cell's
	// water height applies to the whole cell and the water-volume footprint check is only
	// a coarse bounding box, so both happily report "water" across a wide band of dry land
	// around every lake, river and coastline; this is the gate that rejects it. Used by
	// every kHitWater path and by the isinwater filter. Defined in EventSinks.cpp.
	bool HasGroundAtOrAboveWater(const RE::NiPoint3& a_pos, float a_waterHeight);

	// Defined in RuleManager.cpp (used there by the actorvalue filter); declared
	// here so Effects.cpp's ModActorValue effect can resolve the same "Health"/
	// "Stamina"/"Magicka"/etc. names without duplicating the whole name table.
	RE::ActorValue GetActorValueFromString(const std::string& avName);

//███████╗███╗░░██╗██╗░░░██╗███╗░░░███╗░██████╗
//██╔════╝████╗░██║██║░░░██║████╗░████║██╔════╝
//█████╗░░██╔██╗██║██║░░░██║██╔████╔██║╚█████╗░
//██╔══╝░░██║╚████║██║░░░██║██║╚██╔╝██║░╚═══██╗
//███████╗██║░╚███║╚██████╔╝██║░╚═╝░██║██████╔╝
//╚══════╝╚═╝░░╚══╝░╚═════╝░╚═╝░░░░░╚═╝╚═════╝░

	enum class EventType
	{ 
		kNone,			// placeholder for no event (unavailable for users)
		kActivate, 
		kHit,			// TESHitEvent + TESMagicEffectApplyEvent + ExplosionHook + AttackBlockHook + Projectile Impact Hooks
		kGrab,
		kRelease,
		kThrow,
		kTelekinesis,
		kCellAttach,
		kCellDetach,
		kWeatherChange,
		kOnUpdate,
		kDestructionStageChange,
		kDrop,
		kHitGround,		// TESHitEvent-style impact that lands on bare terrain (RE::TESObjectLAND)
						// rather than any TESObjectREFR; ctx.target is nullptr, ctx.hitPos/ctx.hasHitPos
						// carry the impact location instead. Position-based spawn effects (spawnhazard,
						// spawnexplosion, spawnimpactdataset, spawnitem/spawnactor/spawnspell, playsound,
						// shownotification) work; anything that needs to act on the struck reference itself
						// (scaleobject, disableitem/enableitem, swapitem, formType/formID/keyword filters) does not.
		kHitWater		// Same shape as kHitGround, but for a splash on a water plane (interior water
						// form or exterior LOD water) instead of bare terrain; ctx.target is nullptr,
						// ctx.hitPos/ctx.hasHitPos carry the impact location. Same effect support/limits
						// as kHitGround apply.
	};

	enum class EffectType { 
		kRemoveItem, kDisableItem, kEnableItem,
		kSpawnItem, kSpawnSpell, kSpawnSpellOnItem, 
		kSpawnActor, kSpawnImpact, kSpawnImpactDataSet, 
		kSpawnExplosion, kSwapItem, kPlaySound, 
		kSpillInventory, kSwapActor, kSpawnLeveledItem,
		kSwapLeveledItem, kSpawnLeveledSpell, kSpawnLeveledSpellOnItem, 
		kSpawnLeveledActor, kSwapLeveledActor, kApplyIngestible, 
		kApplyOtherIngestible, kSpawnLight, kRemoveLight,
		kEnableLight, kDisableLight, kPlayIdle, 
		kSpawnEffectShader, kSpawnEffectShaderOnItem, kToggleNode, 
		//kToggleShaderFlag,
		kUnlockItem, kLockItem, kActivateItem, 
		kAddContainerItem, kAddActorItem, kRemoveContainerItem,
		kRemoveActorItem, kAddActorSpell, kRemoveActorSpell,
		kAddActorPerk, kRemoveActorPerk, kSpawnArtObject,
		kSpawnArtObjectOnItem, kExecuteConsoleCommand, kExecuteConsoleCommandOnItem, 
		kExecuteConsoleCommandOnSource, kShowNotification, kShowMessageBox, 
		kApplySpell, kAddBounty, kScaleObject, kRestoreScale, kModActorValue, kSpawnHazard,
		kSwapHazard, kDropHarvest, kChangeWeather, kSwapBaseObject, kApplyForce
	};


//███████╗██╗██╗░░░░░████████╗███████╗██████╗░
//██╔════╝██║██║░░░░░╚══██╔══╝██╔════╝██╔══██╗
//█████╗░░██║██║░░░░░░░░██║░░░█████╗░░██████╔╝
//██╔══╝░░██║██║░░░░░░░░██║░░░██╔══╝░░██╔══██╗
//██║░░░░░██║███████╗░░░██║░░░███████╗██║░░██║
//╚═╝░░░░░╚═╝╚══════╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝

	struct FormListEntry {
		std::uint32_t formID{ 0 };											// id of the form of the formlist
		int index{ -1 };													// -1 -use the entire list
	};

	struct LevelCondition {
		std::string operator_type; 											// ">=", "=", "<", ">", "<=", "!="
		int value{ 0 };
	};

	struct ActorValueCondition {
		std::string actorValue; 											// "Health", "Magicka", "Stamina", etc.
		std::string operator_type; 											// ">=", "=", "<", ">", "<=", "!="
		float value{ 0.0f };
	};

	struct ShoutWordsCondition {
		RE::FormID shoutID{ 0 };											// the shout to check
		std::uint32_t minWords{ 1 };										// minimum number of unlocked words (1-3)
		std::uint32_t maxWords{ 3 };										// maximum number of unlocked words (1-3)
	};

	struct InteractionCondition {
		std::uint32_t value{ 1 };
		std::uint32_t min{ 1 };
		std::uint32_t max{ 1 };
		bool useRandom = false;
		mutable bool hasRolled = false;
	};

	struct LimitCondition {
		std::uint32_t value{ 0 };
		std::uint32_t min{ 0 };
		std::uint32_t max{ 0 };
		bool useRandom = false;
		mutable bool hasRolled = false;
	};

	struct ChanceCondition {
		float value{ 100.0f };
		float min{ 100.0f };
		float max{ 100.0f };
		bool useRandom = false;
	};

	struct CountCondition {
		std::uint32_t value{ 1 };
		std::uint32_t min{ 1 };
		std::uint32_t max{ 1 };
		bool useRandom = false;
	};

	struct ScaleCondition {
		float value{ -1.0f };
		float min{ -1.0f };
		float max{ -1.0f };
		bool useRandom = false;
	};

	struct RadiusCondition {
		float value{ 100.0 };
		float min{ 100.0 };
		float max{ 100.0 };
		bool useRandom = false;
	};

	struct AmountCondition {
		float value{ 0.0f };										// modav: positive restores/adds, negative damages/reduces
		float min{ 0.0f };
		float max{ 0.0f };
		bool useRandom = false;
	};

	struct TimerCondition {
		float value{ 0.0f };
		float min{ 0.0f };
		float max{ 0.0f };
		bool useRandom = false;
	};

	struct NearbyEntry
	{
		RE::TESForm* form{ nullptr };										// form to match
		RadiusCondition radius;												// radius to match
	};

	struct TimerEntry {
		TimerCondition time;												// time in seconds
		std::uint32_t matchFilterRecheck{ 0 }; 								// 0 - no re-check, 1 - re-check after timer expires
	};

	struct TimeCondition {
		std::string field;													// "hour", "minute", "day", "month", "year", "dayofweek"
		std::string operator_type;											// ">=", "=", "<", ">", "<=", "!="
		float value{ 0.0f };
	};

	struct Filter {
		// General filters
		std::unordered_set<RE::FormType> formTypes;           	 			// base form type to match
		std::unordered_set<RE::FormType> formTypesNot;       				// base form type to avoid
		std::unordered_set<RE::FormID> formIDs;           	  				// FormIDs to match
		std::unordered_set<RE::FormID> formIDsNot;       	  				// FormIDs to avoid
		std::unordered_set<RE::FormID> references;         	  			// specific placed reference FormIDs to match (the reference itself, not its base object)
		std::unordered_set<RE::FormID> referencesNot;       	  			// specific placed reference FormIDs to avoid
		std::vector<FormListEntry> formLists; 								// formlists contents to match
		std::vector<FormListEntry> formListsNot; 							// formlists contents to avoid 
		std::set<RE::BGSKeyword*> keywords;        	  						// must have ANY of these keywords
		std::set<RE::BGSKeyword*> keywordsNot;      						// must NOT have ANY of these keywords
		std::uint32_t questItemStatus{ 0 };									// 0 - not a quest item, 1 - an item with an alias, 2 - a quest item, 3 - all/undefined
		std::int32_t lockLevel{ -2 };										// -2 - no lock level required, -1+ - specific lock level required
		std::int32_t lockLevelNot{ -2 };									// -2 - no lock level to avoid, -1+ - specific lock level to avoid
		ChanceCondition chance;												// the chance of 0‑100 % for the event to trigger
		InteractionCondition interactions;									// number of interactions required to satisfy filter
		LimitCondition limit;												// number of interactions to stop the effect
		std::uint32_t isInitiallyDisabled{ 2 }; 							// 0 - not initially disabled, 1 - initially disabled, 2 - all/undefined
		std::int32_t destructionStage{ -1 }; 								// destruction stage to match, -1 - any stage
		std::uint32_t isStacked{ 2 };										// 0 - not in stack, 1 - in stack, 2 - all/undefined
		std::uint32_t isParented{ 2 };										// 0 - no enable parent, 1 - has enable parent, 2 - all/undefined
		std::uint32_t isOwned{ 2 };											// 0 - no ownership, 1 - has ownership, 2 - all/undefined, 3 - unowned or player-owned, 4 - has ownership except player-owned
		
		// New hit-specific filters
		std::unordered_set<std::string> weaponsTypes;          	 			// weapon type categories
		std::unordered_set<std::string> weaponsTypesNot;      				// weapon type categories to avoid
		std::set<RE::BGSKeyword*> weaponsKeywords;	   						// specific weapons or spells keywords
		std::set<RE::BGSKeyword*> weaponsKeywordsNot;						// specific weapons or spells keywords to avoid
		std::set<RE::TESForm*> weapons;       	 							// specific weapons or spells
		std::set<RE::TESForm*> weaponsNot;       							// specific weapons or spells to avoid
		std::uint32_t allowProjectiles{ 1 }; 								// 0 - no projectiles, 1 - allow projectiles
		std::set<RE::TESForm*> projectiles;  	  							// specific projectiles
		std::set<RE::TESForm*> projectilesNot; 								// specific projectiles to avoid
		std::unordered_set<std::string> attackTypes;           				// attack types
		std::unordered_set<std::string> attackTypesNot;       				// attack types to avoid
		std::unordered_set<std::string> deliveryTypes;           			// delivery types
		std::unordered_set<std::string> deliveryTypesNot;       			// delivery types to avoid

		// Location and weather filters
		std::unordered_set<RE::FormID> locations; 							// location IDs
		std::unordered_set<RE::FormID> locationsNot; 						// location IDs to avoid
		std::set<RE::BGSKeyword*> locationKeywords;						// the location or any of its parent locations must have ANY of these keywords
		std::set<RE::BGSKeyword*> locationKeywordsNot;						// the location or any of its parent locations must NOT have ANY of these keywords
		std::unordered_set<RE::FormID> weathers; 							// weather IDs
		std::unordered_set<RE::FormID> weathersNot; 						// weather IDs to avoid
		std::uint32_t isInterior{ 2 };										// 0 - not interior, 1 - interior, 2 - all/undefined
		std::uint32_t isTeleportDoor{ 2 };									// 0 - door that doesn't teleport (opens in place), 1 - door that teleports (load door), 2 - all/undefined; only meaningful when the target is a door
		std::uint32_t position{ 3 };										// 0 - below player's middle, 1 - player's middle, 2 - above player's middle, 3 - any position
		std::uint32_t isInWater{ 3 };										// 0 - not touching water, 1 - on the surface (straddles the waterline), 2 - fully submerged, 3 - all/undefined
		// Physical Havok material of the target's collision shape (e.g. "stone", "wood",
		// "metal") - matched as a case-insensitive substring against the target's
		// RE::MATERIAL_ID name (see ResolveTargetMaterials in RuleManager.h), so "stone"
		// matches kStone, kStoneStairs, kStoneHeavy, etc. An object can have several collision
		// bodies: `materials` matches if ANY of them matches, `materialsNot` rejects if ANY of
		// them matches. If no material can be read at all, `materials` fails but `materialsNot`
		// passes (nothing known to exclude). Currently only resolved for rules with a real
		// ctx.target (hit-on-object/activate-object); kHitGround/kHitWater hits (no target)
		// never match either set yet.
		std::unordered_set<std::string> materials;
		std::unordered_set<std::string> materialsNot;

		// Proximity filters
		std::vector<NearbyEntry> nearby;           							// nearby items to match
		std::vector<NearbyEntry> nearbyNot;     							// nearby items to avoid

		// Time filters
		std::vector<TimeCondition> time; 									// time conditions, e.g. ["hour >= 12", "dayofweek = 1"]
		std::vector<TimeCondition> timeNot; 								// time conditions to avoid
		TimerEntry timer;													// timer for the event

		// Actor values and inventory filters
		std::unordered_set<RE::FormID> perks;           					// perks to match
		std::unordered_set<RE::FormID> perksNot;       						// perks to avoid 
		std::unordered_set<RE::FormID> spells; 								// has any of these spells
		std::unordered_set<RE::FormID> spellsNot; 							// does not have any of these spells
		std::unordered_set<RE::FormID> shouts; 								// knows any of these shouts
		std::unordered_set<RE::FormID> shoutsNot; 							// does not know any of these shouts
		std::vector<ShoutWordsCondition> shoutWordsKnown; 					// number of unlocked words of a specific shout, e.g. {"shout": "...", "minWords": 1, "maxWords": 3}
		std::unordered_set<RE::FormID> hasItem; 							// has any of these items
		std::unordered_set<RE::FormID> hasItemNot; 							// does not have any of these items
		std::unordered_set<RE::FormID> isEquip; 							// has any of these weapons/armor/etc. currently equipped or worn
		std::unordered_set<RE::FormID> isEquipNot; 							// does not have any of these weapons/armor/etc. currently equipped or worn
		std::vector<LevelCondition> level; 									// level conditions, e.g. [">= 10"]
		std::vector<LevelCondition> levelNot; 								// level conditions to avoid, e.g. {"<", 5}
		std::vector<ActorValueCondition> actorValue; 						// actor value conditions, e.g. ["Health >= 50.0"]
		std::vector<ActorValueCondition> actorValueNot; 					// actor value conditions to avoid, e.g. ["Magicka < 20.0"]
		std::set<RE::BGSKeyword*> actorKeywords;           					// actor keywords to match
		std::set<RE::BGSKeyword*> actorKeywordsNot; 	 					// actor keywords to avoid
		std::set<RE::TESRace*> actorRaces;           						// actor races to match
		std::set<RE::TESRace*> actorRacesNot; 	 							// actor races to avoid
		std::uint32_t isSneaking{ 2 }; 										// 0 - not sneaking, 1 - sneaking, 2 - all/undefined
		std::uint32_t isSwimming{ 2 }; 										// 0 - not swimming, 1 - swimming, 2 - all/undefined
		std::uint32_t isInCombat{ 2 }; 										// 0 - not in combat, 1 - in combat, 2 - all/undefined
		std::uint32_t isMounted{ 2 }; 										// 0 - not mounted, 1 - mounted, 2 - all/undefined
		std::uint32_t isDualCasting{ 2 }; 									// 0 - not dual casting, 1 - dual casting, 2 - all/undefined
		std::uint32_t isSprinting{ 2 }; 									// 0 - not sprinting, 1 - sprinting, 2 - all/undefined
		std::uint32_t isWeaponDrawn{ 2 }; 									// 0 - not weapon drawn, 1 - weapon drawn, 2 - all/undefined
		std::uint32_t isFirstPerson{ 2 };									// 0 - not in first person, 1 - in first person, 2 - all/undefined
		std::uint32_t isThirdPerson{ 2 };									// 0 - not in third person, 1 - in third person, 2 - all/undefined

		// Other filters
		std::unordered_set<std::string> requiredPlugins; 					// required plugins
		std::unordered_set<std::string> requiredPluginsNot; 				// plugins to avoid
		std::unordered_set<std::string> requiredDLLs; 						// required DLLs
		std::unordered_set<std::string> requiredDLLsNot; 					// DLLs to avoid
	};


//███████╗███████╗███████╗███████╗░█████╗░████████╗░██████╗
//██╔════╝██╔════╝██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝
//█████╗░░█████╗░░█████╗░░█████╗░░██║░░╚═╝░░░██║░░░╚█████╗░
//██╔══╝░░██╔══╝░░██╔══╝░░██╔══╝░░██║░░██╗░░░██║░░░░╚═══██╗
//███████╗██║░░░░░██║░░░░░███████╗╚█████╔╝░░░██║░░░██████╔╝
//╚══════╝╚═╝░░░░░╚═╝░░░░░╚══════╝░╚════╝░░░░╚═╝░░░╚═════╝░

	struct EffectExtendedData {
		RE::TESForm* formID{ nullptr };  							   		// the thing to spawn/cast/play
		std::vector<FormListEntry> formLists; 								// formlists contents to spawn/cast/play
		CountCondition count;      											// the amount of items to spawn/cast/play
		ChanceCondition chance;												// the chance of 0‑100 %
		TimerEntry timer; 													// the timer for the effect, e.g. 1.0f for 1 second wait before applying
		float duration{ 1.f }; 												// the duration of the effect
		RadiusCondition radius;												// the radius of the DetachNearbyLight effect
		ScaleCondition scale;												// the scale of the spawned item
		std::string string;													// the text string to use in different effects
		std::vector<std::string> strings;									// the test strings to use in different effects
		std::uint32_t nonDeletable{ 0 }; 									// 1 if the form should not be deleted after disabling
		std::uint32_t spawnType{ 4 };										// the type of spawn
		std::uint32_t fade{ 1 };											// 0 if the effect should not fade, 1 if it should fade out
		std::uint32_t mode{ 0 };											// the type of search for the node, 0 - by name, 1 - by type
		//std::vector<std::string> flagNames;								// the name of the shader flags to toggle
		std::uint32_t rank{ 0 };											// the rank of the perk to apply
		std::uint32_t violent{ 0 };										// 1 if the AddBounty effect should count as a violent crime
		bool isFormList = false;											// true if the form is a BGSListForm
		int index = -1;														// index of the form in the list
		bool relative{ false };											// scaleobject only: if true, "scale" multiplies the target current scale instead of setting it outright
		AmountCondition amount;												// modav: amount to restore (positive) or damage (negative)
		bool affectSource{ true };											// modav: whether the actor who caused the hit can be affected if in radius
		bool affectPlayer{ true };											// modav: whether the player specifically can be affected if in radius
		bool immediate{ false };											// changeweather: true skips the usual transition and switches instantly
		bool resetInventory{ false };										// swapbaseobject: forwarded to Enable() - whether to reset the reference's inventory as part of the collision/3D refresh
	};

	struct Effect {
		EffectType type{ EffectType::kSpawnItem }; 							// the type of effect
		std::vector<std::pair<RE::TESForm*, EffectExtendedData>> items;		// the vector of items to utilize
		std::uint32_t atStage{ 0 };											// 0: always fires; >0: only fires when this rule's persistent stage counter equals this value - lets a rule sequence distinct effects across successive interactions (1st interaction -> stage 1's effects, 2nd -> stage 2's, etc.) without earlier stages firing again
	};

	struct InventoryData {
		RE::TESBoundObject* item;
		CountCondition count;
	};

	struct ItemSpawnData {
		RE::TESBoundObject* item;
		std::string string; 
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		ScaleCondition scale;
		std::uint32_t nonDeletable{ 0 };
	};

	struct SpellSpawnData {
		RE::SpellItem* spell;
		CountCondition count;
		RadiusCondition radius;
	};

	struct PerkData {
		RE::BGSPerk* perk;
		std::uint32_t rank;
	};

	struct ActorSpawnData {
		RE::TESNPC* npc;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		ScaleCondition scale;
		std::uint32_t nonDeletable{ 0 };
	};

	struct LvlItemSpawnData {
		RE::TESLevItem* item;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		ScaleCondition scale;
		std::uint32_t nonDeletable{ 0 };
	};

	struct LvlSpellSpawnData {
		RE::TESLevSpell* spell;
		CountCondition count;
		RadiusCondition radius;
	};

	struct LvlActorSpawnData {
		RE::TESLevCharacter* npc;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		ScaleCondition scale;
		std::uint32_t nonDeletable{ 0 };
	};

	struct ImpactDataSetSpawnData {
		RE::BGSImpactDataSet* impact;
		CountCondition count;
	};

	// A single Impact record (IPCT) played directly, bypassing any Impact Data Set.
	struct ImpactSpawnData {
		RE::BGSImpactData* impact;
		CountCondition count;
	};

	struct ExplosionSpawnData {
		RE::BGSExplosion* explosion;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
	};

	struct SoundSpawnData {
		RE::BGSSoundDescriptorForm* sound;
		CountCondition count;
	};

	struct IngestibleApplyData {
		RE::MagicItem* ingestible;
		CountCondition count;
		RadiusCondition radius;
	};

	struct LightSpawnData {
		RE::TESObjectLIGH* light;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		ScaleCondition scale;
	};

	struct LightRemoveData {
		RadiusCondition radius;
	};

	struct PlayIdleData {
        RE::Actor* actor;
        std::string string;
        float duration{ 1.0f };
    };

	struct EffectShaderSpawnData {
		RE::TESEffectShader* effectShader;
		CountCondition count;
		RadiusCondition radius;
		float duration{ -1.0f };
	};	

	struct NodeData {
		std::uint32_t mode;
		std::vector<std::string> strings; 
	};

	/*struct ShaderFlagData {
		std::uint32_t mode;
		std::vector<std::string> flagNames;
		std::vector<std::string> strings;
		float chance{ 100.f };
	};*/

	struct ArtObjectData {
		RE::BGSArtObject* artObject;
		CountCondition count;
		RadiusCondition radius;
		float duration{ -1.0f };
	};

	struct StringData {
		std::string string;
		RadiusCondition radius;
	};

	struct BountyData {
		CountCondition amount;
		bool violent{ false };
	};

	struct ScaleObjectData {
		ScaleCondition scale;												// the scale/factor to apply to the target object
		std::uint32_t fade{ 1 };											// 0: no fade when the collision refresh re-enables the object, 1 (default): fade back in
		bool relative{ false };											// if true, multiply the target current scale by "scale" instead of setting it outright
	};

	struct RestoreScaleData {
		std::uint32_t fade{ 1 };											// 0: no fade when the collision refresh re-enables the object, 1 (default): fade back in
	};

	struct ModActorValueData {
		std::string actorValue;												// "Health", "Stamina", "Magicka", or any other actor value name
		AmountCondition amount;											// positive = restore/add, negative = damage/reduce
		RadiusCondition radius;											// search radius around the impact point, in game units
		bool affectSource{ true };										// whether the actor who caused the hit can be affected if in radius
		bool affectPlayer{ true };										// whether the player specifically can be affected if in radius
	};

	struct ForceApplyData {
		AmountCondition force;												// push magnitude, in game units/second. JSON key "amount" (same field ModActorValue's magnitude uses), or "force" as a plain alias for it - see the applyforce-only alias in RuleManager::ParseJSON
		RadiusCondition radius;											// search radius around the source position, in game units
		bool affectSource{ true };										// whether the actor who caused the hit can be pushed if in radius
		bool affectPlayer{ true };										// whether the player specifically can be pushed if in radius
	};

	struct WeatherChangeData {
		RE::TESWeather* weather{ nullptr };								// the weather to force
		bool immediate{ false };											// true: skip the transition and switch instantly, false: fade in normally
	};

	struct BaseObjectSwapData {
		RE::TESBoundObject* newBase{ nullptr };							// the base object to swap the target reference to, in place
		std::uint32_t fade{ 1 };											// 0: no fade when the collision refresh re-enables the object, 1 (default): fade back in
		bool resetInventory{ false };										// forwarded to Enable() - see RE::TESObjectREFR::Enable
	};

	struct HazardSpawnData {
		RE::BGSHazard* hazard;
		std::string string;
		CountCondition count;
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
		float duration{ -1.0f };			// seconds before the spawned hazard is removed; <= 0 means it is never auto-removed
		std::uint32_t nonDeletable{ 0 };	// swaphazard only: 1 - disable the swapped-out target instead of deleting it
	};

	struct HarvestSpawnData {
		CountCondition count;				// how many copies to drop on the ground (default 1, same as other spawn effects) - independent of how many are removed from inventory, which is always exactly 1 (the vanilla engine's own per-activation grant - Flora/Tree records have no "quantity" field)
		ScaleCondition scale;				// optional scale for the dropped copies; value < 0 (default) leaves them at native scale
		std::uint32_t spawnType{ 4 };
		std::uint32_t fade{ 1 };
	};


//███████╗██╗░░░██╗███████╗███╗░░██╗████████╗░██████╗
//██╔════╝██║░░░██║██╔════╝████╗░██║╚══██╔══╝██╔════╝
//█████╗░░╚██╗░██╔╝█████╗░░██╔██╗██║░░░██║░░░╚█████╗░
//██╔══╝░░░╚████╔╝░██╔══╝░░██║╚████║░░░██║░░░░╚═══██╗
//███████╗░░╚██╔╝░░███████╗██║░╚███║░░░██║░░░██████╔╝
//╚══════╝░░░╚═╝░░░╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═════╝░

	struct RuleContext
	{
		// General context
		EventType event;
		RE::Actor* source{ nullptr };
		RE::TESObjectREFR* target{ nullptr };

		// Hit-specific context
		RE::TESForm* attackSource{ nullptr };
		RE::TESForm* projectileSource{ nullptr };
		std::string weaponType;
		std::string attackType;
		std::string deliveryType;
		bool isHitEvent{ false };

		// Additional context
		RE::TESWeather* weather{ nullptr };
		std::int32_t destructionStage{ -1 };

		// kHitGround/kHitWater context: populated instead of "target" when the hit
		// landed on bare terrain (RE::TESObjectLAND) or a water plane, neither of
		// which is a TESObjectREFR and so cannot be a target. hasHitPos tells
		// filters/effects whether hitPos is actually valid (it is always set
		// together with kHitGround/kHitWater).
		RE::NiPoint3 hitPos{};
		bool hasHitPos{ false };
	};

	// RuleContext stores raw RE::Actor* / RE::TESObjectREFR* pointers. That is fine while a rule
	// is evaluated synchronously, but three code paths keep a RuleContext across a delay and use
	// it again on a later frame:
	//   - RuleManager::ApplyEffect's SKSE AddTask hop
	//   - Trigger()'s rule-level "timer" (std::async sleep -> AddTask)
	//   - ProcessEffect's effect-level "timer" (std::async sleep -> AddTask)
	// If the referenced object is freed while waiting (cell unload, another effect deleting it,
	// fade-out cleanup...) the copied pointer dangles and the heap slot can be reused by an
	// unrelated object. IsDeleted() on such a slot just reads garbage bits and can happily pass,
	// so the stale pointer used to be dereferenced anyway. Observed crash:
	// ExecuteConsoleCommandOnItem -> ExecuteCommand -> TESObjectCELL::IsAttached on a "cell"
	// of 0xFFFFFFFFFFFFFFFF, with the "target reference" now pointing at a BSDismemberSkinInstance.
	//
	// DeferredRefs snapshots the pointers as handles while the context is still valid and
	// re-resolves them right before use. A handle to a freed reference resolves to null, and the
	// NiPointers handed back keep the references alive for as long as the task runs.
	struct DeferredRefs
	{
		RE::ObjectRefHandle source{};
		RE::ObjectRefHandle target{};

		// Call while ctx's pointers are still known to be valid (i.e. before any delay).
		static DeferredRefs Capture(const RuleContext& ctx)
		{
			DeferredRefs refs;
			if (ctx.source) refs.source = ctx.source->CreateRefHandle();
			if (ctx.target) refs.target = ctx.target->CreateRefHandle();
			return refs;
		}

		// NOTE for callers: Resolve needs a NON-const RuleContext. A plain by-copy capture of a
		// `const RuleContext&` parameter keeps its const even in a `mutable` lambda, so the task
		// lambdas use an init-capture (`ctx = ctx`), which deduces a plain, writable RuleContext.
		//
		// Replaces ctx.source / ctx.target with freshly resolved pointers. The stale pointers in
		// ctx are only tested against null (to know whether one existed), never dereferenced.
		//
		// Returns false if a source existed but is gone now. A vanished target is reported by
		// setting ctx.target to nullptr, so effects that never needed a target (AddBounty,
		// ChangeWeather, ...) still run, while the callers' existing "target is null/deleted"
		// checks reject everything that does. Ground/water hits never had a target to begin with,
		// so they are left alone.
		bool Resolve(RuleContext& ctx,
			RE::NiPointer<RE::TESObjectREFR>& sourceKeepAlive,
			RE::NiPointer<RE::TESObjectREFR>& targetKeepAlive) const
		{
			if (ctx.source) {
				sourceKeepAlive = source.get();
				ctx.source = sourceKeepAlive ? sourceKeepAlive->As<RE::Actor>() : nullptr;
				if (!ctx.source) {
					ctx.target = nullptr;
					return false;
				}
			}
			if (ctx.target) {
				targetKeepAlive = target.get();
				ctx.target = targetKeepAlive.get();
			}
			return true;
		}
	};

	struct Rule {
		std::vector<EventType> events;
		Filter filter;
		std::vector<Effect> effects;
		int dynamicIndex{ 0 };
	};

	struct Key {
		std::uint32_t sourceID;
		std::uint32_t targetID; 
		std::uint16_t ruleIdx;
		
		bool operator<(const Key& other) const {
			return std::tie(sourceID, targetID, ruleIdx) < std::tie(other.sourceID, other.targetID, other.ruleIdx);
		}
	};

	struct UpdateFilter {
		std::unordered_set<RE::FormType> formTypes;
		std::unordered_set<RE::FormID> formIDs;
		std::unordered_set<RE::FormID> formLists;
		std::unordered_set<RE::BGSKeyword*> keywords;
		
		bool IsEmpty() const {
			return formTypes.empty() && formIDs.empty() && formLists.empty() && keywords.empty();
		}
		
		bool Matches(RE::TESObjectREFR* ref) const {
			if (!ref || !ref->GetBaseObject()) return false;
			
			auto* baseObj = ref->GetBaseObject();

			if (IsEmpty()) return true;
			bool hasMatch = false;

			if (!formTypes.empty()) {
				if (formTypes.contains(baseObj->GetFormType())) {
					hasMatch = true;
				}
			}

			if (!hasMatch && !formIDs.empty()) {
				if (formIDs.contains(baseObj->GetFormID())) {
					hasMatch = true;
				}
			}

			if (!hasMatch && !formLists.empty()) {
				for (const auto& formID : formLists) {
					auto* list = RE::TESForm::LookupByID<RE::BGSListForm>(formID);
					if (list) {
						for (auto* listItem : list->forms) {
							if (listItem && listItem->GetFormID() == baseObj->GetFormID()) {
								hasMatch = true;
								break;
							}
						}
						if (hasMatch) break;
					}
				}
			}
			
			if (!hasMatch && !keywords.empty()) {
				if (auto* kwForm = baseObj->As<RE::BGSKeywordForm>()) {
					for (auto* keyword : keywords) {
						if (keyword && kwForm->HasKeyword(keyword)) {
							hasMatch = true;
							break;
						}
					}
				}
			}
			
			return hasMatch;
		}
	};
	

//███╗░░░███╗░█████╗░███╗░░██╗░█████╗░░██████╗░███████╗██████╗░
//████╗░████║██╔══██╗████╗░██║██╔══██╗██╔════╝░██╔════╝██╔══██╗
//██╔████╔██║███████║██╔██╗██║███████║██║░░██╗░█████╗░░██████╔╝
//██║╚██╔╝██║██╔══██║██║╚████║██╔══██║██║░░╚██╗██╔══╝░░██╔══██╗
//██║░╚═╝░██║██║░░██║██║░╚███║██║░░██║╚██████╔╝███████╗██║░░██║
//╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝░╚═════╝░╚══════╝╚═╝░░╚═╝

	class RuleManager {

	private:
		RuleManager(const RuleManager&) = delete;
		RuleManager& operator=(const RuleManager&) = delete;

		RuleManager() = default;

		void ParseJSON(const std::filesystem::path& path);
		bool MatchFilter(const Filter& f, const RuleContext& ctx, Rule& currentRule) const;
		void ApplyEffect(const Effect& eff, const RuleContext& ctx, Rule& currentRule) const;

		template <typename FormT, typename DataT, typename CreateDataFunc, typename ApplyEffectFunc>
		void ProcessEffect(const Effect& eff, const RuleContext& ctx, const Rule& currentRule, bool needsForm, CreateDataFunc createData, ApplyEffectFunc applyEffect) const {
			static thread_local std::mt19937 rng(std::random_device{}());
			std::vector<DataT> dataList;
			for (const auto& [form, extData] : eff.items) {
				// Roll for random chance if needed
				float currentChance = extData.chance.value;
				if (extData.chance.useRandom) {
					std::uniform_real_distribution<float> chanceDist(extData.chance.min, extData.chance.max);
					currentChance = chanceDist(rng);
				}

				float roll = std::uniform_real_distribution<float>(0.f, 100.f)(rng);
				if (roll > currentChance) continue;

				auto processForm = [&, this](RE::TESForm* el) {
					if constexpr (std::is_same_v<FormT, void>) {
						dataList.emplace_back(createData(nullptr, extData));
					} else {
						FormT* casted = needsForm ? el->As<FormT>() : nullptr;
						if (needsForm && !casted) {
							// This used to be a silent return, which made "the effect does nothing and
							// the log is empty" impossible to diagnose.
							SKSE::log::warn("Effect skipped: form {:08X} (form type {}) is not the form type this effect needs",
								el->GetFormID(), static_cast<int>(el->GetFormType()));
							return;
						}
						dataList.emplace_back(createData(casted, extData));
					}
				};

				if (extData.isFormList && form) {
					auto* list = form->As<RE::BGSListForm>();
					if (!list || list->forms.empty()) continue;
					int idx = extData.index;
					if (idx == -3) {
						idx = std::uniform_int_distribution<int>(0, static_cast<int>(list->forms.size()) - 1)(rng);
					} else if (idx == -2) {
						idx = currentRule.dynamicIndex;
					}
					if (idx == -1) {
						for (auto* el : list->forms) {
							if (el) processForm(el);
						}
					} else if (idx >= 0 && idx < list->forms.size()) {
						if (auto* el = list->forms[idx]) processForm(el);
					}
				} else if (!extData.isFormList) {
					processForm(form);
				}
			}
			if (!dataList.empty()) {
				// Roll for random timer if needed
				float currentTimerValue = 0.0f;
				std::uint32_t matchFilterRecheck = 0;
				if (eff.items.size() > 0) {
					currentTimerValue = eff.items[0].second.timer.time.value;
					if (eff.items[0].second.timer.time.useRandom) {
						std::uniform_real_distribution<float> timerDist(eff.items[0].second.timer.time.min, eff.items[0].second.timer.time.max);
						currentTimerValue = timerDist(rng);
					}
					matchFilterRecheck = eff.items[0].second.timer.matchFilterRecheck;
				}

				if (currentTimerValue > 0.0f) {
					static std::vector<std::future<void>> effectTimerTasks;
					static std::mutex effectTimerMutex;
					static std::set<std::pair<RE::FormID, const void*>> pendingTimerKeys;
					static std::mutex pendingTimerKeysMutex;

					// Guard against runaway thread stacking. Some events (OnUpdate in
					// particular) re-trigger the same matching target repeatedly with no
					// cooldown, so without this guard a rule with a timer longer than the
					// re-trigger interval would spawn a brand new sleeping OS thread every
					// re-trigger, indefinitely, for as long as the target keeps matching -
					// causing steadily worsening lag the longer the player stays nearby.
					auto timerKey = std::make_pair(ctx.target ? ctx.target->GetFormID() : 0u, static_cast<const void*>(&eff));
					{
						std::lock_guard<std::mutex> pendingLock(pendingTimerKeysMutex);
						if (!pendingTimerKeys.insert(timerKey).second) {
							return;
						}
					}

					// Capture a VALUE COPY of the rule (not a reference). currentRule is a
					// reference to a Rule that lives in the caller's stack frame (see
					// RuleManager::ApplyEffect's AddTask lambda); that frame is gone long
					// before this delayed task runs, so capturing "&currentRule" here was
					// a dangling reference and undefined behavior - a very plausible source
					// of intermittent, hard-to-reproduce crashes for any effect using a
					// "timer" with "matchfilterrecheck".
					Rule ruleCopy = currentRule;
					// Snapshot the references as handles NOW (on the calling thread, while ctx is
					// still valid) - the copied raw pointers in ctx must not be trusted after the delay.
					const DeferredRefs deferredRefs = DeferredRefs::Capture(ctx);
					auto effectTimerFuture = std::async(std::launch::async, [this, dataList, ctx, deferredRefs, applyEffect, currentTimerValue, matchFilterRecheck, ruleCopy, timerKey]() mutable {
						std::this_thread::sleep_for(std::chrono::duration<float>(currentTimerValue));

						SKSE::GetTaskInterface()->AddTask([this, dataList, ctx = ctx, deferredRefs, applyEffect, matchFilterRecheck, ruleCopy, timerKey]() mutable {
							struct PendingTimerGuard {
								std::pair<RE::FormID, const void*> key;
								~PendingTimerGuard() {
									std::lock_guard<std::mutex> guard(pendingTimerKeysMutex);
									pendingTimerKeys.erase(key);
								}
							} pendingGuard{ timerKey };

							// Re-resolve source/target from their handles: whatever ctx pointed at when
							// the timer started may have been freed during the delay (that dangling
							// pointer is what used to crash console-command effects like "disable").
							// The NiPointers keep both references alive until this task finishes.
							RE::NiPointer<RE::TESObjectREFR> sourceKeepAlive;
							RE::NiPointer<RE::TESObjectREFR> targetKeepAlive;
							if (!deferredRefs.Resolve(ctx, sourceKeepAlive, targetKeepAlive)) return;

							auto* target = ctx.target;
							auto* source = ctx.source;
							// Ground/water hits (kHitGround/kHitWater) never have a target -
							// only require one for every other event type.
							if (ctx.event != EventType::kHitGround && ctx.event != EventType::kHitWater && (!target || target->IsDeleted())) return;
							if (source && source->IsDeleted()) return;
							if (matchFilterRecheck == 1) {
								Rule mutableRule = ruleCopy;
								if (!MatchFilter(mutableRule.filter, ctx, mutableRule)) return;
							}
							applyEffect(ctx, dataList);
						});
					});
					{
						std::lock_guard<std::mutex> timerLock(effectTimerMutex);
						effectTimerTasks.push_back(std::move(effectTimerFuture));

						effectTimerTasks.erase(
							std::remove_if(effectTimerTasks.begin(), effectTimerTasks.end(),
								[](const std::future<void>& f) {
									return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
								}),
							effectTimerTasks.end());
					}
				} else {
					applyEffect(ctx, dataList);
				}
			}
		}

		std::map<Key, std::uint32_t> _limitCounts;
		std::map<Key, std::uint32_t> _interactionsCounts;
		std::map<Key, std::uint32_t> _stageCounts;			// which stage (see Effect::atStage) each (source, target, rule) combo is currently on - persists across saves the same way _limitCounts does

		std::unordered_map<RE::TESObjectREFR*, std::chrono::steady_clock::time_point> recentlyProcessedItems;
        std::chrono::steady_clock::time_point lastCleanupTime;

		mutable std::vector<Rule*> updateRules;
		mutable bool updateRulesCached = false;

		mutable UpdateFilter cachedUpdateFilter;
    	mutable bool updateFilterCached = false;

		UpdateFilter BuildUpdateFilter() const {
			UpdateFilter filter;
			
			std::shared_lock lock(_ruleMutex);
			
			for (const auto& rule : _rules) {
				bool hasOnUpdate = std::find(rule.events.begin(), rule.events.end(), EventType::kOnUpdate) != rule.events.end();
				if (!hasOnUpdate) continue;
				
				for (auto formType : rule.filter.formTypes) {
					filter.formTypes.insert(formType);
				}
				
				for (auto formID : rule.filter.formIDs) {
					filter.formIDs.insert(formID);
				}
				
				for (const auto& entry : rule.filter.formLists) {
					auto* list = RE::TESForm::LookupByID<RE::BGSListForm>(entry.formID);
					if (!list) continue;
					
					for (auto* form : list->forms) {
						if (form) {
							filter.formIDs.insert(form->GetFormID());
						}
					}
				}

				if (!rule.filter.keywords.empty()) {
					filter.formTypes.insert(RE::FormType::Activator);
					filter.formTypes.insert(RE::FormType::TalkingActivator);
					filter.formTypes.insert(RE::FormType::Weapon);
					filter.formTypes.insert(RE::FormType::Armor);
					filter.formTypes.insert(RE::FormType::Ammo);
					filter.formTypes.insert(RE::FormType::Ingredient);
					filter.formTypes.insert(RE::FormType::Misc);
					filter.formTypes.insert(RE::FormType::Book);
					filter.formTypes.insert(RE::FormType::Note);
					filter.formTypes.insert(RE::FormType::Scroll);
					filter.formTypes.insert(RE::FormType::SoulGem);
					filter.formTypes.insert(RE::FormType::AlchemyItem);
					filter.formTypes.insert(RE::FormType::Furniture);
					filter.formTypes.insert(RE::FormType::Flora);
					filter.formTypes.insert(RE::FormType::KeyMaster);
				}
			}

			return filter;
		}

	public:
		static RuleManager* GetSingleton();

		std::vector<Rule> _rules;
		mutable std::shared_mutex _ruleMutex;

		template <class T = RE::TESForm>
		static T* GetFormFromIdentifier(const std::string& identifier);
		template <class T = RE::TESForm>
		static T* GetFormFromEditorID(const std::string& editorID);

		void LoadRules();
		void Trigger(const RuleContext& ctx);
		void CleanupCounters();
		
		void ResetInteractionCounts();
		void OnSave(SKSE::SerializationInterface* intf);
		void OnLoad(SKSE::SerializationInterface* intf);
		void InitSerialization();

		void InvalidateUpdateCache() { 
			updateRulesCached = false; 
			updateFilterCached = false;
		}
    
		const std::vector<Rule*>& GetUpdateRules() {
			if (!updateRulesCached) {
				updateRules.clear();
				for (auto& rule : _rules) {
					if (std::find(rule.events.begin(), rule.events.end(), EventType::kOnUpdate) != rule.events.end()) {
						updateRules.push_back(&rule);
					}
				}
				updateRulesCached = true;
			}
			return updateRules;
		}

		const UpdateFilter& GetUpdateFilter() const {
			if (!updateFilterCached) {
				cachedUpdateFilter = BuildUpdateFilter();
				updateFilterCached = true;
			}
			return cachedUpdateFilter;
		}
	};
}
