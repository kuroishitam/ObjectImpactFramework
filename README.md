# Object Impact Framework

**Object Impact Framework (OIF)** is a SKSE plugin that lets you add custom effects to items upon **activation**, **grabbing**, **releasing**, **throwing**, **hitting**, **update**, or **weather change**. You can make things happen like spawning or swapping items and actors, casting spells, creating various visual effects, playing sounds, and more - all controlled with JSON files. Compatible with everything.

This guide explains how to set up those JSON files so you can create your own mods. Examples are given at the end of the documentation.

## Users Info

- Place your JSON files in: `Data/SKSE/Plugins/ObjectImpactFramework/`

## Mod Authors Info

- **Filter Note**: An object must be defined by at least one of the three parameters - `formIDs`, `editorIDs`, `formLists`, `formTypes`, `keywords` - for the event to work. **Warning:** Equivalents with the `Not` ending do *not* count.
- **Effect Additional Fields Note**: Make sure you check whether the effect requires the presence of the `items` field. Without specifying and filling out this field (at least with one identifier or, if the effect does not support identifiers, with any available field), the effect will *not* work.
- **Priority Note**: Place effects with `"Remove"` and `"Swap"` prefixes at the very end of the event, otherwise the removed object may not have time to call other effects on itself before gets deleted.
- **Non-Collidable Objects Note**: By default, the framework can detect non-collidable objects, but this does not apply to the `Hit` event. Only `flora` and `tree` hits are supported. Since the system uses workarounds and mathematical calculations, it can cause plants location estimate not to be always accurate. **Warning:** Spells with attached projectiles are currently not supported due to the hooks issues.
- **Troubleshooting Note**: If something doesn't work, look at the mod's log file for error messages. The log can be found in `Documents/My Games/Skyrim Special Edition/SKSE/ObjectImpactFramework.log`
- You can modify existing JSON files without quitting the game, edit the file and reload the save.

---

## Table of Contents

