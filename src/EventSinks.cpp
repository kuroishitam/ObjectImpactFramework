#include "EventSinks.h"
#include <atomic>
#include "PCH.h"
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <mutex>

using namespace std::chrono;

namespace OIF
{

//░█████╗░░█████╗░███╗░░██╗░██████╗████████╗░█████╗░███╗░░██╗████████╗░██████╗
//██╔══██╗██╔══██╗████╗░██║██╔════╝╚══██╔══╝██╔══██╗████╗░██║╚══██╔══╝██╔════╝
//██║░░╚═╝██║░░██║██╔██╗██║╚█████╗░░░░██║░░░███████║██╔██╗██║░░░██║░░░╚█████╗░
//██║░░██╗██║░░██║██║╚████║░╚═══██╗░░░██║░░░██╔══██║██║╚████║░░░██║░░░░╚═══██╗
//╚█████╔╝╚█████╔╝██║░╚███║██████╔╝░░░██║░░░██║░░██║██║░╚███║░░░██║░░░██████╔╝
//░╚════╝░░╚════╝░╚═╝░░╚══╝╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░╚═════╝░                                                                                                              
	
	const std::unordered_set<RE::FormType> EventSinkBase::relevantFormTypes = {
        RE::FormType::Activator,
        RE::FormType::TalkingActivator,
        RE::FormType::Weapon,
        RE::FormType::Armor,
        RE::FormType::Ammo,
        RE::FormType::Ingredient,
        RE::FormType::Misc,
        RE::FormType::Book,
        RE::FormType::Note,
        RE::FormType::Scroll,
        RE::FormType::SoulGem,
        RE::FormType::AlchemyItem,
        RE::FormType::Furniture,
        RE::FormType::Door,
        RE::FormType::Flora,
        RE::FormType::Container,
        RE::FormType::Static,
        RE::FormType::MovableStatic,
        RE::FormType::Tree,
        RE::FormType::KeyMaster,
        RE::FormType::Light
    };


//███████╗███╗░░██╗██╗░░░██╗███╗░░░███╗░██████╗
//██╔════╝████╗░██║██║░░░██║████╗░████║██╔════╝
//█████╗░░██╔██╗██║██║░░░██║██╔████╔██║╚█████╗░
//██╔══╝░░██║╚████║██║░░░██║██║╚██╔╝██║░╚═══██╗
//███████╗██║░╚███║╚██████╔╝██║░╚═╝░██║██████╔╝
//╚══════╝╚═╝░░╚══╝░╚═════╝░╚═╝░░░░░╚═╝╚═════╝░                                 

	enum class WeaponType
	{
        HandToHand,
        OneHandSword,
        Dagger,
        OneHandAxe,
        OneHandMace,
        TwoHandSword,
        TwoHandAxe,
        Ranged,
        Staff,
        Torch,
        Spell,
        Scroll,
        Shout,
        Ability,
        LesserPower,
        Power,
        Explosion,
        Total,
        Other
    };

    enum class AttackType {
        Regular,
        Power,
        Bash,
        Charge,
        Rotating,
        Continuous,
        Constant,
        FireAndForget,
        IgnoreWeapon,
        OverrideData    
    }; 

    enum class DeliveryType {
        Self,
        Aimed,
        TargetActor,
        TargetLocation,
        Touch,
        Total,
        None
    };


//░██████╗████████╗░█████╗░████████╗██╗░█████╗░░██████╗
//██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔══██╗██╔════╝
//╚█████╗░░░░██║░░░███████║░░░██║░░░██║██║░░╚═╝╚█████╗░
//░╚═══██╗░░░██║░░░██╔══██║░░░██║░░░██║██║░░██╗░╚═══██╗
//██████╔╝░░░██║░░░██║░░██║░░░██║░░░██║╚█████╔╝██████╔╝
//╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═════╝░                                 

	static auto startTime = std::chrono::steady_clock::now();


//██╗░░██╗███████╗██╗░░░░░██████╗░███████╗██████╗░░██████╗
//██║░░██║██╔════╝██║░░░░░██╔══██╗██╔════╝██╔══██╗██╔════╝
//███████║█████╗░░██║░░░░░██████╔╝█████╗░░██████╔╝╚█████╗░
//██╔══██║██╔══╝░░██║░░░░░██╔═══╝░██╔══╝░░██╔══██╗░╚═══██╗
//██║░░██║███████╗███████╗██║░░░░░███████╗██║░░██║██████╔╝
//╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═════╝░

	WeaponType GetWeaponType(RE::TESObjectWEAP* weapon) {
        if (!weapon) return WeaponType::Other;
        WeaponType weaponType = WeaponType::Other;
        switch (weapon->GetWeaponType()) {
            case RE::WEAPON_TYPE::kHandToHandMelee:  weaponType = WeaponType::HandToHand;   break;
            case RE::WEAPON_TYPE::kOneHandSword:     weaponType = WeaponType::OneHandSword; break;
            case RE::WEAPON_TYPE::kOneHandDagger:    weaponType = WeaponType::Dagger;       break;
            case RE::WEAPON_TYPE::kOneHandAxe:       weaponType = WeaponType::OneHandAxe;   break;
            case RE::WEAPON_TYPE::kOneHandMace:      weaponType = WeaponType::OneHandMace;  break;
            case RE::WEAPON_TYPE::kTwoHandSword:     weaponType = WeaponType::TwoHandSword; break;
            case RE::WEAPON_TYPE::kTwoHandAxe:       weaponType = WeaponType::TwoHandAxe;   break;
            case RE::WEAPON_TYPE::kBow:
            case RE::WEAPON_TYPE::kCrossbow:         weaponType = WeaponType::Ranged;       break;
            case RE::WEAPON_TYPE::kStaff:            weaponType = WeaponType::Staff;        break;
            case RE::WEAPON_TYPE::kTotal:            weaponType = WeaponType::Total;        break;
            default:                                 weaponType = WeaponType::Other;        break;
        }
        return weaponType;
    }

    WeaponType GetSpellType(RE::SpellItem* spell) {
        if (!spell) return WeaponType::Other;
        WeaponType weaponType = WeaponType::Other;
        switch (spell->GetSpellType()) {
            case RE::MagicSystem::SpellType::kSpell:        weaponType = WeaponType::Spell;        break;
            case RE::MagicSystem::SpellType::kVoicePower:   weaponType = WeaponType::Shout;        break;
            case RE::MagicSystem::SpellType::kAbility:      weaponType = WeaponType::Ability;      break;
            case RE::MagicSystem::SpellType::kLesserPower:  weaponType = WeaponType::LesserPower;  break;
            case RE::MagicSystem::SpellType::kPower:        weaponType = WeaponType::Power;        break;
            case RE::MagicSystem::SpellType::kLeveledSpell: weaponType = WeaponType::Spell;        break;
            case RE::MagicSystem::SpellType::kScroll:       weaponType = WeaponType::Spell;        break;
            case RE::MagicSystem::SpellType::kStaffEnchantment: weaponType = WeaponType::Staff;    break;
            default:                                        weaponType = WeaponType::Other;        break;
        }
        return weaponType;
    }

    AttackType GetAttackType(RE::BGSAttackData* attackData) {
        if (attackData) {
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) return AttackType::Power;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) return AttackType::Bash;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kChargeAttack)) return AttackType::Charge;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kRotatingAttack)) return AttackType::Rotating;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kContinuousAttack)) return AttackType::Continuous;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kIgnoreWeapon)) return AttackType::IgnoreWeapon;
            if (attackData->data.flags.any(RE::AttackData::AttackFlag::kOverrideData)) return AttackType::OverrideData;
        }
        return AttackType::Regular;
    }

    std::string WeaponTypeToString(WeaponType weaponType) {
        switch (weaponType) {
            case WeaponType::HandToHand:     return "handtohand";
            case WeaponType::OneHandSword:   return "onehandsword";
            case WeaponType::Dagger:         return "dagger";
            case WeaponType::OneHandAxe:     return "onehandaxe";
            case WeaponType::OneHandMace:    return "onehandmace";
            case WeaponType::TwoHandSword:   return "twohandsword";
            case WeaponType::TwoHandAxe:     return "twohandaxe";
            case WeaponType::Ranged:         return "ranged";
            case WeaponType::Staff:          return "staff";
            case WeaponType::Torch:          return "torch";
            case WeaponType::Spell:          return "spell";
            case WeaponType::Scroll:         return "scroll";
            case WeaponType::Shout:          return "shout";
            case WeaponType::Ability:        return "ability";
            case WeaponType::LesserPower:    return "lesserpower";
            case WeaponType::Power:          return "power";
            case WeaponType::Explosion:      return "explosion";
            case WeaponType::Total:          return "total";
            default:                         return "other";
        }
    }
    
    std::string AttackTypeToString(AttackType attackType) {
        switch (attackType) {
            case AttackType::Regular:        return "regular";
            case AttackType::Power:          return "power";
            case AttackType::Bash:           return "bash";
            case AttackType::Charge:         return "charge";
            case AttackType::Rotating:       return "rotating";
            case AttackType::Continuous:     return "continuous";
            case AttackType::Constant:       return "constant";
            case AttackType::FireAndForget:  return "fireandforget";
            case AttackType::IgnoreWeapon:   return "ignoreweapon";
            case AttackType::OverrideData:   return "overridedata";
            default:                         return "regular";
        }
    }

    std::string DeliveryTypeToString(DeliveryType delivery) {
        switch (delivery) {
            case DeliveryType::Self:           return "self";
            case DeliveryType::Aimed:          return "aimed";
            case DeliveryType::TargetActor:    return "targetactor";
            case DeliveryType::TargetLocation: return "targetlocation";
            case DeliveryType::Touch:          return "touch";
            case DeliveryType::Total:          return "total";
            case DeliveryType::None:           return "none";
            default:                           return "none";
        }
    }

    void ScanCell(RE::Actor* source, std::vector<RE::TESObjectREFR*>* foundObjects = nullptr, bool triggerEvents = false, 
				  EventType eventType = EventType::kNone, RE::TESWeather* weather = nullptr, const UpdateFilter* updateFilter = nullptr)
    {
		if (!EventSinkBase::IsActorSafe(source)) return;

        auto* cell = source->GetParentCell();
        if (!cell) return;

        std::size_t processedCount = 0;
        std::size_t skippedCount = 0;

        // Collect matching references first, without triggering anything. Calling
        // RuleManager::Trigger() (and whatever effects/hooks - ours or another mod's -
        // it can indirectly set off) from inside TESObjectCELL::ForEachReference's active
        // iteration risks the iterator handing us a stale/freed reference on its next
        // step if anything invalidates an object in this same cell mid-iteration. Doing
        // the actual triggering only after the iteration has fully returned removes that
        // window entirely.
        std::vector<RE::TESObjectREFR*> matched;

        cell->ForEachReference([&](RE::TESObjectREFR* ref) -> RE::BSContainer::ForEachResult {
            if (!EventSinkBase::IsItemSafe(ref)) return RE::BSContainer::ForEachResult::kContinue;

            if (triggerEvents) {
                if (eventType == EventType::kOnUpdate && updateFilter && !updateFilter->IsEmpty()) {
                    if (!updateFilter->Matches(ref)) {
                        skippedCount++;
                        return RE::BSContainer::ForEachResult::kContinue;
                    }
                }

                processedCount++;
                matched.push_back(ref);
            } else {
                if (foundObjects) foundObjects->push_back(ref);
            }

            return RE::BSContainer::ForEachResult::kContinue;
        });

        for (auto* ref : matched) {
            if (!EventSinkBase::IsItemSafe(ref)) continue;

            RuleContext ctx{
                eventType,
                source, 
                ref,
                nullptr,
                nullptr,
                "",
                "",
                "",
                false,
                weather
            };

            RuleManager::GetSingleton()->Trigger(ctx);
        }
    }

