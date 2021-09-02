#include "../Header/Cheat Functions/FiberMain.h"

const float Cheat::GUI::guiX_Default				= 0.11f;
const float Cheat::GUI::guiY_Default				= 0.30f;
const float Cheat::GUI::guiWidth_Default			= 0.21f;
const float Cheat::GUI::SelectableInfoBoxX_Default	= 0.50f;
const float Cheat::GUI::SelectableInfoBoxY_Default	= 0.840f;
float Cheat::GUI::guiX					= 0.11f;
float Cheat::GUI::guiY					= 0.30f;
float Cheat::GUI::guiWidth				= 0.21f; //Do not use (scalling not properly implemented yet)
float Cheat::GUI::SelectableInfoBoxX	= 0.50f;
float Cheat::GUI::SelectableInfoBoxY	= 0.840f;
float Cheat::GUI::SelectableHeight		= 0.040f;
bool Cheat::GUI::ControlsDisabled		= false; //All cheat input is ignored when True
bool Cheat::GUI::selectPressed			= false;
bool Cheat::GUI::leftPressed			= false;
bool Cheat::GUI::rightPressed			= false;
bool Cheat::GUI::ShowHeaderBackground	= true;
bool Cheat::GUI::ShowHeaderTexture		= true;
bool Cheat::GUI::HideGUIElements		= false; //Prevents all GUI elements from being visible when True
bool Cheat::GUI::CheatGUIHasBeenOpened	= false;
bool Cheat::GUI::CurrentOptionIsSavable	= false;
bool Cheat::GUI::RestorePreviousSubmenu = true;
std::string SelectableInformationText;
std::string Cheat::GUI::CurrentTheme;
int Cheat::GUI::maxVisOptions			= 10;
int Cheat::GUI::currentOption			= 0;
int Cheat::GUI::currentOptionVisible    = 0;	//This has GUI::Break excluded
int Cheat::GUI::previousOption			= 0;
int Cheat::GUI::optionCount				= 0;
int Cheat::GUI::optionCountVisible		= 0;	//This has GUI::Break excluded
int Cheat::GUI::TotalOptionsCount		= 0;
int Cheat::GUI::menuLevel				= 0;
SubMenus Cheat::GUI::PreviousMenu		= NOMENU;
SubMenus Cheat::GUI::currentMenu		= NOMENU;
int Cheat::GUI::PreviousMenuLevel		= 0;
int Cheat::GUI::optionsArray			[1000];
SubMenus Cheat::GUI::menusArray			[1000];
std::vector <std::string> Cheat::GUI::ThemeFilesVector;
                 
RGBA Cheat::GUI::PrimaryColor               { 0, 0, 255, 255 };
RGBAF Cheat::GUI::TextColorAndFont			{ 255, 255, 255, 255, FontChaletLondon };

int Cheat::GUI::GUIKeyPressDelay			= 150;
int Cheat::GUI::KeyPressPreviousTick		= GetTickCount64();
int Cheat::GUI::OpenGUIKey					= VK_F4;
int Cheat::GUI::GUINavigationKey			= VK_F5;
int Cheat::GUI::SaveSelectableKey			= VK_F12;

int Cheat::GUI::MenuArrowAnimationDelay		= 1000;


void Cheat::GUI::Title(std::string TitleName)
{
	if (ShowHeaderBackground) { DrawRectInGame({ PrimaryColor.r, PrimaryColor.g, PrimaryColor.b, 200 }, { guiX, GUI::guiY - SelectableHeight - 0.181f }, { guiWidth, SelectableHeight + 0.045f }); }
	if (ShowHeaderTexture) { DrawSpriterInGame("Textures", "HeaderDefaultTransparent", guiX, GUI::guiY - SelectableHeight - 0.181f, guiWidth, SelectableHeight + 0.045f, 0, 255, 255, 255, 255); }
	DrawTextInGame(TitleName, TextColorAndFont, { GUI::guiX, GUI::guiY - 0.174f }, { 0.40f, 0.38f }, true, true);
	DrawRectInGame({ 0, 0, 0, 210 }, { guiX, GUI::guiY - 0.1585f }, { guiWidth, SelectableHeight });

	if (CheatFeatures::CursorGUINavigationEnabled)
	{
		std::string CursorBackCloseString;
		if (GUI::currentMenu == MainMenu) { CursorBackCloseString = "Close"; }
		else { CursorBackCloseString = "Back"; }
		DrawSpriterInGame("commonmenu", "arrowleft", GUI::guiX - 0.100f, GUI::guiY - 0.160f, 0.015f, 0.025f, 0, 255, 255, 255, 255);
		DrawTextInGame(CursorBackCloseString, TextColorAndFont, { GUI::guiX - 0.094f, GUI::guiY - 0.174f }, { 0.35f, 0.37f }, false, true);
	}

	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(false);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_CHARACTER_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MULTIPLAYER_INFO, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_CIN_CAM, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MAP_POI, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_RADIO_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_HEADLIGHT, true);
}