- [Basic Rule Structure](#basic-rule-structure)
- [Filters: Choosing Which Objects to Affect and How](#filters-choosing-which-objects-to-affect-and-how)
  - [General Filters](#general-filters)
  - [Time‑Based Filters](#time-based-filters)
  - [Proximity-Based Filters](#proximity-based-filters)
  - [Source Actor Filters](#source-actor-filters)
  - [Hit-Specific Filters](#hit-specific-filters)
- [Effects: What Happens When the Rule Triggers](#effects-what-happens-when-the-rule-triggers)
  - [Effect Types](#object-management)
  - [Configuring Effects with items](#configuring-effects-with-items)
- [AddBounty Examples](#addbounty-examples)
- [Examples](#examples)

---

## Basic Rule Structure

Each rule in your JSON file defines a specific behavior for the mod. Rules are written as an array of objects, where each object contains three main fields: `event`, `filter`, and `effect`. Here's what each field does:

- **`event`**: Specifies the trigger(s) for the rule. This is an array of strings, allowing multiple events to trigger the same rule. Valid values are:
  - `"Activate"`: Triggered when an object is activated (e.g., opened, used).
  - `"Hit"`: Triggered when an object is hit by a weapon, spell, or projectile.
  - `"Grab"`: Triggered when an object is picked up by the player.
  - `"Release"`: Triggered when a grabbed object is dropped.
  - `"Telekinesis"`: Triggered when an object the player was holding with telekinesis lands.
  - `"Throw"`: Triggered when a grabbed object is thrown (requires the **Grab And Throw** mod by powerofthree).
  - `"CellAttach"`: Triggered when an object is attached to a cell (works on location re-enter as well).
  - `"CellDetach"`: Triggered when an object is detached from a cell.
  - `"WeatherChange"`: Triggered on weather change.
  - `"OnUpdate"`: Triggered every 1 second.
  - `"DestructionStageChange"`: Triggered on object's destruction stage change. The effect will *not* be applied to the scene when the object is disabled or deleted.
  - `"HitGround"`: Triggered when a weapon, spell, or projectile hits bare terrain instead of any object. There is no target object for this event, so `formTypes`/`formIDs`/`editorIDs`/`formLists`/`keywords` filters are not required and effects that need to act on a target reference (e.g. `RemoveItem`, `SwapItem`, `ScaleObject`) do not work with it; position-based spawn/sound/notification effects do. **Note:** projectiles report their ground impact accurately, but melee weapons (and hand-to-hand, which never triggers this at all) rely on an approximation of where the swing "would have" hit the ground, since there's no real projectile travelling to a real impact point to hook. This makes a weapon-based `"HitGround"` rule behave a bit like a `"Hit"`-on-weapon-swing rule - it can occasionally fire while you're attacking an NPC or object too, not only genuinely empty ground. If that's a problem, restrict the rule to a specific weapon (`weapons`/`weaponsTypes`) rather than relying on it only ever firing on true misses.
  - `"HitWater"`: Same as `"HitGround"`, but for a splash on a water surface (a cell's own water, or exterior LOD water) instead of bare terrain. Same lack of a target object, and the same effect support/limits as `"HitGround"`.
  
- **`filter`**: Defines the targeted objects and the conditions under which a rule applies to them. At least one of `formTypes`, `formIDs`, `editorIDs`, `formLists`, or `keywords` must be provided to identify target objects (not required for `"HitGround"`/`"HitWater"` rules, which have no target object).

- **`effect`**: Describes what happens when the rule is triggered. Each effect has a `type` and, for most types, an `items` array specifying what to spawn, swap, or apply.

Here's a basic example of a rule:

```json
[
    {
        "event": ["Activate", "Hit"],
        "filter": {
            "formTypes": ["activator"]
        },
        "effect": [{
            "type": "SpawnItem",
            "items": [{"formID": "Skyrim.esm:0xF"}]
        }]
    }
]
```

This rule triggers when an activator (e.g., a lever) is activated or hit, spawning a single gold coin (`Skyrim.esm:0xF`).

---

## Filters: Choosing Which Objects to Affect and How

The `filter` object determines which objects and interactions trigger a rule. It's highly customizable, allowing precise control over when effects occur.

Below are all possible filter parameters:

### General Filters

- **`formTypes`** (one of the required fields to choose from): An array of strings specifying the types of objects the rule applies to. Possible values:
  - `"activator"`: Standard activators (e.g., levers, buttons).
  - `"talkingactivator"`: Activators that can "speak" (e.g., some quest-related objects).
  - `"weapon"`: Weapons like swords or bows (lying around in the world).
  - `"armor"`: Armor pieces.
  - `"ammo"`: Ammunition (e.g., arrows, bolts).
  - `"ingredient"`: Alchemy ingredients.
  - `"misc"`: Miscellaneous items (e.g., clutter, tools).
  - `"key"`: Keys.
  - `"book"`: Books and journals.
  - `"note"`: Notes.
  - `"scroll"`: Magical scrolls.
  - `"soulgem"`: Soul gems.
  - `"potion"`: Includes potions, poisons, food, and drinks.
  - `"furniture"`: Furniture like chairs or beds.
  - `"door"`: Doors and gates.
  - `"flora"`: Harvestable plants and hanging animals.
  - `"container"`: Containers like chests or barrels.
  - `"static"`: Static objects (e.g., statues).
  - `"moveablestatic"`: Movable static objects.
  - `"tree"`: Trees.
  - `"light"`: Lights with 3D models (e.g., torches).
 
- **`formTypesNot`**: An array of strings specifying the types of objects that the rule should *not* apply to. Same format as `formTypes`.

- **`formIDs`** (one of the required fields to choose from): An array of strings identifying specific objects by their formID in the format `"modName:formID"`. Examples:
  - `"Skyrim.esm:0x123456"` (for esp/esm plugins).
  - `"MyMod.esl:0x456"` (for esl/espfe plugins).
  - `"Dawnguard.esm:00123456"` (alternate format with leading zeros (esp/esm)).
  - `"MyMod.esp:FE000800"` (alternate format with leading FE prefix (esl/espfe)).

- **`formIDsNot`**: An array of strings identifying specific objects that the rule should *not* apply to. Same format as `formIDs`.

- **`references`** (one of the required fields to choose from): An array of strings identifying one or more *specific placed references* by their own FormID, in the same `"modName:formID"` format as `formIDs` — e.g. `"MyMod.esp:0xABCDEF"`. This is different from `formIDs`, which matches *every* instance of a base object (every copy of that chest, every copy of that flora) — `references` matches only that one exact object placed in the world (the FormID you'd get from the in-game console by clicking it, or from the Reference's own FormID in the Render Window in xEdit/CK). Useful for "only this specific door" or "only this one statue" rules. Only works for persistent, pre-placed references from a plugin — it can't target references spawned dynamically at runtime, since those don't have a fixed FormID known ahead of time.

- **`referencesNot`**: An array of strings identifying specific placed references that the rule should *not* apply to. Same format as `references`.

- **`editorIDs`** (one of the required fields to choose from): An array of strings identifying specific objects by their editorID in the format `"EditorIDName"`. Example: `"VendorItemClutter"`.

- **`editorIDsNot`**: An array of strings identifying specific objects that the rule should *not* apply to. Same format as `editorIDs`.

- **`formLists`** (one of the required fields to choose from): An array of objects referencing formlists. Each entry has:
  - **`formID`**: The formlist's form ID in `"modName:formID"` format (use this or `editorID`).
  - **`editorID`**: The formlist's editor ID in `"EditorIDName"` format (use this or `formID`).
  - **`index`** (optional): An integer specifying which item in the formlist to use. Example:
 
    ```json
    "formLists": [
        {"formID": "Skyrim.esm:0x123456", "index": 0},
        {"editorID": "ExampleFormList"}
    ]
    ```
    - **Index `-1`** is default, means that all the items at once will be used. 
    - **Index `-2`** defines the parrallel relationship for mirrored formlists: matching positions between `filter` and effects' `items` formlists correspond directly, allowing ordered transformations (e.g., raw meat at position 0 in formlist 1 becomes cooked meat at position 0 in formlist 2).

- **`formListsNot`**: An array of formlist objects that the rule should *not* apply to. Same format as `formLists`.

- **`keywords`** (one of the required fields to choose from): An array of keywords or formists of keywords that the object must have. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`keywordsNot`**: An array of keywords or formists of keyword the object must *not* have. Same format as `keywords`.

- **`chance`**: A number between 0 and 100 representing the percentage chance the rule triggers (e.g., `"chance": 90.0`). Defaults to `100`.

  **Optional** detailed entry (e.g., `"chance": {"min": 0.1, "max": 85.0}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`interactions`**: An integer specifying how many interactions (e.g., hits) are required to trigger the effect (e.g., `"interactions": 5`). Defaults to `1`.

  **Optional** detailed entry (e.g., `"interactions": {"min": 1, "max": 5}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`limit`**: An integer setting the maximum number of interactions per object (e.g., `"limit": 5`). Defaults to `0` - no limit.

  **Optional** detailed entry (e.g., `"limit": {"min": 0, "max": 10}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`questItemStatus`**: An integer specifying quest item status requirements. Only works with **ACTIVE** player quests:
  - `0` (default): Not a quest item.
  - `1`: Quest alias only.
  - `2`: A full-fledged quest item.
  - `3`: All allowed.
 
- **`isInitiallyDisabled`**: An integer specifying whether the object has the `kInitiallyDisabled` flag.
  - `0`: Not initially disabled.
  - `1`: Initially disabled.
  - `2` (default): All allowed.

 - **`isStacked`**: An integer specifying whether the object is a single reference or a stacked bunch.
   - `0`: Not stacked.
   - `1`: Stacked.
   - `2` (default): All allowed.
 
 - **`isParented`**: An integer specifying whether the object has a valid Creation Kit **Enable Parent** reference attached to it. The filter checks the reference's `ExtraEnableStateParent` data.
 
   - `0`: Does not have an Enable Parent.
   - `1`: Has an Enable Parent.
   - `2` (default): All allowed.

   This is useful for excluding objects that are controlled by another reference, such as hidden fire/light references used by some objects (for example, Khajiit caravan fires).

   Example: affect only fire/light references that are **not** controlled by an Enable Parent:

   ```json
   {
       "event": ["WeatherChange"],
       "filter": {
           "formTypes": ["light"],
           "isParented": 0
       },
       "effect": [
           {
               "type": "DisableItem"
           }
       ]
   }
   ```
- **`isOwned`**: An integer specifying the ownership state of the object reference.
  - `0`: Unowned only.
  - `1`: Owned only (any owner).
  - `2` (default): Ignore ownership.
  - `3`: Unowned or player-owned. NPC-, faction-, and other non-player-owned references are excluded.
  - `4`: Owned except player-owned. Unowned and player-owned references are excluded.
  
 - **Spawn/swap ownership**: Pickable inventory items created by OIF clear ownership when the source reference is unowned or player-owned, preventing them from being treated as stolen. If the source already has an NPC/faction/etc. owner, that ownership is preserved. Non-pickable world objects such as Activators and Movable Statics are not modified by this ownership cleanup.
 
- **`lockLevel`**: An integer specifying a lock level the object must have. **Note:** Do *not* use the filter with formTypes other than `door` and `container`.
  - `-2` (default): All allowed.
  - `-1`: Unlocked.
  - `0`: Novice.
  - `1`: Apprentice.
  - `2`: Adept.
  - `3`: Expert.
  - `4`: Master.
  - `5`: Requires key.
 
- **`lockLevelNot`**: An interger specifying a lock level the object must *not* have. Same format as `lockLevel`.
 
- **`isPluginInstalled`**: An array of plugin names (e.g., `"MyMod.esp"`, `"Skyrim.esm"`) that must be loaded for the rule to apply.

- **`isPluginNotInstalled`**: An array of plugin names that must *not* be loaded.

- **`isDllInstalled`**: An array of DLL filenames (e.g., `"MyPlugin.dll"`) that must be present in `Data/SKSE/Plugins/`.

- **`isDllNotInstalled`**: An array of DLL filenames that must *not* be present.

- **`materials`**: An array of physical Havok material names the target's collision shape must match (e.g., `["stone", "wood"]`). Matching is a case-insensitive *substring* match against the object's `RE::MATERIAL_ID` name, so `"stone"` matches `kStone`, `kStoneStairs`, `kStoneHeavy`, etc. - you don't need to know the exact internal enum name, just a recognizable fragment of it (`"wood"`, `"metal"`, `"flesh"`, `"snow"`, `"water"`, `"glass"`, and so on). Only resolved when the rule has a real target object (`Hit`/`Activate`/etc. on an actual reference); `"HitGround"`/`"HitWater"` rules have no target reference to read a material from, so this filter never matches for them.

  Example: only trigger on stone or metal surfaces:

  ```json
  "filter": {
      "formTypes": ["static"],
      "materials": ["stone", "metal"]
  }
  ```

- **`materialsNot`**: An array of physical material names the target must *not* match. Same format as `materials`. Objects can have several collision parts with different materials: `materials` matches if *any* part matches, `materialsNot` rejects if *any* part matches. If no material can be read at all (no loaded 3D / no collision), `materials` doesn't match, while `materialsNot` passes.

### Time-Based Filters

- **`timer`**: A defined number of seconds before triggering the effect (e.g., `"timer": 1.0`). **Optional** detailed entry has:
  - **`time`**: Number of seconds. (e.g., `"timer": {"time": 1.0}`).
    **Optional** detailed entry (e.g., `"timer": {"time": {"min": 1.0, "max": 10.0}}`:
    - **`min`**: Minimal random value.
    - **`max`**: Maximal random value.
  - **`matchFilterRecheck`**: Whether the effect needs to be canceled if conditions were violated while waiting.
    - `0`: No re-check.
    - `1`: Re-check.

  ```json
  "timer": {"time": 1.0, "matchFilterRecheck": 1}
  ```

- **`time`**: An array of in-game time conditions that must be active for the rule to apply. Format: `["Hour >= 10", "DayOfWeek = 1"]`. Available entries:
  - `Minute`
  - `Hour`
  - `Day`
  - `DayOfWeek`
  - `Month`
  - `Year`
  - `GameTime`
 
- **`timeNot`**: An array of in-game time conditions that must *not* be active. Same format as `time`.

### Proximity-Based Filters

- **`nearbyObjects`**: An array of objects that must be in a certain radius around the target object for the rule should apply. Each entry has:
  - **`formID`**: The object formID or a formlist's formID in `"modName:formID"` format (use this or `editorID`).
  - **`editorID`**: The object editorID or a formlist's editorID in `"EditorIDName"` format (use this or `formID`).
  - **`radius`**: An integer specifying in what radius to search. Example:
 
    ```json
    "nearbyObjects": [
        {"formID": "Skyrim.esm:0x123456", "radius": 150},
        {"editorID": "Player", "radius": 300}
    ]
    ```
- **`nearbyObjectsNot`**: An array of objects that must *not* be in a certain radius around the target object. Same format as `nearbyObjects`.

- **`locations`**: An array of cells, locations, or worldspaces where the rule should apply. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID. **Note:** The worldspace will *not* be detected and the event will be filtered out if the cell has no linked worldspace information.

- **`locationsNot`**: An array of cells, locations, or worldspaces where the rule should *not* apply. Same format as `locations`.

- **`locationKeywords`**: An array of Creation Kit keywords that the target's current location - *or any of that location's parent locations* (e.g. a district's parent city, or a city's parent Hold) - must have at least one of. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"LocTypeInn"`), or a formlist's formID/editorID.

  Example: only trigger while inside an inn (or a district of one), never in a dungeon (or any of its parent locations):

  ```json
  "filter": {
      "locationKeywords": ["LocTypeInn"],
      "locationKeywordsNot": ["LocTypeDungeon"]
  }
  ```

- **`locationKeywordsNot`**: An array of keywords that the target's current location and all of its parent locations must *not* have any of. Same format as `locationKeywords`.

- **`isInterior`**: An integer specifying whether the object is indoors.
  - `0`: Not an interior.
  - `1`: Interior.
  - `2` (default): All allowed.

- **`isTeleportDoor`**: An integer specifying whether the target door is a load door (linked to a destination elsewhere) as opposed to a door that just opens/closes in place (a cell gate, cabinet door, portcullis, etc.). Only meaningful when the target is a `door`; checked via the reference's `ExtraTeleport` data (present only on doors actually set up to teleport the player). Ground/water hits have no door to check and never match either specific value.
  - `0`: Door that does *not* teleport (opens in place).
  - `1`: Door that teleports (a load door).
  - `2` (default): All allowed.

- **`position`**: An integer specifying the object's position in relation to the **player**.
  - `0`: Below the middle.
  - `1`: Middle.
  - `2`: Above the middle.
  - `3` (default): All allowed.

- **`isInWater`**: An integer specifying whether the object is touching water (a cell's own water, or exterior LOD water).
  - `0`: Not touching water.
  - `1`: On the surface (the object straddles the waterline - e.g. floating or partially submerged).
  - `2`: Fully submerged (entirely below the waterline).
  - `3` (default): All allowed.

  For `"HitGround"`/`"HitWater"` rules (which have no target object), this reflects which of the two events actually fired (`0` for `"HitGround"`, `1` for `"HitWater"`) rather than re-checking the impact position.

- **`weathers`**: An array of weathers that must be active for the rule to apply. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`weathersNot`**: An array of weathers that must *not* be active. Same format as `weathers`.

### Source-Actor Filters

- **`perks`**: An array of perks that the event source actor must have. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`perksNot`**: An array of perks that the event source actor must *not* have. Same format as `perks`.

- **`spells`**: An array of spells that the event source actor must have. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`spellsNot`**: An array of spells that the event source actor must *not* have. Same format as `spells`.

- **`shouts`**: An array of shouts that the event source actor must know (fully unlocked). Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"FireBreathShout"`), or a formlist's formID/editorID. Note: shout knowledge is a player-specific mechanic, so this filter is only meaningful when the source actor is the player.

- **`shoutsNot`**: An array of shouts that the event source actor must *not* know. Same format as `shouts`.

- **`shoutWordsKnown`**: An array of objects checking how many words (1-3) of a specific shout are currently unlocked, e.g. `{"shout": "Skyrim.esm:0x0F82BF", "minWords": 1, "maxWords": 3}`. `minWords` defaults to 1, `maxWords` defaults to 3. Like `shouts`, this reflects player-specific word-unlock state, so it's only meaningful when the source actor is the player.

- **`hasItem`**: An array of items that the event source actor must have in their inventory. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`hasItemNot`**: An array of items that the event source actor must *not* have. Same format as `hasItem`.

- **`isEquip`**: An array of weapons/armor/other equippable items that the event source actor must currently have equipped or worn (weapons, shields, staves and torches in either hand, or worn armor/ammo). Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"IronSword"`), or a formlist's formID/editorID.

- **`isEquipNot`**: An array of weapons/armor/other equippable items that the event source actor must *not* currently have equipped or worn. Same format as `isEquip`.

- **`actorKeywords`**: An array of keywords that the event source actor must have in their inventory. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`actorKeywordsNot`**: An array of keywords that the event source actor must *not* have. Same format as `actorKeywords`.

- **`actorRaces`**: An array of races that the event source actor must have in their inventory. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`actorRacesNot`**: An array of keywords that the event source actor must *not* have. Same format as `actorRaces`.

- **`actorValues`**: An array of actor value conditions that the event source actor must meet. Format: `["Health >= 10", "Aggression = 0"]`. Available entries:

  **Core Attributes:**
  - `Health`, `Magicka`, `Stamina`
  - `HealRate`, `HealRateMult`, `MagickaRate`, `MagickaRateMult`, `StaminaRate`, `StaminaRateMult`

  **AI Attributes:**
  - `Aggression`, `Confidence`, `Assistance`, `Morality`, `Mood`

  **Condition Values:**
  - `EnduranceCondition`, `PerceptionCondition`
  - `LeftAttackCondition`, `RightAttackCondition`
  - `LeftMobilityCondition`, `RightMobilityCondition`

  **Misc Values:**
  - `Fame`, `Infamy`, `Energy`, `JumpingBonus`, `MovementNoiseMult`
  - `CombatHealthRegenMultiply`, `LastBribedIntimidated`, `LastFlattered`
  - `FavorActive`, `FavorPointsBonus`, `FavorsPerDay`, `FavorsPerDayTimer`
  - `GrabActorOffset`, `Grabbed`, `WaitingForPlayer`, `IgnoreCrippledLimbs`

  **Combat Stats:**
  - `AttackDamageMult`, `CriticalChance`, `MeleeDamage`, `UnarmedDamage`
  - `WeaponSpeedMult`, `LeftWeaponSpeedMultiply`, `SpeedMult`
  - `DamageResist`, `ReflectDamage`

  **Equipment:**
  - `OneHanded`, `TwoHanded`, `Archery`, `Block`
  - `OneHandedModifier`, `OneHandedPowerModifier`, `OneHandedSkillAdvance`
  - `TwoHandedModifier`, `TwoHandedPowerModifier`, `TwoHandedSkillAdvance`
  - `MarksmanModifier`, `MarksmanPowerModifier`, `MarksmanSkillAdvance`
  - `LeftItemCharge`, `RightItemCharge`
  - `InventoryWeight`, `Mass`

  **Magic:**
  - `Destruction`, `Alteration`, `Restoration`, `Illusion`, `Conjuration`
  - `DestructionModifier`, `DestructionPowerModifier`, `DestructionSkillAdvance`
  - `AlterationModifier`, `AlterationPowerModifier`, `AlterationSkillAdvance`
  - `RestorationModifier`, `RestorationPowerModifier`, `RestorationSkillAdvance`
  - `IllusionModifier`, `IllusionPowerModifier`, `IllusionSkillAdvance`
  - `ConjurationModifier`, `ConjurationPowerModifier`, `ConjurationSkillAdvance`
  - `AbsorbChance`, `Invisibility`, `NightEye`, `Paralysis`, `Blindness`
  - `WaterBreathing`, `WaterWalking`, `Telekinesis`
  - `WardPower`, `WardDeflection`, `DetectLifeRange`
  - `ResistFire`, `ResistFrost`, `ResistShock`, `ResistMagic`
  - `ResistDisease`, `PoisonResist`

  **Shout Related:**
  - `DragonSouls`, `DragonRend`, `VoicePoints`, `VoiceRate`, `ShoutRecoveryMult`

  **Armor:**
  - `HeavyArmor`, `LightArmor`
  - `HeavyArmorModifier`, `HeavyArmorPowerModifier`, `HeavyArmorSkillAdvance`
  - `LightArmorModifier`, `LightArmorPowerModifier`, `LightArmorSkillAdvance`

  **Stealth:**
  - `Sneak`, `Lockpicking`, `Pickpocket`
  - `SneakingModifier`, `SneakingPowerModifier`, `SneakingSkillAdvance`
  - `LockpickingModifier`, `LockpickingPowerModifier`, `LockpickingSkillAdvance`
  - `PickpocketModifier`, `PickpocketPowerModifier`, `PickpocketSkillAdvance`

  **Crafting:**
  - `Smithing`, `Alchemy`, `Enchanting`
  - `SmithingModifier`, `SmithingPowerModifier`, `SmithingSkillAdvance`
  - `AlchemyModifier`, `AlchemyPowerModifier`, `AlchemySkillAdvance`
  - `EnchantingModifier`, `EnchantingPowerModifier`, `EnchantingSkillAdvance`

  **Speechcraft:**
  - `Speech`, `SpeechcraftModifier`, `SpeechcraftPowerModifier`, `SpeechcraftSkillAdvance`

  **Special Perks:**
  - `ArmorPerks`, `ShieldPerks`, `VampirePerks`, `WerewolfPerks`

- **`actorValuesNot`**: An array of actor value conditions that the event source actor must *not* meet. Same format as `actorValues`.

- **`level`**: An array of level conditions that the event source actor must meet. Format: `[">= 10", "= 20"]`.

- **`levelNot`**: An array of level conditions that the event source actor must *not* meet. Same format as `level`.

An integer specifying whether the event source actor is in the state:
- **`isSneaking`**: Sneaking.
- **`isSwimming`**: Swimming.
- **`isInCombat`**: Combat.
- **`isMounted`**: Mounted.
- **`isDualCasting`**: Dual casting.
- **`isSprinting`**: Sprinting.
- **`isWeaponDrawn`**: Drawn weapon.
- **`isFirstPerson`**: First person.
- **`isThirdPerson`**: Third person.
  - `0`: Source actor is *not* in this state.
  - `1`: Source actor is in this state.
  - `2` (default): All states allowed.

### Hit-Specific Filters

For rules with the `"Hit"` event, additional filters can refine which attacks trigger the rule:

- **`destructionStage`**: Checks with the current destruction stage. Possible values:
  - `-1` (default): All stages allowed.
  - `0`, `1`, etc.: Specific destruction stage index.

- **`weaponsTypes`**: An array of weapon or spell types that must be used. Possible values:
  - `"onehandsword"`: One-handed swords.
  - `"twohandsword"`: Two-handed swords.
  - `"onehandaxe"`: One-handed axes.
  - `"twohandaxe"`: Two-handed axes.
  - `"onehandmace"`: One-handed maces.
  - `"twohandmace"`: Two-handed maces.
  - `"dagger"`: Daggers.
  - `"ranged"`: Bows and crossbows.
  - `"staff"`: Staves.
  - `"handtohand"`: Unarmed attacks.
  - `"torch"`: Torch bashes (a torch is a light, not a weapon, so this is a separate type from the melee ones above).
  - `"spell"`: Spells.
  - `"scroll"`: Scrolls.
  - `"shout"`: Shouts. **Note:** All shouts don't work with statics. Those shouts that are pure projectiles (e.g., Unrelenting Force) will work only with `furniture`, `flora`, `doors`, `containers` and `activators`.
  - `"ability"`: Racial or other abilities.
  - `"power"`: Greater powers.
  - `"lesserpower"`: Lesser powers.
  - `"explosion"`: Explosions.
  - `"total"`: Yet to discover. Supposedly matches all weapon types.
  - `"other"`: Miscellaneous types not covered above.
 
- **`weaponsTypesNot`**: An array of weapon or spell types that must *not* be used. Same format as `weaponsTypes`.

- **`weapons`**: An array of specific weapons or spells. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`weaponsNot`**: An array of specific weapons or spells that must *not* be used. Same format as `weapons`.

- **`weaponsKeywords`**: An array of keywords the weapon or spell must have. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`weaponsKeywordsNot`**: An array of keywords the weapon or spell must *not* have. Same format as `weaponsKeywords`.

- **`attacksTypes`** or **`attacks`**: An array of attack types. Possible values:
  - `"regular"` (default): Standard attacks.
  - `"power"`: Power attacks.
  - `"bash"`: Shield or weapon bashes.
  - `"charge"`: Charging attacks (e.g., sprinting strikes).
  - `"rotating"`: Continuous spinning attacks.
  - `"continuous"`: Concentration type spells (e.g., flames, frostbite).
  - `"constant"`: Contant type spells.
  - `"fireandforget"`: Fire-and-forget type spells.
  - `"ignoreweapon"`: Attacks that bypass weapon-specific mechanics.
  - `"overridedata"`: Attacks that override default data.

- **`attacksTypesNot`** or **`attacksNot`**: An array of attack types that must *not* be used. Same format as `attacksTypes`.

- **`deliveryTypes`**: An array of spell delivery types. Possible values:
  - `"self"`: Self-targeted spells.
  - `"aimed"`: Aimed spells requiring targeting.
  - `"targetactor"`: Actor-targeted spells.
  - `"targetlocation"`: Location-targeted spells.
  - `"touch"`: Touch-based spells.
  - `"total"`: Yet to discover. Supposedly matches all delivery types.
  - `"none"`: None.
 
- **`deliveryTypesNot`**: An array of spell delivery types that must *not* be used. Same format as `deliveryTypes`.

- **`allowProjectiles`**: An integer specifying whether projectiles are allowed. Possible values:
  - `0`: Projectiles are not allowed.
  - `1` (default): Projectiles are allowed.

- **`projectiles`**: An array of specific projectiles. Format: `"modName:formID"` (e.g., `"Skyrim.esm:0xABCDEF"`), `"EditorIDName"` (e.g., `"VendorItemFood"`), or a formlist's formID/editorID.

- **`projectilesNot`**: An array of specific projectiles that must *not* be used. Same format as `projectiles`.

---

## Effects: What Happens When the Rule Triggers

The `effect` field defines the outcome when a rule is triggered. Each effect has a mandatory `type` field and, for most types, an `items` array.

Here are all possible `type` values and their supported fields:

### Utility Effects
- **`ExecuteConsoleCommand`**: Executes a console command on nearby actors.
  - Supported fields: `string`, `radius`, `chance`, `timer`.
 
- **`ExecuteConsoleCommandOnItem`**: Executes a console command on the target object.
  - Supported fields: `string`, `chance`, `timer`.
 
- **`ExecuteConsoleCommandOnSource`**: Executes a console command on an actor who interacted with the target object.
  - Supported fields: `string`, `chance`, `timer`.

- **`ShowNotification`**: Shows a notification.
  - Supported fields: `string`, `chance`, `timer`.

- **`ShowMessageBox`**: Shows a message box with "OK" button.
  - Supported fields: `string`, `chance`, `timer`.

- **`AddBounty`**: Adds bounty to the player's current hold/crime faction.
  - Supported fields: `count`, `violent`, `chance`, `timer`.
  - **`count`**: Amount of bounty gold to add.
  - **`violent`**: Determines which crime-gold category is used:
    - `0`: stealing like bounty.
    - `1`: trespassing like  bounty.
  - The hold/crime faction is resolved automatically from the player's current location.
  - The effect also attempts to trigger the corresponding native crime/alarm behavior using the rule target as the crime reference when required.
  - The player remains the crime actor; `AddBounty` does not kill or otherwise alter the target NPC by itself.
  - A notification is shown using the amount and hold name, for example: `100 bounty added to Whiterun`.

### Object Management
- **`RemoveItem`**: Deletes the target object.
  - No `items` array required.

- **`EnableItem`**: Enables the target object.
  - No `items` array required.

- **`DisableItem`**: Disables the target object.
  - No `items` array required.

- **`UnlockItem`**: Unlocks the target object (with animation).
  - No `items` array required.
 
- **`LockItem`**: Locks the target object (with animation).
  - No `items` array required.

- **`ActivateItem`**: Activates the target object. Behaves the same as pressing `E` for all form types. Does *not* work with the `Activate` event to avoid collisions.
  - No `items` array required.
 
- **`ScaleObject`**: Scales the target object bigger or smaller.
  - Supported fields: `scale`, `fade`, `relative`, `chance`, `timer`.
  - **`scale`**: The value to set the target's scale to (e.g., `"scale": 0.5` for half size, `"scale": 2.0` for double size). Accepts the same `{"min": ..., "max": ...}` random format as other effects.
  - **`relative`**: `false` (default): `scale` sets the target's absolute scale outright. `true`: `scale` is instead a multiplier applied to the target's *current* scale, so repeated hits compound (e.g. `2.0` then `3.0` results in `6.0` total) instead of each one overwriting the last.
  - **`fade`**: Same meaning as the `spawn`/`swap` `fade` field (see below) - whether the object is allowed to fade in/out from distance/LOD after the scale change's collision refresh. Defaults to `1`.
  - Values are clamped to a minimum of `0.01`. There is no hard maximum - the engine accepts arbitrarily large scales, the same as the "Scale" field in the Creation Kit, though very large values may look or collide strangely.
  - If `scale` isn't specified, the target is reset to its normal size (`1.0`) in absolute mode, or left unchanged in relative mode.

- **`RestoreScale`**: Restores the target object to the scale it had *before* the most recent `ScaleObject` call touched it (the "original" scale is recorded automatically the first time `ScaleObject` runs on that reference, and cleared again once `RestoreScale` uses it, so a later `ScaleObject` call starts tracking a fresh baseline).
  - Supported fields: `fade`, `chance`, `timer`.
  - If `ScaleObject` was never called on this reference this session (or the game was reloaded since - this tracking is in-memory only, not saved), it falls back to the engine default of `1.0` instead of doing nothing.
  - No `formID`/`editorID`/`items` needed beyond the optional `fade`/`chance`/`timer` fields - it always restores the target itself.

  Example - hitting an object grows it, activating it puts it back:

  ```json
  [
      {
          "event": ["Hit"],
          "filter": { "formTypes": ["static"] },
          "effect": [{ "type": "ScaleObject", "scale": 1.5, "relative": true }]
      },
      {
          "event": ["Activate"],
          "filter": { "formTypes": ["static"] },
          "effect": [{ "type": "RestoreScale" }]
      }
  ]
  ```

- **`SwapBaseObject`**: Changes the target reference's base object *in place*, rather than spawning a new reference and deleting the old one the way `SwapItem`/`SwapActor` do. Because the reference itself never changes (only what it's a reference *to*), anything that tracks the object by its RefID - persistent references, quest aliases, other mods/scripts holding a handle to it - keeps working. Conceptually similar to what Base Object Swapper does, but BOS intercepts before a reference's 3D is ever loaded; this instead runs on a reference that's already loaded, so a disable/enable cycle is used to force the engine to rebuild the model/collision from the new base object.
  - Supported fields: `formID`, `editorID`, `formList`, `fade`, `resetInventory`, `chance`, `timer`.
  - **`resetInventory`**: `false` (default): the reference's current inventory is kept as-is across the swap. `true`: forwarded to the underlying `Enable()` call as a full inventory reset - matters mainly when swapping to/from a container.
  - A reference can only have one base object at a time: if `items`/`formList` resolves to more than one entry, only the *first* one is actually applied (a warning is logged) - use a formlist `index` of `-3` (random) or a specific index rather than `-1` ("all") here.

  Example - permanently turn a specific unlit brazier into a lit one the first time it's activated:

  ```json
  [
      {
          "event": ["Activate"],
          "filter": {
              "formIDs": ["Skyrim.esm:0x1A3F2"]
          },
          "effect": [{
              "type": "SwapBaseObject",
              "items": [{"formID": "Skyrim.esm:0x1A3F5"}]
          }]
      }
  ]
  ```
 
### Inventory Management
- **`SpillInventory`**: Spills the contents of the target container.
  - No `items` array required.

- **`AddContainerItem`**: Adds specified items to the contents of the target container.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.
 
- **`AddActorItem`**: Adds specified items to the inventory of an actor who interacted with the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.
 
- **`RemoveContainerItem`**: Removes specified items from the contents of the target container.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.
 
- **`RemoveActorItem`**: Removes specified items from the inventory of an actor who interacted with the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.

### Item Spawning & Swapping
- **`SpawnItem`**: Spawns specific items at the target object's location.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `chance`, `timer`.

- **`SpawnLeveledItem`**: Spawns random leveled items at the target object's location (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `chance`, `timer`.

- **`SwapItem`**: Replaces the target object with another specified item.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `nonDeletable`, `chance`, `timer`.

- **`SwapLeveledItem`**: Replaces the target object with a random leveled item (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `nonDeletable`, `chance`, `timer`.

- **`DropHarvest`**: For a `Flora` (or a harvestable `Tree`, e.g. a fruit tree) target, drops its produced ingredient/fruit on the ground instead of letting it land in the source actor's inventory.
  - Supported fields: `count`, `scale`, `fade`, `spawnType`, `chance`, `timer`. No `formID`/`editorID` needed - the item comes from the target itself.
  - **How it actually works (important):** the game grants a Flora/Tree's item directly into inventory as part of its own native activation handling, and there's no supported way for this framework to intercept and cancel that grant before it happens (the `Activate` event only fires as a notification *after* the native activation already ran). So this effect doesn't prevent the grant - it reverses it immediately afterward (removing exactly what was actually granted, never more than the source actor's current stock) and spawns a physical copy on the ground instead. From the player's perspective this is functionally the same as never having received it, just with a dropped item on the ground in its place.
  - `count` controls how many copies are dropped on the ground. If omitted, it defaults to `1` - the same amount the vanilla engine always grants per Flora/Tree activation (there's no per-plant "quantity" field; it's always exactly one). Set it explicitly (a fixed number, or `{"min": ..., "max": ...}` for a random amount each trigger, same as other spawn effects) to drop a different number instead.
  - Only works against a target whose base object actually produces an item (Flora, or a Tree with a configured produce item). Other object types are ignored with a warning.
  - Typical usage: pair with the `Activate` event and a `formTypes: ["flora"]` (or `["tree"]`) filter.

### Actor Spawning & Swapping
- **`SpawnActor`**: Spawns specified actors at the target object's location.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `chance`, `timer`.

- **`SpawnLeveledActor`**: Spawns random leveled actors at the target object's location (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `chance`, `timer`.

- **`SwapActor`**: Replaces the target object with specified actors.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `nonDeletable`, `chance`, `timer`.

- **`SwapLeveledActor`**: Replaces the target object with random leveled actors (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `scale`, `fade`, `spawnType`, `string`, `nonDeletable`, `chance`, `timer`.

### Magic Effects
- **`SpawnSpell`**: Casts specified spells on nearby actors.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `radius`, `chance`, `timer`.

- **`SpawnLeveledSpell`**: Casts random leveled spells on nearby actors (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `radius`, `chance`, `timer`.

- **`SpawnSpellOnItem`**: Casts specified spells on the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `chance`, `timer`.

- **`SpawnLeveledSpellOnItem`**: Casts random leveled spells on the target object (based on the player's level).
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `chance`, `timer`.

- **`ApplySpell`**: Applies the target object's spell (if it's a scroll or a spell tome) to nearby actors.
  - Supported fields: `radius`, `chance`, `timer`.

- **`ApplyIngestible`**: Applies the target object's effects (if it's an ingredient or an ingestible) to nearby actors.
  - Supported fields: `radius`, `chance`, `timer`.

- **`ApplyOtherIngestible`**: Applies effects from specified ingestibles to nearby actors. Can be used with any form type.
  - Supported fields: `formID`, `editorID`, `formList`, `radius`, `chance`, `timer`.
 
- **`AddActorSpell`**: Adds specified spells to the source actor.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.

- **`RemoveActorSpell`**: Removes specified spells from the source actor.
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.

- **`AddActorPerk`**: Adds specified perks to the source actor.
  - Supported fields: `formID`, `editorID`, `formList`, `rank`, `chance`, `timer`.

- **`RemoveActorPerk`**: Removes specified perks from the source actor (regardless of the perk rank).
  - Supported fields: `formID`, `editorID`, `formList`, `chance`, `timer`.

### Actor Value & Force Effects

Both of these act on every actor within a radius of the effect's center (the impact position when there is one, otherwise the target's or source's position), rather than on a single item/spell form - so instead of `formID`/`editorID`/`formList`, their per-item configuration lives directly in the `items` array's other fields.

- **`modav`**: Directly changes an actor value (health, magicka, stamina, skills, resistances, regen rates, or any other value from the same name list as the `actorValues` filter above) for every actor in range, the same way potions/spells restore or damage a value.
  - Supported fields: `string`, `amount`, `radius`, `affectSource`, `affectPlayer`, `chance`, `timer`.
  - **`string`**: The actor value's name (e.g. `"Health"`, `"Stamina"`, `"Magicka"` - any name from the `actorValues` filter's list works here too). Required.
  - **`amount`**: How much to change it by. Positive restores/adds, negative damages/reduces (and can reduce Health to 0, i.e. kill the actor, exactly like any other damage source). Accepts the usual `{"min": ..., "max": ...}` random format.
  - **`affectSource`** (default `true`): Whether the actor who caused the hit can be affected if they're in range.
  - **`affectPlayer`** (default `true`): Whether the player specifically can be affected if in range, independent of whether they're the source.

  Example - a "healing ward" static that restores health to anyone standing near it, but never damages the player:

  ```json
  [
      {
          "event": ["OnUpdate"],
          "filter": {
              "formIDs": ["MyMod.esp:0x800"]
          },
          "effect": [{
              "type": "modav",
              "items": [
                  { "string": "Health", "amount": 5, "radius": 300 }
              ]
          }]
      }
  ]
  ```

- **`ApplyForce`**: Pushes every actor in range directly away from the effect's center - a knockback/shove, written straight into the character controller's velocity the same low-level way vanilla knockback/explosions ultimately work, so it interacts correctly with gravity and collision instead of just teleporting the actor. A modest upward lift is mixed into the push direction so it reads as a shove up-and-away rather than a slide. Actors explicitly flagged non-pushable (bosses, scripted sequences, etc.) are skipped, the same as vanilla knockback effects respect that flag.
  - Supported fields: `amount`, `radius`, `affectSource`, `affectPlayer`, `chance`, `timer`.
  - **`amount`**: Push magnitude, in game units/second. Accepts the `{"min": ..., "max": ...}` random format.
  - **`affectSource`** (default `true`): Whether the actor who caused the hit can be pushed if in range.
  - **`affectPlayer`** (default `true`): Whether the player specifically can be pushed if in range.
  
	Amount	Effect on actor
	- Below ~30	No visible effect — this is the engine's own force threshold, not a bug
	- ~150+	A nudge
	- ~500+	Stagger / knocked down
	- ~1000+	Actor is properly thrown/launched

  Example - an explosive barrel that also shoves everyone nearby away from it when it's hit:

  ```json
  [
      {
          "event": ["Hit"],
          "filter": {
              "formIDs": ["MyMod.esp:0x900"]
          },
          "effect": [{
              "type": "ApplyForce",
              "items": [
                  { "amount": 1200, "radius": 400, "affectSource": false }
              ]
          }]
      }
  ]
  ```

### Visual & Audio Effects
- **`PlaySound`**: Plays a sound descriptor on the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `chance`, `timer`.

- **`PlayIdle`**: Plays an animation on an actor who interacted with the target object. [List of available animation names](https://forums.nexusmods.com/topic/11007808-le-list-of-animation-events-for-debugsendanimationevent/?do=findComment&comment=105617168).
  - Supported fields: `string`, `duration`, `chance`, `timer`.
 
- **`SpawnImpact`**: Plays a single **Impact** record (IPCT) directly at the hit point - no Impact Data Set and no material lookup. Only the record's effect model is played (its decal and sound are not). The form must be an Impact (IPCT); a form of the wrong type is skipped and a warning is written to the log.
- **`SpawnImpactDataSet`**: Plays an **Impact Data Set** (IPDS) - the engine picks the impact by the material that is hit. The form must be an Impact Data Set (IPDS), not a single Impact.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `chance`, `timer`.

- **`SpawnExplosion`**: Triggers an explosion at the target object's location.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `fade`, `spawnType`, `chance`, `timer`.

- **`SpawnHazard`**: Spawns a hazard at the target object's location. Works with any event.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `fade`, `spawnType`, `string`, `duration`, `nonDeletable`, `chance`, `timer`.
  - **`duration`**: How many real-time seconds the spawned hazard lasts before being automatically disabled and deleted. Defaults to `0`, meaning no automatic removal - the hazard persists according to its own native lifetime instead.

- **`SwapHazard`**: Same as `SpawnHazard`, but also removes/disables the original target object afterward (same `nonDeletable` semantics as other `Swap*` effects - `0` deletes it, `1` only disables it). Unlike `SpawnHazard`, this requires an actual target reference to swap out, so it doesn't work with `"HitGround"`/`"HitWater"`.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `fade`, `spawnType`, `string`, `duration`, `nonDeletable`, `chance`, `timer`.

- **`SpawnEffectShader`**: Spawns effect shaders on nearby actors.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `radius`, `duration`, `chance`, `timer`.

- **`SpawnEffectShaderOnItem`**: Spawns effect shaders on the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `duration`, `chance`, `timer`.

- **`SpawnArtObject`**: Spawns art objects on nearby actors.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `radius`, `duration`, `chance`, `timer`.

- **`SpawnArtObjectOnItem`**:  Spawns art objects on the target object.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `duration`, `chance`, `timer`.
 
- **`ToggleNode`**: Toggles the target object's nodes on and off (scales to 0.00001 or 1.0).
  - Supported fields: `mode`, `strings`, `chance`, `timer`.

### Lighting Effects
- **`SpawnLight`**: Spawns a light at the target object's location.
  - Supported fields: `formID`, `editorID`, `formList`, `count`, `fade`, `spawnType`, `string`, `chance`, `timer`.

- **`RemoveLight`**: Deletes lights.
  - Supported fields: `radius`, `chance`, `timer`.

- **`DisableLight`**: Disables lights.
  - Supported fields: `radius`, `chance`, `timer`.

- **`EnableLight`**: Enables previously disabled lights.
  - Supported fields: `radius`, `chance`, `timer`.

### Weather Effects
- **`ChangeWeather`**: Forces or transitions to a specific weather.
  - Supported fields: `formID`, `editorID`, `formList`, `immediate`, `chance`, `timer`.
  - **`immediate`** (default `false`): `false` uses a normal gradual blend to the new weather (the same as the `sw` console command). `true` snaps to it instantly with no crossfade at all - the very next frame renders fully transitioned (the same as `fw`, but actually instant, unlike `fw` alone).
  - Works with any event - it doesn't need or use a target object, so it's a good fit for `"OnUpdate"`, `"HitGround"`/`"HitWater"`, or any other target-less trigger.

  Example - force a storm the instant a specific altar is activated:

  ```json
  [
      {
          "event": ["Activate"],
          "filter": {
              "formIDs": ["MyMod.esp:0xA00"]
          },
          "effect": [{
              "type": "ChangeWeather",
              "items": [{"editorID": "SkyrimStormy1", "immediate": true}]
          }]
      }
  ]
  ```
 
---

### Configuring Effects with `items`

For effect types that support an `items` array, you can specify detailed configurations. Each item in the array can include:

- **`formID`**: A specific formID in `"modName:formID"` format (e.g., `"Skyrim.esm:0xF"` for a gold coin).

- **`editorID`**: A specific editorID in `"EditorIDName` format (e.g., `"VendorItemClutter"` for clutter).

- **`formList`**: An array of formlist entries.
  - **`formID`**: The formlist's formID in `"modName:formID"` format (use this or `editorID`).
  - **`editorID`**: The formlist's editorID in `"EditorIDName"` format (use this or `formID`).
  - **`index`** (optional): An integer specifying which item in the formlist to use:
    - `-1` (default): All items at once.
    - `-2`: Parallel relationship for mirrored formlists.
    - `-3`: Select one random object from the list.
    - Other values: Specific index in the formlist.
    
    Example:
    ```json
    "formList": [
        {"formID": "Skyrim.esm:0x123456", "index": 0},
        {"formID": "MyMod.esp:0x789ABC", "index": -3}
    ]
    ```

- **`chance`**: A number between 0 and 100 for the percentage chance this item is used (e.g., `"chance": 90.0`). Defaults to `100`. 

  **Optional** detailed entry (e.g., `"chance": {"min": 0.1, "max": 85.0}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`timer`**: A defined number of seconds before triggering the effect (e.g., `"timer": 1.0`). **Optional** detailed entry has:
  - **`time`**: Number of seconds. (e.g., `"timer": {"time": 1.0}`).

    **Optional** detailed entry (e.g., `"timer": {"time": {"min": 1.0, "max": 10.0}}`):
    - **`min`**: Minimal random value.
    - **`max`**: Maximal random value.
  - **`matchFilterRecheck`**: Whether the effect needs to be canceled if conditions were violated while waiting.
    - `0`: No re-check.
    - `1`: Re-check.

    ```json
    "timer": {"time": 1.0, "matchFilterRecheck": 1}
    ```

- **`count`**: An integer specifying how many instances to spawn or how many times to perform a particular action (e.g., `"count": 2`). Defaults to `1`. 

  **Optional** detailed entry (e.g., `"count": {"min": 1, "max": 5}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`radius`**: Specifies the radius in game units for effect application (e.g., `"radius": 100.0`). Defaults to `100.0`. This is an exact 3D sphere around the effect's center point (the impact position, or the target/dummy's position) - an actor well above or below that point is out of range just like one too far away horizontally.

  **Optional** detailed entry (e.g., `"radius": {"min": 50.0, "max": 150.0}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.
 
- **`scale`**: Used for `spawn`/`swap` functions only. Allows to select the scale of a spawned item (e.g., `"scale": 2.0`). By default, it is copied from the target object.

  **Optional** detailed entry (e.g., `"scale": {"min": 1.5, "max": 3.0}`):
  - **`min`**: Minimal random value.
  - **`max`**: Maximal random value.

- **`duration`**: For `PlayIdle`, defaults to 1.0 (lower values make animation faster). For **effect shaders** and **art objects**, specifies how long the effect lasts. For **`SpawnHazard`/`SwapHazard`**, how many real-time seconds until the spawned hazard is automatically disabled and deleted; defaults to `0` (no automatic removal - it persists per its own native lifetime instead).

- **`string`**: Used for various effects. Takes one entry. For **spawn** and **swap** effects, used to take node name. For `PlayIdle`, used to take animation name.

- **`strings`**: Used for various effects. Takes multiple entries. For `ToggleNode`, used to take multiple node names.

- **`mode`**: Used for various effects. Currently used for `ToggleNode` effect to specify the disable or enable mode.
  - `0` (default): Disable.
  - `1`: Enable.
 
- **`nonDeletable`**: Used for `swap` functions only. During swap, the original object is deactivated and a new one appears in its place. This value determines whether the original object is deleted (`0`) or only disabled (`1`).

- **`fade`**: Used for `spawn`/`swap` functions only. Determines whether the object will have a fade effect upon creation:
  - `0`: Without fade effect (do **not** use with **explosions**).
  - `1` (default): With fade effect.

- **`relative`**: Used for `ScaleObject` only. `false` (default): `scale` sets the target's absolute scale. `true`: `scale` multiplies the target's *current* scale instead, so repeated hits compound.

- **`resetInventory`**: Used for `SwapBaseObject` only. `false` (default): the reference's inventory is kept as-is across the base object swap. `true`: the inventory is reset as part of the swap - matters mainly when swapping to/from a container.

- **`immediate`**: Used for `ChangeWeather` only. `false` (default): a normal gradual blend to the new weather. `true`: snaps to it instantly with no crossfade.

- **`amount`**: Used for `modav` (how much to change the actor value by - positive restores/adds, negative damages/reduces) and `ApplyForce` (push magnitude, in game units/second). Accepts the same `{"min": ..., "max": ...}` random format as other numeric fields.

- **`affectSource`**: Used for `modav` and `ApplyForce` only. Default `true`. Whether the actor who caused the hit can be affected/pushed if they're within `radius`.

- **`affectPlayer`**: Used for `modav` and `ApplyForce` only. Default `true`. Whether the player specifically can be affected/pushed if within `radius`, independent of whether they're the source.

- **`rank`**: Used for `AddActorPerk` only. The perk rank to add (for ranked perks). Defaults to `1`.

- **`stage`**: An integer, valid on *any* effect entry regardless of type, that makes that specific entry only fire on a particular numbered interaction with the target rather than every time. Each `(source, target, rule)` combination tracks its own ever-increasing "current stage" counter, starting at `1` on the very first successful match and incrementing on every subsequent one (independent of `chance` - a match that loses its chance roll still counts). An effect entry with `"stage": N` only fires while that counter equals `N`; an entry with no `stage` (or `"stage": 0`, the default) always fires regardless of the counter. This lets a single rule sequence *different* effects across successive interactions without earlier stages re-firing - e.g. "1st hit does X, 2nd hit does Y" instead of X firing on every hit.

  Example - a shrine that heals a little on the first hit, then breaks (and stops healing) on the second:

  ```json
  [
      {
          "event": ["Hit"],
          "filter": { "formIDs": ["MyMod.esp:0xB00"] },
          "effect": [
              { "type": "modav", "stage": 1, "items": [{ "string": "Health", "amount": 25, "radius": 150 }] },
              { "type": "SwapItem", "stage": 2, "items": [{"formID": "MyMod.esp:0xB01"}] }
          ]
      }
  ]
  ```

  The stage counter never resets or loops back on its own - once it advances past the highest `stage` used in a rule, only `stage: 0` (unstaged) entries keep firing on later interactions.

- **`spawnType`**: Used for `spawn`/`swap` functions only. Allows you to select the type of how the object should be spawned. Options:
  - `0`: Common PlaceAtMe().
  - `1`: PlaceAtMe() spawning the object at the center of the original.
  - `2`: PlaceAtMe() spawning at the top of the original.
  - `3`: PlaceAtMe() spawning at the bottom of the original.
  - `4` (default): Common PlaceAtMe() with buggy engine physics bypassing (e.g., when used with `Throw`/`Grab`/`Release`, the object will be spawned exactly to the landing point without position glitches).
  - `5`: Bypass with spawning the object at the center of the original.
  - `6`: Bypass with spawning at the top of the original.
  - `7`: Bypass with spawning at the bottom of the original.
  - `8`: Pin to the ground regardless of the landing location (e.g., when used with `Throw`, an object that lands on the wall will spawn a new one directly beneath it on the floor).
  - `9`: Pin to the specified node. Requires a node name or it's substring to be passed in `string`.
  - `10`: Bypass with a random horizontal offset around the original. Radius (in game units) is passed in `string` as a number (e.g. `"128"`); defaults to `64` if `string` is empty or not a number.
  - `11`: Bypass, spawning exactly at the projectile's impact point instead of the target's origin. Only meaningful for hits caused by a projectile (arrows, spells, thrown weapons); melee/magic-effect hits carry no such point and fall back to the center of the original, same as `5`.
  - `12`: Bypass, spawning in front of the original along its current facing. Distance (in game units) is passed in `string` as a number (e.g. `"80"`); defaults to `50` if `string` is empty or not a number. A negative value spawns behind the original instead.
  - `13`: Bypass, spawning at the position of the actor who caused the hit (the attacker/thrower) rather than at the target. Falls back to the target's own position if there is no such actor (e.g. a scripted/trap-triggered event).
  - `14`: Bypass, spawning at a fixed vertical offset from the target. Offset (in game units) is passed in `string` as a number (e.g. `"75"`); defaults to `50` (above) if `string` is empty or not a number. A negative value spawns below the target instead - see the note below on how this interacts with the ground safety net.

  **Every spawnType (`0`-`14`) is also passed through a ground safety net**: after its own position logic runs, if the resulting spot would end up below the actual terrain surface at that (x, y), it's raised back up to just above it. This only ever raises the position, never lowers it, and only applies outdoors, where there's a terrain surface to check against - indoor positions (floors, shelves, etc.) are never touched. This means a negative `14` offset ("below target") is only useful for a target that is itself above the ground (e.g. spawning underneath something hanging or floating); it will not dig an object into solid terrain.

  **NOTE**: If you want to spawn **explosions**, use `0` or `4` only.

---


## Examples

1. **Spawn a Gold Coin on Activation**
   ```json
   [
       {
           "event": ["Activate"],
           "filter": {
               "formTypes": ["container"]
           },
           "effect": [{
               "type": "SpawnItem",
               "items": [{"formID": "Skyrim.esm:0xF"}]
           }]
       }
   ]
   ```
   - Activating a container spawns one gold coin.

2. **Delete an Object When Hit**
   ```json
   [
       {
           "event": ["Hit"],
           "filter": {
               "formTypes": ["static"]
           },
           "effect": [{
               "type": "RemoveItem"
           }]
       }
   ]
   ```
   - Hitting a static object removes it.

3. **Spawn Items with Chances and Custom Scale**
   ```json
   [
       {
           "event": ["Hit"],
           "filter": {
               "formTypes": ["container"]
           },
           "effect": [{
               "type": "SpawnItem",
               "items": [
                   {"formID": "Skyrim.esm:0xF", "count": 5, "chance": 50, "scale": 1.5},
                   {"formID": "Skyrim.esm:0xA", "count": 2, "chance": 30, "fade": 1}
               ]
           }]
       }
   ]
   ```
   - Hitting a container has a 50% chance to spawn 5 gold coins at 1.5x scale and a 30% chance to spawn 2 lockpicks with fade effect.

4. **Explode Trees on Hit**
   ```json
   [
       {
           "event": ["Hit"],
           "filter": {
               "formTypes": ["tree"]
           },
           "effect": [{
               "type": "SpawnExplosion",
               "items": [{"formID": "Skyrim.esm:0x123456"}]
           }]
       }
   ]
   ```
   - Hitting a tree triggers an explosion.

5. **Use FormLists with Random Selection**
   ```json
   [
       {
           "event": ["Activate"],
           "filter": {
               "formTypes": ["container"]
           },
           "effect": [{
               "type": "SpawnItem",
               "items": [
                   {
                       "formList": [
                           {"formID": "MyMod.esp:0x789ABC", "index": 2},
                           {"formID": "Skyrim.esm:0x123456", "index": -3}
                       ],
                       "count": 1,
                       "chance": 100
                   }
               ]
           }]
       }
   ]
   ```
   - Activating a container spawns an item from the formlist at index 2 in `MyMod.esp:0x789ABC` and one random item from `Skyrim.esm:0x123456`.

6. **Play Animation on Hit**
   ```json
   [
       {
           "event": ["Hit"],
           "filter": {
               "formTypes": ["activator"]
           },
           "effect": [{
               "type": "PlayIdle",
               "items": [{"string": "IdleName", "duration": 0.5}]
           }]
       }
   ]
   ```
   - Hitting an activator plays the "IdleName" animation at 0.5x speed.

7. **Spawn Effect Shader with Custom Duration**
   ```json
   [
       {
           "event": ["Activate"],
           "filter": {
               "formTypes": ["static"]
           },
           "effect": [{
               "type": "SpawnEffectShaderOnItem",
               "items": [
                   {
                       "formID": "Skyrim.esm:0x123456",
                       "duration": 5.0
                   }
               ]
           }]
       }
   ]
   ```
   - Activating a static object spawns an effect shader on it for 5 seconds.

8. **Weather and Actor Value Dependent Effects**
   ```json
   [
       {
           "event": ["Hit"],
           "filter": {
               "formTypes": ["tree"],
               "weathers": ["Skyrim.esm:0x123456"],
               "actorValues": ["Health >= 50"],
               "level": [">= 10"]
           },
           "effect": [{
               "type": "SpawnSpell",
               "items": [
                   {
                       "formID": "Skyrim.esm:0x789ABC",
                       "radius": 500
                   }
               ]
           }]
       }
   ]
   ```
   - Hitting a tree during specific weather by an actor with at least 50 health and level 10+ casts a spell on actors within 500 units.

9. **Quest Item Filtering with Inventory Check**
    ```json
    [
        {
            "event": ["Grab"],
            "filter": {
                "formTypes": ["misc"],
                "questItemStatus": 2,
                "hasItem": ["Skyrim.esm:0x123456"]
            },
            "effect": [{
                "type": "PlaySound",
                "items": [{"formID": "Skyrim.esm:0x456789"}]
            }]
        }
    ]
    ```
    - Grabbing a full-fledged quest item by an actor who has a specific item plays a sound.

10. **Apply Ingestible with Custom Radius**
    ```json
    [
        {
            "event": ["Activate"],
            "filter": {
                "formTypes": ["ingredient"]
            },
            "effect": [{
                "type": "ApplyIngestible",
                "items": [{"radius": 300, "chance": 75}]
            }]
        }
    ]
    ```
    - Activating an ingredient applies its effects to actors within 300 units with 75% chance.

11. **Require a Specific Weapon Equipped**
    ```json
    [
        {
            "event": ["Hit"],
            "filter": {
                "formTypes": ["static"],
                "isEquip": ["Skyrim.esm:0x13994"],
                "isEquipNot": ["Skyrim.esm:0x139B7"]
            },
            "effect": [{
                "type": "SpawnExplosion",
                "items": [{"formID": "Skyrim.esm:0x123456"}]
            }]
        }
    ]
    ```
    - Hitting a static object only triggers the explosion if the source actor has a specific weapon (`0x13994`) equipped and does *not* have another one (`0x139B7`) equipped - e.g. a "special hammer breaks rocks, but not while a shield is also equipped" setup.

12. **Harvest a Plant onto the Ground Instead of Into Inventory**
    ```json
    [
        {
            "event": ["Activate"],
            "filter": {
                "formTypes": ["flora"]
            },
            "effect": [{
                "type": "DropHarvest",
                "items": [{"scale": 1.2}]
            }]
        }
    ]
    ```
    - Activating a flora reverses the ingredient it normally grants to inventory and instead drops one copy of it on the ground (matching what the plant actually grants) at 1.2x scale. Add `"count": N` to drop a different number instead.
13. **Add a Non-Violent Bounty**
```json
[
    {
        "event": ["Activate"],
        "filter": {
            "formTypes": ["container"]
        },
        "effect": [{
            "type": "AddBounty",
            "count": 100,
            "violent": 0
        }]
    }
]
```

This adds 100 non-violent bounty gold to the player's current hold.

> **Note:** `violent` only controls the bounty/crime category. The actual alarm behavior depends on the target reference and the Skyrim crime system.

---