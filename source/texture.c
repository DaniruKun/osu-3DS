#include <3ds.h>
#include "pp2d/pp2d.h"

char* romfsDir, sdmcDir;
char loadedTextures = {};
char romfsTextures = {};
char sdmcTextures = {};

void loadTextures() {
	if (romfsTextures == {}) {
		printf("Please prepare textures before loading textures!\n");
		return;
	}

	// my brain hurts from coding this
	u32 textureId = 0;
	char textureIdStr[10];
	for (i = 0; i < sizeof(romfsTextures) / sizeof(struct romfsTextures); i++) {
		for (i1 = 0; i1 < sizeof(sdmcTextures) / sizeof(struct sdmcTextures); i1++) {
			if (sdmcTextures[i1][0] == romfsTextures[i][0]) {
				sprintf(textureIdStr, "%d", textureId);
				loadedTextures[textureId] = {sdmcTextures[i1][0], textureIdStr};
				pp2d_load_texture_png(textureId, "sdmc:" + sdmcDir + sdmcTextures[i1][0]);
				textureId++;
				return;
			}
		}

		sprintf(textureIdStr, "%d", textureId);
		loadedTextures[textureId] = {romfsTextures[i][0], textureIdStr};
		pp2d_load_texture_png(textureId, "romfs:" + romfsDir + romfsTextures[i][0]);
		textureId++;
	}
}

void prepareTextures(char* romfsDir, char* sdmcDir) {
	romfsDir = romfsDir;
	sdmcDir = sdmcDir;
    Handle dirHandle;
    FS_DirectoryEntry entry;
    FS_Path dirPath = fsMakePath(PATH_ASCII, romfsDir);
    FS_Archive romfsArchive;
    FSUSER_OpenArchive(&romfsArchive, ARCHIVE_ROMFS, romfsDir);
    FSUSER_OpenDirectory(&dirHandle, romfsArchive, romfsDir);

    u32 entriesRead;
    u32 objectSet = 0;

    for (;;) {
    	entriesRead = 0;
    	FSDIR_Read(dirHandle, &entriesRead, 1, (FS_DirectoryEntry*)&entry);
    	if (entriesRead) {
    		char *fileExt = strrchr(filename, '.');
    		if (fileExt) {
    			if (fileExt + 1 == ".png") romfsTextures[objectSet] = entry.name;
    		}
    		objectSet++;
    	} else break;
    }

    FSDIR_Close(dirHandle);
    svcCloseHandle(dirHandle);
    FSUSER_CloseArchive(romfsArchive);

    if (sdmcDir) {
    	Handle dirHandle;
    	FS_DirectoryEntry entry;
    	FS_Path dirPath = fsMakePath(PATH_ASCII, sdmcDir);
    	FS_Archive sdmcArchive;
    	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, sdmcDir);
    	FSUSER_OpenDirectory(&dirHandle, sdmcArchive, sdmcDir);

    	u32 entriesRead;
    	u32 objectSet = 0;

    	for (;;) {
    		entriesRead = 0;
    		FSDIR_Read(dirHandle, &entriesRead, 1, (FS_DirectoryEntry*)&entry);
    		if (entriesRead) {
    			char *fileExt = strrchr(filename, '.');
    			if (fileExt) {
    				if (fileExt + 1 == ".png") sdmcTextures[objectSet] = entry.name;
    			}
    			objectSet++;
    		} else break;
    	}

    	FSDIR_Close(dirHandle);
    	svcCloseHandle(dirHandle);
    	FSUSER_CloseArchive(sdmcArchive);
    }
}

int getTextureId(char fileName) {
	for (i = 0; i < sizeof(loadedTextures) / sizeof(struct loadedTextures); i++) {
		if (loadedTextures[i][0] == fileName) {
			return atoi(loadedTextures[i][1]);
		}
	}
}