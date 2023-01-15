#pragma once

#include <bf2mods/engine/mm/math_types.hpp>
#include <bf2mods/UpdatableModule.hpp>
#include <nn/hid.hpp>

#include "state.hpp"

namespace bf2mods {

	enum class Keybind : std::uint64_t {
		CLEAR_TCPLOG = nn::hid::KEY_PLUS,
		BDAT_SCRAMBLETYPE_TOGGLE = nn::hid::KEY_X,

		MOVEMENT_SPEED_UP = nn::hid::KEY_DUP,
		MOVEMENT_SPEED_DOWN = nn::hid::KEY_DDOWN,

		TEST_BUTTON = nn::hid::KEY_MINUS | nn::hid::KEY_A,
		TEMPINT_INC = nn::hid::KEY_DRIGHT | nn::hid::KEY_ZL,
		TEMPINT_DEC = nn::hid::KEY_DLEFT | nn::hid::KEY_ZL,

		MAPJUMP_JUMP = nn::hid::KEY_DDOWN | nn::hid::KEY_ZL,
		PLAYSE = nn::hid::KEY_MINUS | nn::hid::KEY_ZL,

		MOONJUMP = nn::hid::KEY_R | nn::hid::KEY_B,
		DISABLE_FALL_DAMAGE = nn::hid::KEY_A,

		RETURN_TO_TITLE = nn::hid::KEY_L | nn::hid::KEY_R | nn::hid::KEY_ZL | nn::hid::KEY_ZR,

		UI_RENDER_TOGGLE = nn::hid::KEY_Y,
		DEBUG_RENDER_TOGGLE = nn::hid::KEY_B,

		FREECAM_TOGGLE = nn::hid::KEY_LSTICK | nn::hid::KEY_RSTICK,
		FREECAM_FOVHOLD = nn::hid::KEY_LSTICK,
		FREECAM_ROLLHOLD = nn::hid::KEY_RSTICK,
		FREECAM_SPEED_UP = nn::hid::KEY_ZL,
		FREECAM_SPEED_DOWN = nn::hid::KEY_L,

		LOGGER_TEST = nn::hid::KEY_PLUS | nn::hid::KEY_R,
	};

	struct HidInput {
		std::uint64_t Buttons {};
		glm::vec2 LAxis;
		glm::vec2 RAxis;
	};

	extern HidInput p1Cur;
	extern HidInput p1Prev;
	extern HidInput p2Cur;
	extern HidInput p2Prev;

	// TODO: make these members of hidinput, and probably store current/previous there (removing the need for "prev" instances).

	inline static bool btnHeld(Keybind combo, std::uint64_t buttons) {
		return (buttons & underlying_value(combo)) == underlying_value(combo);
	}

	inline static bool btnUp(Keybind combo, std::uint64_t curButtons, std::uint64_t prevButtons) {
		return (curButtons & underlying_value(combo)) == underlying_value(combo) && (prevButtons & underlying_value(combo)) != underlying_value(combo);
	}

	inline static bool btnDown(Keybind combo, std::uint64_t curButtons, std::uint64_t prevButtons) {
		return (curButtons & underlying_value(combo)) != underlying_value(combo) && (prevButtons & underlying_value(combo)) == underlying_value(combo);
	}

	/**
 	 * Called on each Framework update.
 	 */
	void update();

	/**
 	 * Called from skylaunch when ready.
 	 */
	void main();

} // namespace bf2mods