bool Cheat::GUI::Option(std::string option, std::string InformationText, int BitFlags)
{
	GUI::optionCount++;
	GUI::optionCountVisible++;
	bool OnCurrent = GUI::currentOption == GUI::optionCount ? true : false;
	VECTOR2 TextPosition, RectPosition;
	RGBA RectColor;

	if(BitFlags & SELECTABLE_DISABLED) { option.append(" (Disabled)"); }
	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		TextPosition = { Cheat::GUI::guiX - 0.100f, GUI::guiY + (GUI::optionCount) * SelectableHeight - 0.174f };
		RectPosition = { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount) * SelectableHeight - 0.1585f };
		OnCurrent ? RectColor = PrimaryColor : RectColor = { 0, 0, 0, 150 };
		DrawTextInGame(option, TextColorAndFont, TextPosition, { 0.35f, 0.35f }, false);
		DrawRectInGame(RectColor, RectPosition, { Cheat::GUI::guiWidth, SelectableHeight });
	}
	else if (GUI::optionCount > (GUI::currentOption - GUI::maxVisOptions) && GUI::optionCount <= GUI::currentOption)
	{
		TextPosition = { Cheat::GUI::guiX - 0.100f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f };
		RectPosition = { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.1585f };
		OnCurrent ? RectColor = PrimaryColor : RectColor = { 0, 0, 0, 150 };
		DrawTextInGame(option, TextColorAndFont, TextPosition, {0.35f, 0.35f}, false);
		DrawRectInGame(RectColor, RectPosition, { Cheat::GUI::guiWidth, SelectableHeight });
	}
	if (OnCurrent)
	{
		//Selectable Information Box
		if (!CheatFeatures::HideSelectableInformationBox)
		{
			DrawRectInGame({ GUI::PrimaryColor.r, GUI::PrimaryColor.g, GUI::PrimaryColor.b, 210 }, { SelectableInfoBoxX, SelectableInfoBoxY + 0.042f }, { 0.25f, 0.005f });
			DrawRectInGame({ 0, 0, 0, 210 }, { SelectableInfoBoxX, SelectableInfoBoxY }, { 0.25f, 0.080f });
			DrawTextInGame(SelectableInformationText != "" ? CheatFunctions::TextWrap(SelectableInformationText, 30) : option, TextColorAndFont, { SelectableInfoBoxX - 0.12f, SelectableInfoBoxY - 0.033f }, { 0.30f, 0.30f }, false);
			if (GUI::CurrentOptionIsSavable) { DrawTextInGame("Save Option: " + Cheat::CheatFunctions::VirtualKeyCodeToString(Cheat::GUI::SaveSelectableKey), TextColorAndFont, { SelectableInfoBoxX + 0.04f, SelectableInfoBoxY - 0.033f }, { 0.30f, 0.30f }, false); }
		}
		
		GUI::currentOptionVisible = GUI::optionCount - (GUI::optionCount - GUI::optionCountVisible);
		GUI::CurrentOptionIsSavable = false;
		GUI::previousOption = GUI::currentOption;
		if (InformationText == "") { SelectableInformationText.clear(); }
		else { SelectableInformationText = InformationText; }
		if (GUI::selectPressed)
		{
			DoSelectAction:
			if (GUI::SelectableHandler(BitFlags & SELECTABLE_DISABLED ? true : false))
			{
				return true;
			}
		}
	}

	if (GameFunctions::IsCursorAtXYPosition(RectPosition, { Cheat::GUI::guiWidth, SelectableHeight }) && CheatFeatures::CursorGUINavigationEnabled)
	{
		UI::_SET_CURSOR_SPRITE(PreGrab);
		if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_CURSOR_ACCEPT) && !(BitFlags & SELECTABLE_DISABLED))
		{
			goto DoSelectAction;
		}
	}
	return false;
}