//░██████╗░██████╗░░█████╗░██╗░░░██╗███╗░░██╗██████╗░
//██╔════╝░██╔══██╗██╔══██╗██║░░░██║████╗░██║██╔══██╗
//██║░░██╗░██████╔╝██║░░██║██║░░░██║██╔██╗██║██║░░██║
//██║░░╚██╗██╔══██╗██║░░██║██║░░░██║██║╚████║██║░░██║
//╚██████╔╝██║░░██║╚█████╔╝╚██████╔╝██║░╚███║██████╔╝
//░╚═════╝░╚═╝░░╚═╝░╚════╝░░╚═════╝░╚═╝░░╚══╝╚═════╝░

	// RE::TESObjectLAND (Skyrim's terrain heightmap) is not a TESObjectREFR, so
	// a projectile that lands on bare ground never turns up in
	// cell->ForEachReferenceInRange - there's structurally nothing there to
	// find, not just a form type that got filtered out. To tell "hit empty
	// terrain" apart from "hit something ForEachReferenceInRange missed", cast
	// a short vertical ray straddling the reported impact point filtered to
	// only the terrain collision layer (the same RE::COL_LAYER::kTerrain used
	// elsewhere in this codebase for decal/impact-position raycasting). A hit
	// close to a_hitPos confirms it's terrain.
	bool IsGroundHitAtPosition(RE::TESObjectCELL* a_cell, const RE::NiPoint3& a_hitPos)
	{
		if (!a_cell) return false;

		auto* world = a_cell->GetbhkWorld();
		if (!world) return false;

		RE::NiPoint3 start = a_hitPos;
		start.z += 32.0f;
		RE::NiPoint3 end = a_hitPos;
		end.z -= 64.0f;

		RE::bhkPickData pick;
		const auto scale = RE::bhkWorld::GetWorldScale();

		pick.rayInput.from = start * scale;
		pick.rayInput.to = end * scale;
		pick.rayInput.filterInfo = static_cast<uint32_t>(RE::COL_LAYER::kTerrain);

		world->PickObject(pick);
		if (!pick.rayOutput.HasHit()) return false;

		auto rayDirection = pick.rayInput.to - pick.rayInput.from;
		auto hitPoint = pick.rayInput.from + rayDirection * pick.rayOutput.hitFraction;
		RE::NiPoint3 hitWorld = RE::NiPoint3(hitPoint.quad.m128_f32[0], hitPoint.quad.m128_f32[1], hitPoint.quad.m128_f32[2]) / scale;

		// 65.0 matches the radius ProcessProjectileImpact already uses when
		// scanning for reference targets around a_hitPos.
		return hitWorld.GetDistance(a_hitPos) <= 65.0f;
	}

	// 1. For melee swings we only ever guess a hit position - "end" is extrapolated flat
	// from the player's own foot height (dir.z is zeroed out), so on a slope, staircase, or
	// ledge the real terrain surface can be well outside IsGroundHitAtPosition's fairly
	// narrow +32/-64 search band and, separately, more than 65 units away in 3D from that
	// guess even where the swing genuinely did hit the ground. This does a much wider
	// vertical-only search (terrain layer only) centered on a_pos.z, and - unlike
	// IsGroundHitAtPosition - corrects a_pos.z to the actual terrain height it finds rather
	// than just confirming proximity to an already-known-good position. That correction
	// also means spawned effects land at the right height on sloped ground instead of
	// floating/clipping at the player's own foot height.
	bool FindGroundZAtPosition(RE::TESObjectCELL* a_cell, RE::NiPoint3& a_pos, float a_verticalRange)
	{
		if (!a_cell) return false;

		auto* world = a_cell->GetbhkWorld();
		if (!world) return false;

		RE::NiPoint3 start = a_pos;
		start.z += a_verticalRange;
		RE::NiPoint3 end = a_pos;
		end.z -= a_verticalRange;

		RE::bhkPickData pick;
		const auto scale = RE::bhkWorld::GetWorldScale();

		pick.rayInput.from = start * scale;
		pick.rayInput.to = end * scale;
		pick.rayInput.filterInfo = static_cast<uint32_t>(RE::COL_LAYER::kTerrain);

		world->PickObject(pick);
		if (!pick.rayOutput.HasHit()) return false;

		auto rayDirection = pick.rayInput.to - pick.rayInput.from;
		auto hitPoint = pick.rayInput.from + rayDirection * pick.rayOutput.hitFraction;
		RE::NiPoint3 hitWorld = RE::NiPoint3(hitPoint.quad.m128_f32[0], hitPoint.quad.m128_f32[1], hitPoint.quad.m128_f32[2]) / scale;

		a_pos.z = hitWorld.z;
		return true;
	}

	// Like FindGroundZAtPosition, but not limited to the terrain layer: this finds the
	// highest SOLID surface of any kind at a_probe's (x,y) - terrain heightmap, but also
	// statics, landscape meshes, rocks, docks, bridges, roads, ice sheets and so on. That
	// distinction matters a lot for the water check below: an enormous amount of Skyrim's
	// visible "ground" near water isn't the TESObjectLAND heightmap at all, it's static
	// mesh geometry laid on top of it (riverbeds, rocky shorelines, piers, stone banks).
	// A terrain-only probe returns "no ground here" for all of it, which is exactly why
	// the previous shoreline cross-check kept letting dry-land hits through as water hits.
	//
	// One pick per layer rather than one combined filterInfo mask: that's the pattern this
	// codebase already uses elsewhere (see Effects.cpp's crosshair pick loop), and it
	// avoids depending on how Havok's collision-filter word happens to be laid out. Each
	// pick is top-down, so its hit is that layer's highest surface; the max across layers
	// is the highest solid surface overall.
	// The band is deliberately asymmetric-capable: a_rangeAbove/a_rangeBelow bound the
	// search separately, because the water check below needs to look for the BED at a
	// position without also picking up whatever is overhead. A symmetric probe finds the
	// highest solid surface within the whole band, which near water very often means a
	// bridge deck, a dock, an overhanging rock or (indoors) the ceiling - none of which
	// say anything about whether the spot below them is wet. See the shoreline gate.
	bool FindSolidSurfaceZAtPosition(RE::TESObjectCELL* a_cell, const RE::NiPoint3& a_probe, float a_rangeAbove, float a_rangeBelow, float& a_outZ)
	{
		if (!a_cell) return false;

		auto* world = a_cell->GetbhkWorld();
		if (!world) return false;

		static const std::vector<RE::COL_LAYER> kSolidLayers = {
			RE::COL_LAYER::kTerrain,
			RE::COL_LAYER::kStatic,
			RE::COL_LAYER::kAnimStatic,
			RE::COL_LAYER::kGround,
			RE::COL_LAYER::kClutterLarge,
			RE::COL_LAYER::kTrap
		};

		const auto scale = RE::bhkWorld::GetWorldScale();
		if (!(scale > 0.0f)) return false;

		RE::NiPoint3 start = a_probe;
		start.z += a_rangeAbove;
		RE::NiPoint3 end = a_probe;
		end.z -= a_rangeBelow;

		bool found = false;
		float best = 0.0f;

		for (RE::COL_LAYER layer : kSolidLayers) {
			RE::bhkPickData pick;
			pick.rayInput.from = start * scale;
			pick.rayInput.to = end * scale;
			pick.rayInput.filterInfo = static_cast<uint32_t>(layer);

			world->PickObject(pick);
			if (!pick.rayOutput.HasHit()) continue;

			auto rayDirection = pick.rayInput.to - pick.rayInput.from;
			auto hitPoint = pick.rayInput.from + rayDirection * pick.rayOutput.hitFraction;
			const float hitZ = RE::NiPoint3(hitPoint.quad.m128_f32[0], hitPoint.quad.m128_f32[1], hitPoint.quad.m128_f32[2]).z / scale;
			if (!std::isfinite(hitZ)) continue;

			if (!found || hitZ > best) {
				best = hitZ;
				found = true;
			}
		}

		if (!found) return false;
		a_outZ = best;
		return true;
	}

	// ╔══════════════════════════════════════════════════════════════════╗
	// ║  THE "DRY GROUND NEAR WATER FIRES kHitWater" GATE                ║
	// ╚══════════════════════════════════════════════════════════════════╝
	//
	// A cell's water height is a single flat number that applies to the whole cell, and
	// IsWithinWaterFootprint's containment test is only an axis-aligned bounding box around
	// each water volume - neither knows anything about where the shoreline actually runs.
	// Together they say "yes, water" for a large rectangle of dry land around every lake,
	// river and coastline. That's the entire cause of the "hitting ground near water still
	// triggers hitwater" bug: a shot or swing that lands on a riverbank a few units either
	// side of the water plane passes both tests with nothing left to contradict them.
	//
	// The previous cross-check did look for terrain, but it only rejected ground standing
	// MORE than a_aboveTolerance (20 units) above the water plane - so a bank sitting 0-20
	// units above the surface, which is most of what a shoreline actually looks like, sailed
	// straight through. It was also terrain-layer-only, so any bank made of static mesh
	// (most of them) registered as no ground at all.
	//
	// SHALLOW WATER FIX. The first version of this gate had two problems, both of which
	// showed up as "a shallow water source never fires kHitWater":
	//
	//  1. THE PROBE LOOKED UP AS WELL AS DOWN. It asked for the highest solid surface
	//     within +/-400 units of the water plane, so anything overhead at that (x,y) -
	//     a bridge deck, a dock, an overhanging rock, a waterfall mesh, or indoors
	//     simply the ceiling - was returned as "the ground here", sat far above the
	//     waterline, and the hit was rejected as shore. Any small pond, trough, basin
	//     or stream with something above it could therefore never register, no matter
	//     how deep it was. The probe is now bounded to the water column: it starts just
	//     above the plane (kBedProbeAbove below - not right at kShoreTolerance itself, for
	//     reasons covered where that constant is defined) and only looks down, so what it
	//     finds is the bed or nothing.
	//
	//  2. IT DEMANDED 4 UNITS OF DEPTH. Real shallow water - streams, marsh, puddles,
	//     the last foot of a riverbank, most interior water - frequently has its bed
	//     sitting less than that below the surface, and every one of those spots read
	//     as shore. Depth is no longer the test; the surface reaching the waterline is.
	//     kShoreTolerance is now just slop for geometry that sits a hair under the
	//     plane, not a minimum depth requirement.
	//
	// A bank still gets rejected the same way it did before: ground standing at or above
	// the waterline is shore, and that is precisely what the probe reports when there is
	// any. The one case the bounded probe cannot see is a bank whose surface is above the
	// plane, because the ray then starts inside solid geometry and Havok reports no hit
	// at all - handled explicitly in the "no bed found" branch below.
	constexpr float kShoreTolerance = 0.5f;      // a surface within this of the plane counts as reaching it

	// How far below the plane to search for a bed, and how far above it the search ray
	// starts. These used to be the same value (kShoreTolerance, 0.5 units above) and a
	// 4096-unit search depth - both turned out to be wrong in opposite directions:
	//
	//  - 4096 units of search depth is far deeper than any Skyrim water body actually is
	//    (even the deepest lakes are nowhere close). When a dry-land hit shares a water
	//    volume's coarse AABB with a completely unrelated, differently-elevated water body
	//    (the imprecise-footprint issue documented above), the probe could reach all the
	//    way down into unrelated deep terrain/foundations far below and mistake THAT for
	//    the nearby water's own bed - misreporting dry stairs or a path near a river as
	//    real deep water. kBedSearchDepth is now generous for even the deepest vanilla
	//    lake, but nowhere near enough to reach terrain that has nothing to do with it.
	//
	//  - Starting the ray only 0.5 units above the plane put it right at (sometimes
	//    effectively touching/embedded in) the floor for Skyrim's many paper-thin
	//    decorative shallow waters - canals, gutters, troughs - which are frequently
	//    modeled essentially flush with their stone bed. A Havok ray that begins already
	//    touching/inside the surface it's aimed at routinely reports NO hit at all (a
	//    well-known Havok picking quirk, not a bug in the pick loop itself) - which the
	//    "no bed found" branch below then misreads as "the ground here rises above the
	//    waterline," rejecting genuinely-wet shallow water as dry shore. kBedProbeAbove
	//    gives the ray enough headroom to actually cross a flush-mounted floor instead of
	//    starting on top of it.
	constexpr float kBedProbeAbove = 8.0f;
	constexpr float kBedSearchDepth = 512.0f;

	// THE TOLERANCE WAS GENEROUS IN THE WRONG DIRECTION, ONCE A BED IS ACTUALLY FOUND.
	// With the probe fixed above, a flush-mounted canal/gutter/trough bed now gets
	// correctly detected right where it really is - a hair below the water plane,
	// exactly as designed for these paper-thin decorative waters. But the shore test
	// that runs on that bed height was "bedZ > waterHeight - kShoreTolerance", which
	// credits ANY bed sitting up to half a unit BELOW the plane as shore too, not
	// just one sitting above it. For water whose real, intended depth is well under
	// half a unit - which is exactly what these flush-mounted waters are - that band
	// swallows the entire thing: it gets found correctly and then rejected anyway,
	// purely because its real depth happens to be smaller than the tolerance meant to
	// absorb measurement noise.
	//
	// A genuine riverbank doesn't have this problem - at the point actually being
	// tested, its ground clears the waterline by a good deal more than half a unit,
	// nowhere close to borderline. So the tolerance only needs to cover noise on the
	// ABOVE side (a bed sitting a hair above the plane through mesh/record mismatch
	// alone), not manufacture half a unit of extra "still shore" room below it.
	// kBedShoreTolerance is that - a bed only counts as shore once it's actually,
	// unambiguously above the waterline, not merely close to it from below. This is
	// deliberately a separate constant from kShoreTolerance above (used only by the
	// "no bed found" branch, which reasons about a totally different situation with
	// its own tradeoffs) rather than reusing it with a flipped sign.
	constexpr float kBedShoreTolerance = 0.1f;

	// The cell to run collision probes in for a world position, falling back to the
	// player's own cell when the position sits just outside the attached grid (cell
	// seams, a projectile at a cell-load edge).
	static RE::TESObjectCELL* ResolveProbeCell(const RE::NiPoint3& a_pos)
	{
		RE::TESObjectCELL* cell = nullptr;
		if (auto* tes = RE::TES::GetSingleton()) {
			cell = tes->GetCell(a_pos);
		}
		if (!cell || !cell->IsAttached()) {
			if (auto* player = RE::PlayerCharacter::GetSingleton()) {
				cell = player->GetParentCell();
			}
		}
		return cell;
	}

	bool HasGroundAtOrAboveWater(const RE::NiPoint3& a_pos, float a_waterHeight)
	{
		auto* cell = ResolveProbeCell(a_pos);
		if (!cell) return false;

		// Probe centered on the water plane, not on the impact position: the impact
		// position can be far below the real surface (a projectile that sank to the
		// seabed) or slightly above it (a melee swing extrapolated from foot height),
		// and either would bias a probe centered on it.
		RE::NiPoint3 probe = a_pos;
		probe.z = a_waterHeight;

		float bedZ = 0.0f;
		if (!FindSolidSurfaceZAtPosition(cell, probe, kBedProbeAbove, kBedSearchDepth, bedZ)) {
			// Nothing solid anywhere in the water column. Two opposite situations produce
			// that, and the impact position is what tells them apart:
			//  - open water whose bed is deeper than the probe, or a spot with no
			//    collision geometry at all. The hit is under the surface: water.
			//  - the ray started INSIDE solid geometry, because the ground here rises
			//    above the waterline. A downward ray beginning inside a solid reports no
			//    hit, so here "nothing found" means the opposite of open water, and a hit
			//    sitting at or above the waterline in such a spot is a bank hit.
			const bool shore = a_pos.z >= a_waterHeight - kShoreTolerance;
			logger::debug("HitWater gate: no bed found at ({:.1f},{:.1f}) waterZ={:.2f} hitZ={:.2f} -> {}",
				a_pos.x, a_pos.y, a_waterHeight, a_pos.z, shore ? "shore" : "water");
			return shore;
		}

		const bool shore = bedZ > a_waterHeight + kBedShoreTolerance;
		logger::debug("HitWater gate: ({:.1f},{:.1f}) waterZ={:.2f} bedZ={:.2f} depth={:.2f} hitZ={:.2f} -> {}",
			a_pos.x, a_pos.y, a_waterHeight, bedZ, a_waterHeight - bedZ, a_pos.z, shore ? "shore" : "water");
		return shore;
	}

	// PREVIOUSLY: adapted from RavenKZP's Frostwalker (itself crediting po3's
	// Splashes-of-Skyrim), this scanned RE::TESWaterSystem::waterObjects directly and did a
	// 2D contains-point test against each water volume's multiBounds, skipping any bound
	// with size.z > 10.0f on the theory that a tall bound meant "sloped water, no single
	// height applies" (waterfalls, etc.). That heuristic is what caused Flames/Frostbite to
	// fail to trigger kHitWater on lakes while working fine on rivers: size.z on a water
	// volume's bound is NOT a measure of surface waviness/slope - it's (roughly) half the
	// vertical extent of the volume down to the bottom of the body of water. A shallow
	// river's bound is thin enough to slip under the 10-unit cutoff; a deep lake's bound
	// routinely isn't, so the lake's water volume was being skipped entirely as "non-flat."
	//
	// FIXED (height): RE::TESObjectCELL exposes exactly this lookup as a supported engine
	// API - GetWaterHeight() - which Elementally Reactive Waters (a separately-written mod
	// that reactively detects these same spell types hitting water) uses for the same
	// purpose, confirmed working on both rivers and lakes. It removes the sloped-water
	// heuristic - and the deep-water bug it caused - entirely.
	//
	// FIXED (footprint): GetWaterHeight() is accurate about HOW HIGH the water is wherever
	// it applies, but says nothing about WHERE it applies - Bethesda cells typically carry a
	// single water height rather than a precise footprint, so it happily returns a height
	// for any position in a cell that has water somewhere in it, dry land included. Ground
	// near the shore that merely happens to sit lower than that height value (a dip in the
	// bank, a low patch of dry land just past the water's edge) was reading as "underwater"
	// with no water anywhere nearby - the "dry ground near water triggers kHitWater" bug.
	// IsWithinWaterFootprint below reintroduces the 2D bounding-box containment check the
	// original code used - MINUS its size.z heuristic, which has no business gating a
	// footprint check at all - purely as a yes/no "is there actually water here" gate.
	// GetWaterHeight() still supplies the real height value once that's confirmed.
	bool IsWithinWaterFootprint(const RE::NiPoint3& a_pos)
	{
		auto* waterSystem = RE::TESWaterSystem::GetSingleton();
		if (!waterSystem) return false;

		const RE::BSSpinLockGuard locker(waterSystem->lock);
		for (auto& waterObjectPtr : waterSystem->waterObjects) {
			auto* waterObject = waterObjectPtr.get();
			if (!waterObject) continue;

			for (auto& boundPtr : waterObject->multiBounds) {
				auto* bound = boundPtr.get();
				if (!bound) continue;

				const auto center = bound->center;
				const auto size = bound->size;
				const auto boundMin = center - size;
				const auto boundMax = center + size;
				if (a_pos.x < boundMin.x || a_pos.x > boundMax.x || a_pos.y < boundMin.y || a_pos.y > boundMax.y) {
					continue;
				}

				return true;
			}
		}

		return false;
	}

	bool FindWaterHeightAtPosition(const RE::NiPoint3& a_pos, float& a_outHeight)
	{
		if (!IsWithinWaterFootprint(a_pos)) return false;

		auto* tes = RE::TES::GetSingleton();
		if (!tes) return false;

		auto cell = tes->GetCell(a_pos);
		if (!cell || !cell->IsAttached()) {
			// A projectile can briefly report a position just outside the currently
			// attached cell grid (cell seams, cell-load edges) - falling back to the
			// player's own parent cell mirrors what ERWS does for the same edge case,
			// and is safe here since world water height doesn't meaningfully vary
			// between adjacent loaded cells in the same worldspace.
			if (auto* player = RE::PlayerCharacter::GetSingleton()) {
				cell = player->GetParentCell();
			}
		}
		if (!cell) return false;

		float height = 0.0f;
		if (!cell->GetWaterHeight(a_pos, height) || !std::isfinite(height)) return false;

		a_outHeight = height;
		return true;
	}

	// ---- Havok material lookup (materials / materialsnot filters) --------------------------
	//
	// Chain confirmed against the actual vendored headers for this fork (bhkRigidBody.h,
	// bhkShape.h, bhkEntity.h, bhkWorldObject.h, hkpShape.h, hkpCdBody.h, MaterialIDs.h):
	//
	//   NiAVObject
	//     -> GetCollisionObject()          bhkCollisionObject
	//     -> GetRigidBody()                bhkRigidBody       (Bethesda wrapper)
	//     -> GetRigidBody()                hkpRigidBody       (native Havok object - yes, both
	//                                                          steps are named GetRigidBody(), on
	//                                                          two different types)
	//     -> GetShape()                    hkpShape           (native)
	//     -> userData                      bhkShape*          (the native shape's back-pointer to
	//                                                          its Bethesda wrapper - this is where
	//                                                          RE::MATERIAL_ID actually lives)
	//     -> materialID                    RE::MATERIAL_ID
	//
	// Reads the shape's own materialID directly rather than bhkShape::GetMaterialID(shapeKey):
	// that overload resolves the sub-material of a compound shape at an actual hit point, and
	// here there's no specific hit point - just "what is this object, overall".
	//
	// WHY THIS WALKS THE WHOLE 3D TREE: the first version only looked at the collision object
	// on the reference's ROOT node. That works for single-body meshes but silently fails for
	// every mesh whose collision hangs off a child node (multi-part statics, kit pieces,
	// anything exported with per-part rigid bodies), so the filter only matched a few statics.
	// Now every node in the subtree is checked and all distinct materials found are returned;
	// a rule matches if ANY of them matches (materials) / none of them match (materialsnot).
	namespace
	{
		// Real reference meshes have a handful of collision bodies at most; the caps only
		// exist so a pathological rig can't make a single filter evaluation walk thousands
		// of nodes.
		constexpr int kMaxMaterialSearchDepth = 24;
		constexpr std::size_t kMaxMaterialSearchNodes = 256;

		struct MaterialSearchState
		{
			std::vector<RE::MATERIAL_ID> materials;	// distinct, non-zero material IDs found
			std::size_t nodesVisited{ 0 };
			std::size_t bodiesSeen{ 0 };			// collision bodies whose shape wrapper was readable
		};

		// Reads the material off ONE object's own collision object (not its children).
		void ReadOwnCollisionMaterial(RE::NiAVObject* a_obj, MaterialSearchState& a_state)
		{
			auto collisionObj = a_obj->GetCollisionObject();
			if (!collisionObj) return;

			auto bhkBody = collisionObj->GetRigidBody();
			if (!bhkBody) return;

			auto hkpBody = bhkBody->GetRigidBody();
			if (!hkpBody) return;

			auto* shape = hkpBody->GetShape();
			if (!shape) return;

			auto* bhkShapeWrapper = shape->userData;
			if (!bhkShapeWrapper) return;

			++a_state.bodiesSeen;

			const RE::MATERIAL_ID id = bhkShapeWrapper->materialID;
			// 0 == "no material set" (real MATERIAL_ID values are non-zero hashes). It carries
			// no information, so it must not count as a material - and must not stop the
			// search from finding a real one on a sibling body.
			if (static_cast<std::uint32_t>(id) == 0) return;

			if (std::find(a_state.materials.begin(), a_state.materials.end(), id) == a_state.materials.end()) {
				a_state.materials.push_back(id);
			}
		}

		void CollectMaterials(RE::NiAVObject* a_obj, int a_depth, MaterialSearchState& a_state)
		{
			if (!a_obj || a_depth > kMaxMaterialSearchDepth || a_state.nodesVisited >= kMaxMaterialSearchNodes) return;
			++a_state.nodesVisited;

			ReadOwnCollisionMaterial(a_obj, a_state);

			if (auto* node = a_obj->AsNode()) {
				for (auto const& child : node->GetChildren()) {
					if (child) CollectMaterials(child.get(), a_depth + 1, a_state);
				}
			}
		}

		// One info-level line per BASE object the first time its material can't be read, so
		// "why doesn't the materials filter match this object" has an answer in the SKSE log
		// without spamming it on every hit.
		void LogMaterialMissOnce(RE::TESObjectREFR* a_target, RE::NiAVObject* a_root, const MaterialSearchState& a_state)
		{
			auto* base = a_target->GetBaseObject();
			const RE::FormID key = base ? base->GetFormID() : a_target->GetFormID();

			static std::mutex s_mutex;
			static std::unordered_set<RE::FormID> s_logged;
			{
				std::lock_guard lock(s_mutex);
				if (!s_logged.insert(key).second) return;
			}

			const char* reason = "3D not loaded";
			if (a_root) {
				reason = a_state.bodiesSeen == 0 ?
				             "no Havok collision body found in its 3D" :
				             "collision body found, but its material ID is unset (0)";
			}

			const char* rootName = (a_root && a_root->name.c_str()) ? a_root->name.c_str() : "";
			logger::info("[OIF materials] can't read a material for base {:08X} (formType {}, ref {:08X}, root node '{}'): {} [nodes searched: {}, collision bodies: {}]",
				key,
				base ? static_cast<std::uint32_t>(base->GetFormType()) : 0u,
				a_target->GetFormID(),
				rootName,
				reason,
				a_state.nodesVisited,
				a_state.bodiesSeen);
		}
	}

	// Collects every distinct physical material found on a_target's collision bodies.
	// Returns false (a_outMaterials empty) when none could be read - typically because the
	// target's 3D isn't loaded (Get3D() is null for an out-of-range/unloaded reference), it has
	// no Havok collision at all (pure trigger volumes, some markers), or its bodies carry no
	// material. Actors only have their ROOT checked (their skeleton's bones carry ragdoll
	// bodies, which isn't what an object-material filter is about).
	bool ResolveTargetMaterials(RE::TESObjectREFR* a_target, std::vector<RE::MATERIAL_ID>& a_outMaterials)
	{
		a_outMaterials.clear();
		if (!a_target) return false;

		auto* root = a_target->Get3D();
		MaterialSearchState state;

		if (!root) {
			LogMaterialMissOnce(a_target, nullptr, state);
			return false;
		}

		if (a_target->As<RE::Actor>()) {
			state.nodesVisited = 1;
			ReadOwnCollisionMaterial(root, state);
		} else {
			CollectMaterials(root, 0, state);
		}

		if (state.materials.empty()) {
			LogMaterialMissOnce(a_target, root, state);
			return false;
		}

		a_outMaterials = std::move(state.materials);
		return true;
	}

	// Water in Skyrim is not a solid Havok collidable, so a projectile shot into open water
	// does not stop at the surface - it keeps falling until it hits something solid, usually
	// the seabed, and AddImpact reports THAT position. In deep water the seabed can be
	// hundreds of units below the actual water height, so checking "is the impact point near
	// the water plane" (an earlier version of this function did, with a tight +/-20
	// symmetric tolerance) rejects exactly the case this is meant to catch: only shots that
	// happen to clip the surface within a few units would ever pass, and any deep-water hit
	// would silently fall through to the ground check below and get misreported as terrain.
	//
	// The correct test is one-sided: anything AT OR BELOW the water plane is underwater
	// regardless of how far below (depth alone never disqualifies a hit). Only a position
	// clearly ABOVE the plane is rejected as dry - which matters because a water volume's
	// bounds don't imply every (x,y) inside them is wet at every z (e.g. a shoreline next to
	// the sea). a_aboveTolerance absorbs the same kind of engine slop (wave animation, a
	// slightly-early impact position) that IsGroundHitAtPosition's own +32/-64 search band
	// exists for. On success a_pos.z is snapped to the exact water height - mirroring
	// FindGroundZAtPosition's terrain correction - so spawned effects land flush with the
	// surface instead of at the seabed.
	// ╔══════════════════════════════════════════════════════════════════════════╗
	// ║  STRUCTURES ABOVE WATER (bridges, docks, jetties, boats, walkways)       ║
	// ╚══════════════════════════════════════════════════════════════════════════╝
	//
	// A bridge deck is inside the water volume's footprint and shares the cell's water
	// height, so the only thing separating "hit the bridge" from "hit the water under
	// the bridge" is height - and a lot of Skyrim's walkways, jetties, plank bridges and
	// boat decks sit only a handful of units above the surface, well inside the
	// tolerance FindWaterHitPosition allows for wave animation and impact jitter. The
	// shoreline gate can't catch these either: it deliberately looks only DOWN from the
	// water plane (that bounded probe is what let shallow water work in the first
	// place), so the structure overhead is invisible to it, and there is genuine water
	// with a genuine bed below - the hit just never reached it.
	//
	// So this probes a narrow band around the IMPACT POINT rather than around the water
	// plane, and asks a different question: is the thing that was struck up here a solid
	// surface standing above the waterline? If it is, the projectile or swing landed on
	// a structure, not in the water. The band is kept tight on purpose - wide enough to
	// find the deck the impact is resting on, too narrow to reach down and find the
	// riverbed under an ordinary open-water splash.
	constexpr float kSurfaceSlop = 2.0f;         // impact jitter / wave animation around the plane
	constexpr float kDeckProbeAbove = 8.0f;
	constexpr float kDeckProbeBelow = 24.0f;

	static bool IsOnStructureAboveWater(const RE::NiPoint3& a_pos, float a_waterHeight)
	{
		auto* cell = ResolveProbeCell(a_pos);
		if (!cell) return false;

		float surfaceZ = 0.0f;
		if (!FindSolidSurfaceZAtPosition(cell, a_pos, kDeckProbeAbove, kDeckProbeBelow, surfaceZ)) {
			return false;
		}

		return surfaceZ > a_waterHeight + kSurfaceSlop;
	}

	// ╔══════════════════════════════════════════════════════════════════════════╗
	// ║  BRIDGES (and anything else standing well above the water)               ║
	// ╚══════════════════════════════════════════════════════════════════════════╝
	//
	// IsOnStructureAboveWater only probes a narrow band (kDeckProbeAbove/Below, 8/24
	// units) around the point being tested. That's the right size for a dock, jetty or
	// boat hull sitting just above the surface, but it is much too short to reach a
	// stone bridge deck that stands many hundreds of units above the water - the probe
	// simply never gets that high, so a bridge is invisible to it wherever it's tested
	// near the waterline.
	//
	// FindWaterHitPosition never runs into this because a_pos there is a REAL Havok
	// impact position - if something hit the bridge deck, a_pos already sits AT the
	// deck's height, so the narrow band centered on it still finds it. But
	// FindContinuousEffectWaterCrossing (below) has no such luck: for a hitscan Beam or a
	// Cone/Flame effect it never has an actual impact, only a straight-line geometric
	// guess at where the aim ray WOULD cross the water plane if nothing were in the way -
	// and that guess point sits right at the water's height, nowhere near the bridge deck
	// far above it. Probing near the guessed crossing point can't see the deck at all.
	//
	// The fix is to stop guessing about a single point and instead ask the one question
	// that actually matters: does the ray the beam is really traveling along hit anything
	// solid before it would reach the water? A real Havok raycast from where the effect
	// actually originates down to (just short of) the computed water crossing catches a
	// blocking bridge deck, dock underside, rock overhang or any other obstruction at
	// WHATEVER height it stands at - not just within a fixed band near the surface.
	static bool IsRayBlockedBeforeWater(const RE::NiPoint3& a_rayStart, const RE::NiPoint3& a_contact)
	{
		auto* cell = ResolveProbeCell(a_contact);
		if (!cell) return false;

		auto* world = cell->GetbhkWorld();
		if (!world) return false;

		static const std::vector<RE::COL_LAYER> kSolidLayers = {
			RE::COL_LAYER::kTerrain,
			RE::COL_LAYER::kStatic,
			RE::COL_LAYER::kAnimStatic,
			RE::COL_LAYER::kGround,
			RE::COL_LAYER::kClutterLarge,
			RE::COL_LAYER::kTrap
		};

		const auto scale = RE::bhkWorld::GetWorldScale();
		if (!(scale > 0.0f)) return false;

		// Pull the far end of the ray back a hair short of the water-crossing point
		// itself, so a bed that happens to reach right up to the waterline (already
		// rejected separately by HasGroundAtOrAboveWater) isn't also flagged as
		// "blocked" here - we only care about something ABOVE the water intercepting
		// the ray before it gets there. When the whole start-to-contact distance is
		// already at or under that same slop (a close-range cast into shallow water a
		// short reach away), there's no meaningful stretch of ray left to test once
		// it's pulled back, so skip the raycast entirely rather than let rayEnd default
		// to the contact point itself - casting all the way down to the water height
		// on shallow water risks clipping the shallow bed just underneath it and
		// misreporting a shore-depth floor as a blocking structure.
		const RE::NiPoint3 seg = a_contact - a_rayStart;
		const float segLen = std::sqrt(seg.x * seg.x + seg.y * seg.y + seg.z * seg.z);
		if (segLen <= kSurfaceSlop) return false;

		const float t = (segLen - kSurfaceSlop) / segLen;
		const RE::NiPoint3 rayEnd = { a_rayStart.x + seg.x * t, a_rayStart.y + seg.y * t, a_rayStart.z + seg.z * t };

		for (RE::COL_LAYER layer : kSolidLayers) {
			RE::bhkPickData pick;
			pick.rayInput.from = a_rayStart * scale;
			pick.rayInput.to = rayEnd * scale;
			pick.rayInput.filterInfo = static_cast<uint32_t>(layer);

			world->PickObject(pick);
			if (pick.rayOutput.HasHit()) return true;
		}

		return false;
	}

	bool FindWaterHitPosition(RE::NiPoint3& a_pos, float a_aboveTolerance = 20.0f)
	{
		float waterHeight = 0.0f;
		if (!FindWaterHeightAtPosition(a_pos, waterHeight)) return false;

		if (a_pos.z > waterHeight + a_aboveTolerance) return false;

		// Anything above the plane by more than wave/jitter slop has to prove it isn't
		// resting on a bridge, dock, jetty or boat deck before it counts as a splash.
		// A hit at or below the surface skips this entirely - it is already in the water.
		if (a_pos.z > waterHeight + kSurfaceSlop && IsOnStructureAboveWater(a_pos, waterHeight)) {
			logger::debug("HitWater gate: ({:.1f},{:.1f}) hitZ={:.2f} waterZ={:.2f} -> struck a structure above the water, not water",
				a_pos.x, a_pos.y, a_pos.z, waterHeight);
			return false;
		}

		// See HasGroundAtOrAboveWater above: solid ground at this exact (x,y) that
		// reaches the waterline means this was a shore/ground hit that merely happens
		// to sit inside the water volume's coarse bounding box, not a water hit.
		if (HasGroundAtOrAboveWater(a_pos, waterHeight)) return false;

		a_pos.z = waterHeight;
		return true;
	}

	// Why Sparks/Lightning Bolt (BeamProjectile) never triggered kHitWater at all, even in
	// shallow water: Beam is a hitscan, not a particle that physically travels through
	// space. Unlike a FlameProjectile/ConeProjectile - whose GetPosition() does advance
	// tick to tick as the effect's leading edge moves - a Beam's GetPosition() sits at (or
	// very near) the caster and its linearVelocity is effectively zero, because there's
	// nothing "in flight" for it to update. Testing that single reported point against the
	// water plane (what the per-frame poll below does for ballistic Missile/Arrow) is
	// therefore checking the wrong point entirely: almost by definition it's the point right
	// next to the caster, not wherever downrange the beam currently reaches.
	//
	// The same root cause can occasionally undershoot Flame/Cone too - their position does
	// move, but only to wherever the effect's own origin/leading-edge currently is, which
	// isn't guaranteed to be the exact spot where the cone's far edge is crossing the water.
	//
	// Fix (matching the approach independently used by Elementally Reactive Waters for these
	// same spell types): reconstruct the effect's actual current ray - starting point to
	// current effective reach - and sample along THAT for a water crossing, rather than
	// trusting a single reported position. Direction comes from linearVelocity when the
	// projectile actually has any (Flame/Cone usually do); when it doesn't (Beam, always),
	// this falls back to the caster's own aim angle, which is the only "direction" a hitscan
	// beam has. Returns true and fills a_outContact (already snapped to the water surface)
	// on the first sampled segment that crosses the water plane.
	bool FindContinuousEffectWaterCrossing(RE::Projectile* a_proj, RE::NiPoint3& a_outContact)
	{
		auto& runtime = a_proj->GetProjectileRuntimeData();
		const auto* base = a_proj->GetProjectileBase();
		const RE::NiPoint3 current = a_proj->GetPosition();

		const bool isBeam = a_proj->As<RE::BeamProjectile>() != nullptr;

		// DIAGNOSTIC (temporary): two guesses at the Beam retrigger bug (stale
		// GetPosition()/GetAngle(), then actor->GetAngle()) haven't fixed it, so rather than
		// guess a third time this logs the actual numbers going into the ray each time for
		// Beam specifically, throttled to ~3/sec so a few seconds of holding Sparks over
		// water produces a readable log instead of a flood. Safe to remove once the real
		// cause is confirmed from the log.
		// NOTE: this function runs on engine job threads (several projectiles' Update()/AddImpact
		// can run concurrently), so this static map MUST be locked. It used to be mutated with
		// no synchronization, which corrupts its bucket bookkeeping and ends in
		// std::length_error("invalid hash bucket count") from a later, unrelated rehash - the
		// crash in the crash logs. A held Beam is also destroyed/recreated every ~0.15s with a
		// fresh FormID, so it grew without bound as well; it is now capped.
		static std::mutex s_beamDebugMutex;
		static std::unordered_map<RE::FormID, float> s_beamDebugLastLog;
		bool shouldLog = false;
		if (isBeam) {
			const auto fid = a_proj->GetFormID();
			std::lock_guard lock(s_beamDebugMutex);
			if (s_beamDebugLastLog.size() > 128) s_beamDebugLastLog.clear();
			auto dit = s_beamDebugLastLog.find(fid);
			if (dit == s_beamDebugLastLog.end() || runtime.livingTime - dit->second >= 0.3f) {
				shouldLog = true;
				s_beamDebugLastLog[fid] = runtime.livingTime;
			}
		}

		// Cone (Flames/Frostbite) tracks its own cast origin separately from GetPosition() -
		// preferring it as the ray's start point (when it looks sane) keeps the sampled ray
		// anchored at the caster instead of at whatever point GetPosition() last reported.
		RE::NiPoint3 start = current;
		RE::NiPoint3 angleSource = a_proj->GetAngle();
		bool usedLiveActor = false;
		if (auto* cone = a_proj->As<RE::ConeProjectile>()) {
			const auto& origin = cone->GetConeRuntimeData().origin;
			if (std::isfinite(origin.x) && std::isfinite(origin.y) && std::isfinite(origin.z) &&
				origin.GetDistance(current) <= 4096.0f) {
				start = origin;
			}
		} else if (isBeam) {
			// Beam is a hitscan - its own GetPosition()/GetAngle() are set once at spawn and
			// don't keep tracking the caster the way Cone's per-tick origin does, or the way
			// the visible beam effect itself does (that's driven straight off the caster's
			// aim node every frame, entirely separately from this Projectile object). Held
			// for more than an instant while the caster keeps adjusting their aim, the frozen
			// values drift away from where the beam is actually pointing - explaining why
			// only the very first poll after casting ever found a valid crossing. Re-deriving
			// both the ray's start point and its direction from the live casting actor each
			// poll keeps it glued to wherever the beam is actually aimed right now.
			if (auto actorCause = runtime.actorCause) {
				if (auto& actorHandle = actorCause->actor) {
					if (auto actorPtr = actorHandle.get()) {
						if (actorPtr) {
							start = actorPtr->GetPosition();
							start.z += 64.0f; // rough hand/chest height above the actor's feet
							angleSource = actorPtr->GetAngle();
							usedLiveActor = true;
						}
					}
				}
			}
		}

		RE::NiPoint3 direction = runtime.linearVelocity;
		const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		bool usedAngleFallback = false;
		if (!std::isfinite(length) || length < 0.001f) {
			// No usable linearVelocity (always true for Beam) - fall back to an aim angle.
			// angleSource is the live casting actor's for Beam (see above), or otherwise
			// a_proj->GetAngle() (x = pitch, z = yaw), matching how RE::Projectile itself
			// derives a heading elsewhere in the engine.
			usedAngleFallback = true;
			const float horizontal = std::cos(angleSource.x);
			direction = { std::sin(angleSource.z) * horizontal, std::cos(angleSource.z) * horizontal, -std::sin(angleSource.x) };
		} else {
			direction.x /= length;
			direction.y /= length;
			direction.z /= length;
		}

		if (shouldLog) {
			logger::info("[OIF kHitWater DEBUG] Beam id={:X} livingTime={:.2f} usedLiveActor={} usedAngleFallback={} "
				"start=({:.1f},{:.1f},{:.1f}) angleSrc=({:.3f},{:.3f},{:.3f}) velLen={:.4f} dir=({:.3f},{:.3f},{:.3f})",
				a_proj->GetFormID(), runtime.livingTime, usedLiveActor, usedAngleFallback,
				start.x, start.y, start.z, angleSource.x, angleSource.y, angleSource.z, length,
				direction.x, direction.y, direction.z);
		}

		if (direction.z >= -0.001f) {
			if (shouldLog) logger::info("[OIF kHitWater DEBUG] Beam id={:X} rejected: direction.z={:.4f} (not aimed downward)", a_proj->GetFormID(), direction.z);
			return false; // aimed level/upward - nothing below it to cross
		}

		const float configuredRange = (std::max)(base ? base->data.range : 0.0f, runtime.range);
		const float rayLength = std::clamp(configuredRange > 0.0f ? configuredRange : 768.0f, 128.0f, 4096.0f);
		const RE::NiPoint3 end{
			start.x + direction.x * rayLength,
			start.y + direction.y * rayLength,
			start.z + direction.z * rayLength
		};

		// STOPPING SHORT: why dry stairs, or dry ground near a riverbank, could still
		// fire kHitWater even after the per-candidate checks below. Those checks only
		// ever look at what's immediately around the ONE candidate contact point that
		// the 8 fixed samples happened to land on - nothing stops an earlier sample
		// from landing PAST a real obstruction (a staircase, a stretch of dry path)
		// that falls between two of those 8 points rather than exactly on one of
		// them. A spark aimed up a flight of stairs, or across dry ground toward a
		// riverbank, could then report whatever real (but far more distant and
		// unrelated) water happened to lie further along the very same aim line, as
		// if that's what got hit - which is exactly what turned up hitting stairs and
		// dry ground nowhere near any water.
		//
		// Fix: one authoritative raycast along the WHOLE nominal ray, done up front,
		// so every sample below is clamped to wherever solid geometry would actually
		// have stopped the beam - never beyond it, no matter where that obstruction
		// falls relative to the 8 fixed sample points. This doesn't replace the
		// per-candidate checks further down (IsOnStructureAboveWater and
		// IsRayBlockedBeforeWater) - a low obstruction sitting right at the reachable
		// limit, or thin geometry this single pick misses, still needs them - it just
		// means they're now working with a ray that can no longer already be past
		// something the beam never got to.
		float blockFraction = 1.0f;
		if (auto* blockCell = ResolveProbeCell(start)) {
			if (auto* blockWorld = blockCell->GetbhkWorld()) {
				if (const auto blockScale = RE::bhkWorld::GetWorldScale(); blockScale > 0.0f) {
					static const std::vector<RE::COL_LAYER> kBlockSolidLayers = {
						RE::COL_LAYER::kTerrain,
						RE::COL_LAYER::kStatic,
						RE::COL_LAYER::kAnimStatic,
						RE::COL_LAYER::kGround,
						RE::COL_LAYER::kClutterLarge,
						RE::COL_LAYER::kTrap
					};
					for (RE::COL_LAYER layer : kBlockSolidLayers) {
						RE::bhkPickData pick;
						pick.rayInput.from = start * blockScale;
						pick.rayInput.to = end * blockScale;
						pick.rayInput.filterInfo = static_cast<uint32_t>(layer);

						blockWorld->PickObject(pick);
						if (pick.rayOutput.HasHit() && pick.rayOutput.hitFraction < blockFraction) {
							blockFraction = pick.rayOutput.hitFraction;
						}
					}
				}
			}
		}
		const RE::NiPoint3 reachEnd{
			start.x + (end.x - start.x) * blockFraction,
			start.y + (end.y - start.y) * blockFraction,
			start.z + (end.z - start.z) * blockFraction
		};

		if (shouldLog) {
			float waterAtStart = 0.0f;
			bool haveWaterAtStart = FindWaterHeightAtPosition(start, waterAtStart);
			logger::info("[OIF kHitWater DEBUG] Beam id={:X} rayLength={:.1f} end=({:.1f},{:.1f},{:.1f}) blockFraction={:.3f} reachEnd=({:.1f},{:.1f},{:.1f}) waterAtStart={} height={:.1f}",
				a_proj->GetFormID(), rayLength, end.x, end.y, end.z, blockFraction, reachEnd.x, reachEnd.y, reachEnd.z, haveWaterAtStart, waterAtStart);
		}

		constexpr int kProbeCount = 8;
		for (int i = 1; i <= kProbeCount; ++i) {
			const float t = static_cast<float>(i) / static_cast<float>(kProbeCount);
			const RE::NiPoint3 sample{
				start.x + (reachEnd.x - start.x) * t,
				start.y + (reachEnd.y - start.y) * t,
				start.z + (reachEnd.z - start.z) * t
			};

			float waterHeight = 0.0f;
			if (!FindWaterHeightAtPosition(sample, waterHeight)) continue;
			if (start.z <= waterHeight + 1.0f || reachEnd.z > waterHeight + 1.0f) continue;

			const float denom = start.z - reachEnd.z;
			if (denom <= std::numeric_limits<float>::epsilon()) return false;

			const float contactT = std::clamp((start.z - waterHeight) / denom, 0.0f, 1.0f);
			RE::NiPoint3 contact{
				start.x + (reachEnd.x - start.x) * contactT,
				start.y + (reachEnd.y - start.y) * contactT,
				start.z + (reachEnd.z - start.z) * contactT
			};

			float confirmHeight = 0.0f;
			if (!FindWaterHeightAtPosition(contact, confirmHeight)) continue;

			// Same shoreline gate the AddImpact path uses (see HasGroundAtOrAboveWater):
			// without it, a beam/cone aimed at a riverbank inside the water volume's
			// bounding box reports a "crossing" over dry land. Continue rather than
			// return, so a ray that clips a bank on an early probe can still find real
			// water further along.
			if (HasGroundAtOrAboveWater(contact, confirmHeight)) {
				if (shouldLog) logger::info("[OIF kHitWater DEBUG] Beam id={:X} crossing at t={:.2f} rejected: solid ground reaches the waterline here",
					a_proj->GetFormID(), t);
				continue;
			}

			// STRUCTURES ABOVE WATER (bridges, docks, rocks/logs poking out of shallow
			// water, etc). Unlike the AddImpact path, this function never has an actual
			// Havok hit to test - it walks straight-line samples along the beam's aim
			// ray purely geometrically, without regard to what the beam would actually
			// have struck first. So a spark/flame/cone aimed at a bridge deck, or at a
			// rock or plant sticking up out of shallow water, still finds a "crossing"
			// wherever the straight ray dips below the water plane underneath or beyond
			// that object - even though in-game the beam was stopped at the object and
			// never reached the water at all. IsOnStructureAboveWater catches this for
			// LOW obstructions close to the surface (docks, jetties), but a tall bridge
			// deck standing far above the water is outside its narrow probe band
			// centered on the (near-waterline) contact point - IsRayBlockedBeforeWater
			// catches that case by testing the beam's actual ray, at whatever height the
			// obstruction stands.
			if (IsOnStructureAboveWater(contact, confirmHeight) || IsRayBlockedBeforeWater(start, contact)) {
				if (shouldLog) logger::info("[OIF kHitWater DEBUG] Beam id={:X} crossing at t={:.2f} rejected: a structure stands above the water here",
					a_proj->GetFormID(), t);
				continue;
			}

			contact.z = confirmHeight;
			a_outContact = contact;
			if (shouldLog) logger::info("[OIF kHitWater DEBUG] Beam id={:X} CROSSING FOUND at t={:.2f} contact=({:.1f},{:.1f},{:.1f})",
				a_proj->GetFormID(), t, contact.x, contact.y, contact.z);
			return true;
		}

		if (shouldLog) logger::info("[OIF kHitWater DEBUG] Beam id={:X} no crossing found along ray", a_proj->GetFormID());
		return false;
	}

	// Reads off a_proj's attack classification (spell/weapon/explosion, weapon type,
	// attack type, delivery type, and the base-form attack/projectile sources used for
	// filter matching). MUST be called synchronously - i.e. directly from the AddImpact
	// hook thunk, right after the original implementation runs, while a_proj is still
	// guaranteed to be a live, addressable object. See the note on QueueProjectileImpact
	// below for why this can no longer be postponed to the deferred main-thread task.
	bool ExtractProjectileAttackInfo(RE::Projectile* a_proj, RE::Actor*& a_actor, RE::TESForm*& a_attackSource,
		RE::TESForm*& a_projectileSource, WeaponType& a_weaponType, AttackType& a_attackType, DeliveryType& a_deliveryType)
	{
		if (!a_proj) return false;

		auto& projData = a_proj->GetProjectileRuntimeData();

		// a_proj is the live Projectile *reference* flying through the world (it derives
		// from TESObjectREFR), so a_proj->GetFormID() is a per-shot, ever-changing runtime
		// FormID. The "projectiles"/"projectilesNot" filters are configured with the
		// BGSProjectile *base form* identifier (e.g. "Skyrim.esm|0x0001B4"), so we must
		// resolve the instance down to its base object before using it as the comparison
		// key - otherwise it can never match and the filter silently does nothing.
		RE::TESForm* projectileBase = a_proj->GetBaseObject();
		a_attackSource = projectileBase ? projectileBase : static_cast<RE::TESForm*>(a_proj);
		a_projectileSource = projectileBase ? projectileBase : static_cast<RE::TESForm*>(a_proj);
		a_weaponType = WeaponType::Ranged;
		a_attackType = AttackType::Regular;
		a_deliveryType = DeliveryType::None;

		a_actor = RE::PlayerCharacter::GetSingleton();
		if (auto actorCause = projData.actorCause) {
			if (auto& actorHandle = actorCause->actor) {
				if (auto actorPtr = actorHandle.get()) {
					if (actorPtr) a_actor = actorPtr.get();
				}
			}
		}
		if (!EventSinkBase::IsActorSafe(a_actor)) return false;

		if (auto* projSpell = projData.spell) {
			if (auto* spell = projSpell->As<RE::SpellItem>()) {
				a_attackSource = spell;
				a_weaponType = GetSpellType(spell);

				switch (spell->data.castingType) {
				case RE::MagicSystem::CastingType::kConcentration:
					a_attackType = AttackType::Continuous;
					break;
				case RE::MagicSystem::CastingType::kFireAndForget:
					a_attackType = AttackType::FireAndForget;
					break;
				case RE::MagicSystem::CastingType::kConstantEffect:
					a_attackType = AttackType::Constant;
					break;
				case RE::MagicSystem::CastingType::kScroll:
					a_weaponType = WeaponType::Scroll;
					break;
				default:
					break;
				}

				switch (spell->data.delivery) {
				case RE::MagicSystem::Delivery::kSelf:
					a_deliveryType = DeliveryType::Self;
					break;
				case RE::MagicSystem::Delivery::kAimed:
					a_deliveryType = DeliveryType::Aimed;
					break;
				case RE::MagicSystem::Delivery::kTargetActor:
					a_deliveryType = DeliveryType::TargetActor;
					break;
				case RE::MagicSystem::Delivery::kTargetLocation:
					a_deliveryType = DeliveryType::TargetLocation;
					break;
				case RE::MagicSystem::Delivery::kTouch:
					a_deliveryType = DeliveryType::Touch;
					break;
				case RE::MagicSystem::Delivery::kTotal:
					a_deliveryType = DeliveryType::Total;
					break;
				default:
					a_deliveryType = DeliveryType::None;
					break;
				}
			}
		} else if (auto* projWeapon = projData.weaponSource) {
			a_attackSource = projWeapon;
			a_weaponType = GetWeaponType(projWeapon);

			if (auto* actorState = a_actor->GetActorRuntimeData().currentProcess) {
				if (auto& highData = actorState->high) {
					if (auto& attackData = highData->attackData) {
						a_attackType = GetAttackType(attackData.get());
					}
				}
			}
		} else if (auto* projExplosion = projData.explosion) {
			a_attackSource = projExplosion;
			a_weaponType = WeaponType::Explosion;
		}

		return true;
	}

	// ╔═══════════════════════════════════════════════╗
	// ║  PER-FRAME WATER-CROSSING DETECTION (kHitWater) ║
	// ╚═══════════════════════════════════════════════╝
	// FindWaterHitPosition (used by ProcessProjectileImpact/AttackBlockHook below) only ever
	// sees the position AddImpact reports - and Skyrim's water has no Havok collision, so a
	// projectile shot into open water doesn't stop there. It keeps falling until it hits
	// something solid (usually the seabed, sometimes nothing at all, in which case AddImpact
	// never fires for that shot). Either way, by the time (or if) AddImpact runs, the actual
	// moment of crossing the surface has already passed silently.
	//
	// The reliable fix - independently confirmed by two sources: the publicly available
	// "Universal Arrow Spin" SKSE plugin, which hooks RE::ArrowProjectile::VTABLE[0] index
	// 0xAB for RE::Projectile::Update(float); and RavenKZP's Frostwalker (the same author
	// credited above for the AddImpact vtable-index research), which uses this same
	// per-frame polling approach to detect frost-spell projectiles touching water - is to
	// check every in-flight projectile's position against the local water height on every
	// Update tick, independent of whether/where/when it eventually collides with something
	// solid. The instant its position is at or below the water plane, that IS the splash.
	//
	// g_waterHitReported exists purely to fire kHitWater once per projectile instance rather
	// than once per frame for every tick it spends underwater - it's cleared for an instance
	// once AddImpact's kProcessedImpacts flag confirms that projectile is fully resolved, so
	// it never grows unbounded during normal play. This "fire once, ever" rule is correct for
	// a one-shot Missile/Arrow (it only ever crosses the surface once, right before it's
	// destroyed), but it's the wrong model for a held continuous effect - see
	// g_continuousWaterRetrigger below.
	// *UpdateHook::thunk (MissileUpdateHook, BeamUpdateHook, FlameUpdateHook,
	// ConeUpdateHook) all funnel into CheckProjectileWaterCrossing below on every
	// in-flight projectile's Update tick, and several installed mods (engine/physics
	// parallelization patches among the load order this crashed under) can end up
	// running different projectiles' Update() concurrently on separate worker
	// threads rather than serially on the main thread. g_waterHitReported,
	// g_continuousWaterRetrigger and g_lastWaterCheckTime below are all mutated by
	// that function with no synchronization of their own, which is exactly the kind
	// of unguarded concurrent std::unordered_map/set mutation that corrupts the
	// container's internal hash-bucket bookkeeping and eventually throws
	// std::length_error("invalid hash bucket count") out of a completely unrelated
	// later emplace/rehash - the crash this mutex fixes. Every read or write of any
	// of these three containers (here, in ForgetProjectileWaterState, and anywhere
	// else they're touched) must hold this lock first.
	std::mutex g_waterStateMutex;

	std::unordered_set<RE::FormID> g_waterHitReported;

	// Continuous effects (Beam/Flame/Cone) are the opposite of a one-shot Missile/Arrow: the
	// same instance can sit in contact with water for as long as the player holds the cast,
	// and each pulse of that contact is a distinct, ongoing interaction rather than a single
	// splash - so reusing g_waterHitReported's "fire once, ever" rule for them meant Sparks/
	// Lightning Bolt (and Flames/Frostbite) only ever triggered kHitWater once per cast and
	// needed a full release-and-recast to fire again.
	//
	// This is keyed by the CASTING ACTOR, not the projectile instance, and stamped with a
	// real wall-clock timestamp rather than the projectile's own livingTime. Both of those
	// choices matter: while Cone/Flame keep the same Projectile instance (and the same
	// steadily-increasing livingTime) for the whole time they're held, a held Beam gets
	// silently destroyed and recreated by the engine every ~0.15s - a brand new FormID with
	// livingTime reset back to 0 each time. Keying/timing this off the projectile would mean
	// every single recreation looks like a fresh, never-before-seen instance with no cooldown
	// history, defeating the whole point of throttling: instead of one clean pulse every
	// kContinuousRetriggerInterval, Beam would re-fire on every ~0.15s recreation - so fast
	// the resulting explosions/notifications visually and audibly blend into what looks like
	// a single sustained effect rather than Flames' distinct, evenly-spaced pulses. Tracking
	// this per actor with steady_clock survives that underlying churn correctly.
	std::unordered_map<RE::FormID, std::chrono::steady_clock::time_point> g_continuousWaterRetrigger;
	constexpr auto kContinuousRetriggerInterval = std::chrono::milliseconds(500);

	// Per Frostwalker: checking every single Update tick (every ~16ms) for every in-flight
	// projectile is wasted work between the moment it's fired and the moment it's actually
	// near any water - kLastCheckInterval throttles the check to a short real-time interval
	// instead of every single call.
	//
	// This used to be measured against the projectile's own livingTime rather than a wall
	// clock, on the theory that livingTime (unlike wall-clock time) is unaffected by game
	// speed/slow-mo. That's true, but it silently assumed livingTime keeps advancing for as
	// long as this function keeps getting called - which turned out not to hold: livingTime
	// is only incremented inside Projectile::Update, and (per an SKSE log showing
	// BeamUpdateHook-driven polling going completely silent a fraction of a second into a
	// 5+ second held Beam cast, despite the beam visibly still firing) Update() apparently
	// stops ticking on a held Beam's Projectile object well before the cast actually ends -
	// this is also now called from AddImpact (BeamImpactHook etc.), which keeps firing for
	// the whole cast. Had this stayed livingTime-based, every AddImpact-driven call after
	// Update() went quiet would compare a frozen value against itself (0.0 < kLastCheckInterval,
	// always true) and be blocked forever after the very first check. A real wall clock has
	// no such assumption baked in.
	constexpr auto kLastCheckInterval = std::chrono::milliseconds(50);
	std::unordered_map<RE::FormID, std::chrono::steady_clock::time_point> g_lastWaterCheckTime;

	void CheckProjectileWaterCrossing(RE::Projectile* a_proj, const RE::NiPoint3* a_realHitPos = nullptr)
	{
		if (!a_proj || a_proj->IsDeleted()) return;

		auto& projData = a_proj->GetProjectileRuntimeData();
		const RE::FormID id = a_proj->GetFormID();

		// kProcessedImpacts means "this instance has resolved its impact and is
		// done" for a ballistic Missile/Arrow that only ever impacts once at
		// the end of its flight - bailing out here for those is correct, since
		// AddImpact/kHitGround has already (or is about to) take over. But a
		// concentration spell - a cone (Flames, Frostbite) or a beam (Sparks,
		// Lightning Bolt) alike - resolves a hit-test against whatever's in its
		// path on effectively every single tick for as long as it's held, by
		// design - it isn't "in flight, not yet resolved" the way a ballistic
		// projectile is. If kProcessedImpacts ends up set on every tick for
		// these (plausible, since they always have *something* to resolve
		// against - the ground if nothing else), this early return would silently
		// skip the water-crossing poll for the spell's entire duration, which is
		// exactly the "continuous spells never trigger kHitWater" symptom this is
		// working around (BeamProjectile was originally missing from this
		// exception, which is exactly why Sparks/Lightning Bolt never triggered
		// kHitWater while Flames/Frostbite did). So FlameProjectile/ConeProjectile/
		// BeamProjectile all skip this particular gate and always fall through to
		// the position check below instead.
		bool isContinuousEffect = a_proj->As<RE::FlameProjectile>() || a_proj->As<RE::ConeProjectile>() || a_proj->As<RE::BeamProjectile>();

		if (!isContinuousEffect && projData.flags.any(RE::Projectile::Flags::kProcessedImpacts)) {
			// AddImpact has (or is about to) resolve this instance through the normal
			// ProcessProjectileImpact/kHitGround path - stop polling it and forget it.
			std::lock_guard lock(g_waterStateMutex);
			g_waterHitReported.erase(id);
			g_lastWaterCheckTime.erase(id);
			return;
		}

		if (!isContinuousEffect) {
			std::lock_guard lock(g_waterStateMutex);
			if (g_waterHitReported.contains(id)) return; // one-shot: already reported this instance's splash
		}

		const auto nowWall = std::chrono::steady_clock::now();
		{
			std::lock_guard lock(g_waterStateMutex);
			if (auto it = g_lastWaterCheckTime.find(id); it != g_lastWaterCheckTime.end()) {
				if (nowWall - it->second < kLastCheckInterval) return;
				it->second = nowWall;
			} else {
				// The SetDelete cleanup hooks are no longer installed (see InstallHooks), so
				// bound these maps here instead: once they get large, drop entries not touched
				// for a few seconds - those projectiles are long gone.
				if (g_lastWaterCheckTime.size() > 256) {
					for (auto pit = g_lastWaterCheckTime.begin(); pit != g_lastWaterCheckTime.end();) {
						if (nowWall - pit->second > std::chrono::seconds(5)) {
							g_waterHitReported.erase(pit->first);
							pit = g_lastWaterCheckTime.erase(pit);
						} else {
							++pit;
						}
					}
				}
				g_lastWaterCheckTime.emplace(id, nowWall);
			}
		}

		// Continuous effects (Beam/Flame/Cone) resolve against whatever's in their path.
		// When a_realHitPos is set, we're being called from the AddImpact hook because a
		// REAL impact just happened - the engine has already determined definitively
		// what the beam/cone struck and where, so test THAT position directly, the same
		// way the ballistic Missile/Arrow path below does. This is the actual fix for
		// dry stairs (or dry ground near a riverbank) intermittently reporting
		// kHitWater: the ray-reconstruction in FindContinuousEffectWaterCrossing has to
		// GUESS the beam's current aim from the caster's angle, because Beam's own
		// GetPosition()/linearVelocity don't track it - and that guess can diverge from
		// the real aim ray the engine used for its own hit test, especially at
		// shallow/grazing angles (aiming roughly along a rising staircase, say), letting
		// the guess sail past a real obstruction the engine already correctly stopped
		// at. On any tick where AddImpact already fired, we have the real answer and
		// don't need to guess at all - only fall back to the speculative
		// ray-reconstruction when there's no real impact to test (the Update-hook poll,
		// mid-flight, since a beam/cone crossing open water with nothing solid in its
		// path may never generate a real impact at all).
		const bool isBeamOuter = a_proj->As<RE::BeamProjectile>() != nullptr; // DIAGNOSTIC (temporary)
		RE::NiPoint3 pos;
		if (isContinuousEffect) {
			if (a_realHitPos) {
				pos = *a_realHitPos;
				if (!FindWaterHitPosition(pos, 0.0f)) return;
			} else if (!FindContinuousEffectWaterCrossing(a_proj, pos)) {
				return;
			}

			// See g_continuousWaterRetrigger's comment above: keyed by the casting actor
			// (stable across a held Beam's rapid instance churn), not the projectile itself.
			// Falls back to the projectile's own FormID only if we genuinely can't resolve a
			// caster, so a stray/orphaned projectile still gets throttled somehow.
			RE::FormID retriggerKey = id;
			if (auto actorCause = projData.actorCause) {
				if (auto& actorHandle = actorCause->actor) {
					if (auto actorPtr = actorHandle.get()) {
						if (actorPtr) retriggerKey = actorPtr->GetFormID();
					}
				}
			}

			const auto nowSteady = std::chrono::steady_clock::now();
			{
				std::lock_guard lock(g_waterStateMutex);
				if (auto it = g_continuousWaterRetrigger.find(retriggerKey); it != g_continuousWaterRetrigger.end()) {
					const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(nowSteady - it->second).count();
					if (nowSteady - it->second < kContinuousRetriggerInterval) {
						if (isBeamOuter) logger::info("[OIF kHitWater DEBUG] Beam id={:X} actor={:X} crossing found but COOLDOWN blocked, elapsedMs={}", id, retriggerKey, elapsedMs);
						return; // still cooling down from this same caster's contact
					}
					if (isBeamOuter) logger::info("[OIF kHitWater DEBUG] Beam id={:X} actor={:X} PASSED cooldown gate, elapsedMs={}, proceeding to dispatch", id, retriggerKey, elapsedMs);
					it->second = nowSteady;
				} else {
					if (g_continuousWaterRetrigger.size() > 128) g_continuousWaterRetrigger.clear();
					g_continuousWaterRetrigger.emplace(retriggerKey, nowSteady);
					if (isBeamOuter) logger::info("[OIF kHitWater DEBUG] Beam id={:X} actor={:X} PASSED cooldown gate (first ever for this actor), proceeding to dispatch", id, retriggerKey);
				}
			}
		} else {
			pos = a_proj->GetPosition();
			if (!FindWaterHitPosition(pos, 0.0f)) return; // still above the surface, or no water here at all
			std::lock_guard lock(g_waterStateMutex);
			g_waterHitReported.insert(id);
		}

		RE::Actor* actor = nullptr;
		RE::TESForm* attackSource = nullptr;
		RE::TESForm* projectileSource = nullptr;
		WeaponType weaponType{};
		AttackType attackType{};
		DeliveryType deliveryType{};
		if (!ExtractProjectileAttackInfo(a_proj, actor, attackSource, projectileSource, weaponType, attackType, deliveryType)) {
			if (isBeamOuter) logger::info("[OIF kHitWater DEBUG] Beam id={:X} ExtractProjectileAttackInfo FAILED, event dropped", id);
			return;
		}

		if (isBeamOuter) logger::info("[OIF kHitWater DEBUG] Beam id={:X} DISPATCHING kHitWater to RuleManager", id);

		RuleContext ctx{
			EventType::kHitWater,
			actor,
			nullptr,
			attackSource,
			projectileSource,
			WeaponTypeToString(weaponType),
			AttackTypeToString(attackType),
			DeliveryTypeToString(deliveryType),
			true
		};
		ctx.hitPos = pos; // already snapped to the water surface by FindWaterHitPosition
		ctx.hasHitPos = true;
		RuleManager::GetSingleton()->Trigger(ctx);
	}

	// See MissileDeleteHook in EventSinks.h - just forgets this instance's water-tracking
	// state the moment it's deleted, so a projectile that never impacts anything can't leave
	// a stale entry in any of these maps.
	void ForgetProjectileWaterState(RE::Projectile* a_proj)
	{
		if (!a_proj) return;
		const auto id = a_proj->GetFormID();
		std::lock_guard lock(g_waterStateMutex);
		g_waterHitReported.erase(id);
		g_lastWaterCheckTime.erase(id);
		// g_continuousWaterRetrigger is intentionally NOT cleared here - it's keyed by the
		// casting actor now, not this (possibly one of several rapidly-churning) projectile
		// instance, and is meant to survive exactly this kind of per-instance deletion. See
		// its declaration comment above CheckProjectileWaterCrossing for why.
	}

	// The deferred half of a projectile impact: scans for nearby Activator/Flora/Tree
	// references (or falls back to a ground hit) and fires rules. Takes only stable
	// data - an Actor re-resolved by FormID, and TESForm* attack/projectile sources that
	// are always persistent base-game forms (a weapon, spell, explosion, or projectile
	// record - never the transient Projectile reference itself) - so it no longer matters
	// whether the original Projectile instance has since been deleted.
	void ProcessProjectileImpact(RE::FormID a_actorID, const RE::NiPoint3& a_hitPos, RE::FormID a_refID,
		RE::TESForm* a_attackSource, RE::TESForm* a_projectileSource, WeaponType a_weaponType, AttackType a_attackType,
		DeliveryType a_deliveryType)
	{
		auto* actor = RE::TESForm::LookupByID<RE::Actor>(a_actorID);
		if (!EventSinkBase::IsActorSafe(actor)) return;

		RE::TESObjectCELL* cell = actor->GetParentCell();
		if (!cell) {
			if (auto* player = RE::PlayerCharacter::GetSingleton()) cell = player->GetParentCell();
		}
		if (!cell) return;

		std::vector<RE::TESObjectREFR*> validObjects;

		cell->ForEachReferenceInRange(a_hitPos, 65.0, [&](RE::TESObjectREFR* ref) {
			if (!EventSinkBase::IsItemSafe(ref)) return RE::BSContainer::ForEachResult::kContinue;
			auto* baseObj = ref->GetBaseObject();
			if (!baseObj) return RE::BSContainer::ForEachResult::kContinue;

			switch (baseObj->GetFormType()) {
			case RE::FormType::Activator:  // For some activators with bad collision (e.g., some levers can't be hit by arrows)
			case RE::FormType::Flora:
			case RE::FormType::Tree:
				break;
			default:
				return RE::BSContainer::ForEachResult::kContinue;
			}

			validObjects.emplace_back(ref);
			return RE::BSContainer::ForEachResult::kContinue;
		});

		if (validObjects.empty()) {
			// AddImpact's own a_ref parameter is the authoritative "what did we actually
			// hit" signal - a much more reliable one than the relevantFormTypes-filtered
			// scan above, which deliberately excludes actors (and other form types outside
			// its narrow list) and was therefore always coming back empty for e.g. an arrow
			// hitting an NPC. That NPC hit is real and is already handled separately by the
			// native TESHitEvent -> HitSink path; without this check it would additionally,
			// wrongly, fire kHitGround here too, since the NPC is normally standing right on
			// top of terrain that IsGroundHitAtPosition would happily confirm.
			auto* hitRef = a_refID ? RE::TESForm::LookupByID<RE::TESObjectREFR>(a_refID) : nullptr;
			const bool struckReference = hitRef && !hitRef->IsDeleted();

			// WATER IS CHECKED BEFORE BAILING OUT ON THAT REFERENCE.
			//
			// Water isn't a solid Havok collidable, so a projectile keeps going until it
			// hits whatever is under it. In DEEP water that is usually the terrain
			// heightmap, which is not a TESObjectREFR, so no reference is reported. In
			// SHALLOW water the bed is very often a static mesh instead - a riverbed,
			// pond bottom, trough, basin - which IS a reference, so bailing out here
			// dropped every splash on that kind of water body while the identical shot
			// into a deep lake worked.
			//
			// PREVIOUSLY: when a reference was struck, the tolerance passed to
			// FindWaterHitPosition was -kSurfaceSlop instead of the generous 20.0f used
			// otherwise - the reasoning being that a bridge deck, dock, jetty or boat hull
			// over the water is also a reference, so the impact ought to be verifiably
			// BELOW the waterline before counting as water at all. That reasoning
			// predates IsOnStructureAboveWater/HasGroundAtOrAboveWater below, which do the
			// actual work of telling a shallow riverbed's real (wet) bed apart from a
			// bridge/dock's real (dry, standing above the plane) deck via an actual Havok
			// probe - not by guessing by depth. Once those gates existed, the negative
			// tolerance stopped adding any real protection and instead became the reason
			// shallow water with a referenced bed (a riverbed a fraction of a unit down,
			// well short of the 2-unit floor -kSurfaceSlop demanded) silently failed to
			// register at all - directly causing "shallow water never fires kHitWater".
			// struckReference no longer changes the tolerance; IsOnStructureAboveWater and
			// HasGroundAtOrAboveWater (both called unconditionally inside
			// FindWaterHitPosition) are what actually separates the two cases now.
			RE::NiPoint3 waterPos = a_hitPos;
			if (FindWaterHitPosition(waterPos, 20.0f)) {
				RuleContext ctx{
					EventType::kHitWater,
					actor,
					nullptr,
					a_attackSource,
					a_projectileSource,
					WeaponTypeToString(a_weaponType),
					AttackTypeToString(a_attackType),
					DeliveryTypeToString(a_deliveryType),
					true
				};
				ctx.hitPos = waterPos;
				ctx.hasHitPos = true;
				RuleManager::GetSingleton()->Trigger(ctx);
				return;
			}

			// Not water: the reference reported above is what was actually struck, and
			// that hit is handled elsewhere (TESHitEvent -> HitSink), so don't also fire
			// a ground hit for it.
			if (struckReference) return;

			// Verify (rather than assume) that the empty scan above is because the
			// projectile actually struck bare terrain, then fire kHitGround with
			// target=nullptr and the impact position instead.
			if (!IsGroundHitAtPosition(cell, a_hitPos)) return;

			RuleContext ctx{
				EventType::kHitGround,
				actor,
				nullptr,
				a_attackSource,
				a_projectileSource,
				WeaponTypeToString(a_weaponType),
				AttackTypeToString(a_attackType),
				DeliveryTypeToString(a_deliveryType),
				true
			};
			ctx.hitPos = a_hitPos;
			ctx.hasHitPos = true;
			RuleManager::GetSingleton()->Trigger(ctx);
			return;
		}

		for (auto& ref : validObjects) {
			if (EventSinkBase::IsItemSafe(ref)) {
				RuleContext ctx{
					EventType::kHit,
					actor,
					ref,
					a_attackSource,
					a_projectileSource,
					WeaponTypeToString(a_weaponType),
					AttackTypeToString(a_attackType),
					DeliveryTypeToString(a_deliveryType),
					true
				};
				// Populated here (in addition to the kHitGround/kHitWater branches
				// above) so spawnType 11 ("exact hit point") also works for a
				// projectile that struck an actual object, not just bare terrain
				// or water. Melee/magic TESHitEvent hits have no equivalent impact
				// coordinate, so ctx.hasHitPos stays false for those and spawnType
				// 11 falls back to the object's center instead.
				ctx.hitPos = a_hitPos;
				ctx.hasHitPos = true;
				RuleManager::GetSingleton()->Trigger(ctx);
			}
		}
	}

	// Entry point called directly from each AddImpact hook thunk, synchronously, while
	// a_proj is still guaranteed valid.
	//
	// This used to work differently: the thunks captured only a_proj's FormID and
	// deferred everything (including re-resolving the Projectile* and reading its attack
	// data) to a SKSE main-thread task, via RE::TESForm::LookupByID<RE::Projectile>(id).
	// That round-trip assumed the projectile would still be alive - and only "IsDeleted()
	// safe" if not - one tick later. Arrows are: they persist visibly stuck in whatever
	// they hit for a while. But fast, decal-less projectiles like Firebolt or Ice Spike
	// are torn down the instant they land, so by the time the deferred task ran,
	// a_proj->IsDeleted() was already true and the whole impact - and any rule relying on
	// it, including "projectiles"/"projectilesNot" filters and Flora/Tree hits - was
	// silently dropped.
	//
	// The fix is to classify the attack (spell/weapon/explosion, weapon/attack/delivery
	// type, attack/projectile source) immediately, here, before deferring anything.
	// Everything this extracts is either a stable base-game form (weapon/spell/explosion/
	// projectile record) or a plain value, so it's safe to carry across the tick boundary
	// with no dependency on a_proj still existing. Only the actual scan + RuleManager
	// trigger - the part with a real reentrancy concern - stays deferred.
	void QueueProjectileImpact(RE::Projectile* a_proj, const RE::NiPoint3& a_hitPos, RE::FormID a_refID = 0)
	{
		if (!a_proj || a_proj->IsDeleted()) return;

		try {
			if (!std::isfinite(a_hitPos.x) || !std::isfinite(a_hitPos.y) || !std::isfinite(a_hitPos.z)) {
				return;
			}
		} catch (...) {
			return;
		}

		RE::Actor* actor = nullptr;
		RE::TESForm* attackSource = nullptr;
		RE::TESForm* projectileSource = nullptr;
		WeaponType weaponType = WeaponType::Ranged;
		AttackType attackType = AttackType::Regular;
		DeliveryType deliveryType = DeliveryType::None;

		if (!ExtractProjectileAttackInfo(a_proj, actor, attackSource, projectileSource, weaponType, attackType, deliveryType)) {
			return;
		}

		RE::FormID actorID = actor->GetFormID();

		SKSE::GetTaskInterface()->AddTask([actorID, a_hitPos, a_refID, attackSource, projectileSource, weaponType, attackType, deliveryType]() {
			ProcessProjectileImpact(actorID, a_hitPos, a_refID, attackSource, projectileSource, weaponType, attackType, deliveryType);
		});
	}


