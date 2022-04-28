#pragma once

namespace Helpers
{
#ifdef CLION_IDE
	static constexpr char gameDirectoryPath[] = "game/";
	static constexpr char engineDirectoryPath[] = "engine/";
	static constexpr char editorDirectoryPath[] = "editor/";
#else
	static constexpr char gameDirectoryPath[] = "../../game/";
	static constexpr char engineDirectoryPath[] = "../../engine/";
	static constexpr char editorDirectoryPath[] = "../../editor/";
#endif

}