bool Cheat::GUI::VehicleOption(std::string option, std::string ModelName)
{
	bool OnCurrent = GUI::currentOption == GUI::optionCount ? true : false;
	if (Option(option, ""))
	{
		return true;
	}
	if (OnCurrent)
	{
		std::string VehiclePreviewDictName, VehiclePreviewName;
		std::transform(ModelName.begin(), ModelName.end(), ModelName.begin(), tolower);
		VehiclePreviewDictName = "Textures";
		VehiclePreviewName	   = "NoVehiclePreviewAvailable";

		for (int i = 0; i < GameArrays::VehicleModelPictures.size(); i++)
		{
			if (GameArrays::VehicleModelPictures[i].PreviewName == ModelName)
			{
				VehiclePreviewDictName = GameArrays::VehicleModelPictures[i].DictName;
				VehiclePreviewName = GameArrays::VehicleModelPictures[i].PreviewName;
			}
		}
		
		if (!CheatFeatures::HideVehicleInfoAndPreview)
		{
			std::ostringstream ModelMaxSpeed;
			if (CheatFeatures::MeasurementSystemVectorPosition == 0)
			{
				ModelMaxSpeed << "Max Speed: " << Cheat::GameFunctions::MSToKMH(VEHICLE::GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(GAMEPLAY::GET_HASH_KEY(CheatFunctions::StringToChar(ModelName)))) << " KM/H";
			}
			else if (CheatFeatures::MeasurementSystemVectorPosition == 1)
			{
				ModelMaxSpeed << "Max Speed: " << Cheat::GameFunctions::MSToMPH(VEHICLE::GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(GAMEPLAY::GET_HASH_KEY(CheatFunctions::StringToChar(ModelName)))) << " MP/H";
			}

			if (guiX < 0.71f)
			{
				DrawTextInGame("Model Name: " + ModelName, TextColorAndFont, { guiX + 0.111f,  guiY - 0.004f }, { 0.45f, 0.30f }, false);
				DrawTextInGame(ModelMaxSpeed.str(), TextColorAndFont, { guiX + 0.111f, guiY + 0.010f }, { 0.45f, 0.30f }, false);
				DrawTextInGame("Seats: " + std::to_string(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(GAMEPLAY::GET_HASH_KEY(CheatFunctions::StringToChar(ModelName)))), TextColorAndFont, { guiX + 0.111f, guiY + 0.025f}, {0.45f, 0.30f}, false);
				DrawRectInGame({ 0, 0, 0, 150 }, { guiX + 0.187f, GUI::guiY - 0.061f }, { 0.16f, 0.22f });
				DrawSpriterInGame(VehiclePreviewDictName, VehiclePreviewName, guiX + 0.187f, guiY - 0.085f, 0.15f, 0.15f, 0.f, 255, 255, 255, 255);
			}
			else
			{
				DrawTextInGame("Model Name: " + ModelName, TextColorAndFont, { guiX - 0.262f, guiY - 0.004f }, { 0.45f, 0.30f }, false);
				DrawTextInGame(ModelMaxSpeed.str(), TextColorAndFont, { guiX - 0.262f, guiY + 0.010f }, { 0.45f, 0.30f }, false);
				DrawTextInGame("Seats: " + std::to_string(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(GAMEPLAY::GET_HASH_KEY(CheatFunctions::StringToChar(ModelName)))), TextColorAndFont, { guiX - 0.262f, guiY + 0.025f }, { 0.45f, 0.30f }, false);
				DrawRectInGame({ 0, 0, 0, 150 }, { guiX - 0.187f, guiY - 0.061f }, { 0.16f, 0.22f });
				DrawSpriterInGame(VehiclePreviewDictName, VehiclePreviewName, guiX - 0.187f, guiY - 0.085f, 0.15f, 0.15f, 0.f, 255, 255, 255, 255);
			}
		}
	}
	return false;
}

bool Cheat::GUI::Break(std::string option, bool TextCentered)
{
	GUI::optionCount++;

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		if (TextCentered) { DrawTextInGame("[ ~c~" + option + " ~s~]", { PrimaryColor.r, PrimaryColor.g, PrimaryColor.b, PrimaryColor.a, TextColorAndFont.f }, {Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount) * SelectableHeight - 0.174f}, {0.35f, 0.35f}, TextCentered, true); }
		else { DrawTextInGame(option, TextColorAndFont, { Cheat::GUI::guiX - 0.100f, GUI::guiY + (GUI::optionCount)*SelectableHeight - 0.174f }, { 0.35f, 0.35f }, TextCentered); }
		DrawRectInGame({ 0, 0, 0, 150 }, { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount) * SelectableHeight - 0.1585f }, { Cheat::GUI::guiWidth, SelectableHeight });
	}
	else if (GUI::optionCount > (GUI::currentOption - GUI::maxVisOptions) && GUI::optionCount <= GUI::currentOption)
	{
		if (TextCentered) { DrawTextInGame("[ ~c~" + option + " ~s~]", { PrimaryColor.r, PrimaryColor.g, PrimaryColor.b, PrimaryColor.a, TextColorAndFont.f }, { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.35f, 0.35f }, TextCentered, true); }
		else { DrawTextInGame(option, TextColorAndFont, { Cheat::GUI::guiX - 0.100f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.35f, 0.35f }, TextCentered); }
		DrawRectInGame({ 0, 0, 0, 150 }, { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions))*SelectableHeight - 0.1585f }, { Cheat::GUI::guiWidth, SelectableHeight });
	}

	if (GUI::optionCount == GUI::currentOption)
	{
		GUI::CurrentOptionIsSavable = false;
		if (GUI::previousOption < GUI::currentOption && GUI::optionCount > 1)
		{
			GUI::currentOption++;
			if (GUI::TotalOptionsCount < GUI::currentOption)
			{
				GUI::currentOption = 1;
			}
		}
		else if (GUI::previousOption > GUI::currentOption && GUI::optionCount > 1)
		{
			GUI::currentOption--;
			if (GUI::TotalOptionsCount < 1)
			{
				GUI::currentOption = GUI::optionCount;
			}
		}
		else if (GUI::currentOption == 1 && GUI::TotalOptionsCount > 1)
		{
			GUI::currentOption++;
		}
	}
	return false;
}

bool Cheat::GUI::MenuOptionArrowAnimationState = false;
bool Cheat::GUI::MenuOption(std::string option, SubMenus newSub, int BitFlags)
{
	if (Option(option, "", BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		GUI::MoveMenu(newSub);
		return true;
	}
	
	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		DrawTextInGame(MenuOptionArrowAnimationState ? ">" : " >", TextColorAndFont, {Cheat::GUI::guiX + 0.09f, GUI::guiY + (GUI::optionCount) * SelectableHeight - 0.174f}, {0.35f, 0.35f}, false);
	}
	else if (GUI::optionCount > (GUI::currentOption - GUI::maxVisOptions) && GUI::optionCount <= GUI::currentOption)
	{
		DrawTextInGame(MenuOptionArrowAnimationState ? ">" : " >", TextColorAndFont, { Cheat::GUI::guiX + 0.09f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.35f, 0.35f }, false);
	}
	return false;
}

