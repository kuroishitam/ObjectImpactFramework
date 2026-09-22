#include "Effects.h"
#include <future>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <random>
#include <charconv>
#include <cmath>
#include "RuleManager.h"
#include "RE/E/ExtraOwnership.h"

namespace OIF::Effects
{

//██╗░░██╗███████╗██╗░░░░░██████╗░███████╗██████╗░░██████╗
//██║░░██║██╔════╝██║░░░░░██╔══██╗██╔════╝██╔══██╗██╔════╝
//███████║█████╗░░██║░░░░░██████╔╝█████╗░░██████╔╝╚█████╗░
//██╔══██║██╔══╝░░██║░░░░░██╔═══╝░██╔══╝░░██╔══██╗░╚═══██╗
//██║░░██║███████╗███████╗██║░░░░░███████╗██║░░██║██████╔╝
//╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═════╝░
	
// ╔════════════════════════════════════╗
// ║           NODES HANDLING           ║
// ╚════════════════════════════════════╝

	/*std::unordered_map<std::string, RE::BSShaderProperty::EShaderPropertyFlag> g_shaderFlagMap = {
		// Shader Flags 1
        {"specular", RE::BSShaderProperty::EShaderPropertyFlag::kSpecular},
        {"skinned", RE::BSShaderProperty::EShaderPropertyFlag::kSkinned},
        {"temp_refraction", RE::BSShaderProperty::EShaderPropertyFlag::kTempRefraction},
        {"vertex_alpha", RE::BSShaderProperty::EShaderPropertyFlag::kVertexAlpha},
        {"grayscale_to_palette_color", RE::BSShaderProperty::EShaderPropertyFlag::kGrayscaleToPaletteColor},
        {"grayscale_to_palette_alpha", RE::BSShaderProperty::EShaderPropertyFlag::kGrayscaleToPaletteAlpha},
        {"falloff", RE::BSShaderProperty::EShaderPropertyFlag::kFalloff},
        {"env_map", RE::BSShaderProperty::EShaderPropertyFlag::kEnvMap},
        {"receive_shadows", RE::BSShaderProperty::EShaderPropertyFlag::kReceiveShadows},
        {"cast_shadows", RE::BSShaderProperty::EShaderPropertyFlag::kCastShadows},
        {"face", RE::BSShaderProperty::EShaderPropertyFlag::kFace},
        {"parallax", RE::BSShaderProperty::EShaderPropertyFlag::kParallax},
        {"model_space_normals", RE::BSShaderProperty::EShaderPropertyFlag::kModelSpaceNormals},
        {"non_projective_shadows", RE::BSShaderProperty::EShaderPropertyFlag::kNonProjectiveShadows},
        {"multi_texture_landscape", RE::BSShaderProperty::EShaderPropertyFlag::kMultiTextureLandscape},
        {"refraction", RE::BSShaderProperty::EShaderPropertyFlag::kRefraction},
        {"refraction_falloff", RE::BSShaderProperty::EShaderPropertyFlag::kRefractionFalloff},
        {"eye_reflect", RE::BSShaderProperty::EShaderPropertyFlag::kEyeReflect},
        {"hair_tint", RE::BSShaderProperty::EShaderPropertyFlag::kHairTint},
        {"screendoor_alpha_fade", RE::BSShaderProperty::EShaderPropertyFlag::kScreendoorAlphaFade},
        {"local_map_clear", RE::BSShaderProperty::EShaderPropertyFlag::kLocalMapClear},
        {"face_gen_rgb_tint", RE::BSShaderProperty::EShaderPropertyFlag::kFaceGenRGBTint},
        {"own_emit", RE::BSShaderProperty::EShaderPropertyFlag::kOwnEmit},
        {"projected_uv", RE::BSShaderProperty::EShaderPropertyFlag::kProjectedUV},
        {"multiple_textures", RE::BSShaderProperty::EShaderPropertyFlag::kMultipleTextures},
        {"remappable_textures", RE::BSShaderProperty::EShaderPropertyFlag::kRemappableTextures},
        {"decal", RE::BSShaderProperty::EShaderPropertyFlag::kDecal},
        {"dynamic_decal", RE::BSShaderProperty::EShaderPropertyFlag::kDynamicDecal},
        {"parallax_occlusion", RE::BSShaderProperty::EShaderPropertyFlag::kParallaxOcclusion},
        {"external_emittance", RE::BSShaderProperty::EShaderPropertyFlag::kExternalEmittance},
        {"soft_effect", RE::BSShaderProperty::EShaderPropertyFlag::kSoftEffect},
        {"z_buffer_test", RE::BSShaderProperty::EShaderPropertyFlag::kZBufferTest},
        
		// Shader Flags 2
        {"z_buffer_write", RE::BSShaderProperty::EShaderPropertyFlag::kZBufferWrite},
        {"lod_landscape", RE::BSShaderProperty::EShaderPropertyFlag::kLODLandscape},
        {"lod_objects", RE::BSShaderProperty::EShaderPropertyFlag::kLODObjects},
        {"no_fade", RE::BSShaderProperty::EShaderPropertyFlag::kNoFade},
        {"two_sided", RE::BSShaderProperty::EShaderPropertyFlag::kTwoSided},
        {"vertex_colors", RE::BSShaderProperty::EShaderPropertyFlag::kVertexColors},
        {"glow_map", RE::BSShaderProperty::EShaderPropertyFlag::kGlowMap},
        {"assume_shadowmask", RE::BSShaderProperty::EShaderPropertyFlag::kAssumeShadowmask},
        {"character_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kCharacterLighting},
        {"multi_index_snow", RE::BSShaderProperty::EShaderPropertyFlag::kMultiIndexSnow},
        {"vertex_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kVertexLighting},
        {"uniform_scale", RE::BSShaderProperty::EShaderPropertyFlag::kUniformScale},
        {"fit_slope", RE::BSShaderProperty::EShaderPropertyFlag::kFitSlope},
        {"billboard", RE::BSShaderProperty::EShaderPropertyFlag::kBillboard},
        {"no_lod_land_blend", RE::BSShaderProperty::EShaderPropertyFlag::kNoLODLandBlend},
        {"envmap_light_fade", RE::BSShaderProperty::EShaderPropertyFlag::kEnvmapLightFade},
        {"wireframe", RE::BSShaderProperty::EShaderPropertyFlag::kWireframe},
        {"weapon_blood", RE::BSShaderProperty::EShaderPropertyFlag::kWeaponBlood},
        {"hide_on_local_map", RE::BSShaderProperty::EShaderPropertyFlag::kHideOnLocalMap},
        {"premult_alpha", RE::BSShaderProperty::EShaderPropertyFlag::kPremultAlpha},
        {"cloud_lod", RE::BSShaderProperty::EShaderPropertyFlag::kCloudLOD},
        {"anisotropic_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kAnisotropicLighting},
        {"no_transparency_multisample", RE::BSShaderProperty::EShaderPropertyFlag::kNoTransparencyMultiSample},
        {"menu_screen", RE::BSShaderProperty::EShaderPropertyFlag::kMenuScreen},
        {"multi_layer_parallax", RE::BSShaderProperty::EShaderPropertyFlag::kMultiLayerParallax},
        {"soft_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kSoftLighting},
        {"rim_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kRimLighting},
        {"back_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kBackLighting},
        {"snow", RE::BSShaderProperty::EShaderPropertyFlag::kSnow},
        {"tree_anim", RE::BSShaderProperty::EShaderPropertyFlag::kTreeAnim},
        {"effect_lighting", RE::BSShaderProperty::EShaderPropertyFlag::kEffectLighting},
        {"hd_lod_objects", RE::BSShaderProperty::EShaderPropertyFlag::kHDLODObjects}
    };*/

    static void CollectNodes(RE::NiNode* root, const std::vector<std::string>& nodeNames, std::vector<RE::NiNode*>& out)
    {
        if (!root || nodeNames.empty()) return;
    
        std::unordered_set<RE::NiNode*> visited;
    
        auto isNodeMatchingPattern = [&](RE::NiAVObject* node) -> bool
        {
            if (!node || !node->name.c_str()) return false;
    
            std::string nName{ node->name.c_str() };
            for (const auto& pattern : nodeNames) {
                if (std::search(nName.begin(), nName.end(), 
                            pattern.begin(), pattern.end(),
                            [](char a, char b) { 
                                return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b)); 
                            }) != nName.end())
                    return true;
            }
            return false;
        };
    
        std::function<void(RE::NiNode*, int)> dfs = [&](RE::NiNode* node, int depth)
        {
            if (!node || depth > 100) return;
            
            if (visited.find(node) != visited.end()) return;
            visited.insert(node);
    
            if (isNodeMatchingPattern(node)) out.emplace_back(node);
    
            auto& children = node->GetChildren();
            for (auto const& child : children)
            {
                if (child && child->AsNode()) dfs(child->AsNode(), depth + 1);
            }
            
            visited.erase(node);
        };
    
        dfs(root, 0);
    }

    /*static void CollectTriShapes(RE::NiNode* root, const std::vector<std::string>& strings, std::vector<RE::BSGeometry*>& out)
    {
        if (!root) return;
        
        bool collectAll = strings.empty();
        
        std::function<void(RE::NiAVObject*)> traverse = [&](RE::NiAVObject* obj) {
            if (!obj) return;
            
            if (auto* geometry = skyrim_cast<RE::BSGeometry*>(obj)) {
                bool shouldAdd = collectAll;
                
                if (!collectAll && obj->name.c_str()) {
                    std::string objName = obj->name.c_str();
                    for (const auto& pattern : strings) {
                        if (objName.find(pattern) != std::string::npos) {
                            shouldAdd = true;
                            break;
                        }
                    }
                }
                
                if (shouldAdd) {
                    out.push_back(geometry);
                }
            }
            
            if (auto* node = obj->AsNode()) {
                auto& children = node->GetChildren();
                for (auto& child : children) {
                    if (child) {
                        traverse(child.get());
                    }
                }
            }
        };
        
        traverse(root);
    }*/

// ╔════════════════════════════════════╗
// ║             UTILITIES              ║
// ╚════════════════════════════════════╝

    // Execute a list of commands in the console - taken and adapted from the ConsoleUtil NG source code
    void ExecuteCommand(const std::string& command, RE::TESObjectREFR* targetRef = nullptr) {
		// compileAndRun below drives the game's own script compiler/VM synchronously, and
		// commands like "enable"/"disable" can trigger native side effects that reenter the
		// Papyrus VM (e.g. firing a vanilla OnCellLoad handler on the target or a linked
		// reference). If the target's cell isn't fully attached yet, those handlers can run
		// against companion objects that are still mid-initialization and crash. Skip rather
		// than risk running a command against a reference that isn't safely loaded.
        if (targetRef) {
            // Last line of defence. The deferred (timer) paths in RuleManager now re-resolve the
            // target from a handle before getting here, so this should never see a freed
            // reference any more - but if one ever slips through, a cell pointer that is null or
            // obviously not a pointer (this crash had 0xFFFFFFFFFFFFFFFF) must be rejected
            // before IsAttached() dereferences it.
            const auto* cell = targetRef->GetParentCell();
            const auto cellAddr = reinterpret_cast<std::uintptr_t>(cell);
            if (targetRef->IsDeleted() || cellAddr < 0x10000 || cellAddr == static_cast<std::uintptr_t>(-1) || !cell->IsAttached()) {
                logger::warn("ExecuteCommand: Skipping '{}' - target's cell is not attached", command);
                return;
            }
        }

        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
        
        if (!script) return;
    
        std::string formattedCommand = command;
        
        script->SetCommand(formattedCommand);
        
        using func_t = void(RE::Script*, RE::ScriptCompiler*, RE::COMPILER_NAME, RE::TESObjectREFR*);
        REL::Relocation<func_t> compileAndRun{
            RELOCATION_ID(21416, REL::Module::get().version().patch() < 1130 ? 21890 : 441582)
        };
    
        RE::ScriptCompiler compiler;
        compileAndRun(script, &compiler, RE::COMPILER_NAME::kSystemWindowCompiler, targetRef);
    
        delete script;
    }

	// Play an idle on an actor by sending the event straight to its animation graph.
	//
	// This used to go through the "sendanimevent" console command, which prints
	// "The event was not processed by the graph" to the in-game console whenever the graph
	// rejects the event - and it rejects it often: an idle name that doesn't exist in that
	// actor's behavior graph, a graph state that can't take the idle right now, or the
	// follow-up "IdleStop" sent after an idle that had already ended or never started.
	// Calling NotifyAnimationGraph directly returns the same accepted/rejected result as a bool
	// without touching the console, so a rejection is now logged (once per event name) instead.
	bool PlayIdleOnActor(RE::Actor* actor, const std::string& idleName)
	{
		if (!actor || idleName.empty()) return false;
		return actor->NotifyAnimationGraph(idleName.c_str());
	}

	// Set the scale of an object
    void SetObjectScale(RE::TESObjectREFR* ref, float scale)
    {
        using func_t = void(RE::TESObjectREFR*, float);
        static REL::Relocation<func_t> func{ RELOCATION_ID(19239, 19665) };
        func(ref, scale);
    }

	// These are the reference types that can normally be picked up into the
	// player's inventory. Do not alter ownership on world objects such as
	// Activators, Movable Statics, Statics, Trees, Furniture, etc.
	bool IsPickableItem(RE::TESObjectREFR* ref)
	{
		if (!ref || ref->IsDeleted()) return false;
		auto* base = ref->GetBaseObject();
		if (!base) return false;

		switch (base->GetFormType()) {
		case RE::FormType::Ammo:
		case RE::FormType::Armor:
		case RE::FormType::Book:
		case RE::FormType::Ingredient:
		case RE::FormType::KeyMaster:
		case RE::FormType::Misc:
		case RE::FormType::Weapon:
		case RE::FormType::AlchemyItem:
		case RE::FormType::SoulGem:
		case RE::FormType::Scroll:
		case RE::FormType::Note:
		case RE::FormType::Light:
			return true;
		default:
			return false;
		}
	}

	// For newly-created pickable items, remove only the ownership that would
	// make the player steal the item. If the original reference had an NPC or
	// faction owner, preserve that ownership on the replacement - but only
	// outside interiors (see comment below).
	void NormalizePickableOwnership(RE::TESObjectREFR* spawned, RE::TESObjectREFR* source)
	{
		if (!IsPickableItem(spawned) || !source) return;

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) return;

		// Skyrim ownership is stored as a TESForm owner. For the player, the
		// correct owner form is the player's TESNPC base (normally FormID 0x7),
		// NOT the PlayerCharacter reference (normally RefID 0x14). Passing the
		// reference to SetOwner() creates an invalid/non-player ownership entry
		// and the item can still show as STEAL.
		auto* playerBase = player->GetActorBase();
		if (!playerBase) return;

		// IMPORTANT: GetOwner() is not sufficient for this purpose. Skyrim can
		// resolve ownership through the parent cell/location, while explicit
		// reference ownership is stored in ExtraOwnership. For this feature we
		// specifically care whether the SOURCE reference is explicitly owned by
		// the player.
		bool playerOwned = false;

		if (auto* ownership = source->extraList.GetByType<RE::ExtraOwnership>()) {
			playerOwned = ownership->owner == playerBase || ownership->owner == player;
		}

		// Also use Skyrim's ownership test as a fallback. This covers ownership
		// states where the engine recognizes the player as the owner but the
		// explicit ExtraOwnership entry is not exposed in the expected state.
		if (!playerOwned) {
			playerOwned = source->IsAnOwner(player, false, true);
		}

		if (!playerOwned) {
			auto* resolvedOwner = source->GetOwner();
			playerOwned = resolvedOwner == playerBase || resolvedOwner == player;
		}

		// A player-owned source ALWAYS produces an explicitly player-owned
		// replacement. Do this before any interior-cell ownership logic.
		if (playerOwned) {
			spawned->SetOwner(playerBase);
			return;
		}

		// Do not copy implicit interior-cell/location ownership to replacements.
		// An actually unowned interior source therefore remains unowned.
		if (auto* cell = spawned->GetParentCell(); cell && cell->IsInteriorCell()) {
			spawned->SetOwner(nullptr);
			return;
		}

		// Preserve explicit NPC/faction/etc. ownership for exterior references.
		if (auto* ownership = source->extraList.GetByType<RE::ExtraOwnership>(); ownership && ownership->owner) {
			spawned->SetOwner(ownership->owner);
			return;
		}

		// Finally fall back to the resolved owner for exterior references.
		spawned->SetOwner(source->GetOwner());
	}

// ╔════════════════════════════════════╗
// ║           LEVELED LISTS            ║
// ╚════════════════════════════════════╝

    // Resolve leveled items
    static RE::TESBoundObject* ResolveLeveledItem(RE::TESLevItem* lvli)
    {
        if (!lvli || lvli->entries.empty()) return nullptr;

		// Get player level
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return nullptr;
        int playerLevel = player->GetLevel();

		// Filter entries by level
        std::vector<RE::LEVELED_OBJECT*> validEntries;
        for (auto& entry : lvli->entries) {
            if (entry.level <= playerLevel) {
                validEntries.push_back(&entry);
            }
        }

        if (validEntries.empty()) return nullptr;

		// Random selection from valid entries
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, validEntries.size() - 1);

        constexpr int maxTries = 8;
        for (int i = 0; i < maxTries; ++i) {
            auto* entry = validEntries[dist(gen)];
            auto* picked = entry->form;
            if (!picked) continue;
            if (picked->GetFormType() == RE::FormType::LeveledItem) {
                auto* resolved = ResolveLeveledItem(picked->As<RE::TESLevItem>());
                if (resolved) return resolved;
            } else {
                return picked->As<RE::TESBoundObject>();
            }
        }
        return nullptr;
    }

	// Resolve leveled spells
    static RE::SpellItem* ResolveLeveledSpell(RE::TESLevSpell* lvls)
    {
        if (!lvls || lvls->entries.empty()) return nullptr;

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return nullptr;
        int playerLevel = player->GetLevel();

        std::vector<RE::LEVELED_OBJECT*> validEntries;
        for (auto& entry : lvls->entries) {
            if (entry.level <= playerLevel) {
                validEntries.push_back(&entry);
            }
        }

        if (validEntries.empty()) return nullptr;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, validEntries.size() - 1);

        constexpr int maxTries = 8;
        for (int i = 0; i < maxTries; ++i) {
            auto* entry = validEntries[dist(gen)];
            auto* picked = entry->form;
            if (!picked) continue;
            if (picked->GetFormType() == RE::FormType::LeveledSpell) {
                auto* resolved = ResolveLeveledSpell(picked->As<RE::TESLevSpell>());
                if (resolved) return resolved;
            } else {
                return picked->As<RE::SpellItem>();
            }
        }
        return nullptr;
    }

	// Resolve leveled NPCs
    static RE::TESNPC* ResolveLeveledNPC(RE::TESLevCharacter* lvlc)
    {
        if (!lvlc || lvlc->entries.empty()) return nullptr;

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return nullptr;
        int playerLevel = player->GetLevel();

        std::vector<RE::LEVELED_OBJECT*> validEntries;
        for (auto& entry : lvlc->entries) {
            if (entry.level <= playerLevel) {
                validEntries.push_back(&entry);
            }
        }

        if (validEntries.empty()) return nullptr;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, validEntries.size() - 1);

        constexpr int maxTries = 8;
        for (int i = 0; i < maxTries; ++i) {
            auto* entry = validEntries[dist(gen)];
            auto* picked = entry->form;
            if (!picked) continue;
            if (picked->GetFormType() == RE::FormType::LeveledNPC) {
                auto* resolved = ResolveLeveledNPC(picked->As<RE::TESLevCharacter>());
                if (resolved) return resolved;
            } else {
                return picked->As<RE::TESNPC>();
            }
        }
        return nullptr;
    }

