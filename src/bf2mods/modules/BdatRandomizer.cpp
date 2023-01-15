#include "BdatRandomizer.hpp"

#include "../State.hpp"
#include "../main.hpp"
#include "bf2mods/Logger.hpp"
#include "bf2mods/engine/bdat/Bdat.hpp"
#include "bf2mods/stuff/utils/util.hpp"
#include "nn/oe.h"
#include "version.h"

// Bdat syms & hooks
namespace Bdat {

	static bool versionBufferPrepared = false;
	static char bdatVersionBuffer[16] {};

	GENERATE_SYM_HOOK(getMSText, "_ZN4Bdat9getMSTextEPhi", const char*, std::uint8_t* bdatData, int n) {
		//skylaunch::logger::s_Instance->LogFormat("Bdat::getMSText(bdat: %p, n: %d)", bdatData, n);
		const char* sheetName = Bdat::getSheetName(bdatData);

		if(!strcmp(sheetName, "menu_ms")) {
			// I don't like hardcoding these but they don't seem to change between versions
			// it's unfortunately nicer (imo) than doing a string comparison
			if(n == 1830) {
				//nn::oe::DisplayVersion displayVersion;
				//nn::oe::GetDisplayVersion(&displayVersion);

				// TODO: probably use fmt
				if(!versionBufferPrepared) {
					std::snprintf(&bdatVersionBuffer[0], sizeof(bdatVersionBuffer) - 1, "(%s)", bf2mods::version::tagDirty);
					versionBufferPrepared = true;
				}

				return &bdatVersionBuffer[0];
			} else if(n == 1610) {
				// it says "Loading" in the japanese version too so I'm not allowed to moan about hardcoding this
				return "Loading (modded)";
			}
		}

		switch(bf2mods::BdatRandomizer::scrambleType) {
			using enum bf2mods::BdatRandomizer::BdatScrambleType;

			case ScrambleIndex:
				// scrambles the index of the ms text sheet
				return Bdat::getMSTextBak(bdatData, (util::nnRand<int16_t>() % Bdat::getIdCount(bdatData)) + Bdat::getIdTop(bdatData));
			case ShowSheetName:
				return sheetName;
			case Off:
			default:
				return Bdat::getMSTextBak(bdatData, n);
		}
	}

	// todo more stuff to randomize more things
} // namespace Bdat

namespace bf2mods {

	BdatRandomizer::BdatScrambleType BdatRandomizer::scrambleType = BdatRandomizer::BdatScrambleType::Off;

	void BdatRandomizer::Initialize() {
		g_Logger->LogDebug("Setting up BDAT randomizer...");

		// Hook stuff
		Bdat::getMSTextHook();
	}

	void BdatRandomizer::Update() {
		if(btnDown(Keybind::BDAT_SCRAMBLETYPE_TOGGLE, p2Cur.Buttons, p2Prev.Buttons)) {
			underlying_value(scrambleType) += 1;

			if(scrambleType >= BdatScrambleType::Count)
				scrambleType = BdatScrambleType::Off;

			g_Logger->LogInfo("Bdat scramble type set to {}", scrambleType);
		}
	}

	BF2MODS_REGISTER_MODULE(BdatRandomizer);

} // namespace bf2mods
