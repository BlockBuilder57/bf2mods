#include <bf2mods/Logger.hpp>
#include <skylaunch/hookng/Hooks.hpp>
#include <bf2mods/stuff/utils/debug_util.hpp>

namespace skylaunch::hook::detail {

	void* HookFunctionBase(void* function, void* replacement) {
		void* backup;

		bf2mods::g_Logger->LogDebug("[hook-ng] Attempting to hook {}...", dbgutil::getSymbol(reinterpret_cast<uintptr_t>(function)));

		A64HookFunction(function, replacement, &backup);
		return backup;
	}

	uintptr_t ResolveSymbolBase(std::string_view symbolName) {
		uintptr_t addr;

		if(Result res; R_SUCCEEDED(res = nn::ro::LookupSymbol(&addr, symbolName.data()))) {
			return addr;
		} else {
			bf2mods::g_Logger->LogError("[hook-ng] Failed to resolve symbol \"{}\"! (Horizon ec {})", symbolName, res);
			return 0xDEADDEAD;
		}
	}

} // namespace skylaunch::hook::detail