// ╔════════════════════════════════════╗
// ║       POSITION CALCULTATIONS       ║
// ╚════════════════════════════════════╝

    inline NiPoint3 GetObjectCenter(RE::TESObjectREFR* target) {
        if (!target) return NiPoint3{0.0f, 0.0f, 0.0f};
        
        if (auto* root = target->Get3D()) {
            return root->worldBound.center;
        }
        
        const auto& bmin = target->GetBoundMin();
        const auto& bmax = target->GetBoundMax();
        return NiPoint3{
            (bmin.x + bmax.x) * 0.5f,
            (bmin.y + bmax.y) * 0.5f,
            (bmin.z + bmax.z) * 0.5f
        };
    }

    inline NiPoint3 GetObjectBottom(RE::TESObjectREFR* target) {
        if (!target) return NiPoint3{0.0f, 0.0f, 0.0f};
        
        if (auto* root = target->Get3D()) {
            const auto& center = root->worldBound.center;
            const auto& radius = root->worldBound.radius;
            return NiPoint3{center.x, center.y, center.z - radius};
        }
        
        const auto& bmin = target->GetBoundMin();
        const auto& bmax = target->GetBoundMax();
        return NiPoint3{
            (bmin.x + bmax.x) * 0.5f,
            (bmin.y + bmax.y) * 0.5f,
            bmin.z
        };
    }
    
    inline NiPoint3 GetObjectTop(RE::TESObjectREFR* target) {
        if (!target) return NiPoint3{0.0f, 0.0f, 0.0f};
        
        if (auto* root = target->Get3D()) {
            const auto& center = root->worldBound.center;
            const auto& radius = root->worldBound.radius;
            return NiPoint3{center.x, center.y, center.z + radius};
        }
        
        const auto& bmin = target->GetBoundMin();
        const auto& bmax = target->GetBoundMax();
        return NiPoint3{
            (bmin.x + bmax.x) * 0.5f,
            (bmin.y + bmax.y) * 0.5f,
            bmax.z
        };
    }
    
    inline void GetToNearestNavmesh(RE::TESObjectREFR* dummy, float verticalOffset = 1.0f) {
        if (!dummy || dummy->IsDeleted() || !dummy->GetParentCell()) {
            logger::warn("GetToNearestNavmesh: Invalid ref, dummy, or cell");
            return;
        }
    
        const auto cell = dummy->GetParentCell();
        const auto& runtimeData = cell->GetRuntimeData();
        if (!runtimeData.navMeshes) {
            logger::warn("GetToNearestNavmesh: No navmeshes in cell");
            return;
        }
    
        const auto dummyPos = dummy->GetPosition();
    
		// Taken and adapted from the Papyrus Extender source code
        auto& navMeshes = runtimeData.navMeshes->navMeshes;
        auto shortestDistance = (std::numeric_limits<float>::max)();
        std::optional<RE::NiPoint3> nearestNavmeshPos = std::nullopt;
    
        int navmeshcount = 0;
        int vertexcount = 0;
    
        for (const auto& navMesh : navMeshes) {
            if (!navMesh) continue;
            navmeshcount++;
            
            for (auto& [location] : navMesh->vertices) {
                vertexcount++;
                const auto linearDistance = dummyPos.GetDistance(location);
                if (linearDistance < shortestDistance) {
                    shortestDistance = linearDistance;
                    nearestNavmeshPos.emplace(location);
                }
            }
        }
    
        if (nearestNavmeshPos) {
            const RE::NiPoint3 finalPos = {
                dummyPos.x, 
                dummyPos.y,      
                nearestNavmeshPos->z + verticalOffset 
            };

            dummy->SetPosition(finalPos);
        } else {
            logger::warn("GetToNearestNavmesh: Navmesh position not found");
        }
    }

    // Parses a float out of a rule's "string" field, used by the newer spawnType
    // options (10/12) as a compact way to pass a single numeric parameter
    // (radius/distance) without adding a dedicated JSON field to every one of the
    // per-effect *SpawnData structs. Falls back to defaultValue on an empty or
    // non-numeric string so existing rules that used "string" for a node name (or
    // left it blank) keep behaving the same way for every other spawnType.
    inline float ParseFloatOrDefault(const std::string& str, float defaultValue) {
        if (str.empty()) return defaultValue;
        float result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
        if (ec != std::errc{}) return defaultValue;
        return result;
    }

    // Random point on a flat disc of the given radius around base, at base's height.
    // Used by spawnType 10.
    inline RE::NiPoint3 GetRandomOffsetPosition(const RE::NiPoint3& base, float radius) {
        if (radius <= 0.0f) return base;

        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> angleDist(0.0f, 6.28318530718f);
        // sqrt so points are distributed evenly across the disc's area rather than
        // bunching up near the center.
        std::uniform_real_distribution<float> unitDist(0.0f, 1.0f);

        const float angle = angleDist(rng);
        const float r = radius * std::sqrt(unitDist(rng));

        return RE::NiPoint3{
            base.x + r * std::cos(angle),
            base.y + r * std::sin(angle),
            base.z
        };
    }

    // Point offset "distance" units in front of target's current facing (negative
    // distance lands behind it instead). angle.z is target's yaw in radians, using
    // the standard Skyrim convention where 0 rad faces true north (+Y). Used by
    // spawnType 12.
    inline RE::NiPoint3 GetFrontOfTargetPosition(RE::TESObjectREFR* target, float distance) {
        if (!target) return RE::NiPoint3{ 0.0f, 0.0f, 0.0f };

        const auto pos = target->GetPosition();
        const float yaw = target->data.angle.z;
        const float forwardX = -std::sin(yaw);
        const float forwardY = std::cos(yaw);

        return RE::NiPoint3{
            pos.x + forwardX * distance,
            pos.y + forwardY * distance,
            pos.z
        };
    }

    // Universal safety net applied after every spawnType's own position logic has run
    // (0-14 alike): raises pos.z if it's below the terrain surface directly beneath
    // it, so a spawned object can no longer end up clipped into/under the ground no
    // matter which spawnType picked its (x, y). Only ever raises pos.z, never lowers
    // it - an object that landed above ground on its own is left exactly where its
    // spawnType put it. cellSource just supplies the parent cell to search in AND
    // (see below) the object whose own bound is used to judge how far its actual
    // visual bottom sits below its origin; it is not itself moved. Indoors, where
    // there is no terrain collision layer to hit, FindGroundZAtPosition simply finds
    // nothing and this is a no-op, so floors, shelves, etc. are unaffected.
    //
    // margin used to be applied as a flat, unconditional +2 units on top of the raw
    // terrain height. That assumes cellSource's origin sits at its own visual bottom -
    // true for plenty of clutter, but not universal, and even when it IS true a flat
    // +2 still visibly lifts anything that was genuinely meant to rest right at ground
    // level (which is the common case for something sitting directly on terrain
    // rather than a shelf or table) up out of the ground by ~3cm for no reason. This
    // showed up as SwapItem's replacement item appearing to hover slightly above
    // where the original item had been sitting. Using the object's own bound-bottom
    // (same object-space GetBoundMin() the isInWater filter uses,
    // scaled by its own GetScale()) instead of a flat margin means an object whose
    // origin is already at its base clamps flush with the ground (no visible lift),
    // while one whose origin sits further up its model still gets pushed out from
    // under the terrain by exactly the amount it actually needs - no more, no less.
    inline void ClampAboveGround(RE::TESObjectREFR* cellSource, RE::NiPoint3& pos, float margin = 0.1f) {
        if (!cellSource) return;
        auto* cell = cellSource->GetParentCell();
        if (!cell) return;

        // FindSolidSurfaceZAtPosition (terrain AND statics/ground/clutter/etc - see its own
        // comment in EventSinks.cpp) rather than FindGroundZAtPosition (terrain heightmap
        // ONLY). A huge amount of what a spawned/swapped item actually rests on - a wall-top
        // ledge, a table, a stone stair, a bridge deck - is static mesh geometry, not the
        // terrain heightmap, and FindGroundZAtPosition finds nothing there at all (an early
        // return, a no-op) - leaving the item exactly wherever PlaceObjectAtMe/the swapType
        // math happened to put it.
        float surfaceZ = 0.0f;
        if (!FindSolidSurfaceZAtPosition(cell, pos, 50.0f, 300.0f, surfaceZ)) return;

        const float boundBottomOffset = cellSource->GetBoundMin().z * cellSource->GetScale();
        const float groundZ = surfaceZ - boundBottomOffset + margin;

        // THIS USED TO ONLY EVER RAISE pos.z, NEVER LOWER IT - on the assumption that
        // whatever set pos.z above the surface already got it right, so the only failure
        // mode worth guarding against was clipping below ground. That assumption doesn't
        // hold for SwapItem/SwapLeveledItem/SwapActor: the replacement's position is
        // inherited directly from the ORIGINAL item's recorded coordinate, but two
        // different base objects very often don't share the same origin-to-visual-base
        // offset in their own mesh - e.g. a small wall sconce and a free-standing floor
        // brazier. A position that was exactly correct for the original item can leave the
        // swapped-in replacement hovering above whatever it's supposed to be resting on,
        // and the old raise-only logic did nothing for that case at all - which is exactly
        // the "SwapItem's replacement floats above the ground" bug. kMaxFloatGap bounds how
        // far this will now pull an item DOWN to close that gap - enough to absorb a normal
        // mesh-pivot mismatch, not so much that a legitimately elevated placement (an item
        // meant to sit on a shelf or table with open space below it) gets silently yanked
        // down to whatever solid surface exists further below.
        constexpr float kMaxFloatGap = 48.0f;
        if (pos.z < groundZ || pos.z - groundZ > kMaxFloatGap) {
            pos.z = groundZ;
        }
    }

// ╔════════════════════════════════════╗
// ║            SPAWN HELPER            ║
// ╚════════════════════════════════════╝

    // ╔════════════════════════════════════════════╗
    // ║  GROUND / WATER HIT (kHitGround/kHitWater)  ║
    // ╚════════════════════════════════════════════╝
    // RE::TESObjectLAND terrain and water planes are not TESObjectREFRs, so
    // neither a ground hit nor a water hit has a "target" to spawn from or
    // reposition relative to. These two helpers let the position-based spawn
    // effects (SpawnItem, SpawnActor, SpawnSpell, SpawnImpactDataSet,
    // SpawnExplosion, SpawnHazard, PlaySound) work anyway: spawn off an anchor
    // reference purely to get a valid cell context, then relocate the result
    // to the exact impact position - the same "place a throwaway ref, then
    // SetPosition()" approach SpawnImpactDataSet already uses for its dummy
    // reference. Despite the name, this anchor-resolution helper is shared by
    // both kHitGround and kHitWater (anywhere ctx.target is null).

    RE::Actor* ResolveGroundHitAnchor(const RuleContext& ctx) {
        if (ctx.source && !ctx.source->IsDeleted()) return ctx.source;
        return RE::PlayerCharacter::GetSingleton();
    }

    RE::NiPointer<RE::TESObjectREFR> SpawnAtPosition(RE::TESObjectREFR* anchor, RE::TESBoundObject* item, std::uint32_t fade, const RE::NiPoint3& pos) {
        if (!anchor || !item) {
            logger::error("SpawnAtPosition: Invalid anchor or item pointer");
            return nullptr;
        }

        if (anchor->IsDeleted() || !anchor->GetParentCell()) {
            logger::error("SpawnAtPosition: Anchor is deleted or has no parent cell");
            return nullptr;
        }

        auto spawnedItem = anchor->PlaceObjectAtMe(item, true);
        if (!spawnedItem || spawnedItem->IsDeleted()) {
            logger::warn("SpawnAtPosition: Spawn failed or was deleted immediately");
            return nullptr;
        }

        try {
            spawnedItem->SetPosition(pos);
        } catch (...) {
            logger::error("SpawnAtPosition: Exception while setting spawned item position");
        }

        if (fade == 0) {
            try {
                spawnedItem->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
            } catch (...) {
                logger::error("SpawnAtPosition: Exception while applying fade to spawned object");
            }
        }

        return spawnedItem;
    }

    // ctx is optional (defaults to nullptr) purely so every existing call site keeps
    // compiling unchanged; it only needs to be supplied by callers that want to use
    // spawnType 11 (exact hit point) or 13 (source actor position), since those two
    // need information Spawn() otherwise has no access to (the projectile's impact
    // point, and the actor who caused the hit).
    RE::NiPointer<RE::TESObjectREFR> Spawn(RE::TESObjectREFR* target, RE::TESBoundObject* item, std::uint32_t type, std::uint32_t fade, const std::string& nodeName = "", const RuleContext* ctx = nullptr, bool applyGroundClamp = true) {
        if (!target || !item) {
            logger::error("Spawn: Invalid target or item pointer");
            return nullptr;
        }
    
        if (target->IsDeleted() || !target->GetParentCell()) {
            logger::error("Spawn: Target is deleted or has no parent cell");
            return nullptr;
        }
    
		// Helper lambda for fade application
        auto ApplyFade = [](RE::NiPointer<RE::TESObjectREFR> ref) {
            if (!ref || ref->IsDeleted()) {
                return;
            }
                        
            try {
                ref->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
            } catch (...) {
                logger::error("Spawn: Exception while applying fade to spawned object");
            }
        };
    
		// For types <= 3, just place the item directly
        if (type <= 3) {
            auto spawnedItem = target->PlaceObjectAtMe(item, true);
            if (spawnedItem) {
                if (spawnedItem->IsDeleted()) {
                    logger::warn("Spawn: Spawned item was deleted immediately");
                    return nullptr;
                }
                
                if (!spawnedItem->IsDeleted() && spawnedItem->GetParentCell()) {
                    try {
                        switch (type) {
                            case 1: spawnedItem->SetPosition(GetObjectCenter(target));  break;
                            case 2: spawnedItem->SetPosition(GetObjectTop(target));     break;
                            case 3: spawnedItem->SetPosition(GetObjectBottom(target));  break;
                            default: break;
                        }
                    } catch (...) {
                        logger::error("Spawn: Exception while setting spawned item position");
                    }

                    // Same ground safety net as the dummy-based path below. Skipped
                    // entirely when applyGroundClamp is false (SwapItem's own call passes
                    // false) - a plain in-place swap wants the replacement to sit at
                    // literally the original object's position, not have it adjusted.
                    if (applyGroundClamp) {
                        try {
                            auto pos = spawnedItem->GetPosition();
                            ClampAboveGround(spawnedItem.get(), pos);
                            spawnedItem->SetPosition(pos);
                        } catch (...) {
                            logger::error("Spawn: Exception while clamping spawned item above ground");
                        }
                    }

                    if (fade == 0) {
                        ApplyFade(spawnedItem);
                    }
                }
                NormalizePickableOwnership(spawnedItem.get(), target);
            }
            return spawnedItem;
        }
    
        static RE::TESBoundObject* dummyForm = nullptr;
        static std::mutex dummyFormMutex;
        static bool dummyFormInitialized = false;
        
        {
            // Try to get the dummy
            std::lock_guard<std::mutex> lock(dummyFormMutex);
            if (!dummyFormInitialized) {
                dummyFormInitialized = true;
                auto* dh = RE::TESDataHandler::GetSingleton();
                if (dh) {
                    auto* form = dh->LookupForm(0x000B79FF, "Skyrim.esm");
                    if (form) {
                        dummyForm = form->As<RE::TESBoundObject>();
                        if (!dummyForm) {
                            logger::error("Spawn: Failed to cast dummy form to TESBoundObject");
                        }
                    } else {
                        logger::error("Spawn: Failed to lookup dummy form 0x000B79FF");
                    }
                } else {
                    logger::error("Spawn: TESDataHandler singleton is null");
                }
            }
        }
    
        if (!dummyForm) {
            logger::warn("Spawn: Dummy form unavailable, falling back to direct spawn");
            auto spawned = target->PlaceObjectAtMe(item, true);
            if (spawned && !spawned->IsDeleted()) {
                if (fade == 0) {
                    ApplyFade(spawned);
                }
                NormalizePickableOwnership(spawned.get(), target);
                return spawned;
            }
            logger::warn("Spawn: Direct spawn fallback also failed");
            return nullptr;
        }
    
		// Create the dummy
        auto dummy = target->PlaceObjectAtMe(dummyForm, false);
        if (!dummy) {
            logger::warn("Spawn: Failed to create dummy, falling back to direct spawn");
            auto spawned = target->PlaceObjectAtMe(item, true);
            if (spawned && !spawned->IsDeleted()) {
                if (fade == 0) {
                    ApplyFade(spawned);
                }
                NormalizePickableOwnership(spawned.get(), target);
                return spawned;
            }
            logger::warn("Spawn: Direct spawn fallback also failed");
            return nullptr;
        }
    
		// Validate dummy state before proceeding
        if (dummy->IsDeleted() || !dummy->GetParentCell() || dummy->IsDisabled()) {
            logger::warn("Spawn: Dummy is in invalid state, cleaning up and falling back");
            if (dummy && !dummy->IsDeleted()) {
                try {
                    dummy->SetDelete(true);
                } catch (...) {
                    logger::error("Spawn: Exception while deleting dummy");
                }
            }
            auto spawned = target->PlaceObjectAtMe(item, true);
            if (spawned && !spawned->IsDeleted()) {
                if (fade == 0) {
                    ApplyFade(spawned);
                }
                NormalizePickableOwnership(spawned.get(), target);
                return spawned;
            }
            logger::warn("Spawn: Direct spawn fallback also failed");
            return nullptr;
        }

		// For type 9, try to find the specific node first
        RE::NiNode* targetNode = nullptr;
        if (type == 9 && !nodeName.empty()) {
            if (!target->Get3D()) target->Load3D(true);
            auto* rootObj = target->Get3D();
            if (rootObj) {
                auto* rootNode = rootObj->AsNode();
                if (rootNode) {
                    std::vector<RE::NiNode*> foundNodes;
                    std::vector<std::string> nodeNames = {nodeName};
                    CollectNodes(rootNode, nodeNames, foundNodes);
                    if (!foundNodes.empty()) {
                        targetNode = foundNodes[0];
                    }
                }
            }
            
            if (!targetNode) {
                logger::warn("Spawn: Node '{}' not found for type 9, falling back to root node", nodeName);
            }
        }
    
		// Move the dummy to the target node
        if (auto targetObject = target->Get3D()) {
            if (auto node = targetObject->AsNode()) {
                if (dummy && !dummy->IsDeleted() && dummy->GetParentCell() && 
                    target && !target->IsDeleted() && target->GetParentCell()) {
                    try {
                        if (type == 9 && targetNode) {
                            // Move to the specific node found
                            dummy->MoveToNode(target, targetNode);
                        } else {
                            // Move to root node for other types
                            dummy->MoveToNode(target, node);
                        }
                        dummy->data.angle = target->data.angle;
                    } catch (...) {
                        logger::error("Spawn: Exception while moving dummy to node");
                    }
                }
            }
        }
    
		// Position the dummy based on type
        if (dummy && !dummy->IsDeleted() && dummy->GetParentCell()) {
            try {
                switch (type) {
                    case 5: dummy->SetPosition(GetObjectCenter(target));  break;
                    case 6: dummy->SetPosition(GetObjectTop(target));     break;
                    case 7: dummy->SetPosition(GetObjectBottom(target));  break;
                    case 8: {
                        auto dummyPos = dummy->GetPosition();
                        GetToNearestNavmesh(dummy.get(), 1.0f);
                        dummy->data.angle = RE::NiPoint3{0.0f, 0.0f, 0.0f};
                        break;
                    }
                    case 9: {
                        dummy->data.angle = target->data.angle;
                        break;
                    }
                    case 10: {
                        // nodeName doubles as the radius here (e.g. "128"); defaults to 64 units if
                        // blank or not a valid number.
                        const float radius = ParseFloatOrDefault(nodeName, 64.0f);
                        dummy->SetPosition(GetRandomOffsetPosition(dummy->GetPosition(), radius));
                        break;
                    }
                    case 11: {
                        // Exact projectile impact point, when the caller supplied one via ctx
                        // (currently only kHit on a TESObjectREFR populates this - see
                        // EventSinks.cpp's ProcessProjectileImpact). Falls back to the object's
                        // center for melee/magic hits, which carry no impact coordinate.
                        if (ctx && ctx->hasHitPos) {
                            dummy->SetPosition(ctx->hitPos);
                        } else {
                            dummy->SetPosition(GetObjectCenter(target));
                        }
                        break;
                    }
                    case 12: {
                        // nodeName doubles as the distance here (e.g. "80"); defaults to 50 units.
                        // Negative values land behind the target instead of in front of it.
                        const float distance = ParseFloatOrDefault(nodeName, 50.0f);
                        dummy->SetPosition(GetFrontOfTargetPosition(target, distance));
                        dummy->data.angle = target->data.angle;
                        break;
                    }
                    case 13: {
                        // The actor who caused the hit (attacker/thrower), not the target.
                        // Falls back to the target's own position if there's no source actor
                        // (e.g. a trap or scripted event with no attacking actor).
                        if (ctx && ctx->source && !ctx->source->IsDeleted()) {
                            dummy->SetPosition(ctx->source->GetPosition());
                        }
                        break;
                    }
                    case 14: {
                        // nodeName doubles as the vertical offset here (e.g. "50" for above
                        // target, "-30" for below it); defaults to 50 units above. Note that
                        // the ClampAboveGround pass further below will still pull a "below"
                        // offset back up if it would end up under the actual terrain surface -
                        // so a negative offset is most useful for targets that are themselves
                        // above the ground (e.g. spawning underneath a hanging/floating
                        // object), not for digging into solid terrain.
                        const float offset = ParseFloatOrDefault(nodeName, 50.0f);
                        auto pos = target->GetPosition();
                        pos.z += offset;
                        dummy->SetPosition(pos);
                        break;
                    }
                }
            } catch (...) {
                logger::error("Spawn: Exception while setting dummy position");
            }
        }

        // Safety net: no matter which spawnType (0-14) computed the position above,
        // never let the final spot sit below the actual ground. Skipped when
        // applyGroundClamp is false, same as the type<=3 path above.
        if (applyGroundClamp && dummy && !dummy->IsDeleted() && dummy->GetParentCell()) {
            try {
                auto pos = dummy->GetPosition();
                ClampAboveGround(dummy.get(), pos);
                dummy->SetPosition(pos);
            } catch (...) {
                logger::error("Spawn: Exception while clamping dummy above ground");
            }
        }
    
		// Spawn the real item from dummy
        RE::NiPointer<RE::TESObjectREFR> spawned;
        if (dummy && !dummy->IsDeleted() && dummy->GetParentCell()) {
            try {
                spawned = dummy->PlaceObjectAtMe(item, true);
            } catch (...) {
                logger::error("Spawn: Exception while spawning item from dummy");
                spawned = nullptr;
            }
        }
    
        if (!spawned || spawned->IsDeleted()) {
            try {
                spawned = target->PlaceObjectAtMe(item, true);
            } catch (...) {
                logger::error("Spawn: Exception during fallback direct spawn");
                spawned = nullptr;
            }
        }
    
        if (fade == 0 && spawned && !spawned->IsDeleted()) {
            ApplyFade(spawned);
        }
    
        if (dummy && !dummy->IsDeleted()) {
            try {
                dummy->Disable();
                dummy->SetDelete(true);
            } catch (...) {
                logger::error("Spawn: Exception while cleaning up dummy");
            }
        }

        if (spawned && !spawned->IsDeleted()) {
            NormalizePickableOwnership(spawned.get(), target);
        }

        return spawned;
    }


