#pragma once
#include "RuleManager.h"
#include "RE/T/TESDestructionStageChangedEvent.h"

namespace OIF
{
	
//██╗░░██╗███████╗██╗░░░░░██████╗░███████╗██████╗░░██████╗
//██║░░██║██╔════╝██║░░░░░██╔══██╗██╔════╝██╔══██╗██╔════╝
//███████║█████╗░░██║░░░░░██████╔╝█████╗░░██████╔╝╚█████╗░
//██╔══██║██╔══╝░░██║░░░░░██╔═══╝░██╔══╝░░██╔══██╗░╚═══██╗
//██║░░██║███████╗███████╗██║░░░░░███████╗██║░░██║██████╔╝
//╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═════╝░

    class EventSinkBase
    {
    public:
        static const std::unordered_set<RE::FormType> relevantFormTypes;
        
        static bool IsRelevantObject(RE::TESForm* form) {
            if (!form) return false;
            return relevantFormTypes.contains(form->GetFormType());
        }
        
        static bool IsRelevantObjectRef(RE::TESObjectREFR* ref) {
            if (!ref || ref->IsDeleted()) return false;
            auto* baseObj = ref->GetBaseObject();
			if (!baseObj) return false;
            return IsRelevantObject(baseObj);
        }

		static bool IsActorSafe(RE::Actor* actor) {
			if (!actor || actor->IsDeleted() || actor->IsDead() || !actor->GetBaseObject()) return false;
			try {
				auto* baseObj = actor->GetBaseObject();
				if (!baseObj) return false;
  
				auto* cell = actor->GetParentCell();
				if (!cell) return false;
    
				return true;
			} catch (...) {
				return false;
			}
		}

		static bool IsItemSafe(RE::TESObjectREFR* item)
		{
			if (!item || item->IsDeleted() || !item->GetBaseObject()) return false;
			if (!EventSinkBase::IsRelevantObjectRef(item)) return false;
			try {
				auto* baseObj = item->GetBaseObject();
				if (!baseObj) return false;

				auto* cell = item->GetParentCell();
				if (!cell) return false;

				return true;
			} catch (...) {
				return false;
			}
		}
    };

	class InputHandler
	{
	public:
		static InputHandler* GetSingleton()
		{
			static InputHandler handler;
			return &handler;
		}

		bool WasKeyJustReleased() const { return KeyJustReleased; }
		void SetKeyJustReleased() { KeyJustReleased = true; }
		void ResetKeyState() { KeyJustReleased = false; }

	private:
		bool KeyJustReleased = false;
	};


//░██████╗██╗███╗░░██╗██╗░░██╗░██████╗
//██╔════╝██║████╗░██║██║░██╔╝██╔════╝
//╚█████╗░██║██╔██╗██║█████═╝░╚█████╗░
//░╚═══██╗██║██║╚████║██╔═██╗░░╚═══██╗
//██████╔╝██║██║░╚███║██║░╚██╗██████╔╝
//╚═════╝░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝╚═════╝░                              

    class ActivateSink : public RE::BSTEventSink<RE::TESActivateEvent>
    {
    public:
        static ActivateSink* GetSingleton()
        {
            static ActivateSink sink;
            return &sink;
        }

        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESActivateEvent* evn,
            RE::BSTEventSource<RE::TESActivateEvent>*) override;
    };

    class HitSink : public RE::BSTEventSink<RE::TESHitEvent>
    {
    public:
        static HitSink* GetSingleton()
        {
            static HitSink sink;
            return &sink;
        }

        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESHitEvent* evn,
            RE::BSTEventSource<RE::TESHitEvent>*) override;
    };

    class MagicEffectApplySink : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
    {
    public:
        static MagicEffectApplySink* GetSingleton()
        {
            static MagicEffectApplySink sink;
            return &sink;
        }
        
        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESMagicEffectApplyEvent* evn,
            RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) override;
    };     

    class GrabReleaseSink : public RE::BSTEventSink<RE::TESGrabReleaseEvent>
    {
    public:
        static GrabReleaseSink* GetSingleton()
        {
            static GrabReleaseSink sink;
            return &sink;
        }

        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESGrabReleaseEvent* evn,
            RE::BSTEventSource<RE::TESGrabReleaseEvent>*) override;
    };

    class LandingSink : public RE::hkpContactListener
    {
    public:
        static LandingSink* GetSingleton()
        {
            static LandingSink sink;
            return &sink;
        }

        void ContactPointCallback(const RE::hkpContactPointEvent& evn) override;

    private:
        std::unordered_set<std::uint32_t> processedObjects;
        std::vector<RE::hkpRigidBody*> bodiesToCleanup;
        static constexpr std::uint32_t HK_PROPERTY_TELEKINESIS{ 314159 };
        static constexpr std::uint32_t HK_PROPERTY_GRABTHROWNOBJECT{ 628318 };
		static constexpr std::uint32_t HK_PROPERTY_DROPPEDOBJECT{ 271828 };
    };

	class CellAttachDetachSink : public RE::BSTEventSink<RE::TESCellAttachDetachEvent>
	{
	public:
		static CellAttachDetachSink* GetSingleton()
		{
			static CellAttachDetachSink sink;
			return &sink;
		}

		RE::BSEventNotifyControl ProcessEvent(
			const RE::TESCellAttachDetachEvent* evn,
			RE::BSTEventSource<RE::TESCellAttachDetachEvent>*) override;
	};

	class DestructionStageChangedSink : public RE::BSTEventSink<RE::TESDestructionStageChangedEvent>
	{
	public:
		static DestructionStageChangedSink* GetSingleton()
		{
			static DestructionStageChangedSink sink;
			return &sink;
		}

		RE::BSEventNotifyControl ProcessEvent(
			const RE::TESDestructionStageChangedEvent* evn,
			RE::BSTEventSource<RE::TESDestructionStageChangedEvent>*) override;
	};

	/*class DropSink : public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		static DropSink* GetSingleton()
		{
			static DropSink sink;
			return &sink;
		}

		RE::BSEventNotifyControl ProcessEvent(
			const RE::TESContainerChangedEvent* evn,
			RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
	};*/

	