bool Cheat::GUI::MenuOptionPlayerList(std::string PlayerName)
{
	if (Option("      " + PlayerName, ""))
	{
		GUI::MoveMenu(SelectedPlayerMenu);
		return true;
	}

	char* CurrentOnlinePlayerPictureName = Cheat::GameFunctions::ReturnOnlinePlayerPictureString(GameFunctions::ReturnPlayerIDFromPlayerName(PlayerName));

	VECTOR2 Position;
	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		Position = { Cheat::GUI::guiX - 0.093f, GUI::guiY + (GUI::optionCount * SelectableHeight - 0.158f) };
	}
	else if (GUI::optionCount > GUI::currentOption - GUI::maxVisOptions && GUI::optionCount <= GUI::currentOption)
	{
		Position = { Cheat::GUI::guiX - 0.093f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.158f };
	}
	DrawSpriterInGame(CurrentOnlinePlayerPictureName, CurrentOnlinePlayerPictureName, Position.x, Position.y, 0.02f, 0.03f, 0, 255, 255, 255, 255);
	return false;
}

bool Cheat::GUI::Toggle(std::string option, bool & TargetBool, std::string InformationText, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, TargetBool); }

	if (Option(option, InformationText, BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		TargetBool ^= 1;
		return true;
	}

	std::string ToggleString;
	TargetBool ? ToggleString = "ToggleOn" : ToggleString = "ToggleOff";

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		DrawSpriterInGame("Textures", ToggleString, Cheat::GUI::guiX + 0.090f, GUI::guiY + (GUI::optionCount * SelectableHeight - 0.159f), 0.025f, 0.025f, 0, 255, 255, 255, 255);
	}
	else if ((GUI::optionCount > (GUI::currentOption - GUI::maxVisOptions)) && GUI::optionCount <= GUI::currentOption)
	{
		DrawSpriterInGame("Textures", ToggleString, Cheat::GUI::guiX + 0.090f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.159f, 0.025f, 0.025f, 0, 255, 255, 255, 255);
	}

	if (GUI::optionCount == GUI::currentOption)
	{
		CheatFunctions::SaveOption(option, TargetBool ? "true" : "false", !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
	}
	return false;
}

bool Cheat::GUI::Int(std::string option, int & _int, int min, int max, int step, std::string InformationText, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, _int); }

	if (Option(option, InformationText, BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		int KeyBoardInput = Cheat::GameFunctions::DisplayKeyboardAndReturnInputInteger(CheatFunctions::ReturnNumberOfDigitsInValue(max), "Enter number");
		if (KeyBoardInput >= min && KeyBoardInput <= max)
		{
			_int = KeyBoardInput;
		}
		return true;
	}

	std::string Text;
	if (_int > min)
	{
		Text.append("< ");
	}
	Text.append(std::to_string(_int));
	if (_int < max)
	{
		Text.append(" >");
	}

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		if (_int < 100)
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.075f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
		else if (_int < 999)
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.07f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
		else
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.06f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
	}
	else if ((GUI::optionCount > (GUI::currentOption - GUI::maxVisOptions)) && GUI::optionCount <= GUI::currentOption)
	{
		if (_int < 100)
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.075f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
		else if (_int < 999)
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.07f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
		else
		{
			DrawTextInGame(Text, TextColorAndFont, { Cheat::GUI::guiX + 0.06f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.32f, 0.32f }, false);
		}
	}

	if (GUI::optionCount == GUI::currentOption)
	{
		if (GUI::leftPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			if (_int < max)
			{
				if (step == 1) { _int++; }
				else { _int = _int + step; }
			}
			return true;
		}
		if (GUI::rightPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			if (_int > min)
			{
				if (step == 1) { _int--; }
				else { _int = _int - step; }
			}
			return true;
		}

		CheatFunctions::SaveOption(option, std::to_string(_int), !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
	}
	return false;
}

bool Cheat::GUI::Float(std::string option, float & _float, float min, float max, float steps, std::string InformationText, std::streamsize FloatPrecision, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, _float); }

	if (Option(option, InformationText, BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		return true;
	}

	bool IgnoreMinMax = min == 0.0f && max == 0.0f;
	if (GUI::optionCount == GUI::currentOption) 
	{	
		CheatFunctions::SaveOption(option, std::to_string(_float), !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
		if (GUI::leftPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			if (_float < max || IgnoreMinMax)
			{
				_float += steps;
			}
			if (BitFlags & SELECTABLE_RETURN_VALUE_CHANGE) { return true;  }
		}
		if (GUI::rightPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			if (_float > min || IgnoreMinMax)
			{
				_float -= steps;
			}
			if (BitFlags & SELECTABLE_RETURN_VALUE_CHANGE) { return true; }
		}
	}

	std::ostringstream DisplayFloat;
	DisplayFloat.precision(FloatPrecision);
	DisplayFloat << std::fixed << _float;
	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		DrawTextInGame("< " + DisplayFloat.str() + " >", TextColorAndFont, {Cheat::GUI::guiX + 0.08f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.172f}, {0.32f, 0.32f}, true);
	}
	else if (GUI::optionCount > GUI::currentOption - GUI::maxVisOptions && GUI::optionCount <= GUI::currentOption)
	{
		DrawTextInGame("< " + DisplayFloat.str() + " >", TextColorAndFont, { Cheat::GUI::guiX + 0.08f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.172f }, { 0.32f, 0.32f }, true);
	}
	return false;
}

