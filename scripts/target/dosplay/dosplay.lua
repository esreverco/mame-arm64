STANDALONE = true

CPUS["I86"] = true

function standalone()
    files{
		MAME_DIR .. "src/dosplay/main.cpp",
		MAME_DIR .. "src/dosplay/dosplay.cpp",
    }
end