//░██████╗██╗███╗░░██╗██╗░░██╗░██████╗
//██╔════╝██║████╗░██║██║░██╔╝██╔════╝
//╚█████╗░██║██╔██╗██║█████═╝░╚█████╗░
//░╚═══██╗██║██║╚████║██╔═██╗░░╚═══██╗
//██████╔╝██║██║░╚███║██║░╚██╗██████╔╝
//╚═════╝░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝╚═════╝░                           
    
	RE::BSEventNotifyControl ActivateSink::ProcessEvent(const RE::TESActivateEvent* evn, RE::BSTEventSource<RE::TESActivateEvent>*)
    {
        if (!evn || !evn->objectActivated) return RE::BSEventNotifyControl::kContinue;

        auto targetRef = evn->objectActivated;
		if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;

        auto* baseObj = targetRef->GetBaseObject();
        if (!baseObj) return RE::BSEventNotifyControl::kContinue;
        
        RE::Actor* source = nullptr;
		if (evn->actionRef && evn->actionRef.get()) source = evn->actionRef.get()->As<RE::Actor>();
        if (!EventSinkBase::IsActorSafe(source)) return RE::BSEventNotifyControl::kContinue;

        RE::FormID sourceID = source->GetFormID();
        RE::FormID targetID = targetRef->GetFormID();

        SKSE::GetTaskInterface()->AddTask([sourceID, targetID]() {
            auto* source = RE::TESForm::LookupByID<RE::Actor>(sourceID);
            auto* targetRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(targetID);
            
            RuleContext ctx{ 
                EventType::kActivate, 
                source, 
                targetRef
            };
            RuleManager::GetSingleton()->Trigger(ctx);
        });

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl HitSink::ProcessEvent(const RE::TESHitEvent* evn, RE::BSTEventSource<RE::TESHitEvent>*)
    {
        if (!evn || !evn->target) return RE::BSEventNotifyControl::kContinue;

        auto targetRef = evn->target;
        if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;

        auto* baseObj = targetRef->GetBaseObject();
        if (!baseObj) return RE::BSEventNotifyControl::kContinue;
        
        RE::Actor* source = nullptr;
        if (evn->cause && evn->cause.get()) source = evn->cause.get()->As<RE::Actor>();
        if (!EventSinkBase::IsActorSafe(source)) return RE::BSEventNotifyControl::kContinue;

        RE::FormID hitSourceID = evn->source;
        RE::FormID projectileID = evn->projectile;
        RE::FormID sourceID = source->GetFormID();
        RE::FormID targetID = targetRef->GetFormID();

        SKSE::GetTaskInterface()->AddTask([hitSourceID, projectileID, sourceID, targetID]() {
            auto* source = RE::TESForm::LookupByID<RE::Actor>(sourceID);
            auto* targetRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(targetID);
            auto* hitSourceForm = hitSourceID ? RE::TESForm::LookupByID(hitSourceID) : nullptr;
            auto* projectileForm = projectileID ? RE::TESForm::LookupByID<RE::BGSProjectile>(projectileID) : nullptr;

            if (!EventSinkBase::IsActorSafe(source) || !EventSinkBase::IsItemSafe(targetRef)) return;

            RE::TESForm* attackSource = nullptr;
            RE::TESForm* projectileSource = nullptr;
            WeaponType weaponType = WeaponType::Other;
            AttackType attackType = AttackType::Regular;
            DeliveryType deliveryType = DeliveryType::None;

            if (hitSourceForm && hitSourceForm->As<RE::BGSExplosion>()) return;

            if (auto* actorState = source->GetActorRuntimeData().currentProcess) {
                auto* highData = actorState->high;
                if (!highData) return;

                if (highData->attackData) attackType = GetAttackType(highData->attackData.get());

                if (highData->muzzleFlash && highData->muzzleFlash->baseProjectile) {
                    auto* projectile = highData->muzzleFlash->baseProjectile;
                    if (projectile->As<RE::BGSExplosion>()) return;
					weaponType = WeaponType::Ranged;
                    attackSource = projectile;
                    projectileSource = projectile;
                }
            }

			if (projectileForm) {
				if (projectileForm->As<RE::BGSExplosion>()) return;
				weaponType = WeaponType::Ranged;
				attackSource = projectileForm;
				projectileSource = projectileForm;
			}

            if (hitSourceForm) {
                if (auto* spell = hitSourceForm->As<RE::SpellItem>()) {
                    weaponType = GetSpellType(spell);
                    attackSource = spell;
                    
                    if (spell->effects.size() > 0) {
                        auto* effect = spell->effects[0];
                        if (effect && effect->baseEffect) {
                            switch (effect->baseEffect->data.delivery) {
                                case RE::MagicSystem::Delivery::kSelf:
                                    deliveryType = DeliveryType::Self;
                                    break;
                                case RE::MagicSystem::Delivery::kAimed:
                                    deliveryType = DeliveryType::Aimed;
                                    break;
                                case RE::MagicSystem::Delivery::kTargetActor:
                                    deliveryType = DeliveryType::TargetActor;
                                    break;
                                case RE::MagicSystem::Delivery::kTargetLocation:
                                    deliveryType = DeliveryType::TargetLocation;
                                    break;
                                case RE::MagicSystem::Delivery::kTouch:
                                    deliveryType = DeliveryType::Touch;
                                    break;
                                default:
                                    deliveryType = DeliveryType::None;
                                    break;
                            }
                        }
                    }

                    switch (spell->GetCastingType()) {
                        case RE::MagicSystem::CastingType::kConcentration:
                            attackType = AttackType::Continuous;
                            break;
                        case RE::MagicSystem::CastingType::kFireAndForget:
                            attackType = AttackType::FireAndForget;
                            break;
                        case RE::MagicSystem::CastingType::kConstantEffect:
                            attackType = AttackType::Constant;
                            break;
                        case RE::MagicSystem::CastingType::kScroll:
                            weaponType = WeaponType::Scroll;
                            break;
                        default:
                            break;
                    }
                }

                else if (auto* weapon = hitSourceForm->As<RE::TESObjectWEAP>()) {
                    weaponType = GetWeaponType(weapon);
                    attackSource = weapon;
                }

                // Torches are TESObjectLIGH, not TESObjectWEAP, so a torch-bash's
                // evn->source form fell through both branches above and was left
                // classified as WeaponType::Other - the hit still fired, it just
                // could never match a "torch" weaponTypes filter. CanBeCarried()
                // is what distinguishes a wieldable torch from a static light
                // fixture (which shouldn't be a plausible hit source here anyway,
                // but the check is kept for consistency with the other torch
                // detection sites in this file).
                else if (auto* light = hitSourceForm->As<RE::TESObjectLIGH>()) {
                    if (light->CanBeCarried()) {
                        weaponType = WeaponType::Torch;
                        attackSource = light;
                    }
                }
            }

            RuleContext ctx{ 
                EventType::kHit,
                source,
                targetRef, 
                attackSource,
                projectileSource,
                WeaponTypeToString(weaponType),
                AttackTypeToString(attackType),
                DeliveryTypeToString(deliveryType),
                true
            };
            
            RuleManager::GetSingleton()->Trigger(ctx);
        });

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl MagicEffectApplySink::ProcessEvent(const RE::TESMagicEffectApplyEvent* evn, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*)
    {
        if (!evn || !evn->target) return RE::BSEventNotifyControl::kContinue;
    
        auto targetRef = evn->target;
        if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;
    
        auto* baseObj = targetRef->GetBaseObject();
        if (!baseObj) return RE::BSEventNotifyControl::kContinue;
    
        RE::FormID targetID = 0;
        RE::FormID magicEffectID = 0;
        RE::FormID casterID = 0;
    
        if (targetRef) targetID = targetRef->GetFormID();

        if (evn->magicEffect) magicEffectID = evn->magicEffect;
    
        if (evn->caster) {
            auto* casterRef = evn->caster.get();
			if (auto* casterActor = casterRef->As<RE::Actor>()) {
				if (EventSinkBase::IsActorSafe(casterActor)) {
					casterID = casterRef->GetFormID();
				}
			}
        }
    
        if (targetID == 0) return RE::BSEventNotifyControl::kContinue;
        if (casterID == 0) return RE::BSEventNotifyControl::kContinue;
        if (magicEffectID == 0) return RE::BSEventNotifyControl::kContinue;
    
        SKSE::GetTaskInterface()->AddTask([targetID, magicEffectID, casterID]() {
            auto* targetRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(targetID);
            auto* magicEffect = RE::TESForm::LookupByID<RE::EffectSetting>(magicEffectID);
            auto* caster = RE::TESForm::LookupByID<RE::Actor>(casterID);
            
            if (!EventSinkBase::IsItemSafe(targetRef)) return;
            if (!magicEffect) return;
			if (!EventSinkBase::IsActorSafe(caster)) return;
    
            RE::TESForm* attackSource = magicEffect;
            RE::TESForm* projectileSource = nullptr;
            WeaponType weaponType = WeaponType::Other;
            AttackType attackType = AttackType::Regular;
            DeliveryType deliveryType = DeliveryType::None;
            bool isShout = false;
            bool isSpell = false;

            if (magicEffect->data.projectileBase) {
                if (!magicEffect->data.projectileBase->As<RE::BGSExplosion>()) {
					weaponType = WeaponType::Ranged;
                    attackSource = magicEffect->data.projectileBase;
                    projectileSource = magicEffect->data.projectileBase;
                }
            }
    
            if (magicEffect->data.associatedForm && magicEffect->data.associatedForm->formType == RE::FormType::Shout) {
                weaponType = WeaponType::Shout;
                attackSource = magicEffect->data.associatedForm;
                isShout = true;
            }
            
            if (!isShout) {
                auto* currentShout = caster->GetCurrentShout();
                if (currentShout) {
                    for (int i = 0; i < RE::TESShout::VariationIDs::kTotal; ++i) {
                        auto& variation = currentShout->variations[i];
                        if (variation.spell) {
                            for (auto* effect : variation.spell->effects) {
                                if (effect && effect->baseEffect == magicEffect) {
                                    weaponType = WeaponType::Shout;
                                    attackSource = currentShout;
                                    isShout = true;
                                    break;
                                }
                            }
                            if (isShout) break;
                        }
                    }
                }
            }
    
            if (!isShout) {
                RE::SpellItem* sourceSpell = nullptr;
    
                std::array<RE::MagicSystem::CastingSource, 4> castingSources = {
                    RE::MagicSystem::CastingSource::kInstant,
                    RE::MagicSystem::CastingSource::kLeftHand,
                    RE::MagicSystem::CastingSource::kOther,
                    RE::MagicSystem::CastingSource::kRightHand
                };
    
                for (auto castingSource : castingSources) {
                    if (auto* casterActor = caster->As<RE::Actor>()) {
                        if (auto* magicCaster = casterActor->GetMagicCaster(castingSource)) {
                            if (auto* currentSpell = magicCaster->currentSpell) {
                                for (auto* effect : currentSpell->effects) {
                                    if (effect && effect->baseEffect == magicEffect) {
                                        sourceSpell = currentSpell->As<RE::SpellItem>();
                                        break;
                                    }
                                }
                                if (sourceSpell) break;
                            }
                        }
                    }
                }
    
                if (sourceSpell) {
                    weaponType = GetSpellType(sourceSpell);
                    attackSource = sourceSpell;
                    isSpell = true;
                }
            }

            switch (magicEffect->data.castingType) {
                case RE::MagicSystem::CastingType::kConcentration:
                    attackType = AttackType::Continuous;
                    break;
                case RE::MagicSystem::CastingType::kFireAndForget:
                    attackType = AttackType::FireAndForget;
                    break;
                case RE::MagicSystem::CastingType::kConstantEffect:
                    attackType = AttackType::Constant;
                    break;
                case RE::MagicSystem::CastingType::kScroll:
                    weaponType = WeaponType::Scroll;
                    break;
                default:
                    break;
            }
    
            switch (magicEffect->data.delivery) {
                case RE::MagicSystem::Delivery::kSelf:
                    deliveryType = DeliveryType::Self;
                    break;
                case RE::MagicSystem::Delivery::kAimed:
                    deliveryType = DeliveryType::Aimed;
                    break;
                case RE::MagicSystem::Delivery::kTargetActor:
                    deliveryType = DeliveryType::TargetActor;
                    break;
                case RE::MagicSystem::Delivery::kTargetLocation:
                    deliveryType = DeliveryType::TargetLocation;
                    break;
                case RE::MagicSystem::Delivery::kTouch:
                    deliveryType = DeliveryType::Touch;
                    break;
                case RE::MagicSystem::Delivery::kTotal:
                    deliveryType = DeliveryType::Total;
                    break;
                default:
                    deliveryType = DeliveryType::None;
                    break;
            }

            RuleContext ctx{
                EventType::kHit,
                caster,
                targetRef,
                attackSource,
                projectileSource,
                WeaponTypeToString(weaponType),
                AttackTypeToString(attackType),
                DeliveryTypeToString(deliveryType),
                true
            };
        
            RuleManager::GetSingleton()->Trigger(ctx);
        });
    
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl GrabReleaseSink::ProcessEvent(const RE::TESGrabReleaseEvent* evn, RE::BSTEventSource<RE::TESGrabReleaseEvent>*)
    {
        if (!evn || !evn->ref) return RE::BSEventNotifyControl::kContinue;

        auto targetRef = evn->ref;
		if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;

        RE::Actor* source = nullptr;
        if (auto* handle = RE::PlayerCharacter::GetSingleton()) source = handle->As<RE::Actor>();

        if (!EventSinkBase::IsActorSafe(source)) return RE::BSEventNotifyControl::kContinue;

        bool isGrabbed = evn->grabbed;

        if (isGrabbed) {
            SKSE::GetTaskInterface()->AddTask([source, targetRef]() {
                RuleContext ctx{
                    EventType::kGrab,
                    source,
					targetRef.get(),
					targetRef->GetBaseObject()
                };
                RuleManager::GetSingleton()->Trigger(ctx);
            });
        } else {
            SKSE::GetTaskInterface()->AddTask([source, targetRef]() {
				bool isTelekinesis = false;
                bool isThrown = false;

                RE::TESForm* leftSpell = source->GetEquippedObject(true);
                RE::TESForm* rightSpell = source->GetEquippedObject(false);
                    
                RE::MagicItem* leftMagicItem = leftSpell ? leftSpell->As<RE::MagicItem>() : nullptr;
                RE::MagicItem* rightMagicItem = rightSpell ? rightSpell->As<RE::MagicItem>() : nullptr;
                    
                if ((leftSpell || rightSpell) && (leftMagicItem || rightMagicItem)) {
                    bool hasGrabEffect = false;
                    for (auto* spell : { leftMagicItem, rightMagicItem }) {
                        if (spell) {
                            for (auto* effect : spell->effects) {
                                if (effect && effect->baseEffect) {
                                    if (effect->baseEffect->data.archetype == RE::EffectArchetypes::ArchetypeID::kTelekinesis ||
                                        effect->baseEffect->data.archetype == RE::EffectArchetypes::ArchetypeID::kGrabActor) {
                                        hasGrabEffect = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (hasGrabEffect) break;
                    }
                    isTelekinesis = hasGrabEffect;
                }

                auto* inputHandler = InputHandler::GetSingleton();
                bool wasRKeyReleased = inputHandler->WasKeyJustReleased();
                    
                if (auto threedimObj = targetRef->Get3D()) {
                    if (auto collisionObj = threedimObj->GetCollisionObject()) {
                        if (auto bhkBody = collisionObj->GetRigidBody()) {
                            if (auto hkpBody = bhkBody->GetRigidBody()) {
                                if (isTelekinesis || (!isTelekinesis && wasRKeyReleased)) {
                                    int propertyId = isTelekinesis ? 314159 : 628318; // HK_PROPERTY_TELEKINESIS : HK_PROPERTY_GRABTHROWNOBJECT
                                        
                                    if (hkpBody->HasProperty(propertyId)) {
                                        float now = duration_cast<duration<float>>(steady_clock::now() - startTime).count();
                                        hkpBody->SetProperty(propertyId, now);
                                    }
                                        
                                    hkpBody->AddContactListener(LandingSink::GetSingleton());
                                }
                                    
                                if (wasRKeyReleased) {
                                    isThrown = true;
                                    InputHandler::GetSingleton()->ResetKeyState();
                                }
                            }
                        }
                    }
                }
                if (!isThrown) {
                    RuleContext ctx{
                        EventType::kRelease,
                        source,
						targetRef.get(),
						targetRef->GetBaseObject()
                    };
                    RuleManager::GetSingleton()->Trigger(ctx);
                }
            });
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void LandingSink::ContactPointCallback(const RE::hkpContactPointEvent& a_event) 
    {
        if (!a_event.contactPoint || !a_event.firstCallbackForFullManifold) return;

        auto bodyA = a_event.bodies[0];
        auto bodyB = a_event.bodies[1];
        
        RE::hkpRigidBody* specialBody = nullptr;
        bool isTelekinesis = false;
        bool isThrown = false;
		bool isDropped = false;

        if (bodyA && bodyA->HasProperty(HK_PROPERTY_TELEKINESIS)) {
            specialBody = bodyA;
            isTelekinesis = true;
        } else if (bodyB && bodyB->HasProperty(HK_PROPERTY_TELEKINESIS)) {
            specialBody = bodyB;
            isTelekinesis = true;
        }

        else if (bodyA && bodyA->HasProperty(HK_PROPERTY_GRABTHROWNOBJECT)) {
            specialBody = bodyA;
            isThrown = true;
        } else if (bodyB && bodyB->HasProperty(HK_PROPERTY_GRABTHROWNOBJECT)) {
            specialBody = bodyB;
            isThrown = true;
        }

		else if (bodyA && bodyA->HasProperty(HK_PROPERTY_DROPPEDOBJECT)) {
			specialBody = bodyA;
			isDropped = true;
		} else if (bodyB && bodyB->HasProperty(HK_PROPERTY_DROPPEDOBJECT)) {
			specialBody = bodyB;
			isDropped = true;
		}
        
        if (specialBody) {
            auto refr = specialBody->GetUserData();
            if (!refr) return;
            
            std::uint32_t objectID = refr->GetFormID();
            if (!objectID) return;

            if (processedObjects.find(objectID) != processedObjects.end()) return;

            processedObjects.insert(objectID);
            bodiesToCleanup.push_back(specialBody);

			SKSE::GetTaskInterface()->AddTask([refr, objectID, isTelekinesis, isThrown, isDropped, this]() {
				EventType eventType = EventType::kNone;
				if (isTelekinesis) {
					eventType = EventType::kTelekinesis;
				} else if (isThrown) {
					eventType = EventType::kThrow;
				} else if (isDropped) {
					eventType = EventType::kDrop;
				}

				RuleContext ctx{
					eventType,
					RE::PlayerCharacter::GetSingleton()->As<RE::Actor>(),
					refr,
					refr->GetBaseObject()
				};
				RuleManager::GetSingleton()->Trigger(ctx);

				SKSE::GetTaskInterface()->AddTask([this, objectID]() {
					processedObjects.erase(objectID);
				});
			});

            SKSE::GetTaskInterface()->AddTask([this]() {
                for (auto* body : bodiesToCleanup) {
                    if (body) {
                        if (body->HasProperty(HK_PROPERTY_TELEKINESIS)) {
                            body->RemoveProperty(HK_PROPERTY_TELEKINESIS);
                        }
                        if (body->HasProperty(HK_PROPERTY_GRABTHROWNOBJECT)) {
                            body->RemoveProperty(HK_PROPERTY_GRABTHROWNOBJECT);
                        }
						if (body->HasProperty(HK_PROPERTY_DROPPEDOBJECT)) {
							body->RemoveProperty(HK_PROPERTY_DROPPEDOBJECT);
						}
                        body->RemoveContactListener(this);
                    }
                }
                bodiesToCleanup.clear();
            });
        }
    }

	RE::BSEventNotifyControl CellAttachDetachSink::ProcessEvent(const RE::TESCellAttachDetachEvent* evn, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*)
	{
		if (!evn || !evn->reference) return RE::BSEventNotifyControl::kContinue;

		auto targetRef = evn->reference;
		bool attached = evn->attached;

		if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;

		SKSE::GetTaskInterface()->AddTask([targetRef, attached]() {
			if (attached) {
				RuleContext ctx{
					EventType::kCellAttach,
					RE::PlayerCharacter::GetSingleton()->As<RE::Actor>(),
					targetRef.get(),
					targetRef->GetBaseObject()
				};
				RuleManager::GetSingleton()->Trigger(ctx);
			} else {
				RuleContext ctx{
					EventType::kCellDetach,
					RE::PlayerCharacter::GetSingleton()->As<RE::Actor>(),
					targetRef.get(),
					targetRef->GetBaseObject()
				};
				RuleManager::GetSingleton()->Trigger(ctx);
			}
		});

		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl DestructionStageChangedSink::ProcessEvent(const RE::TESDestructionStageChangedEvent* evn, RE::BSTEventSource<RE::TESDestructionStageChangedEvent>*)
	{
		if (!evn || !evn->target) return RE::BSEventNotifyControl::kContinue;

		auto targetRef = evn->target;
		if (!EventSinkBase::IsItemSafe(targetRef.get())) return RE::BSEventNotifyControl::kContinue;

		std::int32_t stage = evn->newStage;

		SKSE::GetTaskInterface()->AddTask([targetRef, stage]() {
			RuleContext ctx{
				EventType::kDestructionStageChange,
				RE::PlayerCharacter::GetSingleton()->As<RE::Actor>(),
				targetRef.get(),
				nullptr,
				nullptr,
				"",
				"",
				"",
				true,
				nullptr,
				stage
			};
			RuleManager::GetSingleton()->Trigger(ctx);
		});

		return RE::BSEventNotifyControl::kContinue;
	}

	// Still in development, target object's ref cannot be obtained directly or through actor's ExtraDroppedItemList
	/*RE::BSEventNotifyControl DropSink::ProcessEvent(const RE::TESContainerChangedEvent* evn, RE::BSTEventSource<RE::TESContainerChangedEvent>*)
	{
		logger::error("DropSink::ProcessEvent called");
		if (!evn || !evn->baseObj) return RE::BSEventNotifyControl::kContinue;

		RE::TESForm* baseObjForm = RE::TESForm::LookupByID(evn->baseObj);
		if (!baseObjForm) return RE::BSEventNotifyControl::kContinue;
		if (!EventSinkBase::IsRelevantObject(baseObjForm)) return RE::BSEventNotifyControl::kContinue;
		logger::error("DropSink::ProcessEvent called with baseObj: {}", baseObjForm->GetFormID());

		// From container to world
		if (evn->oldContainer == 0 || evn->newContainer != 0) return RE::BSEventNotifyControl::kContinue;
		logger::error("DropSink::ProcessEvent called with oldContainer: {}, newContainer: {}", evn->oldContainer, evn->newContainer);

		auto actor = RE::TESForm::LookupByID<RE::Actor>(evn->oldContainer);
		if (!actor) return RE::BSEventNotifyControl::kContinue;	 // Not dropped by actor
		logger::error("Old container is an actor");

		const auto eventCopy = evn;
		static std::vector<std::future<void>> runningTasks;
		static std::mutex tasksMutex;
		auto future = std::async(std::launch::async, [eventCopy, actorHandle = actor->CreateRefHandle()]() {
			const int checkIntervalMs = 100;
			RE::TESObjectREFR* targetRef = nullptr;

			// Wait for dropped item list to be populated and find the ref (evn->reference is always null)
			while (true) {
				if (auto actorPtr = actorHandle.get()) {
					logger::error("actorhandle exists");
					if (auto dropped = actorPtr->extraList.GetByType<RE::ExtraDroppedItemList>()) {
						logger::error("extralist exists");
						for (auto& handle : dropped->droppedItemList) {
							if (auto refPtr = handle.get()) {
								logger::error("refptr exists");
								if (auto* ref = refPtr.get()) {
									logger::error("ref exists");
									if (ref->GetBaseObject()->GetFormID() == eventCopy->baseObj) {
										targetRef = ref;
										break;
									}
								}
							}
						}
					}
				}
				if (targetRef) break;
				std::this_thread::sleep_for(std::chrono::milliseconds(checkIntervalMs));
			}

			if (!targetRef || targetRef->IsDeleted() || targetRef->IsDisabled()) return;
			logger::error("DropSink::ProcessEvent called with relevant object reference: {}", targetRef->GetFormID());

			auto* baseObj = targetRef->GetBaseObject();
			if (!baseObj) return;

			// Wait for 3D object and physics to be initialized to add the contact listener (so the effect plays upon landing)
			while (true) {
				if (!targetRef || targetRef->IsDeleted()) return;
				std::atomic<bool> taskCompleted{ false };
				std::atomic<bool> success{ false };
				SKSE::GetTaskInterface()->AddTask([targetRef, baseObj, &taskCompleted, &success]() {
					if (targetRef && !targetRef->IsDeleted()) {
						if (auto threedimObj = targetRef->Get3D()) {
							if (auto collisionObj = threedimObj->GetCollisionObject()) {
								if (auto bhkBody = collisionObj->GetRigidBody()) {
									if (auto hkpBody = bhkBody->GetRigidBody()) {
										int propertyId = 271828;
										float now = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - startTime).count();
										hkpBody->SetProperty(propertyId, now);
										hkpBody->AddContactListener(LandingSink::GetSingleton());
										success = true;
									}
								}
							}
						}
					}
					taskCompleted = true;
				});
				while (!taskCompleted) {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				if (success) break;
				std::this_thread::sleep_for(std::chrono::milliseconds(checkIntervalMs));
			}
		});
		{
			std::lock_guard<std::mutex> lock(tasksMutex);
			runningTasks.push_back(std::move(future));
			runningTasks.erase(
				std::remove_if(runningTasks.begin(), runningTasks.end(),
					[](const std::future<void>& f) {
						return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
					}),
				runningTasks.end());
		}
		return RE::BSEventNotifyControl::kContinue;
	}*/


//██╗░░██╗░█████╗░░█████╗░██╗░░██╗░██████╗
//██║░░██║██╔══██╗██╔══██╗██║░██╔╝██╔════╝
//███████║██║░░██║██║░░██║█████═╝░╚█████╗░
//██╔══██║██║░░██║██║░░██║██╔═██╗░░╚═══██╗
//██║░░██║╚█████╔╝╚█████╔╝██║░╚██╗██████╔╝
//╚═╝░░╚═╝░╚════╝░░╚════╝░╚═╝░░╚═╝╚═════╝░                              
    
	void ExplosionHook::thunk(RE::Explosion* a_this)
	{
		func(a_this);
		if (!a_this || a_this->IsDeleted()) return;

		auto& runtimeData = a_this->GetExplosionRuntimeData();
		auto* baseObj = a_this->GetBaseObject();
		auto explosionPos = a_this->GetPosition();
		auto* cell = a_this->GetParentCell();

		if (!baseObj || !cell) return;

		auto* explosionForm = skyrim_cast<RE::BGSExplosion*>(baseObj);
		if (runtimeData.damage <= 0.0f) {
			if (!explosionForm) return;
			if (explosionForm->data.damage <= 0.0f) return;
		}

		float explosionRadius = 0.0f;
		if (explosionForm) {
			explosionRadius = explosionForm->data.radius;
		} else {
			explosionRadius = runtimeData.radius;
		}
		if (explosionRadius <= 0.0f) return;

		RE::Actor* actor = RE::PlayerCharacter::GetSingleton();
		if (auto actorCause = runtimeData.actorCause) {
			if (auto& actorHandle = actorCause->actor) {
				if (auto actorPtr = actorHandle.get()) {
					if (actorPtr)
						actor = actorPtr.get();
				}
			}
		}
		if (!EventSinkBase::IsActorSafe(actor)) {
			actor = RE::PlayerCharacter::GetSingleton();
			if (!EventSinkBase::IsActorSafe(actor)) return;
		}

		RE::TESForm* attackSource = a_this;

		RE::FormID actorFormID = actor ? actor->GetFormID() : 0;
		RE::FormID attackSourceFormID = attackSource ? attackSource->GetFormID() : 0;
		RE::FormID cellFormID = cell ? cell->GetFormID() : 0;

		SKSE::GetTaskInterface()->AddTask([actorFormID, attackSourceFormID, cellFormID, explosionPos, explosionRadius]() 
		{
			RE::Actor* actor = nullptr;
			if (actorFormID != 0) actor = RE::TESForm::LookupByID<RE::Actor>(actorFormID);
			if (!EventSinkBase::IsActorSafe(actor)) return;

			RE::TESForm* attackSource = nullptr;
			if (attackSourceFormID != 0) attackSource = RE::TESForm::LookupByID(attackSourceFormID);

			RE::TESObjectCELL* cell = nullptr;
			if (cellFormID != 0) {
				cell = RE::TESForm::LookupByID<RE::TESObjectCELL>(cellFormID);
			} else {
				if (actor) cell = actor->GetParentCell();
				if (!cell) {
					if (auto* player = RE::PlayerCharacter::GetSingleton()) cell = player->GetParentCell();
				}
				if (!cell) return;
			}

			cell->ForEachReferenceInRange(explosionPos, explosionRadius, [&](RE::TESObjectREFR* ref) -> RE::BSContainer::ForEachResult 
			{
				RuleContext ctx{
					EventType::kHit,
					actor,
					ref,
					attackSource,
					nullptr,
					WeaponTypeToString(WeaponType::Explosion),
					AttackTypeToString(AttackType::Regular),
					DeliveryTypeToString(DeliveryType::None),
					true
				};

				RuleManager::GetSingleton()->Trigger(ctx);

				return RE::BSContainer::ForEachResult::kContinue;
			});
		});
	}

    void ReadyWeaponHook::thunk(RE::ReadyWeaponHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
    {
		func(a_this, a_event, a_data);

		if (a_event) {
			if (a_event->IsUp()) {
				InputHandler::GetSingleton()->SetKeyJustReleased();
			}
		}
    }

    void WeatherChangeHook::thunk(RE::TESRegion* a_region, RE::TESWeather* a_currentWeather)
    {
		func(a_region, a_currentWeather);
        
        if (!a_currentWeather) return;
        if (a_currentWeather == currentWeather) return;
        
        currentWeather = a_currentWeather;
        
        SKSE::GetTaskInterface()->AddTask([a_currentWeather]() {
            auto* player = RE::PlayerCharacter::GetSingleton();
            if (!player) return;

            ScanCell(player, nullptr, true, EventType::kWeatherChange, a_currentWeather);
        });
    }

    void UpdateHook::thunk(RE::PlayerCharacter* a_this, float a_delta)
    {
		func(a_this, a_delta);
        if (!EventSinkBase::IsActorSafe(a_this)) return;

        static auto lastUpdateTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime);

        if (deltaTime.count() < 1000) return;
        lastUpdateTime = currentTime;

        auto* ruleManager = RuleManager::GetSingleton();
        if (!ruleManager) return;

        bool hasAnyUpdateRules = false;
    
        {
            std::shared_lock lock(ruleManager->_ruleMutex);
            const auto& updateRules = ruleManager->GetUpdateRules();
            if (updateRules.empty()) return;
            hasAnyUpdateRules = !updateRules.empty();
        }
    
        if (hasAnyUpdateRules) {
            const auto& updateFilter = ruleManager->GetUpdateFilter();
            ScanCell(a_this, nullptr, true, EventType::kOnUpdate, nullptr, &updateFilter);
        }
    }

	void AttackBlockHook::thunk(RE::AttackBlockHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
	{
		func(a_this, a_event, a_data);
		
		if (a_event) {
			if (!a_event->IsDown()) return;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!EventSinkBase::IsActorSafe(player)) return;;

		if (player->AsActorState()) {
			if (player->AsActorState()->GetWeaponState() != RE::WEAPON_STATE::kDrawn) return;
		}

		auto* playerNode = player->Get3D();
		if (!playerNode) return;

		RE::TESForm* attackSource = nullptr;
		RE::TESForm* projectileSource = nullptr;
		WeaponType weaponType = WeaponType::Other;
		AttackType attackType = AttackType::Regular;
		DeliveryType deliveryType = DeliveryType::None;
		bool isLeftAttack = false;
		
		// meleeRange = fCombatDistance * AttackerScale * WeaponReach + fObjectHit
		// WeaponReach + fObjectHit calculations are not included anymore, the default range suits the task already
		float reach = 141.0f * player->GetScale();

		auto& actorState = player->GetActorRuntimeData().currentProcess;
		if (!actorState) return;

		auto* highData = actorState->high;
		if (!highData) return;

		if (auto& attackData = highData->attackData) {
			if (!attackData) return;

			attackType = GetAttackType(attackData.get());

			if (highData->muzzleFlash && highData->muzzleFlash->baseProjectile) {
				// If the attack is a projectile, let the projectile impact Hooks handle it
				return;
			}

			if (auto* spell = attackData->data.attackSpell) {
				attackSource = spell;
				weaponType = GetSpellType(spell);
				logger::warn("AttackBlockHook: Spell detected, using it as attack source.");

				switch (spell->data.castingType) {
				case RE::MagicSystem::CastingType::kConcentration:
					attackType = AttackType::Continuous;
					break;
				case RE::MagicSystem::CastingType::kFireAndForget:
					attackType = AttackType::FireAndForget;
					break;
				case RE::MagicSystem::CastingType::kConstantEffect:
					attackType = AttackType::Constant;
					break;
				case RE::MagicSystem::CastingType::kScroll:
					weaponType = WeaponType::Scroll;
					break;
				default:
					break;
				}

				switch (spell->data.delivery) {
				case RE::MagicSystem::Delivery::kSelf:
					deliveryType = DeliveryType::Self;
					break;
				case RE::MagicSystem::Delivery::kAimed:
					deliveryType = DeliveryType::Aimed;
					break;
				case RE::MagicSystem::Delivery::kTargetActor:
					deliveryType = DeliveryType::TargetActor;
					break;
				case RE::MagicSystem::Delivery::kTargetLocation:
					deliveryType = DeliveryType::TargetLocation;
					break;
				case RE::MagicSystem::Delivery::kTouch:
					deliveryType = DeliveryType::Touch;
					break;
				case RE::MagicSystem::Delivery::kTotal:
					deliveryType = DeliveryType::Total;
					break;
				default:
					deliveryType = DeliveryType::None;
					break;
				}
			} else {
				isLeftAttack = attackData->IsLeftAttack();
				if (isLeftAttack) {
					attackSource = actorState->GetEquippedLeftHand();
				} else {
					attackSource = actorState->GetEquippedRightHand();
				}
				if (!attackSource) return;

				if (auto* weapon = attackSource->As<RE::TESObjectWEAP>()) {
					weaponType = GetWeaponType(weapon);
				} else if (auto* light = attackSource->As<RE::TESObjectLIGH>()) {
					// Torches are TESObjectLIGH, not TESObjectWEAP, but they're still a
					// valid equipped/bashable item - CanBeCarried() is what actually
					// distinguishes a wieldable torch from a static light fixture.
					if (light->CanBeCarried()) weaponType = WeaponType::Torch;
				}
			}
		}

		// Determine the attack direction and the end point
		auto* cam = RE::PlayerCamera::GetSingleton();
		if (!cam || !cam->currentState || !cam->currentState->camera) return;

		RE::NiPoint3 start = player->GetPosition();
		RE::NiPoint3 dir = playerNode->world.rotate * RE::NiPoint3{ 0.f, 1.f, 0.f };
		dir.z = 0.f;
		if (dir.SqrLength() < 1e-6f) {
			dir = { 0.f, 1.f, 0.f };
		} else {
			dir.Unitize();
		}
		RE::NiPoint3 end = start + dir * reach;
		
		// If the crosshair is over a valid target (harvestable), use its position as the end point
		float foundGoodPosition = false;

		auto crosshair = RE::CrosshairPickData::GetSingleton();
		if (crosshair) {
			if (crosshair->target[0] && crosshair->target[0].get()) {
				if (auto* ref = crosshair->target[0].get().get()) {
					if (EventSinkBase::IsItemSafe(ref)) {
						if (auto* baseObj = ref->GetBaseObject()) {
							switch (baseObj->GetFormType()) {
							case RE::FormType::Flora:
							case RE::FormType::Tree:
								end = ref->GetPosition();
								foundGoodPosition = true;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}

		auto* cell = player->GetParentCell();
		if (!cell) return;

		std::vector<RE::NiPoint3> searchPoints = {
			end,
			player->GetPosition()
		};

		std::vector<RE::TESObjectREFR*> validObjects;

		// hitSomething is a broader signal than validObjects: it's true for ANY relevant
		// object or actor found nearby, not just Flora/Tree. It exists so a real melee hit
		// on an actor (which is handled separately by the native TESHitEvent -> HitSink
		// path) doesn't get misread as "hit nothing" and spuriously treated as a ground hit
		// below - Skyrim's melee weapon collision has no equivalent "AddImpact" callback and
		// never checks against terrain on its own, so this is the only nearby-reference
		// signal we have to distinguish "the swing hit something" from "the swing hit air/
		// ground".
		bool hitSomething = false;

		for (const auto& point : searchPoints) {
			cell->ForEachReferenceInRange(point, 65.0, [&](RE::TESObjectREFR* ref) {
				if (!ref || ref->IsDeleted()) return RE::BSContainer::ForEachResult::kContinue;

				if (auto* actor = ref->As<RE::Actor>()) {
					if (actor != player && EventSinkBase::IsActorSafe(actor)) hitSomething = true;
					return RE::BSContainer::ForEachResult::kContinue;
				}

				if (!EventSinkBase::IsItemSafe(ref)) return RE::BSContainer::ForEachResult::kContinue;
				hitSomething = true;

				auto* baseObj = ref->GetBaseObject();
				if (!baseObj) return RE::BSContainer::ForEachResult::kContinue;

				switch (baseObj->GetFormType()) {
				case RE::FormType::Flora:
				case RE::FormType::Tree:
					break;
				default:
					return RE::BSContainer::ForEachResult::kContinue;
				}

				//bool a_arg2 = false;
				//if (!player->HasLineOfSight(ref, a_arg2)) return RE::BSContainer::ForEachResult::kContinue;

				validObjects.emplace_back(ref);

				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		// Only a real melee weapon swing (weapon or torch bash - not a spell) that found
		// nothing at all nearby is a ground-hit candidate.
		bool isMeleeWeapon = attackSource && (attackSource->As<RE::TESObjectWEAP>() != nullptr || weaponType == WeaponType::Torch);
		bool canBeGroundHit = isMeleeWeapon && !hitSomething;

		if (validObjects.empty() && !canBeGroundHit) return;

		static std::vector<std::future<void>> runningTasks;
		static std::mutex tasksMutex;

		// The old approach here was a single fixed 0.5s sleep followed by one snapshot check
		// of the attack state. That's unreliable across different weapon speeds: a fast
		// weapon (dagger, haste effects, attack-speed perks) can finish its whole swing
		// animation and return to idle well before 0.5s, so the single check below would see
		// "not mid-swing anymore" and wrongly treat a completed swing as a cancelled one,
		// silently dropping both the tree-harvest kHit and the new kHitGround. A slow weapon
        // (greatsword, warhammer) risks the opposite: 0.5s might land before the real hit
		// frame. Poll instead: keep sampling attack state, remember if we ever saw it enter
		// an active swing phase, and stop as soon as it leaves that phase again (a completed
		// swing) or a generous timeout is hit (treated as "did enter a swing" if we already
		// saw one, otherwise as cancelled/never swung).
		auto future = std::async(std::launch::async, [a_event, validObjects, player, attackSource, projectileSource, weaponType, attackType, deliveryType, canBeGroundHit, end]() {
			auto isActiveSwingState = [](RE::ATTACK_STATE_ENUM state) {
				return state == RE::ATTACK_STATE_ENUM::kBowReleasing ||
					state == RE::ATTACK_STATE_ENUM::kBowReleased ||
					state == RE::ATTACK_STATE_ENUM::kBowFollowThrough ||
					state == RE::ATTACK_STATE_ENUM::kBowNextAttack ||
					state == RE::ATTACK_STATE_ENUM::kFiring ||
					state == RE::ATTACK_STATE_ENUM::kFired ||
					state == RE::ATTACK_STATE_ENUM::kBash ||
					state == RE::ATTACK_STATE_ENUM::kFollowThrough ||
					state == RE::ATTACK_STATE_ENUM::kNextAttack ||
					state == RE::ATTACK_STATE_ENUM::kSwing ||
					state == RE::ATTACK_STATE_ENUM::kHit;
			};

			constexpr auto pollInterval = std::chrono::milliseconds(40);
			constexpr auto maxWait = std::chrono::milliseconds(1200);
			// Real weapon swings always take at least a couple of frames to reach an active
			// state - give the animation a brief head start before the first sample so we
			// don't sample at t=0, before the state has even changed from idle.
			std::this_thread::sleep_for(std::chrono::milliseconds(60));

			bool sawSwingState = false;
			auto deadline = std::chrono::steady_clock::now() + maxWait;

			while (std::chrono::steady_clock::now() < deadline) {
				if (!EventSinkBase::IsActorSafe(player)) return;

				auto* actorState = player->AsActorState();
				auto state = actorState ? actorState->GetAttackState() : RE::ATTACK_STATE_ENUM::kNone;

				if (isActiveSwingState(state)) {
					sawSwingState = true;
				} else if (sawSwingState) {
					// Was mid-swing, now isn't - the animation completed normally.
					break;
				}

				std::this_thread::sleep_for(pollInterval);
			}

			if (!sawSwingState) return;	// Never entered a swing state - cancelled/blocked/interrupted before it started.

			SKSE::GetTaskInterface()->AddTask([validObjects, player, attackSource, projectileSource, weaponType, attackType, deliveryType, canBeGroundHit, end]() {
				for (auto& ref : validObjects) {
					if (EventSinkBase::IsItemSafe(ref)) {
						RuleContext ctx{
							EventType::kHit,
							player->As<RE::Actor>(),
							ref,
							attackSource,
							projectileSource,
							WeaponTypeToString(weaponType),
							AttackTypeToString(attackType),
							DeliveryTypeToString(deliveryType),
							true
						};
						RuleManager::GetSingleton()->Trigger(ctx);
					}
				}

				// Melee weapons have no native "hit terrain" signal the way projectiles do
				// (no AddImpact equivalent, no Havok collision check against the ground
				// during a swing) - canBeGroundHit is the closest approximation we have:
				// a real melee weapon swing that found nothing (no tree/object, no other
				// actor) nearby. Re-verify against terrain here, on the main thread, right
				// before dispatching, and correct the hit position to the actual terrain
				// height at that spot - "end" was extrapolated flat from the player's own
				// foot height and may be well off on a slope or staircase.
				if (!canBeGroundHit) return;
				if (!EventSinkBase::IsActorSafe(player)) return;

				auto* cell = player->GetParentCell();

				// Same priority as the projectile path: a melee swing into shallow
				// water sitting on top of terrain should splash, not kick up dust.
				RE::NiPoint3 waterPos = end;
				if (FindWaterHitPosition(waterPos)) {
					RuleContext ctx{
						EventType::kHitWater,
						player->As<RE::Actor>(),
						nullptr,
						attackSource,
						projectileSource,
						WeaponTypeToString(weaponType),
						AttackTypeToString(attackType),
						DeliveryTypeToString(deliveryType),
						true
					};
					ctx.hitPos = waterPos;
					ctx.hasHitPos = true;
					RuleManager::GetSingleton()->Trigger(ctx);
					return;
				}

				RE::NiPoint3 groundPos = end;
				if (!FindGroundZAtPosition(cell, groundPos)) return;

				RuleContext ctx{
					EventType::kHitGround,
					player->As<RE::Actor>(),
					nullptr,
					attackSource,
					projectileSource,
					WeaponTypeToString(weaponType),
					AttackTypeToString(attackType),
					DeliveryTypeToString(deliveryType),
					true
				};
				ctx.hitPos = groundPos;
				ctx.hasHitPos = true;
				RuleManager::GetSingleton()->Trigger(ctx);
			});
		});

		{
			std::lock_guard<std::mutex> lock(tasksMutex);
			runningTasks.push_back(std::move(future));

			runningTasks.erase(
				std::remove_if(runningTasks.begin(), runningTasks.end(),
					[](const std::future<void>& f) {
						return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
					}),
				runningTasks.end());
		}
	}

	// True only for a projectile launched by a CONCENTRATION spell (Flames, Frostbite, Sparks...).
	// The Flame/Cone projectile types are shared with shouts (Fire Breath, Frost Breath, Ice
	// Form, Unrelenting Force...), which are fire-and-forget. Shout projectiles are left
	// completely alone by the Flame/Cone thunks below - hooking them broke shout damage once.
	bool IsConcentrationSpellProjectile(RE::Projectile* a_proj)
	{
		if (!a_proj) return false;
		auto* projSpell = a_proj->GetProjectileRuntimeData().spell;
		auto* spell = projSpell ? projSpell->As<RE::SpellItem>() : nullptr;
		return spell && spell->data.castingType == RE::MagicSystem::CastingType::kConcentration;
	}

	// Queues CheckProjectileWaterCrossing onto the main thread instead of running it on the
	// calling engine thread. The check does a Havok raycast (FindContinuousEffectWaterCrossing)
	// and can trigger rules, neither of which belongs inside Havok's collision callback or on an
	// engine job thread. Only a handle is captured - the projectile may be gone by the time the
	// task runs (a held Beam is recreated every ~0.15s), in which case the check is skipped and
	// the replacement projectile's own hook call takes over. Cheaply pre-throttled with the same
	// interval CheckProjectileWaterCrossing uses so a held effect doesn't queue a task per frame.
	void DeferWaterCheck(RE::Projectile* a_proj, const RE::NiPoint3* a_hitPos)
	{
		if (!a_proj || a_proj->IsDeleted()) return;

		const RE::FormID id = a_proj->GetFormID();
		{
			std::lock_guard lock(g_waterStateMutex);
			if (auto it = g_lastWaterCheckTime.find(id); it != g_lastWaterCheckTime.end()) {
				if (std::chrono::steady_clock::now() - it->second < kLastCheckInterval) return;
			}
		}

		const bool hasPos = a_hitPos != nullptr;
		const RE::NiPoint3 pos = hasPos ? *a_hitPos : RE::NiPoint3{};
		const auto handle = a_proj->CreateRefHandle();

		SKSE::GetTaskInterface()->AddTask([handle, hasPos, pos]() {
			auto ref = handle.get();
			auto* proj = ref ? ref->As<RE::Projectile>() : nullptr;
			if (!proj) return;
			CheckProjectileWaterCrossing(proj, hasPos ? &pos : nullptr);
		});
	}

	// AddImpact runs inside Havok's collision-resolution callback - not safe main-thread
	// game-loop context. Call the original implementation unconditionally first (vanilla
	// damage/decals/cleanup must behave exactly as before), then classify the attack
	// synchronously via QueueProjectileImpact (a_proj is still guaranteed valid right
	// here) and defer only the actual scan + RuleManager::Trigger work - the part with a
	// real reentrancy concern - to the main-thread task queue. See the comment on
	// QueueProjectileImpact for why the attack classification itself can no longer be
	// postponed: doing so used to silently drop the impact for any projectile (e.g.
	// Firebolt, Ice Spike) that gets deleted before the deferred task runs.
	void MissileImpactHook::thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
							  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable,
							  std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		func(a_proj, a_ref, a_hitPos, a_velocity, a_collidable, a_arg6, a_arg7);
		if (!a_proj) return;
		RE::FormID refID = a_ref ? a_ref->GetFormID() : 0;
		QueueProjectileImpact(a_proj, a_hitPos, refID);
	}

	// AddImpact fires continuously for a held continuous effect for as long as it's cast -
	// that's the whole reason the isContinuousEffect exception exists in
	// CheckProjectileWaterCrossing in the first place. Update(), on the other hand, appears
	// to only keep ticking on a Beam projectile for a brief initial window before the object
	// goes quiet for the rest of a held cast (confirmed from an SKSE log: BeamUpdateHook-
	// driven polling produced a clean burst of activity in the first ~100ms of a 5+ second
	// hold, then nothing at all afterward, despite the beam visibly still firing) - the
	// ongoing visual/damage for the rest of the hold is evidently driven by something other
	// than repeated Update() calls on this same Projectile object. Also calling the water
	// check from here means it keeps getting polled for the entire duration of the cast,
	// not just Update()'s brief active window. This is safe to add unconditionally: the
	// function already self-throttles per instance (kLastCheckInterval) and per caster
	// (kContinuousRetriggerInterval), so calling it from both hooks can't double-fire.
	// Continuous-effect (Beam/Flame/Cone) AddImpact thunks. These fire every frame of a held
	// effect from inside Havok's collision callback - the same code path that applies the
	// damage/effects - so they must stay as light as possible. When the earlier version of
	// these thunks did the water check synchronously (a Havok raycast, from inside that
	// callback) AND ran QueueProjectileImpact, breath/shout attacks (Cone) stopped damaging
	// enemies. They now do exactly one thing after the original: hand the water check to the
	// main thread via DeferWaterCheck, and also call QueueProjectileImpact (needed for HitGround).
	// Flame/Cone do this only for concentration spells (Flames, Frostbite) - never for shouts.
	void BeamImpactHook::thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						   const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable,
						   std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		func(a_proj, a_ref, a_hitPos, a_velocity, a_collidable, a_arg6, a_arg7);
		if (!a_proj) return;
		DeferWaterCheck(a_proj, &a_hitPos);
		// Beam (Sparks, Lightning Bolt, other electric spells) also needs the impact
		// classification back so HitGround/Hit rules fire from it. This is the same call the
		// Missile/Arrow thunks make; it only classifies synchronously and defers the real
		// work to the main thread. Only the Beam thunk does this - Flame/Cone stay minimal.
		RE::FormID refID = a_ref ? a_ref->GetFormID() : 0;
		QueueProjectileImpact(a_proj, a_hitPos, refID);
	}

	void FlameImpactHook::thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
							const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable,
							std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		func(a_proj, a_ref, a_hitPos, a_velocity, a_collidable, a_arg6, a_arg7);
		// One-time log so it's visible in the SKSE log which spells really use FlameProjectile.
		static std::atomic<bool> s_loggedFlame{ false };
		if (a_proj && !s_loggedFlame.exchange(true)) {
			logger::info("FlameProjectile AddImpact hook fired for projectile {:08X} (concentration spell: {})",
				a_proj->GetFormID(), IsConcentrationSpellProjectile(a_proj));
		}
		// Only concentration spells (Flames, Frostbite). Shouts share this projectile type and
		// must not be touched at all.
		if (!IsConcentrationSpellProjectile(a_proj)) return;
		DeferWaterCheck(a_proj, &a_hitPos);
		RE::FormID refID = a_ref ? a_ref->GetFormID() : 0;
		QueueProjectileImpact(a_proj, a_hitPos, refID);
	}

	void ConeImpactHook::thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						   const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable,
						   std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		func(a_proj, a_ref, a_hitPos, a_velocity, a_collidable, a_arg6, a_arg7);
		// Only concentration spells (Flames, Frostbite). Shouts share this projectile type and
		// must not be touched at all.
		if (!IsConcentrationSpellProjectile(a_proj)) return;
		DeferWaterCheck(a_proj, &a_hitPos);
		RE::FormID refID = a_ref ? a_ref->GetFormID() : 0;
		QueueProjectileImpact(a_proj, a_hitPos, refID);
	}

	void ArrowImpactHook::thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
							const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable,
							std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		func(a_proj, a_ref, a_hitPos, a_velocity, a_collidable, a_arg6, a_arg7);
		if (!a_proj) return;
		RE::FormID refID = a_ref ? a_ref->GetFormID() : 0;
		QueueProjectileImpact(a_proj, a_hitPos, refID);
	}

	// Runs on every Update tick for every in-flight projectile of the corresponding type -
	// see CheckProjectileWaterCrossing (above ProcessProjectileImpact) for what it checks.
	// Calling func() first preserves vanilla flight/physics behavior exactly as before,
	// matching the same convention the AddImpact hooks above already use.
	void MissileUpdateHook::thunk(RE::Projectile* a_this, float a_delta)
	{
		func(a_this, a_delta);
		CheckProjectileWaterCrossing(a_this);
	}

	void BeamUpdateHook::thunk(RE::Projectile* a_this, float a_delta)
	{
		func(a_this, a_delta);
		DeferWaterCheck(a_this, nullptr);
	}

	void FlameUpdateHook::thunk(RE::Projectile* a_this, float a_delta)
	{
		func(a_this, a_delta);
		DeferWaterCheck(a_this, nullptr);
	}

	void ConeUpdateHook::thunk(RE::Projectile* a_this, float a_delta)
	{
		func(a_this, a_delta);
		DeferWaterCheck(a_this, nullptr);
	}

	// SetDelete hooks - see ForgetProjectileWaterState and the comment on MissileDeleteHook in
	// EventSinks.h. Not installed for ArrowProjectile, matching the Update hooks above.
	void MissileDeleteHook::thunk(RE::Projectile* a_this, bool a_set)
	{
		func(a_this, a_set);
		ForgetProjectileWaterState(a_this);
	}

	void BeamDeleteHook::thunk(RE::Projectile* a_this, bool a_set)
	{
		func(a_this, a_set);
		ForgetProjectileWaterState(a_this);
	}

	void FlameDeleteHook::thunk(RE::Projectile* a_this, bool a_set)
	{
		func(a_this, a_set);
		ForgetProjectileWaterState(a_this);
	}

	void ConeDeleteHook::thunk(RE::Projectile* a_this, bool a_set)
	{
		func(a_this, a_set);
		ForgetProjectileWaterState(a_this);
	}


//██████╗░███████╗░██████╗░██╗░██████╗████████╗██████╗░░█████╗░████████╗██╗░█████╗░███╗░░██╗
//██╔══██╗██╔════╝██╔════╝░██║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
//██████╔╝█████╗░░██║░░██╗░██║╚█████╗░░░░██║░░░██████╔╝███████║░░░██║░░░██║██║░░██║██╔██╗██║
//██╔══██╗██╔══╝░░██║░░╚██╗██║░╚═══██╗░░░██║░░░██╔══██╗██╔══██║░░░██║░░░██║██║░░██║██║╚████║
//██║░░██║███████╗╚██████╔╝██║██████╔╝░░░██║░░░██║░░██║██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
//╚═╝░░╚═╝╚══════╝░╚═════╝░╚═╝╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝                                  

    void RegisterSinks()
    {
        auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        if (!holder) {
            logger::error("RegisterSinks: RE::ScriptEventSourceHolder::GetSingleton() returned null - no event sinks were registered");
            return;
        }

        if (auto* src = holder->GetEventSource<RE::TESActivateEvent>()) {
            src->AddEventSink(ActivateSink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESActivateEvent source unavailable");
        }
        if (auto* src = holder->GetEventSource<RE::TESHitEvent>()) {
            src->AddEventSink(HitSink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESHitEvent source unavailable");
        }
        if (auto* src = holder->GetEventSource<RE::TESGrabReleaseEvent>()) {
            src->AddEventSink(GrabReleaseSink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESGrabReleaseEvent source unavailable");
        }
        if (auto* src = holder->GetEventSource<RE::TESCellAttachDetachEvent>()) {
            src->AddEventSink(CellAttachDetachSink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESCellAttachDetachEvent source unavailable");
        }
        if (auto* src = holder->GetEventSource<RE::TESMagicEffectApplyEvent>()) {
            src->AddEventSink(MagicEffectApplySink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESMagicEffectApplyEvent source unavailable");
        }
        if (auto* src = holder->GetEventSource<RE::TESDestructionStageChangedEvent>()) {
            src->AddEventSink(DestructionStageChangedSink::GetSingleton());
        } else {
            logger::error("RegisterSinks: TESDestructionStageChangedEvent source unavailable");
        }
		
		// Currently disabled, still in development
		//holder->GetEventSource<RE::TESContainerChangedEvent>()->AddEventSink(DropSink::GetSingleton());
    }

    void InstallHooks() 
    {
        // Note: these do the same thing as the ::stl::write_thunk_call / ::stl::write_vfunc
        // helpers declared in PCH.h, but are inlined directly here so hook installation
        // doesn't depend on that project-level 'stl' namespace being visible - some build
        // setups (e.g. clibdt) were not resolving it at this call site.
        REL::Relocation<std::uintptr_t> weatherChangeHook{ REL::VariantID(25684, 26231, 25684), REL::VariantOffset(0x44F, 0x46C, 0x44F) };
        {
            SKSE::AllocTrampoline(14);
            auto& trampoline = SKSE::GetTrampoline();
            WeatherChangeHook::func = trampoline.write_call<5>(weatherChangeHook.address(), WeatherChangeHook::thunk);
        }
        {
            REL::Relocation<std::uintptr_t> vtbl{ RE::ReadyWeaponHandler::VTABLE[0] };
            ReadyWeaponHook::func = vtbl.write_vfunc(ReadyWeaponHook::size, ReadyWeaponHook::thunk);
        }
        {
            REL::Relocation<std::uintptr_t> vtbl{ RE::Explosion::VTABLE[0] };
            ExplosionHook::func = vtbl.write_vfunc(ExplosionHook::size, ExplosionHook::thunk);
        }
        {
            REL::Relocation<std::uintptr_t> vtbl{ RE::PlayerCharacter::VTABLE[0] };
            // 0xAD for SE/AE, 0xAF for VR - see the comment on UpdateHook in EventSinks.h.
            UpdateHook::func = vtbl.write_vfunc(REL::Relocate(0xAD, 0xAD, 0xAF), UpdateHook::thunk);
        }
        {
            REL::Relocation<std::uintptr_t> vtbl{ RE::AttackBlockHandler::VTABLE[0] };
            AttackBlockHook::func = vtbl.write_vfunc(AttackBlockHook::size, AttackBlockHook::thunk);
        }

		// AddImpact hooks (kHitGround support). Previously disabled due to instability -
		// see the comment above MissileImpactHook etc. in EventSinks.h for why, and note
		// these use the same inline vtbl.write_vfunc(...) pattern as the hooks above
		// rather than ::stl::write_vfunc<T, Hook>(), for the same build-portability reason
		// explained in the top-of-function note. 0xBD for SE/AE, 0xBE for VR - see the
		// comment above MissileImpactHook in EventSinks.h.
		{
			REL::Relocation<std::uintptr_t> vtbl{ RE::MissileProjectile::VTABLE[0] };
			MissileImpactHook::func = vtbl.write_vfunc(REL::Relocate(0xBD, 0xBD, 0xBE), MissileImpactHook::thunk);
		}
		// Continuous projectile types.
		//  Beam  - Sparks / Lightning Bolt.                      ON.
		//  Flame - Flames / Frostbite (concentration spells).    ON.
		//  Cone  - the SHOUT projectile type (Fire Breath, Frost Breath, Ice Form, Unrelenting
		//          Force...).                                    OFF - do not enable.
		// Hooking Cone's AddImpact slot broke shouts twice: first no damage/effects at all, and
		// then (with the lighter concentration-only thunk) a crash inside the engine's own
		// ConeProjectile update when a shout hit a fish. Beam/Flame/Missile/Arrow hooks are fine.
		// Each type is an individual switch.
		constexpr bool kHookBeamWater = true;
		constexpr bool kHookFlameWater = true;
		constexpr bool kHookConeWater = false;

		if constexpr (kHookBeamWater) {
			REL::Relocation<std::uintptr_t> vtbl{ RE::BeamProjectile::VTABLE[0] };
			BeamImpactHook::func = vtbl.write_vfunc(REL::Relocate(0xBD, 0xBD, 0xBE), BeamImpactHook::thunk);
		}
		if constexpr (kHookFlameWater) {
			REL::Relocation<std::uintptr_t> vtbl{ RE::FlameProjectile::VTABLE[0] };
			FlameImpactHook::func = vtbl.write_vfunc(REL::Relocate(0xBD, 0xBD, 0xBE), FlameImpactHook::thunk);
		}
		if constexpr (kHookConeWater) {
			REL::Relocation<std::uintptr_t> vtbl{ RE::ConeProjectile::VTABLE[0] };
			ConeImpactHook::func = vtbl.write_vfunc(REL::Relocate(0xBD, 0xBD, 0xBE), ConeImpactHook::thunk);
		}
		{
			REL::Relocation<std::uintptr_t> vtbl{ RE::ArrowProjectile::VTABLE[0] };
			ArrowImpactHook::func = vtbl.write_vfunc(REL::Relocate(0xBD, 0xBD, 0xBE), ArrowImpactHook::thunk);
		}

		// Water-crossing polling hooks (kHitWater support) - see CheckProjectileWaterCrossing
		// and the comment above MissileUpdateHook in EventSinks.h for why AddImpact alone
		// can't reliably catch a projectile hitting open water. Same vtbl.write_vfunc(...)
		// pattern and per-subclass installation as the AddImpact hooks directly above.
		// 0xAB for SE/AE, 0xAC for VR - see the comment above MissileUpdateHook in
		// EventSinks.h. Not installed for ArrowProjectile - see the same comment for why.
		{
			REL::Relocation<std::uintptr_t> vtbl{ RE::MissileProjectile::VTABLE[0] };
			MissileUpdateHook::func = vtbl.write_vfunc(REL::Relocate(0xAB, 0xAB, 0xAC), MissileUpdateHook::thunk);
		}
		// Beam Update hook: on with Beam's AddImpact hook (Update only ticks briefly for a
		// held Beam, but that brief window is what catches a beam that starts over open water).
		if constexpr (kHookBeamWater) {
			REL::Relocation<std::uintptr_t> vtbl{ RE::BeamProjectile::VTABLE[0] };
			BeamUpdateHook::func = vtbl.write_vfunc(REL::Relocate(0xAB, 0xAB, 0xAC), BeamUpdateHook::thunk);
		}
		// DELIBERATELY NOT INSTALLED (breath / cone / flame attacks - Fire Breath, Frost Breath,
		// Ice Form, Flames, Frostbite - stopped damaging enemies after the update that added
		// these): the Update hooks for FlameProjectile/ConeProjectile and the SetDelete hooks
		// (0x23) for all four projectile types. They only existed to poll for water crossings
		// and to clean up the water-tracking maps:
		//  * Flame/Cone already poll for water from their AddImpact hooks (which fire
		//    continuously while the effect is held - see BeamImpactHook's comment), so the extra
		//    Update poll added little;
		//  * the maps are now size-capped/pruned inside CheckProjectileWaterCrossing, so the
		//    SetDelete cleanup is no longer needed.
		// The structs/thunks are kept so these can be re-enabled one at a time to bisect.
    }
}