bool Cheat::GUI::IntVector(std::string option, std::vector<int> Vector, int& position, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, position); }

	if (Option(option, "", BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		return true;
	}

	if (GUI::optionCount == GUI::currentOption) 
	{
		int max = static_cast<int>(Vector.size()) - 1;
		int min = 0;
		if (GUI::leftPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			position >= 1 ? position-- : position = max;
			return true;
		}
		if (GUI::rightPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			position < max ? position++ : position = min;
			return true;
		}
		CheatFunctions::SaveOption(option, std::to_string(position), !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
	}

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		DrawTextInGame(std::to_string(Vector[position]), TextColorAndFont, { Cheat::GUI::guiX + 0.068f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.5f, 0.5f }, true);
	}
	else if (GUI::optionCount > GUI::currentOption - GUI::maxVisOptions && GUI::optionCount <= GUI::currentOption)
	{
		DrawTextInGame(std::to_string(Vector[position]), TextColorAndFont, { Cheat::GUI::guiX + 0.068f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.5f, 0.5f }, true);
	}
	return false;
}

bool Cheat::GUI::FloatVector(std::string option, std::vector<float> Vector, int& position, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, position); }

	if (Option(option, "", BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		return true;
	}

	if (GUI::optionCount == GUI::currentOption) 
	{
		CheatFunctions::SaveOption(option, std::to_string(position), !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
		size_t max = static_cast<int>(Vector.size()) - 1;
		int min = 0;
		if (GUI::leftPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			position >= 1 ? position-- : position = static_cast<int>(max);
			return true;
		}
		if (GUI::rightPressed && !(BitFlags & SELECTABLE_DISABLED))
		{
			position < max ? position++ : position = static_cast<int>(min);
			return true;
		}
	}

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions)
	{
		DrawTextInGame(std::to_string(Vector[position]), TextColorAndFont, { Cheat::GUI::guiX + 0.068f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.5f, 0.5f }, true);
	}
	else if (GUI::optionCount > GUI::currentOption - GUI::maxVisOptions && GUI::optionCount <= GUI::currentOption)
	{
		DrawTextInGame(std::to_string(Vector[position]), TextColorAndFont, { Cheat::GUI::guiX + 0.068f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.5f, 0.5f }, true);
	}
	return false;
}