//███████╗███████╗███████╗███████╗░█████╗░████████╗░██████╗
//██╔════╝██╔════╝██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝
//█████╗░░█████╗░░█████╗░░█████╗░░██║░░╚═╝░░░██║░░░╚█████╗░
//██╔══╝░░██╔══╝░░██╔══╝░░██╔══╝░░██║░░██╗░░░██║░░░░╚═══██╗
//███████╗██║░░░░░██║░░░░░███████╗╚█████╔╝░░░██║░░░██████╔╝
//╚══════╝╚═╝░░░░░╚═╝░░░░░╚══════╝░╚════╝░░░░╚═╝░░░╚═════╝░                               

// ╔════════════════════════════════════╗
// ║             UTILITIES              ║
// ╚════════════════════════════════════╝
	
	void ExecuteConsoleCommand(const RuleContext& ctx, const std::vector<StringData>& commandsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("ExecuteConsoleCommand: No target to search for closest actors");
			return;
		}

		if (commandsData.empty()) {
			logger::error("ExecuteConsoleCommand: No commands to execute");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("ExecuteConsoleCommand: TES singleton is null");
			return;
		}

		float radius = 0.0f;
		if (!commandsData.empty()) radius = commandsData[0].radius.value;

		std::vector<RE::Actor*> targets;
		tes->ForEachReferenceInRange(ctx.target, radius, [&](RE::TESObjectREFR* a_ref) {
			auto* actor = a_ref->As<RE::Actor>();
			if (actor && !actor->IsDead() && !actor->IsDisabled()) {
				targets.push_back(actor);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		if (targets.empty()) {
			logger::warn("ExecuteConsoleCommand: No valid actors found in range");
			return;
		}

		for (auto* actor : targets) {
			for (const auto& commandData : commandsData) {
				if (commandData.string.empty()) continue;
				ExecuteCommand(commandData.string, actor);
				ExecuteCommand("", nullptr);
			}
		}
	}

	void ExecuteConsoleCommandOnItem(const RuleContext& ctx, const std::vector<StringData>& commandsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("ExecuteConsoleCommandOnItem: No target to execute commands on");
			return;
		}

		if (commandsData.empty()) {
			logger::error("ExecuteConsoleCommandOnItem: No commands to execute");
			return;
		}

		for (const auto& commandData : commandsData) {
			if (commandData.string.empty()) continue;
			ExecuteCommand(commandData.string, ctx.target);
			ExecuteCommand("", nullptr);
		}
	}

	void ExecuteConsoleCommandOnSource(const RuleContext& ctx, const std::vector<StringData>& commandsData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("ExecuteConsoleCommandOnSource: No source to execute commands on");
			return;
		}

		if (commandsData.empty()) {
			logger::error("ExecuteConsoleCommandOnSource: No commands to execute");
			return;
		}

		for (const auto& commandData : commandsData) {
			if (commandData.string.empty()) continue;
			ExecuteCommand(commandData.string, ctx.source);
			ExecuteCommand("", nullptr);
		}
	}

	void ShowNotification(const RuleContext& ctx, const std::vector<StringData>& notificationsData)
	{
		ctx;

		if (notificationsData.empty()) {
			logger::error("ShowNotification: No notifications to show");
			return;
		}

		for (const auto& notificationData : notificationsData) {
			if (notificationData.string.empty()) continue;

			RE::DebugNotification(notificationData.string.c_str());
		}
	}

	void ShowMessageBox(const RuleContext& ctx, const std::vector<StringData>& messagesData)
	{
		ctx;

		if (messagesData.empty()) {
			logger::error("ShowMessageBox: No messages to show");
			return;
		}

		for (const auto& messageData : messagesData) {
			if (messageData.string.empty()) continue;

			RE::DebugMessageBox(messageData.string.c_str());
		}
	}

	// Finds the crime faction that currently applies to an actor, by walking up their location
	// hierarchy until a location with an explicit Crime Faction is found.
	static RE::TESFaction* GetCurrentCrimeFaction(RE::Actor* actor)
	{
		if (!actor) return nullptr;

		auto* loc = actor->GetCurrentLocation();
		std::set<RE::FormID> visited;
		while (loc) {
			if (loc->unreportedCrimeFaction) return loc->unreportedCrimeFaction;

			auto* parent = loc->parentLoc;
			if (!parent || visited.contains(parent->GetFormID())) break;
			visited.insert(loc->GetFormID());
			loc = parent;
		}

		return nullptr;
	}


	// Adds crime gold to the player. The relevant crime faction (and therefore which Hold's
	// bounty gets the gold) is resolved automatically from the player's current location.
	void AddBounty(const RuleContext& ctx, const std::vector<BountyData>& bountyData)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			logger::error("AddBounty: No player");
			return;
		}

		auto* faction = GetCurrentCrimeFaction(player);
		if (!faction) {
			logger::warn("AddBounty: Player's current location has no associated crime faction, skipping");
			return;
		}

		for (const auto& data : bountyData) {
			if (data.amount.value == 0) continue;

			const auto current = data.violent ?
				player->GetViolentCrimeGoldValue(faction) :
				player->GetNonViolentCrimeGoldValue(faction);
			const auto sum = static_cast<std::uint64_t>(current) + static_cast<std::uint64_t>(data.amount.value);
			const auto uint32Max = static_cast<std::uint64_t>(0xFFFFFFFFULL);
			const auto newTotal = static_cast<std::uint32_t>(sum > uint32Max ? uint32Max : sum);
			player->SetCrimeGoldValue(faction, data.violent, newTotal);

			// Make the engine process this as an actual theft committed by the player.
			// StealAlarm() is an Actor method, so calling it on the player makes the player
			// the crime actor instead of trying to make a guard pretend to be the criminal.
			// This follows the same native path used by ObjectReference.SendStealAlarm().
			if (ctx.target) {
				auto* object = ctx.target->GetBaseObject();
				auto* owner = ctx.target->GetOwner();
				if (!owner) {
					owner = faction;
				}

				player->StealAlarm(ctx.target, object, 1, 1, owner, true);
				logger::info("AddBounty: Triggered native theft alarm with player as crime actor");
			} else {
				logger::warn("AddBounty: No target reference available to trigger native theft alarm");
			}

			logger::info("AddBounty: Added {} {} bounty to faction 0x{:08X}",
				data.amount.value,
				data.violent ? "violent" : "non-violent",
				faction->GetFormID());

			const char* factionName = faction->GetFullName();
			const auto notification = std::format("{} bounty added to {}",
				data.amount.value,
				(factionName && *factionName) ? factionName : "current hold");
			RE::DebugNotification(notification.c_str());
		}
	}

	void PlayIdle(const RuleContext& ctx, const std::vector<PlayIdleData>& playIdleData)
	{
		if (!ctx.source || ctx.source->IsDeleted() || ctx.source->IsDead()) {
			logger::error("PlayIdle: No valid actor to play idle animation");
			return;
		}

		const auto& data = playIdleData[0];

		if (data.string.empty()) {
			logger::error("PlayIdle: No idle name (string) provided");
			return;
		}

		if (data.duration <= 0.0f) {
			logger::error("PlayIdle: Invalid duration for idle animation: {}", data.duration);
			return;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (player && ctx.source == player) {
			RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
			if (!camera || camera->IsInFirstPerson()) {
				return;
			}
		}

		if (ctx.source->IsInCombat() || ctx.source->IsInKillMove() || ctx.source->IsInRagdollState() || ctx.source->IsOnMount() ||
			ctx.source->AsActorState()->IsFlying() || ctx.source->AsActorState()->IsWeaponDrawn() || ctx.source->AsActorState()->IsBleedingOut()) {
			return;
		}

		if (!PlayIdleOnActor(ctx.source, data.string)) {
			// Rejected by the graph: log it once per event name (instead of the console spam)
			// and do NOT schedule an IdleStop for an idle that never started.
			static std::mutex loggedMutex;
			static std::unordered_set<std::string> logged;
			bool firstTime = false;
			{
				std::lock_guard<std::mutex> lock(loggedMutex);
				firstTime = logged.insert(data.string).second;
			}
			if (firstTime) {
				logger::info("PlayIdle: the animation graph did not accept '{}' on actor {:08X} (unknown event name for this actor, or its graph can't play it in its current state)",
					data.string, ctx.source->GetFormID());
			}
			return;
		}

		static std::vector<std::future<void>> runningTasks;
		static std::mutex tasksMutex;

		// Keep a handle, not a raw pointer: the actor can be unloaded/deleted during the wait.
		auto future = std::async(std::launch::async, [handle = ctx.source->CreateRefHandle(), duration = data.duration]() {
			std::this_thread::sleep_for(std::chrono::duration<float>(duration));

			SKSE::GetTaskInterface()->AddTask([handle]() {
				auto ref = handle.get();
				auto* actor = ref ? ref->As<RE::Actor>() : nullptr;
				if (actor && !actor->IsDeleted() && !actor->IsDead()) {
					PlayIdleOnActor(actor, "IdleStop");
				}
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

	void UnlockItem(const RuleContext& ctx)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("UnlockItem: No target to open");
			return;
		}

		auto* openCloseForm = ctx.target->GetBaseObject()->As<RE::BGSOpenCloseForm>();
		if (!openCloseForm) {
			return;
		}

		auto state = RE::BGSOpenCloseForm::GetOpenState(ctx.target);
		if (state == RE::BGSOpenCloseForm::OPEN_STATE::kOpen || state == RE::BGSOpenCloseForm::OPEN_STATE::kOpening) {
			return;
		}

		RE::BGSOpenCloseForm::SetOpenState(ctx.target, true, false);
	}

	void LockItem(const RuleContext& ctx)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("LockItem: No target to close");
			return;
		}

		auto* openCloseForm = ctx.target->GetBaseObject()->As<RE::BGSOpenCloseForm>();
		if (!openCloseForm) {
			return;
		}

		auto state = RE::BGSOpenCloseForm::GetOpenState(ctx.target);
		if (state == RE::BGSOpenCloseForm::OPEN_STATE::kClosed || state == RE::BGSOpenCloseForm::OPEN_STATE::kClosing) {
			return;
		}

		RE::BGSOpenCloseForm::SetOpenState(ctx.target, false, false);
	}

	void ActivateItem(const RuleContext& ctx)
	{
		if (ctx.event == EventType::kActivate) {
			logger::debug("ActivateItem: Event is kActivate, skipping activation");
			return;
		}

		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("ActivateItem: No target to activate");
			return;
		}

		ctx.target->ActivateRef(ctx.source, 0, nullptr, 1, false);
	}

	// Minimum scale the engine can be reliably given: at/near 0 the object's
	// collision and render bounds degenerate (it can vanish or its havok
	// bhkRigidBody can fail to update), so it's clamped here rather than
	// letting a 0.0 or negative value reach SetObjectScale. There's no
	// analogous upper clamp - the engine accepts arbitrarily large scales
	// (same as the "Scale" field in the Creation Kit), it just may look or
	// collide strangely well past the values the game ships with.
	static constexpr float kMinObjectScale = 0.01f;

	// Shared between ScaleObject and RestoreObjectScale: remembers the scale a
	// reference had the FIRST time scaleobject ever touched it during this
	// play session, so restorescale has something meaningful to put it back
	// to. This is in-memory only - it is not saved with the game, so it does
	// not survive a reload; a reference scaled in an earlier session (or
	// before this map existed) has no record and restorescale falls back to
	// the engine default of 1.0 for it.
	std::unordered_map<RE::FormID, float>& GetOriginalScaleMap() {
		static std::unordered_map<RE::FormID, float> map;
		return map;
	}

	std::mutex& GetOriginalScaleMutex() {
		static std::mutex m;
		return m;
	}

	void ScaleObject(const RuleContext& ctx, const std::vector<ScaleObjectData>& scaleData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("ScaleObject: No target to scale");
			return;
		}

		if (scaleData.empty()) {
			logger::error("ScaleObject: No scale data provided");
			return;
		}

		// Remember the reference's scale as it was BEFORE this call touches it,
		// but only the first time - if a record already exists (from an
		// earlier hit), keep that one so it still reflects the true original.
		{
			RE::FormID targetID = ctx.target->GetFormID();
			std::lock_guard<std::mutex> lock(GetOriginalScaleMutex());
			auto& map = GetOriginalScaleMap();
			if (map.find(targetID) == map.end()) {
				map[targetID] = ctx.target->GetScale();
			}
		}

		static thread_local std::mt19937 rng(std::random_device{}());

		bool appliedAnyScale = false;
		bool useFade = true;

		for (const auto& data : scaleData) {
			float factor = data.scale.value;

			if (data.scale.useRandom) {
				float lo = (std::min)(data.scale.min, data.scale.max);
				float hi = (std::max)(data.scale.min, data.scale.max);
				factor = std::uniform_real_distribution<float>(lo, hi)(rng);
			}

			// scale.value defaults to -1.0f when the rule doesn't specify a
			// "scale" field at all. In absolute mode that means "reset to
			// normal size" (1.0); in relative mode it means "no change"
			// (multiply by 1.0) - both map to the same substitution.
			if (factor < 0.0f) {
				factor = 1.0f;
			}

			// Relative mode multiplies the CURRENT scale by the given factor,
			// so repeated hits compound (e.g. 2.0 then 3.0 -> 6.0 total)
			// instead of each hit just overwriting the last absolute value.
			float newScale = data.relative ? (ctx.target->GetScale() * factor) : factor;

			newScale = (std::max)(newScale, kMinObjectScale);

			SetObjectScale(ctx.target, newScale);

			// SetObjectScale (native SetScale) writes the new scale into the
			// reference's data and updates the NiNode render transform
			// immediately, so this keeps things visually correct even before
			// the collision rebuild below runs.
			ctx.target->Update3DPosition(true);

			appliedAnyScale = true;
			useFade = (data.fade != 0);
		}

		// SetObjectScale/Update3DPosition update the render transform, but for
		// a reference whose 3D/collision was already loaded before this call
		// (which is always the case here - ctx.target is an existing object
		// that was just hit, not a freshly spawned one) the havok rigid body's
		// collision shape is NOT resynced to match the new scale by either of
		// those calls. The reliable way to force the engine to rebuild the
		// havok body (and everything else 3D-related) at the object's current
		// stored scale is a full disable/enable cycle, which forces a fresh
		// 3D load. This is skipped for actors: their movement collision is a
		// character-controller capsule that already rescales correctly from
		// SetScale alone, and disabling/enabling an actor is far more
		// disruptive (drops AI packages, combat state, etc.) than doing the
		// same to a static/activator/etc.
		//
		// Note this disable/enable cycle is purely a collision-refresh
		// mechanism, not a visual transition - Enable() is always called
		// with a_fade=false here so the object pops straight back with no
		// gap. "fade" below is unrelated to that: it mirrors the ONLY fade
		// mechanism OIF actually uses elsewhere (spawn/swap effects), which
		// is the kNeverFades record flag rather than an alpha animation.
		// That flag doesn't produce any visible change up close either - it
		// only controls whether the engine is allowed to fade the object out
		// due to distance/LOD once you walk away and come back, so "fade 0
		// vs 1" was never going to look different while standing right next
		// to it either way.
		if (appliedAnyScale && !ctx.target->IsDeleted() && !ctx.target->As<RE::Actor>()) {
			if (!ctx.target->IsDisabled()) {
				ctx.target->Disable();
			}
			ctx.target->Enable(false);

			if (useFade) {
				ctx.target->formFlags &= ~static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
			} else {
				ctx.target->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
			}
		}
	}

	void RestoreObjectScale(const RuleContext& ctx, const std::vector<RestoreScaleData>& restoreData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("RestoreObjectScale: No target to restore");
			return;
		}

		if (restoreData.empty()) {
			logger::error("RestoreObjectScale: No restore data provided");
			return;
		}

		RE::FormID targetID = ctx.target->GetFormID();
		float originalScale = 1.0f;
		bool hasRecord = false;

		{
			std::lock_guard<std::mutex> lock(GetOriginalScaleMutex());
			auto& map = GetOriginalScaleMap();
			auto it = map.find(targetID);
			if (it != map.end()) {
				originalScale = it->second;
				hasRecord = true;
			}
		}

		if (!hasRecord) {
			// Nothing on record for this reference - either scaleobject never
			// touched it this session, or the game was reloaded since (this
			// tracking is in-memory only, not saved). Fall back to the engine
			// default rather than silently doing nothing.
			logger::info("RestoreObjectScale: No original scale recorded for {:08X}, defaulting to 1.0", targetID);
			originalScale = 1.0f;
		}

		originalScale = (std::max)(originalScale, kMinObjectScale);

		SetObjectScale(ctx.target, originalScale);
		ctx.target->Update3DPosition(true);

		bool useFade = true;
		for (const auto& data : restoreData) {
			useFade = (data.fade != 0);
		}

		// Same collision-refresh mechanism as ScaleObject - see the comment
		// there for why this disable/enable cycle is necessary and why it's
		// skipped for actors.
		if (!ctx.target->IsDeleted() && !ctx.target->As<RE::Actor>()) {
			if (!ctx.target->IsDisabled()) {
				ctx.target->Disable();
			}
			ctx.target->Enable(false);

			if (useFade) {
				ctx.target->formFlags &= ~static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
			} else {
				ctx.target->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
			}
		}

		// Clear the record now that it's been restored, so a later scaleobject
		// call on this same reference starts tracking a fresh baseline instead
		// of reusing this one.
		{
			std::lock_guard<std::mutex> lock(GetOriginalScaleMutex());
			GetOriginalScaleMap().erase(targetID);
		}
	}

	// ╔══════════════════════════════════════════════════════════════════════════╗
	// ║  SHARED RADIUS-EFFECT PLUMBING (modav / applyforce)                      ║
	// ╚══════════════════════════════════════════════════════════════════════════╝
	//
	// Both effects are "find every actor within R of the impact point and do something to
	// them". They previously each rolled their own search against a single
	// TESObjectCELL::ForEachReferenceInRange, which has a quiet but important limitation:
	// it only ever walks the reference list of that ONE cell. Exterior worldspaces are a
	// grid of cells, so any actor standing on the other side of a cell boundary - often
	// only a few metres away, and well inside the requested radius - was simply never
	// visited. TES::ForEachReferenceInRange walks the whole loaded grid instead, which is
	// what every other radius effect in this file already uses.
	//
	// TES::ForEachReferenceInRange takes a reference as its origin rather than a raw
	// position, so the search is run from whichever reference we have (target, else
	// source) with the radius widened by that reference's distance to the real centre,
	// then each candidate is re-tested against the true centre point. That keeps the
	// effective shape an exact sphere around centerPos even for kHitGround/kHitWater,
	// where there is no target at all and the caster can be some distance from the impact.

	// Resolves the point a radius effect should be centred on: the exact impact position
	// when we have one (kHitGround/kHitWater always do, ordinary projectile hits usually
	// do), then the target's position, then the source's.
	static bool ResolveEffectCenter(const RuleContext& ctx, RE::NiPoint3& a_outCenter)
	{
		if (ctx.hasHitPos) {
			a_outCenter = ctx.hitPos;
			return true;
		}
		if (ctx.target && !ctx.target->IsDeleted()) {
			a_outCenter = ctx.target->GetPosition();
			return true;
		}
		if (ctx.source && !ctx.source->IsDeleted()) {
			a_outCenter = ctx.source->GetPosition();
			return true;
		}
		return false;
	}

	static void CollectActorsInRadius(const RuleContext& ctx, const RE::NiPoint3& a_center, float a_radius,
		bool a_affectSource, bool a_affectPlayer, std::vector<RE::Actor*>& a_out)
	{
		auto* tes = RE::TES::GetSingleton();
		if (!tes) return;

		RE::TESObjectREFR* origin = nullptr;
		if (ctx.target && !ctx.target->IsDeleted()) {
			origin = ctx.target;
		} else if (ctx.source && !ctx.source->IsDeleted()) {
			origin = ctx.source;
		} else if (auto* player = RE::PlayerCharacter::GetSingleton()) {
			origin = player;
		}
		if (!origin) return;

		// Widen the query so the sphere we actually want is fully contained in the sphere
		// TES searches from the origin reference, then discard anything outside the real
		// radius below.
		const float offset = origin->GetPosition().GetDistance(a_center);
		const float queryRadius = a_radius + offset;

		tes->ForEachReferenceInRange(origin, queryRadius, [&](RE::TESObjectREFR* ref) -> RE::BSContainer::ForEachResult {
			auto* actor = ref ? ref->As<RE::Actor>() : nullptr;
			if (!actor || actor->IsDisabled() || actor->IsDeleted() || actor->IsDead() || actor->IsGhost()) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			if (actor->GetPosition().GetDistance(a_center) > a_radius) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			if (actor->IsPlayerRef() && !a_affectPlayer) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			if (ctx.source && actor == ctx.source && !a_affectSource) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			a_out.push_back(actor);
			return RE::BSContainer::ForEachResult::kContinue;
		});
	}

	// Leaner sibling of CollectActorsInRadius for the "dummy caster/anchor spawned
	// exactly at the desired center" callers (SpawnSpell, SpawnLeveledSpell,
	// ApplySpell, ApplyIngestible/ApplyOtherIngestible, SpawnEffectShader,
	// SpawnArtObject): origin's own position IS the center they want to search
	// around, so there's no origin-to-center gap to widen the query for - only the
	// same "TES::ForEachReferenceInRange doesn't return a guaranteed sphere" problem
	// CollectActorsInRadius's a_center.GetDistance() re-check above corrects for.
	// Deliberately does nothing beyond that distance re-check (no IsDead/IsDisabled/
	// etc. filtering) - each caller already has its own such checks in its callback,
	// and this only exists to fix the search volume's shape, not to change who
	// within it gets included. callback receives each actor confirmed to be within
	// an exact 3D radius of origin's position, and returns a
	// RE::BSContainer::ForEachResult the same way a raw ForEachReferenceInRange
	// callback would.
	template <typename F>
	static void ForEachActorInSphere(RE::TESObjectREFR* origin, float radius, F&& callback)
	{
		auto* tes = RE::TES::GetSingleton();
		if (!tes || !origin) return;

		const RE::NiPoint3 center = origin->GetPosition();
		tes->ForEachReferenceInRange(origin, radius, [&](RE::TESObjectREFR* a_ref) -> RE::BSContainer::ForEachResult {
			auto* actor = a_ref ? a_ref->As<RE::Actor>() : nullptr;
			if (!actor || actor->GetPosition().GetDistance(center) > radius) {
				return RE::BSContainer::ForEachResult::kContinue;
			}
			return callback(actor);
		});
	}

	static float RollAmount(const AmountCondition& a_cond, std::mt19937& a_rng)
	{
		if (!a_cond.useRandom) return a_cond.value;
		const float lo = (std::min)(a_cond.min, a_cond.max);
		const float hi = (std::max)(a_cond.min, a_cond.max);
		return std::uniform_real_distribution<float>(lo, hi)(a_rng);
	}

	static float RollRadius(const RadiusCondition& a_cond, std::mt19937& a_rng)
	{
		float r = a_cond.value;
		if (a_cond.useRandom) {
			const float lo = (std::min)(a_cond.min, a_cond.max);
			const float hi = (std::max)(a_cond.min, a_cond.max);
			r = std::uniform_real_distribution<float>(lo, hi)(a_rng);
		}
		return (std::max)(r, 0.0f);
	}

	void ModActorValue(const RuleContext& ctx, const std::vector<ModActorValueData>& avData)
	{
		if (avData.empty()) {
			logger::error("ModActorValue: No actor value data provided");
			return;
		}

		RE::NiPoint3 centerPos;
		if (!ResolveEffectCenter(ctx, centerPos)) {
			logger::error("ModActorValue: No valid position to center the radius search on");
			return;
		}

		static thread_local std::mt19937 rng(std::random_device{}());

		for (const auto& data : avData) {
			if (data.actorValue.empty()) {
				logger::warn("ModActorValue: No actorvalue specified, skipping");
				continue;
			}

			RE::ActorValue av = GetActorValueFromString(data.actorValue);
			if (av == RE::ActorValue::kNone) {
				logger::warn("ModActorValue: Unknown actor value '{}', skipping", data.actorValue);
				continue;
			}

			const float amount = RollAmount(data.amount, rng);
			if (amount == 0.0f) {
				logger::warn("ModActorValue: amount resolved to 0 for '{}' - nothing to do (did the rule set \"amount\"?)", data.actorValue);
				continue;
			}

			const float radius = RollRadius(data.radius, rng);

			std::vector<RE::Actor*> targets;
			CollectActorsInRadius(ctx, centerPos, radius, data.affectSource, data.affectPlayer, targets);

			logger::debug("ModActorValue: av='{}' amount={:.2f} radius={:.1f} center=({:.1f},{:.1f},{:.1f}) actorsFound={}",
				data.actorValue, amount, radius, centerPos.x, centerPos.y, centerPos.z, targets.size());

			if (targets.empty()) continue;

			for (auto* actor : targets) {
				auto* avOwner = actor->AsActorValueOwner();
				if (!avOwner) {
					continue;
				}

				// kDamage is the same pool restored/damaged by potions, spells,
				// and vanilla combat: positive amount heals/restores, negative
				// amount damages/reduces, and it can reduce Health to 0 (i.e.
				// kill the actor) exactly like any other damage source.
				avOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, amount);
			}
		}
	}

	// ╔══════════════════════════════════════════════════════════════════════════╗
	// ║  APPLYFORCE                                                              ║
	// ╚══════════════════════════════════════════════════════════════════════════╝
	//
	// WHY THIS DID NOTHING IN GAME BEFORE
	//
	// The previous version pushed actors by writing a velocity straight into the
	// actor's Havok character controller (bhkCharacterController::SetLinearVelocityImpl).
	// That is effectively what Papyrus' ApplyHavokImpulse does, and it is famously a
	// no-op on a living actor: while an actor is on the ground its character controller
	// re-derives its own velocity from the movement/animation system every single tick,
	// so whatever we wrote is overwritten before it moves anything. Nothing logged an
	// error either, because from the plugin's side the write "succeeded" - which is
	// exactly what the symptom looked like in the log: the effect ran, actorsFound was
	// non-zero, "pushed N of N", and nobody moved.
	//
	// WHAT IT DOES NOW
	//
	// It calls the engine's own knockback routine, AIProcess::KnockExplosion - the same
	// one vanilla explosions and Papyrus' PushActorAway go through. That routine puts
	// the actor into the knocked/ragdoll state FIRST and then applies the impulse, so
	// the push survives the animation system instead of being clamped away by it, and
	// it also works on actors that are already ragdolling and have no character
	// controller at all. Direction is handled by the engine: actors are always pushed
	// away from the origin position handed to it, which here is the impact/interaction
	// point resolved by ResolveEffectCenter.
	//
	// The old velocity write is kept only as a compile-time fallback (see
	// detail::knock_explosion_capable) for CommonLibSSE builds that do not expose
	// KnockExplosion, so this file still builds against them.
	//
	// UNITS. "force" (or "amount") in the JSON stays in the same ballpark as before -
	// game units/second - and is converted to the magnitude scale KnockExplosion wants
	// via kForceToKnockMagnitude. Rough feel: ~150 nudges, ~500 staggers/knocks down,
	// ~1500+ throws actors properly. Values below ~30 will still look like nothing
	// happened; that is the engine's own threshold, not a bug.

	namespace detail
	{
		// True when this CommonLibSSE exposes AIProcess::KnockExplosion with the
		// (actor, origin, magnitude) signature used below.
		template <class P>
		concept knock_explosion_capable = requires(P* a_process, RE::Actor* a_actor, RE::NiPoint3& a_origin, float a_magnitude) {
			a_process->KnockExplosion(a_actor, a_origin, a_magnitude);
		};
	}

	// game units/second (what rules are written in) -> KnockExplosion magnitude scale
	static constexpr float kForceToKnockMagnitude = 1.0f / 150.0f;

	// Pushes a single actor directly away from a_center. Returns true when a push was
	// actually handed to the engine.
	static bool PushActorAwayFrom(RE::Actor* a_actor, const RE::NiPoint3& a_center, float a_force, float a_worldScale)
	{
		if (!a_actor || a_actor->IsDeleted()) return false;

		// No loaded 3D means no physics body to push at all - normal for an actor whose
		// cell is loaded but whose model has not been streamed in yet.
		if (!a_actor->Is3DLoaded()) {
			logger::debug("ApplyForce: actor {:X} has no 3D loaded, skipping", a_actor->GetFormID());
			return false;
		}

		// Respect actors the game itself marks non-pushable (bosses, scripted sequences)
		// exactly like vanilla knockback effects do. An actor with no character
		// controller is NOT skipped here any more - a ragdolling actor has no controller
		// and is still perfectly pushable through KnockExplosion.
		if (auto* controller = a_actor->GetCharController()) {
			if (controller->flags.any(RE::CHARACTER_FLAGS::kNotPushable, RE::CHARACTER_FLAGS::kNotPushablePermanent)) {
				logger::debug("ApplyForce: actor {:X} is flagged not-pushable, skipping", a_actor->GetFormID());
				return false;
			}
		}

		if constexpr (detail::knock_explosion_capable<RE::AIProcess>) {
			if (auto* process = a_actor->GetActorRuntimeData().currentProcess) {
				RE::NiPoint3 origin = a_center;
				const float magnitude = a_force * kForceToKnockMagnitude;
				if (!std::isfinite(magnitude)) return false;
				process->KnockExplosion(a_actor, origin, magnitude);
				return true;
			}
			logger::debug("ApplyForce: actor {:X} has no AI process - falling back to a direct velocity write", a_actor->GetFormID());
		}

		// ── FALLBACK ────────────────────────────────────────────────────────────────
		// Only reached when KnockExplosion is unavailable in this CommonLibSSE build, or
		// the actor has no AI process. Grounded actors may well shrug this off (that is
		// the whole reason for the rewrite above); the upward component is what gives it
		// any chance of surviving the movement system.
		auto* controller = a_actor->GetCharController();
		if (!controller) {
			logger::debug("ApplyForce: actor {:X} has no character controller for the fallback push, skipping", a_actor->GetFormID());
			return false;
		}

		const RE::NiPoint3 actorPos = a_actor->GetPosition();
		const float distance = a_center.GetDistance(actorPos);

		float dirX = 1.0f, dirY = 0.0f;
		if (distance > 1.0f) {
			dirX = (actorPos.x - a_center.x) / distance;
			dirY = (actorPos.y - a_center.y) / distance;
		}

		constexpr float kUpwardLiftRatio = 0.45f;
		const RE::NiPoint3 rawDir{ dirX, dirY, kUpwardLiftRatio };
		const float rawLen = std::sqrt(rawDir.x * rawDir.x + rawDir.y * rawDir.y + rawDir.z * rawDir.z);
		const RE::NiPoint3 pushDir = (rawLen > 0.0001f)
			? RE::NiPoint3{ rawDir.x / rawLen, rawDir.y / rawLen, rawDir.z / rawLen }
			: RE::NiPoint3{ 0.0f, 0.0f, 1.0f };

		const float vx = pushDir.x * a_force * a_worldScale;
		const float vy = pushDir.y * a_force * a_worldScale;
		const float vz = pushDir.z * a_force * a_worldScale;

		if (!std::isfinite(vx) || !std::isfinite(vy) || !std::isfinite(vz)) {
			logger::warn("ApplyForce: computed a non-finite velocity for actor {:X}, skipping", a_actor->GetFormID());
			return false;
		}

		controller->SetLinearVelocityImpl(RE::hkVector4(vx, vy, vz, 0.0f));
		return true;
	}

	void ApplyForce(const RuleContext& ctx, const std::vector<ForceApplyData>& forceData)
	{
		if (forceData.empty()) {
			logger::error("ApplyForce: No force data provided");
			return;
		}

		RE::NiPoint3 centerPos;
		if (!ResolveEffectCenter(ctx, centerPos)) {
			logger::error("ApplyForce: No valid position to center the radius search on");
			return;
		}

		static thread_local std::mt19937 rng(std::random_device{}());

		// Make it loud rather than mysterious if this build had to compile the old
		// (unreliable) path because KnockExplosion wasn't available.
		if constexpr (!detail::knock_explosion_capable<RE::AIProcess>) {
			static bool warnedNoKnockExplosion = false;
			if (!warnedNoKnockExplosion) {
				warnedNoKnockExplosion = true;
				logger::warn("ApplyForce: this CommonLibSSE build does not expose AIProcess::KnockExplosion - using direct velocity writes instead, which grounded actors will often ignore");
			}
		}

		// Only used by the fallback path above. 1/70 is the standard Skyrim
		// world-to-Havok conversion, used if the engine's own value is unavailable.
		float worldScale = RE::bhkWorld::GetWorldScale();
		if (!std::isfinite(worldScale) || worldScale <= 0.0f) {
			worldScale = 0.0142875f;
		}

		for (const auto& data : forceData) {
			const float force = RollAmount(data.force, rng);
			if (force == 0.0f) {
				logger::warn("ApplyForce: force resolved to 0 - nothing to do (did the rule set \"amount\" or \"force\"?)");
				continue;
			}

			const float radius = RollRadius(data.radius, rng);

			std::vector<RE::Actor*> targets;
			CollectActorsInRadius(ctx, centerPos, radius, data.affectSource, data.affectPlayer, targets);

			logger::debug("ApplyForce: force={:.1f} radius={:.1f} center=({:.1f},{:.1f},{:.1f}) actorsFound={}",
				force, radius, centerPos.x, centerPos.y, centerPos.z, targets.size());

			if (targets.empty()) continue;

			int pushed = 0;
			for (auto* actor : targets) {
				if (PushActorAwayFrom(actor, centerPos, force, worldScale)) ++pushed;
			}

			logger::debug("ApplyForce: pushed {} of {} actors found", pushed, targets.size());
		}
	}

    void RemoveItem(const RuleContext& ctx)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("RemoveItem: No target to remove");
            return;
        }

        if (!ctx.target->IsDisabled()) ctx.target->Disable();
        ctx.target->SetDelete(true); 
    }
    
    void DisableItem(const RuleContext& ctx)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("DisableItem: No target to disable");
            return;
        }

        auto* base = ctx.target->GetBaseObject();
        if (!base) {
            logger::error("DisableItem: Target has no base object");
            return;
        }
    
        auto* modInfo = base->GetFile();
        if (!modInfo) {
            logger::error("DisableItem: Cannot get mod info for base object");
            return;
        }
        
		// Workaround for kInintiallyDisabled assigned by the engine to freshly disabled items on cell re-enter
        std::string modName = std::string(modInfo->GetFilename());
        std::uint32_t formID = base->GetLocalFormID();

        std::stringstream ss;
        ss << modName << ":" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << formID;
        std::string identifier = ss.str();
        std::string displayName = "orig:" + identifier;
    
        auto pos = ctx.target->GetPosition();
        auto scale = ctx.target->GetScale();
    
        static RE::TESBoundObject* dummyForm = nullptr;
        if (!dummyForm) {
            auto* dh = RE::TESDataHandler::GetSingleton();
            auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
            dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
        }

        if (!dummyForm) {
            logger::error("DisableItem: Cannot create dummy (required for respawning)");
            return;
        }

		// Preserve ownership while the original is represented by the temporary
		// dummy. Player ownership must be retained even in interior cells. For
		// non-player owners, avoid copying an ambient interior cell/location owner
		// onto the dummy as explicit ownership.
        RE::TESForm* originalOwner = nullptr;
        if (IsPickableItem(ctx.target)) {
            auto* player = RE::PlayerCharacter::GetSingleton();
            auto* playerBase = player ? player->GetActorBase() : nullptr;
            auto* explicitOwnership = ctx.target->extraList.GetByType<RE::ExtraOwnership>();
            auto* owner = explicitOwnership ? explicitOwnership->owner : nullptr;

            // Capture EXPLICIT player ownership before the original reference is
            // disabled/deleted. This is intentionally independent of the interior
            // cell's ambient ownership.
            if (playerBase && (owner == playerBase || owner == player)) {
                originalOwner = playerBase;
            } else if (auto* cell = ctx.target->GetParentCell(); !cell || !cell->IsInteriorCell()) {
                originalOwner = owner ? owner : ctx.target->GetOwner();
            }
        }

        auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
        if (!dummy) {
            logger::error("DisableItem: Failed to create dummy (required for respawning)");
            return;
        }

        if (IsPickableItem(ctx.target)) {
            dummy->SetOwner(originalOwner);
        }
    
        dummy->SetPosition(pos);
        dummy->data.angle = ctx.target->data.angle;
        SetObjectScale(dummy.get(), scale);

        dummy->SetDisplayName(displayName, true);
    
        ctx.target->Disable();
        ctx.target->SetDelete(true);
    }

    void EnableItem(const RuleContext& ctx)
    {
        auto* cell = ctx.target ? ctx.target->GetParentCell() : nullptr;
        if (!cell) {
            logger::error("EnableItem: No cell to search for dummy");
            return;
        }

        RE::TESObjectREFR* foundDummy = nullptr;
        std::string origIdentifier = "";

        static RE::TESBoundObject* dummyForm = nullptr;
        if (!dummyForm) {
            auto* dh = RE::TESDataHandler::GetSingleton();
            auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
            dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
        }

        cell->ForEachReference([&](RE::TESObjectREFR* ref) {
            if (!ref || ref->IsDeleted())
                return RE::BSContainer::ForEachResult::kContinue;

            if (dummyForm && ref->GetBaseObject() != dummyForm) {
                return RE::BSContainer::ForEachResult::kContinue;
            }

            std::string name = ref->GetDisplayFullName() ? ref->GetDisplayFullName() : "";
            if (name.empty() || name.rfind("orig:", 0) != 0) {
                return RE::BSContainer::ForEachResult::kContinue;
            }

            if (ref->IsDeleted()) {
                return RE::BSContainer::ForEachResult::kContinue;
            }

            try {
                origIdentifier = name.substr(5);
                foundDummy = ref;
                return RE::BSContainer::ForEachResult::kStop;
            } catch (...) {
                logger::warn("EnableItem: Failed to parse FormID from dummy DisplayName: {}", name);
                return RE::BSContainer::ForEachResult::kContinue;
            }
        });

        if (!foundDummy) {
            logger::debug("EnableItem: No dummy object found in cell");
            return;
        }

        if (origIdentifier.empty()) {
            logger::error("EnableItem: Couldn't extract original FormID from dummy name");
            return;
        }

        RE::TESForm* origForm = RuleManager::GetFormFromIdentifier<RE::TESForm>(origIdentifier);
        if (!origForm) {
            logger::error("EnableItem: Failed to lookup original form by identifier '{}'", origIdentifier);
            return;
        }

        auto pos = foundDummy->GetPosition();
        auto scale = foundDummy->GetScale();

        auto orig = foundDummy->PlaceObjectAtMe(origForm ? origForm->As<RE::TESBoundObject>() : nullptr, true);
        if (!orig) {
            logger::error("EnableItem: Failed to recreate original object");
            return;
        }

		// The recreated replacement must not inherit cell/reference ownership.
		// Otherwise picking it up can be treated as stealing.
        NormalizePickableOwnership(orig.get(), foundDummy);

        orig->SetPosition(pos);
        orig->data.angle = foundDummy->data.angle;
        SetObjectScale(orig.get(), scale);

        foundDummy->Disable();
        foundDummy->SetDelete(true);
    }

	void RemoveLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("RemoveLight: No target to remove light around");
			return;
		}

		if (lightsData.empty()) {
			logger::warn("RemoveLight: No lights to remove");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("RemoveLight: TES singleton is null");
			return;
		}

		for (const auto& lightData : lightsData) {
			if (lightData.radius.value <= 0) continue;

			tes->ForEachReferenceInRange(ctx.target, lightData.radius.value, [&](RE::TESObjectREFR* ref) {
				if (!ref || ref->IsDisabled() || ref->IsDeleted()) return RE::BSContainer::ForEachResult::kContinue;

				auto* baseObj = ref->GetBaseObject();
				if (ref && baseObj && baseObj->Is(RE::FormType::Light)) {
					SKSE::GetTaskInterface()->AddTask([refHandle = ref->CreateRefHandle()]() {
						if (auto ref = refHandle.get(); ref && !ref->IsDeleted()) {
							if (!ref->IsDisabled()) ref->Disable();
							ref->DeleteThis();
						}
					});
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
	}

	void DisableLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("DisableLight: No target to disable light around");
			return;
		}

		if (lightsData.empty()) {
			logger::warn("DisableLight: No lights to disable");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("DisableLight: TES singleton is null");
			return;
		}

		for (const auto& lightData : lightsData) {
			if (lightData.radius.value <= 0) continue;

			tes->ForEachReferenceInRange(ctx.target, lightData.radius.value, [&](RE::TESObjectREFR* ref) {
				if (!ref || ref->IsDisabled() || ref->IsDeleted()) return RE::BSContainer::ForEachResult::kContinue;

				auto* baseObj = ref->GetBaseObject();
				if (ref && baseObj && baseObj->Is(RE::FormType::Light)) {
					SKSE::GetTaskInterface()->AddTask([refHandle = ref->CreateRefHandle()]() {
						if (auto ref = refHandle.get(); ref && !ref->IsDeleted()) {
							ref->Disable();
						}
					});
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
	}

	void EnableLight(const RuleContext& ctx, const std::vector<LightRemoveData>& lightsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("EnableLight: No target to enable light around");
			return;
		}

		if (lightsData.empty()) {
			logger::warn("EnableLight: No lights to enable");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("EnableLight: TES singleton is null");
			return;
		}

		for (const auto& lightData : lightsData) {
			if (lightData.radius.value <= 0) continue;

			tes->ForEachReferenceInRange(ctx.target, lightData.radius.value, [&](RE::TESObjectREFR* ref) {
				if (!ref || ref->IsDeleted())
					return RE::BSContainer::ForEachResult::kContinue;

				auto* baseObj = ref->GetBaseObject();
				if (ref && baseObj && baseObj->Is(RE::FormType::Light)) {
					if (ref->IsDisabled())
						ref->Enable(false);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
	}

// ╔════════════════════════════════════╗
// ║             INVENTORY              ║
// ╚════════════════════════════════════╝

	void SpillInventory(const RuleContext& ctx)
	{
		auto* containerRef = ctx.target;
		if (!containerRef || containerRef->IsDeleted()) {
			logger::error("SpillInventory: No target to spill inventory");
			return;
		}

		auto* baseObj = containerRef->GetBaseObject();
		if (!baseObj || (!baseObj->Is(RE::FormType::Container))) {
			return;
		}

		NiPoint3 dropPos;
		if (auto* root = containerRef->Get3D()) {
			dropPos = root->worldBound.center;
		} else {
			const auto& bmin = containerRef->GetBoundMin();
			const auto& bmax = containerRef->GetBoundMax();
			dropPos = NiPoint3{
				(bmin.x + bmax.x) * 0.5f,
				(bmin.y + bmax.y) * 0.5f,
				(bmin.z + bmax.z) * 0.5f
			};
		}

		NiPoint3 dropAngle = containerRef->GetAngle();

		for (auto& [obj, data] : containerRef->GetInventory()) {
			if (!obj) {
				logger::warn("SpillInventory: Invalid inventory object for target {}", containerRef ? containerRef->GetFormID() : 0);
				continue;
			} else if (obj && (data.first > 0)) {
				containerRef->RemoveItem(obj, data.first, ITEM_REMOVE_REASON::kDropping, nullptr, containerRef, &dropPos, &dropAngle);
			}
		}
	}

	void AddContainerItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("AddInventoryItem: No target to add items to");
			return;
		}

		if (itemsData.empty()) {
			logger::error("AddInventoryItem: No items to add");
			return;
		}

		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			ctx.target->AddObjectToContainer(itemData.item, nullptr, itemData.count.value, ctx.target);
		}
	}

	void AddActorItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("AddActorItem: No target to add items from");
			return;
		}

		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("AddActorItem: No source actor to add items to");
			return;
		}

		if (itemsData.empty()) {
			logger::error("AddActorItem: No items to add");
			return;
		}

		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			ctx.source->AddObjectToContainer(itemData.item, nullptr, itemData.count.value, ctx.target);
		}
	}

	void RemoveContainerItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("RemoveContainerItem: No target to remove items from");
			return;
		}
		if (itemsData.empty()) {
			logger::error("RemoveContainerItem: No items to remove");
			return;
		}

		auto inventory = ctx.target->GetInventory();
		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			auto it = inventory.find(itemData.item);
			if (it == inventory.end() || it->second.first <= 0) continue;
			std::int32_t removecount = std::min<int32_t>(itemData.count.value, it->second.first);

			ctx.target->RemoveItem(itemData.item, removecount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
		}
	}

	void RemoveActorItem(const RuleContext& ctx, const std::vector<InventoryData>& itemsData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("RemoveActorItem: No source actor to remove items from");
			return;
		}
		if (itemsData.empty()) {
			logger::error("RemoveActorItem: No items to remove");
			return;
		}

		auto inventory = ctx.source->GetInventory();
		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			auto it = inventory.find(itemData.item);
			if (it == inventory.end() || it->second.first <= 0) continue;
			std::int32_t removecount = std::min<int32_t>(itemData.count.value, it->second.first);

			ctx.source->RemoveItem(itemData.item, removecount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
		}
	}