//██╗░░██╗░█████╗░░█████╗░██╗░░██╗░██████╗
//██║░░██║██╔══██╗██╔══██╗██║░██╔╝██╔════╝
//███████║██║░░██║██║░░██║█████═╝░╚█████╗░
//██╔══██║██║░░██║██║░░██║██╔═██╗░░╚═══██╗
//██║░░██║╚█████╔╝╚█████╔╝██║░╚██╗██████╔╝
//╚═╝░░╚═╝░╚════╝░░╚════╝░╚═╝░░╚═╝╚═════╝░                               

	struct ExplosionHook
	{
		static void thunk(RE::Explosion* a_this);
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size = 0xA2;
	};

	// Taken and adapted from Rain Extinguishes Fires source code
	struct WeatherChangeHook
	{
		static void thunk(RE::TESRegion* a_region, RE::TESWeather* a_currentWeather);
		static inline REL::Relocation<decltype(thunk)> func;
		static inline RE::TESWeather* currentWeather{ nullptr };
	};  

    // Taken and adapted from Grab and Throw source code
    struct ReadyWeaponHook {
		static void thunk(RE::ReadyWeaponHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
        static inline REL::Relocation<decltype(thunk)> func;
        static inline constexpr std::size_t size = 0x4;
    };

    struct UpdateHook
    {
		static void thunk(RE::PlayerCharacter* a_this, float a_delta);
        static inline REL::Relocation<decltype(thunk)> func;
        // SE/AE value only - VR uses 0xAF. Installed via REL::Relocate(0xAD, 0xAD, 0xAF) at
        // the call site in InstallHooks(), not from this field - see the comment above
        // MissileImpactHook below for why a flat constant here would be wrong for VR.
        static constexpr std::size_t size = 0xAD;
    };

	struct AttackBlockHook
	{
		static void thunk(RE::AttackBlockHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size = 0x4;
	};

	// Credits to RavenKZP for the original signature/vtable-index research on these hooks.
	// AddImpact (CommonLibSSE-NG: RE::Projectile::AddImpact) runs inside Havok's
	// collision-resolution callback, not main-thread game-loop context. The thunks in
	// EventSinks.cpp therefore call the original implementation first (so vanilla
	// damage/decals/cleanup behave exactly as before), then hand off to
	// QueueProjectileImpact, which classifies the attack synchronously (a_proj is still
	// guaranteed valid at that point) and defers only the actual cell scan +
	// RuleManager::Trigger to the main-thread task queue via ProcessProjectileImpact. Doing
	// that heavier work synchronously from here, instead of deferring it, is what caused
	// these hooks to crash previously - but the attack classification itself must stay
	// synchronous, since some projectiles (e.g. Firebolt, Ice Spike) are deleted before a
	// deferred task would get to run.
	//
	// Vtable index: 0xBD for SE/AE - but 0xBE for VR, per RavenKZP's own Frostwalker
	// (https://github.com/RavenKZP/Frostwalker), which installs this exact slot with
	// REL::Relocate(0xBD, 0xBD, 0xBE). The `size` fields below are therefore SE/AE-only
	// documentation; InstallHooks() computes the real, version-correct value via
	// REL::Relocate at each call site rather than reading these fields, since this project
	// ships a VR preset (see CMakePresets.json) where a flat 0xBD would install onto the
	// wrong vtable slot entirely.
	struct MissileImpactHook
	{
		static void thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable, 
						  std::int32_t a_arg6, std::uint32_t a_arg7);		
		static inline REL::Relocation<decltype(thunk)> func;
		static constexpr std::size_t size = 0xBD; // SE/AE only - see comment above
	};

	struct BeamImpactHook
	{
		static void thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable, 
						  std::int32_t a_arg6, std::uint32_t a_arg7);		
		static inline REL::Relocation<decltype(thunk)> func;
		static constexpr std::size_t size = 0xBD; // SE/AE only - see comment above
	};

	struct FlameImpactHook
	{
		static void thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable, 
						  std::int32_t a_arg6, std::uint32_t a_arg7);		
		static inline REL::Relocation<decltype(thunk)> func;
		static constexpr std::size_t size = 0xBD; // SE/AE only - see comment above
	};

	struct ConeImpactHook
	{
		static void thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable, 
						  std::int32_t a_arg6, std::uint32_t a_arg7);
		static inline REL::Relocation<decltype(thunk)> func;
	 	static constexpr std::size_t size = 0xBD; // SE/AE only - see comment above
	};

	struct ArrowImpactHook
	{
		static void thunk(RE::Projectile* a_proj, RE::TESObjectREFR* a_ref, const RE::NiPoint3& a_hitPos,
						  const RE::NiPoint3& a_velocity, RE::hkpCollidable* a_collidable, 
						  std::int32_t a_arg6, std::uint32_t a_arg7);		
		static inline REL::Relocation<decltype(thunk)> func;
		static constexpr std::size_t size = 0xBD; // SE/AE only - see comment above
	};

	// Vtable index 0xAB (SE/AE) / 0xAC (VR) for RE::Projectile::Update(float). The SE/AE
	// value is confirmed by the publicly available "Universal Arrow Spin" SKSE plugin source,
	// which hooks this slot on RE::ArrowProjectile::VTABLE[0]; the VR value (and the fact
	// that it differs from SE/AE at all) is confirmed by RavenKZP's Frostwalker
	// (https://github.com/RavenKZP/Frostwalker), which hooks this same slot with
	// REL::Relocate(0xAB, 0xAB, 0xAC) - installed via REL::Relocate at the call site in
	// InstallHooks() below, not as a fixed size here, precisely because it does differ by
	// version. Frostwalker also uses this same per-frame Update polling approach to detect
	// frost-spell projectiles touching water - see CheckProjectileWaterCrossing in
	// EventSinks.cpp for what runs on every tick.
	//
	// Deliberately NOT installed for RE::ArrowProjectile: Frostwalker's own source defines
	// (but never installs) an ArrowProjectile::Update hook, leaving the install commented out
	// while every other subclass's is active - a strong signal from a shipped, VR-supporting
	// plugin that hooking this specific slot on Arrow's vtable is unsafe or unreliable, even
	// though a different public source (Universal Arrow Spin) does hook it successfully for
	// arrow-only, presumably SE/AE-only use. Arrows/bolts still get water detection through
	// FindWaterHitPosition in ProcessProjectileImpact/AttackBlockHook (the AddImpact path) -
	// they just don't get the "AddImpact never fires at all" coverage the other subclasses
	// get from this polling hook.
	struct MissileUpdateHook
	{
		static void thunk(RE::Projectile* a_this, float a_delta);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BeamUpdateHook
	{
		static void thunk(RE::Projectile* a_this, float a_delta);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct FlameUpdateHook
	{
		static void thunk(RE::Projectile* a_this, float a_delta);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct ConeUpdateHook
	{
		static void thunk(RE::Projectile* a_this, float a_delta);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	// Vtable index 0x23 (SetDelete) confirmed stable across SE/AE/VR by Frostwalker (unlike
	// Update/AddImpact, it does not need REL::Relocate's per-version variants). Cleans up
	// g_waterHitReported/g_lastWaterCheckTime the instant a projectile is deleted, rather
	// than waiting for kProcessedImpacts (which a deleted-without-impacting projectile may
	// never set) - same scope as the Update hooks above: Missile/Beam/Flame/Cone only, not
	// Arrow, per Frostwalker's own precedent.
	struct MissileDeleteHook
	{
		static void thunk(RE::Projectile* a_this, bool a_set);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BeamDeleteHook
	{
		static void thunk(RE::Projectile* a_this, bool a_set);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct FlameDeleteHook
	{
		static void thunk(RE::Projectile* a_this, bool a_set);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct ConeDeleteHook
	{
		static void thunk(RE::Projectile* a_this, bool a_set);
		static inline REL::Relocation<decltype(thunk)> func;
	};


//██████╗░███████╗░██████╗░██╗░██████╗████████╗██████╗░░█████╗░████████╗██╗░█████╗░███╗░░██╗
//██╔══██╗██╔════╝██╔════╝░██║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
//██████╔╝█████╗░░██║░░██╗░██║╚█████╗░░░░██║░░░██████╔╝███████║░░░██║░░░██║██║░░██║██╔██╗██║
//██╔══██╗██╔══╝░░██║░░╚██╗██║░╚═══██╗░░░██║░░░██╔══██╗██╔══██║░░░██║░░░██║██║░░██║██║╚████║
//██║░░██║███████╗╚██████╔╝██║██████╔╝░░░██║░░░██║░░██║██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
//╚═╝░░╚═╝╚══════╝░╚═════╝░╚═╝╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝   

    void RegisterSinks();
    void InstallHooks();
}