bool Cheat::GUI::StringVector(std::string option, std::vector<std::string> Vector, int & position, std::string InformationText, int BitFlags)
{
	if (!(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED)) { CheatFunctions::LoadConfigOption(option, position); }

	if (Option(option, InformationText, BitFlags & SELECTABLE_DISABLED ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		return true;
	}

	if (GUI::optionCount == GUI::currentOption) 
	{
		CheatFunctions::SaveOption(option, std::to_string(position), !(BitFlags & SELECTABLE_DISABLE_SAVE) && !(BitFlags & SELECTABLE_DISABLED) ? true : false);
		size_t max = static_cast<int>(Vector.size()) - 1;
		int min = 0;
		if (GUI::leftPressed)
		{
			position >= 1 ? position-- : position = static_cast<int>(max);
			if (BitFlags & SELECTABLE_RETURN_VALUE_CHANGE) { return true; }
		}
		if (GUI::rightPressed) 
		{
			position < max ? position++ : position = static_cast<int>(min);
			if (BitFlags & SELECTABLE_RETURN_VALUE_CHANGE) { return true; }
		}
	}

	if (GUI::currentOption <= GUI::maxVisOptions && GUI::optionCount <= GUI::maxVisOptions) 
	{
		DrawTextInGame("< " + (Vector[position]) + " >", TextColorAndFont, { Cheat::GUI::guiX + 0.055f, GUI::guiY + GUI::optionCount * SelectableHeight - 0.174f }, { 0.35f, 0.35f }, true);
	}
	else if (GUI::optionCount > GUI::currentOption - GUI::maxVisOptions && GUI::optionCount <= GUI::currentOption)
	{
		DrawTextInGame("< " + (Vector[position]) + " >", TextColorAndFont, { Cheat::GUI::guiX + 0.055f, GUI::guiY + (GUI::optionCount - (GUI::currentOption - GUI::maxVisOptions)) * SelectableHeight - 0.174f }, { 0.35f, 0.35f }, true);
	}
	return false;
}

void Cheat::GUI::End()
{
	GUI::TotalOptionsCount = GUI::optionCount;
	if (GUI::optionCount >= GUI::maxVisOptions)
	{
		DrawTextInGame(std::to_string(GUI::currentOptionVisible) + "/" + std::to_string(GUI::optionCountVisible), TextColorAndFont, { Cheat::GUI::guiX - 0.088f, GUI::guiY + ((GUI::maxVisOptions + 1) * SelectableHeight - 0.172f) }, { 0.30f, 0.30f }, true);
		DrawTextInGame(CHEAT_BUILD_NUMBER, TextColorAndFont, { Cheat::GUI::guiX + 0.085f, GUI::guiY + ((GUI::maxVisOptions + 1) * SelectableHeight - 0.172f) }, { 0.30f, 0.30f }, true);
		DrawRectInGame({ 0, 0, 0, 210 }, { Cheat::GUI::guiX, GUI::guiY + ((GUI::maxVisOptions + 1) * SelectableHeight - 0.1585f) }, { Cheat::GUI::guiWidth, SelectableHeight });
		DrawSpriterInGame("Textures", "LogoSmall", Cheat::GUI::guiX, GUI::guiY + ((GUI::maxVisOptions + 1) * SelectableHeight - 0.159f), 0.030f, 0.045f, 0, GUI::PrimaryColor.r, GUI::PrimaryColor.g, GUI::PrimaryColor.b, 255);
	}
	else if (GUI::optionCount > 0)
	{
		DrawTextInGame(std::to_string(GUI::currentOptionVisible) + "/" + std::to_string(GUI::optionCountVisible), TextColorAndFont, { Cheat::GUI::guiX - 0.088f, GUI::guiY + (GUI::optionCount + 1) * SelectableHeight - 0.172f }, { 0.30f, 0.30f }, true);
		DrawTextInGame(CHEAT_BUILD_NUMBER, TextColorAndFont, { Cheat::GUI::guiX + 0.085f, GUI::guiY + (GUI::optionCount + 1) * SelectableHeight - 0.172f }, { 0.30f, 0.30f }, true);
		DrawRectInGame({ 0, 0, 0, 210 }, { Cheat::GUI::guiX, GUI::guiY + (GUI::optionCount + 1) * SelectableHeight - 0.1585f }, { Cheat::GUI::guiWidth, SelectableHeight });
		DrawSpriterInGame("Textures", "LogoSmall", Cheat::GUI::guiX, GUI::guiY + ((GUI::optionCount + 1) * SelectableHeight - 0.159f), 0.030f, 0.045f, 0, GUI::PrimaryColor.r, GUI::PrimaryColor.g, GUI::PrimaryColor.b, 255);
	}
}

void Cheat::GUI::ControlsLoop()
{
	if (CheatFunctions::IsKeyCurrentlyPressed(GUI::GUINavigationKey, true)) { GameFunctions::EnableDisableCursorGUINavigation(); }
	if (!ControlsDisabled)
	{
		GUI::selectPressed = false;
		GUI::leftPressed = false;
		GUI::rightPressed = false;

		if (GetTickCount64() - GUI::KeyPressPreviousTick > GUI::GUIKeyPressDelay)
		{
			if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(GUI::OpenGUIKey))
			{
				Cheat::GUI::CheatGUIHasBeenOpened = true;
				if (GUI::menuLevel == 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("YES");
					if (GUI::PreviousMenu != NOMENU && Cheat::GUI::RestorePreviousSubmenu)
					{
						GUI::MoveMenu(GUI::PreviousMenu);
						GUI::menuLevel = GUI::PreviousMenuLevel;
						GUI::currentOption = GUI::previousOption;
					}
					else
					{
						GUI::MoveMenu(SubMenus::MainMenu);
					}
				}
				else
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("NO");
					GUI::CloseMenuGUI();
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD0))
			{
				if (GUI::menuLevel > 0) 
				{
					GUI::BackMenu();
					Cheat::GameFunctions::PlayFrontendSoundDefault("BACK"); 
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD8))
			{
				GUI::currentOption > 1 ? GUI::currentOption-- : GUI::currentOption = GUI::optionCount;
				if (GUI::menuLevel > 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("NAV_UP_DOWN");
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD2))
			{
				GUI::currentOption < GUI::optionCount ? GUI::currentOption++ : GUI::currentOption = 1;
				if (GUI::menuLevel > 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("NAV_UP_DOWN");
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD6))
			{
				GUI::leftPressed = true;
				if (GUI::menuLevel > 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("NAV_UP_DOWN");
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD4))
			{
				GUI::rightPressed = true;
				if (GUI::menuLevel > 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("NAV_UP_DOWN");
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
			else if (Cheat::CheatFunctions::IsKeyCurrentlyPressed(VK_NUMPAD5))
			{
				GUI::selectPressed = true;
				if (GUI::menuLevel > 0)
				{
					Cheat::GameFunctions::PlayFrontendSoundDefault("SELECT");
				}
				GUI::KeyPressPreviousTick = GetTickCount64();
			}
		}
		GUI::optionCount = 0;
		GUI::optionCountVisible = 0;
	}
}
void Cheat::GUI::MoveMenu(SubMenus menu)
{
	GUI::menusArray[GUI::menuLevel] = GUI::currentMenu;
	GUI::optionsArray[GUI::menuLevel] = GUI::currentOption;
	GUI::menuLevel++;
	GUI::currentMenu = menu;
	GUI::currentOption = 1;
}

void Cheat::GUI::CloseMenuGUI()
{
	if (Cheat::CheatFeatures::CursorGUINavigationEnabled) { Cheat::GameFunctions::EnableDisableCursorGUINavigation(); }
	GUI::PreviousMenu = Cheat::GUI::currentMenu;
	GUI::PreviousMenuLevel = GUI::menuLevel;
	GUI::previousOption = GUI::currentOption;
	GUI::menuLevel = 0;
	GUI::currentMenu = GUI::menusArray[GUI::menuLevel];
	GUI::currentOption = GUI::optionsArray[GUI::menuLevel];
}

void Cheat::GUI::BackMenu()
{
	if (GUI::currentMenu == MainMenu) { CloseMenuGUI(); return; }
	GUI::PreviousMenu = GUI::currentMenu;
	GUI::PreviousMenuLevel = GUI::menuLevel;
	GUI::previousOption = GUI::currentOption;
	GUI::menuLevel--;
	GUI::currentMenu = GUI::menusArray[GUI::menuLevel];
	GUI::currentOption = GUI::optionsArray[GUI::menuLevel];
}

void Cheat::GUI::AddPlayerInfoBoxTextEntry(std::string text, int Row1, int Row2, int Row3, int Row4)
{
	if (Cheat::GUI::guiX < 0.54f)
	{		
		if (Row1 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX + 0.110f, GUI::guiY + (Row1 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row2 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX + 0.205f, GUI::guiY + (Row2 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row3 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX + 0.295f, GUI::guiY + (Row3 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row4 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX + 0.365f, GUI::guiY + (Row4 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
	}
	else
	{
		if (Row1 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX - 0.420f, GUI::guiY + (Row1 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row2 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX - 0.340f, GUI::guiY + (Row2 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row3 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX - 0.260f, GUI::guiY + (Row3 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
		else if (Row4 != NULL)
		{
			DrawTextInGame(text, TextColorAndFont, { Cheat::GUI::guiX - 0.165f, GUI::guiY + (Row4 * 0.020f) - 0.158f }, { 0.30f, 0.30f }, false);
		}
	}
}

void Cheat::GUI::LoadTheme(std::string ThemeFileName, bool StartUp)
{
	if (!CheatFunctions::FileOrDirectoryExists(CheatFunctions::ReturnThemeFilePath(ThemeFileName))) 
	{ 
		GameFunctions::MinimapNotification("Requested Theme does not exist. Auto load entry removed from config file."); 
		CheatFunctions::IniFileRemoveKey(CheatFunctions::ReturnConfigFilePath(), "Settings", "Active Theme");
		return;
	}

	Cheat::GUI::CurrentTheme = ThemeFileName;
	try
	{ 
		float menu_guiX = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_x"));
		float menu_guiY	= std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_y"));
		float selectable_information_boxX = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_x"));
		float selectable_information_boxY = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_y"));
		float selectable_height = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_height"));

		GUI::guiX = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_x"));
		GUI::guiY = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_y"));
		GUI::SelectableInfoBoxX = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_x"));
		GUI::SelectableInfoBoxY = std::stof(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_y"));

		GUI::ShowHeaderTexture = CheatFunctions::StringToBool(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Header", "header_texture"));
		GUI::ShowHeaderBackground = CheatFunctions::StringToBool(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Header", "header_background"));
	
		CheatFeatures::RGBDiscoBool = CheatFunctions::StringToBool(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "rgb_disco"));

		if (!CheatFeatures::RGBDiscoBool)
		{
			Cheat::GUI::PrimaryColor.r = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_r"));
			Cheat::GUI::PrimaryColor.g = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_g"));
			Cheat::GUI::PrimaryColor.b = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_b"));

			Cheat::GUI::TextColorAndFont.r = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_r"));
			Cheat::GUI::TextColorAndFont.g = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_g"));
			Cheat::GUI::TextColorAndFont.b = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_b"));
		}

		Cheat::GUI::TextColorAndFont.f = CheatFunctions::StringToInt(CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Font", "text_font"));
	}
	catch (...) {}

	//Check Theme File Version
	if (Cheat::CheatFunctions::IniFileReturnKeyValueAsString(CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Theme", "theme_file_version") != "2.1")
	{
		Cheat::GameFunctions::SubtitleNotification("The loaded Theme is outdated. Please resave it", 5000);
	}

	//Save New Active Theme Name To Config File
	if (!StartUp) 
	{
		Cheat::CheatFunctions::IniFileWriteString(GUI::CurrentTheme, CheatFunctions::ReturnConfigFilePath(), "Settings", "Active Theme");
		Cheat::GameFunctions::MinimapNotification("Theme Loaded"); 
	}
}

void Cheat::GUI::DeleteLoadedTheme()
{
	remove(CheatFunctions::StringToChar(CheatFunctions::ReturnThemeFilePath(CurrentTheme)));
	CheatFunctions::IniFileRemoveKey(CheatFunctions::ReturnConfigFilePath(), "Settings", "Active Theme");
	GUI::CurrentTheme.clear();
	GUI::currentOption = 1;
	Cheat::GameFunctions::MinimapNotification("Theme Removed");
}

void Cheat::GUI::ChangeControlsState(bool State)
{
	if (State)
	{
		Cheat::LogFunctions::DebugMessage("Enabled Controls");
		ControlsDisabled = false;
	}
	else
	{
		Cheat::LogFunctions::DebugMessage("Disabled Controls");
		ControlsDisabled = true;
	}
}

void Cheat::GUI::SaveTheme(std::string ThemeFileName)
{
	CheatFunctions::IniFileWriteString("2.1", CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Theme", "theme_file_version");

	CheatFunctions::IniFileWriteString(std::to_string(GUI::guiX), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_x");
	CheatFunctions::IniFileWriteString(std::to_string(GUI::guiY), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "menu_gui_y");
	CheatFunctions::IniFileWriteString(std::to_string(GUI::SelectableInfoBoxX), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_x");
	CheatFunctions::IniFileWriteString(std::to_string(GUI::SelectableInfoBoxY), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_information_box_y");
	CheatFunctions::IniFileWriteString(std::to_string(GUI::SelectableHeight), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Positioning", "selectable_height");

	CheatFunctions::WriteBoolToIni(GUI::ShowHeaderTexture, CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Header", "header_texture");
	CheatFunctions::WriteBoolToIni(GUI::ShowHeaderBackground, CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Header", "header_background");

	Cheat::CheatFunctions::WriteBoolToIni(CheatFeatures::RGBDiscoBool, CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "rgb_disco");
	if (!CheatFeatures::RGBDiscoBool)
	{
		CheatFunctions::IniFileWriteString(std::to_string(GUI::PrimaryColor.r), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_r");
		CheatFunctions::IniFileWriteString(std::to_string(GUI::PrimaryColor.g), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_g");
		CheatFunctions::IniFileWriteString(std::to_string(GUI::PrimaryColor.b), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "primary_color_b");

		CheatFunctions::IniFileWriteString(std::to_string(GUI::TextColorAndFont.r), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_r");
		CheatFunctions::IniFileWriteString(std::to_string(GUI::TextColorAndFont.g), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_g");
		CheatFunctions::IniFileWriteString(std::to_string(GUI::TextColorAndFont.b), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Color", "text_color_b");
	}

	CheatFunctions::IniFileWriteString(std::to_string(GUI::TextColorAndFont.f), CheatFunctions::ReturnThemeFilePath(ThemeFileName), "Font", "text_font");

	GameFunctions::MinimapNotification("Theme Saved");
}

bool Cheat::GUI::SelectableHandler(bool DisabledState)
{
	//Handle selectable disable state
	if (DisabledState)
	{
		Cheat::GameFunctions::AdvancedMinimapNotification("This selectable is currently disabled", "Textures", "AdvancedNotificationImage", false, 4, "Cheat", "", 0.5f, "");
		return false;
	}
	return true;
}

static FileRegister RegisterTextureFile = (FileRegister)(Memory::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9").count(1).get(0).get<decltype(RegisterTextureFile)>());
void Cheat::GUI::LoadTextureFile()
{
	Cheat::LogFunctions::Message("Loading Texture File");
	remove(CheatFunctions::StringToChar(Cheat::CheatFunctions::TextureFilePath()));

	//Find and load the resource
	HRSRC hResource = FindResourceA(CheatModuleHandle, MAKEINTRESOURCEA(140), "CHEAT_DATA");
	if (!hResource) { goto Error; }
	HGLOBAL hFileResource = LoadResource(CheatModuleHandle, hResource);
	if (!hFileResource) { goto Error; }

	//Open and map this to a disk file
	LPVOID lpFile = LockResource(hFileResource);
	DWORD dwSize = SizeofResource(CheatModuleHandle, hResource);

	//Open the file and filemap
	HANDLE hFile = CreateFileA(CheatFunctions::StringToChar(CheatFunctions::TextureFilePath()), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwSize, NULL);
	if (!hFileMap) { goto Error; }

	LPVOID lpAddress = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (!lpAddress) { goto Error; }

	//Write the file
	CopyMemory(lpAddress, lpFile, dwSize);

	//Un-map the file and close the handles
	UnmapViewOfFile(lpAddress);
	CloseHandle(hFileMap);
	CloseHandle(hFile);

	int textureID;
	if (CheatFunctions::FileOrDirectoryExists(CheatFunctions::TextureFilePath()))
	{
		RegisterTextureFile(&textureID, CheatFunctions::StringToChar(CheatFunctions::TextureFilePath()), true, "Textures.ytd", false);
		return;
	}
	else
	{
		goto Error;
	}

	//Error
Error:
	Cheat::GameFunctions::MinimapNotification("~r~Failed to load Texture");
	Cheat::LogFunctions::DebugMessage("Failed to load Texture");
}

void Cheat::GUI::DrawTextInGame(std::string text, RGBAF rgbaf, VECTOR2 position, VECTOR2_2 size, bool center, bool Outline)
{
	if (!GUI::HideGUIElements)
	{
		UI::SET_TEXT_CENTRE(center);
		UI::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		UI::SET_TEXT_FONT(rgbaf.f);
		if (Outline) { UI::SET_TEXT_OUTLINE(); }
		UI::SET_TEXT_SCALE(size.w, size.h);
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CheatFunctions::StringToChar(text));
		UI::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y);
	}
}

void Cheat::GUI::DrawSpriterInGame(std::string Streamedtexture, std::string textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GUI::HideGUIElements)
	{
		if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(CheatFunctions::StringToChar(Streamedtexture)))
		{
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(CheatFunctions::StringToChar(Streamedtexture.c_str()), false);
		}
		else
		{
			GRAPHICS::DRAW_SPRITE(CheatFunctions::StringToChar(Streamedtexture), CheatFunctions::StringToChar(textureName), x, y, width, height, rotation, r, g, b, a);
		}
	}
}

void Cheat::GUI::DrawRectInGame(RGBA rgba, VECTOR2 position, VECTOR2_2 size)
{
	if (!GUI::HideGUIElements)
	{
		GRAPHICS::DRAW_RECT(position.x, position.y, size.w, size.h, rgba.r, rgba.g, rgba.b, rgba.a);
	}
}