// ╔════════════════════════════════════╗
// ║           ITEMS CREATION           ║
// ╚════════════════════════════════════╝

    void SpawnItem(const RuleContext& ctx, const std::vector<ItemSpawnData>& itemsData)
    {
        const bool isGroundHit = !ctx.target;

        if (!isGroundHit && ctx.target->IsDeleted()) {
            logger::error("SpawnItem: No target to spawn items");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("SpawnItem: Ground hit with no impact position");
            return;
        }

        if (itemsData.empty()) {
            logger::error("SpawnItem: No items to spawn");
            return;
        }

        RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
        if (isGroundHit && !anchor) {
            logger::error("SpawnItem: No anchor actor available for ground hit");
            return;
        }

        for (const auto& itemData : itemsData) {
            if (!itemData.item)
                continue;

            for (std::uint32_t i = 0; i < itemData.count.value; ++i) {
                if (isGroundHit) {
                    auto item = SpawnAtPosition(anchor, itemData.item, itemData.fade, ctx.hitPos);
                    if (item) {
                        NormalizePickableOwnership(item.get(), anchor);
                        SetObjectScale(item.get(), itemData.scale.value == -1.0f ? 1.0f : itemData.scale.value);
                    }
                } else {
                    auto item = Spawn(ctx.target, itemData.item, itemData.spawnType, itemData.fade, itemData.string, &ctx);
                    if (item && ctx.target) {
                        NormalizePickableOwnership(item.get(), ctx.target);
                        if (itemData.scale.value == -1.0f) {
                            SetObjectScale(item.get(), ctx.target->GetScale());
                        } else {
                            SetObjectScale(item.get(), itemData.scale.value);
                        }
                    }
                }
            }
        }
    }

	void SwapItem(const RuleContext& ctx, const std::vector<ItemSpawnData>& itemsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SwapItem: No target to swap items");
			return;
		}

		if (itemsData.empty()) {
			logger::error("SwapItem: No items to swap with");
			return;
		}

		bool anyItemSpawned = false;

		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			for (std::uint32_t i = 0; i < itemData.count.value; ++i) {
				// applyGroundClamp=false: a straight swap should put the replacement
				// exactly where the original item was, full stop - not run it through
				// ClampAboveGround's surface-detection/repositioning at all. Explicitly
				// re-setting position/angle to ctx.target's own below is belt-and-suspenders
				// on top of that: PlaceObjectAtMe already places at the caller's position/
				// angle for spawnType 0 (the common case), but this guarantees it
				// regardless of spawnType, matching the original pre-clamp behavior exactly.
				auto item = Spawn(ctx.target, itemData.item, itemData.spawnType, itemData.fade, itemData.string, &ctx, false);
				if (item && ctx.target) {
					anyItemSpawned = true;
					try {
						item->SetPosition(ctx.target->GetPosition());
						item->data.angle = ctx.target->data.angle;
					} catch (...) {
						logger::error("SwapItem: Exception while matching original item's position/angle");
					}
					NormalizePickableOwnership(item.get(), ctx.target);
					if (itemData.scale.value == -1.0f) {
						SetObjectScale(item.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(item.get(), itemData.scale.value);
					}
				}
			}
		}

		if (anyItemSpawned && ctx.target) {
			for (const auto& itemData : itemsData) {
				if (ctx.target) {
					if (itemData.nonDeletable) {
						DisableItem(ctx);
					} else {
						if (!ctx.target->IsDisabled())
							ctx.target->Disable();
						ctx.target->SetDelete(true);
					}
				}
			}
		}
	}

	void SpawnLeveledItem(const RuleContext& ctx, const std::vector<LvlItemSpawnData>& itemsData)
	{
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::error("SpawnLeveledItem: No target to spawn leveled items");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::error("SpawnLeveledItem: Ground hit with no impact position");
			return;
		}

		if (itemsData.empty()) {
			logger::error("SpawnLeveledItem: No leveled items to spawn with");
			return;
		}

		RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
		if (isGroundHit && !anchor) {
			logger::error("SpawnLeveledItem: No anchor actor available for ground hit");
			return;
		}

		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			for (std::uint32_t i = 0; i < itemData.count.value; ++i) {
				auto* obj = ResolveLeveledItem(itemData.item);
				if (!obj) {
					logger::warn("SpawnLeveledItem: can't resolve LVLI {:X}", itemData.item ? itemData.item->GetFormID() : 0);
					continue;
				}

				if (isGroundHit) {
					auto item = SpawnAtPosition(anchor, obj, itemData.fade, ctx.hitPos);
					if (item) {
						NormalizePickableOwnership(item.get(), anchor);
						SetObjectScale(item.get(), itemData.scale.value == -1.0f ? 1.0f : itemData.scale.value);
					} else {
						logger::warn("SpawnLeveledItem: place failed for {:X}", obj->GetFormID());
					}
				} else {
					auto item = Spawn(ctx.target, obj, itemData.spawnType, itemData.fade, itemData.string, &ctx);
					if (item && ctx.target) {
						NormalizePickableOwnership(item.get(), ctx.target);
						if (itemData.scale.value == -1.0f) {
							SetObjectScale(item.get(), ctx.target->GetScale());
						} else {
							SetObjectScale(item.get(), itemData.scale.value);
						}
					} else {
						logger::warn("SpawnLeveledItem: place failed for {:X}", obj->GetFormID());
					}
				}
			}
		}
	}

	void SwapLeveledItem(const RuleContext& ctx, const std::vector<LvlItemSpawnData>& itemsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SwapLeveledItem: No target to swap leveled items");
			return;
		}

		if (itemsData.empty()) {
			logger::error("SwapLeveledItem: No leveled items to swap with");
			return;
		}

		bool spawned = false;

		for (const auto& itemData : itemsData) {
			if (!itemData.item) continue;

			for (std::uint32_t i = 0; i < itemData.count.value; ++i) {
				auto* obj = ResolveLeveledItem(itemData.item);
				if (!obj) {
					logger::warn("SwapLeveledItem: can't resolve LVLI {:X}", itemData.item ? itemData.item->GetFormID() : 0);
					continue;
				}

				auto item = Spawn(ctx.target, obj, itemData.spawnType, itemData.fade, itemData.string, &ctx);
				if (item && ctx.target) {
					spawned = true;
					NormalizePickableOwnership(item.get(), ctx.target);
					if (itemData.scale.value == -1.0f) {
						SetObjectScale(item.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(item.get(), itemData.scale.value);
					}
				} else {
					logger::warn("SwapLeveledItem: place failed for {:X}", obj ? obj->GetFormID() : 0);
				}
			}
		}

		if (spawned && ctx.target) {
			for (const auto& itemData : itemsData) {
				if (ctx.target) {
					if (itemData.nonDeletable) {
						DisableItem(ctx);
					} else {
						if (!ctx.target->IsDisabled())
							ctx.target->Disable();
						ctx.target->SetDelete(true);
					}
				}
			}
		}
	}

	void SpawnLight(const RuleContext& ctx, const std::vector<LightSpawnData>& lightsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SpawnLight: No target to spawn lights");
			return;
		}

		if (lightsData.empty()) {
			logger::error("SpawnLight: No lights to spawn");
			return;
		}

		for (const auto& lightData : lightsData) {
			if (!lightData.light) continue;

			for (std::uint32_t i = 0; i < lightData.count.value; ++i) {
				auto light = Spawn(ctx.target, lightData.light, lightData.spawnType, lightData.fade, lightData.string, &ctx);
				if (light && ctx.target) {
					light->Enable(false);
					if (lightData.scale.value == -1.0f) {
						SetObjectScale(light.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(light.get(), lightData.scale.value);
					}
				}
			}
		}
	}

// ╔════════════════════════════════════╗
// ║          ACTORS CREATION           ║
// ╚════════════════════════════════════╝

	void SpawnActor(const RuleContext& ctx, const std::vector<ActorSpawnData>& actorsData)
	{
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::error("SpawnActor: No target to spawn actors");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::error("SpawnActor: Ground hit with no impact position");
			return;
		}

		if (actorsData.empty()) {
			logger::error("SpawnActor: No actors to spawn");
			return;
		}

		RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
		if (isGroundHit && !anchor) {
			logger::error("SpawnActor: No anchor actor available for ground hit");
			return;
		}

		for (const auto& actorData : actorsData) {
			if (!actorData.npc) continue;

			for (std::uint32_t i = 0; i < actorData.count.value; ++i) {
				if (isGroundHit) {
					auto actor = SpawnAtPosition(anchor, actorData.npc, actorData.fade, ctx.hitPos);
					if (actor) {
						SetObjectScale(actor.get(), actorData.scale.value == -1.0f ? 1.0f : actorData.scale.value);
					}
				} else {
					auto actor = Spawn(ctx.target, actorData.npc, actorData.spawnType, actorData.fade, actorData.string, &ctx);
					if (actor && ctx.target) {
						if (actorData.scale.value == -1.0f) {
							SetObjectScale(actor.get(), ctx.target->GetScale());
						} else {
							SetObjectScale(actor.get(), actorData.scale.value);
						}
					}
				}
			}
		}
	}

	void SwapActor(const RuleContext& ctx, const std::vector<ActorSpawnData>& actorsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SwapActor: No target to swap actors");
			return;
		}

		if (actorsData.empty()) {
			logger::error("SwapActor: No actors to swap with");
			return;
		}

		bool anyActorSpawned = false;

		for (const auto& actorData : actorsData) {
			if (!actorData.npc) continue;

			for (std::uint32_t i = 0; i < actorData.count.value; ++i) {
				auto actor = Spawn(ctx.target, actorData.npc, actorData.spawnType, actorData.fade, actorData.string, &ctx);
				if (actor && ctx.target) {
					anyActorSpawned = true;
					if (actorData.scale.value == -1.0f) {
						SetObjectScale(actor.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(actor.get(), actorData.scale.value);
					}
				}
			}
		}

		if (anyActorSpawned && ctx.target) {
			for (const auto& itemData : actorsData) {
				if (ctx.target) {
					if (itemData.nonDeletable) {
						DisableItem(ctx);
					} else {
						if (!ctx.target->IsDisabled())
							ctx.target->Disable();
						ctx.target->SetDelete(true);
					}
				}
			}
		}
	}

	void SpawnLeveledActor(const RuleContext& ctx, const std::vector<LvlActorSpawnData>& actorsData)
	{
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::error("SpawnLeveledActor: No target to spawn actors");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::error("SpawnLeveledActor: Ground hit with no impact position");
			return;
		}

		if (actorsData.empty()) {
			logger::error("SpawnLeveledActor: No actors to spawn");
			return;
		}

		RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
		if (isGroundHit && !anchor) {
			logger::error("SpawnLeveledActor: No anchor actor available for ground hit");
			return;
		}

		for (const auto& actorData : actorsData) {
			if (!actorData.npc) continue;

			for (std::uint32_t i = 0; i < actorData.count.value; ++i) {
				auto* npcBase = ResolveLeveledNPC(actorData.npc);
				if (!npcBase) {
					logger::warn("SpawnLeveledActor: Can't resolve LVLC {:X}", actorData.npc ? actorData.npc->GetFormID() : 0);
					continue;
				}

				if (isGroundHit) {
					auto actor = SpawnAtPosition(anchor, npcBase, actorData.fade, ctx.hitPos);
					if (actor) {
						SetObjectScale(actor.get(), actorData.scale.value == -1.0f ? 1.0f : actorData.scale.value);
					}
					continue;
				}

				auto actor = Spawn(ctx.target, npcBase, actorData.spawnType, actorData.fade, actorData.string, &ctx);
				if (actor && ctx.target) {
					if (actorData.scale.value == -1.0f) {
						SetObjectScale(actor.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(actor.get(), actorData.scale.value);
					}
				}
			}
		}
	}

	void SwapLeveledActor(const RuleContext& ctx, const std::vector<LvlActorSpawnData>& actorsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SwapLeveledActor: No target to swap actors with");
			return;
		}

		if (actorsData.empty()) {
			logger::error("SwapLeveledActor: No actors to swap with");
			return;
		}

		bool spawned = false;

		for (const auto& actorData : actorsData) {
			if (!actorData.npc) continue;

			for (std::uint32_t i = 0; i < actorData.count.value; ++i) {
				auto* npcBase = ResolveLeveledNPC(actorData.npc);
				if (!npcBase) {
					logger::warn("SpawnLeveledActor: Can't resolve LVLC {:X}", actorData.npc ? actorData.npc->GetFormID() : 0);
					continue;
				}

				auto actor = Spawn(ctx.target, npcBase, actorData.spawnType, actorData.fade, actorData.string, &ctx);
				if (actor && ctx.target) {
					spawned = true;
					if (actorData.scale.value == -1.0f) {
						SetObjectScale(actor.get(), ctx.target->GetScale());
					} else {
						SetObjectScale(actor.get(), actorData.scale.value);
					}
				}
			}
		}

		if (spawned && ctx.target) {
			for (const auto& itemData : actorsData) {
				if (ctx.target) {
					if (itemData.nonDeletable) {
						DisableItem(ctx);
					} else {
						if (!ctx.target->IsDisabled()) ctx.target->Disable();
						ctx.target->SetDelete(true);
					}
				}
			}
		}
	}

// ╔════════════════════════════════════╗
// ║           MAGIC EFFECTS            ║
// ╚════════════════════════════════════╝
	
	void SpawnSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData)
	{
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::warn("SpawnSpell: No valid target location for dummy caster");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::warn("SpawnSpell: Ground hit with no impact position");
			return;
		}

		if (spellsData.empty()) {
			logger::error("SpawnSpell: No spells to spawn");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("SpawnSpell: Cannot create dummy caster");
			return;
		}

		RE::NiPointer<RE::TESObjectREFR> dummy;
		if (isGroundHit) {
			auto* anchor = ResolveGroundHitAnchor(ctx);
			if (!anchor) {
				logger::error("SpawnSpell: No anchor actor available for ground hit");
				return;
			}
			dummy = SpawnAtPosition(anchor, dummyForm, 0, ctx.hitPos);
		} else {
			dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
			if (dummy) {
				if (auto targetObject = ctx.target->Get3D()) {
					if (auto node = targetObject->AsNode()) {
						dummy->MoveToNode(ctx.target, node);
					}
				}
			}
		}

		if (!dummy) {
			logger::error("SpawnSpell: Failed to create dummy caster");
			return;
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("SpawnSpell: Dummy has no MagicCaster");
			dummy->Disable();
			dummy->SetDelete(true);
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("SpawnLeveledSpell: Cannot get TES singleton");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
				return;
			}
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell) continue;
			if (spellData.radius.value <= 0) return;

			std::vector<RE::Actor*> targets;

			// See ForEachActorInSphere's comment: dummy is already spawned exactly at
			// the target/impact position, so its own position IS the center to search
			// around - this fixes "radius" here from whatever raw shape
			// TES::ForEachReferenceInRange happens to return into an exact 3D sphere.
			ForEachActorInSphere(dummy.get(), spellData.radius.value, [&](RE::Actor* actor) {
				if (!actor->IsDead() && !actor->IsDisabled()) {
					targets.push_back(actor);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			if (targets.empty()) continue;

			for (auto* tgt : targets) {
				for (std::uint32_t i = 0; i < spellData.count.value; ++i) {
					if (!mc || !tgt || tgt->IsDeleted() || !spellData.spell) {
						logger::warn("SpawnSpell: Invalid MagicCaster, target, or spell");
						continue;
					}
					mc->CastSpellImmediate(spellData.spell, false, tgt, 1.0f, false, 0.0f, nullptr);
				}
			}
		}

		if (!dummy) {
			logger::error("SpawnSpell: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void SpawnSpellOnItem(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::warn("SpawnSpellOnItem: No valid target to cast the spell on");
			return;
		}

		if (spellsData.empty()) {
			logger::error("SpawnSpellOnItem: No spells to spawn");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("SpawnSpellOnItem: Cannot create dummy for spell casting");
			return;
		}

		auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
		if (!dummy) {
			logger::error("SpawnSpellOnItem: Failed to create dummy");
			return;
		}

		if (auto targetObject = ctx.target->Get3D()) {
			if (auto node = targetObject->AsNode()) {
				dummy->MoveToNode(ctx.target, node);
			}
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("SpawnSpellOnItem: Dummy has no MagicCaster");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
			}
			return;
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell) continue;

			for (std::uint32_t i = 0; i < spellData.count.value; ++i) {
				if (!mc || !ctx.target || ctx.target->IsDeleted() || !spellData.spell) {
					logger::warn("SpawnSpellOnItem: Invalid MagicCaster, target, or spell");
					continue;
				}
				mc->CastSpellImmediate(spellData.spell, false, ctx.target, 1.0f, false, 0.0f, nullptr);
			}
		}

		if (!dummy) {
			logger::error("SpawnSpellOnItem: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void SpawnLeveledSpell(const RuleContext& ctx, const std::vector<LvlSpellSpawnData>& spellsData)
	{
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::warn("SpawnLeveledSpell: No valid target location for dummy caster");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::warn("SpawnLeveledSpell: Ground hit with no impact position");
			return;
		}

		if (spellsData.empty()) {
			logger::error("SpawnLeveledSpell: No spells to spawn");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("SpawnLeveledSpell: Cannot create dummy caster");
			return;
		}

		RE::NiPointer<RE::TESObjectREFR> dummy;
		if (isGroundHit) {
			auto* anchor = ResolveGroundHitAnchor(ctx);
			if (!anchor) {
				logger::error("SpawnLeveledSpell: No anchor actor available for ground hit");
				return;
			}
			dummy = SpawnAtPosition(anchor, dummyForm, 0, ctx.hitPos);
		} else {
			dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
			if (dummy) {
				if (auto targetObject = ctx.target->Get3D()) {
					if (auto node = targetObject->AsNode()) {
						dummy->MoveToNode(ctx.target, node);
					}
				}
			}
		}

		if (!dummy) {
			logger::error("SpawnLeveledSpell: Failed to create dummy caster");
			return;
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("SpawnLeveledSpell: Dummy has no MagicCaster");
			dummy->Disable();
			dummy->SetDelete(true);
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("SpawnLeveledSpell: Cannot get TES singleton");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
				return;
			}
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell)
				continue;
			if (spellData.radius.value <= 0)
				return;

			std::vector<RE::Actor*> targets;

			// See ForEachActorInSphere's comment: dummy is already spawned exactly at
			// the target/impact position, so its own position IS the center to search
			// around - this fixes "radius" here from whatever raw shape
			// TES::ForEachReferenceInRange happens to return into an exact 3D sphere.
			ForEachActorInSphere(dummy.get(), spellData.radius.value, [&](RE::Actor* actor) {
				if (!actor->IsDead() && !actor->IsDisabled()) {
					targets.push_back(actor);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			if (targets.empty()) {
				continue;
			}

			auto* spell = ResolveLeveledSpell(spellData.spell);
			if (!spell) {
				logger::warn("SpawnLeveledSpell: Can't resolve LVLS {:X}", spellData.spell ? spellData.spell->GetFormID() : 0);
				continue;
			}

			for (auto* tgt : targets) {
				for (std::uint32_t i = 0; i < spellData.count.value; ++i) {
					if (!mc || !tgt || tgt->IsDeleted() || !spell) {
						logger::warn("SpawnLeveledSpell: Invalid MagicCaster, target, or spell");
						continue;
					}
					mc->CastSpellImmediate(spell, false, tgt, 1.0f, false, 0.0f, nullptr);
				}
			}
		}

		if (!dummy) {
			logger::error("SpawnLeveledSpell: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void SpawnLeveledSpellOnItem(const RuleContext& ctx, const std::vector<LvlSpellSpawnData>& spellsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::warn("SpawnLeveledSpellOnItem: No valid target to cast the spell on");
			return;
		}

		if (spellsData.empty()) {
			logger::error("SpawnLeveledSpellOnItem: No spells to spawn");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("SpawnLeveledSpellOnItem: Cannot create dummy for spell casting");
			return;
		}

		auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
		if (!dummy) {
			logger::error("SpawnLeveledSpellOnItem: Failed to create dummy");
			return;
		}

		if (auto targetObject = ctx.target->Get3D()) {
			if (auto node = targetObject->AsNode()) {
				dummy->MoveToNode(ctx.target, node);
			}
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("SpawnLeveledSpellOnItem: Dummy has no MagicCaster");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
			}
			return;
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell)
				continue;

			for (std::uint32_t i = 0; i < spellData.count.value; ++i) {
				auto* spell = ResolveLeveledSpell(spellData.spell);
				if (!spell) {
					logger::warn("SpawnLeveledSpellOnItem: Can't resolve LVLS {:X}", spellData.spell ? spellData.spell->GetFormID() : 0);
					continue;
				}
				if (!mc || !ctx.target || ctx.target->IsDeleted() || !spell) {
					logger::warn("SpawnLeveledSpellOnItem: Invalid MagicCaster, target, or spell");
					continue;
				}
				mc->CastSpellImmediate(spell, false, ctx.target, 1.0f, false, 0.0f, nullptr);
			}
		}

		if (!dummy) {
			logger::error("SpawnLeveledSpellOnItem: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void ApplySpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::warn("ApplySpell: No valid target location for dummy caster");
			return;
		}

		if (spellsData.empty()) {
			logger::error("ApplySpell: No ingestibles to apply");
			return;
		}

		auto* baseObject = ctx.target->GetBaseObject();
		if (!baseObject) {
			logger::error("ApplySpell: Target has no base object");
			return;
		}

		RE::MagicItem* magicItem = nullptr;

		if (auto scroll = baseObject->As<RE::ScrollItem>()) {
			magicItem = scroll;
		} else if (auto book = baseObject->As<RE::TESObjectBOOK>(); book && book->TeachesSpell()) {
			magicItem = book->GetSpell();
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("ApplySpell: Cannot create dummy caster");
			return;
		}

		auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
		if (!dummy) {
			logger::error("ApplySpell: Failed to create dummy caster");
			return;
		}

		if (auto targetObject = ctx.target->Get3D()) {
			if (auto node = targetObject->AsNode()) {
				dummy->MoveToNode(ctx.target, node);
			}
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("ApplySpell: Dummy has no MagicCaster");
			dummy->Disable();
			dummy->SetDelete(true);
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("ApplySpell: TES singleton is null");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
				return;
			}
		}

		const auto& data = spellsData[0];
		if (data.radius.value <= 0) return;

		const bool hostile = magicItem->IsHostile();
		std::vector<RE::Actor*> targets;

		// See ForEachActorInSphere's comment: dummy is already spawned exactly at the
		// target/impact position, so its own position IS the center to search around -
		// this fixes "radius" here into an exact 3D sphere.
		ForEachActorInSphere(dummy.get(), data.radius.value, [&](RE::Actor* actor) {
			if (!actor->IsDisabled() && !actor->IsDeleted() && !actor->IsDead() && !actor->IsGhost()) {
				targets.push_back(actor);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		if (targets.empty()) {
		} else {
			for (auto* actor : targets) {
				if (!mc || !actor || actor->IsDeleted() || !magicItem) {
					logger::warn("ApplySpell: Invalid MagicCaster, target, or magic item");
					continue;
				}
				mc->CastSpellImmediate(magicItem, false, actor, 1.0f, hostile, 0.0f, nullptr);
			}
		}

		if (!dummy) {
			logger::error("ApplySpell: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void ApplyIngestible(const RuleContext& ctx, const std::vector<IngestibleApplyData>& ingestiblesData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::warn("ApplyIngestible: No valid target location for dummy caster");
			return;
		}

		if (ingestiblesData.empty()) {
			logger::error("ApplyOtherIngestible: No ingestibles to apply");
			return;
		}

		auto* baseObject = ctx.target->GetBaseObject();
		if (!baseObject) {
			logger::error("ApplyIngestible: Target has no base object");
			return;
		}

		auto* magicItem = baseObject->As<RE::MagicItem>();
		if (!magicItem) {
			logger::warn("ApplyIngestible: The target is not a MagicItem");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("ApplyIngestible: Cannot create dummy caster");
			return;
		}

		auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
		if (!dummy) {
			logger::error("ApplyIngestible: Failed to create dummy caster");
			return;
		}

		if (auto targetObject = ctx.target->Get3D()) {
			if (auto node = targetObject->AsNode()) {
				dummy->MoveToNode(ctx.target, node);
			}
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("ApplyIngestible: Dummy has no MagicCaster");
			dummy->Disable();
			dummy->SetDelete(true);
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("ApplyIngestible: TES singleton is null");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
				return;
			}
		}

		const auto& data = ingestiblesData[0];
		if (data.radius.value <= 0) return;

		const bool hostile = magicItem->IsPoison();
		std::vector<RE::Actor*> targets;

		// See ForEachActorInSphere's comment: dummy is already spawned exactly at the
		// target/impact position, so its own position IS the center to search around -
		// this fixes "radius" here into an exact 3D sphere.
		ForEachActorInSphere(dummy.get(), data.radius.value, [&](RE::Actor* actor) {
			if (!actor->IsDisabled() && !actor->IsDeleted() && !actor->IsDead() && !actor->IsGhost()) {
				targets.push_back(actor);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		if (targets.empty()) {
		} else {
			for (auto* actor : targets) {
				if (!mc || !actor || actor->IsDeleted() || !magicItem) {
					logger::warn("ApplyIngestible: Invalid MagicCaster, target, or magic item");
					continue;
				}
				mc->CastSpellImmediate(magicItem, false, actor, 1.0f, hostile, 0.0f, nullptr);
			}
		}

		if (!dummy) {
			logger::error("ApplyIngestible: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void ApplyOtherIngestible(const RuleContext& ctx, const std::vector<IngestibleApplyData>& ingestiblesData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::warn("ApplyOtherIngestible: No valid target location for dummy caster");
			return;
		}

		if (ingestiblesData.empty()) {
			logger::error("ApplyOtherIngestible: No ingestibles to apply");
			return;
		}

		static RE::TESBoundObject* dummyForm = nullptr;
		if (!dummyForm) {
			auto* dh = RE::TESDataHandler::GetSingleton();
			auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
			dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
		}

		if (!dummyForm) {
			logger::error("ApplyOtherIngestible: Cannot create dummy caster");
			return;
		}

		auto dummy = ctx.target->PlaceObjectAtMe(dummyForm, true);
		if (!dummy) {
			logger::error("ApplyOtherIngestible: Failed to create dummy caster");
			return;
		}

		if (auto targetObject = ctx.target->Get3D()) {
			if (auto node = targetObject->AsNode()) {
				dummy->MoveToNode(ctx.target, node);
			}
		}

		auto* mc = dummy->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!mc) {
			logger::error("ApplyOtherIngestible: Dummy has no MagicCaster");
			dummy->Disable();
			dummy->SetDelete(true);
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("ApplyIngestible: TES singleton is null");
			if (dummy) {
				dummy->Disable();
				dummy->SetDelete(true);
				return;
			}
		}

		for (const auto& ingestibleData : ingestiblesData) {
			if (!ingestibleData.ingestible) continue;
			if (ingestibleData.radius.value <= 0) continue;

			std::vector<RE::Actor*> targets;

			// See ForEachActorInSphere's comment: dummy is already spawned exactly at
			// the target/impact position, so its own position IS the center to search
			// around - this fixes "radius" here into an exact 3D sphere.
			ForEachActorInSphere(dummy.get(), ingestibleData.radius.value, [&](RE::Actor* actor) {
				if (!actor->IsDisabled() && !actor->IsDeleted() && !actor->IsDead() && !actor->IsGhost()) {
					targets.push_back(actor);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			if (targets.empty()) continue;

			const bool hostile = ingestibleData.ingestible->IsPoison();

			for (auto* tgt : targets) {
				for (std::uint32_t i = 0; i < ingestibleData.count.value; ++i) {
					if (!mc || !tgt || tgt->IsDeleted() || !ingestibleData.ingestible) {
						logger::warn("ApplyOtherIngestible: Invalid MagicCaster, target, or ingestible");
						continue;
					}
					mc->CastSpellImmediate(ingestibleData.ingestible, false, tgt, 1.0f, hostile, 0.0f, nullptr);
				}
			}
		}

		if (!dummy) {
			logger::error("ApplyOtherIngestible: Dummy caster is null after casting");
			return;
		}

		dummy->Disable();
		dummy->SetDelete(true);
	}

	void AddActorSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("AddActorSpell: No source actor to add spells to");
			return;
		}

		if (spellsData.empty()) {
			logger::error("AddActorSpell: No spells to add");
			return;
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell) continue;

			if (!ctx.source->HasSpell(spellData.spell)) {
				ctx.source->AddSpell(spellData.spell);
			}
		}
	}

	void RemoveActorSpell(const RuleContext& ctx, const std::vector<SpellSpawnData>& spellsData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("RemoveActorSpell: No source actor to remove spells from");
			return;
		}

		if (spellsData.empty()) {
			logger::error("RemoveActorSpell: No spells to remove");
			return;
		}

		for (const auto& spellData : spellsData) {
			if (!spellData.spell) continue;

			if (ctx.source->HasSpell(spellData.spell)) {
				ctx.source->RemoveSpell(spellData.spell);
			}
		}
	}

	void AddActorPerk(const RuleContext& ctx, const std::vector<PerkData>& perksData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("AddActorPerk: No source actor to add perks to");
			return;
		}

		auto* npc = ctx.source->GetActorBase();
		if (!npc)
			return;

		if (perksData.empty()) {
			logger::error("AddActorPerk: No perks to add");
			return;
		}

		for (const auto& perkData : perksData) {
			if (!perkData.perk) continue;

			std::int8_t currentRank = -1;
			if (auto idx = npc->GetPerkIndex(perkData.perk); idx) {
				const auto& perks = npc->perks;
				if (perks) {
					currentRank = perks[*idx].currentRank;
				}
			}

			if (currentRank < 0 || currentRank < static_cast<std::int8_t>(perkData.rank)) {
				ctx.source->AddPerk(perkData.perk, perkData.rank);
			}
		}
	}

	void RemoveActorPerk(const RuleContext& ctx, const std::vector<PerkData>& perksData)
	{
		if (!ctx.source || ctx.source->IsDeleted()) {
			logger::error("RemoveActorPerk: No source actor to remove perks from");
			return;
		}

		if (perksData.empty()) {
			logger::error("RemoveActorPerk: No perks to remove");
			return;
		}

		for (const auto& perkData : perksData) {
			if (!perkData.perk) continue;

			if (ctx.source->HasPerk(perkData.perk)) {
				ctx.source->RemovePerk(perkData.perk);
			}
		}
	}

// ╔════════════════════════════════════╗
// ║           VISUAL EFFECTS           ║
// ╚════════════════════════════════════╝

    // ─────────────────────────────────────────────────────────────────────────────
    // WHY SpawnImpactDataSet SHOWED NOTHING:
    // BGSImpactManager::PlayImpactEffect does NOT take a world position. Its 4th/5th
    // parameters are a pick DIRECTION and a pick LENGTH: the engine casts a ray from the
    // given reference's 3D node (root node if the node name is empty) along that direction
    // for that length, and plays whichever impact of the data set matches the material the
    // ray hits. The original code passed the hit POSITION as the "direction" and 0.0f as the
    // length, so the ray had zero length and never hit anything.
    //
    // A first fix used a placed marker reference as the ray origin, but that marker form never
    // gets 3D loaded (the log said so), and the engine needs a ref WITH 3D. So instead the ray
    // now starts from a node on the SOURCE ACTOR (always has 3D): we read that node's world
    // position ourselves, aim from there at the hit point, and give the ray enough length to
    // reach it and pass slightly through.
    // ─────────────────────────────────────────────────────────────────────────────

    static constexpr float kImpactOvershoot = 24.0f;  // how far past the hit point the ray extends

    // TEMPORARY DIAGNOSTICS - set to false once SpawnImpactDataSet works.
    //  * logs what our own Havok ray hits first along the same line the impact ray takes
    //  * also fires the data set straight down from the source's feet as a known-good control
    static constexpr bool kImpactDebug = false;

    static void ProbeImpactRay(RE::Actor* a_source, const RE::NiPoint3& a_origin, const RE::NiPoint3& a_dir, float a_length)
    {
        auto* cell = a_source->GetParentCell();
        auto* world = cell ? cell->GetbhkWorld() : nullptr;
        if (!world) return;

        static const RE::COL_LAYER layers[] = {
            RE::COL_LAYER::kStatic, RE::COL_LAYER::kAnimStatic, RE::COL_LAYER::kTerrain, RE::COL_LAYER::kGround,
            RE::COL_LAYER::kClutter, RE::COL_LAYER::kProps, RE::COL_LAYER::kTrees, RE::COL_LAYER::kWeapon,
            RE::COL_LAYER::kBiped, RE::COL_LAYER::kCharController, RE::COL_LAYER::kDebrisSmall, RE::COL_LAYER::kDebrisLarge
        };

        const auto scale = RE::bhkWorld::GetWorldScale();
        const RE::NiPoint3 end = a_origin + a_dir * a_length;

        uint32_t filterInfo = 0;
        a_source->GetCollisionFilterInfo(filterInfo);

        for (auto layer : layers) {
            RE::bhkPickData pick;
            pick.rayInput.from = a_origin * scale;
            pick.rayInput.to = end * scale;
            pick.rayInput.filterInfo = (filterInfo & 0xFFFF0000) | static_cast<uint32_t>(layer);
            world->PickObject(pick);

            if (!pick.rayOutput.HasHit()) continue;

            RE::TESObjectREFR* hitRef = nullptr;
            if (auto* niObj = RE::TESHavokUtilities::FindCollidableObject(*pick.rayOutput.rootCollidable)) {
                hitRef = static_cast<RE::TESObjectREFR*>(niObj->GetUserData());
            }
            logger::info("SpawnImpactDataSet probe: layer {} hit ref {:08X} '{}' at {:.0f} units",
                static_cast<int>(layer), hitRef ? hitRef->GetFormID() : 0u,
                hitRef && hitRef->GetName() ? hitRef->GetName() : "", pick.rayOutput.hitFraction * a_length);
        }
    }

    // Shared by SpawnImpact / SpawnImpactDataSet: works out WHERE the impact should appear and
    // the direction it arrived from. Ground/water hits use ctx.hitPos. For a hit on an object
    // the player's camera ray is used when it lands on the target, otherwise the bottom of
    // the target's bounds. Melee hits carry no real impact coordinate, so this is an estimate.
    static bool ResolveImpactHitPoint(const RuleContext& ctx, RE::Actor* sourceActor, bool isGroundHit, RE::NiPoint3& hitPos, RE::NiPoint3& incomingDir)
    {
        bool hitPosFound = false;
        bool haveDir = false;

        if (isGroundHit) {
            hitPos = ctx.hitPos;
            hitPosFound = true;
        } else if (sourceActor->IsPlayerRef()) {
            auto* cam = RE::PlayerCamera::GetSingleton();
            if (cam && cam->currentState && cam->currentState->camera) {
                const auto start = cam->currentState->camera->cameraRoot->world.translate;
                auto camDir = cam->currentState->camera->cameraRoot->world.rotate * RE::NiPoint3{ 0.f, 1.f, 0.f };
                camDir.Unitize();

                const float kSearchRadius = 4096.0f;
                auto end = start + camDir * kSearchRadius;

                auto* cell = sourceActor->GetParentCell();
                auto* world = cell ? cell->GetbhkWorld() : nullptr;
                if (world) {
                    static std::vector<RE::COL_LAYER> targetLayers = {
                        RE::COL_LAYER::kCollisionBox,
                        RE::COL_LAYER::kStatic,
                        RE::COL_LAYER::kAnimStatic,
                        RE::COL_LAYER::kClutter,
                        RE::COL_LAYER::kClutterLarge,
                        RE::COL_LAYER::kTrap,
                        RE::COL_LAYER::kProps,
                        RE::COL_LAYER::kTerrain,
                        RE::COL_LAYER::kWeapon,
                        RE::COL_LAYER::kBiped,
                        RE::COL_LAYER::kTrees,
                        RE::COL_LAYER::kGround,
                        RE::COL_LAYER::kDoorDetection,
                        RE::COL_LAYER::kDebrisSmall,
                        RE::COL_LAYER::kDebrisLarge,
                        RE::COL_LAYER::kItemPicker,
                        RE::COL_LAYER::kDroppingPick
                    };

                    RE::bhkPickData pick;
                    const auto scale = RE::bhkWorld::GetWorldScale();

                    pick.rayInput.from = start * scale;
                    pick.rayInput.to = end * scale;

                    uint32_t filterInfo = 0;
                    sourceActor->GetCollisionFilterInfo(filterInfo);

                    for (RE::COL_LAYER layer : targetLayers) {
                        pick.rayInput.filterInfo = (filterInfo & 0xFFFF0000) | static_cast<uint32_t>(layer);
                        world->PickObject(pick);

                        if (pick.rayOutput.HasHit()) {
                            auto* niObj = RE::TESHavokUtilities::FindCollidableObject(*pick.rayOutput.rootCollidable);
                            if (niObj) {
                                auto* hitRef = static_cast<RE::TESObjectREFR*>(niObj->GetUserData());
                                if (hitRef == ctx.target) {
                                    auto rayDirection = pick.rayInput.to - pick.rayInput.from;
                                    auto hitPoint = pick.rayInput.from + rayDirection * pick.rayOutput.hitFraction;
                                    hitPos = RE::NiPoint3(hitPoint.quad.m128_f32[0], hitPoint.quad.m128_f32[1], hitPoint.quad.m128_f32[2]) / scale;
                                    hitPosFound = true;
                                    incomingDir = camDir;
                                    haveDir = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!hitPosFound) {
            auto* targetRef3D = ctx.target->Get3D();
            if (!targetRef3D) {
                logger::warn("SpawnImpact*: Target has no 3D model, skipping");
                return false;
            }

            hitPos = ctx.target->GetPosition();
            auto bounds = targetRef3D->worldBound;
            if (bounds.radius > 0.0f) {
                hitPos = bounds.center;
                hitPos.z = bounds.center.z - bounds.radius + 5.0f;
            }
            hitPosFound = true;
        }

        if (!haveDir) {
            auto from = sourceActor->GetPosition();
            from.z += 96.0f;
            auto toHit = hitPos - from;
            if (toHit.Length() > 1.0f) {
                toHit.Unitize();
                incomingDir = toHit;
            }
        }
        return hitPosFound;
    }

    void SpawnImpactDataSet(const RuleContext& ctx, const std::vector<ImpactDataSetSpawnData>& impactsData)
    {
        const bool isGroundHit = !ctx.target;
        logger::info("SpawnImpactDataSet: called ({} impact set(s), {})", impactsData.size(), isGroundHit ? "ground/water hit" : "target hit");

        if ((!isGroundHit && ctx.target->IsDeleted()) || impactsData.empty()) {
            logger::error("SpawnImpactDataSet: No target or impacts data to spawn");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("SpawnImpactDataSet: Ground hit with no impact position");
            return;
        }

        // Who "fired" the hit - the impact ray is cast from a node on this actor.
        auto* sourceActor = ctx.source ? ctx.source : RE::PlayerCharacter::GetSingleton();
        if (!sourceActor) {
            logger::error("SpawnImpactDataSet: No source actor or player found");
            return;
        }

        RE::NiPoint3 hitPos{};
        RE::NiPoint3 incomingDir{ 0.0f, 0.0f, -1.0f };
        if (!ResolveImpactHitPoint(ctx, sourceActor, isGroundHit, hitPos, incomingDir)) {
            return;
        }

        auto* im = RE::BGSImpactManager::GetSingleton();
        if (!im) {
            logger::error("SpawnImpactDataSet: BGSImpactManager is null");
            return;
        }

        auto* sourceRoot = sourceActor->Get3D();
        if (!sourceRoot) {
            logger::error("SpawnImpactDataSet: source actor has no 3D, cannot cast the impact ray");
            return;
        }

        // Candidate ray origins on the source actor, tried in order until the engine accepts one.
        // "" = the 3D root node itself.
        static const char* kOriginNodes[] = { "NPC Head [Head]", "NPC Root [Root]", "" };

        for (const auto& data : impactsData) {
            if (!data.impact) continue;

            for (std::uint32_t i = 0; i < data.count.value; ++i) {
                bool played = false;

                for (const char* nodeName : kOriginNodes) {
                    RE::NiAVObject* node = nodeName[0] ? sourceRoot->GetObjectByName(nodeName) : sourceRoot;
                    if (!node) continue;

                    const RE::NiPoint3 origin = node->world.translate;
                    RE::NiPoint3 pickDir = hitPos - origin;
                    const float dist = pickDir.Length();
                    if (dist < 1.0f) {
                        pickDir = RE::NiPoint3{ 0.0f, 0.0f, -1.0f };
                    } else {
                        pickDir.Unitize();
                    }
                    const float pickLength = dist + kImpactOvershoot;

                    played = im->PlayImpactEffect(sourceActor, data.impact, nodeName, pickDir, pickLength, false, false);
                    logger::info("SpawnImpactDataSet: node '{}' origin ({:.0f}, {:.0f}, {:.0f}) -> hit ({:.0f}, {:.0f}, {:.0f}), ray length {:.0f}, result {}",
                        nodeName, origin.x, origin.y, origin.z, hitPos.x, hitPos.y, hitPos.z, pickLength, played);
                    if (played) break;
                }

                if (!played) {
                    logger::warn("SpawnImpactDataSet: PlayImpactEffect found nothing to hit from any origin node for {:08X}", data.impact->GetFormID());
                }

                if (kImpactDebug) {
                    if (auto* headNode = sourceRoot->GetObjectByName("NPC Head [Head]")) {
                        RE::NiPoint3 d = hitPos - headNode->world.translate;
                        const float dl = d.Length();
                        if (dl > 1.0f) {
                            d.Unitize();
                            ProbeImpactRay(sourceActor, headNode->world.translate, d, dl + kImpactOvershoot);
                        }
                    }

                    // Control: the exact call footstep-style Papyrus scripts use - straight down from the feet.
                    RE::NiAVObject* rootNode = sourceRoot->GetObjectByName("NPC Root [Root]");
                    RE::NiPoint3 down{ 0.0f, 0.0f, -1.0f };
                    const bool ctrl = im->PlayImpactEffect(sourceActor, data.impact, rootNode ? "NPC Root [Root]" : "", down, 128.0f, false, false);
                    logger::info("SpawnImpactDataSet DEBUG: straight-down control from feet, result {}", ctrl);
                }
            }
        }
    }

    // Plays a single Impact record (IPCT) directly - no Impact Data Set, no material lookup, no
    // ray. Spawns the record's effect model at the hit point, oriented along the surface normal
    // (approximated as facing back toward the source; straight up for ground/water hits).
    // Only the effect MODEL is played: the IPCT's decal and sound are not.
    static constexpr float kImpactEffectLifetime = 4.0f;

    void SpawnImpact(const RuleContext& ctx, const std::vector<ImpactSpawnData>& impactsData)
    {
        const bool isGroundHit = !ctx.target;
        logger::info("SpawnImpact: called ({} impact(s), {})", impactsData.size(), isGroundHit ? "ground/water hit" : "target hit");

        if ((!isGroundHit && ctx.target->IsDeleted()) || impactsData.empty()) {
            logger::error("SpawnImpact: No target or impacts data to spawn");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("SpawnImpact: Ground hit with no impact position");
            return;
        }

        auto* sourceActor = ctx.source ? ctx.source : RE::PlayerCharacter::GetSingleton();
        if (!sourceActor) {
            logger::error("SpawnImpact: No source actor or player found");
            return;
        }

        RE::NiPoint3 hitPos{};
        RE::NiPoint3 incomingDir{ 0.0f, 0.0f, -1.0f };
        if (!ResolveImpactHitPoint(ctx, sourceActor, isGroundHit, hitPos, incomingDir)) {
            return;
        }

        const RE::NiPoint3 normal = isGroundHit ? RE::NiPoint3{ 0.0f, 0.0f, 1.0f }
                                                : RE::NiPoint3{ -incomingDir.x, -incomingDir.y, -incomingDir.z };

        auto* cell = isGroundHit ? sourceActor->GetParentCell() : ctx.target->GetParentCell();
        if (!cell) cell = sourceActor->GetParentCell();
        if (!cell) {
            logger::error("SpawnImpact: no cell to spawn the impact in");
            return;
        }

        for (const auto& data : impactsData) {
            if (!data.impact) continue;

            auto* model = data.impact->As<RE::TESModel>();
            const char* modelPath = model ? model->GetModel() : nullptr;
            if (!modelPath || !modelPath[0]) {
                logger::warn("SpawnImpact: impact {:08X} has no effect model, nothing to spawn", data.impact->GetFormID());
                continue;
            }

            for (std::uint32_t i = 0; i < data.count.value; ++i) {
                auto* fx = RE::BSTempEffectParticle::Spawn(cell, kImpactEffectLifetime, modelPath, normal, hitPos, 1.0f, 0, nullptr);
                logger::info("SpawnImpact: {:08X} model '{}' at ({:.0f}, {:.0f}, {:.0f}) -> {}",
                    data.impact->GetFormID(), modelPath, hitPos.x, hitPos.y, hitPos.z, fx ? "spawned" : "FAILED");
            }
        }
    }

    void SpawnExplosion(const RuleContext& ctx, const std::vector<ExplosionSpawnData>& explosionsData)
    {
        const bool isGroundHit = !ctx.target;

        if (!isGroundHit && ctx.target->IsDeleted()) {
            logger::error("SpawnExplosion: No target to spawn explosions");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("SpawnExplosion: Ground hit with no impact position");
            return;
        }

        if (explosionsData.empty()) {
            logger::error("SpawnExplosion: No explosions to spawn");
            return;
        }

        RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
        if (isGroundHit && !anchor) {
            logger::error("SpawnExplosion: No anchor actor available for ground hit");
            return;
        }

        for (const auto& explosionData : explosionsData) {
            if (!explosionData.explosion)
                continue;

            for (std::uint32_t i = 0; i < explosionData.count.value; ++i) {
                if (isGroundHit) {
                    auto explosion = SpawnAtPosition(anchor, explosionData.explosion, explosionData.fade, ctx.hitPos);
                } else {
                    auto explosion = Spawn(ctx.target, explosionData.explosion, explosionData.spawnType, explosionData.fade, explosionData.string, &ctx);
                }
            }
        }
    }

    void SpawnHazard(const RuleContext& ctx, const std::vector<HazardSpawnData>& hazardsData)
    {
        const bool isGroundHit = !ctx.target;

        if (!isGroundHit && ctx.target->IsDeleted()) {
            logger::error("SpawnHazard: No target to spawn hazards");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("SpawnHazard: Ground hit with no impact position");
            return;
        }

        if (hazardsData.empty()) {
            logger::error("SpawnHazard: No hazards to spawn");
            return;
        }

        RE::Actor* anchor = isGroundHit ? ResolveGroundHitAnchor(ctx) : nullptr;
        if (isGroundHit && !anchor) {
            logger::error("SpawnHazard: No anchor actor available for ground hit");
            return;
        }

        static std::vector<std::future<void>> hazardRemovalTasks;
        static std::mutex hazardRemovalMutex;

        for (const auto& hazardData : hazardsData) {
            if (!hazardData.hazard)
                continue;

            for (std::uint32_t i = 0; i < hazardData.count.value; ++i) {
                auto hazard = isGroundHit
                    ? SpawnAtPosition(anchor, hazardData.hazard, hazardData.fade, ctx.hitPos)
                    : Spawn(ctx.target, hazardData.hazard, hazardData.spawnType, hazardData.fade, hazardData.string, &ctx);

                if (hazard && !hazard->IsDeleted() && hazardData.duration > 0.0f) {
                    RE::FormID hazardID = hazard->GetFormID();

                    auto future = std::async(std::launch::async, [hazardID, duration = hazardData.duration]() {
                        std::this_thread::sleep_for(std::chrono::duration<float>(duration));

                        SKSE::GetTaskInterface()->AddTask([hazardID]() {
                            auto* spawnedHazard = RE::TESForm::LookupByID<RE::TESObjectREFR>(hazardID);
                            if (!spawnedHazard || spawnedHazard->IsDeleted()) return;

                            if (!spawnedHazard->IsDisabled()) spawnedHazard->Disable();
                            spawnedHazard->SetDelete(true);
                        });
                    });

                    std::lock_guard<std::mutex> lock(hazardRemovalMutex);
                    hazardRemovalTasks.push_back(std::move(future));

                    hazardRemovalTasks.erase(
                        std::remove_if(hazardRemovalTasks.begin(), hazardRemovalTasks.end(),
                            [](const std::future<void>& f) {
                                return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
                            }),
                        hazardRemovalTasks.end());
                }
            }
        }
    }

    void SwapHazard(const RuleContext& ctx, const std::vector<HazardSpawnData>& hazardsData)
    {
        // Unlike SpawnHazard, swapping only makes sense against an actual struck
        // reference - there's nothing to "swap out" on a ground/water hit (ctx.target is
        // null there by design), so this intentionally has no ground-hit path.
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("SwapHazard: No target to swap with a hazard");
            return;
        }

        if (hazardsData.empty()) {
            logger::error("SwapHazard: No hazards to swap with");
            return;
        }

        static std::vector<std::future<void>> hazardRemovalTasks;
        static std::mutex hazardRemovalMutex;

        bool anyHazardSpawned = false;

        for (const auto& hazardData : hazardsData) {
            if (!hazardData.hazard)
                continue;

            for (std::uint32_t i = 0; i < hazardData.count.value; ++i) {
                auto hazard = Spawn(ctx.target, hazardData.hazard, hazardData.spawnType, hazardData.fade, hazardData.string, &ctx);
                if (!hazard || hazard->IsDeleted()) continue;

                anyHazardSpawned = true;

                if (hazardData.duration > 0.0f) {
                    RE::FormID hazardID = hazard->GetFormID();

                    auto future = std::async(std::launch::async, [hazardID, duration = hazardData.duration]() {
                        std::this_thread::sleep_for(std::chrono::duration<float>(duration));

                        SKSE::GetTaskInterface()->AddTask([hazardID]() {
                            auto* spawnedHazard = RE::TESForm::LookupByID<RE::TESObjectREFR>(hazardID);
                            if (!spawnedHazard || spawnedHazard->IsDeleted()) return;

                            if (!spawnedHazard->IsDisabled()) spawnedHazard->Disable();
                            spawnedHazard->SetDelete(true);
                        });
                    });

                    std::lock_guard<std::mutex> lock(hazardRemovalMutex);
                    hazardRemovalTasks.push_back(std::move(future));

                    hazardRemovalTasks.erase(
                        std::remove_if(hazardRemovalTasks.begin(), hazardRemovalTasks.end(),
                            [](const std::future<void>& f) {
                                return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
                            }),
                        hazardRemovalTasks.end());
                }
            }
        }

        if (anyHazardSpawned && ctx.target) {
            for (const auto& hazardData : hazardsData) {
                if (ctx.target) {
                    if (hazardData.nonDeletable) {
                        DisableItem(ctx);
                    } else {
                        if (!ctx.target->IsDisabled())
                            ctx.target->Disable();
                        ctx.target->SetDelete(true);
                    }
                }
            }
        }
    }

    // Flora (and harvestable Trees, e.g. fruit trees) grant their ingredient
    // directly into the activating actor's inventory as part of the native
    // engine's activation handling - there's no supported hook point to
    // intercept and cancel that grant before it happens, since the "Activate"
    // event this framework reacts to is a post-facto notification fired only
    // after the native activation has already run. So instead of preventing
    // the grant, this reverses it immediately afterward (removes exactly what
    // was actually granted) and drops a physical copy on the ground instead,
    // which is functionally equivalent from the player's perspective.
    // Flora (and harvestable Trees, e.g. fruit trees) grant their ingredient
    // directly into the activating actor's inventory as part of the native
    // engine's activation handling - there's no supported hook point to
    // intercept and cancel that grant before it happens, since the "Activate"
    // event this framework reacts to is a post-facto notification fired only
    // after the native activation has already run. So instead of preventing
    // the grant, this reverses it immediately afterward (removes exactly what
    // was actually granted) and drops a physical copy on the ground instead,
    // which is functionally equivalent from the player's perspective.
    //
    // Per-reference bookkeeping (keyed by the target's own, stable FormID -
    // not a raw TESObjectREFR* which can dangle/be reused once a reference
    // is deleted) used to reliably tell "a harvest just happened" apart from
    // "this already-emptied plant got re-clicked".
    struct HarvestBaseline
    {
        RE::FormID producedItemFormID;
        std::int32_t lastKnownCount;
    };
    static std::unordered_map<RE::FormID, HarvestBaseline> harvestBaselines;

    void DropHarvest(const RuleContext& ctx, const std::vector<HarvestSpawnData>& harvestsData)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("DropHarvest: No target to harvest");
            return;
        }

        if (harvestsData.empty()) {
            logger::error("DropHarvest: No harvest data provided");
            return;
        }

        auto* baseObj = ctx.target->GetBaseObject();
        if (!baseObj) {
            logger::error("DropHarvest: Target has no base object");
            return;
        }

        // Flora and harvestable Trees both share the TESProduceForm mixin,
        // which holds the ingredient/fruit they grant on activation
        // (produceItem, PFIG). Confirmed against the actual vendored header:
        // TESProduceForm has no per-harvest count field at all - the only
        // other member is produceChance[4] (PFPC), a per-season percentage
        // chance of the plant being harvestable that season, not a quantity.
        // The vanilla engine always grants exactly 1 unit of produceItem per
        // activation, so that's hardcoded below rather than read from a
        // nonexistent field.
        auto* produceForm = baseObj->As<RE::TESProduceForm>();
        if (!produceForm || !produceForm->produceItem) {
            logger::warn("DropHarvest: Target's base object doesn't produce an item (not Flora or a harvestable tree?)");
            return;
        }

        RE::TESBoundObject* producedItem = produceForm->produceItem;

        // NOTE: formFlags::kHarvested (bit 1<<13, documented for
        // TESObjectTREE) is deliberately NOT used here - confirmed by live
        // testing that the engine sets this bit synchronously as part of the
        // very same native harvest action that grants the item. Since this
        // whole effect is built on reacting to the Activate event *after*
        // that native action has already completed (there's no earlier hook
        // point to intercept the grant), by the time this code runs the flag
        // has ALWAYS just been set - including on the harvest that is
        // happening right now. So checking it here doesn't distinguish
        // "already harvested before this click" from "harvested by this
        // click a moment ago" - it reads true in both cases, which silently
        // blocked every single Tree harvest from the very first click.
        //
        // The delta-based inventory check below (source's produceItem count
        // actually increasing since the last time this specific reference
        // was processed) doesn't depend on this flag at all and correctly
        // distinguishes both cases for every reference type, Flora and Tree
        // alike.

        // The vanilla engine always grants exactly 1 unit of produceItem per
        // activation (there's no per-plant "quantity" field to read), so
        // that's what gets removed/reversed from inventory below - kept as
        // its own named constant since it's a distinct concept from the
        // user-configurable spawn count right below.
        constexpr std::uint32_t kVanillaGrantCount = 1;

        static thread_local std::mt19937 rng(std::random_device{}());

        // A bare "does the source currently hold more than 0 of this item"
        // check is not a reliable signal that THIS activation actually
        // granted anything: once the player has picked up even a single
        // copy of the produced item (from this plant, a different one, a
        // vendor, wherever), that count stays > 0 forever, so every
        // subsequent re-click of the same already-emptied plant would still
        // pass the check, remove another unit from inventory, and drop a
        // duplicate on the ground - the exact "keeps firing on repeated
        // activation of an already-harvested plant" bug being fixed here.
        // Instead, track a per-reference baseline of the source's inventory
        // count for this specific produced item, and only treat it as a
        // genuine harvest when the count has actually increased since the
        // last time this same reference was processed.
        RE::FormID targetFormID = ctx.target->GetFormID();
        RE::FormID producedItemFormID = producedItem->GetFormID();

        bool actuallyHarvested = false;
        if (ctx.source && !ctx.source->IsDeleted()) {
            auto inventory = ctx.source->GetInventory();
            auto it = inventory.find(producedItem);
            std::int32_t currentCount = (it != inventory.end()) ? it->second.first : 0;

            auto baselineIt = harvestBaselines.find(targetFormID);
            std::int32_t lastKnownCount = 0;
            bool hasBaseline = false;
            if (baselineIt != harvestBaselines.end() && baselineIt->second.producedItemFormID == producedItemFormID) {
                lastKnownCount = baselineIt->second.lastKnownCount;
                hasBaseline = true;
            }

            // First time this reference is seen (no baseline yet), fall
            // back to "is there anything at all to reverse" - matches the
            // previous behavior for a plant's very first activation, and
            // establishes the baseline that makes every later re-click on
            // the same reference correctly detected via the delta above.
            bool grantDetected = hasBaseline ? (currentCount > lastKnownCount) : (currentCount > 0);

            if (grantDetected) {
                std::int32_t removeCount = (std::min)(static_cast<std::int32_t>(kVanillaGrantCount), currentCount);
                ctx.source->RemoveItem(producedItem, removeCount, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                actuallyHarvested = true;
                currentCount -= removeCount;
            }

            harvestBaselines[targetFormID] = HarvestBaseline{ producedItemFormID, currentCount };
        }

        if (!actuallyHarvested) {
            return;
        }

        for (const auto& data : harvestsData) {
            // How many copies actually land on the ground - a normal "count"
            // field just like every other spawn effect (default 1).
            std::uint32_t spawnCount = data.count.value;

            float scale = data.scale.value;
            if (data.scale.useRandom) {
                float lo = (std::min)(data.scale.min, data.scale.max);
                float hi = (std::max)(data.scale.min, data.scale.max);
                scale = std::uniform_real_distribution<float>(lo, hi)(rng);
            }

            for (std::uint32_t i = 0; i < spawnCount; ++i) {
                auto spawned = Spawn(ctx.target, producedItem, data.spawnType, data.fade, "", &ctx);
                if (spawned && !spawned->IsDeleted() && scale >= 0.0f) {
                    SetObjectScale(spawned.get(), scale);
                }
            }
        }
    }

    // Weather is global, not tied to ctx.target at all (see the requiresTarget exemption in
    // RuleManager::ApplyEffect) - RE::Sky::ForceWeather is the same native entry point the
    // vanilla "Weather.ForceActive"/console "fw" path resolves to: it pushes the given
    // TESWeather onto the sky as the new current weather. Passing true for "immediate"
    // (extData.immediate in the rule JSON) skips the usual multi-second transition/fade and
    // switches instantly; false lets it transition the normal way.
    void ChangeWeather([[maybe_unused]] const RuleContext& ctx, const std::vector<WeatherChangeData>& weatherData)
    {
        auto* sky = RE::Sky::GetSingleton();
        if (!sky) {
            logger::error("ChangeWeather: Sky singleton is null");
            return;
        }

        if (weatherData.empty()) {
            logger::error("ChangeWeather: No weather data provided");
            return;
        }

        for (const auto& data : weatherData) {
            if (!data.weather) {
                logger::warn("ChangeWeather: Skipping entry with no weather form resolved");
                continue;
            }

            // ForceWeather ("fw" in console) ALWAYS applies abruptly, no matter what its
            // bool argument is - confirmed against the real CommonLibSSE-NG header, where
            // that param is named a_override (governs whether an existing forced/locked
            // weather gets replaced, not transition speed). That's why immediate true vs
            // false looked identical before: both paths were calling ForceWeather.
            //
            // Sky::SetWeather(weather, a_override, a_accelerate) is the actual "sw"-style
            // gradual/blended transition (over in-game hours, same as the console command).
            // So the two options now genuinely map to two different native calls:
            //   immediate == false -> SetWeather: normal gradual blend
            //   immediate == true  -> ForceWeather + a direct currentWeatherPct snap, so
            //                         the very next frame renders fully transitioned with
            //                         no crossfade at all (the pct is what actually drives
            //                         the frame-by-frame sky/fog/lighting blend).
            if (data.immediate) {
                sky->ForceWeather(data.weather, true);
                sky->currentWeather = data.weather;
                sky->currentWeatherPct = 1.0f;
            } else {
                sky->SetWeather(data.weather, true, false);
            }
        }
    }

    // Real-time, in-place base object swap on the EXISTING reference - as opposed to
    // SwapItem/SwapActor, which spawn a brand-new reference and delete the old one. That
    // matters for anything that tracks the object by its RefID: persistent references,
    // quest aliases, other mods/scripts holding a handle to it - all of that keeps working
    // here because the reference itself never changes, only what it's a reference TO.
    //
    // Conceptually similar to what Base Object Swapper does, but BOS intercepts BEFORE a
    // reference's 3D is ever loaded (hooking the load pipeline so the engine loads the
    // substitute from the start) - there's no live state to reconcile at that point. This
    // runs on a reference that's already fully loaded into the world, which is a
    // fundamentally different problem: the existing 3D model, collision, and (for some
    // types) inventory are all tied to the CURRENT base object, so simply repointing
    // data.objectReference isn't enough on its own to make any of that catch up.
    //
    // RE::TESObjectREFR::SetObjectReference(TESBoundObject*) does the repoint (it's a real,
    // documented CommonLibSSE-NG virtual function - confirmed against the actual vendored
    // header rather than assumed). The disable/enable cycle immediately after is the exact
    // same collision/3D refresh mechanism ScaleObject already uses elsewhere in this file:
    // forcing a fresh Load3D is the reliable way to get the engine to rebuild everything
    // 3D-related (model, collision) from whatever the base object is NOW, rather than what
    // it was when the reference was first loaded.
    //
    // Deliberately restricted to non-actors for now. An Actor carries far more state tied
    // to its base (race, AI packages, combat/faction data, animation graph) than a simple
    // disable/enable cycle can safely reconcile - swapping an Actor's identity in place is
    // a meaningfully different and riskier operation than this. SwapActor's existing spawn-
    // new/delete-old approach remains the right tool for that.
    void SwapBaseObject(const RuleContext& ctx, const std::vector<BaseObjectSwapData>& swapData)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("SwapBaseObject: No target to swap");
            return;
        }

        if (swapData.empty()) {
            logger::error("SwapBaseObject: No swap data provided");
            return;
        }

        if (ctx.target->As<RE::Actor>()) {
            logger::warn("SwapBaseObject: Target {:08X} is an Actor - not supported, use SwapActor instead", ctx.target->GetFormID());
            return;
        }

        if (swapData.size() > 1) {
            logger::warn("SwapBaseObject: {} entries provided, only the first will be used - a reference can only have one base object at a time", swapData.size());
        }

        const auto& data = swapData.front();
        if (!data.newBase) {
            logger::error("SwapBaseObject: No new base object resolved");
            return;
        }

        ctx.target->SetObjectReference(data.newBase);

        // Deliberately left at literally the original position - SetObjectReference
        // already leaves it untouched, so no further adjustment is made here (a previous
        // version of this shifted pos.z by the bound-bottom delta between the old and new
        // mesh to compensate for differing pivot heights, but that's the same kind of
        // automatic repositioning SwapItem's own applyGroundClamp=false now deliberately
        // avoids - removed to match).

        // Same collision-refresh mechanism as ScaleObject - see the comment there for why
        // this disable/enable cycle is necessary. Unlike ScaleObject this isn't skipped for
        // any reference type here since Actors are already rejected above entirely.
        if (!ctx.target->IsDeleted()) {
            if (!ctx.target->IsDisabled()) {
                ctx.target->Disable();
            }
            ctx.target->Enable(data.resetInventory);

            if (data.fade != 0) {
                ctx.target->formFlags &= ~static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
            } else {
                ctx.target->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
            }
        }
    }

	void SpawnEffectShader(const RuleContext& ctx, const std::vector<EffectShaderSpawnData>& effectShadersData)
	{
		// kHitGround/kHitWater never have a TESObjectREFR target (terrain and
		// water planes aren't references) - ctx.hitPos/hasHitPos carry the
		// impact location instead. See ResolveGroundHitAnchor's comment.
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::error("SpawnEffectShader: No target to search for closest actors");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::error("SpawnEffectShader: Ground hit with no impact position");
			return;
		}

		if (effectShadersData.empty()) {
			logger::error("SpawnEffectShader: No effect shaders to spawn");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("SpawnEffectShader: TES singleton is null");
			return;
		}

		// For a ground/water hit, spawn a throwaway marker reference exactly at
		// the impact position and use it as the search origin, the same way
		// SpawnSpell does for its dummy caster - ForEachActorInSphere needs an
		// actual TESObjectREFR to search around, and ctx.target doesn't exist here.
		RE::NiPointer<RE::TESObjectREFR> dummy;
		if (isGroundHit) {
			static RE::TESBoundObject* dummyForm = nullptr;
			if (!dummyForm) {
				auto* dh = RE::TESDataHandler::GetSingleton();
				auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
				dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
			}
			if (!dummyForm) {
				logger::error("SpawnEffectShader: Cannot create dummy marker for ground hit");
				return;
			}

			auto* anchor = ResolveGroundHitAnchor(ctx);
			if (!anchor) {
				logger::error("SpawnEffectShader: No anchor actor available for ground hit");
				return;
			}
			dummy = SpawnAtPosition(anchor, dummyForm, 0, ctx.hitPos);
			if (!dummy) {
				logger::error("SpawnEffectShader: Failed to create dummy marker for ground hit");
				return;
			}
		}

		RE::TESObjectREFR* searchOrigin = isGroundHit ? dummy.get() : ctx.target;

		for (const auto& effectShaderData : effectShadersData) {
			if (!effectShaderData.effectShader) continue;

			std::vector<RE::Actor*> targets;

			// This fixes "radius" here into an exact 3D sphere around searchOrigin -
			// see ForEachActorInSphere's comment above.
			ForEachActorInSphere(searchOrigin, effectShaderData.radius.value, [&](RE::Actor* actor) {
				if (!actor->IsDead() && !actor->IsDisabled()) {
					targets.push_back(actor);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			if (targets.empty()) {
				logger::warn("SpawnEffectShader: No valid actors found in range");
				continue;
			}

			for (auto* actor : targets) {
				for (std::uint32_t i = 0; i < effectShaderData.count.value; ++i) {
					auto shaderEffect = actor->ApplyEffectShader(effectShaderData.effectShader, effectShaderData.duration, nullptr, false, false, nullptr, false);

					if (!shaderEffect) {
						logger::error("SpawnEffectShader: Failed to apply effect shader {} on actor {}",
							effectShaderData.effectShader ? effectShaderData.effectShader->GetFormID() : 0,
							actor ? actor->GetFormID() : 0);
					}
				}
			}
		}

		if (dummy) {
			dummy->Disable();
			dummy->SetDelete(true);
		}
	}

	void SpawnEffectShaderOnItem(const RuleContext& ctx, const std::vector<EffectShaderSpawnData>& effectShadersData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SpawnEffectShader: No target to spawn effect shaders");
			return;
		}

		if (effectShadersData.empty()) {
			logger::error("SpawnEffectShader: No effect shaders to spawn");
			return;
		}

		for (const auto& effectShaderData : effectShadersData) {
			if (!effectShaderData.effectShader) continue;

			for (std::uint32_t i = 0; i < effectShaderData.count.value; ++i) {
				auto shaderEffect = ctx.target->ApplyEffectShader(effectShaderData.effectShader, effectShaderData.duration, nullptr, false, false, nullptr, false);

				if (!shaderEffect) {
					logger::error("SpawnEffectShader: Failed to apply effect shader {} on target {}",
						effectShaderData.effectShader ? effectShaderData.effectShader->GetFormID() : 0,
						ctx.target ? ctx.target->GetFormID() : 0);
				}
			}
		}
	}

	void SpawnArtObject(const RuleContext& ctx, const std::vector<ArtObjectData>& artObjectsData)
	{
		// kHitGround/kHitWater never have a TESObjectREFR target (terrain and
		// water planes aren't references) - ctx.hitPos/hasHitPos carry the
		// impact location instead. See ResolveGroundHitAnchor's comment.
		const bool isGroundHit = !ctx.target;

		if (!isGroundHit && ctx.target->IsDeleted()) {
			logger::error("SpawnArtObject: No target to search for closest actors");
			return;
		}
		if (isGroundHit && !ctx.hasHitPos) {
			logger::error("SpawnArtObject: Ground hit with no impact position");
			return;
		}

		if (artObjectsData.empty()) {
			logger::error("SpawnArtObject: No art objects to spawn");
			return;
		}

		auto* tes = RE::TES::GetSingleton();
		if (!tes) {
			logger::error("SpawnArtObject: TES singleton is null");
			return;
		}

		// For a ground/water hit, spawn a throwaway marker reference exactly at
		// the impact position and use it as the search origin, the same way
		// SpawnEffectShader/SpawnSpell do for theirs - ForEachActorInSphere needs
		// an actual TESObjectREFR to search around, and ctx.target doesn't exist here.
		RE::NiPointer<RE::TESObjectREFR> dummy;
		if (isGroundHit) {
			static RE::TESBoundObject* dummyForm = nullptr;
			if (!dummyForm) {
				auto* dh = RE::TESDataHandler::GetSingleton();
				auto* form = dh ? dh->LookupForm(0x000B79FF, "Skyrim.esm") : nullptr;
				dummyForm = form ? form->As<RE::TESBoundObject>() : nullptr;
			}
			if (!dummyForm) {
				logger::error("SpawnArtObject: Cannot create dummy marker for ground hit");
				return;
			}

			auto* anchor = ResolveGroundHitAnchor(ctx);
			if (!anchor) {
				logger::error("SpawnArtObject: No anchor actor available for ground hit");
				return;
			}
			dummy = SpawnAtPosition(anchor, dummyForm, 0, ctx.hitPos);
			if (!dummy) {
				logger::error("SpawnArtObject: Failed to create dummy marker for ground hit");
				return;
			}
		}

		RE::TESObjectREFR* searchOrigin = isGroundHit ? dummy.get() : ctx.target;

		for (const auto& artObjectData : artObjectsData) {
			if (!artObjectData.artObject) continue;

			std::vector<RE::Actor*> targets;

			// This fixes "radius" here into an exact 3D sphere around searchOrigin -
			// see ForEachActorInSphere's comment above.
			ForEachActorInSphere(searchOrigin, artObjectData.radius.value, [&](RE::Actor* actor) {
				if (!actor->IsDead() && !actor->IsDisabled()) {
					targets.push_back(actor);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});

			if (targets.empty()) {
				logger::warn("SpawnArtObject: No valid actors found in range");
				continue;
			}

			for (auto* actor : targets) {
				for (std::uint32_t i = 0; i < artObjectData.count.value; ++i) {
					auto artObjectEffect = actor->ApplyArtObject(artObjectData.artObject, artObjectData.duration, nullptr, false, false, nullptr, false);

					if (!artObjectEffect) {
						logger::error("SpawnArtObject: Failed to apply art object {} on actor {}",
							artObjectData.artObject ? artObjectData.artObject->GetFormID() : 0,
							actor ? actor->GetFormID() : 0);
					}
				}
			}
		}

		if (dummy) {
			dummy->Disable();
			dummy->SetDelete(true);
		}
	}

	void SpawnArtObjectOnItem(const RuleContext& ctx, const std::vector<ArtObjectData>& artObjectsData)
	{
		if (!ctx.target || ctx.target->IsDeleted()) {
			logger::error("SpawnArtObjectOnItem: No target to apply art objects to");
			return;
		}

		if (artObjectsData.empty()) {
			logger::error("SpawnArtObjectOnItem: No art objects to apply");
			return;
		}

		for (const auto& artObjectData : artObjectsData) {
			if (!artObjectData.artObject) continue;

			for (std::uint32_t i = 0; i < artObjectData.count.value; ++i) {
				ctx.target->ApplyArtObject(artObjectData.artObject, artObjectData.duration, nullptr, false, false, nullptr, false);
			}
		}

		// Causes stuttering for some load orders, the cause is yet to identify

		//// Physics nudge to ensure the art object won't freeze when target stops moving
		//static std::vector<std::future<void>> runningTasks;
		//static std::mutex tasksMutex;

		//auto future = std::async(std::launch::async, [target = ctx.target, duration = artObjectsData[0].duration]() {
		//	const float tickInterval = 0.05f;
		//	auto startTime = std::chrono::steady_clock::now();
		//	auto endTime = startTime + std::chrono::duration<float>(duration + 1.0f);

		//	while (std::chrono::steady_clock::now() < endTime) {
		//		std::this_thread::sleep_for(std::chrono::duration<float>(tickInterval));

		//		SKSE::GetTaskInterface()->AddTask([target]() {
		//			if (target && !target->IsDeleted()) {
		//				auto node3D = target->Get3D();
		//				if (node3D) {
		//					auto collisionObject = node3D->GetCollisionObject();
		//					if (collisionObject) {
		//						auto rigidBody = collisionObject->GetRigidBody();
		//						if (rigidBody) {
		//							hkVector4 nudge(0.0f, 0.0f, 1e-4f, 0.0f);
		//							rigidBody->SetLinearImpulse(nudge);
		//						}
		//					}
		//				}
		//			}
		//		});
		//	}

		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//});

		//{
		//	std::lock_guard<std::mutex> lock(tasksMutex);
		//	runningTasks.push_back(std::move(future));

		//	runningTasks.erase(
		//		std::remove_if(runningTasks.begin(), runningTasks.end(),
		//			[](const std::future<void>& f) {
		//				return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
		//			}),
		//		runningTasks.end());
		//}
	}

// ╔════════════════════════════════════╗
// ║           AUDIO EFFECTS            ║
// ╚════════════════════════════════════╝  

    void PlaySound(const RuleContext& ctx, const std::vector<SoundSpawnData>& soundsData)
    {
        const bool isGroundHit = !ctx.target;

        if (!isGroundHit && ctx.target->IsDeleted()) {
            logger::error("PlaySound: No target to play sound");
            return;
        }
        if (isGroundHit && !ctx.hasHitPos) {
            logger::error("PlaySound: Ground hit with no impact position");
            return;
        }

        if (soundsData.empty()) {
            logger::error("PlaySound: No sounds to play");
            return;
        }

        RE::NiPoint3 pos;
        RE::NiAVObject* followObj = nullptr;
        if (isGroundHit) {
            // No reference to attach the sound to - just play it at the exact
            // impact position.
            pos = ctx.hitPos;
        } else if (auto* root = ctx.target->Get3D()) {
            pos = root->worldBound.center;
            followObj = root;
        } else {
            pos = ctx.target->GetPosition();
        }

        RE::BSSoundHandle handle;
        auto* audioManager = RE::BSAudioManager::GetSingleton();

        if (!audioManager) {
            logger::error("PlaySound: Failed to get audio manager");
            return;
        }

        for (const auto& soundData : soundsData) {
            if (!soundData.sound)
                continue;

            for (std::uint32_t i = 0; i < soundData.count.value; ++i) {
                if (audioManager->BuildSoundDataFromDescriptor(handle, soundData.sound, 1)) {
                    if (followObj) handle.SetObjectToFollow(followObj);
                    handle.SetPosition(pos);
                    handle.Play();
                } else {
                    logger::error("PlaySound: Failed to play sound");
                }
            }
        }
    }

// ╔════════════════════════════════════╗
// ║   MODEL EFFECTS (NON_SERIAIZABLE)  ║
// ╚════════════════════════════════════╝

    void ToggleNode(const RuleContext& ctx, const std::vector<NodeData>& nodeData)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("ToggleNode: No target to toggle node on");
            return;
        }

        if (!ctx.target->Get3D()) ctx.target->Load3D(true);

        auto* rootObj = ctx.target->Get3D();
        if (!rootObj) {
            logger::error("ToggleNode: Target has no 3D");
            return;
        }

        auto* rootNode = rootObj->AsNode();
        if (!rootNode) {
            logger::error("ToggleNode: Root 3D is not NiNode");
            return;
        }

        for (const auto& data : nodeData) {
            std::vector<RE::NiNode*> matches;
            CollectNodes(rootNode, data.strings, matches);
            if (matches.empty()) {
                logger::warn("ToggleNode: No nodes found for {} names", data.strings.size());
                continue;
            }
            
            for (RE::NiNode* node : matches) {
                if (!node) continue;

                try {
                    if (data.mode == 0) {
                        node->local.scale = 0.00001f;
                    } else {
                        node->local.scale = 1.0f;
                    }
                }
                catch (...) {
                    logger::error("ToggleNode: Exception while trying to toggle '{}'", node->name.c_str());
                }
            }
        }
    }
         
    /*void ToggleShaderFlag(const RuleContext& ctx, const std::vector<ShaderFlagData>& shaderFlagsData)
    {
        if (!ctx.target || ctx.target->IsDeleted()) {
            logger::error("ToggleShaderFlag: No target to modify shader flags on");
            return;
        }
    
        if (!ctx.target->Get3D()) {
            ctx.target->Load3D(true);
        }
    
        auto* original3D = ctx.target->Get3D();
        if (!original3D) {
            logger::error("ToggleShaderFlag: Target has no 3D");
            return;
        }
    
        auto* cloned3D = original3D->Clone();
        if (!cloned3D) {
            logger::error("ToggleShaderFlag: Clone3D failed");
            return;
        }
    
        auto* rootNode = cloned3D->AsNode();
        if (!rootNode) {
            logger::error("ToggleShaderFlag: Root 3D is not NiNode");
            return;
        }
    
        for (const auto& data : shaderFlagsData) {
            std::vector<RE::BSGeometry*> matches;
            CollectTriShapes(rootNode, data.strings, matches);
    
            if (matches.empty()) {
                logger::warn("ToggleShaderFlag: No TriShape found");
                continue;
            }
    
            for (RE::BSGeometry* geometry : matches) {
                if (!geometry) continue;
    
                using State = RE::BSGeometry::States;
                
                auto effect = geometry->GetGeometryRuntimeData().properties[State::kEffect].get();
                if (!effect) {
                    logger::warn("ToggleShaderFlag: TriShape '{}' has no effect property", geometry->name.c_str());
                    continue;
                }
                
                auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
                if (!lightingShader) {
                    logger::warn("ToggleShaderFlag: TriShape '{}' does not have BSLightingShaderProperty", geometry->name.c_str());
                    continue;
                }

                for (const auto& flagName : data.flagNames) {
                    auto it = g_shaderFlagMap.find(flagName);
                    if (it == g_shaderFlagMap.end()) {
                        continue;
                    }

                    RE::BSShaderProperty::EShaderPropertyFlag flag = it->second;

                    if (flag == RE::BSShaderProperty::EShaderPropertyFlag::kOwnEmit ||
                        flag == RE::BSShaderProperty::EShaderPropertyFlag::kGlowMap ||
                        flag == RE::BSShaderProperty::EShaderPropertyFlag::kEffectLighting ||
                        flag == RE::BSShaderProperty::EShaderPropertyFlag::kExternalEmittance) {

                        lightingShader->flags.reset(RE::BSShaderProperty::EShaderPropertyFlag::kOwnEmit);
                        lightingShader->flags.reset(RE::BSShaderProperty::EShaderPropertyFlag::kGlowMap);
                        lightingShader->flags.reset(RE::BSShaderProperty::EShaderPropertyFlag::kEffectLighting);
                        lightingShader->flags.reset(RE::BSShaderProperty::EShaderPropertyFlag::kExternalEmittance);
                        
                        if (lightingShader->emissiveColor) {
                            delete lightingShader->emissiveColor;
                            lightingShader->emissiveColor = nullptr;
                        }
                        lightingShader->emissiveMult = 0.0f;
                    }

                    lightingShader->lastRenderPassState = (std::numeric_limits<std::int32_t>::max)();
                    
                    lightingShader->DoClearRenderPasses();

                    logger::info("Before: flags = 0x{:X}, lastRenderPassState = {}", 
                        lightingShader->flags.underlying(), lightingShader->lastRenderPassState);
                    
                    if (data.mode == 0) {
                        lightingShader->flags.reset(flag);
                        logger::info("ToggleShaderFlag: Disabled flag '{}' on TriShape '{}'", flagName, geometry->name.c_str());
                    } else {
                        lightingShader->flags.set(flag);
                        logger::info("ToggleShaderFlag: Enabled flag '{}' on TriShape '{}'", flagName, geometry->name.c_str());
                        if (flag == RE::BSShaderProperty::EShaderPropertyFlag::kOwnEmit ||
                            flag == RE::BSShaderProperty::EShaderPropertyFlag::kGlowMap ||
                            flag == RE::BSShaderProperty::EShaderPropertyFlag::kEffectLighting ||
                            flag == RE::BSShaderProperty::EShaderPropertyFlag::kExternalEmittance) {
                            if (!lightingShader->emissiveColor) {
                                lightingShader->emissiveColor = new RE::NiColor(1.0f, 1.0f, 1.0f);
                            }
                            lightingShader->emissiveMult = 1.0f;
                        }
                    }

                    logger::info("After: flags = 0x{:X}, lastRenderPassState = {}", 
                        lightingShader->flags.underlying(), lightingShader->lastRenderPassState);
    
                    lightingShader->SetupGeometry(geometry);
                    lightingShader->FinishSetupGeometry(geometry);
                }
            }
        }
    
        ctx.target->Set3D(nullptr, false);
        ctx.target->Set3D(cloned3D, true);
        ctx.target->Load3D(true);
        ctx.target->Disable();
        ctx.target->Enable(false);
        ctx.target->formFlags |= static_cast<std::uint32_t>(RE::TESObjectREFR::RecordFlags::kNeverFades);
    }*/
}
